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
 * Работа с терминами словаря.
 *
 * \struct TermPosting
 * \brief Постинг термина.
 *
 * \var TermPosting::mfn
 *      \brief MFN записи с искомым термином.
 *
 * \var TermPosting::tag
 *      \brief Метка поля с искомым термином.
 *
 * \var TermPosting::occurrence
 *      \brief Номер повторения поля.
 *
 * \var TermPosting::count
 *      \brief Смещение (номер слова) в повторении поля.
 *
 * \var TermPosting::text
 *      \brief Результат расформатирования (если есть).
 *
 */

/*=========================================================*/

/**
 * Простая инициализация структуры.
 * Не выделяет память в куче.
 *
 * @param term Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL term_init
    (
        TermInfo *term
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
        TermInfo *term
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

    array_init (terms, sizeof (TermInfo));
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
    size_t index;
    TermInfo *term;

    assert (terms != NULL);

    for (index = 0; index < terms->len; ++index) {
        term = (TermInfo*) array_get (terms, index);
        term_destroy (term);
    }

    array_destroy (terms);
    mem_clear (terms, sizeof (*terms));
}

/**
 * Разбор строки с текстовым представлением термина.
 *
 * @param term Термин, подлежащий заполнению.
 * @param line Строка с текстовым представлением.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL term_parse_line
    (
        TermInfo *term,
        const Span line
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
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL term_parse_response
    (
        Array *terms,
        Response *response
    )
{
    Span line;

    assert (terms != NULL);

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Текстовое представление термина.
 *
 * @param term Термин.
 * @param output Буффер для размещения результата.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL term_to_string
    (
        const TermInfo *term,
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
 * Простая инициализация структуры.
 * Не выделяет памяти в куче.
 *
 * @param posting Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL posting_init
    (
        TermPosting *posting
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
MAGNA_API void MAGNA_CALL posting_init_array
    (
        Array *postings
    )
{
    assert (postings != NULL);

    array_init (postings, sizeof (TermPosting));
}

/**
 * Освобождение ресурсов, занятых структурой.
 *
 * @param posting Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL posting_destroy
    (
        TermPosting *posting
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
MAGNA_API void MAGNA_CALL posting_destroy_array
    (
        Array *postings
    )
{
    size_t index;
    TermPosting *posting;

    assert (postings != NULL);

    for (index = 0; index < postings->len; ++index) {
        posting = (TermPosting*) array_get (postings, index);
        posting_destroy (posting);
    }

    array_destroy (postings);
}

/**
 * Разбор строки с текстовым представлением постинга.
 *
 * @param posting Постинг (инициализированный), подлежащий заполнению.
 * @param line Текстовое представление постинга.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL posting_parse_line
    (
        TermPosting *posting,
        Span line
    )
{
    Span parts[5];
    size_t nparts;

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
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL posting_parse_response
    (
        Array *postings,
        Response *response
    )
{
    assert (postings != NULL);
    assert (response != NULL);

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Текстовое представление постинга.
 *
 * @param posting Постинг.
 * @param output Буфер для размещения результата.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL posting_to_string
    (
        const TermPosting *posting,
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

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
