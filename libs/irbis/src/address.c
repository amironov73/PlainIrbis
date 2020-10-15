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
   \file address.c

   Адрес читателя: поле 13 в базе RDR.

    \struct Address

    \var Address::TAG
        \brief Метка поля для адреса.

    \var Address::postcode
        \brief Почтовый индекс, подполе a.

    \var Address::country
        \brief Страна/республика, подполе b.

    \var Address::city
        \brief Город, подполе c.

    \var Address::street
        \brief Улица, подполе d.

    \var Address::building
        \brief Номер дома, подполе e.

    \var Address::entrance
        \brief Номер подъезда, подполе g.

    \var Address::apartment
        \brief Номер квартиры, подполе h.

    \var Address::additional
        \brief Дополнительные данные, подполе f.

    \var Address::field
        \brief Поле, из которого извлечены данные об адресе.

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
 * @param address Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL address_init
    (
        Address *address
    )
{
    assert (address != NULL);

    mem_clear (address, sizeof (*address));
}

/**
 * Освобождение памяти, занятой структурой.
 *
 * @param address Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL address_destroy
    (
        Address *address
    )
{
    assert (address != NULL);

    buffer_destroy (&address->postcode);
    buffer_destroy (&address->country);
    buffer_destroy (&address->city);
    buffer_destroy (&address->street);
    buffer_destroy (&address->building);
    buffer_destroy (&address->entrance);
    buffer_destroy (&address->apartment);
    buffer_destroy (&address->additional);
    mem_clear (address, sizeof (*address));
}

/**
 * Применение адреса к указанному полю.
 *
 * @param address Структура адреса.
 * @param field Поле, подлежащее заполнению элементами адреса.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL address_apply
    (
        const Address *address,
        MarcField *field
    )
{
    assert (address != NULL);
    assert (field != NULL);

    field_clear (field);
    return apply (field, 'a', &address->postcode)
        && apply (field, 'b', &address->country)
        && apply (field, 'c', &address->city)
        && apply (field, 'd', &address->street)
        && apply (field, 'e', &address->building)
        && apply (field, 'g', &address->entrance)
        && apply (field, 'h', &address->apartment)
        && apply (field, 'f', &address->additional);
}

/**
 * Разбор указанного поля на элементы адреса.
 *
 * @param address Структура адреса, подлежащая заполнению.
 * @param field Поле для разбора.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL address_parse_field
    (
        Address *address,
        const MarcField *field
    )
{
    assert (address != NULL);
    assert (field != NULL);

    address->field = (MarcField*) field;
    return assign (&address->postcode,   field, 'a')
        && assign (&address->country,    field, 'b')
        && assign (&address->city,       field, 'c')
        && assign (&address->street,     field, 'd')
        && assign (&address->building,   field, 'e')
        && assign (&address->entrance,   field, 'g')
        && assign (&address->apartment,  field, 'h')
        && assign (&address->additional, field, 'f');
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
