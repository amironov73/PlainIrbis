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
 * \file response.c
 *
 * Ответ сервера.
 */

/*=========================================================*/

MAGNA_API am_bool MAGNA_CALL response_create
    (
        struct IrbisConnection *connection,
        Response *response
    )
{
    assert (connection != NULL);
    assert (response != NULL);

    mem_clear (response, sizeof (Response));
    response->connection = connection;

    return AM_TRUE;
}

/**
 * Освобождение ресурсов, занятых ответом сервера.
 *
 * @param response Ответ сервера.
 */
MAGNA_API void MAGNA_CALL response_destroy
    (
        Response *response
    )
{
    assert (response != NULL);

    buffer_destroy(&response->answer);
    mem_clear (response, sizeof (Response));
}

MAGNA_API am_bool response_check
    (
        Response *response,
        ...
    )
{
    assert (response != NULL);

    return 0;
}

MAGNA_API Span MAGNA_CALL response_get_line
    (
        Response *response
    )
{
    Span result;

    assert (response != NULL);

    result = nav_read_line (&response->navigator);

    return result;
}

MAGNA_API am_int32 MAGNA_CALL response_get_return_code
    (
        Response *response
    )
{
    assert (response != NULL);

    response->connection->lastError
        = response->returnCode = response_read_int32 (response);

    return response->returnCode;
}

MAGNA_API Span MAGNA_CALL response_read_ansi
    (
        Response *response
    )
{
    assert (response != NULL);

    /* TODO: implement properly */

    return response_get_line (response);
}

MAGNA_API am_int32 MAGNA_CALL response_read_int32
    (
        Response *response
    )
{
    Span line;
    am_int32 result;

    assert (response != NULL);

    /* TODO: implement properly */

    line = response_get_line (response);
    result = span_to_int32 (line);

    return result;
}

MAGNA_API Span MAGNA_CALL response_read_utf
    (
        Response *response
    )
{
    assert (response != NULL);

    return response_get_line (response);
}

MAGNA_API am_bool MAGNA_CALL response_remaining_lines
    (
        Response *response,
        SpanArray *lines
    )
{
    assert (response != NULL);
    assert (lines != NULL);
}

MAGNA_API am_bool MAGNA_CALL response_remaining_ansi_lines
    (
        Response *response,
        Vector *array
    )
{
    assert (response != NULL);
    assert (array != NULL);

    return AM_FALSE;
}

MAGNA_API Span MAGNA_CALL response_remaining_ansi_text
    (
        Response *response
    )
{
    Span result = { NULL, 0 };

    assert (response != NULL);

    return result;
}

MAGNA_API am_bool MAGNA_CALL response_remaining_utf_lines
    (
        Response *response,
        Vector *array
    )
{
    assert (response != NULL);
    assert (array != NULL);

    return AM_FALSE;
}

MAGNA_API Span MAGNA_CALL response_remaining_utf_text
    (
        Response *response
    )
{
    Span result;

    assert (response != NULL);

    result = nav_remaining (&response->navigator);

    return result;
}

MAGNA_API void MAGNA_CALL response_null
    (
        Response *response
    )
{
    assert (response != NULL);

    mem_clear (response, sizeof (Response));
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
