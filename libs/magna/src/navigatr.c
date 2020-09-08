/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 5045)
#endif

/*=========================================================*/

#include <assert.h>
#include <ctype.h>

/*=========================================================*/

/**
 * \file navigatr.c
 *
 * Навигация по произвольному тексту.
 *
 */

/*=========================================================*/

/**
 * Инициализация.
 *
 * @param nav
 * @param data
 * @param dataSize
 * @return
 */
MAGNA_API TextNavigator* MAGNA_CALL nav_init
    (
            TextNavigator* nav,
            const am_byte *data,
            am_size_t dataSize
    )
{
    assert (data != NULL);
    assert (data != NULL);

    memset (nav, 0, sizeof (TextNavigator));
    nav->data = data;
    nav->length = dataSize;
    nav->position = 0;
    nav->line = 1;
    nav->column = 1;

    return nav;
}

/**
 * Инициализация из фрагмента.
 *
 * @param nav Навигатор.
 * @param span Фрагмент.
 * @return Навигатор.
 */
MAGNA_API TextNavigator* MAGNA_CALL nav_from_span
    (
        TextNavigator *nav,
        Span span
    )
{
    assert (nav != NULL);

    return nav_init (nav, span.ptr, span.len);
}

/**
 * Инициализация из буфера.
 *
 * @param nav Навигатор.
 * @param buffer Буфер.
 * @return Навигатор.
 */
MAGNA_API TextNavigator* MAGNA_CALL nav_from_buffer
    (
        TextNavigator *nav,
        const Buffer *buffer
    )
{
    assert (nav != NULL);
    assert (buffer != NULL);

    return nav_init (nav, buffer->ptr, buffer->position);
}

/**
 * Инициализация строкой. Стоп-символ в навигатор не помещается.
 *
 * @param nav Навигатор.
 * @param text Строка с текстом.
 * @return Навигатор.
 */
MAGNA_API TextNavigator* MAGNA_CALL nav_from_text
    (
        TextNavigator *nav,
        const char *text
    )
{
    assert (nav != NULL);
    assert (text != NULL);

    return nav_init (nav, (am_byte*) text, (am_size_t) strlen (text));
}

/**
 * Указатель прямо за последним байтом.
 *
 * @param nav
 * @return
 */
MAGNA_API const am_byte* MAGNA_CALL nav_end
    (
        const TextNavigator *nav
    )
{
    assert(nav != NULL);

    return nav->data + nav->length;
}

/**
 * Указатель на текущий байт.
 *
 * @param nav
 * @return
 */
MAGNA_API const am_byte* MAGNA_CALL nav_current
    (
        const TextNavigator *nav
    )
{
    assert(nav != NULL);

    return nav->data + nav->position;
}

/**
 * Достигнут ли конец текста?
 *
 * @param nav
 * @return
 */
MAGNA_API am_bool MAGNA_CALL nav_eot
    (
        const TextNavigator *nav
    )
{
    assert (nav != NULL);

    return nav->position >= nav->length;
}

/**
 * Подглядываем символ в указанной позиции
 * (отсчет от начала текста, от 0).
 *
 * @param nav
 * @param position
 * @return
 */
MAGNA_API int MAGNA_CALL nav_at
    (
            const TextNavigator *nav,
            am_size_t position
    )
{
    assert (nav != NULL);

    return position < nav->length
        ? nav->data[position]
        : NAV_EOT;
}

/**
 * Первый символ в тексте.
 *
 * @param nav
 * @return
 */
MAGNA_API int MAGNA_CALL nav_front
    (
        const TextNavigator *nav
    )
{
    assert (nav != NULL);

    return nav_at (nav, 0);
}

/**
 * Последний символ в тексте.
 *
 * @param nav
 * @return
 */
MAGNA_API int MAGNA_CALL nav_back
    (
        const TextNavigator *nav
    )
{
    assert (nav != NULL);

    return nav_at (nav, nav->length - 1);
}

/**
 * Заглядывание вперед на указанное количество символов.
 *
 * @param nav
 * @param distance
 * @return
 */
MAGNA_API int MAGNA_CALL nav_look_ahead
    (
            const TextNavigator *nav,
            am_size_t distance
    )
{
    am_size_t newPos;

    assert (nav != NULL);

    newPos = nav->position + distance;

    return nav_at (nav, newPos);
}

/**
 * Заглядывание назад на указанное количество символов.
 *
 * @param nav
 * @param distance
 * @return
 */
MAGNA_API int MAGNA_CALL nav_look_behind
    (
            const TextNavigator *nav,
            am_size_t distance
    )
{
    am_size_t newPos;

    assert (nav != NULL);

    newPos = nav->position - distance;

    return nav_at (nav, newPos);
}

/**
 * Перемещение по тексту вперед/назад.
 *
 * @param nav
 * @param distance
 * @return
 */
MAGNA_API TextNavigator* MAGNA_CALL nav_move
    (
            TextNavigator *nav,
            am_ssize_t distance
    )
{
    assert (nav != NULL);

    /* TODO: some checks */
    nav->column += distance;
    nav->position += distance;

    return nav;
}

/**
 * Подсматриваем текущий символ.
 *
 * @param nav
 * @return
 */
MAGNA_API int MAGNA_CALL nav_peek
    (
        const TextNavigator * nav
    )
{
    assert (nav != NULL);

    return nav_at (nav, nav->position);
}

/**
 * Подсматриваем текущий символ.
 * @param nav
 * @return
 */
MAGNA_API int MAGNA_CALL nav_peek_no_crlf
    (
        const TextNavigator *nav
    )
{
    am_size_t distance = 0;
    int result;

    assert (nav != NULL);

    while (AM_TRUE) {
        result = nav_at (nav, nav->position + distance);
        if (result != '\r' && result != '\n') {
            break;
        }
        ++distance;
    }

    return result;
}

/**
 * Считывание текущего символа, движение вперед по тексту.
 *
 * @param nav
 * @return
 */
MAGNA_API int MAGNA_CALL nav_read
    (
        TextNavigator *nav
    )
{
    int result;

    assert (nav != NULL);

    result = nav_at (nav, nav->position);
    if (result == '\n') {
        ++nav->line;
        nav->column = 1;
    }
    else {
        ++nav->column;
    }

    return result;
}

/**
 * Считывание текущего символа. Перевод строки и возврат каретки
 * пропускаются. Движение вперед по тексту.
 *
 * @param nav Навигатор.
 * @return Прочитанный символ либо `NAV_EOT`.
 */
MAGNA_API int MAGNA_CALL nav_read_no_crlf
    (
        TextNavigator *nav
    )
{
    int result;

    assert (nav != NULL);

    while (AM_TRUE) {
        result = nav_read (nav);
        if (result != '\r' && result != '\n') {
            break;
        }
    }

    return result;
}

/**
 * Подглядывание строки вплоть до указанной длины.
 * Движения не происходит.
 *
 * @param nav
 * @param length
 * @return Подсмотренная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_peek_string
    (
            const TextNavigator *nav,
            am_size_t length
    )
{
    int c;
    am_size_t i;
    Span result = SPAN_INIT;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav_current (nav);
    for (i = 0; i < length; ++i) {
        c = nav_look_ahead (nav, i);
        if ((c == NAV_EOT) || (c == '\r') || (c == '\n')) {
            break;
        }
        ++result.len;
    }

    return result;
}

/**
 * Подглядывание вплоть до указанного символа (включая его).
 * Движения не происходит.
 *
 * @param nav
 * @param stopChar
 * @return Подсмотренная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_peek_to
    (
        const TextNavigator *nav,
        am_byte stopChar
    )
{
    int c;
    am_size_t i, length;
    Span result = SPAN_INIT;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    length = nav->length - nav->position;
    for (i = 0; i < length; ++i) {
        c = nav_look_ahead (nav, i);
        ++result.len;
        if (c == stopChar) {
            break;
        }
    }

    return result;
}

/**
 * Подглядывание вплоть до указанного символа (не включая его).
 * Движения не происходит.
 *
 * @param nav
 * @param stopChar
 * @return Подсмотренная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_peek_until
    (
        const TextNavigator *nav,
        am_byte stopChar
    )
{
    int c;
    am_size_t i, length;
    Span result = SPAN_INIT;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    length = nav->length - nav->position;
    for (i = 0; i < length; ++i) {
        c = nav_look_ahead (nav, i);
        if (c == stopChar) {
            break;
        }
        ++result.len;
    }

    return result;
}

/**
 * Считывание строки (вплоть до символа перевода строки).
 * Символ перевода строки считывается, но в результат не помещается.
 * Происходит движение вперед.
 *
 * @param nav
 * @return Считанная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_read_line
    (
        TextNavigator *nav
    )
{
    am_byte c;
    am_size_t start;
    Span result = SPAN_INIT;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    start = nav->position;
    while (nav->position < nav->length) {
        c = nav->data[nav->position];
        if (c == '\r' || c == '\n') {
            break;
        }
        (void) nav_read (nav);
    }

    result.len = nav->position - start;

    /* Проглатываем перевод строки */
    if (nav->position < nav->length) {
        c = nav->data[nav->position];
        if (c == '\r') {
            (void) nav_read (nav);
            c = nav->data[nav->position];
        }

        if (c == '\n') {
            (void) nav_read (nav);
        }
    }

    return result;
}

/**
 * Текущий символ -- управляющий?
 * Движения не происходит.
 *
 * @param nav
 * @return
 */
MAGNA_API am_bool MAGNA_CALL nav_is_control
    (
        const TextNavigator *nav
    )
{
    int c;

    assert (nav != NULL);

    c = nav_peek (nav);

    return (c > 0) && (c < ' ');
}

/**
 * Текущий символ -- арабская цифра?
 * Движения не происходит.
 *
 * @param nav
 * @return
 */
MAGNA_API am_bool MAGNA_CALL nav_is_digit
    (
        const TextNavigator *nav
    )
{
    int c;

    assert (nav != NULL);

    c = nav_peek (nav);

    return (c >= '0') && (c <= '9');
}

/**
 * Текущий символ -- буква латинского алфавита?
 * Движения не происходит.
 *
 * @param nav
 * @return
 */
MAGNA_API am_bool MAGNA_CALL nav_is_letter
    (
        const TextNavigator *nav
    )
{
    int c;

    assert (nav != NULL);

    c = nav_peek (nav);

    return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'));
}

/**
 * Текущий символ -- пробельный?
 * Движения не происходит.
 *
 * @param nav
 * @return
 */
MAGNA_API am_bool MAGNA_CALL nav_is_whitespace
    (
        const TextNavigator *nav
    )
{
    int c;

    assert (nav != NULL);

    c = nav_peek (nav);

    return (c == ' ') || (c == '\t') || (c == '\v') || (c == '\r')
        || (c == '\n');
}

/**
 * Чтение целого числа без знака.
 * Происходит движение вперед.
 *
 * @param nav
 * @return Прочитанная строка с числом (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_read_integer
    (
        TextNavigator *nav
    )
{
    Span result = SPAN_INIT;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    while (nav_is_digit (nav)) {
        ++result.len;
        (void) nav_read (nav);
    }

    return result;
}

/**
 * Извлечение целого числа без знака.
 * Сначала пропускаются все не-цифры,
 * затем считывается число (если возможно).
 *
 * @param nav
 * @return Прочитанная строка с числом (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_extract_integer
    (
        TextNavigator *nav
    )
{
    assert (nav != NULL);

    while (!nav_eot (nav) && !nav_is_digit (nav)) {
        (void) nav_read (nav);
    }

    return nav_read_integer (nav);
}

/**
 * Чтение строки вплоть до указанной длины.
 * Происходит движение вперед.
 *
 * @param nav
 * @param length Максимальная длина строки.
 * @return Прочитанная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_read_string
    (
            TextNavigator *nav,
            am_size_t length
    )
{
    int c;
    am_size_t i;
    Span result = SPAN_INIT;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    for (i = 0; i < length; ++i) {
        c = nav_read (nav);
        if (c == NAV_EOT) {
            break;
        }
        ++result.len;
    }

    return result;
}

/**
 * Считывание вплоть до указанного символа (не включая его).
 * Сам стоп-символ считывается, но в результат не помещается.
 * Происходит движение вперед.
 *
 * @param nav Навигатор.
 * @param stopChar Стоп-символ.
 * @return Прочитанная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_read_to
    (
        TextNavigator *nav,
        am_byte stopChar
    )
{
    int c;
    Span result = SPAN_INIT;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    while (AM_TRUE) {
        c = nav_read (nav);
        if ((c == NAV_EOT) || (c == stopChar)) {
            break;
        }
        ++result.len;
    }

    return result;
}

/**
 * Считывание вплоть до указанного символа (не включая его).
 * Сам стоп-символ остается не прочитанным.
 * Происходит движение вперед.
 *
 * @param nav Навигатор.
 * @param stopChar Стоп-символ.
 * @return Прочитанная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_read_until
    (
        TextNavigator *nav,
        am_byte stopChar
    )
{
    int c;
    Span result = SPAN_INIT;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    while (AM_TRUE) {
        c = nav_peek (nav);
        if ((c == NAV_EOT) || (c == stopChar)) {
            break;
        }

        (void) nav_read (nav);
        ++result.len;
    }

    return result;
}

/**
 * Считывание, пока встречается указанный символ.
 * Происходит движение вперед.
 *
 * @param nav Навигатор.
 * @param goodChar "Хороший" символ.
 * @return Прочитанна строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_read_while
    (
        TextNavigator *nav,
        am_byte goodChar
    )
{
    int c;
    Span result = SPAN_INIT;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    while (AM_TRUE) {
        c = nav_peek (nav);
        if ((c == NAV_EOT) || (c != goodChar)) {
            break;
        }

        (void) nav_read (nav);
        ++result.len;
    }

    return result;
}

/**
 * Чтение слова.
 * Происходит двжиение вперед.
 *
 * @param nav Навигатор.
 * @return Прочитанное слово (возможно, пустое).
 */
MAGNA_API Span MAGNA_CALL nav_read_word
    (
        TextNavigator *nav
    )
{
    int c;
    Span result = SPAN_INIT;

    assert (nav != NULL);
    result.ptr = (am_byte*) nav->data + nav->position;
    while (AM_TRUE) {
        c = nav_peek (nav);
        if ((c == NAV_EOT) || !isalnum (c)) {
            break;
        }

        (void) nav_read (nav);
        ++result.len;
    }

    return result;
}

/**
 * Получение непрочитанного остатка.
 * Движения не происходит.
 *
 * @param nav Навигатор.
 * @return Непрочитанный фрагмент (возможно, пустой).
 */
MAGNA_API Span MAGNA_CALL nav_remaining
    (
        const TextNavigator *nav
    )
{
    Span result;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    result.len = nav->length - nav->position;

    return result;
}

/**
 * Преобразование в фрагмент.
 * Движения не происходит.
 *
 * @param nav Навигатор.
 * @return Фрагмент (возможно, пустой).
 */
MAGNA_API Span MAGNA_CALL nav_to_span
    (
        const TextNavigator *nav
    )
{
    Span result;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data;
    result.len = nav->length;

    return result;
}

/**
 * Получение фрагмента заданного размера.
 * Движения не происходит.
 *
 * @param nav Навигатор.
 * @param offset Смечение от начала данных.
 * @param length Желаемая длина фрагмента.
 * @return Полученный фрагмент (возможно, пустой).
 */
MAGNA_API Span MAGNA_CALL nav_slice
    (
            const TextNavigator *nav,
            am_size_t offset,
            am_size_t size
    )
{
    Span result;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + offset;
    result.len = size;

    return result;
}

/**
 * Получение нескольких последних прочитанных символов.
 * Движения не происходит.
 *
 * @param nav Навигатор.
 * @param length Сколько символов надо.
 * @return Полученный фрагмент (возможно, пустой).
 */
MAGNA_API Span MAGNA_CALL nav_recent
    (
            const TextNavigator *nav,
            am_ssize_t length
    )
{
    am_ssize_t start;

    assert (nav != NULL);

    start = nav->position - length;
    if (start < 0) {
        length += start;
        start = 0;
    }

    if (((am_size_t)start) >= nav->length) {
        start = 0;
        length = 0;
    }

    if (length < 0) {
        length = 0;
    }

    return nav_slice (nav, (am_size_t) start, (am_size_t) length);
}

/**
 * Пропуск символов, не входящих в слово.
 * Происходит движение вперед.
 *
 * @param nav Навигатор.
 */
MAGNA_API void MAGNA_CALL nav_skip_non_word
    (
        TextNavigator *nav
    )
{
    int c;

    assert (nav != NULL);

    while (AM_TRUE) {
        c = nav_peek (nav);
        if ((c == NAV_EOT) || isalnum (c)) {
            break;
        }

        (void) nav_read (nav);
    }
}

/**
 * Пропуск пробельных.
 * Происходит движение вперед.
 *
 * @param nav Навигатор.
 */
MAGNA_API void MAGNA_CALL nav_skip_whitespace
    (
        TextNavigator *nav
    )
{
    int c;

    assert (nav != NULL);

    while (AM_TRUE) {
        c = nav_peek (nav);
        if ((c == NAV_EOT) || !isspace (c)) {
            break;
        }

        (void) nav_read (nav);
    }
}

/**
 * Пропуск пробельных.
 * Происходит движение вперед.
 *
 * @param nav Навигатор.
 */
MAGNA_API void MAGNA_CALL nav_skip_punctuation
    (
        TextNavigator *nav
    )
{
    int c;

    assert (nav != NULL);

    while (AM_TRUE) {
        c = nav_peek (nav);
        if ((c == NAV_EOT) || !ispunct (c)) {
            break;
        }

        (void) nav_read (nav);
    }
}

/**
 * Считывание одного символа в кодировке UTF-8.
 *
 * @param nav
 * @return
 */
MAGNA_API int MAGNA_CALL nav_read_utf8
    (
        TextNavigator *nav
    )
{
    assert (nav != NULL);

    return -1;
}

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
