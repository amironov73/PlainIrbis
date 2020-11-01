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
 * \file source.c
 *
 * Издание, в котором опубликована статья, поля 463 и 963.
 */

/*=========================================================*/

typedef struct
{
    Buffer title;
    Buffer year;
    Buffer publisher;
    Buffer city;
    Buffer unit;
    Buffer position;
    Buffer articleNumber;
    Buffer illustrations;
    Buffer comments;
    Buffer volume;

} Source;

MAGNA_API void MAGNA_CALL source_init
    (
        Source *source
    )
{
    assert (source != NULL);

    mem_clear (source, sizeof (*source));
}

MAGNA_API void MAGNA_CALL source_destroy
    (
        Source *source
    )
{
    assert (source != NULL);

    mem_clear (source, sizeof (*source));
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
