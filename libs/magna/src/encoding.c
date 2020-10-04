// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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

static Encoding* builtinEncodings[] = {&cp1251_encoding, &cp866_encoding, &koi8r_encoding };

static Vector registeredEncodings = VECTOR_INIT;

static void register_builtin_encodings()
{
    size_t index, count;

    if (registeredEncodings.ptr == NULL) {
        count = sizeof (builtinEncodings) / sizeof (Encoding);
        vector_create (&registeredEncodings, count);
        for (index = 0; index < count; ++index) {
            vector_push_back(&registeredEncodings, builtinEncodings[index]);
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
    assert (encoding->name != NULL);

    register_builtin_encodings();

    if (encoding_get (encoding->name) != NULL) {
        return AM_TRUE;
    }

    return vector_push_back(&registeredEncodings, (void *) encoding);
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
    size_t index;
    Encoding *encoding;

    assert (name != NULL);

    for (index = 0; index < registeredEncodings.len; ++index) {
        encoding = (Encoding*) vector_get(&registeredEncodings, index);
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
MAGNA_API ssize_t MAGNA_CALL search_for_unicode
    (
        const am_wchar *array,
        ssize_t left,
        ssize_t right,
        am_wchar value
    )
{
    ssize_t mid;

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

/**
 * Преобразование текста из кодировки ANSI в UTF-8.
 *
 * @param output Буфер для размещения результата.
 * @param input Исходный текст.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL ansi2utf
    (
        Buffer *output,
        Span input
    )
{
    const Encoding *ansi;
    size_t index;
    unsigned int chr;

    assert (output != NULL);

    ansi = encoding_ansi();
    assert (ansi != NULL);

    for (index = 0; index < input.len; ++index) {
        chr = ansi->char_to_unicode (input.ptr[index]);
        if (!buffer_putc_utf8 (output, chr)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/**
 * Преобразование текста из кодировки UTF-8 в UTF-8.
 *
 * @param output Буфер для размещения результата.
 * @param input Исходный текст.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL utf2ansi
    (
        Buffer *output,
        Span input
    )
{
    const Encoding *ansi;
    const am_byte *text, *stop;
    UtfHelper rc;
    am_byte chr;

    assert (output != NULL);

    ansi = encoding_ansi();
    assert (ansi != NULL);

    stop = (text = input.ptr) + input.len;
    while (text < stop) {
        rc = utf8_read (text);
        chr = ansi->unicode_to_char (rc.value);
        if (!buffer_putc (output, chr)) {
            return AM_FALSE;
        }

        text = rc.position;
    }

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
