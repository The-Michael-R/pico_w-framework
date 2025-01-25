/** ****************************************************************************
 * @file   wlan_state.c
 *
 * @author Michael R.
 *
 * @brief
 *
 * @date   2023-09-22
 **************************************************************************** */

/* --- Includes ------------------------------------------------------------- */

// libc includes
// pico-sdk includes
// FreeRTOS includes
// Project includes
#include "wlan/wlan_state.h"

/* --- Local macro definitions ---------------------------------------------- */


/* --- Local type/struct definitions ---------------------------------------- */


/* --- Static variables ----------------------------------------------------- */

static sWlanState_t sWlanState;


/* --- Static function prototypes ------------------------------------------- */

/* --- Public functions ----------------------------------------------------- */

sWlanState_t* sWlanGetState(void)
{
    return (&sWlanState);
}

/* --- Static functions ----------------------------------------------------- */
