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
 * \file chunked.c
 *
 * Буфер, состоящий из мелких блоков.
 *
 * Владеет своей памятью.
 * Для освобождения ресурсов используйте `chunked_free`.
 *
 * Потоконебезопасный.
 *
 * Находится в одном из двух режимов: записи (сразу после создания)
 * или чтения (переводится вызовом `chunked_rewind`). Совмещать эти
 * режимы невозможно. Попытки чтения без вызова `chunked_rewind`
 * приведут к неопределенному поведению. Повторный вызов `chunked_rewind`
 * отмотает буфер к началу, это предусмотренное поведение.
 *
 * Записанные в буфер данные находятся от начала чанка `first`
 * до (включая) чанк `last` до (исключая) смещения `position`.
 *
 * Текущая позиция чтения из буфера находится в чанке `current`
 * по смещению `read` (следующее чтение начнется именно
 * с этой позиции).
 */

/*=========================================================*/

/* Двигаемся вперед при чтении буфера */
static am_bool advance
    (
        ChunkedBuffer *chunked
    )
{
    if (chunked->current == chunked->last) {
        return AM_FALSE;
    }

    chunked->current = chunked->current->next;
    chunked->read = 0;

    return AM_TRUE;
}

/* Добавляем чанк (при записи), соответственно сдвигаем позицию */
static am_bool append_chunk
    (
        ChunkedBuffer *chunked
    )
{
    MemoryChunk *newChunk;
    am_byte *data;

    if (chunked->current != NULL
        && chunked->current->next != NULL) {
        /* У нас уже есть последующие чанки */
        chunked->current = chunked->current->next;
        chunked->position = 0;
        return AM_TRUE;
    }

    /* Последующих чанков нет, размещаем их в куче */
    newChunk = mem_alloc (sizeof (MemoryChunk));
    if (newChunk == NULL) {
        return AM_FALSE;
    }

    data = mem_alloc (chunked->chunkSize);
    if (data == NULL) {
        mem_free (newChunk);
        return AM_FALSE;
    }

    newChunk->data = data;
    newChunk->next = NULL;

    if (chunked->first != NULL) {
        chunked->last->next = newChunk;
    }
    else {
        chunked->first = newChunk;
    }

    chunked->current = chunked->last = newChunk;
    chunked->position = 0;

    return AM_TRUE;
}

/**
 * Инициализация буфера.
 *
 * @param chunked Указатель на ненинициализированную структуру.
 * @param chunkSize Размер блока (<= означает "на усмотрение системы").
 * @return Указатель на инициализированную структуру.
 */
MAGNA_API ChunkedBuffer* MAGNA_CALL chunked_init
    (
        ChunkedBuffer *chunked,
        am_size_t chunkSize
    )
{
    assert (chunked != NULL);

    mem_clear (chunked, sizeof (ChunkedBuffer));
    if (chunkSize <= 0) {
        chunkSize = 4096;
    }

    chunked->chunkSize = chunkSize;

    return chunked;
}

/**
 * Освобождение буфера.
 *
 * @param chunked Буфер, подлежащий освобождению.
 */
MAGNA_API void MAGNA_CALL chunked_free
    (
        ChunkedBuffer *chunked
    )
{
    MemoryChunk *chunk, *next;

    assert (chunked != NULL);

    chunk = chunked->first;
    while (chunk != NULL) {
        next = chunk->next;
        mem_free (chunk->data);
        mem_free (chunk);
        chunk = next;
    }

    chunked->first = chunked->current = chunked->last = NULL;
}

/**
 * Прверка, пуст ли буфер (при записи)?
 *
 * @param chunked Указатель на буфер.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL chunked_empty
    (
        const ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    if (chunked->first == NULL) {
        return AM_TRUE;
    }

    if (chunked->current == chunked->first) {
        return chunked->position == 0;
    }

    return AM_FALSE;
}

/**
 * Достигнут ли конец буфера (при чтении)?
 *
 * @param chunked Указатель на буфер.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL chunked_eof
    (
        const ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    if (chunked->current == NULL) {
        return AM_TRUE;
    }

    if (chunked->current == chunked->last) {
        return chunked->read == chunked->position;
    }

    return AM_FALSE;
}

/**
 * Подглядывание текущего символа в буфере.
 *
 * @param chunked Указатель на буфер.
 * @return -1, если достигнут конец.
 */
MAGNA_API int MAGNA_CALL chunked_peek
    (
        ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    if (chunked->current == NULL) {
        return -1;
    }

    if (chunked->current == chunked->last) {
        if (chunked->read >= chunked->position) {
            return -1;
        }
    }
    else {
        if (chunked->read >= chunked->chunkSize) {
            (void) advance (chunked);
        }
    }

    return chunked->current->data [chunked->read];
}

/**
 * Текущая позиция в буфере (при чтении).
 *
 * @param chunked Указатель на буфер.
 * @return Смещение от начала в байтах.
 */
MAGNA_API am_size_t MAGNA_CALL chunked_position
    (
        const ChunkedBuffer *chunked
    )
{
    am_size_t result;
    const MemoryChunk *chunk;

    assert (chunked != NULL);

    result = chunked->read;
    for (chunk = chunked->first; chunk != NULL; chunk = chunk->next) {
        if (chunk == chunked->current) {
            break;
        }
        else {
            result += chunked->chunkSize;
        }
    }

    return result;
}

/**
 * Чтение текущего байта в буфере.
 *
 * @param chunked Указатель на буфер.
 * @return -1, если достигнут конец.
 */
MAGNA_API int MAGNA_CALL chunked_read_byte
    (
        ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    if (chunked->current == NULL) {
        return -1;
    }

    if (chunked->current == chunked->last) {
        if (chunked->read >= chunked->position) {
            return -1;
        }
    }
    else {
        if (chunked->read >= chunked->chunkSize) {
            (void) advance (chunked);
        }
    }

    return chunked->current->data [chunked->read++];
}

/**
 * Чтение данных из буфера.
 *
 * @param chunked Указатель на буфер.
 * @param buffer Куда помещать данные.
 * @param count Число байт для чтения.
 * @return Количество прочитанных байт. -1 при ошибке.
 */
MAGNA_API am_ssize_t MAGNA_CALL chunked_read
    (
        ChunkedBuffer *chunked,
        Buffer *buffer,
        am_size_t count
    )
{
    am_size_t total = 0, remaining, portion;

    assert (chunked != NULL);
    assert (buffer != NULL);

    if ((count == 0)
        || (chunked->current == NULL)) {
        return 0;
    }

    do {
        remaining = chunked->current == chunked->last
                ? chunked->position - chunked->read
                : chunked->chunkSize - chunked->read;

        if (!remaining) {
            if (!advance (chunked)) {
                break;
            }
        }

        portion = remaining;
        if (remaining < count) {
            portion = count;
        }

        if (!buffer_write
            (
                buffer,
                chunked->current->data + chunked->read,
                portion
            )) {
            return -1;
        }

        chunked->read += portion;
        count -= portion;
        total += portion;

    } while (count);

    return (am_ssize_t) total;
}

/**
 * Чтение данных из буфера.
 *
 * @param chunked Указатель на буфер.
 * @param buffer Куда помещать данные.
 * @param count Число байт для чтения.
 * @return Количество прочитанных байт. -1 при ошибке.
 */
MAGNA_API am_ssize_t MAGNA_CALL chunked_read_raw
    (
        ChunkedBuffer *chunked,
        am_byte *buffer,
        am_size_t count
    )
{
    am_size_t total = 0, remaining, portion, offset = 0;

    assert (chunked != NULL);
    assert (buffer != NULL);

    if ((count == 0)
        || (chunked->current == NULL)) {
        return 0;
    }

    do {
        remaining = chunked->current == chunked->last
                ? chunked->position - chunked->read
                : chunked->chunkSize - chunked->read;

        if (!remaining) {
            if (!advance (chunked)) {
                break;
            }
        }

        portion = remaining;
        if (remaining < count) {
            portion = count;
        }

        mem_copy
            (
                buffer + offset,
                chunked->current->data + chunked->read,
                portion
            );

        offset += portion;
        chunked->read += portion;
        count -= portion;
        total += portion;

    } while (count);

    return (am_ssize_t) total;
}

/**
 * Чтение строки из буфера.
 *
 * @param chunked Указатель на буфер.
 * @param buffer Куда помещать строку. Перевод строки считывается но в результат не помещается.
 * @return Длина прочитанной строки, -1 при ошибке.
 */
MAGNA_API am_ssize_t MAGNA_CALL chunked_read_line
    (
        ChunkedBuffer *chunked,
        Buffer *buffer
    )
{
    am_size_t result = 0;
    int c;

    assert (chunked != NULL);
    assert (buffer != NULL);

    while (AM_TRUE) {
        c = chunked_read_byte (chunked);
        if (c < 0) {
            break;
        }

        if (c == '\r') {
            if (chunked_peek (chunked) == '\n') {
                (void) chunked_read_byte (chunked);
            }
            break;
        }

        if (c == '\n') {
            break;
        }

        if (!buffer_putc (buffer, (char) c)) {
            return -1;
        }

        ++result;
    }

    return (am_ssize_t) result;
}

/**
 * Чтение оставшихся в буфере данных.
 * Позиция в буфере не сдвигается!
 *
 * @param chunked Указатель на буффер.
 * @param buffer Куда складывать результат.
 * @return Количество успешно прочитанных байт. -1 при ошибке.
 */
MAGNA_API am_ssize_t MAGNA_CALL chunked_read_remaining
    (
        const ChunkedBuffer *chunked,
        Buffer *buffer
    )
{
    am_size_t result = 0, portion;
    am_byte *data;
    MemoryChunk *chunk;

    assert (chunked != NULL);
    assert (buffer != NULL);

    if (!buffer_grow (buffer, chunked_remaining_size (chunked))) {
        return -1;
    }

    if (chunked->current != NULL) {

        /* У нас есть данные */
        if (chunked->current == chunked->last) {
            /* Все поместилось в одном чанке */
            portion = chunked->position - chunked->read;
            data = chunked->current->data + chunked->read;
            if (!buffer_write (buffer, data, portion)) {
                return -1;
            }

            result += portion;
        }
        else {
            /* У нас несколько чанков, будем есть по частям */

            /* Сначала отрабатываем текущий чанк */
            data = chunked->current->data;
            portion = chunked->chunkSize - chunked->read;
            if (!buffer_write (buffer, data, portion)) {
                return -1;
            }

            result += portion;

            /* Теперь последующие чанки кроме последнего */
            for (
                    chunk = chunked->current->next;
                    chunk != chunked->last;
                    chunk = chunk->next
                ) {
                if (!buffer_write (buffer, chunk->data, chunked->chunkSize)) {
                    return -1;
                }

                result += chunked->chunkSize;
            }

            /* Последний чанк */
            data = chunked->last->data;
            portion = chunked->position;
            if (!buffer_write (buffer, data, portion)) {
                return -1;
            }

            result += portion;
        }
    }

    return (am_ssize_t) result;
}

/**
 * Число байт, оставшихся непрочитанными.
 *
 * @param chunked Буфер.
 * @return Общее количество непрочитванных байт.
 */
MAGNA_API am_size_t MAGNA_CALL chunked_remaining_size
    (
        const ChunkedBuffer *chunked
    )
{
    am_size_t result = 0;
    const MemoryChunk *chunk;

    assert (chunked != NULL);

    if (chunked->current) {
        /* У нас есть данные */
        if (chunked->current == chunked->last) {
            /* Все поместилось в одном чанке */
            result = chunked->position - chunked->read;
        }
        else {
            /* У нас несколько чанков, будем есть по частям */

            /* Сначала считаем текущий чанк */
            result = chunked->chunkSize - chunked->read;

            /* Потом последующие чанки кроме последнего */
            for (
                    chunk = chunked->current->next;
                    chunk != chunked->last;
                    chunk = chunk->next
                ) {
                result += chunked->chunkSize;
            }

            /* Последний чанк */
            result += chunked->position;
        }
    }

    return result;
}

/**
 * Перемотка к началу, чтобы начать чтение.
 *
 * @param chunked Указатель на буфер.
 * @return Тот же указатель на буфер.
 */
MAGNA_API ChunkedBuffer* MAGNA_CALL chunked_rewind
    (
        ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    chunked->current = chunked->first;
    chunked->read = 0;

    return chunked;
}

/**
 * Общий размер данных в буфере.
 *
 * @param chunked Указатель на буфер.
 * @return Общий размер данных в байтах.
 */
MAGNA_API am_size_t MAGNA_CALL chunked_size
    (
        const ChunkedBuffer *chunked
    )
{
    am_size_t result;
    const MemoryChunk *chunk;

    assert (chunked != NULL);

    result = chunked->position;
    for (chunk = chunked->first; chunk != NULL; chunk = chunk->next) {
        if (chunk == chunked->current) {
            break;
        }
        result += chunked->chunkSize;
    }

    return result;
}

/**
 * Общая емкость буфера.
 *
 * @param chunked Указатель на буфер.
 * @return Общая емкость в байтах.
 */
MAGNA_API am_size_t MAGNA_CALL chunked_capacity
    (
        const ChunkedBuffer *chunked
    )
{
    am_size_t result = 0;
    const MemoryChunk *chunk;

    assert (chunked != NULL);

    for (chunk = chunked->first; chunk != NULL; chunk = chunk->next) {
        result += chunked->chunkSize;
    }

    return result;
}

/**
 * Получение всех данных из буфера.
 * Движения вперед не происходит.
 *
 * @param chunked Указатель на буфер.
 * @param buffer Куда помещать результат.
 * @return Общий размер считанных данных.
 */
MAGNA_API am_ssize_t MAGNA_CALL chunked_all
    (
        const ChunkedBuffer *chunked,
        Buffer *buffer
    )
{
    am_size_t result = 0, portion;
    am_byte *data;
    MemoryChunk *chunk;

    assert (chunked != NULL);
    assert (buffer != NULL);

    if (!buffer_grow (buffer, chunked_size (chunked))) {
        return -1;
    }

    if (chunked->current != NULL) {

        /* У нас есть данные */
        if (chunked->first == chunked->last) {
            /* Все поместилось в одном чанке */
            portion = chunked->position - chunked->read;
            data = chunked->first->data + chunked->read;
            if (!buffer_write (buffer, data, portion)) {
                return -1;
            }

            result += portion;
        }
        else {
            /* У нас несколько чанков, будем есть по частям */

            /* Сначала отрабатываем первый чанк */
            data = chunked->first->data;
            portion = chunked->chunkSize;
            if (!buffer_write (buffer, data, portion)) {
                return -1;
            }

            result += portion;

            /* Теперь последующие чанки кроме последнего */
            for (
                    chunk = chunked->first->next;
                    chunk != chunked->last;
                    chunk = chunk->next
                ) {
                if (!buffer_write (buffer, chunk->data, chunked->chunkSize)) {
                    return -1;
                }

                result += chunked->chunkSize;
            }

            /* Последний чанк */
            data = chunked->last->data;
            portion = chunked->position;
            if (!buffer_write (buffer, data, portion)) {
                return -1;
            }

            result += portion;
        }
    }

    return (am_ssize_t) result;
}

/**
 * Получение указателя на байт по указанному смещению.
 *
 * @param chunked Указатель на буфер.
 * @param offset Смещение.
 * @return Указатель либо `NULL`.
 */
MAGNA_API am_byte* MAGNA_CALL chunked_at
    (
        const ChunkedBuffer *chunked,
        am_size_t offset
    )
{
    const MemoryChunk *chunk;

    assert (chunked != NULL);

    if (chunked->current == NULL) {
        return NULL;
    }

    /* Пробегаем все чанки кроме последнего */
    for (chunk = chunked->first; chunk != chunked->last; chunk = chunk->next) {
        if (offset < chunked->chunkSize) {
            return chunk->data + offset;
        }

        offset -= chunked->chunkSize;
    }

    /* Последний чанк */
    if (offset < chunked->position) {
        return chunked->last->data + offset;
    }

    return NULL;
}

/**
 * Увеличение емкости буфера.
 *
 * @param chunked Указатель на буфер.
 * @param newSize Предполагаемая новая емкость буфера.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL chunked_grow
    (
        ChunkedBuffer *chunked,
        am_size_t newSize
    )
{
    MemoryChunk *newChunk;
    am_byte *data;
    am_size_t alreadyHave;

    assert (chunked != NULL);

    alreadyHave = chunked_capacity (chunked);
    if (newSize <= alreadyHave) {
        return AM_TRUE;
    }

    while (newSize != 0) {

        newChunk = mem_alloc (sizeof (MemoryChunk));
        if (newChunk == NULL) {
            return AM_FALSE;
        }

        data = mem_alloc (chunked->chunkSize);
        if (data == NULL) {
            mem_free (newChunk);
            return AM_FALSE;
        }

        newChunk->data = data;
        newChunk->next = NULL;

        if (chunked->first != NULL) {
            chunked->last->next = newChunk;
        }
        else {
            chunked->first = chunked->current = newChunk;
        }

        chunked->last = newChunk;

        if (newSize <= chunked->chunkSize) {
            break;
        }

        newSize -= chunked->chunkSize;
    }

    return AM_TRUE;
}

/**
 * Запись данных в буфер.
 *
 * @param chunked Указатель на буфер.
 * @param data Данные.
 * @param dataSize Размер данных в байтах.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL chunked_write
    (
        ChunkedBuffer *chunked,
        const am_byte *data,
        am_size_t dataSize
    )
{
    am_size_t available, portion;
    am_byte *ptr;

    assert (chunked != NULL);
    assert (data != NULL);

    if (dataSize == 0) {
        return AM_TRUE;
    }

    if (dataSize > chunked->chunkSize) {
        /* Если данных много, пытаемся зарезервировать память сразу */
        if (!chunked_grow (chunked, chunked_size(chunked) + dataSize)) {
            return AM_FALSE;
        }
    }

    if (chunked->first == NULL) {
        /* Если нет первого чанка, создаем его */
        if (!append_chunk (chunked)) {
            return AM_FALSE;
        }
    }

    do {
        /* Переходим к собственно записи */

        /* Оставшийся свободный кусок в текущем чанке */
        available = chunked->chunkSize - chunked->position;
        if (available == 0) {
            if (!append_chunk (chunked)) {
                return AM_FALSE;
            }

            available = chunked->chunkSize;
        }

        portion = dataSize;
        if (portion > available) {
            portion = available;
        }

        ptr = chunked->current->data + chunked->position;
        mem_copy (ptr, data, portion);
        chunked->position += portion;
        dataSize -= portion;
        data += portion;

    } while (dataSize);

    return AM_TRUE;
}

/**
 * Запись одного байта.
 *
 * @param chunked
 * @param value
 * @return
 */
MAGNA_API am_bool MAGNA_CALL chunked_write_byte
    (
        ChunkedBuffer *chunked,
        am_byte value
    )
{
    assert (chunked != NULL);

    if (chunked->current == NULL) {
        if (!append_chunk (chunked)) {
            return AM_FALSE;
        }
    }

    if (chunked->position >= chunked->chunkSize) {
        if (!append_chunk (chunked)) {
            return AM_FALSE;
        }
    }

    chunked->current->data [chunked->position++] = value;

    return AM_TRUE;
}

/**
 * Чтение из буфера символа в кодировке UTF-8.
 *
 * @param chunked Указатель на буфер.
 * @return Прочитанный символ либо -1.
 */
MAGNA_API int MAGNA_CALL chunked_read_utf8
    (
        ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    return -1;
}

/**
 * Запись в буфер символа в кодировке UTF-8.
 *
 * @param chunked Указатель на буфер.
 * @param chr Записываемый символ.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL chunked_write_utf8
    (
        ChunkedBuffer *chunked,
        unsigned chr
    )
{
    assert (chunked != NULL);

    if (chr < (1u << 7u)) {
        return chunked_write_byte (chunked, (char) chr);
    }

    if (chr < (1u << 11u)) {
        return chunked_write_byte (chunked, (chr >> 6u) | 0xC0u)
               && chunked_write_byte (chunked, (chr & 0x3Fu) | 0x80u);
    }

    if (chr < (1u << 16u)) {
        return chunked_write_byte (chunked, (chr >> 12u) | 0xE0u)
               && chunked_write_byte (chunked, ((chr >> 6u) & 0x3Fu) | 0x80u)
               && chunked_write_byte (chunked, (chr & 0x3Fu) | 0x80u);
    }

    if (chr < (1u << 21u)) {
        return chunked_write_byte (chunked, (chr >> 18u) | 0xF0u)
               && chunked_write_byte (chunked, ((chr >> 12u) & 0x3Fu) | 0x80u)
               && chunked_write_byte (chunked, ((chr >> 6u) & 0x3Fu) | 0x80u)
               && chunked_write_byte (chunked, (chr & 0x3Fu) | 0x80u);
    }

    return AM_FALSE;
}

/**
 * Запись строки в буфер.
 *
 * @param chunked Указатель на буфер.
 * @param text Текст.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL chunked_write_text
    (
        ChunkedBuffer *chunked,
        const char *text
    )
{
    assert (chunked != NULL);
    assert (text != NULL);

    return chunked_write (chunked, (const am_byte*) text, strlen (text));
}

/**
 * Добавление перевода на новую строку.
 *
 * @param chunked Указатель на буфер.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL chunked_new_line
    (
        ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    return chunked_write_byte (chunked, '\n');
}

/**
 * Преобразование данных из кодировки UTF-8 в ANSI.
 *
 * @param target Инициализированный буфер назначения.
 * @param source Буфер с исходными данными.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL chunked_utf8_to_ansi
    (
        ChunkedBuffer *target,
        const Buffer *source
    )
{
    am_size_t delta;

    assert (target != NULL);
    assert (source != NULL);

    delta = utf8_code_points (source->ptr, source->position);
    if (!chunked_grow (target, delta)) {
        return AM_FALSE;
    }

    /* TODO: сделать собственно преобразование */

    return AM_FALSE;
}

/**
 * Преобразование данных из кодировки ANSI  в UTF-8.
 *
 * @param target Инициализированный буфер назначения.
 * @param source Буфер с исходными данными.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL chunked_ansi_to_utf8
    (
        ChunkedBuffer *target,
        const Buffer *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    if (!chunked_grow (target, source->position)) {
        return AM_FALSE;
    }

    /* TODO: собственно преобразование */

    return AM_FALSE;
}


/*=========================================================*/

/* Stream-обертка */

MAGNA_API am_ssize_t MAGNA_CALL chunked_read_function
    (
        Stream *stream,
        am_byte *data,
        am_size_t length
    )
{
    ChunkedBuffer *buffer;

    assert (stream != NULL);

    buffer = (ChunkedBuffer *) stream->data;
    assert (buffer != NULL);

    return chunked_read_raw (buffer, data, length);
}

MAGNA_API am_ssize_t MAGNA_CALL chunked_write_function
    (
        Stream *stream,
        const am_byte *data,
        am_size_t length
    )
{
    ChunkedBuffer *buffer;

    assert (stream != NULL);

    buffer = (ChunkedBuffer *) stream->data;
    assert (buffer != NULL);

    return chunked_write (buffer, data, length) ? length : 0;
}

MAGNA_API am_bool MAGNA_CALL chunked_close_function
    (
        Stream *stream
    )
{
    ChunkedBuffer *buffer;

    assert (stream != NULL);

    buffer = (ChunkedBuffer *) stream->data;
    if (buffer != NULL) {
        chunked_free (buffer);
        mem_free (buffer);
        stream->data = NULL;
    }

    return AM_TRUE;
}

/**
 * Предполагается, что память будут записывать.
 *
 * @param stream
 * @param chunkSize
 * @return
 */
MAGNA_API am_bool MAGNA_CALL chunked_stream_create
    (
        Stream *stream,
        am_size_t chunkSize
    )
{
    ChunkedBuffer *chunked;

    assert (stream != NULL);

    if (!stream_init (stream)) {
        return AM_FALSE;
    }

    chunked = (ChunkedBuffer*) mem_alloc (sizeof (ChunkedBuffer));
    if (chunked == NULL) {
        return AM_FALSE;
    }

    chunked_init (chunked, chunkSize);

    stream->data = (void*) chunked;
    stream->readFunction  = chunked_read_function;
    stream->writeFunction = chunked_write_function;
    stream->closeFunction = chunked_close_function;

    return AM_TRUE;
}

/**
 * Перемотка к началу, чтобы начать чтение.
 *
 * @param stream Указатель на буфер.
 * @return Тот же указатель на буфер.
 */
MAGNA_API am_bool MAGNA_CALL chunked_stream_rewind
    (
        Stream *stream
    )
{
    ChunkedBuffer *buffer;

    assert (stream != NULL);

    buffer = (ChunkedBuffer *) stream->data;
    assert (buffer != NULL);

    chunked_rewind (buffer);

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
