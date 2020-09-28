c// This is an open source non-commercial project. Dear PVS-Studio, please check it.
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

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
