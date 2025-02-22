/** ****************************************************************************
 * @file   wlan.c
 *
 * @author Michael R.
 *
 * @brief  WLAN driver
 *
 * @date   2023-08-08
 **************************************************************************** */

/* --- Includes ------------------------------------------------------------- */

// libc includes
#include <stdio.h>
#include <stdbool.h>

// pico-sdk includes
#include "pico/cyw43_arch.h"
#include "lwip/ip4_addr.h"

// Kernel includes
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "timers.h"   /* RTOS timer related API prototypes. */

// Project includes
#include "wlan/wlan.h"
#include "wlan/wlan_state.h"
#include "wlan/tcp_udp.h"
#include "wlan/mysntp.h"

#include "global/debug_print.h"
#include "global/utils.h"


/**
 * @brief  This file is not part of the public package.
 *
 * The file should contain the Wlan SSID and password. It should contain the
 * following variables:
 *
 *  #define WLAN_SSID "Dark Helmet"
 *  #define WLAN_PASSWORD "123456"
 */
#if __has_include("_wlan_credentials.h")
#include "_wlan_credentials.h"
#else
#define WLAN_SSID "Dark Helmet"
#define WLAN_PASSWORD "123456"
#endif


/* --- Local macro definitions ---------------------------------------------- */

#define WLAN_PRIORITY (tskIDLE_PRIORITY + 2UL)
#define WLAN_MAIN_STACK           (512UL * 5U)

#define WLAN_POLL_RATE_MS     (2UL * 1000UL)


/* --- Local type/struct definitions ---------------------------------------- */

/* --- Static variables ----------------------------------------------------- */

/* --- Static function prototypes ------------------------------------------- */

/**
 * @brief Main task taking care of WLAN connection. Will be started by FreeRTOS
 * task scheduler.
 *
 * @param pvParameters Unused
 */
static void vWlanMainTask(void *pvParameters);

/**
 * @brief Check if WLAN connection is active or needs to be re-connected
 *
 * @return true  WLAN is down, connection should be re-started
 * @return false WLAN up and usable
 */
static bool bWlanNeedReconnect(void);

/**
 * @brief Function tries to connect to WLAN
 *
 * @return eRetVal_t ErrNoError In case success; ErrError in case connection not
 * established
 */
static eRetVal_t eWlanConnect(void);

/**
 * @brief Timer function to trigger main task on regular basis.
 *
 * @param xTimer Unused
 */
static void vWlanTimerCB(TimerHandle_t xTimer);


/* --- Public functions ----------------------------------------------------- */

eRetVal_t eWlanPreInit(void)
{
    eRetVal_t eRetVal = ErrNoError;

    vSntpPreInit();

    return(eRetVal);
}


eRetVal_t eWlanRtosInit(void)
{
    eRetVal_t eRetVal = ErrNoError;
    BaseType_t xReturned;
    sWlanState_t* const sState = sWlanGetState();

    xReturned = xTaskCreateAffinitySet(
                    vWlanMainTask,
                    "WLAN",
                    WLAN_MAIN_STACK,
                    NULL,
                    WLAN_PRIORITY,
                    1UL << 0U,
                    &sState->xMainTask);

    if (pdPASS != xReturned)
    {
        eRetVal = ErrError;
    }
    else
    {
        sState->xTimer = xTimerCreate(
            "WLAN_Tmr",
            pdMS_TO_TICKS(WLAN_POLL_RATE_MS),
            pdTRUE,
            0,
            vWlanTimerCB);

        if (NULL != sState->xTimer)
        {
            xTimerStart(sState->xTimer, 0);
        }
        else
        {
            eRetVal = ErrError;
        }
    }

    return (eRetVal);
}


bool bWlanIsConnected(void)
{
    bool bRetVal = false;

    if (CYW43_LINK_UP == cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA))
    {
        bRetVal = true;
    }

    return (bRetVal);
}


/* --- Static functions ----------------------------------------------------- */

/**
 * @brief WLAN main task. Controls (re-)connect to WLAN network.
 *
 * @param pvParameters Unused
 */
static void vWlanMainTask(void *pvParameters)
{
    (void)pvParameters; // Silence 'unused parameters'

    uint32_t uNotifyVector = 0UL;
    eRetVal_t eRetVal;

    DBG_PR(DBG_INFO, FN_WLAN, "\n");

    if (0 != cyw43_arch_init_with_country(CYW43_COUNTRY_GERMANY))
    {
        DBG_PR(DBG_INFO, FN_WLAN, "Failed to initialise.\n");
        eRetVal = ErrError;
    }

    while (1)
    {
        xTaskNotifyWait(0UL, 0UL, &uNotifyVector, portMAX_DELAY);

        if (TEST_PAT(uNotifyVector, WlanTimerExpired))
        {
            CLEAR_PAT(uNotifyVector, WlanTimerExpired);

            if (true == bWlanNeedReconnect())
            {
                eRetVal = eWlanConnect();
                if (IS_NO_ERR(eRetVal))
                {
                    vSntpStart();
                }
            }
        }

        if (0UL != uNotifyVector)
        {
            DBG_PR(
                DBG_ERROR,
                FN_WLAN,
                "Received unknown notification (%d)!\n",
                uNotifyVector);

            uNotifyVector = 0UL;
        }
    }
}


static bool bWlanNeedReconnect(void)
{
    bool bRetVal;
    int iStatus;

    iStatus = cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA);

    switch (iStatus)
    {
    case CYW43_LINK_DOWN:
    case CYW43_LINK_FAIL:
    case CYW43_LINK_NONET:
    case CYW43_LINK_BADAUTH:
        bRetVal = true;
        break;

    case CYW43_LINK_JOIN:
        bRetVal = false;
        break;

    default:
        DBG_PR(
            DBG_ERROR,
            FN_WLAN,
            "Unknown wifi_link_status (%d)!\n",
            iStatus);

        vSntpStop();
        cyw43_arch_deinit();
        bRetVal = true;
        break;
    }

    if (!bRetVal)
    {
        iStatus = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);

        switch (iStatus)
        {
        case CYW43_LINK_DOWN:
        case CYW43_LINK_NOIP:
        case CYW43_LINK_FAIL:
        case CYW43_LINK_NONET:
        case CYW43_LINK_BADAUTH:
        case CYW43_LINK_JOIN:
            bRetVal = true;
            break;

        case CYW43_LINK_UP:
            bRetVal = false;
            break;

        default:
            DBG_PR(
                DBG_ERROR,
                FN_WLAN,
                "Unknown cyw43_tcpip_link_status (%d)!\n",
                iStatus);

            vSntpStop();
            cyw43_arch_deinit();
            bRetVal = true;
            break;
        }
    }

    return (bRetVal);
}


static eRetVal_t eWlanConnect(void)
{
    eRetVal_t eRetVal = ErrNoError;

    if (IS_NO_ERR(eRetVal))
    {
        cyw43_arch_enable_sta_mode();

        DBG_PR(DBG_INFO, FN_WLAN, "Connecting to Wi-Fi...\n");
        if (0 != cyw43_arch_wifi_connect_timeout_ms(WLAN_SSID, WLAN_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
        {
            DBG_PR(DBG_INFO, FN_WLAN, "Failed to connect.\n");
            cyw43_arch_deinit();
            eRetVal = ErrError;
        }
        else
        {
            eTcpUdpOpenSocket(IP_UDP, HOST_LOG_PORT);
        }
    }

    return (eRetVal);
}


/**
 * @brief Simple timer to wake the addressed task
 *
 * @param xTimer Unused
 */
static void vWlanTimerCB(TimerHandle_t xTimer)
{
    (void)xTimer;

    sWlanState_t *const sState = sWlanGetState();

    xTaskNotify(
        sState->xMainTask,
        WlanTimerExpired,
        eSetBits);
}
