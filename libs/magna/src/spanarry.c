/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

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
 * \file spanarra.c
 *
 * Простой динамический массив фрагметов (например,
 * для разбиения строки).
 *
 * Владеет собственной памятью. Для освобождения
 * ресурсов используйте `span_array_free`.
 */

/*=========================================================*/

/**
 * Клонирование массива.
 *
 * @param target Неиницализированный массив назначения.
 * @param source Массив-источник.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL span_array_clone
    (
        SpanArray *target,
        const SpanArray *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    if (source->len == 0) {
        memset (target, 0, sizeof (SpanArray));
        return AM_TRUE;
    }

    return span_array_create (target, source->len)
           && span_array_copy (target, source);
}

/**
 * Копирование массива.
 *
 * @param target Инициализированный массив назначения.
 * @param source Массив-источник.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL span_array_copy
    (
        SpanArray *target,
        const SpanArray *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    span_array_truncate (target, 0);

    return span_array_concat (target, source);
}

/**
 * Добавление одного массива в конец другого.
 *
 * @param target Инициализированный массив назначения.
 * @param source Массив-источник.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL span_array_concat
    (
        SpanArray *target,
        const SpanArray *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    if (!span_array_grow (target, target->len + source->len)) {
        return AM_FALSE;
    }

    memcpy (target->ptr + target->len, source->ptr, source->len * sizeof (Span));
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
MAGNA_API am_bool MAGNA_CALL span_array_create
    (
        SpanArray *array,
        am_size_t capacity
    )
{
    assert (array != NULL);
    assert (capacity > 0);

    array->len = 0;
    array->capacity = capacity;
    array->ptr = (Span*) malloc (capacity * sizeof (Span));
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
MAGNA_API void MAGNA_CALL span_array_free
    (
        SpanArray *array
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
MAGNA_API Span MAGNA_CALL span_array_get
    (
        const SpanArray *array,
        am_size_t index
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
MAGNA_API am_bool MAGNA_CALL span_array_grow
    (
        SpanArray *array,
        am_size_t newSize
    )
{
    am_size_t size;
    Span *newPtr;

    assert (array != NULL);
    assert (newSize > 0);
    assert (array->len <= array->capacity);

    if (array->capacity < newSize) {

        size = 2;
        while (size < newSize) {
            size *= 2;
        }
        newSize = size;

        newPtr = malloc (newSize * sizeof(Span));
        if (newPtr == NULL) {
            return AM_FALSE;
        }

        if (array->len) {
            assert (array->ptr != NULL);
            memcpy (newPtr, array->ptr, array->len * sizeof(Span));
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
MAGNA_API am_bool MAGNA_CALL span_array_shrink
    (
        SpanArray *array
    )
{
    Span *newPtr;
    am_size_t newSize;

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
            newSize = array->len * sizeof (Span);
            newPtr = (Span *) malloc (newSize);
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
MAGNA_API Span MAGNA_CALL span_array_pop_back
    (
        SpanArray *array
    )
{
    Span result;

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
MAGNA_API Span MAGNA_CALL span_array_pop_front
    (
        SpanArray *array
    )
{
    Span result;

    assert (array != NULL);
    assert (array->len != 0);

    result = array->ptr [0];
    --array->len;
    memmove (array->ptr, array->ptr + 1, array->len * sizeof (Span));

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
MAGNA_API am_bool MAGNA_CALL span_array_push_back
    (
        SpanArray *array,
        Span value
    )
{
    assert (array != NULL);

    if (!span_array_grow (array, array->len + 1)) {
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
MAGNA_API am_bool MAGNA_CALL span_array_push_front
    (
        SpanArray *array,
        Span value
    )
{
    assert (array != NULL);

    if (!span_array_grow (array, array->len + 1)) {
        return AM_FALSE;
    }

    memmove (array->ptr + 1, array->ptr, array->len * sizeof (Span));
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
MAGNA_API void MAGNA_CALL span_array_set
    (
        SpanArray *array,
        am_size_t index,
        Span value
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
MAGNA_API void MAGNA_CALL span_array_truncate
    (
        SpanArray *array,
        am_size_t newSize
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
MAGNA_API am_bool MAGNA_CALL span_array_to_text
    (
        const SpanArray *array,
        struct MagnaBuffer *buffer,
        const am_byte *delimiter
    )
{
    am_size_t index;
    Span *span;

    assert (array != NULL);
    assert (buffer != NULL);

    span = array->ptr;
    for (index = 0; index < array->len; ++index, ++span) {
        if (!buffer_write (buffer, span->ptr, span->len)) {
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

/**
 * Формирование массива из указателей на строки.
 *
 * @param array Неинициализированный массив.
 * @param text Массив указателей на строки.
 * @param count Количество указателей.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL span_array_from_text
    (
        SpanArray *array,
        const char **text,
        am_size_t count
    )
{
    am_size_t index;
    Span *span;

    assert (array != NULL);

    if (!span_array_create (array, count)) {
        return AM_FALSE;
    }

    span = array->ptr;
    for (index = 0; index < count; ++index, ++span) {
        *span = span_from_text (text [index]);
    }

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
