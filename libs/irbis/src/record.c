/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 5045)
#endif

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 * Инициализация записи.
 *
 * @param record Запись.
 * @return Запись.
 */
MAGNA_API MarcRecord* MAGNA_CALL record_init
    (
        MarcRecord *record
    )
{
    assert (record != NULL);

    memset (record, 0, sizeof (MarcRecord));

    return record;
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
        const char *value
    )
{
    MarcField *field;

    assert (record != NULL);

    field = calloc (1, sizeof (MarcField));
    field->tag = tag;
    buffer_from_text (&field->value, value);
    array_push_back (&record->fields, field);

    return field;
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
    am_size index;
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
        src = (const MarcField*) array_get (&source->fields, index);
        dst = (MarcField*) calloc (1, sizeof (MarcField));
        if (dst == NULL) {
            return NULL;
        }

        field_init (dst, src->tag);
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
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL record_decode_lines
    (
        MarcRecord *record,
        Array *lines
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
 * @return Признак успешности завершения операции.
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
        char code
    )
{
    am_size i, j;
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
                    subfield = (const SubField*) array_get (&field->subfields, j);
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
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL record_fma
    (
        const MarcRecord *record,
        Array *array,
        am_uint32 tag,
        char code
    )
{
    assert (record != NULL);
    assert (array != NULL);

    return AM_FALSE;
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
        am_size occurrence
    )
{
    const MarcField *field;
    am_size i;

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

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
