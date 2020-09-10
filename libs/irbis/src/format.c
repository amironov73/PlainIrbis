/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 * \file format.c
 *
 * Работа с форматом.
 *
 */

/*=========================================================*/

/**
 * Подготовка формата к отсылке на сервер.
 * Удаляются служебные символы, на которые
 * сервер реагирует очень нервно.
 * Также удаляются комментарии.
 *
 * @param target Буфер для размещения результата.
 * @param source Фрагмент, содержащий исходный формат.
 * @return
 */
MAGNA_API am_bool format_prepare
    (
        Buffer *target,
        const Span source
    )
{
    assert (target != NULL);

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
