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

/*=========================================================*/

/**
 * \file collecti.c
 *
 * Коллективный (в т. ч. временный) автор.
 * Раскладка полей 710, 711, 962, 972.
 */

/*=========================================================*/

#define assign(__b, __f, __c) \
    buffer_assign_span ((__b), field_get_first_subfield_value ((__f), (__c)))

#define apply(__f, __c, __b) \
    field_set_subfield((__f), (__c), buffer_to_span (__b))

typedef struct
{
    Buffer title;
    Buffer country;
    Buffer abbreviation;
    Buffer number;
    Buffer date;
    Buffer city;
    Buffer department;
    Buffer characteristic;
    Buffer gost;
    MarcField *field;

} Collective;

/**
 * Простая инициализация структуры.
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
 * Применение сведений о коллеквтиве к указанному полю.
 *
 * @param collective Сведения о коллективе.
 * @param field Поле, подлежащее заполнению сведениями о коллективе.
 * @return Признак успешности завершения операции.
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
 * @return Признак успешности завершения операции.
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
 * @param array Массив, подлежащий заполнению.
 * @param record Запись для разбора.
 * @param ... Метки полей, заканчивающиеся 0.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool collective_parse_record
    (
        Array *array,
        MarcRecord *record,
        ...
    )
{
    Collective *collective;
    va_list args;
    size_t index;
    const MarcField *field;
    am_uint32 tag;

    assert (array != NULL);
    assert (record != NULL);

    va_start (args, record);
    while (AM_TRUE) {
        tag = va_arg (args, am_uint32);
        if (tag == 0) {
            break;
        }

        for (index = 0; index < record->fields.len; ++index) {
            field = (const MarcField*) array_get (&record->fields, index);
            if (field->tag == tag) {
                collective = array_emplace_back (array);
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
