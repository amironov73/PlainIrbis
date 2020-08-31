/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 5045)
#endif

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 * \file navigatr.c
 *
 * Навигация по тексту.
 *
 */

/*=========================================================*/

/**
 * Инициализация.
 *
 * @param nav
 * @param data
 * @param dataSize
 * @return
 */
MAGNA_API TextNavigator* MAGNA_CALL nav_init
    (
        TextNavigator* nav,
        const am_byte *data,
        am_size dataSize
    )
{
    assert (data != NULL);
    assert (data != NULL);

    memset (nav, 0, sizeof (TextNavigator));
    nav->data = data;
    nav->length = dataSize;
    nav->position = 0;
    nav->line = 1;
    nav->column = 1;

    return nav;
}

/**
 * Указатель прямо за последним байтом.
 *
 * @param nav
 * @return
 */
MAGNA_API const am_byte* MAGNA_CALL nav_end
    (
        const TextNavigator *nav
    )
{
    assert(nav != NULL);

    return nav->data + nav->length;
}

/**
 * Указатель на текущий байт.
 *
 * @param nav
 * @return
 */
MAGNA_API const am_byte* MAGNA_CALL nav_current
    (
        const TextNavigator *nav
    )
{
    assert(nav != NULL);

    return nav->data + nav->position;
}

/**
 * Достигнут ли конец текста?
 *
 * @param nav
 * @return
 */
MAGNA_API am_bool MAGNA_CALL nav_eot
    (
        const TextNavigator *nav
    )
{
    assert (nav != NULL);

    return nav->position >= nav->length;
}

/**
 * Подглядываем символ в указанной позиции
 * (отсчет от начала текста, от 0).
 *
 * @param nav
 * @param position
 * @return
 */
MAGNA_API int MAGNA_CALL nav_at
    (
        const TextNavigator *nav,
        am_size position
    )
{
    assert (nav != NULL);

    return position < nav->length
        ? nav->data[position]
        : NAV_EOT;
}

/**
 * Первый символ в тексте.
 *
 * @param nav
 * @return
 */
MAGNA_API int MAGNA_CALL nav_front
    (
        const TextNavigator *nav
    )
{
    assert (nav != NULL);

    return nav_at (nav, 0);
}

/**
 * Последний символ в тексте.
 *
 * @param nav
 * @return
 */
MAGNA_API int MAGNA_CALL nav_back
    (
        const TextNavigator *nav
    )
{
    assert (nav != NULL);

    return nav_at (nav, nav->length - 1);
}

/**
 * Заглядывание вперед на указанное количество символов.
 *
 * @param nav
 * @param distance
 * @return
 */
MAGNA_API int MAGNA_CALL nav_look_ahead
    (
        const TextNavigator *nav,
        am_size distance
    )
{
    am_size newPos;

    assert (nav != NULL);

    newPos = nav->position + distance;

    return nav_at (nav, newPos);
}

/**
 * Заглядывание назад на указанное количество символов.
 *
 * @param nav
 * @param distance
 * @return
 */
MAGNA_API int MAGNA_CALL nav_look_behind
    (
        const TextNavigator *nav,
        am_size distance
    )
{
    am_size newPos;

    assert (nav != NULL);

    newPos = nav->position - distance;

    return nav_at (nav, newPos);
}

/**
 * Перемещение по тексту вперед/назад.
 *
 * @param nav
 * @param distance
 * @return
 */
MAGNA_API TextNavigator* MAGNA_CALL nav_move
    (
        TextNavigator *nav,
        am_ssize distance
    )
{
    assert (nav != NULL);

    /* TODO: some checks */
    nav->column += distance;
    nav->position += distance;

    return nav;
}

/**
 * Подсматриваем текущий символ.
 *
 * @param nav
 * @return
 */
MAGNA_API int MAGNA_CALL nav_peek
    (
        const TextNavigator * nav
    )
{
    assert (nav != NULL);

    return nav_at (nav, nav->position);
}

/**
 * Подсматриваем текущий символ.
 * @param nav
 * @return
 */
MAGNA_API int MAGNA_CALL nav_peek_no_crlf
    (
        const TextNavigator *nav
    )
{
    am_size distance = 0;
    int result;

    assert (nav != NULL);

    while (AM_TRUE) {
        result = nav_at (nav, nav->position + distance);
        if (result != '\r' && result != '\n') {
            break;
        }
        ++distance;
    }

    return result;
}

/**
 * Считывание текущего символа, движение вперед по тексту.
 *
 * @param nav
 * @return
 */
MAGNA_API int MAGNA_CALL nav_read
    (
        TextNavigator *nav
    )
{
    int result;

    assert (nav != NULL);

    result = nav_at (nav, nav->position);
    if (result == '\n') {
        ++nav->line;
        nav->column = 1;
    }
    else {
        ++nav->column;
    }

    return result;
}

/**
 * Считывание текущего символа. Перевод строки и возврат каретки
 * пропускаются. Движение вперед по тексту.
 *
 * @param nav
 * @return
 */
MAGNA_API int MAGNA_CALL nav_read_no_crlf
    (
        TextNavigator *nav
    )
{
    int result;

    while (AM_TRUE) {
        result = nav_read (nav);
        if (result != '\r' && result != '\n') {
            break;
        }
    }

    return result;
}

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
