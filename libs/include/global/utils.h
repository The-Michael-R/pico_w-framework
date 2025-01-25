/** ****************************************************************************
 * @file   utils.h
 *
 * @author Michael R.
 *
 * @brief  Set of useful macros / functions.
 *
 * @date   2023-09-19
 **************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

/* --- Includes ------------------------------------------------------------- */

// libc includes
// pico-sdk includes
// FreeRTOS includes
// Project includes

/* --- Public macro definitions --------------------------------------------- */


/** Clears the _N-th bit in _VAL*/
#define CLEAR_BIT(_VAL, _N)          (_VAL = _VAL & ~((uint32_t)1UL << _N))
/** Clears the _BITPATTERN bits in _VAL */
#define CLEAR_PAT(_VAL, _BITPATTERN) (_VAL = _VAL & ~((uint32_t)_BITPATTERN))

/** Tests if _N-th bit in _VAL is set */
#define TEST_BIT(_VAL, _N)          (((_VAL & (1UL << _N)) == 0) ? false : true)
/** Tests if all bis in _BITPATTERN in _VAL are set */
#define TEST_PAT(_VAL, _BITPATTERN) (((_VAL & _BITPATTERN) == 0) ? false : true)

/** Set the _N-th bit in _VAL*/
#define SET_BIT(_VAL, _N) (_VAL = _VAL | (1UL << _N))

/* --- Public type/struct definitions --------------------------------------- */


/* --- Public variables ----------------------------------------------------- */


/* --- Public function prototypes ------------------------------------------- */


#endif /* UTILS_H */
