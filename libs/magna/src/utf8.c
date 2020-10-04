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
 * \file utf8.c
 *
 * Кодировка UTF-8.
 */

/*=========================================================*/

/**
 * Добавление к буферу символа в кодировке UTF-8.
 *
 * @param buffer Инициализированный буфер.
 * @param chr Символ.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_putc_utf8
    (
        Buffer *buffer,
        unsigned int chr
    )
{
    assert (buffer != NULL);

    if (chr < (1u << 7u)) {
        return buffer_putc (buffer, (char) chr);
    }

    if (chr < (1u << 11u)) {
        return buffer_putc (buffer, (am_byte) ((chr >> 6u) | 0xC0u))
            && buffer_putc (buffer, (am_byte) ((chr & 0x3Fu) | 0x80u));
    }

    if (chr < (1u << 16u)) {
        return buffer_putc (buffer, (am_byte) ((chr >> 12u) | 0xE0u))
            && buffer_putc (buffer, (am_byte) (((chr >> 6u) & 0x3Fu) | 0x80u))
            && buffer_putc (buffer, (am_byte) ((chr & 0x3Fu) | 0x80u));
    }

    if (chr < (1u << 21u)) {
        return buffer_putc (buffer, (am_byte) ((chr >> 18u) | 0xF0u))
            && buffer_putc (buffer, (am_byte) (((chr >> 12u) & 0x3Fu) | 0x80u))
            && buffer_putc (buffer, (am_byte) (((chr >> 6u) & 0x3Fu) | 0x80u))
            && buffer_putc (buffer, (am_byte) ((chr & 0x3Fu) | 0x80u));
    }

    return AM_FALSE;
}

/**
 * Чтение текста в кодировке UTF-8 по кодовым точкам.
 *
 * @param text Текст в кодировке UTF-8.
 * @param stop Указатель прямо за последним символом.
 * @return Прочитанный символ и указатель на следующий байт для чтения.
 */
MAGNA_API UtfHelper MAGNA_CALL utf8_read
    (
        const am_byte *text
    )
{
    unsigned int chr;
    UtfHelper result;

    assert (text != NULL);

    chr = (unsigned int) *text++;
    if ((chr & 0x80u) == 0u)
    {
        // 1-Byte sequence: 000000000xxxxxxx = 0xxxxxxx
    }
    else if ((chr & 0xE0u) == 0xC0u)
    {
        // 2-Byte sequence: 00000yyyyyxxxxxx = 110yyyyy 10xxxxxx
        chr = (chr & 0x1Fu) << 6u;
        chr |= (*text++ & 0x3Fu);
    }
    else if ((chr & 0xF0u) == 0xE0u)
    {
        // 3-Byte sequence: zzzzyyyyyyxxxxxx = 1110zzzz 10yyyyyy 10xxxxxx
        chr = (chr & 0x0Fu) << 12u;
        chr |= (*text++ & 0x3Fu) << 6u;
        chr |= (*text++ & 0x3Fu);
    }
    else if ((chr & 0xF8u) == 0xF0u)
    {
        // 4-Byte sequence: 11101110wwwwzzzzyy + 110111yyyyxxxxxx = 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
        chr = (chr & 0x07u) << 18u;
        chr |= (*text++ & 0x3Fu) << 12u;
        chr |= (*text++ & 0x3Fu) << 6u;
        chr |= (*text++ & 0x3Fu);
    }

    result.position = text;
    result.value = chr;

    return result;
}

/**
 * Подсчитывает число code point в массиве данных в кодировке UTF-8.
 *
 * @param data Данные.
 * @param dataLength Длина данных в байтах.
 * @return Число code point.
 */
MAGNA_API size_t MAGNA_CALL utf8_code_points
    (
        const am_byte *data,
        size_t dataLength
    )
{
    assert (data != NULL);

    /* TODO: implement */

    return 0;
}

/**
 * Подсчитывает число байт, необходимых для размещения текста в UTF-8.
 *
 * @param src Указатель на текст в UCS-16.
 * @param length Длина текста в символах.
 * @return Длина заданного текста в UTF-8 в байтах.
 */
MAGNA_API size_t MAGNA_CALL count_utf
    (
        const am_wchar *src,
        size_t length
    )
{
    size_t result = 0;
    am_uint32 c;

    while (length != 0) {
        c = *src++;
        if (c < (1u << 7u)) {
            ++result;
        }
        else if (c < (1u << 11u)) {
            ++result;
            ++result;
        }
        else if (c < (1u << 16u)) {
            result += 3;
        }
        else if (c < (1u << 21u)) {
            result += 4;
        }
        else {
            return 0;
        }
        --length;
    }

    return result;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
