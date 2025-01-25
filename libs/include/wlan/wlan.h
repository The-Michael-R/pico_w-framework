/** ****************************************************************************
 * @file   wlan.h
 *
 * @author Michael R.
 *
 * @brief  WLAN driver
 *
 * @date   2023-08-18
 **************************************************************************** */

#ifndef WLAN_H
#define WLAN_H

/* --- Includes ------------------------------------------------------------- */
#include <stdbool.h>

#include "global/error_types.h"


/* --- Public macro definitions --------------------------------------------- */


/* --- Public type/struct definitions --------------------------------------- */


/* --- Public variables ----------------------------------------------------- */


/* --- Public function prototypes ------------------------------------------- */

/**
 * @brief Configure FreeRTOS independent (HW and SW) parts.
 *
 * @return Returns success/error
 */
eRetVal_t eWlanPreInit(void);

/**
 * @brief Configure the FreeRTOS specific code.
 *
 * @return eRetVal_t Returns success/error
 */
eRetVal_t eWlanRtosInit(void);

/**
 * @brief Returns the WLAN connecton state.
 *
 * @return true   WLAN is ready to transmit.
 * @return false  WLAN is down.
 */
bool bWlanIsConnected(void);

#endif /* WLAN_H */
