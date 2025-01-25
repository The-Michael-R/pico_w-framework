/** ****************************************************************************
 * @file   mysntp.c
 *
 * @author Michael R.
 *
 * @brief  Start SNTP background time and date fetch
 *
 * @date   2023-10-02
 **************************************************************************** */

/* --- Includes ------------------------------------------------------------- */

// libc includes
// pico-sdk includes
#include "lwip/apps/sntp.h"
#include "pico/time.h"

// pico-sdk includes
// FreeRTOS includes
// Project includes
#include "wlan/mysntp.h"
#include "global/debug_print.h"

/* --- Local macro definitions ---------------------------------------------- */

#define SNTP_SERVER "pool.ntp.org"

/* --- Local type/struct definitions ---------------------------------------- */

/* --- Static function prototypes ------------------------------------------- */

/* --- Static variables ----------------------------------------------------- */

/* --- Public functions ----------------------------------------------------- */

void vSntpPreInit(void)
{
    const struct timespec ts = {
        .tv_sec = 0,
        .tv_nsec = 0
    };

    DBG_PR(DBG_INFO, FN_SNTP, "\n");

    sntp_setservername(0, SNTP_SERVER);
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    aon_timer_start(&ts);
}


eRetVal_t eSntpRtosInit(void)
{
    eRetVal_t eRetVal = ErrNoError;

    return (eRetVal);
}


void vSntpStart(void)
{
    if (!sntp_enabled())
    {
        sntp_init();
    }
}


void vSntpStop(void)
{
    if (sntp_enabled())
    {
        sntp_stop();
    }
}


uint32_t uSntpGetTimeBCD(void)
{
    uint32_t uTimeBCD = 0UL;
    struct tm tm;

    if (aon_timer_get_time_calendar(&tm))
    {
        uTimeBCD = ((tm.tm_sec       ) % 10U) <<  0U |
                   ((tm.tm_sec  / 10U) % 10U) <<  4U |
                   ((tm.tm_min       ) % 10U) <<  8U |
                   ((tm.tm_min  / 10U) % 10U) << 12U |
                   ((tm.tm_hour      ) % 10U) << 16U |
                   ((tm.tm_hour / 10U) % 10U) << 20U;
    }
    else
    {
        uTimeBCD = UINT32_MAX;
    }

    return (uTimeBCD);
}

uint32_t uSntpGetDateBCD(void)
{
    uint32_t uDateBCD = 0UL;
    struct tm tm;

    if (aon_timer_get_time_calendar(&tm))
    {
        uDateBCD = ((tm.tm_year        ) % 10U) <<  0U |
                   ((tm.tm_year /   10U) % 10U) <<  4U |
                   ((tm.tm_year /  100U) % 10U) <<  8U |
                   ((tm.tm_year / 1000U) % 10U) << 12U |
                   ((tm.tm_mon         ) % 10U) << 16U |
                   ((tm.tm_mon    / 10U) % 10U) << 20U |
                   ((tm.tm_mday        ) % 10U) << 24U |
                   ((tm.tm_mday   / 10U) % 10U) << 28U;
    }
    else
    {
        uDateBCD = UINT32_MAX;
    }

    return (uDateBCD);
}

/* --- Static functions ----------------------------------------------------- */
