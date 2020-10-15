// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/fields.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
   \file visit.c

   Информация о посещении/книговыдаче.

    \struct Visit

    \var Visit::database
        \brief Имя БД каталога, подполе g.

    \var Visit::index
        \brief Шифр документа, подполе a.

    \var Visit::inventory
        \brief Инвентарный номер экземпляра, подполе b.

    \var Visit::barcode
        \brief Штрих-код экземпляра, подполе h.

    \var Visit::sigla
        \brief Место хранения экземпляра, подполе k.

    \var Visit::given
        \brief Дата выдачи, подполе d.

    \var Visit::department
        \brief Место выдачи, подполе v.

    \var Visit::expected
        \brief Дата предполагаемого возврата, подполе e.

    \var Visit::returned
        \brief Дата фактического возврата, подполе f.

    \var Visit::prolong
        \brief Дата продления, подполе l.

    \var Visit::lost
        \brief Признак утерянной книги, подполе u.

    \var Visit::description
        \brief Краткое библиографическое описание, подполе c.

    \var Visit::responsible
        \brief Ответственное лицо, подполе i.

    \var Visit::timeIn
        \brief Время начала визита в библиотеку, подполе 1.

    \var Visit::timeOut
        \brief Время окончания визита в библиотеку, подполе 2.

    \var Visit::count
        \brief Счетчик продлений, подполе 4.

    \var Visit::field
        \brief Поле, из которого извлечены данные о посещении.

 */

/*=========================================================*/

#define assign(__b, __f, __c) \
    buffer_assign_span ((__b), field_get_first_subfield_value ((__f), (__c)))

#define apply(__f, __c, __b) \
    field_set_subfield((__f), (__c), buffer_to_span (__b))

/*=========================================================*/

/**
 * Инициализация структуры.
 * Не выделяет память в куче.
 *
 * @param visit Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL visit_init
    (
        Visit *visit
    )
{
    assert (visit != NULL);

    mem_clear (visit, sizeof (*visit));
}

/**
 * Освобождение памяти, занятой структурой.
 *
 * @param visit Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL visit_destroy
    (
        Visit *visit
    )
{
    assert (visit != NULL);

    buffer_destroy (&visit->database);
    buffer_destroy (&visit->index);
    buffer_destroy (&visit->inventory);
    buffer_destroy (&visit->barcode);
    buffer_destroy (&visit->sigla);
    buffer_destroy (&visit->given);
    buffer_destroy (&visit->department);
    buffer_destroy (&visit->expected);
    buffer_destroy (&visit->returned);
    buffer_destroy (&visit->prolong);
    buffer_destroy (&visit->lost);
    buffer_destroy (&visit->description);
    buffer_destroy (&visit->responsible);
    buffer_destroy (&visit->timeIn);
    buffer_destroy (&visit->timeOut);
    buffer_destroy (&visit->count);
    mem_clear (visit, sizeof (*visit));
}

/**
 * Чистое посещение?
 *
 * @param visit Указатель на структуру посещения.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL visit_is_visit
    (
        const Visit *visit
    )
{
    assert (visit != NULL);

    return buffer_is_empty (&visit->index);
}

/**
 * Книга возвращена?
 *
 * @param visit Указатель на структуру книговыдачи.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL visit_is_returned
    (
        const Visit *visit
    )
{
    assert (visit != NULL);

    return buffer_compare_text (&visit->returned, CBTEXT (STOP_MARKER));
}

/**
 * Применение информации о посещении к указанному полю.
 *
 * @param visit Структура посещения.
 * @param field Поле, подлежащее заполнению элементами посещения.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL visit_apply
    (
        const Visit *visit,
        MarcField *field
    )
{
    assert (visit != NULL);
    assert (field != NULL);

    field_clear (field);
    return apply (field, 'g', &visit->database)
           && apply (field, 'a', &visit->index)
           && apply (field, 'b', &visit->inventory)
           && apply (field, 'h', &visit->barcode)
           && apply (field, 'k', &visit->sigla)
           && apply (field, 'd', &visit->given)
           && apply (field, 'v', &visit->department)
           && apply (field, 'e', &visit->expected)
           && apply (field, 'f', &visit->returned)
           && apply (field, 'l', &visit->prolong)
           && apply (field, 'u', &visit->lost)
           && apply (field, 'c', &visit->description)
           && apply (field, 'i', &visit->responsible)
           && apply (field, '1', &visit->timeIn)
           && apply (field, '2', &visit->timeOut)
           && apply (field, '4', &visit->count);
}

/**
 * Разбор указанного поля на элементы посещения/книговыдачи.
 *
 * @param visit Структура посещения, подлежащая заполнению.
 * @param field Поле для разбора.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL visit_parse_field
    (
        Visit *visit,
        const MarcField *field
    )
{
    assert (visit != NULL);
    assert (field != NULL);

    visit->field = (MarcField*) field;
    return assign (&visit->database,       field, 'g')
           && assign (&visit->index,       field, 'a')
           && assign (&visit->inventory,   field, 'b')
           && assign (&visit->barcode,     field, 'h')
           && assign (&visit->sigla,       field, 'k')
           && assign (&visit->given,       field, 'd')
           && assign (&visit->department,  field, 'v')
           && assign (&visit->expected,    field, 'e')
           && assign (&visit->returned,    field, 'f')
           && assign (&visit->prolong,     field, 'l')
           && assign (&visit->lost,        field, 'u')
           && assign (&visit->description, field, 'c')
           && assign (&visit->responsible, field, 'i')
           && assign (&visit->timeIn,      field, '1')
           && assign (&visit->timeOut,     field, '2')
           && assign (&visit->count,       field, '4');
}

/**
 * Поиск в записи сведений о посещениях/выдачах с последующим их разбором.
 *
 * @param record Запись для разбора.
 * @param visits Массив для размещения сведений о посещениях/выдачах.
 * @param tag Метка поля, как правило, 40.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL visit_parse_record
    (
        const MarcRecord *record,
        Array *visits,
        am_uint32 tag
    )
{
    size_t index;
    const MarcField *field;
    Visit *visit;

    assert (visits != NULL);
    assert (record != NULL);
    assert (tag != 0);

    for (index = 0; index < record->fields.len; ++index) {
        field = (const MarcField*) array_get (&record->fields, index);
        if (field->tag == tag) {
            visit = (Visit*) array_emplace_back (visits);
            if (visit == NULL) {
                return AM_FALSE;
            }

            visit_init (visit);
            if (!visit_parse_field (visit, field)) {
                --visits->len;
                return AM_FALSE;
            }
        }
    }

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
