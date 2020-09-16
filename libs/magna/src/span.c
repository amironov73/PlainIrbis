/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/*=========================================================*/

#include "warnpush.h"

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

/*=========================================================*/

/**
 * Простая инициализация.
 *
 * @param ptr Указатель на начало.
 * @param len Длина участка памяти.
 * @return Инициализированный спан.
 */
MAGNA_API Span MAGNA_CALL span_init
    (
        const am_byte *ptr,
        am_size_t len
    )
{
    Span result = { (am_byte*) ptr, len };

    return result;
}

/**
 * Инициализация строкой, завершающейся нулём.
 *
 * @param str Указатель на начало строки.
 * @return Инициализированный спан. Завершающий нуль не включается.
 */
MAGNA_API Span MAGNA_CALL span_from_text
    (
        const char *str
    )
{
    Span result;

    assert (str != NULL);

    result.ptr = (am_byte*) str;
    result.len = strlen (str);

    return result;
}

/**
 * Удаление пробельных символов из начала спана.
 *
 * @param span Спан.
 * @return Тот же спан, возможно, укороченный.
 */
MAGNA_API Span MAGNA_CALL span_trim_start
    (
        Span span
    )
{
    Span result = span;

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
 * @param span Спан.
 * @return Тот же спан, возможно, укороченный.
 */
MAGNA_API Span MAGNA_CALL span_trim_end
    (
        Span span
    )
{
    Span result = span;

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
 *
 * @param span Спан.
 * @return Тот же спан, возможно, укороченный.
 */
MAGNA_API Span MAGNA_CALL span_trim
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
 * @param span Спан.
 * @return Результат разбора. Мусор на входе -- мусор на выходе!
 */
MAGNA_API am_uint32 MAGNA_CALL span_to_uint_32
    (
        Span span
    )
{
    am_uint32 result = 0;
    am_byte *ptr = span.ptr;
    am_size_t len = span.len;

    while (len--) {
        result = result * 10 + (*ptr++ - '0');
    }

    return result;
}

/**
 * Преобразование спана в целое число без знака.
 *
 * @param span Спан.
 * @return Результат разбора. Мусор на входе -- мусор на выходе!
 */
MAGNA_API am_uint64 MAGNA_CALL span_to_uint_64
    (
        Span span
    )
{
    am_uint64 result = 0;
    am_byte *ptr = span.ptr;
    am_size_t len = span.len;

    while (len--) {
        result = result * 10 + (*ptr++ - '0');
    }

    return result;
}

/**
 * Перевод спана в верхний регистр (в нативной кодировке).
 *
 * @param span Спан.
 * @return Тот же спан.
 */
MAGNA_API Span MAGNA_CALL span_toupper
    (
        Span span
    )
{
    am_byte *ptr = span.ptr;
    am_size_t len = span.len;

    while (len--) {
        *ptr = (am_byte) toupper (*ptr);
        ++ptr;
    }

    return span;
}

/**
 * Перевод спана в нижний регистр (в нативной кодировке).
 *
 * @param span Спан.
 * @return Тот же спан.
 */
MAGNA_API Span MAGNA_CALL span_tolower
    (
        Span span
    )
{
    am_byte *ptr = span.ptr;
    am_size_t len = span.len;

    while (len--) {
        *ptr = (am_byte) tolower (*ptr);
        ++ptr;
    }

    return span;
}

/**
 * Спан пустой?
 *
 * @param span Проверяемый спан.
 * @return Результат проверки.
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
 * @param span Спан.
 * @param value Искомый элемент.
 * @return Указатель на найденный байт либо `NULL`.
 */
MAGNA_API am_byte* MAGNA_CALL span_find_byte
    (
        Span span,
        am_byte value
    )
{
    am_byte *ptr = span.ptr;
    am_size_t len = span.len;

    while (len--) {
        if (*ptr == value) {
            return ptr;
        }
        ++ptr;
    }

    return NULL;
}

/**
 * Заполнение спана указанным значением.
 *
 * @param span Спан.
 * @param value Значение-заполнитель.
 * @return Тот же спан.
 */
MAGNA_API Span MAGNA_CALL span_fill
    (
        Span span,
        am_byte value
    )
{
    am_byte *ptr = span.ptr;
    am_size_t len = span.len;

    while (len--) {
        *ptr++ = value;
    }

    return span;
}

/**
 * Поиск указанного значения в спане с начала.
 *
 * @param span Спан.
 * @param value Искомое значение.
 * @return Найденный индекс либо -1.
 */
MAGNA_API am_ssize_t MAGNA_CALL span_index_of
    (
        Span span,
        am_byte value
    )
{
    am_size_t i;

    for (i = 0; i < span.len; ++i) {
        if (span.ptr[i] == value) {
            return (am_ssize_t) i;
        }
    }

    return -1;
}

/**
 * Поиск указанного значения в спане с конца.
 *
 * @param span Спан.
 * @param value Искомое значение.
 * @return Найденный индекс либо -1.
 */
MAGNA_API am_ssize_t MAGNA_CALL span_last_index_of
    (
        Span span,
        am_byte value
    )
{
    am_ssize_t i;

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
 * @param span Спан.
 * @param start Смещение от начала.
 * @param length Длина среза.
 * @return Обрезанный спан.
 */
MAGNA_API Span MAGNA_CALL span_slice
    (
        Span span,
        am_ssize_t start,
        am_ssize_t length
    )
{
    Span result = span;

    if (length == -1) {
        length = result.len - start;
    }

    if (length + start > span.len) {
        length = span.len - start;
    }

    if (length < 0) {
        length = 0;
    }

    result.ptr += start;
    result.len = (am_size_t) length;

    return result;
}

/**
 * Преобразование спана в строку.
 *
 * @param span Спан для преобразования.
 * @return Размещенная в куче копия строки.
 */
MAGNA_API char* MAGNA_CALL span_to_string
    (
        Span span
    )
{
    char *result = mem_alloc (span.len + 1);
    if (!result) {
        return result;
    }

    if (span.len != 0 && span.ptr != NULL) {
        mem_copy (result, span.ptr, span.len);
    }
    result [span.len] = 0;

    return result;
}

/**
 * Преобразование спана в вектор байтов.
 *
 * @param span Спан для преобразования.
 * @return Размещенный в куче вектор байтов (копия).
 */
MAGNA_API am_byte* MAGNA_CALL span_to_vector
    (
        Span span
    )
{
    am_byte *result = mem_alloc (span.len);

    if (span.len != 0 && span.ptr != NULL) {
        mem_copy (result, span.ptr, span.len);
    }

    return result;
}

/**
 * Совпадает ли начало спана с другим спаном?
 *
 * @param span Проверяемый спан.
 * @param prefix Предполагаемый префикс.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL span_starts_with
    (
        Span span,
        Span prefix
    )
{
    am_bool result = prefix.len <= span.len;
    am_size_t i;

    if (result) {
        for (i = 0; i < prefix.len; ++i) {
            if (span.ptr[i] != prefix.ptr[i]) {
                result = AM_FALSE;
                break;
            }
        }
    }

    return result;
}

/**
 * Совпадает ли конец спана с другим спаном?
 *
 * @param span Проверяемый спан.
 * @param suffix Предполагаемый суффикс.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL span_ends_with
    (
        Span span,
        Span suffix
    )
{
    am_bool result = suffix.len <= span.len;
    am_size_t i;

    if (result) {
        for (i = 0; i < suffix.len; ++i) {
            if (span.ptr[span.len - i - 1] != suffix.ptr [suffix.len - i - 1]) {
                result = AM_FALSE;
                break;
            }
        }
    }

    return result;
}

/**
 * Побайтовое сравнение двух спанов.
 *
 * @param first Первый спан.
 * @param second Второй спан.
 * @return Результат сравнения: < 0, если первый спан меньше,
 * > 0, если первый спан больше и = 0, если содержимое спанов совпадает.
 */
MAGNA_API int MAGNA_CALL span_compare
    (
        Span first,
        Span second
    )
{
    int result;
    am_size_t i;

    for (i = 0; ; ++i) {
        if (i == first.len) {
            if (i == second.len) {
                return 0;
            }

            return -1;
        }

        if (i == second.len) {
            return 1;
        }

        result = first.ptr[i] - second.ptr[i];
        if (result) {
            return result;
        }
    }
}

/**
 * Разбиение по указанному символу.
 *
 * @param span Спан для разбиения.
 * @param array Инициализированный динамический массив для помещения результата.
 * @param value Символ-разделитель.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL span_split_by_char
    (
        Span span,
        SpanArray *array,
        am_byte value
    )
{
    am_size_t left, right;
    am_bool found;
    Span item;

    assert (array != NULL);

    left = 0;
    while (left < span.len) {
        found = AM_FALSE;
        for (right = left; right < span.len; ++right) {
            if (span.ptr[right] == value) {
                if (left != right) {
                    item.ptr = span.ptr + left;
                    item.len = right - left;
                    if (!span_array_push_back (array, item)) {
                        return AM_FALSE;
                    }
                }

                found = AM_TRUE;
                break;
            }
        }

        if (!found) {
            item.ptr = span.ptr + left;
            item.len = span.len - left;
            if (!span_array_push_back (array, item)) {
                return AM_FALSE;
            }

            break;
        }

        left = right + 1;
    }

    return AM_TRUE;
}

/**
 * Разбиение по указанным символам.
 *
 * @param span Спан для разбиения.
 * @param array Инициализированный динамический массив для помещения результата.
 * @param values Указатель на символы-разделители.
 * @param valueCount Количество символов-разделителей.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL span_split_by_chars
    (
        Span span,
        SpanArray *array,
        const am_byte *values,
        am_size_t valueCount
    )
{
    am_size_t left, right, i;
    Span item;
    am_byte c;
    am_bool found;

    assert (array != NULL);
    assert (values != NULL);

    left = 0;
    while (left < span.len) {
        for (right = left; right < span.len; ++right) {
            c = span.ptr[right];
            found = AM_FALSE;
            for (i = 0; i < valueCount; ++i) {
                if (c == values[i]) {
                    found = AM_TRUE;
                    break;
                }
            }

            if (found) {
                if (left != right) {
                    item.ptr = span.ptr + left;
                    item.len = right - left;
                    if (!span_array_push_back (array, item)) {
                        return AM_FALSE;
                    }
                }
                break;
            }
        }

        if (!found) {
            item.ptr = span.ptr + left;
            item.len = span.len - left;
            if (!span_array_push_back (array, item)) {
                return AM_FALSE;
            }

            break;
        }

        left = right + 1;
    }

    return AM_TRUE;
}

static const am_byte* find_one
    (
        const am_byte *from,
        const am_byte *to,
        am_byte value
    )
{
    const am_byte *ptr;

    for (ptr = from; ptr < to; ++ptr) {
        if (*ptr == value) {
            return ptr;
        }
    }

    return NULL;
}

static const am_byte* find_many
    (
        const am_byte *from,
        const am_byte *to,
        const am_byte *values,
        am_size_t valueCount
    )
{
    const am_byte *ptr1, *ptr2 = values + valueCount, *ptr3;

    for (ptr1 = from; ptr1 < to; ++ptr1) {
        ptr3 = find_one (values, ptr2, *ptr1);
        if (ptr3) {
            return ptr1;
        }
    }

    return NULL;
}

/**
 * Разбиение по указанному символу.
 *
 * @param span Спан для разбиения.
 * @param array Статический массив для размещения результатов.
 * @param arraySize Размер статического массива в элементах.
 * @param value Символ-разделитель.
 * @return Количество получившихся фрагметов.
 */
MAGNA_API am_size_t MAGNA_CALL span_split_n_by_char
    (
        Span span,
        Span *array,
        am_size_t arraySize,
        am_byte value
    )
{
    am_size_t count;
    const am_byte *ptr = span.ptr, *end = ptr + span.len, *found;
    Span *item;

    assert (array != NULL);

    for (count = 0; count < arraySize - 1; ) {
        found = find_one (ptr, end, value);
        if (!found) {
            break;
        }

        if (found == ptr) {
            ++ptr;
            continue;
        }

        item = array + count;
        item->ptr = (am_byte*) ptr;
        item->len = found - ptr;
        ptr = found + 1;
        ++count;
    }

    if (ptr != end) {
        item = array + count;
        item->ptr = (am_byte*) ptr;
        item->len = end - ptr;
        ++count;
    }

    return count;
}

/**
 * Разбиение по указанным символам.
 *
 * @param span Спан для разбиения.
 * @param array Статический массив для размещения результата.
 * @param arraySize Размер статического массива в элементах.
 * @param values Символы-разделители.
 * @param valueCount Количество символов-разделителей.
 * @return Количество получившихся фрагментов.
 */
MAGNA_API am_size_t MAGNA_CALL span_split_n_by_chars
    (
        Span span,
        Span *array,
        am_size_t arraySize,
        const am_byte *values,
        am_size_t valueCount
    )
{
    am_size_t count;
    const am_byte *ptr = span.ptr, *end = ptr + span.len, *found;
    Span *item;

    assert (array != NULL);

    for (count = 0; count < arraySize - 1; ) {
        found = find_many (ptr, end, values, valueCount);
        if (!found) {
            break;
        }

        if (found == ptr) {
            ++ptr;
            continue;
        }

        item = array + count;
        item->ptr = (am_byte*) ptr;
        item->len = found - ptr;
        ptr = found + 1;
        ++count;
    }

    if (ptr != end) {
        item = array + count;
        item->ptr = (am_byte*) ptr;
        item->len = end - ptr;
        ++count;
    }

    return count;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
