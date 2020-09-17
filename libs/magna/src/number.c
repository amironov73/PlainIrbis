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
 * \file number.c
 *
 * Текст, содержащий фрагменты с числами.
 */

/*=========================================================*/

/**
 * Инициализация чанка.
 *
 * @param chunk Указатель на неинициализированный чанк.
 */
MAGNA_API void MAGNA_CALL ntc_init
    (
        NumberTextChunk *chunk
    )
{
    Buffer *prefix;

    assert (chunk != NULL);

    (void) buffer_init (&chunk->prefix);
    chunk->value = 0;
    chunk->length = 0;
    chunk->haveValue = 0;
}

MAGNA_API int MAGNA_CALL ntc_compare
    (
        const NumberTextChunk *first,
        const NumberTextChunk *second
    )
{
    assert (first != NULL);
    assert (second != NULL);

    return 0;
}

MAGNA_API am_bool MAGNA_CALL ntc_have_prefix
    (
        const NumberTextChunk *chunk
    )
{
    assert (chunk != NULL);

    return chunk->prefix.position != 0;
}

MAGNA_API am_bool MAGNA_CALL ntc_setup
    (
        NumberTextChunk *chunk,
        Span text,
        Span number
    )
{
    assert (chunk != NULL);

    return AM_FALSE;
}

MAGNA_API am_bool MAGNA_CALL ntc_to_string
    (
        const NumberTextChunk *chunk,
        Buffer *output
    )
{
    assert (chunk != NULL);
    assert (output != NULL);

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
