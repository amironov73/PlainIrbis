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
 * \file chunked.c
 *
 * Буфер, состоящий из мелких блоков.
 *
 */

/*=========================================================*/

/**
 * Инициализация буфера.
 *
 * @param chunked Буфер, подлежащий инициализации.
 * @param chunkSize Размер блока.
 */
MAGNA_API ChunkedBuffer* MAGNA_CALL chunked_init
    (
            ChunkedBuffer *chunked,
            am_size_t chunkSize
    )
{
    assert (chunked != NULL);

    memset (chunked, 0, sizeof (ChunkedBuffer));
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
    assert (chunked != NULL);
}

/**
 * Достигнут ли конец буфера?
 *
 * @param chunked
 * @return
 */
MAGNA_API am_bool MAGNA_CALL chunked_eof
    (
        const ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    return AM_FALSE;
}

/**
 * Подглядывание текущего символа в буфере.
 *
 * @param chunked
 * @return -1, если достигнут конец.
 */
MAGNA_API int MAGNA_CALL chunked_peek
    (
        const ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    return -1;
}

/**
 * Текущая позиция в буфере.
 *
 * @param chunked
 * @return
 */
MAGNA_API am_size_t MAGNA_CALL chunked_position
    (
        const ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    return 0;
}

/**
 * Чтение текущего байта в буфере.
 *
 * @param chunked
 * @return -1, если достигнут конец.
 */
MAGNA_API int MAGNA_CALL chunked_read_byte
    (
        ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    return -1;
}

/**
 * Чтение данных из буфера.
 *
 * @param chunked
 * @param buffer
 * @param count
 * @return
 */
MAGNA_API am_size_t MAGNA_CALL chunked_read
    (
            ChunkedBuffer *chunked,
            Buffer *buffer,
            am_size_t count
    )
{
    assert (chunked != NULL);
    assert (buffer != NULL);

    return 0;
}

/**
 * Чтение строки из буфера.
 *
 * @param chunked
 * @param buffer
 * @return
 */
MAGNA_API am_size_t MAGNA_CALL chunked_read_line
    (
        ChunkedBuffer *chunked,
        Buffer *buffer
    )
{
    assert (chunked != NULL);
    assert (buffer != NULL);

    return 0;
}

/**
 * Чтение оставшихся в буфере данных.
 *
 * @param chunked
 * @param buffer
 * @return
 */
MAGNA_API am_size_t MAGNA_CALL chunked_read_remaining
    (
        ChunkedBuffer *chunked,
        Buffer *buffer
    )
{
    assert (chunked != NULL);
    assert (buffer != NULL);

    return 0;
}

/**
 * Число байт, оставшихся непрочитанными.
 *
 * @param chunked Буфер.
 * @return
 */
MAGNA_API am_size_t MAGNA_CALL chunked_remaining_size
    (
        const ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    return 0;
}

/**
 * Перемотка к началу.
 *
 * @param chunked
 * @return
 */
MAGNA_API ChunkedBuffer* MAGNA_CALL chunked_rewind
    (
        ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    return chunked;
}

/**
 * Общий размер данных в буфере.
 *
 * @param chunked
 * @return
 */
MAGNA_API am_size_t MAGNA_CALL chunked_size
    (
        const ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    return 0;
}

/**
 * Общий размер данных в буфере.
 *
 * @param chunked
 * @return
 */
MAGNA_API am_size_t MAGNA_CALL chunked_capacity
    (
        const ChunkedBuffer *chunked
    )
{
    assert (chunked != NULL);

    return 0;
}

/**
 * Получение всех данных из буфера.
 *
 * @param chunked
 * @param buffer
 * @return
 */
MAGNA_API Buffer* MAGNA_CALL chunked_all
    (
        const ChunkedBuffer *chunked,
        Buffer *buffer
    )
{
    assert (chunked != NULL);
    assert (buffer != NULL);

    return buffer;
}

/**
 * Увеличение емкости буфера.
 *
 * @param chunked
 * @param newSize
 * @return
 */
MAGNA_API am_bool MAGNA_CALL chunked_grow
    (
            ChunkedBuffer *chunked,
            am_size_t newSize
    )
{
    assert (chunked != NULL);

    return AM_FALSE;
}

/**
 * Запись данных в буфер.
 *
 * @param chunked
 * @param data
 * @param dataSize
 * @return
 */
MAGNA_API am_bool MAGNA_CALL chunked_write
    (
            ChunkedBuffer *chunked,
            am_byte *data,
            am_size_t dataSize
    )
{
    assert (chunked != NULL);
    assert (data != NULL);

    return AM_FALSE;
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

    return AM_FALSE;
}

/**
 * Запись строки в буфер.
 *
 * @param chunked
 * @param text
 * @return
 */
MAGNA_API am_bool MAGNA_CALL chunked_write_text
    (
        ChunkedBuffer *chunked,
        const char *text
    )
{
    assert (chunked != NULL);
    assert (text != NULL);

    return AM_FALSE;
}

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/

