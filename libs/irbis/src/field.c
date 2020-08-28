/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/**
 * \file field.c
 *
 * Поле записи. Состоит из метки, значения до первого разделителя
 * и произвольного количества подполей.
 */

#include <assert.h>

/*=========================================================*/

MAGNA_API SubField* MAGNA_CALL field_add
    (
        MarcField *field,
        char code,
        const char *value
    )
{
    SubField *subfield;

    assert (field != NULL);

    subfield = (SubField*) calloc(1, sizeof (SubField));
    array_push_back
        (
            &field->subfields,
            subfield_init (subfield, code, value)
        );

    return subfield;
}

MAGNA_API MarcField* MAGNA_CALL field_clear
    (
        MarcField *field
    )
{
    assert (field != NULL);

    /* buffer_truncate (&field->value, 0); */

    return field;
}

MAGNA_API MarcField* MAGNA_CALL field_clone
    (
        MarcField *target,
        const MarcField *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    return target;
}

MAGNA_API MarcField* MAGNA_CALL field_decode
    (
        MarcField *field,
        Span span
    )
{
    assert (field != NULL);

    return field;
}

MAGNA_API am_bool MAGNA_CALL field_empty
    (
        const MarcField *field
    )
{
    assert (field != NULL);

    return 0;
}

MAGNA_API Array* MAGNA_CALL field_get_embedded_fields
    (
        const MarcField *field,
        Array *array
    )
{
    assert (field != NULL);
    assert (array != NULL);

    return array;
}

MAGNA_API SubField* MAGNA_CALL field_get_first_subfield
    (
        const MarcField *field,
        char code
    )
{
    assert (field != NULL);

    return NULL;
}

MAGNA_API Span MAGNA_CALL field_get_first_subfield_value
    (
        const MarcField *field,
        char code
    )
{
    Span result = { NULL, 0 };

    assert (field != NULL);

    return result;
}

MAGNA_API MarcField* MAGNA_CALL field_init
    (
        MarcField *field,
        am_uint32 tag
    )
{
    assert (field != NULL);

    field->tag = tag;

    return field;
}

MAGNA_API MarcField* MAGNA_CALL field_insert_at
    (
        MarcField *field,
        am_size index,
        const SubField *subfield
    )
{
    assert (field != NULL);
    assert (subfield != NULL);
}

MAGNA_API MarcField* MAGNA_CALL field_remove_at
    (
        MarcField *field,
        am_size index
    )
{
    assert (field != NULL);

    return field;
}

MAGNA_API MarcField* MAGNA_CALL field_remove_subfield
    (
        MarcField *field,
        char code
    )
{
    assert (field != NULL);

    return field;
}

MAGNA_API Buffer* MAGNA_CALL field_to_string
    (
        const MarcField *field,
        Buffer *buffer
    )
{
    assert (field != NULL);
    assert (buffer != NULL);

    return buffer;
}

MAGNA_API am_bool MAGNA_CALL field_verify
    (
        const MarcField *field
    )
{
    assert (field != NULL);

    return 0;
}
