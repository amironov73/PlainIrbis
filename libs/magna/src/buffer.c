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

/*=========================================================*/

/**
 * \file buffer.c
 *
 * Простой динамический буфер для данных.
 *
 */

/*=========================================================*/

MAGNA_API am_size_t MAGNA_CALL buffer_calculate_size
    (
            am_size_t newSize
    )
{
    if (newSize < 8) {
        newSize = 8;
    }

    return newSize * 2;
}

/**
 * Клонирование буфера. Происходит полное копирование памяти исходного буфера.
 *
 * @param target Неинициализированный буфер назначения.
 * @param source Буфер, подлежащий клонированию.
 * @return
 */
MAGNA_API Buffer* MAGNA_CALL buffer_clone
    (
        Buffer* target,
        const Buffer *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    target->position = source->position;
    target->capacity = source->capacity;
    target->ptr = malloc (source->capacity);
    if (target->ptr == NULL) {
        return NULL;
    }

    memcpy (target->ptr, source->ptr, source->position);

    return target;
}

/**
 * Копирует один буфер в другой. Происходит полное копирование памяти
 * исходного буфера.
 *
 * @param target Инициализированный буфер назначения.
 * @param source Исходный буфер.
 */
MAGNA_API am_bool MAGNA_CALL buffer_copy
    (
        Buffer *target,
        Buffer *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    if (!buffer_grow (target, source->position)) {
        return AM_FALSE;
    }

    target->position = 0;

    if (source->position != 0) {
        memcpy(target->ptr, source->ptr, source->position);
    }

    target->position = source->position;

    return AM_TRUE;
}

/**
 * Дописывает один буфер в конец другого.
 *
 * @param target
 * @param source
 */
MAGNA_API am_bool MAGNA_CALL buffer_concat
    (
        Buffer *target,
        Buffer *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    if (!buffer_grow (target, target->position + source->position)) {
        return AM_FALSE;
    }

    memcpy (target-> ptr + target->position, source->ptr, source->position);
    target->position += source->position;

    return AM_TRUE;
}

/**
 * Создаёт буфер, предназначенный для динамического роста, копируя
 * уже имеющиеся данные в кучк.
 *
 * @param buffer
 * @param data
 * @param length
 */
MAGNA_API am_bool MAGNA_CALL buffer_create
    (
            Buffer *buffer,
            am_byte *data,
            am_size_t length
    )
{
    assert (buffer != NULL);

    buffer->ptr = NULL;
    buffer->position = 0;
    buffer->capacity = 0;

    if (length != 0) {
        if (!buffer_grow (buffer, length)) {
            return AM_FALSE;
        }

        if (data != NULL) {
            memcpy (buffer->ptr, data, length);
            buffer->position = length;
        }
    }

    return AM_TRUE;
}

/**
 * Создаёт статический буфер, не предназначенный для роста.
 *
 * @param buffer
 * @param data
 * @param newSize
 */
MAGNA_API Buffer* MAGNA_CALL buffer_static
    (
            Buffer *buffer,
            am_byte *data,
            am_size_t newSize
    )
{
    assert (buffer != NULL);

    buffer->ptr = data;
    buffer->position = 0;
    buffer->capacity = newSize;

    return buffer;
}

/**
 * Освобождает буфер. Статические буферы освобождать нельзя!
 *
 * @param buffer
 */
MAGNA_API void MAGNA_CALL buffer_free
    (
        Buffer *buffer
    )
{
    assert (buffer != NULL);

    if (buffer->ptr) {
        free (buffer->ptr);
        buffer->ptr = NULL;
    }
    buffer->position = 0;
    buffer->capacity = 0;
}

/**
 * При необходимости увеличивает размер буфера.
 *
 * @param buffer
 * @param newSize
 */
MAGNA_API am_bool MAGNA_CALL buffer_grow
    (
            Buffer *buffer,
            am_size_t newSize
    )
{
    am_byte *newPtr;

    assert (buffer != NULL);

    if (newSize > buffer->capacity) {
        newSize = buffer_calculate_size (newSize);
        newPtr = malloc (newSize);
        if (!newPtr) {
            return AM_FALSE;
        }

        if (buffer->ptr != NULL) {
            memcpy(newPtr, buffer->ptr, buffer->position);
            free (buffer->ptr);
        }
        buffer->ptr = newPtr;
        buffer->capacity = newSize;
    }

    return AM_TRUE;
}

/**
 * Дописывает байт в конец буфера.
 * @param buffer
 * @param c
 */
MAGNA_API am_bool MAGNA_CALL buffer_putc
    (
        Buffer *buffer,
        am_byte c
    )
{
    assert (buffer != NULL);

    if (!buffer_grow (buffer, buffer->position + 1)) {
        return AM_FALSE;
    }
    buffer->ptr [buffer->position++] = c;

    return AM_TRUE;
}

/**
 * Пишет строку в буфер.
 *
 * @param buffer
 * @param str
 */
MAGNA_API am_bool MAGNA_CALL buffer_puts
    (
        Buffer *buffer,
        const char *str
    )
{
    assert (buffer != NULL);
    assert (str != NULL);

    size_t delta = strlen (str);
    if (!buffer_grow (buffer, buffer->position + delta)) {
        return AM_FALSE;
    }

    memcpy (buffer->ptr + buffer->position, str, delta);
    buffer->position += delta;

    return AM_TRUE;
}

/**
 * Пишет данные в буфер.
 *
 * @param buffer
 * @param data
 * @param length
 */
MAGNA_API void MAGNA_CALL buffer_write
    (
        Buffer *buffer,
        const am_byte *data,
        am_size_t length
    )
{
    assert (buffer != NULL);

    if (data != NULL && length > 0) {
        buffer_grow (buffer, buffer->position + length);
        memcpy (buffer->ptr + buffer->position, data, length);
        buffer->position += length;
    }
}

/**
 * Присваивает буферу новые данные, полностью заменяя старые.
 *
 * @param buffer
 * @param data
 * @param length
 */
MAGNA_API Buffer* MAGNA_CALL buffer_assign
    (
        Buffer *buffer,
        const am_byte *data,
        am_size_t length
    )
{
    assert (buffer != NULL);

    if (data == NULL || length == 0) {
        buffer->position = 0;
    }
    else {
        if (!buffer_grow (buffer, length)) {
            return NULL;
        }

        memcpy (buffer->ptr, data, length);
        buffer->position = length;
    }

    return buffer;
}

/**
 * Присвоение буферу текстовой строки (вместе с завершающим 0).
 *
 * @param buffer
 * @param text
 * @return
 */
MAGNA_API Buffer* MAGNA_CALL buffer_assign_text
    (
        Buffer *buffer,
        const char *text
    )
{
    am_size_t len;

    assert (buffer != NULL);

    if (text == NULL) {
        buffer_grow (buffer, 1);
        buffer->ptr[0] = '\0';
        buffer->position = 0;
    }
    else {
        len = strlen (text) + 1;
        buffer_grow (buffer, len);
        memcpy (buffer->ptr, text, len);
        buffer->position = len - 1;
    }

    return buffer;
}

/**
 * Инициализирует буфер строкой.
 *
 * @param buffer Буфер, подлежащий инициализации.
 * @param text Текст. Может быть NULL.
 * @return buffer.
 */
MAGNA_API Buffer* MAGNA_CALL buffer_from_text
    (
        Buffer *buffer,
        const char *text
    )
{
    am_size_t len;

    assert (buffer != NULL);

    if (text == NULL) {
        buffer_grow (buffer, 1);
        buffer->ptr[0] = '\0';
        buffer->position = 0;
    }
    else {
        len = strlen (text) + 1;
        buffer_grow (buffer, len);
        memcpy (buffer->ptr, text, len);
        buffer->position = len - 1;
    }

    return buffer;
}

/**
 * Преобразование буфера в фрагмент.
 *
 * @param buffer Буфер.
 * @return Фрагмент.
 */
MAGNA_API Span MAGNA_CALL buffer_to_span
    (
        const Buffer *buffer
    )
{
    Span result;

    assert (buffer != NULL);

    result.ptr = buffer->ptr;
    result.len = buffer->position;

    return result;
}

/**
 * Преобразование фрагмента в буфер.
 *
 * @param buffer
 * @param span
 * @return
 */
MAGNA_API Buffer* MAGNA_CALL buffer_from_span
    (
        Buffer *buffer,
        Span span
    )
{
    assert (buffer != NULL);

    buffer->ptr = span.ptr;
    buffer->position = 0;
    buffer->capacity = span.len;

    return buffer;
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
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL buffer_replace_text
    (
        Buffer *buffer,
        const am_byte *from,
        const am_byte *to
    )
{
    am_byte *ptr1, *ptr2, *end;
    am_size_t lenFrom, lenTo, count, offset;

    assert (buffer != NULL);
    assert (from != NULL);
    assert (to != NULL);

    if (buffer->position == 0) {
        /* В пустом буфере нам нечего делать */
        return AM_TRUE;
    }

    lenFrom = strlen (from);
    lenTo = strlen (to);
    end = buffer->ptr + buffer->position;
    for (ptr1 = buffer->ptr; ptr1 < end; ) {
        ptr2 = find_text (ptr1, end, from);
        if (ptr2) {
            /* нашли заменяемую строку */
            if (lenFrom < lenTo) {
                /* заменяющая строка длинее */
                offset = ptr2 - buffer->ptr;
                if (!buffer_grow (buffer, buffer->position + lenTo - lenFrom)) {
                    return AM_FALSE;
                }
                /* Из-за переаллокации буфера указатель мог измениться */
                ptr2 = buffer->ptr + offset;
                count = end - ptr2 - lenFrom;
                if (count > 0) {
                    memmove (ptr2 + lenTo, ptr2 + lenFrom, count);
                }
            }
            else if (lenFrom > lenTo) {
                count = end - ptr2 - lenTo;
                if (count > 0) {
                    memmove (ptr2 + lenTo, ptr2 + lenFrom, count);
                }
            }

            /* копируем заменяющую строку */
            memcpy (ptr2, to, lenTo);
            buffer->position += (lenTo - lenFrom);
            end = buffer->ptr + buffer->position;
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
 * @param first
 * @param second
 * @return
 */
MAGNA_API int MAGNA_CALL buffer_compare
    (
        const Buffer *first,
        const Buffer *second
    )
{
    am_size_t i;
    int result;

    assert (first != NULL);
    assert (second != NULL);

    for (i = 0; ; ++i) {
        if (i == first->position) {
            if (i == second->position) {
                return 0;
            }

            return -1;
        }
        else if (i == second->position) {
                return 1;
            }
        else {
            result = first->ptr[i] - second->ptr[i];
            if (result != 0) {
                return result;
            }
        }
    }
}

/**
 * Побайтовое сравнение буфера со строкой.
 *
 * @param buffer
 * @param text
 * @return
 */
MAGNA_API int MAGNA_CALL buffer_compare_text
    (
        const Buffer *buffer,
        const am_byte *text
    )
{
    const am_byte *ptr = text;
    am_size_t i;
    int result;

    assert (buffer != NULL);
    assert (text != NULL);

    for (i = 0; ; ++i, ++ptr) {
        if (i == buffer->position) {
            if (!*ptr) {
                return 0;
            }

            return -1;
        }
        else if (!ptr) {
            return 1;
        }
        else {
            result = buffer->ptr[i] - *ptr;
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
            buffer->ptr,
            buffer->ptr + buffer->position,
            text
        );
}

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
