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
 * \file array.c
 *
 * Простой динамический массив.
 *
 * Содержит указатели на элементы (в общем случае могут
 * быть равны `NULL`).
 *
 * Владеет только собственной памятью. Памятью элементов
 * распоряжаются сами элементы. Чтобы разрешить освобождение
 * памяти элементов, необходимо задать `liberator`.
 */

/*=========================================================*/

/**
 * Клонирование (глубокое копирование) массива.
 *
 * @param target Массив назначения.
 * @param source Источник.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_clone
    (
        Array *target,
        const Array *source
    )
{
    void *item;
    am_size_t index;

    assert (target != NULL);
    assert (source != NULL);

    array_truncate (target, 0);
    if (!array_grow (target, source->len)) {
        return AM_FALSE;
    }

    target->cloner = source->cloner;
    target->liberator = source->liberator;

    for (index = 0; index < source->len; ++index) {
        item = source->ptr [index];
        if (source->cloner != NULL) {
            target->ptr [index] = source->cloner (item);
        }
        else {
            target->ptr [index] = item;
        }
    }

    target->len = source->len;

    return AM_TRUE;
}

/**
 * Неглубокое копирование массива.
 *
 * @param target Массив назначения.
 * @param source Источник.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_copy
    (
        Array *target,
        const Array *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    array_truncate (target, 0);

    return array_concat (target, source);
}

/**
 * Добавление массива в конец другого.
 *
 * @param target Массив назначения.
 * @param source Исходный массив.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_concat
    (
        Array *target,
        const Array *source
    )
{
    am_size_t index;

    assert (target != NULL);
    assert (source != NULL);

    if (!array_grow (target, target->len + source->len)) {
        return AM_FALSE;
    }

    for (index = 0; index < source->len; ++index) {
        target->ptr [target->len] = source->ptr [index];
        ++(target->len);
    }

    return AM_TRUE;
}

/**
 * Создание массива заданной емкости.
 *
 * @param array Массив, подлежащий инициализации.
 * @param capacity Требуемая емкость массива.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_create
    (
        Array *array,
        am_size_t capacity
    )
{
    assert (array != NULL);
    assert (capacity > 0);

    array->cloner = NULL;
    array->liberator = NULL;
    array->len = 0;
    array->capacity = capacity;

    array->ptr = (void**) mem_alloc (capacity * sizeof (void*));
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
MAGNA_API void MAGNA_CALL array_free
    (
        Array *array
    )
{
    am_size_t index;

    assert (array != NULL);

    if (array->liberator != NULL) {
        for (index = 0; index < array->len; ++index) {
            array->liberator (array->ptr[index]);
        }
    }

    array->len = 0;
    array->capacity = 0;
    free (array->ptr);
    array->ptr = NULL;
}

/**
 * Получение элемента массива по его индексу.
 *
 * @param array Массив.
 * @param index Индекс.
 * @return Элемент массива.
 */
MAGNA_API void* MAGNA_CALL array_get
    (
        const Array *array,
        am_size_t index
    )
{
    assert (array != NULL);
    assert (index < array->len);

    return array->ptr[index];
}

/**
 * Увеличение емкости массива, если это необходимо.
 * Если массив уже имеет необходимую или большую емкость,
 * никаких действий не производится.
 *
 * @param array Массив, емкость которого необходимо увеличить.
 * @param newSize Требуемая емкость массива.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_grow
    (
        Array *array,
        am_size_t newSize
    )
{
    void **newPtr;

    assert (array != NULL);
    assert (newSize > 0);

    if (newSize > array->capacity) {
        newPtr = (void**) calloc (newSize, sizeof (void*));
        if (newPtr == NULL) {
            return AM_FALSE;
        }

        memcpy (newPtr, array->ptr, array->len * sizeof (void*));
        free (array->ptr);
        array->ptr = newPtr;
        array->capacity = newSize;
    }

    return AM_TRUE;
}

/**
 * Извлечение элемента (с последующим удалением из массива).
 * Оставшиеся элементы не перемещаются.
 *
 * @param array Массив.
 * @return Извлеченный элемент.
 * @warning На пустом массиве приводит к неопределенному поведению.
 */
MAGNA_API void* MAGNA_CALL array_pop_back
    (
        Array *array
    )
{
    void *result;

    assert (array != NULL);
    assert (array->len != 0);

    --(array->len);
    result = array->ptr [array->len];

    return result;
}

/**
 * Извлечение элемента (с последующим удалением из массива)
 * из начала массива. Оставшиеся элементы соответственно
 * сдвигаются к началу.
 *
 * @param array Массив.
 * @return Извлеченный элемент.
 * @warning На пустом массиве приводит к неопределенному поведению.
 */
MAGNA_API void* MAGNA_CALL array_pop_front
    (
        Array *array
    )
{
    am_size_t index;
    void *result;

    assert (array != NULL);
    assert (array->len != 0);

    result = array->ptr [0];
    --(array->len);
    for (index = 0; index < array->len; ++index) {
        array->ptr [index] = array->ptr [index + 1];
    }

    return result;
}

/**
 * Помещение элемента в конец массива.
 *
 * @param array Массив.
 * @param item Помещаемый элемент.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_push_back
    (
        Array *array,
        void *item
    )
{
    assert (array != NULL);

    if (!array_grow (array, array->len + 1)) {
        return AM_FALSE;
    }

    array->ptr [array->len] = item;
    ++(array->len);

    return AM_TRUE;
}

/**
 * Помещение элемента в начало массива.
 *
 * @param array Массив.
 * @param item Помещаемый элемент.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_push_front
    (
        Array *array,
        void *item
    )
{
    am_size_t index;

    assert (array != NULL);

    if (!array_grow (array, array->len + 1)) {
        return AM_FALSE;
    }

    for (index = array->len; index != 0; --index) {
        array->ptr [index] = array->ptr [index - 1];
    }

    array->ptr [0] = item;
    ++(array->len);

    return AM_TRUE;
}

/**
 * Установка элемента массива.
 *
 * @param array Массив.
 * @param index Индекс.
 * @param item Новое значение для элемента массива.
 */
MAGNA_API void MAGNA_CALL array_set
    (
        Array *array,
        am_size_t index,
        void *item
    )
{
    assert (array != NULL);
    assert (index < array->len);

    if (array->liberator != NULL) {
        array->liberator (array->ptr [index]);
    }

    array->ptr [index] = item;
}

/**
 * Удаление элементов массива сверх указанного размера.
 * Емкость массива при этом не уменьшается.
 *
 * @param array Массив.
 * @param newSize Новая длина массива.
 */
MAGNA_API void MAGNA_CALL array_truncate
    (
        Array *array,
        am_size_t newSize
    )
{
    am_size_t index;

    assert (array != NULL);

    if (array->liberator != NULL) {
        for (index = array->len; index < newSize; ++index) {
            array->liberator (array->ptr [index]);
            array->ptr [index] = NULL;
        }
    }

    array->len = newSize;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
