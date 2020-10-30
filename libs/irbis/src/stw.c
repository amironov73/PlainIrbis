// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/irbis.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 * \file stw.c
 *
 * Работа с STW-файлами (списки стоп-слов).
 *
 * Пример списка стоп-слов:
 *
 * ```
 * A
 * ABOUT
 * AFTER
 * AGAINST
 * ALL
 * ALS
 * AN
 * AND
 * AS
 * AT
 * AUF
 * AUS
 * AUX
 * B
 * BIJ
 * BY
 * ```
 *
 */

/*=========================================================*/

/* Список стоп-слов */

typedef struct
{
    Array words;

} StopWords;

MAGNA_API void MAGNA_CALL stw_init
    (
        StopWords *words
    )
{
    assert (words != NULL);

    array_init (&words->words, sizeof (Buffer));
}

MAGNA_API void MAGNA_CALL stw_destroy
    (
        StopWords *words
    )
{
    assert (words != NULL);

    array_destroy (&words->words, (Liberator) buffer_destroy);
}

MAGNA_API am_bool MAGNA_CALL stw_contains
    (
        const StopWords *words,
        Span word
    )
{
    assert (words != NULL);

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
