// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/irbis.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/*!
    \file upc.c

    UPC или Universal Product Code (универсальный код
    товара) — американский стандарт штрихкода,
    предназначенный для отслеживания товаров в магазинах.
    UPC был разработан в 1973 году Джорджем Джосефом
    Лорером (George Joseph Laurer), работавшим инженером
    в корпорации IBM. В июне 1974 года первый UPC сканер
    производства корпорации NCR был установлен
    в супермаркете Марш (Marsh) в городе Трой (Troy)
    штата Огайо. 26 июня 1974 года кассиром этого
    супермаркета был просканирован первый товар — блок
    10 фруктовых жевательных резинок компании Wrigley.

    Пример проверки контрольной суммы

    ```
    041689300494 (бензин для зажигалки «Zippo») — код UPC-12.
    0x3 + 4x1 + 1x3 + 6x1 + 8x3 + 9x1 + 3x3 + 0x1 + 0x3 + 4x1 + 9x3 + 4x1=
    0 + 4 + 3 + 6 + 24 + 9 + 9 + 0 + 0 + 4 + 27 + 4= 90.

    Контрольная сумма = 0 — номер правильный.
    ```
 */

/*=========================================================*/

static int coefficients[] = { 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1 };

/**
 * Вычисление контрольной цифры UPC-12.
 *
 * @param text Цифры UPC без посторонних знаков.
 * @return Вычисленная контрольная цифра.
 */
MAGNA_API am_byte MAGNA_CALL upc12_compute_check_digit
    (
        const Span text
    )
{
    am_int32 sum, i, length;
    am_byte result;

    assert (text.len >= 11);

    length = min_int32 (text.len, 11);
    for (sum = i = 0; i < length; ++i) {
        sum = sum + (text.ptr[i] - '0') * coefficients[i];
    }

    result = (am_byte) (10 - sum % 10 + '0');

    return result;
}

/**
 * Проверка контрольной цифры.
 *
 * @param text Цифры UPC без посторонних знаков.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL upc12_check_control_digit
    (
        const Span text
    )
{
    am_int32 sum, i, length;

    assert (text.len >= 12);

    length = min_int32 (text.len, 12);

    for (sum = i = 0; i < length; ++i) {
        sum = sum + (text.ptr[i] - '0') * coefficients[i];
    }

    return sum % 10 == 0;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
