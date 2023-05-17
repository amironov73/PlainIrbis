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
 * \file variant.c
 *
 * Контейнер, умеющий хранить объект произвольного типа.
 */

/**
 * Клонирование (глубокое копирование) вектора.
 *
 * @param target Вектор назначения.
 * @param source Источник.
 * @return Признак успешного завершения операции.
 */
MAGNA_API AM_VARIANT* MAGNA_CALL variant_copy
    (
        AM_VARIANT *target,
        const AM_VARIANT *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    mem_copy (target, source, sizeof (AM_VARIANT));

    return target;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
