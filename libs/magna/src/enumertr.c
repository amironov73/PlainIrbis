// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 * \file enumertr.c
 *
 * Перечислитель.
 */

/*=========================================================*/

/**
 * Перечисление всех элементов и выполнение некоего действия
 * с каждым элементом.
 *
 * @param enumerator Перечислитель.
 * @param action Действие. Возвращает `AM_FALSE` если перечисление должно быть прервано.
 * @return `AM_TRUE`, если перечисление не было прервано.
 */
MAGNA_API am_bool MAGNA_CALL enum_for_all
    (
        Enumerator *enumerator,
        EnumerationAction action
    )
{
    am_bool result = AM_TRUE;
    void *item;

    assert (enumerator != NULL);
    assert (enumerator->next != NULL);
    assert (action != NULL);

    while (AM_TRUE) {
        item = enum_next (enumerator);
        if (item == NULL) {
            break;
        }

        if (!action (enumerator, item)) {
            result = AM_FALSE;
            break;
        }
    }

    if (enumerator->cleanup != NULL) {
        enumerator->cleanup (enumerator);
    }

    return result;
}

/**
 * Переход к следующему элементу.
 *
 * @param enumerator Перечислитель.
 * @return Указатель на следующий элемент либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL enum_next
    (
        Enumerator *enumerator
    )
{
    assert (enumerator != NULL);
    assert (enumerator->next != NULL);

    return enumerator->next (enumerator);
}

/*=========================================================*/

/* Перечисление элементов обычного массива */

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
