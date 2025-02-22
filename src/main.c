/** ****************************************************************************
 * @file   main.c
 *
 * @author Michael R.
 *
 * @brief
 *
 * @date   2025-01-12
 **************************************************************************** */

/* --- Includes ------------------------------------------------------------- */
// libc includes
#include <stdio.h>

// Kernel includes
#include "FreeRTOS.h" /* Must come first. */
#include "FreeRTOSConfig.h"
#include "task.h"

// pico-sdk includes
#include "pico/stdlib.h"
#include "pico/multicore.h"

// Project includes
#include "project_conf.h"

#include "task1/task1.h"
#include "global/debug_print.h"
#include "global/utils.h"
#include "wlan/wlan.h"
#include "wlan/tcp_udp.h"


/* --- Private macro definitions -------------------------------------------- */


/* --- Local type/struct definitions ---------------------------------------- */


/* --- Static variables ----------------------------------------------------- */


/* --- Static function prototypes ------------------------------------------- */

/**
 * @brief Run some bare-metal HW-Init and call the pre-RTOS init functions from
 * the lib-functions
 *
 * @return eRetVal_t Returns success/error
 */
static eRetVal_t eMainPreInit(void);

/**
 * @brief Call the RTOS task initializer functions of the lib-functions prior
 * FreeRTOS task-sheduler is started
 *
 * @return eRetVal_t Returns success/error
 */
static eRetVal_t eMainRtosInit(void);

/* --- Public functions ----------------------------------------------------- */

int main(void)
{
    eRetVal_t eRetVal = ErrNoError;

    if (IS_NO_ERR(eRetVal))
    {
        // Iitialize HW stuff prior any FreeRTOS related config
         eRetVal = eMainPreInit();
    }

    if (IS_NO_ERR(eRetVal))
    {
        sleep_ms(2000);  // Wait a bit (UART via USB takes a while)

        DBG_PR(
            DBG_INFO,
            FN_MAIN,
            "Version = %d.%d.%d\n",
            PROJECT_VERSION_MAJOR,
            PROJECT_VERSION_MINOR,
            PROJECT_VERSION_PATCH
            );

        // Initialize FreeRTOS stuff
        eRetVal = eMainRtosInit();
    }

    if (IS_NO_ERR(eRetVal))
    {
        // Start FreeRTOS
        /* Start the tasks and timer running. */
        vTaskStartScheduler();
        printf ("-E- C0 main.c:main(): FATAL - FreeRTOS sheduler exited!\n");
    }
    else
    {
        printf ("-E- C0 main.c:main(): Program start failed!\n");
    }

    panic_unsupported();

    return (0);
}


/* --- Static functions ----------------------------------------------------- */

static eRetVal_t eMainPreInit( void )
{
    eRetVal_t eRetVal = ErrNoError;

    if (IS_NO_ERR(eRetVal))
    {
        /* Want to be able to printf */
        stdio_init_all();
        sleep_ms(800); // Wait a bit (UART via USB takes a while)
        eRetVal = eDebugPreInit();
    }

    if (IS_NO_ERR(eRetVal))
    {
        eRetVal = eWlanPreInit();
    }

    if (IS_NO_ERR(eRetVal))
    {
        eRetVal = eTask1HwInit();
    }

    if(IS_ERR(eRetVal))
    {
        printf ("-E- C0 main.c:eMainPreInit(): Pre-Init failed!\n");
    }

    return(eRetVal);
}


static eRetVal_t eMainRtosInit(void)
{
    eRetVal_t eRetVal = ErrNoError;

    if (IS_NO_ERR(eRetVal))
    {
        eRetVal = eDebugRtosInit();
    }

    if (IS_NO_ERR(eRetVal))
    {
        eRetVal = eWlanRtosInit();
    }

    if (IS_NO_ERR(eRetVal))
    {
        eRetVal = eTcpUdpRtosInit();
    }

    if (IS_NO_ERR(eRetVal))
    {
        eRetVal = eTask1RtosInit();
    }

    if(IS_ERR(eRetVal))
    {
        DBG_PR(
            DBG_ERROR,
            FN_MAIN,
            "-E- C0 main.c:eMainRtosInit(): vMainRtosInit failed\n"
            );
    }

    return(eRetVal);
}


/* --- FreeRTOS specific function implementation ---------------------------- */

void vApplicationMallocFailedHook( void )
{
    /* The malloc failed hook is enabled by setting
    configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

    Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
    panic("malloc failed");
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) xTask;

    printf ("FATAL: FreeRTOS detected Stack-Overflow!\n");

    /* Run time stack overflow checking is performed if
    configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected.  pxCurrentTCB can be
    inspected in the debugger if the task name passed into this function is
    corrupt. */
    for( ;; );
}
