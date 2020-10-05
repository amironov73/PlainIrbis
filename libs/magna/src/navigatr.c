// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 * Инициализация навигатора.
 *
 * @param nav Указатель на неинициализированную структуру.
 * @param data Указатель на начало данных.
 * @param dataSize Длина данных в байтах.
 * @return Указатель на инициализированную структуру.
 */
MAGNA_API TextNavigator* MAGNA_CALL nav_init
    (
        TextNavigator* nav,
        const am_byte *data,
        size_t dataSize
    )
{
    assert (nav != NULL);
    assert (data != NULL);

    // mem_clear (nav, sizeof (TextNavigator));
    nav->data     = data;
    nav->length   = dataSize;
    nav->position = 0;
    nav->line     = 1;
    nav->column   = 1;

    return nav;
}

/**
 * Инициализация из фрагмента.
 *
 * @param nav Указатель на неинициализированную структуру.
 * @param span Фрагмент.
 * @return Указатель на инициализированную структуру.
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
 * Инициализация навигатора из буфера.
 * Навигатор воспримет данные от начала буфера до `position`.
 *
 * @param nav Указатель на неинициализированную структуру.
 * @param buffer Указатель на буфер.
 * @return Указатель на инициализированную структуру.
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
 * Инициализация навигатора строкой.
 * Стоп-символ в навигатор не помещается.
 *
 * @param nav Указатель на неинициализированную структуру.
 * @param text Строка с текстом.
 * @return Указатель на инициализированную структуру.
 */
MAGNA_API TextNavigator* MAGNA_CALL nav_from_text
    (
        TextNavigator *nav,
        const am_byte *text
    )
{
    assert (nav != NULL);
    assert (text != NULL);

    return nav_init (nav, (am_byte*) text, (size_t) strlen (text));
}

/**
 * Указатель прямо за последним байтом.
 *
 * @param nav Навигатор.
 * @return Указатель за последним байтом.
 */
MAGNA_API MAGNA_INLINE const am_byte* MAGNA_CALL nav_end
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
 * @param nav Навигатор.
 * @return Указатель на текущий байт.
 */
MAGNA_API MAGNA_INLINE const am_byte* MAGNA_CALL nav_current
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
 * @param nav Навигатор.
 * @return Результат проверки.
 */
MAGNA_API MAGNA_INLINE am_bool MAGNA_CALL nav_eot
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
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @param position Предполагаемая позиция.
 * @return Подсмотренный символ либо `NAV_EOT`,
 * если позиция за пределами текста.
 */
MAGNA_API int MAGNA_CALL nav_at
    (
        const TextNavigator *nav,
        size_t position
    )
{
    assert (nav != NULL);

    return position < nav->length
        ? nav->data[position]
        : NAV_EOT;
}

/**
 * Самый первый символ в тексте.
 *
 * @param nav Навигатор.
 * @return Первый символ либо `NAV_EOT`, если текст пустой.
 */
MAGNA_API MAGNA_INLINE int MAGNA_CALL nav_front
    (
        const TextNavigator *nav
    )
{
    assert (nav != NULL);

    return nav_at (nav, 0);
}

/**
 * Самый последний символ в тексте.
 *
 * @param nav Навигатор.
 * @return Последний символ, либо `NAV_EOT`, если текст пустой.
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
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @param distance Предполагаемая дистанция.
 * @return Подсмотренный символ либо `NAV_EOT`,
 * если позиция выходит за пределы текста.
 */
MAGNA_API int MAGNA_CALL nav_look_ahead
    (
        const TextNavigator *nav,
        size_t distance
    )
{
    assert (nav != NULL);

    return nav_at (nav, nav->position + distance);
}

/**
 * Заглядывание назад на указанное количество символов.
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @param distance Предполагаемая дистанция (положительное число).
 * @return Подсмотренный символ либо `NAV_EOT`,
 * если позиция выходит за пределы текста.
 */
MAGNA_API int MAGNA_CALL nav_look_behind
    (
        const TextNavigator *nav,
        size_t distance
    )
{
    assert (nav != NULL);

    return nav_at (nav, nav->position - distance);
}

/**
 * Перемещение по тексту вперед/назад.
 *
 * @param nav Навигатор.
 * @param distance Предполагаемая дистанция (положительное число
 * при движении вперед, отрицательное при движении назад).
 * @return Навигатор.
 */
MAGNA_API TextNavigator* MAGNA_CALL nav_move
    (
        TextNavigator *nav,
        ssize_t distance
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
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @return Подсмотренный символ либо `NAV_EOT`,
 * если произошел выход за границы текста.
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
 * Подсматриваем текущий символ (без учета перевода строки).
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @return Подсмотренный символ либо `NAV_EOT`,
 * если произошел выход за границы текста.
 */
MAGNA_API int MAGNA_CALL nav_peek_no_crlf
    (
        const TextNavigator *nav
    )
{
    size_t distance = 0;
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
    if (result >= 0) {
        if (result == '\n') {
            ++nav->line;
            nav->column = 0;
        } else {
            ++nav->column;
        }

        ++nav->position;
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
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @param length Предполгаемая длина строки.
 * @return Подсмотренная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_peek_string
    (
        const TextNavigator *nav,
        size_t length
    )
{
    int c;
    size_t i;
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
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @param stopChar Стоп-символ.
 * @return Подсмотренная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_peek_to
    (
        const TextNavigator *nav,
        am_byte stopChar
    )
{
    int c;
    size_t i, length;
    Span result;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    result.len = 0;
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
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @param stopChar Стоп-символ.
 * @return Подсмотренная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_peek_until
    (
        const TextNavigator *nav,
        am_byte stopChar
    )
{
    int c;
    size_t i, length;
    Span result;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    result.len = 0;
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
 * Происходит движение вперед по тексту.
 *
 * @param nav Навигатор.
 * @return Прочитанная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_read_line
    (
        TextNavigator *nav
    )
{
    am_byte c;
    size_t start;
    Span result;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    result.len = 0;
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
 * Считывание ИРБИСной "строки" (вплоть до символа перевода строки).
 * Символ перевода строки считывается, но в результат не помещается.
 * Происходит движение вперед по тексту.
 *
 * @param nav Навигатор.
 * @return Прочитанная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_read_irbis
    (
        TextNavigator *nav
    )
{
    am_byte c;
    size_t start;
    Span result;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    result.len = 0;
    start = nav->position;
    while (nav->position < nav->length) {
        c = nav->data[nav->position];
        if (c == '\x1F' || c == '\x1E') {
            break;
        }
        (void) nav_read (nav);
    }

    result.len = nav->position - start;

    /* Проглатываем перевод строки */
    if (nav->position < nav->length) {
        c = nav->data[nav->position];
        if (c == '\x1F') {
            (void) nav_read (nav);
            c = nav->data[nav->position];
        }

        if (c == '\x1E') {
            (void) nav_read (nav);
        }
    }

    return result;
}

/**
 * Текущий символ -- управляющий?
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @return Результат проверки.
 * Если достигнут конец текста, возвращается `AM_FALSE`.
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
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @return Результат проверки.
 * Если достигнут конец текста, возвращается `AM_FALSE`.
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
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @return Результат проверки.
 * Если достигнут конец текста, возвращается `AM_FALSE`.
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
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @return Результат проверки.
 * Если достигнут конец текста, возвращается `AM_FALSE`.
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
 * Считываются все символы до первого нецифрового.
 * Происходит движение вперед по тексту.
 *
 * @param nav Навигатор.
 * @return Прочитанная строка с числом (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_read_integer
    (
        TextNavigator *nav
    )
{
    Span result;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    result.len = 0;
    while (nav_is_digit (nav)) {
        ++result.len;
        (void) nav_read (nav);
    }

    return result;
}

/**
 * Извлечение целого числа без знака.
 * Сначала пропускаются все не-цифры,
 * затем считываются все цифры (если есть).
 * Происходит движение вперед по тексту.
 *
 * @param nav Навигатор.
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
 * Происходит движение вперед по тексту.
 *
 * @param nav Навигатор.
 * @param length Максимальная длина строки.
 * @return Прочитанная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_read_string
    (
        TextNavigator *nav,
        size_t length
    )
{
    int c;
    size_t i;
    Span result;

    assert (nav != NULL);

    result.ptr = (am_byte*) nav->data + nav->position;
    result.len = 0;
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
 * Происходит движение вперед по тексту.
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
 * Происходит движение вперед по тексту.
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
 * Чтение слова из текста.
 * Происходит двжиение вперед по тексту.
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
 * Получение непрочитанного остатка текста.
 * Движения по тексту не происходит.
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
 * Преобразование всего текста в навигаторе в фрагмент.
 * Движения по тексту не происходит.
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
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @param offset Смечение от начала данных.
 * @param length Предполагаемая длина фрагмента.
 * @return Полученный фрагмент (возможно, пустой).
 */
MAGNA_API Span MAGNA_CALL nav_slice
    (
        const TextNavigator *nav,
        size_t offset,
        size_t size
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
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @param length Предполагаемая длина фрагмента с последними символами
 * (положительное число).
 * @return Полученный фрагмент (возможно, пустой).
 */
MAGNA_API Span MAGNA_CALL nav_recent
    (
        const TextNavigator *nav,
        ssize_t length
    )
{
    ssize_t start;

    assert (nav != NULL);

    start = nav->position - length;
    if (start < 0) {
        length += start;
        start = 0;
    }

    if (((size_t)start) >= nav->length) {
        start = 0;
        length = 0;
    }

    if (length < 0) {
        length = 0;
    }

    return nav_slice (nav, (size_t) start, (size_t) length);
}

/**
 * Пропуск символов, не входящих в слово.
 * Происходит движение вперед по тексту.
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
 * Пропуск пробельных символов.
 * Происходит движение вперед по тексту.
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
 * Пропуск символов пунктуации.
 * Происходит движение вперед по тексту.
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
 * Происходит движение вперед по тексту.
 *
 * @param nav Навигатор.
 * @return Прочитанный символ либо `NAV_EOT`,
 * если достигнут конец текста.
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
