/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

#include <assert.h>

MAGNA_API am_bool MAGNA_CALL response_create
    (
        struct IrbisConnection *connection,
        Response *response
    )
{
    assert (connection != NULL);
    assert (response != NULL);

    memset (response, 0, sizeof (Response));
    response->connection = connection;

    return 0;
}

/**
 * Освобождение ресурсов, занятых ответом сервера.
 *
 * @param response Ответ сервера.
 */
MAGNA_API void MAGNA_CALL response_free
    (
        Response *response
    )
{
    assert (response != NULL);

    buffer_destroy(&response->answer);
    memset (response, 0, sizeof (Response));
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
    Span result = { NULL, 0 };

    assert (response != NULL);

    return result;
}

MAGNA_API am_int32 MAGNA_CALL response_get_return_code
    (
        Response *response
    )
{
    assert (response != NULL);

    return 0;
}

MAGNA_API Span MAGNA_CALL response_read_ansi
    (
        Response *response
    )
{
    Span result = { NULL, 0 };

    assert (response != NULL);

    return result;
}

MAGNA_API am_int32 MAGNA_CALL response_read_int32
    (
        Response *response
    )
{
    assert (response != NULL);

    return 0;
}

MAGNA_API Span MAGNA_CALL response_read_utf
    (
        Response *response
    )
{
    Span result = { NULL, 0 };

    return result;
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
    Span result = { NULL, 0 };

    assert (response != NULL);

    return result;
}
