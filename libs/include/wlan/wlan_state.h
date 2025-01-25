/** ****************************************************************************
 * @file   wlan_state.h
 *
 * @author Michael R.
 *
 * @brief  Public state for the WLAN library
 *
 * @date   2023-09-22
 **************************************************************************** */

#ifndef WLAN_STATE_H
#define WLAN_STATE_H

/* --- Includes ------------------------------------------------------------- */

#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "timers.h"   /* RTOS timer related API prototypes. */

/* --- Public macro definitions --------------------------------------------- */


/* --- Public type/struct definitions --------------------------------------- */

/**
 * @brief Ennumeration of the Notification flags for the main task
 */
typedef enum eWlanNotifications_tag
{
    WlanTimerExpired = (1UL << 0U),
} eWlanNotifications_t;

typedef struct sWlanState_tag
{
    // RTOS related variables
    TaskHandle_t xMainTask;
    TimerHandle_t xTimer;
} sWlanState_t;

/* --- Public variables ----------------------------------------------------- */


/* --- Public function prototypes ------------------------------------------- */

sWlanState_t* sWlanGetState(void);

#endif /* WLAN_STATE_H */
