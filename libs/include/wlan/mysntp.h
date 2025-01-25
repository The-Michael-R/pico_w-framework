/** ****************************************************************************
 * @file   mysntp.h
 *
 * @author Michael R.
 *
 * @brief  Start SNTP background time and date fetch
 *
 * @date   2023-10-02
 **************************************************************************** */

#ifndef MYSNTP_H
#define MYSNTP_H

/* --- Includes ------------------------------------------------------------- */

// libc includes
#include <stdint.h>

// pico-sdk includes
// FreeRTOS includes
// Project includes
#include "global/error_types.h"

/* --- Public macro definitions --------------------------------------------- */


/* --- Public type/struct definitions --------------------------------------- */


/* --- Public variables ----------------------------------------------------- */


/* --- Public function prototypes ------------------------------------------- */

/**
 * @brief Configure FreeRTOS independent (HW and SW) parts.
 */
void vSntpPreInit(void);

/**
 * @brief Configure the FreeRTOS specific code.
 */
eRetVal_t eSntpRtosInit(void);

/**
 * @brief Starts the SNTP client
 */
void vSntpStart(void);

/**
 * @brief Stops the SNTP client (e.g. if WIFI stops)
 */
void vSntpStop(void);

/**
 * @brief Get the current time in BCD format
 *
 * @return If time is synced the current time in BCD format,.
 * @return f not synced UINT32_MAX.
 */
uint32_t uSntpGetTimeBCD(void);

/**
 * @brief Get the current date in BCD format
 *
 * @return If time is synced the current Date in BCD format.
 * @return if not synced UINT32_MAX.
 */
uint32_t uSntpGetDateBCD(void);

#endif /* MYSNTP_H */
