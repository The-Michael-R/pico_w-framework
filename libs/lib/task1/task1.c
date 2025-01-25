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
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// pico-sdk includes
// FreeRTOS includes
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "FreeRTOSConfig.h"

// Project includes
#include "task1/task1.h"
#include "global/debug_print.h"

#include "pico/time.h"
#include "pico/aon_timer.h"
#include "pico/util/datetime.h"


/* --- Private macro defines ------------------------------------------------ */

#define TASK1_PRIORITY (2)

/* --- Local type/struct definitions ---------------------------------------- */

/* --- Static variables ----------------------------------------------------- */

/* --- Static function prototypes ------------------------------------------- */

/**
 * @brief Main task for task1 example.
 *
 * @param pvParameters Filled from FreRTOS scheduler, unused.
 */
static void vTask1Main( void * pvParameters );


/* --- Public functions ----------------------------------------------------- */

eRetVal_t eTask1HwInit(void)
{
    eRetVal_t eRetVal = ErrNoError;

    // Nothing do do here for this simple example

    return(eRetVal);
}

eRetVal_t eTask1RtosInit(void)
{
    eRetVal_t eRetVal = ErrNoError;

    /* Create the queue receive task as described in the comments at the top
    of this file. */
    xTaskCreate (vTask1Main,
                "Task1",
                configMINIMAL_STACK_SIZE,
                NULL,
                TASK1_PRIORITY,
                NULL
                );

    return (eRetVal);
}

/* --- Static functions ----------------------------------------------------- */

void vTask1Main(void * pvParameters)
{
    uint32_t i = 0;
    struct tm tm;

    (void) pvParameters;  // Silence compiler about unused parameters

    while(1)
    {
        aon_timer_get_time_calendar(&tm);
        i++;
        vTaskDelay(1000);
        DBG_PR(
            DBG_ERROR,
            FN_TEMP,
            "Ping %02d:%02d:%02d!\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    }
}
