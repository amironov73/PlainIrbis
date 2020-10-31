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
#include <stdarg.h>

/*=========================================================*/

/**
   \file collecti.c

   Коллективный (в т. ч. временный) автор.
   Раскладка полей 710, 711, 962, 972.

   \struct Collective
        \brief Сведения о коллективном авторе.
        \details Структура владеет собственной памятью,
        за исключением поля `field`.
        Для освобождения ресурсов необходимо вызвать
        `collective_destroy`.

   \var Collective::title
        \brief Наименование коллектива, подполе A.

   \var Collective::country
        \brief Страна, подполе S.

   \var Collective::abbreviation
        \brief Аббревиатура, подполе R.

   \var Collective::number
        \brief Номер, подполе N.

   \var Collective::date
        \brief Дата, подполе F.

   \var Collective::city
        \brief Город, подполе C.

   \var Collective::department
        \brief Подразделение, подполе B.

   \var Collective::characteristic
        \brief Характерное название подразделения, подполе X.

   \var Collective::gost
        \brief Сокращение названия по ГОСТ, подполе 7.

   \var Collective::field
        \brief Поле, из которого извлечена информация.
        \warning Структура не владеет памятью,
        на которую ссылается данный указатель.

 */

/*=========================================================*/

#define assign(__b, __f, __c) \
    buffer_assign_span ((__b), field_get_first_subfield_value ((__f), (__c)))

#define apply(__f, __c, __b) \
    field_set_subfield((__f), (__c), buffer_to_span (__b))

/*=========================================================*/

/**
 * Простая инициализация структуры.
 * Не выделяет памяти в куче.
 *
 * @param collective Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL collective_init
    (
        Collective *collective
    )
{
    assert (collective != NULL);

    mem_clear (collective, sizeof (*collective));
}

/**
 * Освобождение памяти, занятой структурой.
 *
 * @param collective Указатель на освобождаемую структуру.
 */
MAGNA_API void MAGNA_CALL collective_destroy
    (
        Collective *collective
    )
{
    assert (collective != NULL);

    buffer_destroy (&collective->title);
    buffer_destroy (&collective->country);
    buffer_destroy (&collective->abbreviation);
    buffer_destroy (&collective->number);
    buffer_destroy (&collective->date);
    buffer_destroy (&collective->city);
    buffer_destroy (&collective->department);
    buffer_destroy (&collective->characteristic);
    buffer_destroy (&collective->gost);
    mem_clear (collective, sizeof (*collective));
}

/**
 * Применение сведений о коллективе к указанному полю.
 *
 * @param collective Сведения о коллективе.
 * @param field Поле, подлежащее заполнению сведениями о коллективе.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL collective_apply
    (
        const Collective *collective,
        MarcField *field
    )
{
    assert (collective != NULL);
    assert (field != NULL);

    field_clear (field);
    return apply (field, 'a', &collective->title)
        && apply (field, 's', &collective->country)
        && apply (field, 'r', &collective->abbreviation)
        && apply (field, 'n', &collective->number)
        && apply (field, 'f', &collective->date)
        && apply (field, 'c', &collective->city)
        && apply (field, 'b', &collective->department)
        && apply (field, 'x', &collective->characteristic)
        && apply (field, '7', &collective->gost);
}

/**
 * Разбор указанного поля на сведения о коллективном авторе.
 *
 * @param collective Сведения о коллективе, подлежащие заполнению.
 * @param field Поле для разбора.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL collective_parse_field
    (
        Collective *collective,
        const MarcField *field
    )
{
    assert (collective != NULL);
    assert (field != NULL);

    collective->field = (MarcField*) field;
    return assign (&collective->title,          field, 'a')
        && assign (&collective->country,        field, 's')
        && assign (&collective->abbreviation,   field, 'r')
        && assign (&collective->number,         field, 'n')
        && assign (&collective->date,           field, 'f')
        && assign (&collective->city,           field, 'c')
        && assign (&collective->department,     field, 'b')
        && assign (&collective->characteristic, field, 'x')
        && assign (&collective->gost,           field, '7');
}

/**
 * Разбор записи на сведения о коллективных авторах.
 *
 * @param collectives Массив, подлежащий заполнению.
 * @param record Запись для разбора.
 * @param ... Метки полей, заканчивающиеся 0.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool collective_parse_record
    (
        const MarcRecord *record,
        Array *collectives,
        ...
    )
{
    Collective *collective;
    va_list args;
    size_t index;
    const MarcField *field;
    am_uint32 tag;

    assert (collectives != NULL);
    assert (record != NULL);

    va_start (args, collectives);
    while (AM_TRUE) {
        tag = va_arg (args, am_uint32);
        if (tag == 0) {
            break;
        }

        for (index = 0; index < record->fields.len; ++index) {
            field = (const MarcField*) array_get (&record->fields, index);
            if (field->tag == tag) {
                collective = array_emplace_back (collectives);
                if (collective == NULL) {
                    return AM_FALSE;
                }

                collective_init (collective);
                if (!collective_parse_field (collective, field)) {
                    return AM_FALSE;
                }
            }
        }
    }

    va_end (args);

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
