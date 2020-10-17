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
 * \file search.c
 *
 * Спецификация поиска в базе данных.
 *
 * Владеет собственной памятью. Для освобождения
 * ресурсов используйте `search_free`.
 *
 * Сценарий поиска.
 *
 * Владеет собственной памятью. Для освобождения
 * ресурсов используйте `scenario_free`.
 */

/*=========================================================*/

/**
 * Инициализация структуры.
 * Выделяет память в куче.
 *
 * @param search Указатель на неинициализированную структуру.
 * @param text Затравочное значение (не должно быть пустым).
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL search_create
    (
        Search *search,
        Span text
    )
{
    assert (search != NULL);
    assert (!span_is_empty (text));

    buffer_init (&search->buffer);

    return buffer_assign_span (&search->buffer, text);
}

/**
 * Освобождение ресурсов, занятых структурой.
 *
 * @param search Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL search_destroy
    (
        Search *search
    )
{
    assert (&search->buffer);

    buffer_destroy (&search->buffer);
    mem_clear (search, sizeof (*search));
}

MAGNA_API am_bool MAGNA_CALL search_equals
    (
        Search *search,
        Span prefix,
        Span value
    )
{
    assert (search != NULL);

    buffer_init (&search->buffer);

    return buffer_assign_span (&search->buffer, prefix)
        && buffer_write_span (&search->buffer, value);
}

MAGNA_API am_bool MAGNA_CALL search_and
    (
        Search *first,
        Span second
    )
{
    assert (first != NULL);
    assert (!span_is_empty (second));

    return AM_FALSE;
}

/*=========================================================*/

/**
 * Инициализация параметров поиска.
 * Не выделяет память в куче.
 *
 * @param parameters Параметры поиска, подлежащие инициализации.
 * @return Признак успешности завершения операции.
 */
MAGNA_API void MAGNA_CALL search_init_parameters
    (
        SearchParameters *parameters
    )
{
    assert (parameters != NULL);

    mem_clear (parameters, sizeof (*parameters));
    parameters->firstRecord = 1;
}

/**
 * Освобождение ресурсов, занятых параметрами поиска.
 *
 * @param parameters Структура, подлежащая освобождению.
 */
MAGNA_API void MAGNA_CALL search_destroy_parameters
    (
        SearchParameters *parameters
    )
{
    assert (parameters != NULL);

    buffer_destroy (&parameters->expression);
    buffer_destroy (&parameters->database);
    buffer_destroy (&parameters->format);
    buffer_destroy (&parameters->sequential);
    buffer_destroy (&parameters->filter);
    mem_clear (parameters, sizeof (*parameters));
}

/**
 * Инициализация сценария поиска.
 *
 * @param scenario Сценарий поиска, подлежащий иниализации.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL scenario_init
    (
        SearchScenario *scenario
    )
{
    assert (scenario != NULL);

    memset (scenario, 0, sizeof (SearchScenario));

    return AM_TRUE;
}

/**
 * Освобождение ресурсов, занятых сценарием поиска.
 *
 * @param scenario Сценарий поиска.
 */
MAGNA_API void MAGNA_CALL scenario_destroy
    (
        SearchScenario *scenario
    )
{
    assert (scenario != NULL);

    buffer_destroy(&scenario->name);
    buffer_destroy(&scenario->prefix);
    buffer_destroy(&scenario->menuName);
    buffer_destroy(&scenario->oldFormat);
    buffer_destroy(&scenario->correction);
    buffer_destroy(&scenario->truncation);
    buffer_destroy(&scenario->hint);
    buffer_destroy(&scenario->modByDicAuto);
    buffer_destroy(&scenario->logic);
    buffer_destroy(&scenario->advance);
    buffer_destroy(&scenario->format);
    memset (scenario, 0, sizeof (SearchScenario));
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
