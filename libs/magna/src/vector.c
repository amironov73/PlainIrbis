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
 * \file vector.c
 *
 * Вектор -- простой динамический вектор.
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
 * Клонирование (глубокое копирование) вектора.
 *
 * @param target Вектор назначения.
 * @param source Источник.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL vector_clone
    (
        Vector *target,
        const Vector *source
    )
{
    void *item;
    size_t index;

    assert (target != NULL);
    assert (source != NULL);

    vector_truncate (target, 0, NULL);
    if (!vector_grow (target, source->len)) {
        return AM_FALSE;
    }

    for (index = 0; index < source->len; ++index) {
        item = source->ptr [index];
        target->ptr [index] = item;
    }

    target->len = source->len;

    return AM_TRUE;
}

/**
 * Неглубокое копирование вектора.
 *
 * @param target Вектор назначения.
 * @param source Источник.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL vector_copy
    (
        Vector *target,
        const Vector *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    vector_truncate (target, 0, NULL);

    return vector_concat (target, source);
}

/**
 * Добавление одного вектораа в конец другого.
 *
 * @param target Вектор назначения.
 * @param source Исходный вектор.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL vector_concat
    (
        Vector *target,
        const Vector *source
    )
{
    size_t index;

    assert (target != NULL);
    assert (source != NULL);

    if (!vector_grow(target, target->len + source->len)) {
        return AM_FALSE;
    }

    for (index = 0; index < source->len; ++index) {
        target->ptr [target->len] = source->ptr [index];
        ++(target->len);
    }

    return AM_TRUE;
}

/**
 * Создание вектора заданной емкости.
 *
 * @param vector Вектор, подлежащий инициализации.
 * @param capacity Требуемая емкость вектора.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL vector_create
    (
        Vector *vector,
        size_t capacity
    )
{
    assert (vector != NULL);
    assert (capacity != 0);

    vector->len = 0;
    vector->capacity = capacity;

    vector->ptr = (void**) mem_alloc (capacity * sizeof (void*));
    if (vector->ptr == NULL) {
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * Освобождение ресурсов, занятых вектором.
 *
 * @param vector Вектор, подлежащий освобождению.
 * @param liberator Функция для освобождения памяти, занятой элементом вектора.
 */
MAGNA_API void MAGNA_CALL vector_destroy
    (
        Vector *vector,
        Liberator liberator
    )
{
    size_t index;
    void *ptr;

    assert (vector != NULL);

    if (liberator != NULL) {
        for (index = 0; index < vector->len; ++index) {
            ptr = vector->ptr [index];
            if (ptr != NULL) {
                liberator (ptr);
            }
        }
    }

    mem_free (vector->ptr);
    vector->ptr = NULL;
    vector->len = 0;
    vector->capacity = 0;
}

/**
 * Получение элемента вектора по его индексу.
 *
 * @param vector Вектор.
 * @param index Индекс.
 * @return Элемент вектора.
 */
MAGNA_API MAGNA_INLINE void* MAGNA_CALL vector_get
    (
        const Vector *vector,
        size_t index
    )
{
    assert (vector != NULL);
    assert (index < vector->len);

    return vector->ptr [index];
}

/**
 * Увеличение емкости вектора, если это необходимо.
 * Если вектор уже имеет необходимую или большую емкость,
 * никаких действий не производится.
 *
 * @param vector Вектор, емкость которого необходимо увеличить.
 * @param newSize Требуемая емкость вектора.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL vector_grow
    (
        Vector *vector,
        size_t newSize
    )
{
    void **newPtr;

    assert (vector != NULL);
    assert (newSize > 0);

    if (newSize > vector->capacity) {
        newPtr = (void**) mem_alloc (newSize * sizeof (void*));
        if (newPtr == NULL) {
            return AM_FALSE;
        }

        mem_copy (newPtr, vector->ptr, vector->len * sizeof (void*));
        mem_free (vector->ptr);
        vector->ptr = newPtr;
        vector->capacity = newSize;
    }

    return AM_TRUE;
}

/**
 * Извлечение последнего элемента (с последующим удалением из вектора).
 * Оставшиеся элементы не перемещаются.
 *
 * @param vector Вектор.
 * @return Извлеченный элемент.
 * @warning На пустом векторе приводит к неопределенному поведению.
 */
MAGNA_API void* MAGNA_CALL vector_pop_back
    (
        Vector *vector
    )
{
    void *result;

    assert (vector != NULL);
    assert (vector->len != 0);

    --(vector->len);
    result = vector->ptr [vector->len];

    return result;
}

/**
 * Извлечение элемента (с последующим удалением из вектора)
 * из начала вектора. Оставшиеся элементы соответственно
 * сдвигаются к началу.
 *
 * @param vector Вектор.
 * @return Извлеченный элемент.
 * @warning На пустом векторе приводит к неопределенному поведению.
 */
MAGNA_API void* MAGNA_CALL vector_pop_front
    (
        Vector *vector
    )
{
    size_t index;
    void *result;

    assert (vector != NULL);
    assert (vector->len != 0);

    result = vector->ptr [0];
    --(vector->len);
    for (index = 0; index < vector->len; ++index) {
        vector->ptr [index] = vector->ptr [index + 1];
    }

    return result;
}

/**
 * Помещение элемента в конец вектора.
 *
 * @param vector Вектор.
 * @param item Помещаемый элемент.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL vector_push_back
    (
        Vector *vector,
        void *item
    )
{
    assert (vector != NULL);

    if (!vector_grow(vector, vector->len + 1)) {
        return AM_FALSE;
    }

    vector->ptr [vector->len] = item;
    ++(vector->len);

    return AM_TRUE;
}

/**
 * Помещение элемента в начало вектора.
 * Остальные элементы соответственно сдвигаются к концу вектора.
 *
 * @param vector Вектор.
 * @param item Помещаемый элемент.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL vector_push_front
    (
        Vector *vector,
        void *item
    )
{
    size_t index;

    assert (vector != NULL);

    if (!vector_grow(vector, vector->len + 1)) {
        return AM_FALSE;
    }

    for (index = vector->len; index != 0; --index) {
        vector->ptr [index] = vector->ptr [index - 1];
    }

    vector->ptr [0] = item;
    ++(vector->len);

    return AM_TRUE;
}

/**
 * Установка элемента вектора.
 *
 * @param vector Вектор.
 * @param index Индекс.
 * @param item Новое значение для элемента вектора.
 * @param liberator Функция для освобождения ресурсов, занятых элементом массива.
 */
MAGNA_API void MAGNA_CALL vector_set
    (
        Vector *vector,
        size_t index,
        void *item,
        Liberator liberator
    )
{
    void *ptr;

    assert (vector != NULL);
    assert (index < vector->len);

    if (liberator != NULL) {
        ptr = vector->ptr [index];
        if (ptr != NULL) {
            liberator (ptr);
        }
    }

    vector->ptr [index] = item;
}

/**
 * Удаление элементов вектора сверх указанного размера.
 * Емкость вектора при этом не уменьшается.
 *
 * @param vector Вектор.
 * @param newSize Новая длина вектора.
 */
MAGNA_API void MAGNA_CALL vector_truncate
    (
        Vector *vector,
        size_t newSize,
        Liberator liberator
    )
{
    size_t index;
    void *ptr;

    assert (vector != NULL);

    if (liberator != NULL) {
        for (index = vector->len; index < newSize; ++index) {
            ptr = vector->ptr [index];
            if (ptr != NULL) {
                liberator (ptr);
            }
            vector->ptr [index] = NULL;
        }
    }

    vector->len = newSize;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
