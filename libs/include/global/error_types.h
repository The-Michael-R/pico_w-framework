/** ****************************************************************************
 * @file   error_types.h
 *
 * @author Michael R.
 *
 * @brief  Defines the error types
 *
 * @date   2023-08-18
 **************************************************************************** */

#ifndef ERROR_TYPES_H
#define ERROR_TYPES_H

/* --- Includes ------------------------------------------------------------- */

// libc includes
// pico-sdk includes
// FreeRTOS includes
// Project includes

/* --- Public macro definitions --------------------------------------------- */

#define IS_ERR(_X)    (_X != ErrNoError)
#define IS_NO_ERR(_X) (_X == ErrNoError)


/* --- Public type/struct definitions --------------------------------------- */
typedef enum eRetVal_tag
{
    ErrNoError = 0,
    ErrError   = 1,
} eRetVal_t;


/* --- Public variables ----------------------------------------------------- */


/* --- Public function prototypes ------------------------------------------- */


#endif /* ERROR_TYPES_H */
