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
 * Простой двусвязный список.
 *
 * Содержит элементы плюс указатели на следующий и предыдущий.
 *
 * Владеет только собственной памятью. Памятью элементов
 * распоряжаются сами элементы.
 */

/*=========================================================*/

struct MaganaListItem
{
    ListItem *next;
    ListItem *previous;
    am_byte dataStart;
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
