// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <stdarg.h>

/*=========================================================*/

/**
 * \file utils.c
 *
 * Разнообразные мелкие утилиты.
 */

/*=========================================================*/

/**
 * Выбор первой не пустой строки.
 *
 * @param first Строка для затравки.
 * @param ... Прочие строки.
 * @return Первая не пустая строка.
 * @warning Если все строки пустые, неопределенное поведение.
 */
MAGNA_API const char* choose_string
    (
        const char *first,
        ...
    )
{
    va_list args;
    const char *next;

    if (first != NULL && *first) {
        return first;
    }

    va_start (args, first);

    while (AM_TRUE) {
        next = va_arg (args, const char*);
        if (next != NULL && *next) {
            va_end (args);
            return next;
        }
    }
}

/**
 * Выбор первого не пустого спана.
 *
 * @param first Спан для затравки.
 * @param ... Прочие спаны.
 * @return Первый не пустой спан.
 * @warning Если все спаны пустые, неопределенное поведение.
 */
MAGNA_API Span choose_span
    (
        Span first,
        ...
    )
{
    va_list args;
    Span next;

    if (!span_is_empty (first)) {
        return first;
    }

    va_start (args, first);

    while (AM_TRUE) {
        next = va_arg (args, Span);
        if (!span_is_empty (next)) {
            va_end (args);
            return next;
        }
    }
}

/**
 * Выдает специфичный для платформы перевод строки.
 *
 * @return ASCIIZ с переводом строки.
 */
MAGNA_API MAGNA_INLINE const char* newline (void)
{
#if defined(MAGNA_WINDOWS)

    return "\r\n";

#else

    return "\n";

#endif
}

/**
 * Выбор первого не пустого буфера.
 *
 * @param first Буфер для затравки.
 * @param ... Прочие буферы.
 * @return Первый не пустой буфер.
 * @warning Если все буферы пустые, неопределенное поведение.
 */
MAGNA_API Buffer* choose_buffer
    (
        const Buffer *first,
        ...
    )
{
    va_list args;
    const Buffer *next;

    if (first != NULL && first->position != 0) {
        return (Buffer*) first;
    }

    va_start (args, first);

    while (AM_TRUE) {
        next = va_arg (args, const Buffer*);
        if (next != NULL && next->position != 0) {
            va_end (args);
            return (Buffer*) next;
        }
    }
}

/**
 * Вычисление минимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение минимума.
 */
MAGNA_API am_int32 MAGNA_CALL min_int32
    (
        am_int32 first,
        am_int32 second
    )
{
    return first < second ? first : second;
}

/**
 * Вычисление максимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение максимума.
 */
MAGNA_API am_int32 MAGNA_CALL max_int32
    (
        am_int32 first,
        am_int32 second
    )
{
    return first > second ? first : second;
}

/**
 * Вычисление минимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение минимума.
 */
MAGNA_API am_int32 MAGNA_CALL min_uint32
    (
        am_uint32 first,
        am_uint32 second
    )
{
    return first < second ? first : second;
}

/**
 * Вычисление максимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение максимума.
 */
MAGNA_API am_uint32 MAGNA_CALL max_uint32
    (
        am_uint32 first,
        am_uint32 second
    )
{
    return first > second ? first : second;
}

/**
 * Вычисление минимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение минимума.
 */
MAGNA_API am_int64 MAGNA_CALL min_int64
    (
        am_int64 first,
        am_int64 second
    )
{
    return first < second ? first : second;
}

/**
 * Вычисление максимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение максимума.
 */
MAGNA_API am_int64 MAGNA_CALL max_int64
    (
        am_int64 first,
        am_int64 second
    )
{
    return first > second ? first : second;
}

/**
 * Вычисление минимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение минимума.
 */
MAGNA_API am_uint64 MAGNA_CALL min_uint64
    (
        am_uint64 first,
        am_uint64 second
    )
{
    return first < second ? first : second;
}

/**
 * Вычисление максимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение максимума.
 */
MAGNA_API am_uint64 MAGNA_CALL max_uint64
    (
        am_uint64 first,
        am_uint64 second
    )
{
    return first > second ? first : second;
}

/**
 * Вычисление минимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение минимума.
 */
MAGNA_API size_t MAGNA_CALL min_size_t
    (
        size_t first,
        size_t second
    )
{
    return first < second ? first : second;
}

/**
 * Вычисление максимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение максимума.
 */
MAGNA_API size_t MAGNA_CALL max_size_t
    (
        size_t first,
        size_t second
    )
{
    return first > second ? first : second;
}

/**
 * Знак целого числа.
 *
 * @param value Изучаемое значение.
 * @return -1, если отрицательное, 1 если положительное, иначе 0.
 */
MAGNA_API MAGNA_INLINE int MAGNA_CALL magna_sign_int
    (
        int value
    )
{
    return value > 0 ? 1 : (value < 0) ? -1 : 0;
}

/**
 * Знак целого числа.
 *
 * @param value Изучаемое значение.
 * @return -1, если отрицательное, 1 если положительное, иначе 0.
 */
MAGNA_API MAGNA_INLINE int MAGNA_CALL magna_sign_int32
    (
        am_int32 value
    )
{
    return value > 0 ? 1 : (value < 0) ? -1 : 0;
}

/**
 * Знак целого числа.
 *
 * @param value Изучаемое значение.
 * @return -1, если отрицательное, 1 если положительное, иначе 0.
 */
MAGNA_API MAGNA_INLINE int MAGNA_CALL magna_sign_int64
    (
        am_int64 value
    )
{
    return value > 0 ? 1 : (value < 0) ? -1 : 0;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
