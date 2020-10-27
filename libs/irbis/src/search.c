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
#include <ctype.h>

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
 * \struct SearchParameters
 *      \brief Параметры для поиска в базе данных.
 *
 * \var SearchParameters::expression
 *      \brief Выражение для поиска по словарю (быстрый поиск).
 *      Не требуется, если задано выражение для последовательного поиска,
 *      в иных случаях обязательно.
 *
 * \var SearchParameters::database
 *      \brief Имя базы данных (опционально). Если не задано,
 *      поиск производится по текущей базе данных.
 *
 * \var SearchParameters::format
 *      \brief Опциональный формат для найденных записей.
 *
 * \var SearchParameters::sequential
 *      \brief Выражение для последовательного поиска (медленный поиск).
 *      Опционально.
 *
 * \var SearchParameters::filter
 *      \brief Фильтр для записей (фильтрация производится на клиенте).
 *      Опционально.
 *
 * \var SearchParameters::firstRecord
 *      \brief Индекс первой из возвращаемых записей. По умолчанию `1`.
 *      `0` означает, что будет возвращено только количество
 *      найденных записей.
 *
 * \var SearchParameters::minMfn
 *      \brief Минимальный MFN для поиска (опционально).
 *
 * \var SearchParameters::maxMfn
 *      \brief Максимальный MFN для поиско (опционально).
 *
 * \var SearchParameters::number
 *      \brief Количество возвращаемых записей (`0` означает "все").
 */

/**
 * Инициализация параметров поиска строкой-выражением для поиска по словарю.
 * Размещает в куче копию строки-выражения.
 *
 * @param parameters Указатель на неинициализированную структуру.
 * @param expression Строка с поисковым выражением.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL search_parameters_create
    (
        SearchParameters *parameters,
        const am_byte *expression
    )
{
    assert (parameters != NULL);
    assert (expression != NULL);

    search_parameters_init (parameters);

    return buffer_assign_text (&parameters->expression, expression);
}

/**
 * Простая инициализация параметров поиска.
 * Не выделяет память в куче.
 *
 * @param parameters Параметры поиска, подлежащие инициализации.
 * @return Признак успешного завершения операции.
 */
MAGNA_API void MAGNA_CALL search_parameters_init
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
MAGNA_API void MAGNA_CALL search_parameters_destroy
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
 * Кодирование параметров поиска в клиентском запросе.
 *
 * @param parameters Параметры поиска.
 * @param connection Активное подключение к серверу.
 * @param query Указатель на структуру клиентского запроса.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL search_parameters_encode
    (
        const SearchParameters *parameters,
        const Connection *connection,
        Query *query
    )
{
    Buffer *database; /* имя базы данных */

    assert (parameters != NULL);
    assert (connection != NULL);
    assert (query != NULL);

    /* Если база данных не указана явно, используем текущую. */
    database = choose_buffer (&parameters->database, &connection->database, NULL);

    return query_add_ansi_buffer (query, database)
        && query_add_utf_buffer (query, &parameters->expression)
        && query_add_uint32 (query, parameters->number)
        && query_add_uint32 (query, parameters->firstRecord)
        /* TODO: сделать правильное добавление формата */
        && query_add_ansi_buffer (query, &parameters->format)
        && query_add_uint32 (query, parameters->minMfn)
        && query_add_uint32 (query, parameters->maxMfn)
        && query_add_ansi_buffer (query, &parameters->sequential);
}


/**
 * Верификация параметров поиска.
 *
 * @param parameters Указатель на структуру параметров.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL search_parameters_verify
    (
        const SearchParameters *parameters
    )
{
    assert (parameters != NULL);

    /* TODO: implement */

    return AM_TRUE;
}

/*=========================================================*/

/* Строка из списка найденных записей. */

/**
 * \struct FoundLine
 *      \brief Одна строка из списка найденных записей.
 *      \details Владеет своей памятью.
 *      Для освобождения использовать `found_destroy`.
 *
 * \var FoundLine::mfn
 *      \brief MFN найденной записи.
 *
 * \var FoundLine::description
 *      \brief Опциональный текст, например, результат расформатирования записи.
 */

/**
 * Простая инициализация структуры.
 * Не выделяет память в куче.
 *
 * @param found Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL found_init
    (
        FoundLine *found
    )
{
    assert (found != NULL);

    mem_clear (found, sizeof (*found));
}

/**
 * Освобождение ресурсов, занимаемых структурой.
 *
 * @param found Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL found_destroy
    (
        FoundLine *found
    )
{
    assert (found != NULL);

    buffer_destroy (&found->description);
    mem_clear (found, sizeof (*found));
}

/**
 * Инициализация массива найденных строк.
 *
 * @param array Указатель на неинициализированный массив.
 */
MAGNA_API void MAGNA_CALL found_array_init
    (
        Array *array
    )
{
    assert (array != NULL);

    array_init (array, sizeof (FoundLine));
}

/**
 * Освобождение ресурсов, занятых массивом найденых строк.
 *
 * @param array Указатель на массив, подлежащий освобождению.
 */
MAGNA_API void MAGNA_CALL found_array_destroy
    (
        Array *array
    )
{
    assert (array != NULL);

    array_destroy (array, (Liberator) found_destroy);
}

/**
 * Декодирование строки.
 *
 * @param found Указатель на инициализированную структуру.
 * @param line Строка с информацией о найденной записи.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL found_decode_line
    (
        FoundLine *found,
        Span line
    )
{
    Span parts[2];

    assert (found != NULL);

    if (!span_contains (line, '#')) {
        found->mfn = span_to_uint32 (line);
    }
    else {
        buffer_clear (&found->description);
        if (span_split_n_by_char (line, parts, 2, '#') == 2) {
            if (!buffer_assign_span (&found->description, parts[1])) {
                return AM_FALSE;
            }
        }

        found->mfn = span_to_uint32 (parts[0]);
    }

    return AM_TRUE;
}

/**
 * Разбор только MFN из ответа сервера.
 *
 * @param array Массив MFN.
 * @param response Ответ сервера.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL found_decode_response_mfn
    (
        Int32Array *array,
        Response *response
    )
{
    am_mfn mfn;
    Span line, number;

    assert (array != NULL);
    assert (response != NULL);

    while (!response_eot (response)) {
        line = response_get_line (response);
        if (span_is_empty (line)) {
            break;
        }

        number.start = line.start;
        for (number.end = line.start; number.end != line.end; ++number.end) {
            if (!isdigit (*number.end)) {
                break;
            }
        }
        mfn = span_to_uint32 (number);
        if (mfn != 0) {
            if (!int32_array_push_back(array, mfn)) {
                return AM_FALSE;
            }
        }
    }

    return AM_TRUE;
}

/**
 * Полное декодирование ответа сервера.
 *
 * @param array Инициализированный массив для заполнения.
 * @param response Ответ сервера.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL found_decode_response
    (
        Array *array,
        Response *response
    )
{
    FoundLine *found;
    Span line;

    assert (array != NULL);
    assert (response != NULL);

    while (!response_eot (response)) {
        line = response_get_line (response);
        if (span_is_empty (line)) {
            break;
        }

        found = (FoundLine*) array_emplace_back (array);
        if (found == NULL) {
            return AM_FALSE;
        }

        found_init (found);
        if (!found_decode_line (found, line)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/*=========================================================*/

/**
 * \struct SearchScenario
 *      \brief Сценарий поиска.
 *
 * \var SearchScenario::name
 *      \brief Наименование поискового атрибута
 *      ("Автор", "Заглавие" и т. п.). Обязательный элемент.
 *
 * \var SearchScenario::prefix
 *      \brief Префикс соответствующих терминов в поисковом словаре
 *      (может быть пустым).
 *
 * \var SearchScenario::menuName
 *      \brief Имя файла справочника (меню). Опционально.
 *
 * \var SearchScenario::oldFormat
 *      \brief Имя формата (без расширения). Опционально.
 *
 * \var SearchScenario::correction
 *      \brief Способ корректировки по словарю. Опционально.
 *
 * \var SearchScenario::truncation
 *      \brief Исходное положение переключателя "усечение". Опционально.
 *
 * \var SearchScenario::hint
 *      \brief Текст подсказки/предупреждения. Опционально.
 *
 * \var SearchScenario::modByDicAuto
 *      \brief Параметр пока не задействован.
 *
 * \var SearchScenario::logic
 *      \brief Применимые логические операторы. Опционально.
 *
 * \var SearchScenario::advance
 *      \brief Правила автоматического расширения поиска
 *      на основе авторитетного файла или тезауруса. Опционально.
 *
 * \var SearchScenario::format
 *      \brief Имя формата показа документов. Опционально.
 *
 * \var SearchScenario::dictionaryType
 *      \brief Тип словаря для соответствующего поиска. Опционально.
 */

/**
 * Простая нициализация сценария поиска.
 * Не выделяет памяти из кучи.
 *
 * @param scenario Указатель на неинициализированную структуру..
 */
MAGNA_API void MAGNA_CALL scenario_init
    (
        SearchScenario *scenario
    )
{
    assert (scenario != NULL);

    mem_clear (scenario, sizeof (*scenario));
}

/**
 * Освобождение ресурсов, занятых структурой.
 *
 * @param scenario Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL scenario_destroy
    (
        SearchScenario *scenario
    )
{
    assert (scenario != NULL);

    buffer_destroy (&scenario->name);
    buffer_destroy (&scenario->prefix);
    buffer_destroy (&scenario->menuName);
    buffer_destroy (&scenario->oldFormat);
    buffer_destroy (&scenario->correction);
    buffer_destroy (&scenario->truncation);
    buffer_destroy (&scenario->hint);
    buffer_destroy (&scenario->modByDicAuto);
    buffer_destroy (&scenario->logic);
    buffer_destroy (&scenario->advance);
    buffer_destroy (&scenario->format);
    mem_clear (scenario, sizeof (SearchScenario));
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
