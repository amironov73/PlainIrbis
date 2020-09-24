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
 * \file list.c
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

struct MaganaListItem
{
    ListItem *next;
    ListItem *previous;
};

typedef struct
{
    ListItem *first;
    ListItem *last;
    am_size_t itemSize;

} LinkedList;

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
