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
 * Простой динамический массив, хранящий собственно объекты.
 *
 * Владеет только собственной памятью. Памятью элементов
 * распоряжаются сами элементы.
 */

/*=========================================================*/

/**
 * Инициализация пустого массива.
 * Размер элемента округляется вверх по модулю 4
 * с целью оптимизации доступа к памяти.
 *
 * @param array Указатель на неинициализированную структуру.
 * @param itemSize Размер элемента в байтах.
 */
MAGNA_API void MAGNA_CALL array_init
    (
        Array *array,
        size_t itemSize
    )
{
    assert (array != NULL);
    assert (itemSize != 0);

    /* Округляем вверх по модулю 4 */
    array->itemSize = (itemSize + 3u) & ~3u;
    array->ptr = NULL;
    array->len = 0;
    array->capacity = 0;
    array->offset = 0;
}

/**
 * Создание пустого массива с указанной емкостью.
 *
 * @param array Указатель на неинициализированную структуру.
 * @param itemSize Размер элемента в байтах.
 * @param capacity Начальная емкость массива.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_create
    (
        Array *array,
        size_t itemSize,
        size_t capacity
    )
{
    assert (array != NULL);
    assert (itemSize != 0);
    assert (capacity != 0);

    array->itemSize = itemSize;
    array->len = 0;
    array->capacity = capacity;
    array->offset = 0;
    array->ptr = mem_alloc (capacity * itemSize);

    return (array->ptr != NULL);
}

/**
 * Освобождение ресурсов, занятых массивом.
 *
 * @param array Массив, подлежащий освобождению.
 * @param liberator Функция для освобождения памяти, занятой элементом массива (может быть `NULL`).
 */
MAGNA_API void MAGNA_CALL array_destroy
    (
        Array *array,
        Liberator liberator
    )
{
    size_t index;
    void *ptr;

    assert (array != NULL);

    if (liberator != NULL) {
        for (index = 0; index < array->len; ++index) {
            ptr = array_get(array, index);
            liberator(ptr);
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
MAGNA_API MAGNA_INLINE void* MAGNA_CALL array_get
    (
        const Array *array,
        size_t index
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
MAGNA_API void MAGNA_CALL array_set
    (
        Array *array,
        size_t index,
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
 * @return Признак успешного операции.
 */
MAGNA_API am_bool MAGNA_CALL array_grow
    (
        Array *array,
        size_t newSize
    )
{
    am_byte *newPtr;
    size_t size;

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
MAGNA_API void* MAGNA_CALL array_pop_back
    (
        Array *array
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
MAGNA_API void* MAGNA_CALL array_pop_front
    (
        Array *array
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
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_push_back
    (
        Array *array,
        void *item
    )
{
    am_byte *ptr;

    assert (array != NULL);
    assert (item != NULL);

    if (!array_grow(array, array->len + 1)) {
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
MAGNA_API void* MAGNA_CALL array_emplace_back
    (
        Array *array
    )
{
    am_byte *result;

    assert (array != NULL);

    if (!array_grow (array, array->len + 1)) {
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
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_push_front
    (
        Array *array,
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
        if (!array_grow(array, array->len + 1)) {
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
MAGNA_API void MAGNA_CALL array_truncate
    (
        Array *array,
        size_t newSize
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
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_clone
    (
        Array *target,
        const Array *source,
        Cloner cloner
    )
{
    am_byte *sourceItem, *targetItem;
    size_t index;

    assert (target != NULL);
    assert (source != NULL);
    assert (source->itemSize == target->itemSize);

    array_truncate(target, 0);
    target->offset = 0;
    if (!array_grow(target, source->len)) {
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
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_copy
    (
        Array *target,
        const Array *source
    )
{
    assert (target != NULL);
    assert (source != NULL);
    assert (source->itemSize == target->itemSize);

    array_clear(target);

    return array_concat(target, source);
}

/**
 * Добавление одного массива в конец другого (неглубокое копирование).
 *
 * @param target Массив назначения (инициализированный).
 * @param source Массив-источник.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_concat
    (
        Array *target,
        const Array *source
    )
{
    size_t index;
    am_byte *sourceItem, *targetItem;

    assert (target != NULL);
    assert (source != NULL);
    assert (source->itemSize == target->itemSize);

    if (!array_grow(target, target->len + source->len)) {
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
MAGNA_API void MAGNA_CALL array_clear
    (
        Array *array
    )
{
    assert (array != NULL);

    array->len = 0;
    array->offset = 0;
}

/**
 * Вставка в массив элемента непосредственно перед указанным.
 *
 * @param array Массив.
 * @param index Индекс.
 * @param item Размещаемый элемент (происходит копирование).
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL array_insert_at
    (
        Array *array,
        size_t index,
        void *item
    )
{
    assert (array != NULL);
    assert (index < array->len);
    assert (item != NULL);

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Вставка в массив элемента непосредственно перед указанным.
 *
 * @param array Массив.
 * @param index Индекс.
 * @return Указатель на место для размещения элемента либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL array_emplace_at
    (
        Array *array,
        size_t index
    )
{
    assert (array != NULL);
    assert (index < array->len);

    /* TODO: implement */

    return NULL;
}

/**
 * Удаление элемента из массива по указателю.
 *
 * @param array Массив.
 * @param item Указатель на удалемый элемент.
 * @warning Указатель вне пределов массива приводит к неопределенному поведению.
 */
MAGNA_API void MAGNA_CALL array_remove_item
    (
        Array *array,
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
        memmove (item, ((am_byte*) item) + array->itemSize, array->itemSize);
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
MAGNA_API void MAGNA_CALL array_remove_index
    (
        Array *array,
        size_t index
    )
{
    am_byte *item;

    assert (array != NULL);
    assert (index < array->len);

    item = array_get(array, index);
    array_remove_item(array, item);
}

static void va_swap
    (
        am_byte *left,
        am_byte *right,
        size_t size
    )
{
    size_t index;
    am_byte temp;

    /* TODO: optimize */

    for (index = 0; index < size; ++index) {
        temp = left[index];
        left[index] = right[index];
        right[index] = temp;
    }
}

static void va_qsort
    (
        am_byte *ptr,
        ssize_t left,
        ssize_t right,
        size_t size,
        Comparer comparer,
        const void *data
    )
{
    size_t i, j, pivot;

    if (left < right) {
        pivot = left;
        i = left;
        j = right;

        while(i < j) {
            while (comparer (ptr + i * size, ptr + pivot * size, data) <= 0
                && (((ssize_t)i) < right)) {
                i++;
            }

            while (comparer (ptr + j * size, ptr + pivot * size, data) > 0) {
                j--;
            }

            if (i < j) {
                va_swap (ptr + i * size, ptr + j * size, size);
            }
        }

        va_swap (ptr + pivot * size, ptr + j * size, size);
        va_qsort (ptr, left, j-1, size, comparer, data);
        va_qsort (ptr, j+1, right, size, comparer, data);
    }
}

/**
 * Сортировка массива.
 *
 * @param array Массив.
 * @param comparer Функция сравнения.
 * @param data Произвольные пользовательские данные.
 */
MAGNA_API void MAGNA_CALL array_sort
    (
        Array *array,
        Comparer comparer,
        const void *data
    )
{
    am_byte *ptr;

    assert (array != NULL);
    assert (comparer != NULL);

    if (array->len == 0) {
        return;
    }

    ptr = array->ptr + array->offset * array->itemSize;
    va_qsort (ptr, 0, array->len - 1, array->itemSize, comparer, data);
}

static void* va_bsearch
    (
        const am_byte *ptr,
        const void *value,
        ssize_t left,
        ssize_t right,
        size_t size,
        Comparer comparer,
        const void *data
    )
{
    ssize_t middle;
    const am_byte *pmiddle;
    int rc;

    if (right >= left) {
        middle = left + (right - left) / 2;
        pmiddle = ptr + middle * size;
        rc = comparer (pmiddle, value, data);

        /* Если сразу попали куда надо */
        if (rc == 0) {
            return (void*) pmiddle;
        }

        /* Если элемент посередине оказался меньше,
           ищем в левом подмассиве */
        if (rc > 0) {
            return va_bsearch (ptr, value, left, middle - 1, size, comparer, data);
        }

        /* Иначе ищем в правом подмассиве */
        return va_bsearch (ptr, value, middle + 1, right, size, comparer, data);
    }

    /* Сюда мы попадаем, если ничего не найдено */

    return NULL;
}

/**
 * Бинарный поиск в массиве.
 *
 * @param array Массив.
 * @param value Искомое значение.
 * @param comparer Функция сравнения.
 * @param data Произвольные пользовательские данные.
 * @return Указатель на найденный элемент либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL array_bsearch
    (
        Array *array,
        const void *value,
        Comparer comparer,
        const void *data
    )
{
    am_byte *ptr;

    assert (array != NULL);
    assert (value != NULL);
    assert (comparer != NULL);

    if (array->len == 0) {
        return NULL;
    }

    ptr = array->ptr + array->offset * array->itemSize;
    return va_bsearch
        (
            ptr,
            value,
            0,
            array->len - 1,
            array->itemSize,
            comparer,
            data
        );
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
