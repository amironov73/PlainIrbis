/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

#include <assert.h>

MAGNA_API void MAGNA_CALL query_add_ansi
    (
        Query *query,
        const char *text
    )
{
    assert (query != NULL);
    assert (text != NULL);
}

MAGNA_API void MAGNA_CALL query_add_format
    (
        Query *query,
        const char *text
    )
{
    assert (query != NULL);
    assert (text != NULL);
}

MAGNA_API void MAGNA_CALL query_add_int32
    (
        Query *query,
        am_int32 value
    )
{
    assert (query != NULL);
}

MAGNA_API void MAGNA_CALL query_add_utf
    (
        Query *query,
        const char *text
    )
{
    assert (query != NULL);
    assert (text != NULL);
}

MAGNA_API void MAGNA_CALL query_new_line
    (
        Query *query
    )
{
    assert (query != NULL);
}

MAGNA_API void MAGNA_CALL query_create
    (
        struct IrbisConnection *connection,
        Query *query
    )
{
    assert (connection != NULL);
    assert (query != NULL);
}
