// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 * \file varray.c
 *
 * Простой динамический массив, хранящий собственно объекты.
 *
 * Владеет только собственной памятью. Памятью элементов
 * распоряжаются сами элементы. Чтобы разрешить освобождение
 * памяти элементов, необходимо задать `liberator`.
 */

/*=========================================================*/

/**
 * Инициализация пустого массива.
 *
 * @param array Указатель на неинициализированную структуру.
 * @param itemSize Размер элемента в байтах.
 */
MAGNA_API void MAGNA_CALL varray_init
    (
        ValueArray *array,
        am_size_t itemSize
    )
{
    assert (array != NULL);
    assert (itemSize != 0);

    array->ptr = NULL;
    array->itemSize = itemSize;
    array->len = 0;
    array->capacity = 0;
    array->offset = 0;
    array->liberator = NULL;
}

/**
 * Создание пустого массива с указанной емкостью.
 *
 * @param array Указатель на неинициализированную структуру.
 * @param itemSize Размер элемента в байтах.
 * @param capacity Начальная емкость массива.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL varray_create
    (
        ValueArray *array,
        am_size_t itemSize,
        am_size_t capacity
    )
{
    assert (array != NULL);
    assert (itemSize != 0);
    assert (capacity != 0);

    array->itemSize = itemSize;
    array->len = 0;
    array->capacity = capacity;
    array->offset = 0;
    array->liberator = NULL;
    array->ptr = mem_alloc (capacity * itemSize);

    return (array->ptr != NULL);
}

/**
 * Освобождение ресурсов, занятых массивом.
 *
 * @param array Массив, подлежащий освобождению.
 */
MAGNA_API void MAGNA_CALL varray_free
    (
        ValueArray *array
    )
{
    am_size_t index;
    am_byte *ptr;

    assert (array != NULL);

    if (array->liberator != NULL) {
        ptr = array->ptr;
        for (index = 0; index < array->len; ++index) {
            array->liberator (ptr);
            ptr += array->itemSize;
        }
    }

    array->len = 0;
    array->capacity = 0;
    array->offset = 0;
    mem_free (array->ptr);
    array->ptr = NULL;
}

/**
 * Получение элемента массива по его индексу.
 *
 * @param array Массив.
 * @param index Индекс.
 * @return Указатель на элемент массива.
 */
MAGNA_API MAGNA_INLINE void* MAGNA_CALL varray_get
    (
        const ValueArray *array,
        am_size_t index
    )
{
    assert (array != NULL);
    assert (index < array->len);

    return array->ptr + (index + array->offset) * array->itemSize;
}

/**
 * Присваивание элемента массива по его индексу.
 *
 * @param array Массив.
 * @param index Индекс.
 * @param value Указатель на новое значение элемента массива.
 */
MAGNA_API void MAGNA_CALL varray_set
    (
        ValueArray *array,
        am_size_t index,
        void *value
    )
{
    assert (array != NULL);
    assert (index < array->len);
    assert (value != NULL);

    mem_copy
        (
            array->ptr + (index + array->offset) * array->itemSize,
            value,
            array->itemSize
        );
}

/**
 * Увеличение емкости массива, если это необходимо.
 * Если массив уже имеет необходимую или большую емкость,
 * никаких действий не производится.
 *
 * @param array Массив.
 * @param newSize Требуемая емкость в элементах.
 * @return Признак успешности операции.
 */
MAGNA_API am_bool MAGNA_CALL varray_grow
    (
        ValueArray *array,
        am_size_t newSize
    )
{
    am_byte *newPtr;
    am_size_t size;

    assert (array != NULL);
    assert (newSize != 0);

    if ((newSize > array->len)
        && (newSize <= array->capacity)
        && (array->offset != 0)) {
        /* У нас есть место в массиве,
         * просто нужно сдвинуть элементы к началу */
        if (array->len != 0) {
            memmove
                (
                    array->ptr,
                    array->ptr + array->offset * array->itemSize,
                    array->len * array->itemSize
                );
        }
        array->offset = 0;
    }

    if (newSize > array->capacity) {
        /* Округляем к ближайшей степени двойки */
        size = 2;
        while (size < newSize) {
            size *= 2;
        }

        newSize = size;

        newPtr = (am_byte*) mem_alloc (newSize * array->itemSize);
        if (newPtr == NULL) {
            return AM_FALSE;
        }

        if (array->ptr != NULL) {
            mem_copy
                (
                    newPtr,
                    array->ptr + array->offset * array->itemSize,
                    array->len * array->itemSize
                );
            mem_free (array->ptr);
        }

        array->ptr = newPtr;
        array->capacity = newSize;
    }

    return AM_TRUE;
}

/**
 * Извлечение последнего элемента (с последующим удалением из массива).
 * Оставшиеся элементы не перемещаются.
 *
 * @param array Массив.
 * @return Указатель на извлеченный элемент.
 * @warning На пустом массиве приводит к неопределенному поведению.
 */
MAGNA_API void* MAGNA_CALL varray_pop_back
    (
        ValueArray *array
    )
{
    am_byte *result;

    assert (array != NULL);
    assert (array->len != 0);

    if (--array->len == 0) {
        array->offset = 0;
    }
    result = array->ptr + (array->len + array->offset) * array->itemSize;

    return result;
}

/**
 * Извлечение первого элемента (с последующим удалением из массива).
 * Оставшиеся элементы соответственно сдвигаются к началу.
 *
 * @param array Массив.
 * @return Указатель на извлеченный элемент.
 * @warning На пустом массиве приводит к неопределенному поведению.
 */
MAGNA_API void* MAGNA_CALL varray_pop_front
    (
        ValueArray *array
    )
{
    am_byte *result;

    assert (array != NULL);
    assert (array->len != 0);

    result = array->ptr + array->offset * array->itemSize;
    ++array->offset;
    if (--array->len == 0) {
        array->offset = 0;
    }

    return result;
}

/**
 * Помещение элемента в начало массива.
 *
 * @param array Массив.
 * @param item Указатель на помещаемый элемент.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL varray_push_back
    (
        ValueArray *array,
        void *item
    )
{
    am_byte *ptr;

    assert (array != NULL);
    assert (item != NULL);

    if (!varray_grow (array, array->len + 1)) {
        return AM_FALSE;
    }

    ptr = array->ptr + (array->len + array->offset) * array->itemSize;
    ++array->len;
    mem_copy (ptr, item, array->itemSize);

    return AM_TRUE;
}

/**
 * Выделение места под элемент в конце массива.
 *
 * @param array Массив.
 * @return Указатель на выделенное место либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL varray_emplace_back
    (
        ValueArray *array
    )
{
    am_byte *result;

    assert (array != NULL);

    if (!varray_grow (array, array->len + 1)) {
        return NULL;
    }

    result = array->ptr + (array->len + array->offset) * array->itemSize;
    ++array->len;

    return result;
}

/**
 * Помещение элемента в начало массива.
 * Остальные элементы соответственно сдвигаются к концу массива.
 *
 * @param array Массив.
 * @param item Указатель на помещаемый элемент.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL varray_push_front
    (
        ValueArray *array,
        void *item
    )
{
    am_byte *ptr;

    assert (array != NULL);
    assert (item != NULL);

    if (array->offset != 0) {
        --array->offset;
        ptr = array->ptr + array->offset * array->itemSize;
    }
    else {
        if (!varray_grow (array, array->len + 1)) {
            return AM_FALSE;
        }

        ptr = array->ptr;
        if (array->len != 0) {
            memmove(ptr + array->itemSize, ptr, array->len * array->itemSize);
        }
    }

    mem_copy (ptr, item, array->itemSize);
    ++array->len;

    return AM_TRUE;
}

/**
 * Удаление из массива элементов сверх указанного размера.
 *
 * @param array Массив.
 * @param newSize Новая длина массива (должна быть не больше старой).
 */
MAGNA_API void MAGNA_CALL varray_truncate
    (
        ValueArray *array,
        am_size_t newSize
    )
{
    assert (array != NULL);
    assert (newSize <= array->len);

    if (newSize == 0) {
        array->offset = 0;
    }

    array->len = newSize;
}

/**
 * Клонирование (глубокое копирование массива).
 *
 * @param target Массив назначения (инициализированный).
 * @param source Массив-источник.
 * @param cloner Функция, клонирующая элементы (опционально).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL varray_clone
    (
        ValueArray *target,
        const ValueArray *source,
        Cloner cloner
    )
{
    am_byte *sourceItem, *targetItem;
    am_size_t index;

    assert (target != NULL);
    assert (source != NULL);
    assert (source->itemSize == target->itemSize);

    varray_truncate (target, 0);
    target->offset = 0;
    if (!varray_grow (target, source->len)) {
        return AM_FALSE;
    }

    for (index = 0; index < source->len; ++index) {
        sourceItem = source->ptr + (index + source->offset) * source->itemSize;
        targetItem = target->ptr + index * source->itemSize;
        if (cloner != NULL) {
            sourceItem = (am_byte*) cloner (sourceItem);
        }

        mem_copy (targetItem, sourceItem, source->itemSize);
    }

    target->len = source->len;

    return AM_TRUE;
}

/**
 * Неглубокое копирование массива.
 *
 * @param target Массив назначения (инициализированный).
 * @param source Массив-источник.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL varray_copy
    (
        ValueArray *target,
        const ValueArray *source
    )
{
    assert (target != NULL);
    assert (source != NULL);
    assert (source->itemSize == target->itemSize);

    varray_clear (target);

    return varray_concat (target, source);
}

/**
 * Добавление одного массива в конец другого (неглубокое копирование).
 *
 * @param target Массив назначения (инициализированный).
 * @param source Массив-источник.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL varray_concat
    (
        ValueArray *target,
        const ValueArray *source
    )
{
    am_size_t index;
    am_byte *sourceItem, *targetItem;

    assert (target != NULL);
    assert (source != NULL);
    assert (source->itemSize == target->itemSize);

    if (!varray_grow (target, target->len + source->len)) {
        return AM_FALSE;
    }

    for (index = 0; index < source->len; ++index) {
        sourceItem = source->ptr + (index + source->offset) * source->itemSize;
        targetItem = target->ptr + (target->len + target->offset) * target->itemSize;
        mem_copy (targetItem, sourceItem, source->itemSize);
        ++target->len;
    }

    return AM_TRUE;
}

/**
 * Удаление всех элементов массива.
 * Память остается занятой.
 *
 * @param array Массив.
 */
MAGNA_API void MAGNA_CALL varray_clear
    (
        ValueArray *array
    )
{
    assert (array != NULL);

    array->len = 0;
    array->offset = 0;
}

/**
 * Удаление элемента из массива по указателю.
 *
 * @param array Массив.
 * @param item Указатель на удалемый элемент.
 * @warning Указатель вне пределов массива приводит к неопределенному поведению.
 */
MAGNA_API void MAGNA_CALL varray_remove_item
    (
        ValueArray *array,
        void *item
    )
{
    assert (array != NULL);
    assert (item != NULL);
    assert (array->len != 0);

    if (item == (array->ptr + array->offset * array->itemSize)) {
        /* Элемент в голове */
        ++array->offset;
        --array->len;
    }
    else if (item == (array->ptr + (array->offset + array->len - 1) * array->itemSize)) {
        /* Элемент в хвосте */
        --array->len;
    }
    else {
        /* Элемент где-то в середине */
        memmove (item, item + array->itemSize, array->itemSize);
        --array->len;
    }
}

/**
 * Удаление элемента из массива по его индексу.
 *
 * @param array Массив.
 * @param index Индекс удаляемого элемента.
 * @warning Индекс за пределами массива приводит к неопределенному поведению.
 */
MAGNA_API void MAGNA_CALL varray_remove_index
    (
        ValueArray *array,
        am_size_t index
    )
{
    am_byte *item;

    assert (array != NULL);
    assert (index < array->len);

    item = varray_get (array, index);
    varray_remove_item (array, item);
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
