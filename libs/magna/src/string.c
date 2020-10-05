// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 5045)
#endif

/*=========================================================*/

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>

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
MAGNA_API size_t MAGNA_CALL strocc
    (
        char *str,
        int ch
    )
{
    size_t result = 0;
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
        size_t length
    )
{
    am_uint32 result = 0;

    while (length--) {
        result = result * 10 + (*text++ - '0');
    }

    return result;
}

/**
 * Быстрый грязный разбор 64-битного целого без знака.
 *
 * @param text
 * @param length
 * @return
 */
MAGNA_API am_uint64 MAGNA_CALL fastParse64
    (
        const am_byte *text,
        size_t length
    )
{
    am_uint64 result = 0;

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
    size_t size;
    char *result;

    assert (text != NULL);

    size = strlen (text) + 1;
    result = malloc (size);
    memcpy (result, text, size);

    return result;
}

/**
 * Является ли символ одним из перечисленных.
 *
 * @param one Проверяемый символ.
 * @param many Перечисленные символы.
 * @return
 */
MAGNA_API am_bool MAGNA_CALL char_one_of
    (
        am_byte one,
        const am_byte *many
    )
{
    assert (many != NULL);

    while (*many) {
        if (same_char (one, *many)) {
            return AM_TRUE;
        }

        ++many;
    }

    return AM_FALSE;
}

MAGNA_API am_bool str_one_of
    (
        const char *one,
        ...
    )
{
    am_bool result = AM_FALSE;
    const char *ptr;
    va_list args;

    assert (one != NULL);

    va_start (args, one);

    while ((ptr = va_arg (args, const char*)) != NULL) {
        if (same_text (one, ptr)) {
            return AM_TRUE;
        }
    }

    va_end (args);

    return result;
}

MAGNA_API int MAGNA_CALL str_safe_compare
    (
        const char *first,
        const char *second
    )
{
    if (first == NULL) {
        if (second == NULL) {
            return AM_TRUE;
        }

        return -1;
    }

    if (second == NULL) {
        return 1;
    }

    return strcmp (first, second);
}

/**
 * Превращает строку в видимую.
 *
 * @param text Строка.
 * @return Видимая строка, например, "null".
 */
MAGNA_API const char* MAGNA_CALL str_to_visible
    (
        const char *text
    )
{
    const char *ptr;

    if (text == NULL) {
        return "(null)";
    }

    if (*text == 0) {
        return "(empty)";
    }

    for (ptr = text; *ptr; ++ptr) {
        if (*ptr > ' ') {
            break;
        }
    }

    if (*ptr == 0) {
        return "(blank)";
    }

    return text;
}

/**
 * Переводим текст из ИРБИСного представления в клиентское:
 * 1) переводим из кодировки ANSI в UTF-8,
 * 2) заменяем ИРБИСные переводы строки на нормальные.
 *
 * @param output
 * @param input
 * @return
 */
MAGNA_API am_bool MAGNA_CALL irbis_to_client
    (
        Buffer *output,
        Span input
    )
{
    TextNavigator navigator;
    Span line;
    am_bool first = AM_TRUE;

    assert (output != NULL);

    nav_from_span (&navigator, input);
    while (!nav_eot(&navigator)) {
        if (!first) {
            if (!buffer_new_line (output)) {
                return AM_FALSE;
            }
        }
        line = nav_read_irbis (&navigator);
        if (!ansi2utf (output, line)) {
            return AM_FALSE;
        }

        first = AM_FALSE;
    }
}


/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
