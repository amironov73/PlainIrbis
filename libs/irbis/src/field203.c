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
 * \file field203.c
 *
 * В связи с ГОСТ 7.0.100-2018 введено 203 поле.
 * Это поле содержит подполя: вид содержания,
 * средства доступа, характеристика содержания.
 */

/*=========================================================*/

#define assign(__b, __f, __c) \
    buffer_assign_span ((__b), field_get_first_subfield_value ((__f), (__c)))

#define apply(__f, __c, __b) \
    field_set_subfield((__f), (__c), buffer_to_span (__b))

/**
 * Инициализация структуры.
 *
 * @param field Указатель на ненициализированную структуру.
 */
MAGNA_API void MAGNA_CALL field203_init
    (
        Field203 *field
    )
{
    assert (field != NULL);

    mem_clear (field, sizeof (*field));
}

/**
 * Освобождение памяти, занимаемой структурой.
 *
 * @param field Указатель на освобождаемую структуру.
 */
MAGNA_API void MAGNA_CALL field203_destroy
    (
        Field203 *field
    )
{
    size_t index;

    assert (field != NULL);

    for (index = 0; index < 9; ++index) {
        buffer_destroy (&field->contentType [index]);
        buffer_destroy (&field->access [index]);
        buffer_destroy (&field->contentDescription [index]);
    }

    buffer_destroy (&field->contentDescription [9]);
    buffer_destroy (&field->contentDescription [10]);

    mem_clear (field, sizeof (*field));
}

MAGNA_API am_bool MAGNA_CALL field203_parse_field
    (
        Field203 *fld203,
        MarcField *field
    )
{
    assert (fld203 != NULL);
    assert (field != NULL);

    fld203->field = field;

    return assign (&fld203->contentType[0], field, 'a')
        && assign (&fld203->contentType[1], field, 'b')
        && assign (&fld203->contentType[2], field, 'd')
        && assign (&fld203->contentType[3], field, 'e')
        && assign (&fld203->contentType[4], field, 'f')
        && assign (&fld203->contentType[5], field, 'g')
        && assign (&fld203->contentType[6], field, 'i')
        && assign (&fld203->contentType[7], field, 'k')
        && assign (&fld203->contentType[8], field, 'l')

        && assign (&fld203->access[0], field, 'c')
        && assign (&fld203->access[1], field, '1')
        && assign (&fld203->access[2], field, '2')
        && assign (&fld203->access[3], field, '3')
        && assign (&fld203->access[4], field, '4')
        && assign (&fld203->access[5], field, '5')
        && assign (&fld203->access[6], field, '6')
        && assign (&fld203->access[7], field, '7')
        && assign (&fld203->access[8], field, '8')

        && assign (&fld203->contentDescription[0],  field, 'o')
        && assign (&fld203->contentDescription[1],  field, 'p')
        && assign (&fld203->contentDescription[2],  field, 'u')
        && assign (&fld203->contentDescription[3],  field, 'y')
        && assign (&fld203->contentDescription[4],  field, 't')
        && assign (&fld203->contentDescription[5],  field, 'r')
        && assign (&fld203->contentDescription[6],  field, 'w')
        && assign (&fld203->contentDescription[7],  field, 'q')
        && assign (&fld203->contentDescription[8],  field, 'x')
        && assign (&fld203->contentDescription[9],  field, 'v')
        && assign (&fld203->contentDescription[10], field, 'z');
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
