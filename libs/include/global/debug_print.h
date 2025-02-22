/** ****************************************************************************
 * @file   debug_print.h
 *
 * @author Michael R.
 *
 * @brief  Functions for debug-prints with debug-level.
 *
 * @date   2023-08-26
 **************************************************************************** */

#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

/* --- Includes ------------------------------------------------------------- */

// libc includes
#include <stdarg.h>
#include <stdint.h>

// pico-sdk includes
// FreeRTOS includes
// Project includes
#include "global/error_types.h"


/* --- Public macro definitions --------------------------------------------- */

#ifndef __FILE_NAME__
#define __FILE_NAME__ __FILE__
#endif

/**
 * @brief Macro to do some pretty printing of messages within the project.
 *        If WLAN is active the message is also send via UDP.
 *
 * Example <code>DBG_PR(DBG_WARN, FN_MAIN, "Answer: %u\n", 42U);</code>
 *
 */
#define DBG_PR(_DBG_LVL, _CLASS, ...) (         \
                                _vDebugPrint(   \
                                _CLASS,         \
                                _DBG_LVL,       \
                                __FILE_NAME__,  \
                                __func__,       \
                                __LINE__,       \
                                get_core_num(), \
                                __VA_ARGS__))

/* --- Public type/struct definitions --------------------------------------- */

/**
 * @brief definition of the debug-level used by @ref DBG_PR
 */
typedef enum logLevel_tag
{
    DBG_OFF         = 0,               ///< Disables the print (must not used as DBG_PR parameter)
    DBG_ERROR       = 1,               ///< Error level (highest priority)
    DBG_WARNING     = 2,               ///< Warning level, identical to DBG_WARN
    DBG_WARN        = DBG_WARNING,     ///< Warning level, identical to DBG_WARNING
    DBG_INFORMATION = 3,               ///< Information level, identical to DBG_INFO
    DBG_INFO        = DBG_INFORMATION, ///< Information level, identical to DBG_INFORMATION
    DBG_DEBUG       = 4,               ///< Debug level, identical to DBG_DBG
    DBG_DBG         = DBG_DEBUG,       ///< Debug level, identical to DBG_DEBUG
    DBG_ALL         = DBG_DEBUG,       ///< Print all messages, identical to DBG_DEBUG
    NumDbgLvl       = 5                ///< Number of entries in this enum type
} logLevel_t;

/**
 * @brief List of individual functions to be addressed via print function
 */
typedef enum function_tag
{
    FN_UNKNOWN,
    FN_MAIN,
    FN_WLAN,
    FN_SNTP,
    FN_TCPUDP,
    NumCl
} function_t;

/* --- Public variables ----------------------------------------------------- */

/* --- Public function prototypes ------------------------------------------- */

/**
 * @brief Pre-FreeRTOS initialisiation.
 *
 * This function should be called early to enable the pint funtion at an early
 * stage.
 *
 * @return eRetVal_t Returns success/error
 */
eRetVal_t eDebugPreInit(void);


/**
 * @brief FreeRTOS related initialisation
 *
 * @return eRetVal_t Returns success/error
 */
eRetVal_t eDebugRtosInit(void);


/**
 * @brief Change severity-level of selected class
 *
 * @param eFunction  Selecting the class that severity to be changed
 * @param eSeverity  Target severity
 */
void vDebugSetSeverity(const function_t eFunction, const logLevel_t eSeverity);

/**
 * @brief A private function that does the actual printing
 *
 * @param eFunction     Fuction identifier; also used to check severity level
 * @param eLevel        Debug level
 * @param cpFileName    Filename from the function that does the DBG_PRINT
 * @param cpFunction    Function name
 * @param uLineNumber   The current line number
 * @param uCore         The active core
 * @param format        The string to be prionted
 * @param ...           Any additional parameters forwarded to vsnprintf
 */
void _vDebugPrint(
    const function_t eFunction,
    const logLevel_t eLevel,
    const char *cpFileName,
    const char *cpFunction,
    const uint32_t uLineNumber,
    const uint8_t uCore,
    const char *format,
    ...);

#endif /* DEBUG_PRINT_H */
