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
 * Инициализация параметров поиска.
 *
 * @param parameters Параметры поиска, подлежащие инициализации.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL search_init
    (
        SearchParameters *parameters
    )
{
    assert (parameters != NULL);

    memset (parameters, 0, sizeof (SearchParameters));
    parameters->firstRecord = 1;

    return AM_TRUE;
}

/**
 * Освобождение ресурсов, занятых параметрами поиска.
 *
 * @param parameters Параметры поиска.
 */
MAGNA_API void MAGNA_CALL search_free
    (
        SearchParameters *parameters
    )
{
    assert (parameters != NULL);

    buffer_free (&parameters->expression);
    buffer_free (&parameters->database);
    buffer_free (&parameters->format);
    buffer_free (&parameters->sequential);
    buffer_free (&parameters->filter);
    memset (parameters, 0, sizeof (SearchParameters));
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
MAGNA_API void MAGNA_CALL scenario_free
    (
        SearchScenario *scenario
    )
{
    assert (scenario != NULL);

    buffer_free (&scenario->name);
    buffer_free (&scenario->prefix);
    buffer_free (&scenario->menuName);
    buffer_free (&scenario->oldFormat);
    buffer_free (&scenario->correction);
    buffer_free (&scenario->truncation);
    buffer_free (&scenario->hint);
    buffer_free (&scenario->modByDicAuto);
    buffer_free (&scenario->logic);
    buffer_free (&scenario->advance);
    buffer_free (&scenario->format);
    memset (scenario, 0, sizeof (SearchScenario));
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
