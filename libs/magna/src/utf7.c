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
 * \file utf7.c
 *
 * Кодировка UTF-7.
 *
 * https://ru.wikipedia.org/wiki/UTF-7
 *
 * UTF-7 (от англ. 7-bit Unicode Transformation Format — «формат
 * преобразования Юникода, 7 бит») формат кодирования текста
 * Юникод с переменной длиной символьных слов в последовательность
 * символов ASCII. Первоначально предназначался для кодирования
 * текстов Юникода в сообщениях электронной почты Интернета,
 * которые были более эффективными, чем комбинация UTF-8
 * с quoted-printable.
 *
 */

/*=========================================================*/

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
