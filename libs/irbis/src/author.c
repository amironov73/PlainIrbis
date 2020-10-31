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
   \file author.c

   Информация об индивидуальном авторе/редакторе книги/статьи.
   Поля 70x.

    \struct Author
        \brief Информация об индивидуальном авторе/редакторе книги/статьи.
        Поля 70x.

    \var Author::familyName
        \brief Фамилия, подполе `a`.

    \var Author::initials
        \brief Инициалы (сокращение), подполе `b`.

    \var Author::fullName
        \brief Расширение инициалов (имя и отчество), подполе `g`.

    \var Author::canBeInverted
        \brief Инвертирование имени недопустимо? Подполе `9`.

    \var Author::postfix
        \brief Неотъемлемая части (отец, сын, младший, старший и т. п.), подполе `1`.

    \var Author::appendix
        \brief Дополнения к имени кроме дат (род деятельности, звание, титул и т. д.), подполе `c`.

    \var Author::number
        \brief Династический номер (римские цифры), подполе `d`.

    \var Author::dates
        \brief Даты жизни, подполе `f`.

    \var Author::variant
        \brief Разночтение фамилии, подполе `r`.

    \var Author::workplace
        \brief Место работы автора, подполе `p`.

    \var Author::field
        \brief Поле, из которого была извлечена информация.

 */

/*=========================================================*/

#define assign(__b, __f, __c) \
    buffer_assign_span ((__b), field_get_first_subfield_value ((__f), (__c)))

#define apply(__f, __c, __b) \
    field_set_subfield((__f), (__c), buffer_to_span (__b))

/*=========================================================*/

/**
 * Простая инициализация структуры.
 * Не выделяет память в куче.
 *
 * @param author Указатель на неиницализированную структуру.
 */
MAGNA_API void MAGNA_CALL author_init
    (
        Author *author
    )
{
    assert (author != NULL);

    mem_clear (author, sizeof (*author));
}

/**
 * Освобождение памяти, занятой структурой.
 *
 * @param address Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL author_destroy
    (
        Author *author
    )
{
    assert (author != NULL);

    buffer_destroy (&author->familyName);
    buffer_destroy (&author->initials);
    buffer_destroy (&author->fullName);
    buffer_destroy (&author->canBeInverted);
    buffer_destroy (&author->postfix);
    buffer_destroy (&author->appendix);
    buffer_destroy (&author->number);
    buffer_destroy (&author->dates);
    buffer_destroy (&author->variant);
    buffer_destroy (&author->workplace);
    mem_clear (author, sizeof (*author));
}

/**
 * Простая инициализация массива авторов.
 * Не выделяет память в куче.
 *
 * @param authors Указатель на неинициализированный массив.
 */
MAGNA_API void MAGNA_CALL author_array_init
    (
        Array *authors
    )
{
    assert (authors != NULL);

    array_init (authors, sizeof (Author));
}

/**
 * Освобождение ресурсов, занятых массивом авторов.
 *
 * @param authors Указатель на массив, подлежащий освобождению.
 */
MAGNA_API void MAGNA_CALL author_array_destroy
    (
        Array *authors
    )
{
    assert (authors != NULL);

    array_destroy (authors, (Liberator) author_destroy);
}

/**
 * Применение ФИО автора к указанному полю.
 *
 * @param author Структура автора.
 * @param field Поле, подлежащее заполнению элементами ФИО автора.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL author_apply
    (
        const Author *author,
        MarcField *field
    )
{
    assert (author != NULL);
    assert (field != NULL);

    field_clear (field);
    return apply (field, 'a', &author->familyName)
        && apply (field, 'b', &author->initials)
        && apply (field, 'g', &author->fullName)
        && apply (field, '9', &author->canBeInverted)
        && apply (field, '1', &author->postfix)
        && apply (field, 'c', &author->appendix)
        && apply (field, 'd', &author->number)
        && apply (field, 'f', &author->dates)
        && apply (field, 'r', &author->variant)
        && apply (field, 'p', &author->workplace);
}

/**
 * Разбор указанного поля на элементы ФИО автора.
 *
 * @param author Структура автора, подлежащая заполнению.
 * @param field Поле для разбора.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL author_parse_field
    (
        Author *author,
        const MarcField *field
    )
{
    assert (author != NULL);
    assert (field != NULL);

    author->field = (MarcField*) field;
    return assign (&author->familyName,    field, 'a')
        && assign (&author->initials,      field, 'b')
        && assign (&author->fullName,      field, 'g')
        && assign (&author->canBeInverted, field, '9')
        && assign (&author->postfix,       field, '1')
        && assign (&author->appendix,      field, 'c')
        && assign (&author->number,        field, 'd')
        && assign (&author->dates,         field, 'f')
        && assign (&author->variant,       field, 'r')
        && assign (&author->workplace,     field, 'p');
}

/**
 * Разбор полей записи на элементы ФИО автора.
 *
 * @param record Библиографическая запись.
 * @param tag Метка поля.
 * @param authors Массив для размещения результата.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL author_parse_record
    (
        const MarcRecord *record,
        am_uint32 tag,
        Array *authors
    )
{
    size_t index;
    const MarcField *field;
    Author *author;

    assert (record != NULL);
    assert (tag != 0);
    assert (authors != NULL);

    for (index = 0; index < record->fields.len; ++index) {
        field = (const MarcField*) array_get (&record->fields, index);
        if (field->tag == tag) {
            author = (Author*) array_emplace_back (authors);
            if (author == NULL) {
                return AM_FALSE;
            }

            author_init (author);
            if (!author_parse_field (author, field)) {
                return AM_FALSE;
            }
        }
    }

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
