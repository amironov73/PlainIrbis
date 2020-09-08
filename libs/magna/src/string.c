/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 5045)
#endif

/*=========================================================*/

#include <assert.h>
#include <ctype.h>

/*=========================================================*/

/**
 * Совпадают ли символы с точностью до регистра?
 *
 * @param first
 * @param second
 * @return
 */
MAGNA_API am_bool MAGNA_CALL same_char
    (
        int first,
        int second
    )
{
    return toupper(first) == toupper(second);
}

/**
 * Совпадают ли строки с точностью до регистра?
 *
 * @param first
 * @param second
 * @return
 */
MAGNA_API am_bool MAGNA_CALL same_text
    (
        const char *first,
        const char *second
    )
{
    int result;
    char c1, c2;

    assert (first != NULL);
    assert (second != NULL);

    while (1) {
        c1 = *first++;
        c2 = *second++;
        result = toupper(c1) - toupper(c2);
        if (result != 0) {
            return result;
        }

        if (!c1) {
            return 0;
        }
    }
}


/**
 * Determines if a given string is blank (contains
 * whitespace only).
 *
 * @param str string to check.
 * @return Zero if the string is not blank, otherwise it is.
 */
MAGNA_API am_bool MAGNA_CALL strblank
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
        char oldch,
        char newch
    )
{
    int result = 0;
    char *ptr = str;

    while (*ptr) {
        if (*ptr == oldch) {
            *ptr = newch;
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
MAGNA_API am_size_t MAGNA_CALL strocc
    (
        char *str,
        int ch
    )
{
    am_size_t result = 0;
    char *ptr = str;

    while (*ptr) {
        if (*ptr == ch) {
            ++result;
        }
        ++ptr;
    }

    return result;
}

/**
 * Быстрый грязный разбор 32-битного целого без знака.
 *
 * @param text
 * @param length
 * @return
 */
MAGNA_API am_uint32 MAGNA_CALL fastParse32
    (
        const am_byte *text,
        am_size_t length
    )
{
    am_uint32 result = 0;

    while (length--) {
        result = result * 10 + (*text++ - '0');
    }

    return result;
}

/**
 * Копия строки в куче. Освобождать должен вызывающий.
 *
 * @param text
 * @return
 */
MAGNA_API char* MAGNA_CALL str_dup
    (
        const char *text
    )
{
    am_size_t size;
    char *result;

    assert (text != NULL);

    size = strlen (text) + 1;
    result = malloc (size);
    memcpy (result, text, size);

    return result;
}

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
