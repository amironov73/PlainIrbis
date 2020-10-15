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
   \file reader.c

   Сведения о читателе (база `RDR`).

   \struct ReaderInfo
        \brief Сведения о читателе (база `RDR`).
        \details Владеет собственной память (кроме поля `record`).
        Для освобождения ресурсов используйте `reader_destroy`.

   \var ReaderInfo::familyName
        \brief Фамилия, поле 10.

   \var ReaderInfo::firstName
        \brief Имя, поле 11.

   \var ReaderInfo::patronymic
        \brief Отчество, поле 12.

   \var ReaderInfo::dateOfBirth
        \brief Дата рождения, поле 21.

   \var ReaderInfo::passCard
        \brief Номер пропуска, поле 22.

   \var ReaderInfo::ticket
        \brief Номер читательского билета (идентификатор читателя), поле 30.

   \var ReaderInfo::gender
        \brief Пол, поле 23.

   \var ReaderInfo::category
        \brief Категория читателя, поле 50.

   \var ReaderInfo::workPlace
        \brief Место работы, поле 15.

   \var ReaderInfo::education
        \brief Образование, поле 20.

   \var ReaderInfo::email
        \brief Электронная почта, поле 32.

   \var ReaderInfo::phone
        \brief Домашний телефон, поле 17.

   \var ReaderInfo::record
        \brief Запись, из которой извлечены сведения о читателе (не владеет памятью).

 */

/*=========================================================*/

#define assign(__b, __r, __t) \
    buffer_assign_span ((__b), record_fm ((__r), (__t), 0))

#define apply(__r, __t, __b) \
    record_set_field ((__r), (__t), buffer_to_span (__b))

/*=========================================================*/

/**
 * Простая инициализация структуры.
 * Не выделяет памяти в куче.
 *
 * @param reader Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL reader_init
    (
        ReaderInfo *reader
    )
{
    assert (reader != NULL);

    mem_clear (reader, sizeof (*reader));
}

/**
 * Освобождение ресурсов, занятых структурой.
 *
 * @param reader Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL reader_destroy
    (
        ReaderInfo *reader
    )
{
    assert (reader != NULL);

    buffer_destroy (&reader->familyName);
    buffer_destroy (&reader->firstName);
    buffer_destroy (&reader->patronymic);
    buffer_destroy (&reader->dateOfBirth);
    buffer_destroy (&reader->passCard);
    buffer_destroy (&reader->ticket);
    buffer_destroy (&reader->gender);
    buffer_destroy (&reader->category);
    buffer_destroy (&reader->workPlace);
    buffer_destroy (&reader->education);
    buffer_destroy (&reader->email);
    buffer_destroy (&reader->phone);
    reader->record = NULL;
}

/**
 * Применение сведений об читателе к указанной записи.
 *
 * @param reader Сведения о читателе.
 * @param record Запись для заполнения.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL reader_apply
    (
        const ReaderInfo *reader,
        MarcRecord *record
    )
{
    assert (reader != NULL);
    assert (record != NULL);

    record_clear (record);
    return apply (record, 10, &reader->familyName)
           && apply (record, 11, &reader->firstName)
           && apply (record, 12, &reader->patronymic)
           && apply (record, 21, &reader->dateOfBirth)
           && apply (record, 22, &reader->passCard)
           && apply (record, 30, &reader->ticket)
           && apply (record, 23, &reader->gender)
           && apply (record, 50, &reader->category)
           && apply (record, 15, &reader->workPlace)
           && apply (record, 20, &reader->education)
           && apply (record, 32, &reader->email)
           && apply (record, 17, &reader->phone);
}

/**
 * Разбор указанной записи на сведения о читателе.
 *
 * @param reader Сведения о читателе, подлежащие заполнению.
 * @param record Запись для разбора.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL reader_parse_record
    (
        ReaderInfo *reader,
        const MarcRecord *record
    )
{
    assert (reader != NULL);
    assert (record != NULL);

    reader->record = (MarcRecord*) record;

    return assign (&reader->familyName,     record, 10)
           && assign (&reader->firstName,   record, 11)
           && assign (&reader->patronymic,  record, 12)
           && assign (&reader->dateOfBirth, record, 21)
           && assign (&reader->passCard,    record, 22)
           && assign (&reader->ticket,      record, 30)
           && assign (&reader->gender,      record, 23)
           && assign (&reader->category,    record, 50)
           && assign (&reader->workPlace,   record, 15)
           && assign (&reader->education,   record, 20)
           && assign (&reader->email,       record, 32)
           && assign (&reader->phone,       record, 17);
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
