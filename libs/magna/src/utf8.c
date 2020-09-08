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
        unsigned chr
    )
{
    assert (buffer != NULL);

    if (chr < (1u << 7u)) {
        return buffer_putc (buffer, (char) chr);
    }

    if (chr < (1u << 11u)) {

        return buffer_putc (buffer, (chr >> 6u) | 0xC0u)
            && buffer_putc (buffer, (chr & 0x3Fu) | 0x80u);
    }

    if (chr < (1u << 16u)) {
        return buffer_putc (buffer, (chr >> 12u) | 0xE0u)
            && buffer_putc (buffer, ((chr >> 6u) & 0x3Fu) | 0x80u)
            && buffer_putc (buffer, (chr & 0x3Fu) | 0x80u);
    }

    if (chr < (1u << 21u)) {
        return buffer_putc (buffer, (chr >> 18u) | 0xF0u)
            && buffer_putc (buffer, ((chr >> 12u) & 0x3Fu) | 0x80u)
            && buffer_putc (buffer, ((chr >> 6u) & 0x3Fu) | 0x80u)
            && buffer_putc (buffer, (chr & 0x3Fu) | 0x80u);
    }

    return AM_FALSE;
}

/**
 * Подсчитывает число code point в массиве данных в кодировке UTF-8.
 *
 * @param data Данные.
 * @param dataLength Длина данных в байтах.
 * @return Число code point.
 */
MAGNA_API am_size_t MAGNA_CALL utf8_code_points
    (
        const am_byte *data,
        am_size_t dataLength
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
MAGNA_API am_size_t MAGNA_CALL count_utf
    (
        const am_wchar *src,
        am_size_t length
    )
{
    am_size_t result = 0;
    unsigned int c;

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
