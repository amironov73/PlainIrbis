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
