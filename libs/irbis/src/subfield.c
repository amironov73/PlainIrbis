/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/**
 * \file subfield.c
 *
 * Подполе записи. Состоит из кода и значения.
 */

#include <assert.h>

/*=========================================================*/

MAGNA_API SubField* MAGNA_CALL subfield_clone
    (
        SubField *target,
        const SubField *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    return target;
}

MAGNA_API SubField* MAGNA_CALL subfield_decode
    (
        SubField *subfield,
        Span span
    )
{
    assert (subfield != NULL);

    return subfield;
}

MAGNA_API am_bool MAGNA_CALL subfield_empty
    (
        const SubField *subfield
    )
{
    assert (subfield != NULL);

    return 0;
}

MAGNA_API SubField* MAGNA_CALL subfield_init
    (
        SubField *subfield,
        char code,
        const char *value
    )
{
    assert (subfield != NULL);

    subfield->code = code;

    return subfield;
}

MAGNA_API Buffer* MAGNA_CALL subfield_to_string
    (
        const SubField *subfield,
        Buffer *buffer
    )
{
    assert (subfield != NULL);
    assert (buffer != NULL);

    return buffer;
}

MAGNA_API am_bool MAGNA_CALL subfield_verify
    (
        const SubField *subfield
    )
{
    assert (subfield != NULL);

    return 0;
}
