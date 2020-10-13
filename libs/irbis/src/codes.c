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
 * \file codes.c
 *
 * Кодированная информация о книге/журнале/статье.
 * Поле 900: Коды: Тип, Вид, Характер документа, Код целевого назначения.
 */

/*=========================================================*/

#define assign(__b, __f, __c) \
    buffer_assign_span ((__b), field_get_first_subfield_value ((__f), (__c)))

#define apply(__f, __c, __b) \
    field_set_subfield((__f), (__c), buffer_to_span (__b))


/**
 * Инициализация структуры.
 *
 * @param codes Указатель на неиницализированную структуру.
 */
MAGNA_API void MAGNA_CALL codes_init
    (
        Codes *codes
    )
{
    assert (codes != NULL);

    mem_clear (codes, sizeof (*codes));
}

/**
 * Освобождение памяти, занятой структурой.
 *
 * @param address Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL codes_destroy
    (
        Codes *codes
    )
{
    assert (codes != NULL);

    buffer_destroy (&codes->type);
    buffer_destroy (&codes->kind);
    buffer_destroy (&codes->character [0]);
    buffer_destroy (&codes->character [1]);
    buffer_destroy (&codes->character [2]);
    buffer_destroy (&codes->character [3]);
    buffer_destroy (&codes->character [4]);
    buffer_destroy (&codes->character [5]);
    buffer_destroy (&codes->purpose [0]);
    buffer_destroy (&codes->purpose [1]);
    buffer_destroy (&codes->purpose [2]);
    buffer_destroy (&codes->restrictions);
    mem_clear (codes, sizeof (*codes));
}

/**
 * Применение ФИО автора к указанному полю.
 *
 * @param codes Структура адреса.
 * @param field Поле, подлежащее заполнению элементами ФИО автора.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL codes_apply
    (
        const Codes *codes,
        MarcField *field
    )
{
    assert (codes != NULL);
    assert (field != NULL);

    return apply (field, 't', &codes->type)
        && apply (field, 'b', &codes->kind)
        && apply (field, 'c', &codes->character [0])
        && apply (field, '2', &codes->character [1])
        && apply (field, '3', &codes->character [2])
        && apply (field, '4', &codes->character [3])
        && apply (field, '5', &codes->character [4])
        && apply (field, '6', &codes->character [5])
        && apply (field, 'x', &codes->purpose [0])
        && apply (field, 'y', &codes->purpose [1])
        && apply (field, '9', &codes->purpose [2])
        && apply (field, 'z', &codes->restrictions);
}

/**
 * Разбор указанного поля а элементы адреса.
 *
 * @param author Структура адреса, подлежащая заполнению.
 * @param field Поле для разбора.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL codes_parse_field
    (
        Codes *codes,
        const MarcField *field
    )
{
    assert (codes != NULL);
    assert (field != NULL);

    codes->field = (MarcField*) field;
    return assign (&codes->type,          field, 't')
        && assign (&codes->kind,          field, 'b')
        && assign (&codes->character [0], field, 'c')
        && assign (&codes->character [1], field, '2')
        && assign (&codes->character [2], field, '3')
        && assign (&codes->character [3], field, '4')
        && assign (&codes->character [4], field, '5')
        && assign (&codes->character [5], field, '6')
        && assign (&codes->purpose [0],   field, 'x')
        && assign (&codes->purpose [1],   field, 'y')
        && assign (&codes->purpose [2],   field, '9')
        && assign (&codes->restrictions,  field, 'z');
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
