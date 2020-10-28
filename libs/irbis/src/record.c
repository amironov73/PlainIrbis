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
#include <ctype.h>

/*=========================================================*/

/*!
 * \file record.c
 *
 * Запись в формате MARC.
 */

/*=========================================================*/

/**
 * Простая инициализация структуры.
 * Не выделяет память в куче.
 *
 * @param record Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL record_init
    (
        MarcRecord *record
    )
{
    assert (record != NULL);

    mem_clear (record, sizeof (*record));
    array_init (&record->fields, sizeof (MarcField));
}

/**
 * Освобождение ресурсов, занятых записью.
 *
 * @param record Запись, подлежащая освобождению.
 */
MAGNA_API void MAGNA_CALL record_destroy
    (
        MarcRecord *record
    )
{
    assert (record != NULL);

    array_destroy (&record->fields, (Liberator) field_destroy);
}

/**
 * Добавление в конец записи поля с указанными меткой и значением.
 *
 * @param record Запись.
 * @param tag Метка добавляемого поля.
 * @param value Значение поля до первого разделителя (может быть `NULL`).
 * @return Вновь созданное поле.
 */
MAGNA_API MarcField* MAGNA_CALL record_add
    (
        MarcRecord *record,
        am_uint32 tag,
        const am_byte *value
    )
{
    MarcField *field;

    assert (record != NULL);

    field = calloc (1, sizeof (MarcField));
    field->tag = tag;
    buffer_from_text (&field->value, CBTEXT (value));
    array_push_back (&record->fields, field);

    return field;
}

/**
 * Удаление всех полей из записи.
 *
 * @param record Запись.
 */
MAGNA_API void MAGNA_CALL record_clear
    (
        MarcRecord *record
    )
{
    size_t index;
    MarcField *field;

    assert (record != NULL);

    for (index = 0; index < record->fields.len; ++index) {
        field = (MarcField*) array_get (&record->fields, index);
        field_destroy (field);
    }

    array_clear (&record->fields);
}

/**
 * Создание клона (глубокой копии записи).
 *
 * @param target Инициализированная запись.
 * @param source Запись, подлежащая копированию.
 * @return Копия.
 */
MAGNA_API MarcRecord* MAGNA_CALL record_clone
    (
        MarcRecord *target,
        const MarcRecord *source
    )
{
    size_t index;
    const MarcField *src;
    MarcField *dst;

    assert (target != NULL);
    assert (source != NULL);

    target->mfn = source->mfn;
    target->status = source->status;
    target->version = source->version;
    buffer_clone (&target->database, &source->database);
    if (!array_grow (&target->fields, source->fields.len)) {
        return NULL;
    }

    for (index = 0; index < source->fields.len; ++index) {
        src = (const MarcField*) array_get(&source->fields, index);
        dst = (MarcField*) calloc (1, sizeof (MarcField));
        if (dst == NULL) {
            return NULL;
        }

        field_create (dst);
        dst->tag = src->tag;
        field_clone (dst, src);
        array_push_back (&target->fields, dst);
    }

    return target;
}

/**
 * Разбор представления записи в виде массива строк.
 *
 * @param record Инициализированная запись.
 * @param lines Массив строк.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL record_decode_lines
    (
        MarcRecord *record,
        Vector *lines
    )
{
    assert (record != NULL);
    assert (lines != NULL);

    return AM_FALSE;
}

/**
 * Кодирование записи в текстовую форму.
 *
 * @param record Запись.
 * @param delimiter Разделитель строк (может быть `NULL`).
 * @param buffer Буфер для результата.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL record_encode
    (
        const MarcRecord *record,
        const char *delimiter,
        Buffer *buffer
    )
{
    assert (record != NULL);
    assert (buffer != NULL);

    return AM_FALSE;
}

/**
 * Получение значения поля/подполя.
 *
 * @param record Запись.
 * @param tag Искомая метка поля.
 * @param code Код подполя. 0 означает выдачу значения поля
 * до первого разделителя.
 * @return Значение поля/подполя (возможно, пустой фрагмент).
 */
MAGNA_API Span MAGNA_CALL record_fm
    (
        const MarcRecord *record,
        am_uint32 tag,
        am_byte code
    )
{
    size_t i, j;
    const MarcField *field;
    const SubField *subfield;

    Span result = SPAN_INIT;

    assert (record != NULL);
    for (i = 0; i < record->fields.len; ++i) {
        field = (const MarcField *) array_get (&record->fields, i);
        if (field->tag == tag) {
            if (code) {
                result = buffer_to_span (&field->value);
                goto DONE;
            }
            else {
                for (j = 0; j < field->subfields.len; ++j) {
                    subfield = field_get_subfield_by_index (field, j);
                    if (same_char (code, subfield->code)) {
                        result = buffer_to_span (&subfield->value);
                        goto DONE;
                    }
                }
            }
        }
    }

    DONE:
    return result;
}

/**
 * Получение массива значений поля/подполя.
 *
 * @param record Запись.
 * @param array Массив, в который должен быть помещен результат.
 * @param tag Искомая метка поля.
 * @param code Код подполя.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL record_fma
    (
        const MarcRecord *record,
        Vector *array,
        am_uint32 tag,
        am_byte code
    )
{
    assert (record != NULL);
    assert (array != NULL);

    return AM_FALSE;
}

/**
 * Получение ссылки на поле по его индексу (не по метке!).
 *
 * @param record Запись.
 * @param index Индекс поля (нумерация с 0).
 * @return Ссылка на поле.
 */
MAGNA_API MarcField* MAGNA_CALL record_get
    (
        const MarcRecord *record,
        size_t index
    )
{
    assert (record != NULL);
    assert (index < record->fields.len);

    return (MarcField*) array_get (&record->fields, index);
}

/**
 * Получение указателя на поле с указанной меткой.
 *
 * @param record Запись.
 * @param tag Искомая метка поля.
 * @param occurrence Повторение поля (нумерация с 0).
 * @return Указатель на поле либо `NULL`.
 */
MAGNA_API MarcField* MAGNA_CALL record_get_field
    (
        const MarcRecord *record,
        am_uint32 tag,
        size_t occurrence
    )
{
    const MarcField *field;
    size_t i;

    assert (record != NULL);

    for (i = 0; i < record->fields.len; ++i) {
        field = (const MarcField *) array_get (&record->fields, i);
        if (field->tag == tag) {
            if (!occurrence) {
                return (MarcField*) field;
            }
            --occurrence;
        }
    }

    return NULL;
}

/**
 * Получение массива ссылок на поля с указанной меткой.
 *
 * @param record Запись.
 * @param array Массив, заполняемый ссылками на поля.
 * @param tag Искомая метка поля.
 * @return Признак успешного выполнения операции.
 */
MAGNA_API am_bool MAGNA_CALL record_get_fields
    (
        const MarcRecord *record,
        Vector *array,
        am_uint32 tag
    )
{
    size_t i;
    const MarcField *field;

    assert (record != NULL);
    assert (array != NULL);

    for (i = 0; i < record->fields.len; ++i) {
        field = (const MarcField *) array_get (&record->fields, i);
        if (field->tag == tag) {
            if (!vector_push_back (array, (void *) field)) {
                return AM_FALSE;
            }
        }
    }

    return AM_TRUE;
}

/**
 * Выяснение количества полей с указанной меткой.
 *
 * @param record
 * @param tag
 * @return
 */
MAGNA_API size_t MAGNA_CALL record_count_fields
    (
        const MarcRecord *record,
        am_uint32 tag
    )
{
    size_t i, result = 0;
    const MarcField *field;

    assert (record != NULL);

    for (i = 0; i < record->fields.len; ++i) {
        field = (const MarcField *) array_get (&record->fields, i);
        if (field->tag == tag) {
            ++result;
        }
    }

    return result;
}

/**
 * Удаление всех повторений поля с указанной меткой.
 *
 * @param record Запись.
 * @param tag Метка поля, подлежащая удалению.
 */
MAGNA_API void MAGNA_CALL record_remove_field
    (
        MarcRecord *record,
        am_uint32 tag
    )
{
    assert (record != NULL);
}

/**
 * Установка значения поля.
 *
 * @param record
 * @param tag
 * @param value
 * @return
 */
MAGNA_API am_bool MAGNA_CALL record_set_field
    (
        MarcRecord *record,
        am_uint32 tag,
        Span value
    )
{
    assert (record != NULL);

    return AM_FALSE;
}

/**
 * Сброс состояния записи. Может потребоваться, например,
 * при переносе записи в другую базу данных.
 *
 * @param record Запись.
 * @return Запись.
 */
MAGNA_API MarcRecord* MAGNA_CALL record_reset
    (
        MarcRecord *record
    )
{
    assert (record != NULL);

    record->mfn = 0;
    record->status = 0;
    record->version = 0;
    buffer_clear (&record->database);

    return record;
}

/**
 * Разбор ответа сервера для ситуации, когда сервер присылает одну запись.
 *
 * @param record Запись, которая должна быть заполнена.
 * @param response Ответ сервера.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL record_parse_single
    (
        MarcRecord *record,
        Response *response
    )
{
    Span line;
    Span parts[2];
    size_t nparts;
    MarcField *field;

    assert (record != NULL);
    assert (response != NULL);

    record_clear (record);
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

    while (!response_eot (response)) {
        line = response_get_line (response);
        if (!span_is_empty (line)) {
            field = (MarcField*) array_emplace_back (&record->fields);
            if (field == NULL) {
                return AM_FALSE;
            }

            field_create (field);
            if (!field_decode (field, line)) {
                return AM_FALSE;
            }
        }
    }

    return AM_TRUE;
}

/**
 * Верификация записи.
 *
 * @param record Запись.
 * @return Результат выполнения верификации.
 */
MAGNA_API am_bool MAGNA_CALL record_verify
    (
        const MarcRecord *record
    )
{
    am_bool result;
    size_t i;
    const MarcField *field;

    assert (record != NULL);

    result = record->fields.len != 0;
    for (i = 0; i < record->fields.len; ++i) {
        field = (const MarcField *) array_get (&record->fields, i);
        if (!field_verify (field)) {
            result = AM_FALSE;
        }
    }

    return result;
}

MAGNA_API void MAGNA_CALL record_to_console
    (
        const MarcRecord *record
    )
{
    size_t index;
    const MarcField *field;

    assert (record != NULL);

    for (index = 0; index < record->fields.len; ++index) {
        field = (const MarcField *) array_get (&record->fields, index);
        field_to_console (field);
    }
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
