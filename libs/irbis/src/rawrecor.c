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
 * \file rawrecor.c
 *
 * Сырая запись.
 */

/*=========================================================*/

/**
 * Простая инициализация структуры.
 * Не выделяет память в куче.
 *
 * @param record
 */
MAGNA_API void MAGNA_CALL raw_record_init
    (
        RawRecord *record
    )
{
    assert (record != NULL);

    mem_clear (record, sizeof (*record));
}

/**
 * Освобождение ресурсов, занятых записью.
 *
 * @param record Запись, подлежащая освобождению.
 */
MAGNA_API void MAGNA_CALL raw_record_destroy
    (
        RawRecord *record
    )
{
    size_t index;
    Buffer *field;

    assert (record != NULL);

    for (index = 0; index < record->fields.len; ++index) {
        field = (Buffer*) array_get (&record->fields, index);
        buffer_destroy (field);
    }

    array_destroy (&record->fields);
}

/**
 * Разбор ответа сервера для ситуации, когда сервер присылает одну запись.
 *
 * @param record Запись, которая должна быть заполнена.
 * @param response Ответ сервера.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL raw_record_parse_single
    (
        RawRecord *record,
        void *response
    )
{
    Span line;
    Span parts[2];
    size_t nparts;
    Buffer *field;

    assert (record != NULL);
    assert (response != NULL);

    line = response_get_line (response);
    nparts = span_split_n_by_char (line, parts, 2, '#');
    record->mfn = span_to_uint32 (parts[0]);
    record->status = 0;
    if (nparts == 2) {
        record->status = span_to_uint32 (parts[1]);
    }

    line = response_get_line (response);
    record->version = 0;
    nparts = span_split_n_by_char (line, parts, 2, '#');
    if (nparts == 2) {
        record->version = span_to_uint32 (parts[1]);
    }

    array_clear (&record->fields);
    while (!response_eot (response)) {
        line = response_get_line (response);
        if (!span_is_empty (line)) {
            field = (Buffer*) array_emplace_back (&record->fields);
            if (field == NULL) {
                return AM_FALSE;
            }

            buffer_init (field);
            if (!buffer_assign_span (field, line)) {
                return AM_FALSE;
            }
        }
    }

    return AM_TRUE;
}

/**
 * Текстовое представление записи.
 *
 * @param record Запись.
 * @param output Буфер для размещения результата.
 * @param delimiter Разделитель полей.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL raw_record_to_string
    (
        const RawRecord *record,
        Buffer *output,
        const am_byte *delimiter
    )
{
    size_t index;
    const Buffer *field;

    assert (record != NULL);
    assert (output != NULL);

    if (!buffer_put_uint32 (output, record->mfn)
        || !buffer_putc (output, '#')
        || !buffer_put_uint32 (output, record->status)
        || !buffer_puts (output, delimiter)
        || !buffer_puts (output, CBTEXT ("0#"))
        || !buffer_put_uint32 (output, record->version)
        || !buffer_puts (output, delimiter)) {
        return AM_FALSE;
    }

    for (index = 0; index < record->fields.len; ++index) {
        field = (const Buffer*) array_get (&record->fields, index);
        if (!buffer_concat (output, field)
            || !buffer_puts (output, delimiter)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
