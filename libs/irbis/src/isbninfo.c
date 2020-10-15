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
   \file isbninfo.c

   ISBN и цена, поле 10.

   \struct IsbnInfo
        \brief ISBN и цена, поле 10.
        \details Структура владеет собственной памятью (кроме поля `field`),
        для освобождения ресурсов используйте `isbn_destroy`.

   \var IsbnInfo::isbn
        \brief ISBN, подполе A.

   \var IsbnInfo::refinement
        \brief Уточнение для ISBN, подполе B.

   \var IsbnInfo::erroneous
        \brief Ошибочный ISBN, подполе Z.

   \var IsbnInfo::price
        \brief Цена общая для всех экземпляров (только цифры), подпое D.

   \var IsbnInfo::currency
        \brief Обозначение валюты, подполе C.

   \var IsbnInfo::field
        \brief Поле, из которого извлечена информация.

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
MAGNA_API void MAGNA_CALL isbn_init
    (
        IsbnInfo *isbn
    )
{
    assert (isbn != NULL);

    mem_clear (isbn, sizeof (*isbn));
}

/**
 * Освобождение памяти, занятой структурой.
 *
 * @param collective Указатель на освобождаемую структуру.
 */
MAGNA_API void MAGNA_CALL isbn_destroy
    (
        IsbnInfo *isbn
    )
{
    assert (isbn != NULL);

    buffer_destroy (&isbn->isbn);
    buffer_destroy (&isbn->refinement);
    buffer_destroy (&isbn->erroneous);
    buffer_destroy (&isbn->price);
    buffer_destroy (&isbn->currency);
    mem_clear (isbn, sizeof (*isbn));
}

/**
 * Применение сведений об ISBN к указанному полю.
 *
 * @param isbn Сведения об ISBN.
 * @param field Поле, подлежащее заполнению.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL isbn_apply
    (
        const IsbnInfo *isbn,
        MarcField *field
    )
{
    assert (isbn != NULL);
    assert (field != NULL);

    field_clear (field);
    return apply (field, 'a', &isbn->isbn)
           && apply (field, 'b', &isbn->refinement)
           && apply (field, 'z', &isbn->erroneous)
           && apply (field, 'd', &isbn->price)
           && apply (field, 'c', &isbn->currency);
}

/**
 * Разбор указанного поля на сведения об ISBN и цене.
 *
 * @param collective Сведения об ISBN и цене, подлежащие заполнению.
 * @param field Поле для разбора.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL isbn_parse_field
    (
        IsbnInfo *isbn,
        const MarcField *field
    )
{
    assert (isbn != NULL);
    assert (field != NULL);

    isbn->field = (MarcField*) field;
    return assign (&isbn->isbn,           field, 'a')
           && assign (&isbn->refinement,  field, 'b')
           && assign (&isbn->erroneous,   field, 'z')
           && assign (&isbn->price,       field, 'd')
           && assign (&isbn->currency,    field, 'c');
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
