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
 * \file userinfo.c
 *
 * Информация о пользователе системы.
 */

/*=========================================================*/

/**
 * Простая инициализация структуры.
 * Не выделяет память в куче.
 *
 * @param userinfo Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL userinfo_init
    (
        UserInfo *userinfo
    )
{
    assert (userinfo != NULL);

    mem_clear (&userinfo, sizeof (*userinfo));
}

/**
 * Инициализация динамического массива пользователей.
 *
 * @param users Указатель не неинициализированный массив.
 */
MAGNA_API void MAGNA_CALL userinfo_init_array
    (
        Array *users
    )
{
    assert (users != NULL);

    array_init (users, sizeof (UserInfo));
}

/**
 * Освобождение ресурсов, занятых структурой.
 *
 * @param userinfo Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL userinfo_destroy
    (
        UserInfo *userinfo
    )
{
    assert (userinfo != NULL);

    buffer_destroy (&userinfo->number);
    buffer_destroy (&userinfo->name);
    buffer_destroy (&userinfo->password);
    buffer_destroy (&userinfo->cataloger);
    buffer_destroy (&userinfo->reader);
    buffer_destroy (&userinfo->circulation);
    buffer_destroy (&userinfo->acquisitions);
    buffer_destroy (&userinfo->provision);
    buffer_destroy (&userinfo->administrator);
    mem_clear (&userinfo, sizeof (*userinfo));
}

/**
 * Освобождение ресурсов, занятых массивом пользователей.
 *
 * @param users Массив пользователей, подлежащий освобождению.
 */
MAGNA_API void MAGNA_CALL userinfo_destroy_array
    (
        Array *users
    )
{
    assert (users != NULL);

    array_destroy (users, (Liberator) userinfo_destroy);
}

/**
 * Разбор трех строк из ответа сервера.
 *
 * @param userinfo Структура, подлежащая заполнению.
 * @param lines Три строки.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL userinfo_parse_lines
    (
        UserInfo *userinfo,
        const Span *lines
    )
{
    assert (userinfo != NULL);
    assert (lines != NULL);

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Разбор ответа сервера.
 *
 * @param users Массив пользователей, который должен быть заполнен.
 * @param response Ответ сервера.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL userinfo_parse_menu
    (
        Array *users,
        Response *response
    )
{
    Span lines[3];
    UserInfo user;

    assert (users != NULL);
    assert (response != NULL);

    while (!response_eot (response)) {
        lines[0] = response_get_line (response);
        if (span_is_empty (lines[0])
          || span_compare (lines[0], TEXT_SPAN (STOP_MARKER)) == 0) {
            break;
        }

        lines[1] = response_get_line (response);
        lines[2] = response_get_line (response);
        userinfo_init (&user);
        if (!userinfo_parse_lines (&user, lines)
           || !array_push_back (users, &user)) {
            return AM_FALSE;
        }

    }

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
