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
    \file ean.c

    European Article Number, EAN (европейский номер
    товара) — европейский стандарт штрихкода,
    предназначенный для кодирования идентификатора
    товара и производителя. Является надмножеством
    американского стандарта UPC.

    Пример проверки контрольной суммы EAN13

    ```
    4600051000057 (сигареты «Прима») — код EAN-13.

    4x1 + 6x3 + 0x1 + 0x3 + 0x1 + 5x3 + 1x1 + 0x3 + 0x1 + 0x3 + 0x1 + 5x3 + 7x1=
    4 + 18 + 0 + 0 + 0 + 15 + 1 + 0 + 0 + 0 + 0 + 15 + 7= 60.

    Контрольная сумма = 0 — номер правильный.
    ```

    Источник: https://ru.wikipedia.org/wiki/European_Article_Number

    Пример проверки контрольной суммы EAN8

    ```
    46009333 (папиросы «Беломорканал») — код EAN-8.

    4x3 + 6x1 + 0x3 + 0x1 + 9x3 + 3x1 + 3x3 + 3x1=
    12 + 6 + 0 + 0 + 27 + 3 + 9 + 3= 60.

    Контрольная сумма = 0 — номер правильный.
    ```

 */

/*=========================================================*/

/* У EAN8 по сравнению с EAN13 коэффициенты сдвинуты на одну позицию. */
static int coefficients[] = { 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1 };

/**
 * Вычисление контрольной цифры EAN13.
 *
 * @param text Цифры EAN без посторонних знаков.
 * @return Вычисленная контрольная цифра. 0 означает ошибку.
 */
MAGNA_API am_byte MAGNA_CALL ean13_compute_check_digit
    (
        const Span text
    )
{
    am_byte result = 0;
    int i, sum = 0;

    if (span_length (text) < 12) {
        return result;
    }

    for (i = 0; i < 12; ++i) {
        sum = sum + (text.start [i] - '0') * coefficients[i];
    }

    result = (char)(10 - sum % 10 + '0');

    return result;
}

/**
 * Проверка контрольной цифры EAN13.
 *
 * @param text Цифры EAN13 без посторонних знаков.
 * @return Результат сравнения контрольной цифры с наличествующей.
 */
MAGNA_API am_bool MAGNA_CALL ean13_check_control_digit
    (
        const Span text
    )
{
    am_bool result = 0;
    int i, sum = 0;

    if (span_length (text) < 13) {
        return result;
    }

    for (i = 0; i < 13; ++i) {
        sum = sum + (text.start [i] - '0') * coefficients[i];
    }

    result = (sum % 10) == 0;

    return result;
}

/**
 * Вычисление контрольной цифры EAN8.
 * @param text Цифры EAN без посторонних знаков.
 * @return Вычисленная контрольная цифра.
 */
MAGNA_API am_byte MAGNA_CALL ean8_compute_check_digit
    (
        const Span text
    )
{
    am_byte result = 0;
    int i, sum = 0;

    if (span_length (text) < 7) {
        return result;
    }

    for (i = 0; i < 7; ++i) {
        sum = sum + (text.start [i] - '0') * coefficients[i + 1];
    }

    result = (char) (10 - sum % 10 + '0');

    return result;
}

/**
 * Проверка контрольной цифры.
 *
 * @param text Цифры EAN без посторонних знаков.
 * @return Результат сравнения контрольной цифры с наличествующей.
 */
MAGNA_API am_bool MAGNA_CALL ean8_check_control_digit
    (
        const Span text
    )
{
    am_bool result = 0;
    int i, sum = 0;

    if (span_length (text) < 8) {
        return result;
    }

    for (i = 0; i < 8; ++i) {
        sum = sum + (text.start [i] - '0') * coefficients[i + 1];
    }

    result = sum % 10 == 0;

    return result;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
