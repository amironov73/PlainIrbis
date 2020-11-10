// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>
#include <ctype.h>

/*=========================================================*/

/**
 * \file navigatr.c
 *
 * Навигация по произвольному тексту.
 *
 * \struct Navigator
 *      \brief Навигатор по произвольному тексту.
 *      \details Не владеет никакой памятью,
 *      не требует освобождения ресурсов.
 *
 * \var Navigator::data
 *      \brief Указатель на начало текста.
 *      \details Невладеющий указатель.
 *
 * \var Navigator::length
 *      \brief Длина данных в байтах.
 *
 * \var Navigator::position
 *      \brief Текущая позиция (в байтах).
 *
 */

/*=========================================================*/

/**
 * Инициализация навигатора.
 * Не выделяет память в куче.
 *
 * @param nav Указатель на неинициализированную структуру.
 * @param data Указатель на начало данных.
 * @param dataSize Длина данных в байтах.
 * @return Указатель на инициализированную структуру.
 */
MAGNA_API Navigator* MAGNA_CALL nav_init
    (
        Navigator* nav,
        const am_byte *data,
        size_t dataSize
    )
{
    assert (nav != NULL);
    assert (data != NULL);

    nav->data     = data;
    nav->length   = dataSize;
    nav->position = 0;

    return nav;
}

/**
 * Инициализация из фрагмента.
 * Не выделяет память в куче.
 *
 * @param nav Указатель на неинициализированную структуру.
 * @param span Фрагмент.
 * @return Указатель на инициализированную структуру.
 */
MAGNA_API Navigator* MAGNA_CALL nav_from_span
    (
        Navigator *nav,
        Span span
    )
{
    assert (nav != NULL);

    return nav_init (nav, span.start, span_length (span));
}

/**
 * Инициализация навигатора из буфера.
 * Навигатор воспримет данные от начала буфера до `position`.
 * Не выделяет память в куче.
 *
 * @param nav Указатель на неинициализированную структуру.
 * @param buffer Указатель на буфер.
 * @return Указатель на инициализированную структуру.
 */
MAGNA_API Navigator* MAGNA_CALL nav_from_buffer
    (
        Navigator *nav,
        const Buffer *buffer
    )
{
    assert (nav != NULL);
    assert (buffer != NULL);

    return nav_init (nav, buffer->start, buffer_length (buffer));
}

/**
 * Инициализация навигатора строкой.
 * Стоп-символ в навигатор не помещается.
 * Не выделяет память в куче.
 *
 * @param nav Указатель на неинициализированную структуру.
 * @param text Строка с текстом.
 * @return Указатель на инициализированную структуру.
 */
MAGNA_API Navigator* MAGNA_CALL nav_from_text
    (
        Navigator *nav,
        const am_byte *text
    )
{
    assert (nav != NULL);
    assert (text != NULL);

    return nav_init (nav, CBTEXT (text), (size_t) strlen (CCTEXT (text)));
}

/**
 * Определение текущей позиции в тексте.
 *
 * @param nav Навигатор.
 * @return Позиция в тексте: номер колонки и номер строки (у обоих нумерация с 1).
 */
MAGNA_API NavigatorPosition MAGNA_CALL nav_position
    (
        const Navigator *nav
    )
{
    am_byte *end; /* за концом фрагмента */
    am_byte *ptr; /* переменная цикла */
    NavigatorPosition result = { 1, 1 }; /* результат */

    assert (nav != NULL);

    end = (am_byte*) (nav->data + nav->position);
    for (ptr = (am_byte*)nav->data; ptr != end; ++ptr) {
        if (*ptr == '\n') {
            ++result.line;
            result.column = 1;
        }
        else {
            ++result.column;
        }
    }

    return result;
}

/**
 * Указатель прямо за последним байтом.
 *
 * @param nav Навигатор.
 * @return Указатель за последним байтом.
 */
MAGNA_API MAGNA_INLINE const am_byte* MAGNA_CALL nav_end
    (
        const Navigator *nav
    )
{
    assert (nav != NULL);

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
        const Navigator *nav
    )
{
    assert (nav != NULL);

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
        const Navigator *nav
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
        const Navigator *nav,
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
        const Navigator *nav
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
        const Navigator *nav
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
        const Navigator *nav,
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
        const Navigator *nav,
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
MAGNA_API Navigator* MAGNA_CALL nav_move
    (
        Navigator *nav,
        ssize_t distance
    )
{
    assert (nav != NULL);

    if (distance < 0) {
        if ((-distance) > ((ssize_t)nav->position)) {
            nav->position = 0;
            distance = 0;
        }
    }
    else {
        if ((nav->position + distance) > nav->length) {
            nav->position = nav->length;
            distance = 0;
        }
    }

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
        const Navigator * nav
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
        const Navigator *nav
    )
{
    size_t distance = 0; /* смещение от текущего символа */
    int result; /* результат */

    assert (nav != NULL);

    for (;;) {
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
 * @param nav Навигатор.
 * @return Прочитанный символ либо `NAV_EOT`,
 * если произошел выход за границы текста.
 */
MAGNA_API int MAGNA_CALL nav_read
    (
        Navigator *nav
    )
{
    int result; /* результат */

    assert (nav != NULL);

    result = nav_at (nav, nav->position);
    if (result >= 0) {
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
        Navigator *nav
    )
{
    int result; /* результат */

    assert (nav != NULL);

    for (;;) {
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
        const Navigator *nav,
        size_t length
    )
{
    int current;  /* текущий символ */
    size_t index; /* индекс в цикле */
    Span result;  /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav_current (nav);
    for (index = 0; index < length; ++index) {
        current = nav_look_ahead (nav, index);
        if ((current == NAV_EOT) || (current == '\r') || (current == '\n')) {
            break;
        }

        ++result.end;
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
        const Navigator *nav,
        am_byte stopChar
    )
{
    int current;    /* текущий символ */
    size_t index;   /* индекс в цикле */
    size_t length;  /* доступная часть фрагмента */
    Span result;    /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav->data + nav->position;
    length = nav->length - nav->position;
    for (index = 0; index < length; ++index) {
        current = nav_look_ahead (nav, index);
        ++result.end;
        if (current == stopChar) {
            break;
        }
    }

    return result;
}

/**
 * Подглядывание вплоть до указанного символа (включая его).
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @param stopChars Стоп-символы.
 * @return Подсмотренная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_peek_to_many
    (
        const Navigator *nav,
        const am_byte *stopChars
    )
{
    int current;    /* текущий символ */
    size_t index;   /* индекс в цикле */
    size_t length;  /* доступная часть фрагмента */
    Span result;    /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav->data + nav->position;
    length = nav->length - nav->position;
    for (index = 0; index < length; ++index) {
        current = nav_look_ahead (nav, index);
        ++result.end;
        if (str_contains (stopChars, current)) {
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
        const Navigator *nav,
        am_byte stopChar
    )
{
    int current;   /* текущий символ */
    size_t index;  /* индекс в цикле */
    size_t length; /* доступная часть фрагмента */
    Span result;   /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav->data + nav->position;
    length = nav->length - nav->position;
    for (index = 0; index < length; ++index) {
        current = nav_look_ahead (nav, index);
        if (current == stopChar) {
            break;
        }

        ++result.end;
    }

    return result;
}

/**
 * Подглядывание вплоть до указанного символа (не включая его).
 * Движения по тексту не происходит.
 *
 * @param nav Навигатор.
 * @param stopChars Стоп-символы.
 * @return Подсмотренная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_peek_until_many
    (
        const Navigator *nav,
        const am_byte *stopChars
    )
{
    int current;   /* текущий символ */
    size_t index;  /* индекс в цикле */
    size_t length; /* доступная часть фрагмента */
    Span result;   /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav->data + nav->position;
    length = nav->length - nav->position;
    for (index = 0; index < length; ++index) {
        current = nav_look_ahead (nav, index);
        if (str_contains (stopChars, current)) {
            break;
        }

        ++result.end;
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
        Navigator *nav
    )
{
    am_byte current; /* текущий символ */
    Span result;     /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav->data + nav->position;
    while (nav->position < nav->length) {
        current = nav->data[nav->position];
        if (current == '\r' || current == '\n') {
            break;
        }

        (void) nav_read (nav);
        ++result.end;
    }

    /* Проглатываем перевод строки */
    if (nav->position < nav->length) {
        current = nav->data[nav->position];
        if (current == '\r') {
            (void) nav_read (nav);
            current = nav->data[nav->position];
        }

        if (current == '\n') {
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
        Navigator *nav
    )
{
    am_byte current; /* текущий символ */
    Span result;     /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav->data + nav->position;
    while (nav->position < nav->length) {
        current = nav->data[nav->position];
        if (current == 0x1F || current == 0x1E) {
            break;
        }

        (void) nav_read (nav);
        ++result.end;
    }

    /* Проглатываем перевод строки */
    if (nav->position < nav->length) {
        current = nav->data[nav->position];
        if (current == 0x1F) {
            (void) nav_read (nav);
            current = nav->data[nav->position];
        }

        if (current == 0x1E) {
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
        const Navigator *nav
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
        const Navigator *nav
    )
{
    int c; /* прочитанный символ */

    assert (nav != NULL);

    c = nav_peek (nav);

    /* определяем сами, не полагаясь на ctype */

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
        const Navigator *nav
    )
{
    int c; /* прочитанный символ */

    assert (nav != NULL);

    c = nav_peek (nav);

    /* определяем сами, не полагаясь на ctype */

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
        const Navigator *nav
    )
{
    int c; /* прочитанный символ */

    assert (nav != NULL);

    c = nav_peek (nav);

    /* определяем сами, не полагаясь на ctype */

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
        Navigator *nav
    )
{
    Span result; /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav->data + nav->position;
    while (nav_is_digit (nav)) {
        ++result.end;
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
        Navigator *nav
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
        Navigator *nav,
        size_t length
    )
{
    int current;  /* текущий символ */
    size_t index; /* индекс цикла */
    Span result;  /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav->data + nav->position;
    for (index = 0; index < length; ++index) {
        current = nav_read (nav);
        if (current == NAV_EOT) {
            break;
        }

        ++result.end;
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
        Navigator *nav,
        am_byte stopChar
    )
{
    int current; /* текущий символ */
    Span result; /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav->data + nav->position;
    for (;;) {
        current = nav_read (nav);
        if ((current == NAV_EOT) || (current == stopChar)) {
            break;
        }

        ++result.end;
    }

    return result;
}

/**
 * Считывание вплоть до указанного символа (не включая его).
 * Сам стоп-символ считывается, но в результат не помещается.
 * Происходит движение вперед по тексту.
 *
 * @param nav Навигатор.
 * @param stopChars Стоп-символы.
 * @return Прочитанная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_read_to_many
    (
        Navigator *nav,
        const am_byte *stopChars
    )
{
    int current; /* текущий символ */
    Span result; /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav->data + nav->position;
    for (;;) {
        current = nav_read (nav);
        if ((current == NAV_EOT) || str_contains (stopChars, current)) {
            break;
        }

        ++result.end;
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
        Navigator *nav,
        am_byte stopChar
    )
{
    int current; /* текущий символ */
    Span result; /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav->data + nav->position;
    for (;;) {
        current = nav_peek (nav);
        if ((current == NAV_EOT) || (current == stopChar)) {
            break;
        }

        (void) nav_read (nav);
        ++result.end;
    }

    return result;
}

/**
 * Считывание вплоть до указанного символа (не включая его).
 * Сам стоп-символ остается не прочитанным.
 * Происходит движение вперед по тексту.
 *
 * @param nav Навигатор.
 * @param stopChars Стоп-символы.
 * @return Прочитанная строка (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL nav_read_until_many
    (
        Navigator *nav,
        const am_byte *stopChars
    )
{
    int current; /* текущий символ */
    Span result; /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav->data + nav->position;
    for (;;) {
        current = nav_peek (nav);
        if ((current == NAV_EOT) || str_contains (stopChars, current)) {
            break;
        }

        (void) nav_read (nav);
        ++result.end;
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
        Navigator *nav,
        am_byte goodChar
    )
{
    int current; /* текущий символ */
    Span result; /* результат */

    assert (nav != NULL);

    result.end = result.start = (am_byte*) nav->data + nav->position;
    for (;;) {
        current = nav_peek (nav);
        if ((current == NAV_EOT) || (current != goodChar)) {
            break;
        }

        (void) nav_read (nav);
        ++result.end;
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
        Navigator *nav
    )
{
    int current; /* текущий символ */
    Span result; /* результат */

    assert (nav != NULL);
    result.end = result.start = (am_byte*) nav->data + nav->position;
    for (;;) {
        current = nav_peek (nav);
        if ((current == NAV_EOT) || !isalnum (current)) {
            break;
        }

        (void) nav_read (nav);
        ++result.end;
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
        const Navigator *nav
    )
{
    Span result; /* результат */

    assert (nav != NULL);

    result.start = (am_byte*) nav->data + nav->position;
    result.end   = (am_byte*) nav->data + nav->length;

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
        const Navigator *nav
    )
{
    Span result; /* результат */

    assert (nav != NULL);

    result.start = (am_byte*) nav->data;
    result.end   = (am_byte*) nav->data + nav->length;

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
        const Navigator *nav,
        size_t offset,
        size_t size
    )
{
    Span result; /* результат */

    assert (nav != NULL);

    result.start = (am_byte*) nav->data + offset;
    result.end   = result.start + size;

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
        const Navigator *nav,
        ssize_t length
    )
{
    ssize_t start; /* смещение от начала, байты */

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
        Navigator *nav
    )
{
    int current; /* текущий символ */

    assert (nav != NULL);

    for (;;) {
        current = nav_peek (nav);
        if ((current == NAV_EOT) || isalnum (current)) {
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
        Navigator *nav
    )
{
    int current; /* текущий символ */

    assert (nav != NULL);

    for (;;) {
        current = nav_peek (nav);
        if ((current == NAV_EOT) || !isspace (current)) {
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
        Navigator *nav
    )
{
    int current; /* текущий символ */

    assert (nav != NULL);

    for (;;) {
        current = nav_peek (nav);
        if ((current == NAV_EOT) || !ispunct (current)) {
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
        Navigator *nav
    )
{
    unsigned int chr, chr2; /* текущие символы */

    assert (nav != NULL);

    if (nav_eot (nav)) {
        return -1;
    }

    chr = (unsigned int) nav_read (nav);
    if ((chr & 0x80u) == 0u)
    {
        // 1-Byte sequence: 000000000xxxxxxx = 0xxxxxxx
    }
    else if ((chr & 0xE0u) == 0xC0u)
    {
        // 2-Byte sequence: 00000yyyyyxxxxxx = 110yyyyy 10xxxxxx
        if (nav_eot (nav)) {
            return -1;
        }

        chr = (chr & 0x1Fu) << 6u;
        chr2 = (unsigned int) nav_read (nav);
        chr |= (chr2 & 0x3Fu);
    }
    else if ((chr & 0xF0u) == 0xE0u)
    {
        // 3-Byte sequence: zzzzyyyyyyxxxxxx = 1110zzzz 10yyyyyy 10xxxxxx
        if (nav_eot (nav)) {
            return -1;
        }

        chr = (chr & 0x0Fu) << 12u;
        chr2 = (unsigned int) nav_read (nav);
        chr |= (chr2 & 0x3Fu) << 6u;

        if (nav_eot (nav)) {
            return -1;
        }

        chr2 = (unsigned int) nav_read (nav);
        chr |= (chr2 & 0x3Fu);
    }
    else if ((chr & 0xF8u) == 0xF0u)
    {
        // 4-Byte sequence: 11101110wwwwzzzzyy + 110111yyyyxxxxxx = 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
        if (nav_eot (nav)) {
            return -1;
        }

        chr = (chr & 0x07u) << 18u;
        chr2 = (unsigned int) nav_read (nav);
        chr |= (chr2 & 0x3Fu) << 12u;

        if (nav_eot (nav)) {
            return -1;
        }

        chr2 = (unsigned int) nav_read (nav);
        chr |= (chr2 & 0x3Fu) << 6u;

        if (nav_eot (nav)) {
            return -1;
        }

        chr2 = (unsigned int) nav_read (nav);
        chr |= (chr2 & 0x3Fu);
    }

    return (int) chr;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
