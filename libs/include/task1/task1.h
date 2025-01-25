/** ****************************************************************************
 * @file   task1.h
 *
 * @author Michael R.
 *
 * @brief  Simple example
 *
 * @date   2023-08-18
 **************************************************************************** */

#ifndef _TASK1_TASK1_H
#define _TASK1_TASK1_H

/* --- Includes ------------------------------------------------------------- */

// libc includes
// pico-sdk includes
// FreeRTOS includes
// Project includes
#include "global/error_types.h"

/* --- Public macro definitions --------------------------------------------- */

/* --- Public type/struct definitions --------------------------------------- */

/* --- Public variables ----------------------------------------------------- */

/* --- Public function prototypes ------------------------------------------- */

/**
 * @brief Initializes all things that can/should be configured prior RTOS starts
 *
 * @return Returns success/error
 */
eRetVal_t eTask1HwInit(void);

/**
 * @brief Initializes the FreeRTOS task for the example-code.
 *
 * Can/should be called before the FreeRTOS sheduler starts.
 *
 * @return Returns success/error
 */
eRetVal_t eTask1RtosInit(void);

#endif  /* _TASK1_TASK1_H */
