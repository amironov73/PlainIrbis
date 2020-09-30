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

#define HEADER_SIZE (2 * sizeof (void*))
#define DATA_SIZE(__itemSize) ((__itemSize) + HEADER_SIZE)
#define ITEM_TO_DATA(__i) (((am_byte*)(__i)) + HEADER_SIZE)
#define DATA_TO_ITEM(__d) ((ListItem*)(((am_byte*)(__d)) - HEADER_SIZE))

/*=========================================================*/

/**
 * \file list.c
 *
 * Простой двусвязный список.
 *
 * Содержит элементы плюс указатели на следующий и предыдущий.
 *
 * Владеет только собственной памятью. Памятью элементов
 * распоряжаются сами элементы.
 */

/*=========================================================*/

/**
 * Инициализация двусвязного списка.
 *
 * @param list Указатель на неинициализированную структуру.
 * @param itemSize Размер данных элемента
 * (не учитывает указатели вперед и назад).
 */
MAGNA_API void MAGNA_CALL list_init
    (
        LinkedList *list,
        am_size_t itemSize
    )
{
    assert (list != NULL);

    list->first = list->last = NULL;
    list->itemSize = itemSize;
}

/**
 * Освобождение ресурсов, занятых списком, удаление всех элементов.
 *
 * @param list Список.
 * @param liberator Функция освобождения ресурсов,
 * занятых элементом списка (опционально).
 */
MAGNA_API void MAGNA_CALL list_clear
    (
        LinkedList *list,
        Liberator liberator
    )
{
    ListItem *item, *next;
    void *ptr;

    assert (list != NULL);

    for (item = list->first; item != NULL; )
    {
        next = item->next;
        if (liberator != NULL) {
            ptr = ITEM_TO_DATA (item);
            liberator (ptr);
        }

        mem_free (item);
        item = next;
    }

    list->first = NULL;
    list->last = NULL;
}

/**
 * Добавление элемента в конец списка.
 *
 * @param list Список.
 * @param data Указатель на добавляемые данные (происходит копирование).
 * @return Указатель на свежеразмещенный элемент либо NULL.
 */
MAGNA_API ListItem* MAGNA_CALL list_push_back
    (
        LinkedList *list,
        const void *data
    )
{
    ListItem *result, *last;
    void *ptr;

    assert (list != NULL);
    assert (data != NULL);

    result = mem_alloc (DATA_SIZE (list->itemSize));
    if (result == NULL) {
        return NULL;
    }

    result->next = NULL;
    last = list->last;
    result->previous = last;
    if (last != NULL) {
        last->next = result;
    }

    list->last = result;
    if (list->first == NULL) {
        list->first = result;
    }

    ptr = ITEM_TO_DATA (result);
    mem_copy (ptr, data, list->itemSize);

    return result;
}

/**
 * Добавление элемента в конец списка.
 *
 * @param list Список.
 * @return Указатель на место для размещения данных либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL list_emplace_back
    (
        LinkedList *list
    )
{
    ListItem *item, *last;
    void *result;

    assert (list != NULL);

    item = mem_alloc (DATA_SIZE (list->itemSize));
    if (item == NULL) {
        return NULL;
    }

    item->next = NULL;
    last = list->last;
    item->previous = last;
    if (last != NULL) {
        last->next = item;
    }

    list->last = item;
    if (list->first == NULL) {
        list->first = item;
    }

    result = ITEM_TO_DATA (item);

    return result;
}

/**
 * Добавление элемента в начало списка.
 *
 * @param list Список.
 * @param data Указатель на добавляемые данные (происходит копирование).
 * @return Указатель на добавленный элемент либо `NULL`.
 */
MAGNA_API ListItem* MAGNA_CALL list_push_front
    (
        LinkedList *list,
        const void *data
    )
{
    ListItem *result, *first;
    void *ptr;

    assert (list != NULL);
    assert (data != NULL);

    result = mem_alloc (DATA_SIZE (list->itemSize));
    if (result == NULL) {
        return NULL;
    }

    result->previous = NULL;
    first = list->first;
    result->next = first;
    if (first != NULL) {
        first->previous = result;
    }

    list->first = result;
    if (list->last == NULL) {
        list->last = result;
    }

    ptr = ITEM_TO_DATA (result);
    mem_copy (ptr, data, list->itemSize);

    return result;
}

/**
 * Добавление элемента в начало списка.
 *
 * @param list Список.
 * @return Указатель на место для размещения данных либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL list_emplace_front
    (
        LinkedList *list
    )
{
    ListItem *item, *first;
    void *result;

    assert (list != NULL);

    item = mem_alloc (DATA_SIZE (list->itemSize));
    if (item == NULL) {
        return NULL;
    }

    item->previous = NULL;
    first = list->first;
    item->next = list->first;
    if (first != NULL) {
        first->previous = item;
    }

    list->first = item;
    if (list->last == NULL) {
        list->last = item;
    }

    result = ITEM_TO_DATA (item);

    return result;
}

/**
 * Удаление указанного элемента.
 *
 * @param list Список.
 * @param item Удаляемый элемент.
 * @param liberator Функция освобождения ресурсов (опционально).
 */
MAGNA_API void MAGNA_CALL list_remove
    (
        LinkedList *list,
        ListItem *item,
        Liberator liberator
    )
{
    void *data;

    assert (list != NULL);
    assert (item != NULL);

    if (list->first == item) {
        list->first = item->next;
    }

    if (list->last == item) {
        list->last = item->previous;
    }

    if (item->next != NULL) {
        item->next->previous = item->previous;
    }

    if (item->previous != NULL) {
        item->previous->next = item->next;
    }

    if (liberator != NULL) {
        data = ITEM_TO_DATA (item);
        liberator (data);
    }
}

/**
 * Список пуст?
 *
 * @param list Список для проверки.
 * @return Результат проверки.
 */
MAGNA_API MAGNA_INLINE am_bool MAGNA_CALL list_is_empty
    (
        const LinkedList *list
    )
{
    assert (list != NULL);

    return list->first == NULL;
}

/**
 * Вычисление длины списка.
 *
 * @param list Список.
 * @return Количество элементов в списке.
 */
MAGNA_API am_size_t MAGNA_CALL list_length
    (
        const LinkedList *list
    )
{
    const ListItem *item;
    am_size_t result = 0;

    assert (list != NULL);

    for (item = list->first; item != NULL; item = item->next) {
        ++result;
    }

    return result;
}

/**
 * Вставка элемента перед указанным.
 *
 * @param list Список.
 * @param item Элемент.
 * @param data Указатель на данные (происходит копирование).
 * @return Указатель на созданный элемент либо `NULL`.
 */
MAGNA_API ListItem* MAGNA_CALL list_insert_before
    (
        LinkedList *list,
        ListItem *item,
        const void *data
    )
{
    ListItem *result, *previous;
    void *ptr;

    assert (list != NULL);
    assert (item != NULL);
    assert (data != NULL);

    result = mem_alloc (DATA_SIZE (list->itemSize));
    if (result == NULL) {
        return NULL;
    }

    previous = item->previous;
    result->previous = previous;
    item->previous = result;
    result->next = item;
    if (previous != NULL) {
        previous->next = result;
    }

    if (item == list->first) {
        list->first = result;
    }

    ptr = ITEM_TO_DATA (result);
    mem_copy (ptr, data, list->itemSize);

    return result;
}

/**
 * Вставка элемента перед указанным.
 *
 * @param list Список.
 * @param item Элемент.
 * @return Указатель на место для размещения данных либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL list_emplace_before
    (
        LinkedList *list,
        ListItem *item
    )
{
    ListItem *result, *previous;
    void *ptr;

    assert (list != NULL);
    assert (item != NULL);

    result = mem_alloc (DATA_SIZE (list->itemSize));
    if (result == NULL) {
        return NULL;
    }

    previous = item->previous;
    result->previous = previous;
    item->previous = result;
    result->next = item;
    if (previous != NULL) {
        previous->next = result;
    }

    if (item == list->first) {
        list->first = result;
    }

    ptr = ITEM_TO_DATA (result);

    return ptr;
}

/**
 * Вставка элемента после указанного.
 *
 * @param list Список.
 * @param item Элемент.
 * @param data Указатель на данные (происходит копирование).
 * @return Указатель на созданный элемент либо NULL.
 */
MAGNA_API ListItem* MAGNA_CALL list_insert_after
    (
        LinkedList *list,
        ListItem *item,
        const void *data
    )
{
    ListItem *result, *next;
    void *ptr;

    assert (list != NULL);
    assert (item != NULL);
    assert (data != NULL);

    result = mem_alloc (DATA_SIZE (list->itemSize));
    if (result == NULL) {
        return NULL;
    }

    next = item->next;
    result->next = next;
    result->previous = item;
    item->next = result;
    if (next != NULL) {
        next->previous = result;
    }

    if (item == list->last) {
        list->last = result;
    }

    ptr = ITEM_TO_DATA (result);
    mem_copy (ptr, data, list->itemSize);

    return result;
}

/**
 * Вставка элемента после указанного.
 *
 * @param list Список.
 * @param item Элемент.
 * @return Указатель на место для размещения данных либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL list_emplace_after
    (
        LinkedList *list,
        ListItem *item
    )
{
    ListItem *result, *next;
    void *ptr;

    assert (list != NULL);
    assert (item != NULL);

    result = mem_alloc (DATA_SIZE (list->itemSize));
    if (result == NULL) {
        return NULL;
    }

    next = item->next;
    result->next = next;
    result->previous = item;
    item->next = result;
    if (next != NULL) {
        next->previous = result;
    }

    if (item == list->last) {
        list->last = result;
    }

    ptr = ITEM_TO_DATA (result);

    return ptr;
}

/**
 * Получение указателя на начало данных в данном элементе списка.
 *
 * @param item Элемент списка.
 * @return Указатель на начало данных.
 */
MAGNA_API MAGNA_INLINE void* MAGNA_CALL list_data
    (
        const ListItem *item
    )
{
    assert (item != NULL);

    return ITEM_TO_DATA (item);
}

/**
 * Получение указателя на элемент списка
 * из указателя на начало данных в данном элементе списка.
 *
 * @param item Указатель на начало данных.
 * @return Указатель на элемент списка.
 */
MAGNA_API MAGNA_INLINE ListItem* MAGNA_CALL list_item
    (
        const void *data
    )
{
    assert (data != NULL);

    return (ListItem*) DATA_TO_ITEM (data);
}

/**
 * Проход по списку от начала к концу.
 *
 * @param list Список.
 * @param walker Пользовательская функция. Возвращает `AM_FALSE`,
 * если необходимо прервать проход.
 * @param data Пользовательские данные (опционально).
 * @return Список был пройден полностью?
 */
MAGNA_API am_bool MAGNA_CALL list_walk_forward
    (
        LinkedList *list,
        ListWalker walker,
        void *data
    )
{
    ListItem *item;

    assert (list != NULL);
    assert (walker != NULL);

    for (item = list->first; item; item = item->next) {
        if (!walker (list, item, data)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/**
 * Проход по списку от конца к началу.
 *
 * @param list Список.
 * @param walker Пользовательская функция. Возвращает `AM_FALSE`,
 * если необходимо прервать проход.
 * @param data Пользовательские данные (опционально).
 * @return Список был пройден полностью?
 */
MAGNA_API am_bool MAGNA_CALL list_walk_backward
    (
        LinkedList *list,
        ListWalker walker,
        void *data
    )
{
    ListItem *item;

    assert (list != NULL);
    assert (walker != NULL);

    for (item = list->last; item; item = item->previous) {
        if (!walker (list, item, data)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/**
 * Поиск первого вхождения указанных данных.
 *
 * @param list Список.
 * @param comparer Функция сравнения.
 * @param data Искомые данные.
 * @param extra Дополнительные пользовательские данные (оцпионально).
 * @return Указатель на найденный элемент либо `NULL`.
 */
MAGNA_API ListItem* MAGNA_CALL list_find_first
    (
        const LinkedList *list,
        Comparer comparer,
        void *data,
        void *extra
    )
{
    const ListItem *item;
    void *ptr;

    assert (list != NULL);
    assert (comparer != NULL);

    for (item = list->first; item != NULL; item = item->next) {
        ptr = ITEM_TO_DATA (item);
        if (comparer (ptr, data, extra) == 0) {
            return (ListItem*) item;
        }
    }

    return NULL;
}

/**
 * Поиск последнего вхождения указанных данных.
 *
 * @param list Список.
 * @param comparer Функция сравнения.
 * @param data Искомые данные.
 * @param extra Дополнительные пользовательские данные (оцпионально).
 * @return Указатель на найденный элемент либо `NULL`.
 */
MAGNA_API ListItem* MAGNA_CALL list_find_last
    (
        const LinkedList *list,
        Comparer comparer,
        void *data,
        void *extra
    )
{
    const ListItem *item;
    void *ptr;

    assert (list != NULL);
    assert (comparer != NULL);

    for (item = list->first; item != NULL; item = item->next) {
        ptr = ITEM_TO_DATA (item);
        if (comparer (ptr, data, extra) == 0) {
            return (ListItem*) item;
        }
    }

    return NULL;
}

/**
 * Извлечение последнего элемента из списка
 * (с последующим удалением из списка).
 * Освобождение элемента возложено вызывающий код.
 *
 * @param list Список.
 * @return Извлеченный элемент либо `NULL`.
 */
MAGNA_API ListItem* MAGNA_CALL list_pop_back
    (
        LinkedList *list
    )
{
    ListItem *result;

    assert (list != NULL);

    result = list->last;
    if (result != NULL) {
        list->last = result->previous;
    }

    return result;
}

/**
 * Извлечение последнего элемента из списка
 * (с последующим удалением из списка).
 * Освобождение полученных возложено на вызывающий код.
 *
 * @param list Список.
 * @param place Указатель на место для размещения данных (происходит копирование).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL list_extract_back
    (
        LinkedList *list,
        void *place
    )
{
    ListItem *item;
    void *ptr;

    assert (list != NULL);

    item = list->last;
    if (item != NULL) {
        list->last = item->previous;
        ptr = ITEM_TO_DATA (item);
        mem_copy (place, ptr, list->itemSize);
        mem_free (item);

        return AM_TRUE;
    }

    return AM_FALSE;
}

/**
 * Извлечение первого элемента из списка
 * (с последующим удалением из списка).
 * Освобождение элемента возложено вызывающий код.
 *
 * @param list Список.
 * @return Извлеченный элемент либо `NULL`.
 */
MAGNA_API ListItem* MAGNA_CALL list_pop_front
    (
        LinkedList *list
    )
{
    ListItem *result;

    assert (list != NULL);

    result = list->first;
    if (result != NULL) {
        list->first = result->next;
    }

    return result;
}

/**
 * Извлечение первого элемента из списка
 * (с последующим удалением из списка).
 * Освобождение полученных возложено на вызывающий код.
 *
 * @param list Список.
 * @param place Место для размещения данных (происходит копирование).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL list_extract_front
    (
        LinkedList *list,
        void *place
    )
{
    ListItem *item;
    void *ptr;

    assert (list != NULL);

    item = list->first;
    if (item != NULL) {
        list->first = item->next;
        ptr = ITEM_TO_DATA (item);
        mem_copy (place, ptr, list->itemSize);
        mem_free (item);

        return AM_TRUE;
    }

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
