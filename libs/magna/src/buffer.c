// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <ctype.h>
#include <stdarg.h>
#include <assert.h>

/*=========================================================*/

/**
 * \file buffer.c
 *
 * Простой динамический буфер для данных.
 * Замена `std::string`.
 *
 * Владеет своей памятью (кроме специальных случаев).
 * Для освобождения ресурсов используйте `buffer_destroy`.
 *
 * \struct Buffer
 *      \brief Простой динамический буфер.
 *
 * \var Buffer::start
 *      \brief Указатель на начало данных.
 *
 * \var Bufffer::current
 *      \brief Указатель, куда будет помещен следующий записываемый байт.
 *
 * \var Buffer::end
 *      \brief Указатель за последним доступным байтом.
 */

/*=========================================================*/

/**
 * Инициализация буфера нулями.
 * Не выделяет память в куче.
 *
 * @param buffer Указатель на неинициализированный буфер.
 * @return Инициализированный пустой буфер.
 */
MAGNA_API void MAGNA_CALL buffer_init
    (
        Buffer *buffer
    )
{
    assert (buffer != NULL);

    buffer->start   = NULL;
    buffer->current = NULL;
    buffer->end     = NULL;
}

/**
 * Создаёт статический буфер, не предназначенный для роста.
 * Не выделяет память в куче.
 *
 * @param buffer Указатель на неиницализированную структуру.
 * @param data Данные.
 * @param length Длина данных в байтах.
 * @return Указатель на иницализированный буфер.
 * @warning Нельзя `buffer_destroy`!
 * Вообще, лучше не пытаться писать в статический буфер!
 */
MAGNA_API void MAGNA_CALL buffer_static
    (
        Buffer *buffer,
        const am_byte *data,
        size_t length
    )
{
    assert (buffer != NULL);

    buffer->start = (am_byte*) data;
    buffer->current = buffer->end = ((am_byte*) data) + length;
}

/**
 * Степень заполненности буфера -- количество записанных данных в байтах.
 *
 * @param buffer Инициализированный буфер.
 * @return Длина данных в байтах.
 */
MAGNA_API MAGNA_INLINE size_t MAGNA_CALL buffer_position
    (
        const Buffer *buffer
    )
{
    assert (buffer != NULL);

    return (size_t) (buffer->current - buffer->start);
}

/**
 * Алиас для `buffer_position`.
 *
 * @param buffer Инициализированный буфер.
 * @return Длина буфера в байтах.
 */
MAGNA_API MAGNA_INLINE size_t MAGNA_CALL buffer_length
    (
        const Buffer *buffer
    )
{
    assert (buffer != NULL);

    return (size_t) (buffer->current - buffer->start);
}

/**
 * Общая емкость буфера в байтах.
 *
 * @param buffer Инициализированный буфер.
 * @return Емкость буфера в байтах.
 */
MAGNA_API MAGNA_INLINE size_t MAGNA_CALL buffer_capacity
    (
        const Buffer *buffer
    )
{
    assert (buffer != NULL);

    return (size_t) (buffer->end - buffer->start);
}

/**
 * Алиас для `buffer_capacity`.
 *
 * @param buffer Инициализированный буфер.
 * @return Емкость буфера в байтах.
 */
MAGNA_API MAGNA_INLINE size_t MAGNA_CALL buffer_size
    (
        const Buffer *buffer
    )
{
    assert (buffer != NULL);

    return (size_t) (buffer->end - buffer->start);
}

/**
 * Вычисление наиболее выгодного размера буфера.
 *
 * @param newSize Предполагаемый размер.
 * @return Вычисленный размер.
 */
MAGNA_API size_t MAGNA_CALL buffer_calculate_size
    (
        size_t newSize
    )
{
    size_t result = 8;

    if (newSize < 8) {
        newSize = 8;
    }

    while (result < newSize) {
        result *= 2;
    }

    return result;
}

/**
 * Создаёт буфер, предназначенный для динамического роста, копируя
 * уже имеющиеся данные в кучу.
 *
 * @param buffer Указатель на неиницализированный буфер.
 * @param data Данные для инициализации (может быть `NULL`).
 * @param length Длина данных в байтах.
 */
MAGNA_API am_bool MAGNA_CALL buffer_create
    (
        Buffer *buffer,
        const am_byte *data,
        size_t length
    )
{
    assert (buffer != NULL);

    buffer->start   = NULL;
    buffer->current = NULL;
    buffer->end     = NULL;

    if (length != 0) {
        if (!buffer_grow (buffer, length)) {
            return AM_FALSE;
        }

        if (data != NULL) {
            mem_copy (buffer->start, data, length);
            buffer->current = buffer->start + length;
        }
    }

    return AM_TRUE;
}

/**
 * Освобождает память, занятую буфером.
 * Статические буферы освобождать нельзя!
 *
 * @param buffer Буфер, подлежащий освобождению.
 */
MAGNA_API void MAGNA_CALL buffer_destroy
    (
        Buffer *buffer
    )
{
    assert (buffer != NULL);

    if (buffer->start) {
        mem_free (buffer->start);
        buffer->start = NULL;
    }

    buffer->current = NULL;
    buffer->end = NULL;
}

/**
 * При необходимости увеличивает размер буфера.
 * Данные в буфере (если есть) остаются неизменными,
 * но могут поменять свой адрес.
 *
 * @param buffer Буфер.
 * @param newSize Предполагаемый размер буфера.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_grow
    (
        Buffer *buffer,
        size_t newSize
    )
{
    size_t capacity, length;
    am_byte *newStart;

    assert (buffer != NULL);

    capacity = buffer_capacity (buffer);
    if (newSize > capacity) {
        length = buffer_length (buffer);
        newSize = buffer_calculate_size (newSize);
        newStart = mem_realloc (buffer->start, newSize);
        if (newStart == NULL) {
            return AM_FALSE;
        }

        buffer->start   = newStart;
        buffer->current = newStart + length;
        buffer->end     = newStart + newSize;
    }

    return AM_TRUE;
}

MAGNA_API am_bool MAGNA_CALL buffer_fit
    (
        Buffer *buffer,
        const am_byte *high
    )
{
    size_t newSize, length;
    am_byte *newStart;

    assert (buffer != NULL);

    if (high >= buffer->end) {
        length = buffer_length (buffer);
        newSize = buffer_calculate_size (high - buffer->start);
        newStart = mem_realloc (buffer->start, newSize);
        if (newStart == NULL) {
            return AM_FALSE;
        }

        buffer->start   = newStart;
        buffer->current = newStart + length;
        buffer->end     = newStart + newSize;
    }

    return AM_TRUE;
}

/**
 * Клонирование буфера. Происходит полное копирование памяти исходного буфера.
 *
 * @param target Неинициализированный буфер назначения.
 * @param source Буфер, подлежащий клонированию.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_clone
    (
        Buffer* target,
        const Buffer *source
    )
{
    size_t capacity, length;

    assert (target != NULL);
    assert (source != NULL);

    length = buffer_length (source);
    capacity = buffer_capacity (source);
    if (buffer_is_empty (source)) {
        target->start   = NULL;
        target->current = NULL;
        target->end     = NULL;
    }
    else {
        target->start = mem_alloc (capacity);
        if (target->start == NULL) {
            return AM_FALSE;
        }

        mem_copy (target->start, source->start, length);
        target->current = target->start + length;
        target->end = target->start + capacity;
    }

    return AM_TRUE;
}

/**
 * Копирует один буфер в другой. Происходит полное копирование памяти
 * исходного буфера.
 *
 * @param target Инициализированный буфер назначения.
 * @param source Исходный буфер.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_copy
    (
        Buffer *target,
        const Buffer *source
    )
{
    size_t length;

    assert (target != NULL);
    assert (source != NULL);

    length = buffer_length (source);
    if (!buffer_grow (target, length)) {
        return AM_FALSE;
    }

    buffer_clear (target);

    if (length != 0) {
        mem_copy (target->start, source->start, length);
    }

    target->current = target->start + length;

    return AM_TRUE;
}

/**
 * Дописывает один буфер в конец другого.
 *
 * @param target Буфер назначения.
 * @param source Буфер-источник.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_concat
    (
        Buffer *target,
        const Buffer *source
    )
{
    size_t targetLength, sourceLength;

    assert (target != NULL);
    assert (source != NULL);

    targetLength = buffer_length (target);
    sourceLength = buffer_length (source);
    if (!buffer_grow (target, targetLength + sourceLength)) {
        return AM_FALSE;
    }

    if (sourceLength != 0) {
        mem_copy
            (
                target->current,
                source->start,
                sourceLength
            );
        target->current += sourceLength;
    }

    return AM_TRUE;
}

/**
 * Дописывает один буфер в конец другого
 * с опциональными префиксом и суффиксом.
 *
 * @param output Буфер назначения.
 * @param prefix Опциональный префикс.
 * @param source Буфер-источник.
 * @param suffix Опциональный суффикс.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_combine
    (
        Buffer *output,
        const am_byte *prefix,
        const Buffer *source,
        const am_byte *suffix
    )
{
    assert (output != NULL);
    assert (source != NULL);

    if (prefix != NULL) {
        if (!buffer_puts (output, prefix)) {
            return AM_FALSE;
        }
    }

    if (!buffer_concat (output, source)) {
        return AM_FALSE;
    }

    if (suffix != NULL) {
        if (!buffer_puts (output, suffix)) {
            return  AM_FALSE;
        }
    }

    return AM_TRUE;
}

/**
 * Дописывает байт в конец буфера.
 *
 * @param buffer Инициализированный буфер.
 * @param value Байт для записи.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_putc
    (
        Buffer *buffer,
        am_byte value
    )
{
    assert (buffer != NULL);

    if (!buffer_fit (buffer, buffer->current + 1)) {
        return AM_FALSE;
    }

    *buffer->current++ = value;

    return AM_TRUE;
}

/**
 * Дописывает строку в конец буфера.
 *
 * @param buffer Инициализированный буфер.
 * @param str ASCIIZ-строка для записи.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_puts
    (
        Buffer *buffer,
        const am_byte *str
    )
{
    size_t delta;

    assert (buffer != NULL);
    assert (str != NULL);

    delta = strlen (CCTEXT (str));
    if (!buffer_fit (buffer, buffer->current + delta)) {
        return AM_FALSE;
    }

    mem_copy (buffer->current, str, delta);
    buffer->current += delta;

    return AM_TRUE;
}

/**
 * Дописывает данные в конец буфера.
 *
 * @param buffer Инициализированный буфер.
 * @param data Указатель на данные.
 * @param length Длина данных в байтах.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_write
    (
        Buffer *buffer,
        const am_byte *data,
        size_t length
    )
{
    assert (buffer != NULL);

    if (data != NULL && length > 0) {
        if (!buffer_fit (buffer, buffer->current + length)) {
            return AM_FALSE;
        }

        mem_copy (buffer->current, data, length);
        buffer->current += length;
    }

    return AM_TRUE;
}

/**
 * Дописывает данные в конец буфера.
 *
 * @param buffer Инициализированный буфер.
 * @param span Спан с данными.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_write_span
    (
        Buffer *buffer,
        Span span
    )
{
    assert (buffer != NULL);

    return buffer_write (buffer, span.start, span_length (span));
}

/**
 * Присваивает буферу новые данные, полностью заменяя старые.
 *
 * @param buffer Инициализированный буфер.
 * @param data Указатель на данные.
 * @param length Длина данных в байтах.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_assign
    (
        Buffer *buffer,
        const am_byte *data,
        size_t length
    )
{
    assert (buffer != NULL);

    if (data == NULL || length == 0) {
        buffer->current = buffer->start;
    }
    else {
        if (!buffer_grow (buffer, length)) {
            return AM_FALSE;
        }

        mem_copy (buffer->start, data, length);
        buffer->current = buffer->start + length;
    }

    return AM_TRUE;
}

MAGNA_API am_bool MAGNA_CALL buffer_assign_span
    (
        Buffer *buffer,
        Span span
    )
{
    assert (buffer != NULL);

    return buffer_assign (buffer, span.start, span_length (span));
}

/**
 * Присвоение буферу текстовой строки (вместе с завершающим 0).
 * Размещает копию строки в куче.
 *
 * @param buffer Инициализированный буфер.
 * @param text Текстовая строка.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_assign_text
    (
        Buffer *buffer,
        const am_byte *text
    )
{
    size_t len;

    assert (buffer != NULL);

    if (text == NULL) {
        if (!buffer_grow (buffer, 1)) {
            return AM_FALSE;
        }

        *buffer->start = 0;
        buffer->current = buffer->start;
    }
    else {
        len = strlen (CCTEXT (text)) + 1;
        if (!buffer_grow (buffer, len)) {
            return AM_FALSE;
        }

        /* Копируем вместе с завершающим нулем. */
        mem_copy (buffer->start, text, len);
        buffer->current = buffer->start + len - 1;
    }

    return AM_TRUE;
}

/**
 * Инициализирует буфер строкой.
 * Размещает копию строки в куче.
 *
 * @param buffer Буфер, подлежащий инициализации.
 * @param text Текст. Может быть NULL.
 * @return buffer.
 */
MAGNA_API am_bool MAGNA_CALL buffer_from_text
    (
        Buffer *buffer,
        const am_byte *text
    )
{
    size_t len;

    assert (buffer != NULL);

    buffer_init (buffer);
    if (text == NULL) {
        if (!buffer_grow (buffer, 1)) {
            return AM_FALSE;
        }

        *buffer->start = 0;
        buffer->current = buffer->start;
    }
    else {
        len = strlen (CCTEXT (text)) + 1;
        if (!buffer_grow (buffer, len)) {
            return AM_FALSE;
        }

        /* Копируем вместе с завершающим нулем. */
        mem_copy (buffer->start, text, len);
        buffer->current = buffer->start +  len - 1;
    }

    return AM_TRUE;
}

/**
 * Преобразование буфера в фрагмент.
 *
 * @param buffer Буфер.
 * @return Фрагмент.
 */
MAGNA_API MAGNA_INLINE Span MAGNA_CALL buffer_to_span
    (
        const Buffer *buffer
    )
{
    Span result;

    assert (buffer != NULL);

    result.start = buffer->start;
    result.end = buffer->current;

    return result;
}

/**
 * Преобразование фрагмента в буфер.
 * Выделяет память в куче.
 *
 * @param buffer Неинициализированный буфер.
 * @param span Спан с данными (будут скопированы в кучу).
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_from_span
    (
        Buffer *buffer,
        Span span
    )
{
    assert (buffer != NULL);

    buffer_init (buffer);

    return buffer_assign (buffer, span.start, span_length (span));
}

static am_byte* find_text
    (
        const am_byte *begin,
        const am_byte *end,
        const am_byte *what
    )
{
    const am_byte *ptr1, *ptr2, *ptr3;
    am_bool found;

    for (ptr1 = begin; ptr1 < end; ++ptr1) {
        found = AM_TRUE;
        for (ptr2 = ptr1, ptr3 = what; *ptr3; ++ptr2, ++ptr3) {
            if (*ptr2 != *ptr3) {
                found = AM_FALSE;
                break;
            }
        }
        if (found) {
            return (am_byte*) ptr1;
        }
    }

    return NULL;
}

/**
 * Замена текста в буфере.
 *
 * @param buffer Буфер.
 * @param from Заменяемый текст.
 * @param to Заменяющий текст.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_replace_text
    (
        Buffer *buffer,
        const am_byte *from,
        const am_byte *to
    )
{
    am_byte *ptr1, *ptr2;
    size_t lenFrom, lenTo, count, offset;

    assert (buffer != NULL);
    assert (from != NULL);
    assert (to != NULL);

    if (buffer->current == buffer->start) {
        /* В пустом буфере нам нечего делать */
        return AM_TRUE;
    }

    lenFrom = strlen (CCTEXT (from));
    lenTo = strlen (CCTEXT (to));
    for (ptr1 = buffer->start; ptr1 < buffer->current; ) {
        ptr2 = find_text (ptr1, buffer->end, from);
        if (ptr2 != NULL) {
            /* нашли заменяемую строку */
            if (lenFrom < lenTo) {
                /* заменяющая строка длинее */
                offset = ptr2 - buffer->start;
                if (!buffer_grow (buffer, buffer_length (buffer) + lenTo - lenFrom)) {
                    return AM_FALSE;
                }

                /* Из-за переаллокации буфера указатель мог измениться */
                ptr2 = buffer->start + offset;
                count = buffer->current - ptr2 - lenFrom;
                if (count > 0) {
                    memmove (ptr2 + lenTo, ptr2 + lenFrom, count);
                }
            }
            else if (lenFrom > lenTo) {
                count = buffer->current - ptr2 - lenTo;
                if (count > 0) {
                    memmove (ptr2 + lenTo, ptr2 + lenFrom, count);
                }
            }

            /* копируем заменяющую строку */
            memcpy (ptr2, to, lenTo);
            buffer->current += (lenTo - lenFrom);
            ptr1 = ptr2 + lenTo;
        }
        else {
            /* заменяемая строка не найдена */
            break;
        }
    }

    return AM_TRUE;
}

/**
 * Побайтовое сравнение двух буферов.
 *
 * @param first Первый буфер.
 * @param second Второй буфер.
 * @return Результат сравнения: <0, если первый буфер меньше,
 * >0, если первый буфер больше, =0, если содержимое буферов совпадает.
 */
MAGNA_API int MAGNA_CALL buffer_compare
    (
        const Buffer *first,
        const Buffer *second
    )
{
    size_t i, firstLength, secondLength;
    int result;

    assert (first != NULL);
    assert (second != NULL);

    firstLength = buffer_length (first);
    secondLength = buffer_length (second);
    for (i = 0; ; ++i) {
        if (i == firstLength) {
            if (i == secondLength) {
                return 0;
            }

            return -1;
        }
        else if (i == secondLength) {
                return 1;
            }
        else {
            result = first->start[i] - second->start[i];
            if (result != 0) {
                return result;
            }
        }
    }
}

/**
 * Побайтовое сравнение буфера и спана.
 *
 * @param buffer Первый буфер.
 * @param span Спан.
 * @return Результат сравнения: <0, если буфер меньше,
 * >0, если буфер больше, =0, если содержимое буфера и спана совпадает.
 */
MAGNA_API int MAGNA_CALL buffer_compare_span
    (
        const Buffer *buffer,
        Span span
    )
{
    size_t i, spanLength = span_length (span), bufferLength = buffer_length (buffer);
    int result;

    assert (buffer != NULL);

    for (i = 0; ; ++i) {
        if (i == bufferLength) {
            if (i == spanLength) {
                return 0;
            }

            return -1;
        }
        else if (i == spanLength) {
                return 1;
            }
        else {
            result = buffer->start[i] - span.start[i];
            if (result != 0) {
                return result;
            }
        }
    }
}

/**
 * Побайтовое сравнение буфера и спана без учета регистра символов.
 *
 * @param buffer Первый буфер.
 * @param span Спан.
 * @return Результат сравнения: <0, если буфер меньше,
 * >0, если буфер больше, =0, если содержимое буфера и спана совпадает.
 */
MAGNA_API int MAGNA_CALL buffer_compare_span_ignore_case
    (
        const Buffer *buffer,
        Span span
    )
{
    size_t i, spanLength = span_length (span), bufferLength = buffer_length (buffer);
    int result;

    assert (buffer != NULL);

    for (i = 0; ; ++i) {
        if (i == bufferLength) {
            if (i == spanLength) {
                return 0;
            }

            return -1;
        }
        else if (i == spanLength) {
                return 1;
            }
        else {
            result = toupper (buffer->start[i]) - toupper (span.start[i]);
            if (result != 0) {
                return result;
            }
        }
    }
}

/**
 * Побайтовое сравнение буфера со строкой.
 *
 * @param buffer Буфер.
 * @param text Строка для сравнения (не может быть `NULL`).
 * @return Результат сравнения: <0, если буфер меньше,
 * >0, если буфер больше, и =0, если буфер совпадает со строкой.
 */
MAGNA_API int MAGNA_CALL buffer_compare_text
    (
        const Buffer *buffer,
        const am_byte *text
    )
{
    const am_byte *ptr = text;
    size_t i, length;
    int result;

    assert (buffer != NULL);
    assert (text != NULL);

    length = buffer_length (buffer);
    for (i = 0; ; ++i, ++ptr) {
        if (i == length) {
            if (!*ptr) {
                return 0;
            }

            return -1;
        }
        else if (!*ptr) {
            return 1;
        }
        else {
            result = buffer->start[i] - *ptr;
            if (result != 0) {
                return result;
            }
        }
    }
}

/**
 * Побайтовое сравнение буфера со строкой без учета регистра.
 *
 * @param buffer Буфер.
 * @param text Строка для сравнения (не может быть `NULL`).
 * @return Результат сравнения: <0, если буфер меньше,
 * >0, если буфер больше, и =0, если буфер совпадает со строкой.
 */
MAGNA_API int MAGNA_CALL buffer_compare_text_ignore_case
    (
        const Buffer *buffer,
        const am_byte *text
    )
{
    const am_byte *ptr = text;
    size_t i, length;
    int result;

    assert (buffer != NULL);
    assert (text != NULL);

    length = buffer_length (buffer);
    for (i = 0; ; ++i, ++ptr) {
        if (i == length) {
            if (!*ptr) {
                return 0;
            }

            return -1;
        }
        else if (!*ptr) {
            return 1;
        }
        else {
            result = toupper (buffer->start[i]) - toupper (*ptr);
            if (result != 0) {
                return result;
            }
        }
    }
}

/**
 * Поиск первого вхождения строки в буфере.
 *
 * @param buffer
 * @param text
 * @return
 */
MAGNA_API am_byte* MAGNA_CALL buffer_find_text
    (
        const Buffer *buffer,
        const am_byte *text
    )
{
    assert (buffer != NULL);
    assert (text != NULL);

    return find_text
        (
            buffer->start,
            buffer->current,
            text
        );
}

/**
 * Преобразование данных из кодировки UTF-8 в ANSI.
 *
 * @param target Инициализированный буфер назначения.
 * @param source Буфер с исходными данными.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_utf8_to_ansi
    (
        Buffer *target,
        const Buffer *source
    )
{
    /* size_t delta; */

    assert (target != NULL);
    assert (source != NULL);

    /* delta = utf8_code_points (source->ptr, source->position);
    if (!buffer_grow (target, delta)) {
        return AM_FALSE;
    } */

    return utf2ansi (target, buffer_to_span (source));
}

/**
 * Преобразование данных из кодировки ANSI  в UTF-8.
 *
 * @param target Инициализированный буфер назначения.
 * @param source Буфер с исходными данными.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_ansi_to_utf8
    (
        Buffer *target,
        const Buffer *source
    )
{
    /* size_t delta; */

    assert (target != NULL);
    assert (source != NULL);

    /* delta = utf8_code_points (source->ptr, source->position);
    if (!buffer_grow (target, delta)) {
        return AM_FALSE;
    } */

    return ansi2utf (target, buffer_to_span (source));
}

/**
 * Представление буфера в виде строки. Если необходимо,
 * сразу после данных  * добавляется нулевой байт.
 *
 * @param buffer Буфер.
 * @return Указатель на строку либо `NULL`.
 */
MAGNA_API const am_byte* MAGNA_CALL buffer_to_text
    (
        Buffer *buffer
    )
{
    assert (buffer != NULL);

    if (buffer->current == buffer->end) {
        if (!buffer_fit (buffer, buffer->current + 1)) {
            return NULL;
        }
    }

    *buffer->current = 0;

    return buffer->start;
}

/**
 * Обмен содержимым между двумя буферами.
 *
 * @param first Первый буфер.
 * @param second Второй буфер.
 * @return Первый буфер.
 */
MAGNA_API Buffer* MAGNA_CALL buffer_swap
    (
        Buffer *first,
        Buffer *second
    )
{
    am_byte *temp;

    assert (first != NULL);
    assert (second != NULL);

    temp = first->start;
    first->start = second->start;
    second->start = temp;

    temp = first->current;
    first->current = second->current;
    second->current = temp;

    temp = first->end;
    first->end = second->end;
    second->end = temp;

    return first;
}

/**
 * Добавление перевода на новую строку.
 *
 * @param buffer Буфер.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_new_line
    (
        Buffer *buffer
    )
{
    assert (buffer != NULL);

    return buffer_putc (buffer, '\n');
}

/**
 * Добавление в буфер целого числа без знака.
 *
 * @param buffer Буфер.
 * @param value Целое число.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_put_uint32
    (
        Buffer *buffer,
        am_uint32 value
    )
{
    am_byte temp [16];

    assert (buffer != NULL);

    /* TODO: implement properly */
    sprintf ((char*) temp, "%lu", (unsigned long) value);

    return buffer_puts (buffer, temp);
}

/**
 * Добавление в буфер целого числа без знака.
 *
 * @param buffer Буфер.
 * @param value Целое число.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_put_uint64
    (
        Buffer *buffer,
        am_uint64 value
    )
{
    am_byte temp [64];

    assert (buffer != NULL);

    /* TODO: implement properly */
    sprintf ((char*) temp, "%llu", value);

    return buffer_puts (buffer, temp);
}

/**
 * Очистка буфера (текущая позиция устанавливается в 0).
 *
 * @param buffer Буфер.
 * @return Буфер.
 */
MAGNA_API Buffer* MAGNA_CALL buffer_clear
    (
        Buffer *buffer
    )
{
    assert (buffer != NULL);

    buffer->current = buffer->start;

    return buffer;
}

/**
 * Проверка, не пуст ли буфер?
 *
 * @param buffer Проверяемый буфер.
 * @return Резульат проверки.
 */
MAGNA_API am_bool MAGNA_CALL buffer_is_empty
    (
        const Buffer *buffer
    )
{
    assert (buffer != NULL);

    return (buffer->current == buffer->start) || (buffer->start == NULL);
}

/**
 * Аналог printf для буфера.
 *
 * @param buffer Буфер для вывода.
 * @param format Строка формата.
 * @param ... Прочие аргументы.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool buffer_format
    (
        Buffer *buffer,
        Span format,
        ...
    )
{
    am_bool result;
    va_list args;

    assert (buffer != NULL);

    va_start (args, format);
    result = format_generic (buffer, (OutputFunction) buffer_write, format, args);
    va_end (args);

    return result;
}

/**
 * Вывод содержимого буфера в файл.
 *
 * @param buffer Буфер.
 * @param handle Дескриптор файла.
 */
MAGNA_API void MAGNA_CALL buffer_to_file
    (
        const Buffer *buffer,
        am_handle handle
    )
{
    assert (buffer != NULL);

    if (!buffer_is_empty (buffer)) {
        file_write (handle, buffer->start, buffer_length (buffer));
    }
}

/**
 * Вывод содержимого буфера в консоль.
 *
 * @param buffer Буфер.
 */
MAGNA_API void MAGNA_CALL buffer_to_console
    (
        const Buffer *buffer
    )
{
    assert (buffer != NULL);

    if (!buffer_is_empty (buffer)) {
        fwrite (buffer->start, 1, buffer_length (buffer), stdout);
    }
}

/**
 * Удаление из буфера указанных байтов.
 *
 * @param buffer Буфер.
 * @param index Индекс первого удаляемого байта.
 * @param size Количество удаляемых байт.
 */
MAGNA_API void MAGNA_CALL buffer_remove_at
    (
        Buffer *buffer,
        size_t index,
        size_t size
    )
{
    assert (buffer != NULL);

    memmove
        (
            buffer->start + index,
            buffer->start + index + size,
            buffer_length (buffer) - index - size
        );
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
