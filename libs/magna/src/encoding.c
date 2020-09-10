/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 * \file encoding.c
 *
 * Работа с кодировками.
 */

/*=========================================================*/

Encoding cp1251_encoding = {
    "CP1251",
    cp1251_char_to_unicode,
    cp1251_unicode_to_char
};

Encoding cp866_encoding = {
    "CP866",
    cp866_char_to_unicode,
    cp866_unicode_to_char
};

Encoding koi8r_encoding = {
    "KOI-8r",
    koi8r_char_to_unicode,
    koi8r_unicode_to_char
};

static Encoding* _builtin_encodings[] = { &cp1251_encoding, &cp866_encoding, &koi8r_encoding };

static Array _registered_encodings = ARRAY_INIT;

static void _register_builtin_encodings()
{
    am_size_t index, count;

    if (_registered_encodings.ptr == NULL) {
        count = sizeof (_builtin_encodings) / sizeof (_builtin_encodings[0]);
        array_create (&_registered_encodings, count);
        for (index = 0; index < count; ++index) {
            array_push_back (&_registered_encodings, _builtin_encodings[index]);
        }
    }
}

/**
 * Регистрация кодировки.
 *
 * @param encoding Кодировка.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL encoding_register
    (
        const Encoding *encoding
    )
{
    assert (encoding != NULL);

    _register_builtin_encodings();

    /* TODO: проверять наличие среди уже зарегистрированных */
    return array_push_back (&_registered_encodings, (void*) encoding);
}

/**
 * Поиск кодировки по имени.
 *
 * @param name Имя кодировки.
 * @return Указатель на найденную кодировку либо `NULL`.
 */
MAGNA_API Encoding* MAGNA_CALL encoding_get
    (
        const char *name
    )
{
    am_size_t index;
    Encoding *encoding;

    assert (name != NULL);

    for (index = 0; index < _registered_encodings.len; ++index) {
        encoding = (Encoding*) array_get (&_registered_encodings, index);
        if (same_text(encoding->name, name)) {
            return encoding;
        }
    }

    return NULL;
}

/**
 * Встроенная кодировка ANSI.
 *
 * @return
 */
MAGNA_API Encoding* encoding_ansi (void)
{
    return &cp1251_encoding;
}

/**
 * Поиск широкого символа в таблице перекодировки в узкий.
 * Таблица перекодировки должна быть отсортированной!
 *
 * @param array Таблица перекодировки (256 символов).
 * @param left Левая граница (включительно).
 * @param right Правая граница (включительно).
 * @param value Искомый широкий символ.
 * @return Поизиция, в которой символ найден, либо -1.
 */
MAGNA_API am_ssize_t MAGNA_CALL search_for_unicode
    (
        am_wchar *array,
        am_ssize_t left,
        am_ssize_t right,
        am_wchar value
    )
{
    am_ssize_t mid;

    AGAIN:
    if (right >= left) {
        mid = left + (right - left) / 2;

        /* If the element is present at the middle itself */
        if (array [mid] == value) {
            return mid;
        }

        /* If element is smaller than mid, then
           it can only be present in left subarray */
        if (array [mid] > value) {
            right = mid - 1;
            goto AGAIN;
            /* return search_for_unicode (array, left, mid - 1, value); */
        }

        /* Else the element can only be present in right subarray */
        left = mid + 1;
        goto AGAIN;
        /* return search_for_unicode (array, mid + 1, right, value); */
    }

    /* We reach here when element is not present in array */
    return -1;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
