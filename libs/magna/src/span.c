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
 * @file span.c
 *
 * Невладеющий указатель на участок памяти.
 *
 */

/**
 * Простая инициализация.
 *
 * @param ptr
 * @param len
 * @return
 */
MAGNA_API Span MAGNA_CALL span_init
    (
        am_byte *ptr,
        am_size len
    )
{
    Span result = { ptr, len };

    return result;
}

/**
 * Инициализация строкой.
 *
 * @param str
 * @return
 */
MAGNA_API Span MAGNA_CALL span_from_text
    (
        char *str
    )
{
    Span result;

    assert (str != NULL);

    result.ptr = str;
    result.len = strlen (str);

    return result;
}

/**
 * Удаление пробельных символов из начала спана.
 *
 * @param span
 * @return
 */
MAGNA_API Span MAGNA_CALL span_trim_start
    (
        Span span
    )
{
    Span result = span;

    assert (span.ptr != NULL);

    while (result.len != 0) {
        if (!isspace (*result.ptr)) {
            break;
        }
        ++result.ptr;
        --result.len;
    }

    return result;
}

/**
 * Удаление пробельных символов из конца спана.
 *
 * @param span
 * @return
 */
MAGNA_API Span MAGNA_CALL span_trim_end
    (
        Span span
    )
{
    Span result = span;

    assert (span.ptr != NULL);

    while (result.len != 0) {
        if (!isspace (result.ptr [result.len - 1])) {
            break;
        }
        --result.len;
    }

    return result;
}

/**
 * Удаление пробельных символов из начала и конца спана.
 * @param span
 * @return
 */
MAGNA_API Span  MAGNA_CALL span_trim
    (
        Span span
    )
{
    return span_trim_end
        (
            span_trim_start (span)
        );
}

/**
 * Преобразование спана в целое число без знака.
 *
 * @param span
 * @return
 */
MAGNA_API am_uint32 MAGNA_CALL span_to_uint_32
    (
        Span span
    )
{
    am_uint32 result = 0;
    am_byte *ptr = span.ptr;
    am_size len = span.len;

    while (len--) {
        result = result * 10 + (*ptr++ - '0');
    }

    return result;
}

/**
 * Преобразование спана в целое число без знака.
 *
 * @param span
 * @return
 */
MAGNA_API am_uint64 MAGNA_CALL span_to_uint_64
    (
        Span span
    )
{
    am_uint64 result = 0;
    am_byte *ptr = span.ptr;
    am_size len = span.len;

    while (len--) {
        result = result * 10 + (*ptr++ - '0');
    }

    return result;
}

/**
 * Перевод спана в верхний регистр.
 *
 * @param span
 */
MAGNA_API void MAGNA_CALL span_toupper
    (
        Span span
    )
{
    am_byte *ptr = span.ptr;
    am_size len = span.len;

    while (len--) {
        *ptr = (am_byte) toupper (*ptr);
        ++ptr;
    }
}

/**
 * Перевод спана в нижний регистр.
 *
 * @param span
 */
MAGNA_API void MAGNA_CALL span_tolower
    (
        Span span
    )
{
    am_byte *ptr = span.ptr;
    am_size len = span.len;

    while (len--) {
        *ptr = (am_byte) tolower (*ptr);
        ++ptr;
    }
}

/**
 * Спан пустой?
 *
 * @param span
 * @return
 */
MAGNA_API am_bool MAGNA_CALL span_is_empty
    (
        Span span
    )
{
    return span.len == 0 || span.ptr == NULL;
}

/**
 * Содержит ли спан указанный элемент?
 *
 * @param span
 * @param value
 * @return
 */
MAGNA_API am_bool MAGNA_CALL span_contains
    (
        Span span,
        am_byte value
    )
{
    am_bool result = 0;
    am_byte *ptr = span.ptr;
    am_size len = span.len;

    while (len--) {
        if (*ptr++ == value) {
            result = 1;
            break;
        }
    }

    return result;
}

/**
 * Заполнение спана указанным значением.
 *
 * @param span
 * @param value
 */
MAGNA_API void MAGNA_CALL span_fill
    (
        Span span,
        am_byte value
    )
{
    am_byte *ptr = span.ptr;
    am_size len = span.len;

    while (len--) {
        *ptr++ = value;
    }
}

/**
 * Поиск указанного значения в спане с начала.
 *
 * @param span
 * @param value
 * @return
 */
MAGNA_API am_ssize MAGNA_CALL span_index_of
    (
        Span span,
        am_byte value
    )
{
    am_size i;

    for (i = 0; i < span.len; ++i) {
        if (span.ptr[i] == value) {
            return (am_ssize) i;
        }
    }

    return -1;
}

/**
 * Поиск указанного значения в спане с конца.
 *
 * @param span
 * @param value
 * @return
 */
MAGNA_API am_ssize MAGNA_CALL span_last_index_of
    (
        Span span,
        am_byte value
    )
{
    am_ssize i;

    for (i = span.len - 1; i >= 0; --i) {
        if (span.ptr[i] == value) {
            return i;
        }
    }

    return i;
}

/**
 * Срез спана.
 *
 * @param span
 * @param start
 * @param length
 * @return
 */
MAGNA_API Span MAGNA_CALL span_slice
    (
        Span span,
        am_ssize start,
        am_ssize length
    )
{
    Span result = span;

    if (length == -1) {
        length = result.len - start;
    }

    if (length < 0) {
        length = 0;
    }

    result.ptr += start;
    result.len = length;

    return result;
}

/**
 * Преобразование спана в строку.
 *
 * @param span
 * @return
 */
MAGNA_API char* MAGNA_CALL span_to_string
    (
        Span span
    )
{
    char *result = calloc (span.len + 1, 1);

    if (span.len != 0 && span.ptr != NULL) {
        memcpy (result, span.ptr, span.len);
    }
    result [span.len] = 0;

    return result;
}

/**
 * Преобразование спана в вектор байтов.
 *
 * @param span
 * @return
 */
MAGNA_API am_byte* MAGNA_CALL span_to_vector
    (
        Span span
    )
{
    am_byte *result = malloc (span.len);

    if (span.len != 0 && span.ptr != NULL) {
        memcpy (result, span.ptr, span.len);
    }

    return result;
}

/**
 * Совпадает ли начало спана с другим спаном?
 *
 * @param span
 * @param prefix
 * @return
 */
MAGNA_API am_bool MAGNA_CALL span_starts_with
    (
        Span span,
        Span prefix
    )
{
    return 0;
}

/**
 * Совпадает ли конец спана с другим спаном?
 *
 * @param span
 * @param suffix
 * @return
 */
MAGNA_API am_bool MAGNA_CALL span_ends_with
    (
        Span span,
        Span suffix
    )
{
    return 0;
}

/**
 * Побайтовое сравнение двух спанов.
 *
 * @param first
 * @param second
 * @return
 */
MAGNA_API int MAGNA_CALL span_compare
    (
        Span first,
        Span second
    )
{
    return 0;
}

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
