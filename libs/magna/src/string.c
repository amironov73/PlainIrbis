/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 5045)
#endif

/*=========================================================*/

#include <ctype.h>

/*=========================================================*/

/**
 * Determines if a given string is blank (contains
 * whitespace only).
 *
 * @param str string to check.
 * @return Zero if the string is not blank, otherwise it is.
 */
MAGNA_API int MAGNA_CALL strblank
    (
        char *str
    )
{
    char *ptr = str;

    while (*ptr) {
        if (!isspace (*ptr)) {
            return 0;
        }
        ++ptr;
    }

    return 1;
}

/**
 * Finds all letters in a string matching one character
 * and replaces them with another.
 *
 * @param str string to search.
 * @param oldch character to search for.
 * @param newch character to replace with.
 * @return The number of matches found.
 */
MAGNA_API int MAGNA_CALL strchg
    (
        char *str,
        int oldch,
        int newch
    )
{
    int result = 0;
    char *ptr = str;

    while (*ptr) {
        if (*ptr == oldch) {
            *ptr = (char) newch;
            ++result;
        }
        ++ptr;
    }

    return result;
}

/**
 * Returns the number of occurrences of a given character
 * in a string.
 *
 * @param str string to search.
 * @param ch character to look for.
 * @return The number of occurrences of the given character
 * in the string.
 */
MAGNA_API int MAGNA_CALL strocc
    (
        char *str,
        int ch
    )
{
    int result = 0;
    char *ptr = str;

    while (*ptr) {
        if (*ptr == ch) {
            ++result;
        }
        ++ptr;
    }

    return result;
}

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
