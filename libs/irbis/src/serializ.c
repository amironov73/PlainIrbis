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
 * \file serializ.c
 *
 * Сериализация/десериализация библиографических записей.
 */

/*=========================================================*/

typedef struct
{
    Stream *stream;

} RecordSerializer;

MAGNA_API am_bool MAGNA_CALL serializer_init
    (
        RecordSerializer *serializer,
        Stream *stream
    )
{
    assert (serializer != NULL);
    assert (stream != NULL);

    serializer->stream = stream;

    return AM_TRUE;
}

MAGNA_API void MAGNA_CALL serializer_destroy
    (
        RecordSerializer *serializer
    )
{
    assert (serializer != NULL);

    serializer->stream = NULL;
}

MAGNA_API am_bool MAGNA_CALL serializer_write_record
    (
        RecordSerializer *serializer,
        const MarcRecord *record
    )
{
    assert (serializer != NULL);
    assert (record != NULL);

    /* TODO: implement */

    return AM_FALSE;
}

MAGNA_API am_bool MAGNA_CALL serialier_eos
    (
        RecordSerializer *serializer
    )
{
    assert (serializer != NULL);

    /* TODO: implement */

    return AM_TRUE;
}

MAGNA_API am_bool MAGNA_CALL serializer_read_record
    (
        RecordSerializer *serializer,
        MarcRecord *record
    )
{
    assert (serializer != NULL);
    assert (record != NULL);

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
