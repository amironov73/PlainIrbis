// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#if defined(MAGNA_WINDOWS)

    #ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4668)
    #endif

    #include <windows.h>

    #ifdef _MSC_VER
    #pragma warning(pop)
    #endif

#elif defined(MAGNA_MSDOS)

#include <dos.h>

#else

/* variouse UNIXes */

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#endif

#include <assert.h>

/*=========================================================*/

/*
 * \file date.c
 *
 * Работа с датой.
 */

/*=========================================================*/

/**
 * Конструктор из компонентов даты.
 *
 * @param date Инициализируемая дата.
 * @param year Год (4 цифры).
 * @param month Месяц (1-12).
 * @param day День месяца (1-31).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL date_init
    (
        struct tm *date,
        int year,
        int month,
        int day
    )
{
    assert (date != NULL);
    assert (year >= 1900);
    assert (month >= 1 && month <= 12);
    assert (day >= 1 && day <= 31);

    memset (date, 0, sizeof (struct tm));
    date->tm_mday = day;
    date->tm_mon = month - 1;
    date->tm_year = year - 1900;

    return AM_TRUE;
}

/**
 * Разбор даты на компоненты.
 *
 * @param date Дата.
 * @param year Указатель на год (может быть `NULL`).
 * @param month Указатель на месяц (может быть `NULL`).
 * @param day Указатель на день месяца (может быть `NULL`).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL date_decode
    (
        const struct tm *date,
        int *year,
        int *month,
        int *day
    )
{
    assert (date != NULL);
    assert (year != NULL || month != NULL || day != NULL);

    if (year != NULL) {
        *year = 1900 + date->tm_year;
    }

    if (month != NULL) {
        *month = date->tm_mon + 1;
    }

    if (day != NULL) {
        *day = date->tm_mday;
    }

    return AM_TRUE;
}

/**
 * Разбор текстового представления даты.
 *
 * @param date Дата, в которую должен быть помещен результат.
 * @param text Представление даты в виде YYYYMMDD (8 символов минимумм).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL date_parse
    (
        struct tm *date,
        Span text
    )
{
    int year, month, day;

    assert (date != NULL);
    assert (span_length (text) >= 8);

    year  = fastParse32 (text.start, 4);
    month = fastParse32 (text.start + 4, 2);
    day   = fastParse32 (text.start + 6, 2);

    return AM_FALSE;
}

/**
 * Преобразование даты в строку, принятую в ИРБИС64.
 *
 * @param date Дата.
 * @param buffer Адрес, по которому должна быть сформирована строка (минимум 9 байт).
 * @return Строка либо `NULL`.
 */
MAGNA_API am_byte* MAGNA_CALL date_to_text
    (
        const struct tm *date,
        am_byte *buffer
    )
{
    int year, month, day;

    assert (date != NULL);
    assert (buffer != NULL);

    if (!date_decode (date, &year, &month, &day)) {
        return NULL;
    }

    sprintf (CTEXT (buffer), "%04u%02u%02u", year, month, day);

    return buffer;
}

/**
 * Получение сегодняшней даты.
 *
 * @param date Куда должен быть помещен результат.
 * @return Признак успешности заверешения операции.
 */
MAGNA_API am_bool MAGNA_CALL date_today
    (
        struct tm *date
    )
{
    time_t t1;
    struct tm *t2;
    assert (date != NULL);

    t1 = time (NULL);
    t2 = localtime (&t1);
    *date = *t2;

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
