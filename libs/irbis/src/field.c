// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/irbis.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/**
 * \file field.c
 *
 * Поле записи. Состоит из метки, значения до первого разделителя
 * и произвольного количества подполей.
 *
 * Поле записи характеризуется числовой меткой в диапазоне
 * от 1 до 2147483647 (на практике встречаются коды от 1 до 9999)
 * и содержит значение до первого разделителя (опционально)
 * и произвольное количество подполей (см. класс `SubField`).
 *
 * Стандартом MARC у полей предусмотрены также два односимвольных
 * индикатора, но ИРБИС вслед за ISIS их не поддерживает.
 *
 * Кроме того, стандарт MARC предусматривает т. наз. "фиксированные"
 * поля с метками от 1 до 9 включительно, которые не должны содержать
 * ни индикаторов, ни подполей, но имеют строго фиксированную структуру.
 * ИРБИС такие поля обрабатывает особым образом только в ситуации
 * импорта/экспорта в формат ISO2709, в остальном же он их трактует
 * точно так же, как и прочие поля (которые стандарт называет
 * полями переменной длины).
 *
 * Стандартом MARC предусмотрены метки в диапазоне от 1 до 999,
 * все прочие являются самодеятельностью ИРБИС. Поля с нестандартными
 * метками не могут быть выгружены в формат ISO2709.
 *
 * Хотя технически поле может содержать одновременно и значение
 * до первого разделителя, и подполя, но стандартом такая ситуация
 * не предусмотрена, на практике она означает сбой. В стандарте
 * MARC поле содержит либо значение либо подполя.
 *
 * Начиная с версии 2018, ИРБИС64 резервирует метку 2147483647
 * для поля GUID - уникального идентификатора записи.
 *
 * Порядок подполей в поле важен, т. к. на этот порядок завязана
 * обработка т. наз. "вложенных полей".
 *
 * Стандартом MARC предусмотрено, что внутри поля могут повторяться
 * подполя с одинаковым кодом, однако, ИРБИС вслед за ISIS очень
 * ограниченно поддерживает эту ситуацию (см. форматный выход `&umarci`).
 */

#include <assert.h>

/*=========================================================*/

#define DO_EXPAND(VAL)  VAL ## 1
#define EXPAND(VAL)     DO_EXPAND(VAL)

/*=========================================================*/

/**
 * Добавление подполя с указанными кодом и значением.
 *
 * @param field Указатель на инициализированную структуру поля.
 * @param code Код подполя.
 * @param value Значение подполя (может быть пустым).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL field_add
    (
        MarcField *field,
        am_byte code,
        Span value
    )
{
    SubField *subfield;

    assert (field != NULL);
    assert (subfield_code_is_valid (code));

    subfield = (SubField*) array_emplace_back (&field->subfields);
    if (subfield == NULL) {
        return AM_FALSE;
    }

    subfield->code = subfield_normalize_code (code);

    return subfield_assign (subfield, value);
}

/**
 * Очистка поля: удаление значения до первого разделителя
 * и всех подполей.
 *
 * @param field Поле, подлежащее очистке.
 * @return Указатель на поле.
 */
MAGNA_API MarcField* MAGNA_CALL field_clear
    (
        MarcField *field
    )
{
    size_t index;
    SubField *subfield;

    assert (field != NULL);

    for (index = 0; index < field->subfields.len; ++index) {
        subfield = field_get_subfield_by_index (field, index);
        subfield_destroy (subfield);
    }

    array_clear (&field->subfields);
    buffer_clear (&field->value);

    return field;
}

/**
 * Создание глубокой копии поля со всеми подполями.
 *
 * @param target Указатель не неинициализированную структуру поля.
 * @param source Указатель на поле, подлежащее клонированию.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL field_clone
    (
        MarcField *target,
        const MarcField *source
    )
{
    size_t index;
    const SubField *sourceSubfield;

    assert (target != NULL);
    assert (source != NULL);

    field_create (target);
    target->tag = source->tag;
    if (!buffer_clone (&target->value, &source->value)) {
        return AM_FALSE;
    }

    for (index = 0; index < source->subfields.len; ++index) {
        sourceSubfield = (const SubField*) array_get (&source->subfields, index);
        if (!field_add (target, sourceSubfield->code, buffer_to_span (&sourceSubfield->value))) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

#if EXPAND(MAGNA_CALL) == 1

#define subfield_liberator subfield_destroy

#else

/* Тривиальная обертка для случая не пустого `MAGNA_CALL` */
static void subfield_liberator
    (
        SubField *subfield
    )
{
    subfield_destroy (subfield);
}

#endif

/**
 * Простая инициализация поля.
 *
 * @param field Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL field_create
    (
        MarcField *field
    )
{
    assert (field != NULL);

    field->tag = 0;
    buffer_null (&field->value);
    array_init (&field->subfields, sizeof (SubField));
    field->subfields.liberator = (Liberator) subfield_liberator;
}

/**
 * Декодирование текстового представления поля
 * (метка плюс тело поля).
 *
 * @param field Указатель на инициализированную структуру поля.
 * @param span Текст для декодирования (не должен быть пустым).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL field_decode
    (
        MarcField *field,
        Span span
    )
{
    Span parts[2];
    size_t nparts;

    assert (field != NULL);

    if (span_is_empty (span)) {
        return AM_FALSE;
    }

    if (!span_contains (span, '#')) {
        return AM_FALSE;
    }

    nparts = span_split_n_by_char (span, parts, 2, '#');

    field->tag = span_to_uint32 (parts[0]);

    if (nparts == 1) {
        return AM_TRUE;
    }

    return field_decode_body (field, parts[1]);
}

/**
 * Декодирование текстового представления тела поля.
 *
 * @param field Указатель на инициализированную структуру поля.
 * @param span Текст для декодирования (может быть пустым).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL field_decode_body
    (
        MarcField *field,
        Span span
    )
{
    TextNavigator nav;
    Span text;
    SubField *subfield;

    assert (field != NULL);

    if (span_is_empty (span)) {
        return AM_TRUE;
    }

    nav_from_span (&nav, span);
    text = nav_read_to (&nav, '^');
    if (!span_is_empty (text)) {
        if (!buffer_assign_span (&field->value, text)) {
            return AM_FALSE;
        }
    }

    while (!nav_eot (&nav)) {
        text = nav_read_to (&nav, '^');
        if (!span_is_empty (text)) {
            subfield = (SubField*) array_emplace_back (&field->subfields);
            if (subfield == NULL) {
                return AM_FALSE;
            }

            if (!subfield_decode (subfield, text)) {
                return AM_FALSE;
            }
        }
    }

    return AM_TRUE;
}

/**
 * Освобождение ресурсов, занятых полем.
 *
 * @param field Поле, подлежащее освобождению.
 */
MAGNA_API void MAGNA_CALL field_destroy
    (
        MarcField *field
    )
{
    size_t index;

    assert (field != NULL);

    buffer_destroy (&field->value);
    array_destroy (&field->subfields);
}

/**
 * Проверка, является ли поле пустым.
 *
 * @param field Поле, подлежащее проверке.
 * @return Результат проверки.
 * @remarks Поле считается пустым, если у него одновременно
 * отсутствуют как значение до первого разделителя, так и подполя.
 */
MAGNA_API am_bool MAGNA_CALL field_is_empty
    (
        const MarcField *field
    )
{
    assert (field != NULL);

    return buffer_is_empty (&field->value)
        && (field->subfields.len == 0);
}

/**
 * Получение подполя по его индексу.
 *
 * @param field Поле.
 * @param index Индекс подполя (нумерация с 0).
 * @return Указатель на подполе.
 */
MAGNA_API SubField* MAGNA_CALL field_get_subfield_by_index
    (
        const MarcField *field,
        size_t index
    )
{
    assert (field != NULL);
    assert (index < field->subfields.len);

    return (SubField*) array_get (&field->subfields, index);
}

MAGNA_API Vector* MAGNA_CALL field_get_embedded_fields
    (
        const MarcField *field,
        Vector *array
    )
{
    assert (field != NULL);
    assert (array != NULL);

    return array;
}

/**
 * Получение указателя на первое подполе с указанным кодом.
 *
 * @param field Поле.
 * @param code Искомый код подполя. При сравнении регистр символов не учитывается.
 * @return Указатель на найденное подполе либо `NULL`.
 */
MAGNA_API SubField* MAGNA_CALL field_get_first_subfield
    (
        const MarcField *field,
        am_byte code
    )
{
    size_t index;
    SubField *subfield;

    assert (field != NULL);

    for (index = 0; index < field->subfields.len; ++index) {
        subfield = field_get_subfield_by_index (field, index);
        if (same_char (code, subfield->code)) {
            return subfield;
        }
    }

    return NULL;
}

/**
 * Получение значения первого подполя с указанным кодом.
 *
 * @param field Поле.
 * @param code Искомый код подполя. При сравнении регистр символов не учитывается.
 * @return Спан со значением найденного поля либо пустой спан.
 */
MAGNA_API Span MAGNA_CALL field_get_first_subfield_value
    (
        const MarcField *field,
        am_byte code
    )
{
    const SubField *found;

    assert (field != NULL);

    found = field_get_first_subfield (field, code);

    return (found == NULL)
        ? span_null()
        : buffer_to_span (&found->value);
}

/**
 * Вставка подполя по указанному индексу.
 *
 * @param field Поле.
 * @param index Индекс.
 * @param code Код подполя.
 * @param value Значение подполя.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL field_insert_at
    (
        MarcField *field,
        size_t index,
        am_byte code,
        Span value
    )
{
    assert (field != NULL);
    assert (index <= field->subfields.len);

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Удаление подполя по указанному индексу.
 *
 * @param field Поле.
 * @param index Индекс удаляемого подполя.
 * @return Указатель на поле.
 */
MAGNA_API MarcField* MAGNA_CALL field_remove_at
    (
        MarcField *field,
        size_t index
    )
{
    SubField *subfield;

    assert (field != NULL);
    assert (index < field->subfields.len);

    subfield = field_get_subfield_by_index (field, index);
    subfield_destroy (subfield);
    array_remove_index (&field->subfields, index);

    return field;
}

/**
 * Удаление всех подполей с указанным кодом.
 *
 * @param field Поле.
 * @param code Искомый код подполя. При сравнении регистр символов не учитывается.
 * @return Указатель на поле.
 */
MAGNA_API MarcField* MAGNA_CALL field_remove_subfield
    (
        MarcField *field,
        am_byte code
    )
{
    am_bool found;
    size_t index;
    SubField *subfield;

    assert (field != NULL);

    do {
        found = AM_FALSE;
        for (index = 0; index < field->subfields.len; ++index) {
            subfield = field_get_subfield_by_index (field, index);
            if (same_char (code, subfield->code)) {
                subfield_destroy (subfield);
                array_remove_index (&field->subfields, index);
                found = AM_TRUE;
                break;
            }
        }
    }
    while (found);

    return field;
}

/**
 * Установка значения подполя с указанным кодом.
 *
 * @param field Поле.
 * @param code Искомый код подполя. При сравнении регистр символов не учитывается.
 * @param value Новое значение подполя (может быть пустым, тогда происходит удаление подполя).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL field_set_subfield
    (
        MarcField *field,
        am_byte code,
        Span value
    )
{
    SubField *subfield;

    assert (field != NULL);

    if (span_is_empty (value)) {
        field_remove_subfield (field, code);

        return AM_TRUE;
    }

    subfield = field_get_first_subfield (field, code);
    if (subfield == NULL) {
        return field_add (field, code, value);
    }

    return subfield_assign (subfield, value);
}

/**
 * Установка значения поля до первого разделителя.
 *
 * @param field Указатель на инициализированную структуру поля.
 * @param value Устанавливаемое значение (может быть пустым).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL field_set_value
    (
        MarcField *field,
        Span value
    )
{
    assert (field != NULL);

    return buffer_assign_span (&field->value, value);
}

MAGNA_API am_bool MAGNA_CALL field_to_string
    (
        const MarcField *field,
        Buffer *output
    )
{
    size_t index;
    const SubField *subfield;

    assert (field != NULL);
    assert (output != NULL);

    if (!buffer_put_uint32 (output, field->tag)
        || !buffer_putc (output, '#')
        || !buffer_concat (output, &field->value)) {
        return AM_FALSE;
    }

    for (index = 0; index < field->subfields.len; ++index) {
        subfield = field_get_subfield_by_index (field, index);
        if (!subfield_to_string (subfield, output)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/**
 * Верификация поля. Поле считается построенным верно, если:
 * 1) Метка не равна 0, 2) присутствует значение до первого разделителя
 * и/или подполя, 3) подполя (если есть) верифицированы.
 *
 * @param field Поле для проверки.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL field_verify
    (
        const MarcField *field
    )
{
    am_bool result;
    size_t index;
    const SubField *subfield;

    assert (field != NULL);

    result = field->tag != 0;
    if (result) {
        if (field->subfields.len == 0) {
            result = !buffer_is_empty (&field->value);
        }
        else {
            for (index = 0; index < field->subfields.len; ++index) {
                subfield = field_get_subfield_by_index (field, index);
                if (!subfield_verify (subfield)) {
                    result = AM_FALSE;
                    break;
                }
            }
        }
    }

    return result;
}
