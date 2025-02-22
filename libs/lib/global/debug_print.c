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
#include <stdlib.h>

// pico-sdk includes
// FreeRTOS includes
#include "FreeRTOS.h" /* Must come first. */
#include "semphr.h"

// Project includes
#include "global/debug_print.h"

#include "wlan/wlan.h"
#include "wlan/tcp_udp.h"


/* --- Local macro definitions ---------------------------------------------- */

/* --- Local type/struct definitions ---------------------------------------- */

/* --- Static variables ----------------------------------------------------- */

static SemaphoreHandle_t xMessageBufferSem = NULL;
static char caMessageBuffer[MAX_UDP_BUFFER];

static logLevel_t eaDebugServerityLevel[NumCl];
static char caLevelIndicator[NumDbgLvl + 1UL][13U] =
{
    {},
    {"\e[0;31m-E-"},
    {"\e[0;33m-W-"},
    {"\e[0;32m-I-"},
    {"\e[0;37m-D-"},
    {"\e[0;37m---"}
};

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
    eaDebugServerityLevel[FN_SNTP]    = DEFAULT_DEBUG_LEVEL;

    return(eRetVal);
}


eRetVal_t eDebugRtosInit(void)
{
    eRetVal_t eRetVal = ErrNoError;

    xMessageBufferSem = xSemaphoreCreateMutex();

    if(NULL == xMessageBufferSem)
    {
        eRetVal = ErrError;
    }

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
    const char* cpFileName,
    const char* cpFunction,
    const uint32_t uLineNumber,
    const uint8_t uCore,
    const char* format,
    ...)
{
    char* cDbgLvl;
    uint16_t uCurrPos;
    va_list args;
    BaseType_t xGotSema = pdTRUE;
    const bool bRtosActive = !(taskSCHEDULER_NOT_STARTED == xTaskGetSchedulerState());


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

        if (bRtosActive)
        {
            xGotSema = xSemaphoreTake(xMessageBufferSem, (TickType_t)2U);
        }

        if(pdTRUE == xGotSema)
        {

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
            printf("%s", caMessageBuffer);

            // If WIFI is up, send the string via UDP
            if (bWlanIsConnected())
            {
                vTcpUdpPrintUdp(caMessageBuffer);
            }

            if (bRtosActive)
            {
                xSemaphoreGive(xMessageBufferSem);
            }
        }
    }
}

/* --- Static functions ----------------------------------------------------- */
