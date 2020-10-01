/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

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
 *
 * @param version Структура, подлежащая инициализации.
 * @return Проинициализированная структура.
 */
MAGNA_API ServerVersion* MAGNA_CALL version_init
    (
        ServerVersion *version
    )
{
    assert (version != NULL);

    mem_clear (version, sizeof (ServerVersion));

    return version;
}

/**
 * Разбор ответа сервера.
 *
 * @param version Информация о версии сервера.
 * @param response Ответ сервера.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL version_parse
    (
        ServerVersion *version,
        Response *response
    )
{
    am_bool result = AM_FALSE;
    Vector lines;

    assert (version != NULL);
    assert (response != NULL);

    if (!vector_create(&lines, 4)) {
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
    vector_destroy(&lines);

    return result;
}

/**
 * Текстовое представление информации о версии сервера.
 *
 * @param version Информация о версии.
 * @param buffer Буфер, в который должно быть помещено текстовое представление.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL version_to_string
    (
        const ServerVersion *version,
        Buffer *buffer
    )
{
    assert (version != NULL);
    assert (buffer != NULL);

    return buffer_puts (buffer, "organization=")
        && buffer_concat (buffer, &version->organization)
        && buffer_puts (buffer, ", version=")
        && buffer_concat (buffer, &version->version)
        && buffer_puts (buffer, ", maxClients=")
        && buffer_put_uint_32 (buffer, version->maxClients)
        && buffer_puts (buffer, ", connected=")
        && buffer_put_uint_32 (buffer, version->connected);
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
