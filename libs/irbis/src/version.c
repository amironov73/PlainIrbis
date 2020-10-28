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
 * \file version.c
 *
 * Информация о версии сервера.
 */

/*=========================================================*/

/**
 * Инициализация структуры.
 * Не выделяет память в куче.
 *
 * @param version Структура, подлежащая инициализации.
 */
MAGNA_API void MAGNA_CALL version_init
    (
        ServerVersion *version
    )
{
    assert (version != NULL);

    mem_clear (version, sizeof (*version));
}

/**
 * Освобождение ресурсов, занятых структурой.
 *
 * @param version Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL version_destroy
    (
        ServerVersion *version
    )
{
    assert (version != NULL);

    buffer_destroy (&version->organization);
    buffer_destroy (&version->version);
    mem_clear (version, sizeof (*version));
}

/**
 * Разбор ответа сервера.
 *
 * @param version Информация о версии сервера.
 * @param response Ответ сервера.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL version_parse_response
    (
        ServerVersion *version,
        Response *response
    )
{
    am_bool result = AM_FALSE;
    SpanArray lines;

    assert (version != NULL);
    assert (response != NULL);

    if (!span_array_create (&lines, 4)) {
        return AM_FALSE;
    }

    if (!response_remaining_ansi_lines (response, &lines)
        || lines.len < 3) {
        goto DONE;
    }

/*
    if (lines.len == 3) {

    }
    else {

    }
*/

    result = AM_TRUE;

    DONE:
    span_array_destroy (&lines);

    return result;
}

/**
 * Текстовое представление информации о версии сервера.
 *
 * @param version Информация о версии.
 * @param output Буфер, в который должно быть помещено текстовое представление.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL version_to_string
    (
        const ServerVersion *version,
        Buffer *output
    )
{
    assert (version != NULL);
    assert (output != NULL);

    return buffer_puts (output, CBTEXT ("organization="))
           && buffer_concat (output, &version->organization)
           && buffer_puts (output, CBTEXT (", version="))
           && buffer_concat (output, &version->version)
           && buffer_puts (output, CBTEXT (", maxClients="))
           && buffer_put_uint32(output, version->maxClients)
           && buffer_puts (output, CBTEXT (", connected="))
           && buffer_put_uint32(output, version->connected);
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
