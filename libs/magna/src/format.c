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
#include <stdarg.h>

/*=========================================================*/

/**
 * \file format.c
 *
 * Форматированый вывод текста.
 */

/*=========================================================*/

/**
 * Генерализованный форматированный вывод.
 *
 * @param output Куда направляется вывод.
 * @param handler Функция вывода.
 * @param format Спецификация формата.
 * @param args Аргументы.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL format_generic
    (
        void *output,
        OutputFunction handler,
        Span format,
        va_list args
    )
{
    assert (handler != NULL);

    /* TODO: implement */

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
