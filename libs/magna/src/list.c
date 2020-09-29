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
#define ITEM_TO_DATA(__i) ((__i) + HEADER_SIZE)
#define DATA_TO_ITEM(__d) ((__d) - HEADER_SIZE)

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
    ListItem *result;
    void *ptr;

    assert (list != NULL);
    assert (data != NULL);

    result = mem_alloc (DATA_SIZE (list->itemSize));
    if (result == NULL) {
        return NULL;
    }

    result->next = NULL;
    result->previous = list->last;
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
    ListItem *item;
    void *result;

    assert (list != NULL);

    item = mem_alloc (DATA_SIZE (list->itemSize));
    if (item == NULL) {
        return NULL;
    }

    item->next = NULL;
    item->previous = list->last;
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
    ListItem *result;
    void *ptr;

    assert (list != NULL);
    assert (data != NULL);

    result = mem_alloc (DATA_SIZE (list->itemSize));
    if (result == NULL) {
        return NULL;
    }

    result->previous = NULL;
    result->next = list->first;
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
    ListItem *item;
    void *result;

    assert (list != NULL);

    item = mem_alloc (DATA_SIZE (list->itemSize));
    if (item == NULL) {
        return NULL;
    }

    item->previous = NULL;
    item->next = list->last;
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

    return list->first != NULL;
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
 * @return Указатель на созданный элемент либо NULL.
 */
MAGNA_API ListItem* MAGNA_CALL list_insert_before
    (
        LinkedList *list,
        ListItem *item,
        const void *data
    )
{
    ListItem *result;
    void *ptr;

    assert (list != NULL);
    assert (item != NULL);
    assert (data != NULL);

    result = mem_alloc (DATA_SIZE (list->itemSize));
    if (result == NULL) {
        return NULL;
    }

    result->previous = item->previous;
    item->previous = result;
    result->next = item;

    /* TODO: implement properly */

    if (item == list->first) {
        list->first = result;
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
    ListItem *result;
    void *ptr;

    assert (list != NULL);
    assert (item != NULL);
    assert (data != NULL);

    result = mem_alloc (DATA_SIZE (list->itemSize));
    if (result == NULL) {
        return NULL;
    }

    result->next = item->next;
    item->previous = result;
    result->next = item;

    /* TODO: implement properly */

    if (item == list->last) {
        list->last = result;
    }

    ptr = ITEM_TO_DATA (result);
    mem_copy (ptr, data, list->itemSize);

    return result;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
