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
 * \file term.c
 *
 * Работа с терминами словаря и их постингами.
 */

/*=========================================================*/

/**
 * \struct Term
 *      \brief Информация о термине поискового словаря.
 *
 * \var Term::count
 *      \brief Количество постингов (вхождений) термина в поисковом словаре.
 *
 * \var Term::text
 *      \brief Собственно значение термина.
 */

/**
 * Простая инициализация структуры.
 * Не выделяет память в куче.
 *
 * @param term Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL term_init
    (
        Term *term
    )
{
    assert (term != NULL);

    mem_clear (term, sizeof (*term));
}

/**
 * Освобождение памяти, занятой структурой.
 *
 * @param term Структура, подлежащая освобождению.
 */
MAGNA_API void MAGNA_CALL term_destroy
    (
        Term *term
    )
{
    assert (term != NULL);

    buffer_destroy (&term->text);
    mem_clear (term, sizeof (*term));
}

/**
 * Инициализация динамического массива терминов.
 * Не выделяет память в куче.
 *
 * @param terms Указатель на неинициализированный массив.
 */
MAGNA_API void MAGNA_CALL term_init_array
    (
        Array *terms
    )
{
    assert (terms != NULL);

    array_init (terms, sizeof (Term));
}

/**
 * Освобождение ресурсов, занятых массивом терминов.
 *
 * @param terms Массив терминов, подлежащий освобождению.
 */
MAGNA_API void MAGNA_CALL term_destroy_array
    (
        Array *terms
    )
{
    assert (terms != NULL);

    array_destroy (terms, (Liberator) term_destroy);
    mem_clear (terms, sizeof (*terms));
}

/**
 * Разбор строки с текстовым представлением термина.
 *
 * @param term Термин, подлежащий заполнению (инициализированный).
 * @param line Строка с текстовым представлением.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL term_parse_line
    (
        Term *term,
        Span line
    )
{
    Span parts[2];

    assert (term != NULL);

    if (span_split_n_by_char (line, parts, 2, '#') != 2) {
        return AM_FALSE;
    }

    term->count = span_to_uint32 (parts[0]);

    return buffer_assign_span (&term->text, parts[1]);
}

/**
 * Разбор ответа сервера -- получение массива терминов.
 *
 * @param terms Массив (инициализированный), подлежащий заполнению.
 * @param response Ответ сервера.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL term_parse_response
    (
        Array *terms,
        Response *response
    )
{
    Span line;
    Term *term;

    assert (terms != NULL);
    assert (response != NULL);

    while (!response_eot (response)) {
        line = response_get_line (response);
        if (span_is_empty (line)) {
            break;
        }

        term = (Term*) array_emplace_back (terms);
        if (term == NULL) {
            return AM_FALSE;
        }

        term_init (term);
        if (!term_parse_line (term, line)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/**
 * Текстовое представление термина.
 *
 * @param term Термин.
 * @param output Буффер для размещения результата.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL term_to_string
    (
        const Term *term,
        Buffer *output
    )
{
    assert (term != NULL);
    assert (output != NULL);

    return buffer_put_uint32 (output, term->count)
        && buffer_putc (output, '#')
        && buffer_concat (output, &term->text);
}

/**
 * Вывод термина в консоль.
 *
 * @param term Термин для вывода.
 */
MAGNA_API void MAGNA_CALL term_to_console
    (
        const Term *term
    )
{
    am_byte temp[12];

    assert (term != NULL);

    sprintf ((char*) temp, "%u#", (unsigned) term->count);
    fputs ((const char*) temp, stdout);
    buffer_to_console (&term->text);
}

/**
 * Инициализация массива терминов.
 *
 * @param terms Указатель на неинициализированный массив.
 */
MAGNA_API void MAGNA_CALL term_array_init
    (
        Array *terms
    )
{
    assert (terms != NULL);

    array_init (terms, sizeof (Term));
}

/**
 * Освобождение ресурсов, занятых массивом терминов.
 *
 * @param array Указатель на массив, подлежащий освобождению.
 */
MAGNA_API void MAGNA_CALL term_array_destroy
    (
        Array *terms
    )
{
    assert (terms != NULL);

    array_destroy (terms, (Liberator) term_destroy);
}

/**
 * Вывод массива терминов на консоль.
 *
 * @param terms Массив терминов.
 * @param separator Разделитель терминов.
 */
MAGNA_API void MAGNA_CALL term_array_to_console
    (
        const Array *terms,
        const am_byte *separator
    )
{
    size_t index;
    const Term *term;

    assert (terms != NULL);

    for (index = 0; index < terms->len; ++index) {
        term = (const Term*) array_get (terms, index);
        term_to_console (term);
        if (separator != NULL) {
            fputs (CCTEXT (separator), stdout);
        }
    }
}

/*=========================================================*/

/**
 * \struct TermParameters
 *      \brief Параметры для запроса терминов с сервера.
 *
 * \var TermParameters::database
 *      \brief Имя базы данных.
 *
 * \var TermParameters::number
 *      \brief Количество затребуемых терминов.
 *
 * \var TermParameters::reverseOrder
 *      \brief Выдавать термины в обратном порядке?
 *
 * \var TermParameters::format
 *      \brief Опциональный формат.
 *
 * \var TermParameters::startTerm
 *      \brief Стартовый термин. Обязательное поле.
 */

/**
 * Инициализация параметров с указанием стартового термина.
 * Размещает в куче копию заданного терма.
 *
 * @param parameters Указатель на неинициализированную структуру.
 * @param startTerm Стартовый термин.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL term_parameters_create
    (
        TermParameters *parameters,
        const am_byte *startTerm
    )
{
    assert (parameters != NULL);
    assert (startTerm != NULL);

    term_parameters_init (parameters);

    return buffer_assign_text (&parameters->startTerm, startTerm);
}

/**
 * Простая инициализация структуры.
 * Не выделяет памяти в куче.
 *
 * @param parameters Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL term_parameters_init
    (
        TermParameters *parameters
    )
{
    assert (parameters != NULL);

    mem_clear (parameters, sizeof (*parameters));
}

/**
 * Освобождение ресурсов, занятых структурой.
 *
 * @param parameters Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL term_parameters_destroy
    (
        TermParameters *parameters
    )
{
    assert (parameters != NULL);

    buffer_destroy (&parameters->database);
    buffer_destroy (&parameters->startTerm);
    buffer_destroy (&parameters->format);
    mem_clear (parameters, sizeof (*parameters));
}

/**
 * Кодирование параметров терминов в клиентском запросе.
 *
 * @param parameters Параметры терминов.
 * @param connection Активное подключение к серверу.
 * @param query Указзатель на структуру клиентского запроса.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL term_parameters_encode
    (
        const TermParameters *parameters,
        const Connection *connection,
        Query *query
    )
{
    const Buffer *database; /* имя базы данных */

    assert (parameters != NULL);
    assert (connection != NULL);
    assert (query != NULL);

    /* Если база данных не указана явно, используем текущую. */
    database = choose_buffer (&parameters->database, &connection->database, NULL);

    return query_add_ansi_buffer (query, database)
        && query_add_utf_buffer (query, &parameters->startTerm)
        && query_add_uint32 (query, parameters->number)
           /* TODO: сделать правильное добавление формата */
        && query_add_ansi_buffer (query, &parameters->format);
}

/**
 * Верификация параметров терминов.
 *
 * @param parameters Проверяемые параметры.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL term_parameters_verify
    (
        const TermParameters *parameters
    )
{
    assert (parameters != NULL);

    /* TODO: implement */

    return AM_TRUE;
}

/*=========================================================*/

/**
 * \struct Posting
 *      \brief Постинг (вхождение) термина в поисковом индексе.
 *
 * \var Posting::mfn
 *      \brief MFN записи с искомым термином.
 *
 * \var Posting::tag
 *      \brief Метка поля с искомым термином.
 *
 * \var Posting::occurrence
 *      \brief Номер повторения поля.
 *
 * \var Posting::count
 *      \brief Смещение (номер слова) в повторении поля.
 *
 * \var Posting::text
 *      \brief Результат расформатирования (если есть).
 */

/**
 * Простая инициализация структуры.
 * Не выделяет памяти в куче.
 *
 * @param posting Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL posting_init
    (
        Posting *posting
    )
{
    assert (posting != NULL);

    mem_clear (posting, sizeof (*posting));
}

/**
 * Инициализация динамического массива постингов.
 * Не выделяет память в куче.
 *
 * @param postings Указатель на неинициализированный массив.
 */
MAGNA_API void MAGNA_CALL posting_array_init
    (
        Array *postings
    )
{
    assert (postings != NULL);

    array_init (postings, sizeof (Posting));
}

/**
 * Освобождение ресурсов, занятых структурой.
 *
 * @param posting Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL posting_destroy
    (
        Posting *posting
    )
{
    assert (posting != NULL);

    buffer_destroy (&posting->text);
    mem_clear (posting, sizeof (*posting));
}

/**
 * Освобождение ресурсов, занятых массивом постингов.
 *
 * @param postings Массив постингов, подлежащий освобождению.
 */
MAGNA_API void MAGNA_CALL posting_array_destroy
    (
        Array *postings
    )
{
    assert (postings != NULL);

    array_destroy (postings, (Liberator) posting_destroy);
}

/**
 * Разбор строки с текстовым представлением постинга.
 *
 * @param posting Постинг (инициализированный), подлежащий заполнению.
 * @param line Текстовое представление постинга.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL posting_parse_line
    (
        Posting *posting,
        Span line
    )
{
    Span parts[5]; /* компоненты постинга */
    size_t nparts; /* число компонент */

    assert (posting != NULL);

    nparts = span_split_n_by_char (line, parts, 5, '#');
    if (nparts < 4) {
        return AM_FALSE;
    }

    posting->mfn        = span_to_uint32 (parts[0]);
    posting->tag        = span_to_uint32 (parts[1]);
    posting->occurrence = span_to_uint32 (parts[2]);
    posting->count      = span_to_uint32 (parts[3]);
    if (nparts > 4) {
        return buffer_assign_span (&posting->text, parts[4]);
    }

    return AM_TRUE;
}

/**
 * Разбор ответа сервера -- получение массива постингов.
 *
 * @param postings Массив (инициализированный), подлежащий заполнению.
 * @param response Ответ сервера.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL posting_parse_response
    (
        Array *postings,
        Response *response
    )
{
    Span line;        /* текущая строка */
    Posting *posting; /* размещенный в массиве постинг */

    assert (postings != NULL);
    assert (response != NULL);

    while (!response_eot (response)) {
        line = response_get_line (response);
        if (span_is_empty (line)) {
            break;
        }

        posting = array_emplace_back (postings);
        if (posting == NULL) {
            return AM_FALSE;
        }

        posting_init (posting);
        if (!posting_parse_line (posting, line)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/**
 * Текстовое представление постинга.
 *
 * @param posting Постинг.
 * @param output Буфер для размещения результата.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL posting_to_string
    (
        const Posting *posting,
        Buffer *output
    )
{
    assert (posting != NULL);
    assert (output != NULL);

    return buffer_put_uint32 (output, posting->mfn)
        && buffer_putc (output, '#')
        && buffer_put_uint32 (output, posting->tag)
        && buffer_putc (output, '#')
        && buffer_put_uint32 (output, posting->occurrence)
        && buffer_putc (output, '#')
        && buffer_put_uint32 (output, posting->count)
        && buffer_putc (output, '#')
        && buffer_concat (output, &posting->text);
}

/**
 * Вывод постинга в консоль.
 *
 * @param posting Постинг для вывода.
 */
MAGNA_API void MAGNA_CALL posting_to_console
    (
        const Posting *posting
    )
{
    am_byte temp [12]; /* временный буфер */

    assert (posting != NULL);

    sprintf ((char*) temp, "%u#", (unsigned) posting->mfn);
    fputs ((const char*) temp, stdout);
    sprintf ((char*) temp, "%u#", (unsigned) posting->tag);
    fputs ((const char*) temp, stdout);
    sprintf ((char*) temp, "%u#", (unsigned) posting->occurrence);
    fputs ((const char*) temp, stdout);
    sprintf ((char*) temp, "%u#", (unsigned) posting->count);
    fputs ((const char*) temp, stdout);
    buffer_to_console (&posting->text);
}

MAGNA_API void MAGNA_CALL posting_array_to_console
    (
        const Array *postings,
        const am_byte *separator
    )
{
    size_t index;           /* индекс в массиве */
    const Posting *posting; /* указатель на постинг в массиве */

    assert (postings != NULL);

    for (index = 0; index < postings->len; ++index) {
        posting = (const Posting*) array_get (postings, index);
        posting_to_console (posting);
        if (separator != NULL) {
            fputs ((const char*) separator, stdout);
        }
    }
}

/*=========================================================*/

/**
 * \struct PostingParameters
 *      \brief Параметры для запроса постингов с сервера.
 *
 * \var PostingParameters::terms
 *      \brief Массив терминов, для которых требуются постинги.
 *
 * \var PostingParameters::database
 *      \brief Имя базы данных.
 *
 * \var PostingParameters::format
 *      \brief Опциональный формат.
 *
 * \var PostingParameters::first
 *      \brief Номер первого постинга (нумерация с 1).
 *
 * \var PostingParameters::number
 *      \brief Количество затребуемых постингов.
 */

/**
 * Инициализация параметров постингов.
 * Размещает в куче копию указанного термина.
 *
 * @param parameters Указатель на неинициализированную структуру.
 * @param term Текст термина.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL posting_parameters_create
    (
        PostingParameters *parameters,
        const am_byte *term
    )
{
    Term *ptr; /* указатель на термин, размещенный в массиве */

    assert (parameters != NULL);
    assert (term != NULL);

    posting_parameters_init (parameters);
    ptr = (Term*) array_emplace_back (&parameters->terms);
    if (ptr == NULL) {
        return AM_FALSE;
    }

    term_init (ptr);

    return buffer_assign_text (&ptr->text, term);
}

/**
 * Простая инициализация параметров постингов.
 * Не выделяет памяти в куче.
 *
 * @param parameters Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL posting_parameters_init
    (
        PostingParameters *parameters
    )
{
    assert (parameters != NULL);

    mem_clear (parameters, sizeof (*parameters));
    array_init (&parameters->terms, sizeof (Term));
    parameters->first = 1;
}

/**
 * Освобождение памяти, занятой структурой.
 *
 * @param parameters Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL posting_parameters_destroy
    (
        PostingParameters *parameters
    )
{
    array_destroy  (&parameters->terms, (Liberator) term_destroy);
    buffer_destroy (&parameters->database);
    buffer_destroy (&parameters->format);
    mem_clear (parameters, sizeof (*parameters));
}

/**
 * Кодирование параметров терминов в клиентском запросе.
 *
 * @param parameters Параметры терминов.
 * @param connection Активное подключение к серверу.
 * @param query Указзатель на структуру клиентского запроса.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL posting_parameters_encode
    (
        const PostingParameters *parameters,
        const Connection *connection,
        Query *query
    )
{
    const Buffer *database; /* имя базы данных */
    size_t index;           /* переменная цикла */
    const Term *term;       /* текущий термин */

    assert (parameters != NULL);
    assert (connection != NULL);
    assert (query != NULL);

    /* Если база данных не указана явно, используем текущую. */
    database = choose_buffer (&parameters->database, &connection->database, NULL);

    if (!query_add_ansi_buffer (query, database)
           || !query_add_uint32 (query, parameters->number)
           || !query_add_uint32 (query, parameters->first)
           /* TODO: сделать правильное добавление формата */
           || !query_add_ansi_buffer (query, &parameters->format)) {
        return AM_FALSE;
    }

    for (index = 0; index < parameters->terms.len; ++index) {
        term = (const Term*) array_get (&parameters->terms, index);
        if (!query_add_utf_buffer (query, &term->text)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/**
 * Верификация параметров постингов.
 *
 * @param parameters Параметры постингов, подлежащие проверке.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL posting_parameters_verify
    (
        const PostingParameters *parameters
    )
{
    assert (parameters != NULL);

    /* TODO: implement */

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
