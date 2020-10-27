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
#include <stdarg.h>

/*=========================================================*/

/**
 * \file response.c
 *
 * Ответ сервера.
 *
 * \struct Response
 *      \brief Ответ сервера.
 *
 * \var Response::answer
 *      \brief Содержимое ответа, помещенное в буфер.
 *
 * \var Response::command
 *      \brief Код команды, присланный в ответе.
 *
 * \var Response clientId
 *      \brief Идентификатор клиента, присланный в ответе.
 *
 * \var Response::queryId
 *      \brief Идентификатор запроса, присланный в ответе.
 *
 * \var Response::returnCode
 *      \brief Код ошибки (не всегда доступен).
 *
 * \var Response::answerSize
 *      \brief Размер ответа сервера в байтах (может высылаться сервером).
 *
 * \var Response::serverVersion
 *      \brief Версия сервера (может высылаться в ответ на команду `A`.
 *
 * \var Response::connection
 *      \brief Указатель на подключение для выставления
 *      в нем кода ошибки при сбоях декодирования ответа.
 *
 * \var Response::navigator
 *      \brief Навигатор для считывания строк.
 *
 */

/*=========================================================*/

/**
 * Простая инициализация структуры ответа сервера.
 * Не выделяет память в куче.
 *
 * @param response Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL response_init
    (
        Response *response
    )
{
    assert (response != NULL);

    mem_clear (response, sizeof (*response));
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

    buffer_destroy (&response->answer);
    mem_clear (response, sizeof (*response));
}

/**
 * Достигнут конец ответа сервера?
 *
 * @param response Ответ сервера.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL response_eot
    (
        const Response *response
    )
{
    assert (response != NULL);

    return nav_eot (&response->navigator);
}

MAGNA_API am_bool response_check
    (
        Response *response,
        ...
    )
{
    am_bool result;
    va_list args;
    am_int32 rc;

    assert (response != NULL);

    result = response_get_return_code (response) >= 0;
    if (!result) {
        va_start (args, response);
        while (AM_TRUE) {
            rc = va_arg (args, am_int32);
            if (rc >= 0) {
                break;
            }

            if (rc == response->returnCode) {
                result = AM_TRUE;
                break;
            }
        }
        va_end (args);
    }

    return result;
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

    return AM_FALSE;
}

MAGNA_API am_bool MAGNA_CALL response_remaining_ansi_lines
    (
        Response *response,
        SpanArray *array
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
        SpanArray *array
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

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
