/** ****************************************************************************
 * @file   debug_print.c
 *
 * @author Michael R.
 *
 * @brief  Functions for debug-prints with debug-level.
 *
 * @date   2023-08-26
 **************************************************************************** */

/* --- Includes ------------------------------------------------------------- */

// libc includes
#include <stdio.h>
#include <stdbool.h>

// pico-sdk includes
// FreeRTOS includes
// Project includes
#include "global/debug_print.h"

#include "wlan/wlan.h"
#include "wlan/tcp_udp.h"


/* --- Local macro definitions ---------------------------------------------- */

/* --- Local type/struct definitions ---------------------------------------- */

/* --- Static variables ----------------------------------------------------- */

static logLevel_t eaDebugServerityLevel[NumCl];

/* --- Static function prototypes ------------------------------------------- */

/* --- Public functions ----------------------------------------------------- */

eRetVal_t eDebugPreInit(void)
{
    eRetVal_t eRetVal = ErrNoError;

    eaDebugServerityLevel[FN_UNKNOWN] = DEFAULT_DEBUG_LEVEL;
    eaDebugServerityLevel[FN_MAIN]    = DEFAULT_DEBUG_LEVEL;
    eaDebugServerityLevel[FN_WLAN]    = DEFAULT_DEBUG_LEVEL;
    eaDebugServerityLevel[FN_SNTP]    = DEFAULT_DEBUG_LEVEL;
    eaDebugServerityLevel[FN_TCPUDP]  = DEFAULT_DEBUG_LEVEL;
    eaDebugServerityLevel[FN_TEMP]    = DEFAULT_DEBUG_LEVEL;

    return(eRetVal);
}


void vDebugSetSeverity(const function_t eFunction, const logLevel_t eSeverity)
{
    if ((eFunction < NumCl) && (eSeverity < NumDbgLvl))
    {
        eaDebugServerityLevel[eFunction] = NumDbgLvl;
    }
}


void _vDebugPrint(
    const function_t eFunction,
    const logLevel_t eLevel,
    const char *cpFileName,
    const char *cpFunction,
    const uint32_t uLineNumber,
    const uint8_t uCore,
    const char *format,
    ...)
{
    char caLevelIndicator[NumDbgLvl + 1UL][13U] =
        {
            {"\e[0;31m-E-"},
            {"\e[0;33m-W-"},
            {"\e[0;32m-I-"},
            {"\e[0;37m-D-"},
            {"\e[0;37m---"}
        };
    char caMessageBuffer[MAX_UDP_BUFFER];
    char *cDbgLvl;
    uint16_t uCurrPos;
    va_list args;

    if (eLevel <= eaDebugServerityLevel[eFunction])
    {
        if (eLevel < NumDbgLvl)
        {
            cDbgLvl = caLevelIndicator[eLevel];
        }
        else
        {
            cDbgLvl = caLevelIndicator[NumDbgLvl];
        }

        uCurrPos = snprintf(
            caMessageBuffer,
            MAX_UDP_BUFFER,
            "%s C%d %s:%s.%d\e[0m: ",
            cDbgLvl,
            uCore,
            cpFileName,
            cpFunction,
            (int)uLineNumber
            );

        va_start(args, format);
        vsnprintf(
            &caMessageBuffer[uCurrPos],
            (MAX_UDP_BUFFER - uCurrPos),
            format, args
            );
        va_end(args);

        // Print to UART
        printf(caMessageBuffer);

        // If WIFI is up, send the string via UDP
        if (bWlanIsConnected())
        {
            vTcpUdpPrintUdp(caMessageBuffer);
        }
    }
}


/* --- Static functions ----------------------------------------------------- */
