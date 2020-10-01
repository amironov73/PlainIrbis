// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 * \file intarray.c
 *
 * Простой динамический массив 32-битных чисел (например,
 * для MFN).
 *
 * Владеет собственной памятью. Для освобождения
 * ресурсов используйте `int32_array_free`.
 */

/*=========================================================*/

/**
 * Клонирование массива.
 *
 * @param target Неиницализированный массив назначения.
 * @param source Массив-источник.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL int32_array_clone
    (
        Int32Array *target,
        const Int32Array *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    if (source->len == 0) {
        memset (target, 0, sizeof (Int32Array));
        return AM_TRUE;
    }

    return int32_array_create (target, source->len)
        && int32_array_copy (target, source);
}

/**
 * Копирование массива.
 *
 * @param target Инициализированный массив назначения.
 * @param source Массив-источник.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL int32_array_copy
    (
        Int32Array *target,
        const Int32Array *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    int32_array_truncate (target, 0);

    return int32_array_concat (target, source);
}

/**
 * Добавление одного массива в конец другого.
 *
 * @param target Инициализированный массив назначения.
 * @param source Массив-источник.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL int32_array_concat
    (
        Int32Array *target,
        const Int32Array *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    if (!int32_array_grow (target, target->len + source->len)) {
        return AM_FALSE;
    }

    memcpy (target->ptr + target->len, source->ptr, source->len * sizeof (am_int32));
    target->len += source->len;

    return AM_TRUE;
}

/**
 * Создание массива заданной емкости.
 *
 * @param array Указатель на структуру, подлежащую инициализации.
 * @param capacity Требуемая емкость массива (больше нуля).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL int32_array_create
    (
        Int32Array *array,
        size_t capacity
    )
{
    assert (array != NULL);
    assert (capacity > 0);

    array->len = 0;
    array->capacity = capacity;
    array->ptr = (am_int32*) malloc (capacity * sizeof (am_int32));
    if (array->ptr == NULL) {
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * Освобождение ресурсов, занятых массивом.
 *
 * @param array Массив, подлежащий освобождению.
 */
MAGNA_API void MAGNA_CALL int32_array_free
    (
        Int32Array *array
    )
{
    assert (array != NULL);

    array->len = 0;
    array->capacity = 0;
    if (array->ptr != NULL) {
        free (array->ptr);
        array->ptr = NULL;
    }
}

/**
 * Получение элемента массива по его индексу.
 *
 * @param array Массив.
 * @param index Индекс.
 * @return Элемент массива.
 */
MAGNA_API am_int32 MAGNA_CALL int32_array_get
    (
        const Int32Array *array,
        size_t index
    )
{
    assert (array != NULL);
    assert (index < array->len);

    return array->ptr [index];
}

/**
 * Увеличение емкости массива, если это необходимо.
 * Если массив уже имеет запрошенную или большую емкость,
 * никаких действий не производится.
 *
 * @param array Массив.
 * @param newSize Требуемая емкость массива.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL int32_array_grow
    (
        Int32Array *array,
        size_t newSize
    )
{
    size_t size;
    am_int32 *newPtr;

    assert (array != NULL);
    assert (newSize > 0);
    assert (array->len <= array->capacity);

    if (array->capacity < newSize) {

        size = 2;
        while (size < newSize) {
            size *= 2;
        }
        newSize = size;

        newPtr = malloc (newSize * sizeof (am_int32));
        if (newPtr == NULL) {
            return AM_FALSE;
        }

        if (array->len) {
            assert (array->ptr != NULL);
            memcpy (newPtr, array->ptr, array->len * sizeof(am_int32));
        }

        if (array->ptr) {
            free (array->ptr);
        }

        array->ptr = newPtr;
        array->capacity = newSize;
    }

    return AM_TRUE;
}

/**
 * Уменьшение емкости массива до минимально необходимой.
 * Если массив пустой, возврщается вся память, занятая массивом.
 *
 * @param array Массив.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL int32_array_shrink
    (
        Int32Array *array
    )
{
    am_int32 *newPtr;
    size_t newSize;

    assert (array != NULL);
    assert (array->len <= array->capacity);

    if (array->len != array->capacity) {
        if (array->len == 0) {
            if (array->ptr != NULL) {
                free (array->ptr);
                array->ptr = NULL;
            }

            array->capacity = 0;
        }
        else {
            newSize = array->len * sizeof (am_int32);
            newPtr = (am_int32 *) malloc (newSize);
            if (newPtr == NULL) {
                return AM_FALSE;
            }

            memcpy (newPtr, array->ptr, newSize);
            free (array->ptr);
            array->ptr = newPtr;
            array->capacity = array->len;
        }
    }

    return AM_TRUE;
}

/**
 * Извлечение последнего элемента массива
 * с последующим удалением его из массива.
 * Оставшиеся элементы не перемещаются.
 *
 * @param array Массив.
 * @return Извлеченный элемент.
 * @warning На пустом массиве приводит к неопределенному поведению.
 */
MAGNA_API am_int32 MAGNA_CALL int32_array_pop_back
    (
        Int32Array *array
    )
{
    am_int32 result;

    assert (array != NULL);
    assert (array->len != 0);

    --array->len;
    result = array->ptr [array->len];

    return result;
}

/**
 * Извлечение первого элемента массива
 * с последующим удалением его из массива.
 * Оставшиеся элементы соответственно
 * сдвигаются к началу.
 *
 * @param array Массив.
 * @return Извлеченный элемент.
 * @warning На пустом массиве приводит к неопределенному поведени.
 */
MAGNA_API am_int32 MAGNA_CALL int32_array_pop_front
    (
        Int32Array *array
    )
{
    am_int32 result;

    assert (array != NULL);
    assert (array->len != 0);

    result = array->ptr [0];
    --array->len;
    memmove (array->ptr, array->ptr + 1, array->len * sizeof (am_int32));

    return result;
}

/**
 * Помещение элемента в конец массива.
 * Прочие элементы остаются на своих местах.
 *
 * @param array Массив.
 * @param value Помещаемый элемент.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL int32_array_push_back
    (
        Int32Array *array,
        am_int32 value
    )
{
    assert (array != NULL);

    if (!int32_array_grow (array, array->len + 1)) {
        return AM_FALSE;
    }

    array->ptr [array->len] = value;
    ++array->len;

    return AM_TRUE;
}

/**
 * Помещение элемента в начало массива. Все прочие
 * элементы соответственно сдвигаются к концу массива.
 *
 * @param array Массив.
 * @param value ПОмещаемый элемент.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL int32_array_push_front
    (
        Int32Array *array,
        am_int32 value
    )
{
    assert (array != NULL);

    if (!int32_array_grow (array, array->len + 1)) {
        return AM_FALSE;
    }

    memmove (array->ptr + 1, array->ptr, array->len * sizeof (am_int32));
    array->ptr [0] = value;
    ++array->len;

    return AM_TRUE;
}

/**
 * Установка значения элемента массива.
 *
 * @param array Массив.
 * @param index Индекс.
 * @param value Новое значение для элемента массива.
 */
MAGNA_API void MAGNA_CALL int32_array_set
    (
        Int32Array *array,
        size_t index,
        am_int32 value
    )
{
    assert (array != NULL);
    assert (index < array->len);

    array->ptr [index] = value;
}

/**
 * Удаление элементов массива свеех указанного размера.
 * Емкость массива при этом не уменьшается.
 *
 * @param array Массив.
 * @param newSize Новая длина массива.
 */
MAGNA_API void MAGNA_CALL int32_array_truncate
    (
        Int32Array *array,
        size_t newSize
    )
{
    assert (array != NULL);

    if (newSize < array->len) {
        array->len = newSize;
    }
}

/**
 * Вывод массива в виде текста.
 *
 * @param array Массив.
 * @param buffer Буфер для вывода.
 * @param delimiter Разделитель между элементами массива (может быть NULL).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL int32_array_to_text
    (
        const Int32Array *array,
        struct MagnaBuffer *buffer,
        const am_byte *delimiter
    )
{
    size_t index;
    am_int32 *item;

    assert (array != NULL);
    assert (buffer != NULL);

    item = array->ptr;
    for (index = 0; index < array->len; ++index, ++item) {
        if (!buffer_put_uint_32(buffer, *item)) {
            return AM_FALSE;
        }

        if (delimiter != NULL) {
            if (!buffer_puts (buffer, delimiter)) {
                return AM_TRUE;
            }
        }
    }

    return AM_TRUE;
}

static am_bool format_range
    (
        struct MagnaBuffer *buffer,
        am_uint32 first,
        am_uint32 last
    )
{
    if (first == last) {
        return buffer_put_uint_32 (buffer, first);
    }

    if (first == last - 1) {
        return buffer_put_uint_32 (buffer, first)
            && buffer_puts (buffer, ", ")
            && buffer_put_uint_32 (buffer, last);
    }

    return buffer_put_uint_32 (buffer, first)
        && buffer_putc (buffer, '-')
        && buffer_put_uint_32 (buffer, last);
}

MAGNA_API am_bool MAGNA_CALL int32_array_compress
    (
        const Int32Array *array,
        struct MagnaBuffer *buffer
    )
{
    am_bool first = AM_TRUE;
    am_uint32 previous, last, current;
    size_t index;

    assert (array != NULL);
    assert (buffer != NULL);

    if (array->len == 0) {
        return AM_TRUE;
    }

    last = previous = array->ptr[0];
    for (index = 1; index < array->len; ++index) {
        current = array->ptr[index];
        if (current != last + 1) {
            if (!first) {
                if (!buffer_puts (buffer, ", ")) {
                    return AM_FALSE;
                }
            }

            if (!format_range (buffer, previous, last)) {
                return AM_FALSE;
            }

            previous = current;

            first = AM_FALSE;
        }

        last = current;
    }

    if (!first) {
        if (!buffer_puts (buffer, ", ")) {
            return AM_FALSE;
        }
    }

    return format_range (buffer, previous, last);
}

MAGNA_API am_bool MAGNA_CALL int32_array_parse
    (
        Int32Array *array,
        TextNavigator *nav
    )
{
    assert (array != NULL);
    assert (nav != NULL);

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
