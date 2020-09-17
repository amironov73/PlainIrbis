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
 * \file stream.c
 *
 * Простая абстракция потока.
 */

/*=========================================================*/

/**
 * Базовая инициализация потока.
 *
 * @param stream Указатель на неинициализированную структуру.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL stream_init
    (
        Stream *stream
    )
{
    assert (stream != NULL);

    memset (stream, 0, sizeof (Stream));

    return AM_TRUE;
}

/**
 * Чтение из потока.
 *
 * @param stream
 * @param buffer
 * @param length
 * @return Количество успешно прочитанных байт. Отрицательное значение свидетельствует об ошибке.
 */
MAGNA_API am_ssize_t MAGNA_CALL stream_read
    (
        Stream *stream,
        am_byte *buffer,
        am_size_t length
    )
{
    assert (stream != NULL);
    assert (stream->readFunction != NULL);

    return stream->readFunction (stream, buffer, length);
}

/**
 * Запись данных в поток.
 *
 * @param stream
 * @param buffer
 * @param length
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL stream_write
    (
        Stream *stream,
        const am_byte *buffer,
        am_size_t length
    )
{
    assert (stream != NULL);
    assert (stream->writeFunction != NULL);

    return stream->writeFunction (stream, buffer, length) == length;
}

/**
 * Позиционирование в потоке.
 *
 * @param stream
 * @param position
 * @return
 */
MAGNA_API am_ssize_t MAGNA_CALL stream_seek
    (
        Stream *stream,
        am_size_t position
    )
{
    assert (stream != NULL);
    assert (stream->seekFunction != NULL);

    return stream->seekFunction (stream, position);
}

/**
 * Получение текущей позиции в потоке.
 *
 * @param stream
 * @return
 */
MAGNA_API am_ssize_t MAGNA_CALL stream_tell
    (
        Stream *stream
    )
{
    assert (stream != NULL);
    assert (stream->tellFunction != NULL);

    return stream->tellFunction (stream);
}

/**
 * Закрытие Потока.
 *
 * @param stream Поток.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL stream_close
    (
        Stream *stream
    )
{
    assert (stream != NULL);

    if (stream->closeFunction != NULL) {
        return stream->closeFunction (stream);
    }

    return AM_TRUE;
}

/**
 * Копирование одного потока в другой.
 *
 * @param target
 * @param source
 * @return
 */
MAGNA_API am_bool MAGNA_CALL stream_copy
    (
        Stream *target,
        Stream *source
    )
{
    am_byte buffer [1024];
    am_ssize_t rc;

    assert (target != NULL);
    assert (source != NULL);

    while (AM_TRUE) {
        rc = stream_read (source, buffer, sizeof (buffer));
        if (rc < 0) {
            return AM_FALSE;
        }

        if (rc == 0) {
            break;
        }

        if (!stream_write (target, buffer, rc)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/*=========================================================*/

/* Нулевой (пустой) поток */

MAGNA_API am_ssize_t MAGNA_CALL null_read_function
    (
        Stream *stream,
        am_byte *buffer, /* NOLINT(readability-non-const-parameter) */
        am_size_t length
    )
{
    (void) stream;
    (void) buffer;
    (void) length;

    return 0;
}

MAGNA_API am_ssize_t MAGNA_CALL null_write_function
    (
        Stream *stream,
        const am_byte *buffer,
        am_size_t length
    )
{
    (void) stream;
    (void) buffer;

    return length;
}

MAGNA_API am_ssize_t MAGNA_CALL null_seek_function
    (
        Stream *stream,
        am_size_t position
    )
{
    (void) stream;

    return position;
}

MAGNA_API am_ssize_t MAGNA_CALL null_tell_function
    (
        Stream *stream
    )
{
    (void) stream;

    return 0;
}

MAGNA_API am_bool MAGNA_CALL null_close_function
    (
        Stream *stream
    )
{
    (void) stream;

    return AM_TRUE;
}

MAGNA_API am_bool MAGNA_CALL null_stream_open
    (
        Stream *stream
    )
{
    assert (stream != NULL);

    if (!stream_init (stream)) {
        return AM_FALSE;
    }

    stream->readFunction  = null_read_function;
    stream->writeFunction = null_write_function;
    stream->seekFunction  = null_seek_function;
    stream->tellFunction  = null_tell_function;
    stream->closeFunction = null_close_function;

    return AM_TRUE;
}

/*=========================================================*/

/* Поток в памяти (буфер) */

MAGNA_API am_ssize_t MAGNA_CALL memory_read_function
    (
        Stream *stream,
        am_byte *data,
        am_size_t length
    )
{
    Buffer *buffer;

    assert (stream != NULL);

    buffer = (Buffer *) stream->data;
    assert (buffer != NULL);

    return (am_ssize_t) buffer_read (buffer, data, length);
}

MAGNA_API am_ssize_t MAGNA_CALL memory_write_function
    (
        Stream *stream,
        const am_byte *data,
        am_size_t length
    )
{
    Buffer *buffer;

    assert (stream != NULL);

    buffer = (Buffer *) stream->data;
    assert (buffer != NULL);

    return buffer_write (buffer, data, length) ? length : 0;
}

MAGNA_API am_ssize_t MAGNA_CALL memory_seek_function
    (
        Stream *stream,
        am_size_t position
    )
{
    Buffer *buffer;

    assert (stream != NULL);

    buffer = (Buffer *) stream->data;
    assert (buffer != NULL);

    if (!buffer_grow (buffer, position)) {
        return -1;
    }

    return buffer->position = position;
}

MAGNA_API am_ssize_t MAGNA_CALL memory_tell_function
    (
        Stream *stream
    )
{
    Buffer *buffer;

    assert (stream != NULL);

    buffer = (Buffer *) stream->data;
    assert (buffer != NULL);

    return (am_ssize_t) buffer->position;
}

/**
 * Не высвобождает память, занятую буфером.
 *
 * @param stream
 * @return
 */
MAGNA_API am_bool MAGNA_CALL memory_close_function_1
    (
        Stream *stream
    )
{
    assert (stream != NULL);

    stream->data = NULL;

    return AM_TRUE;
}

/**
 * Высвобождает память, занятую буфером.
 *
 * @param stream
 * @return
 */
MAGNA_API am_bool MAGNA_CALL memory_close_function_2
    (
        Stream *stream
    )
{
    Buffer *buffer;

    assert (stream != NULL);

    buffer = (Buffer *) stream->data;
    if (buffer != NULL) {
        buffer_free (buffer);
        stream->data = NULL;
    }

    return AM_TRUE;
}

/**
 * Предполагается, что память будут записывать.
 *
 * @param stream
 * @return
 */
MAGNA_API am_bool MAGNA_CALL memory_stream_create
    (
        Stream *stream
    )
{
    Buffer *buffer;

    assert (stream != NULL);

    if (!stream_init (stream)) {
        return AM_FALSE;
    }

    buffer = (Buffer*) calloc (1, sizeof (Buffer));
    if (buffer == NULL) {
        return AM_FALSE;
    }

    stream->data = (void*) buffer;
    stream->readFunction  = memory_read_function;
    stream->writeFunction = memory_write_function;
    stream->seekFunction  = memory_seek_function;
    stream->tellFunction  = memory_tell_function;
    stream->closeFunction = memory_close_function_2; /* Высвобождает! */

    return AM_TRUE;
}

/**
 * Предполагается, память будут только читать.
 *
 * @param stream
 * @param data
 * @param length
 * @return
 */
MAGNA_API am_bool MAGNA_CALL memory_stream_open
    (
        Stream *stream,
        am_byte *data,
        am_size_t length
    )
{
    Buffer *buffer;

    assert (stream != NULL);
    assert (data != NULL);

    if (!stream_init (stream)) {
        return AM_FALSE;
    }

    buffer = (Buffer*) calloc (1, sizeof (Buffer));
    if (buffer == NULL) {
        return AM_FALSE;
    }

    stream->data = (void*) buffer;
    buffer_static (buffer, data, length);
    stream->readFunction  = memory_read_function;
    stream->writeFunction = memory_write_function;
    stream->seekFunction  = memory_seek_function;
    stream->tellFunction  = memory_tell_function;
    stream->closeFunction = memory_close_function_1; /* Не высвобождает! */

    return AM_TRUE;
}

/**
 * Получение фрагмента памяти.
 *
 * @param stream
 * @return
 */
MAGNA_API Span MAGNA_CALL memory_stream_to_span
    (
        const Stream *stream
    )
{
    Buffer *buffer;

    assert (stream != NULL);

    buffer = (Buffer *) stream->data;
    assert (buffer != NULL);

    return buffer_to_span (buffer);
}

MAGNA_API am_byte* MAGNA_CALL memory_stream_to_text
    (
        Stream *stream
    )
{
    Buffer *buffer;

    assert (stream != NULL);

    buffer = (Buffer *) stream->data;
    assert (buffer != NULL);

    return (am_byte*) buffer_to_text (buffer);
}

/*=========================================================*/

/* Поток, выдающий ошибки (для отладки) */

MAGNA_API am_ssize_t MAGNA_CALL broken_read_function
    (
        Stream *stream,
        am_byte *buffer, /* NOLINT(readability-non-const-parameter) */
        am_size_t length
    )
{
    (void) stream;
    (void) buffer;
    (void) length;

    return -1;
}

MAGNA_API am_ssize_t MAGNA_CALL broken_write_function
    (
        Stream *stream,
        const am_byte *buffer,
        am_size_t length
    )
{
    (void) stream;
    (void) buffer;
    (void) length;

    return -1;
}

MAGNA_API am_ssize_t MAGNA_CALL broken_seek_function
    (
        Stream *stream,
        am_size_t position
    )
{
    (void) stream;
    (void) position;

    return -1;
}

MAGNA_API am_ssize_t MAGNA_CALL broken_tell_function
    (
        Stream *stream
    )
{
    (void) stream;

    return -1;
}

MAGNA_API am_bool MAGNA_CALL broken_close_function
    (
        Stream *stream
    )
{
    (void) stream;

    return AM_FALSE;
}

MAGNA_API am_bool MAGNA_CALL broken_stream_open
    (
        Stream *stream
    )
{
    assert (stream != NULL);

    if (!stream_init (stream)) {
        return AM_FALSE;
    }

    stream->readFunction  = broken_read_function;
    stream->writeFunction = broken_write_function;
    stream->seekFunction  = broken_seek_function;
    stream->tellFunction  = broken_tell_function;
    stream->closeFunction = broken_close_function;

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/

