/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#ifdef MAGNA_WINDOWS

#include <windows.h>

#elif defined(MAGNA_MSDOS)

#include <stdio.h>
#include <fcntl.h>
#include <io.h>

typdef unsigned mode_t;

#define S_IRUSR 0
#define S_IWUSR 0
#define S_IRGRP 0
#define S_IWGRP 0

#else

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#endif

#include <assert.h>

/*=========================================================*/

/*
 * \file file.c
 *
 * Работа с файлами.
 */

/*=========================================================*/

/**
 * Создание файла с указанным именем.
 * Если файл с таким именем уже существует, он затирается.
 *
 * @param fileName Имя файла в кодировке, принятой в системе.
 * @return Дескриптор файла либо `AM_BAD_HANDLE`.
 */
MAGNA_API am_handle MAGNA_CALL file_create
    (
        const char *fileName
    )
{
    am_handle result;

#ifdef MAGNA_WINDOWS

    DWORD desiredAccess = GENERIC_READ | GENERIC_WRITE; /* NOLINT(hicpp-signed-bitwise) */
    DWORD shareMode = 0;
    LPSECURITY_ATTRIBUTES securityAttributes = NULL;
    DWORD creationDisposition = CREATE_ALWAYS;
    DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE templateFile = NULL;

    assert (fileName != NULL);

    result = CreateFileA
        (
            fileName,
            desiredAccess,
            shareMode,
            securityAttributes,
            creationDisposition,
            flagsAndAttributes,
            templateFile
        );

#else

    int flags = O_RDWR | O_CREAT | O_TRUNC;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;

    assert (fileName != NULL);

#if defined (MAGNA_APPLE) || defined (MAGNA_FREEBSD) || defined (MAGNA_ANDROID)

    result = open (fileName, flags, mode);

#else

    result = open64 (fileName, flags, mode);

#endif

#endif

    return result;
}

/**
 * Открытие существующего файла только для чтения.
 *
 * @param fileName Имя файла в принятой в системе кодировке.
 * @return Дескриптор файла либо `AM_BAD_HANDLE`.
 */
MAGNA_API am_handle MAGNA_CALL file_open_read
    (
        const char *fileName
    )
{
    am_handle result;

#ifdef MAGNA_WINDOWS

    DWORD desiredAccess = GENERIC_READ;
    DWORD shareMode = 0;
    LPSECURITY_ATTRIBUTES securityAttributes = NULL;
    DWORD creationDisposition = OPEN_EXISTING;
    DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE templateFile = NULL;

    assert (fileName != NULL);

    result = CreateFileA
        (
            fileName,
            desiredAccess,
            shareMode,
            securityAttributes,
            creationDisposition,
            flagsAndAttributes,
            templateFile
        );

#else

    int flags = O_RDONLY;

    assert (fileName != NULL);

#if defined (MAGNA_APPLE) || defined (MAGNA_FREEBSD) || defined (MAGNA_ANDROID)

    result = open (fileName, flags);

#else

    result = open64 (fileName, flags);

#endif

#endif

    return result;
}

/**
 * Открытие уже существующего файла для чтения/записи.
 *
 * @param fileName Имя файла в принятой в системе кодировке.
 * @return Дескриптор файла либо `AM_BAD_HANDLE`.
 */
MAGNA_API am_handle MAGNA_CALL file_open_write
    (
        const char *fileName
    )
{
    am_handle result;

#ifdef MAGNA_WINDOWS

    DWORD desiredAccess = GENERIC_READ | GENERIC_WRITE; /* NOLINT(hicpp-signed-bitwise) */
    DWORD shareMode = 0;
    LPSECURITY_ATTRIBUTES securityAttributes = NULL;
    DWORD creationDisposition = OPEN_EXISTING;
    DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE templateFile = NULL;

    assert (fileName != NULL);

    result = CreateFileA
        (
            fileName,
            desiredAccess,
            shareMode,
            securityAttributes,
            creationDisposition,
            flagsAndAttributes,
            templateFile
        );

#else

    int flags = O_RDWR;

    assert (fileName != NULL);

#if defined (MAGNA_APPLE) || defined (MAGNA_FREEBSD) || defined (MAGNA_ANDROID)

    result = open (fileName, flags);

#else

    result = open64 (fileName, flags);

#endif

#endif

    return result;
}

MAGNA_API am_bool MAGNA_CALL file_close
    (
        am_handle handle
    )
{
    assert (handle != AM_BAD_HANDLE);

#ifdef MAGNA_WINDOWS

    CloseHandle (handle);

#else

    close (handle);

#endif

    return AM_TRUE;
}

/**
 * Проверка, не достигнут ли конец файла.
 *
 * @param handle Файловый дескриптор.
 * @return `AM_TRUE` если достигнут.
 */
MAGNA_API am_bool MAGNA_CALL file_eof
    (
        am_handle handle
    )
{
    assert (handle != AM_BAD_HANDLE);

    return AM_FALSE;
}

/**
 * Чтение данных из файла.
 *
 * @param handle Дескриптор файла.
 * @param buffer Буфер, в который должен быть помещен результат.
 * @param size Количество считываемых байт.
 * @return Количество прочитанных байт, либо отрицательное значение.
 */
MAGNA_API am_ssize_t MAGNA_CALL file_read
    (
        am_handle handle,
        am_byte *buffer,
        am_ssize_t size
    )
{
    am_size_t result;

#ifdef MAGNA_WINDOWS

    DWORD numberOfBytesRead;
    BOOL rc;

    assert (handle != AM_BAD_HANDLE);
    assert (buffer != NULL);

    rc = ReadFile
        (
            handle,
            buffer,
            size,
            &numberOfBytesRead,
            NULL
        );

    result = rc ? numberOfBytesRead : -1;

#else

    assert (handle != AM_BAD_HANDLE);
    assert (buffer != NULL);

    result = read (handle, buffer, size);

#endif

    return result;
}

/**
 * Чтение одного байта из файла.
 *
 * @param handle Дескриптор файла.
 * @return Прочитанный байт либо -1.
 */
MAGNA_API int MAGNA_CALL file_read_byte
    (
        am_handle handle
    )
{
    am_byte result;
    am_ssize_t rc;

    assert (handle != AM_BAD_HANDLE);

    rc = file_read (handle, &result, 1);

    return (rc == 1 ? result : -1);
}

/**
 * Считывание всего остатка файла в оперативную память.
 *
 * @param handle Файловый дескриптор.
 * @param buffer Буфер, в который должен быть помещен результат.
 * @return Количество прочитанных байт либо отрицательное значение.
 */
MAGNA_API am_ssize_t MAGNA_CALL file_read_all
    (
        am_handle handle,
        Buffer *buffer
    )
{
    assert (handle != AM_BAD_HANDLE);
    assert (buffer != NULL);

    return -1;
}

MAGNA_API am_uint32 MAGNA_CALL file_read_int_32
    (
        am_handle handle
    )
{
    assert (handle != AM_BAD_HANDLE);

    return 0;
}

MAGNA_API am_uint64 MAGNA_CALL file_read_int_64
    (
        am_handle handle
    )
{
    assert (handle != AM_BAD_HANDLE);

    return 0;
}

/**
 * Чтение строки из файла в нативной кодировке.
 * Символ перевода строки считывается,
 * но не помещается в результирующую строку.
 *
 * @param handle Файловый дескриптор.
 * @param buffer Буфер, в который должна быть помещена строка.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_read_line
    (
        am_handle handle,
        Buffer *buffer
    )
{
    int c;

    assert (handle != AM_BAD_HANDLE);
    assert (buffer != NULL);

    while (AM_TRUE) {
        c = file_read_byte (handle);
        if (c < 0) {
            break;
        }

        if (c == '\r') {
            c = file_read_byte (handle);
            if (c == '\n' || c < 0) {
                break;
            }
            if (!buffer_putc (buffer, c)) {
                return AM_FALSE;
            }
        }
        else if (c == '\n') {
            break;
        }
        else {
            if (!buffer_putc (buffer, c)) {
                return AM_FALSE;
            }
        }
    }

    return AM_TRUE;
}

MAGNA_API am_int64 MAGNA_CALL file_tell
    (
        am_handle handle
    )
{
    assert (handle != AM_BAD_HANDLE);

    return 0;
}

MAGNA_API am_uint64 MAGNA_CALL file_size
    (
        am_handle handle
    )
{
    assert (handle != AM_BAD_HANDLE);

    return 0;
}

MAGNA_API am_bool MAGNA_CALL file_seek
    (
        am_handle handle,
        am_int64 offset
    )
{
    assert (handle != AM_BAD_HANDLE);

    return AM_FALSE;
}

/**
 * Запись данных в файл.
 *
 * @param handle Файловый дескриптор.
 * @param data Указатель на данные.
 * @param size Размер данных в байтах.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_write
    (
        am_handle handle,
        const am_byte *data,
        am_size_t size
    )
{
    assert (handle != AM_BAD_HANDLE);
    assert (data != NULL);

    return AM_FALSE;
}

/**
 * Запись байта в файл.
 *
 * @param handle Файловый дескриптор.
 * @param value Записываемое значение.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_write_byte
    (
        am_handle handle,
        am_byte value
    )
{
    assert (handle != AM_BAD_HANDLE);

    return AM_FALSE;
}

/**
 * Запись беззнакового 32-битного целого в сетевом формате.
 *
 * @param handle Файловый дескриптор.
 * @param value Записываемое значение.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_write_int_32
    (
        am_handle handle,
        am_uint32 value
    )
{
    assert (handle != AM_BAD_HANDLE);

    return AM_FALSE;
}

/**
 * Запись беззнакового 64-битного целого в сетевом формате.
 *
 * @param handle Файловый дескриптор.
 * @param value Записываемое значение.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_write_int_64
    (
        am_handle handle,
        am_uint64 value
    )
{
    assert (handle != AM_BAD_HANDLE);

    return AM_FALSE;
}

/**
 * Запись в файл содержимого буфера.
 *
 * @param handle Файловый дескриптор.
 * @param buffer Буфер, содержимое которого должно быть записано.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_write_buffer
    (
        am_handle handle,
        const Buffer *buffer
    )
{
    am_bool result;

    assert (handle != AM_BAD_HANDLE);
    assert (buffer != NULL);

    result = file_write
        (
            handle,
            buffer->ptr,
            buffer->position
        );

    return result;
}

/**
 * Запись в файл содержимого фрагмента.
 *
 * @param handle Файловый дескриптор.
 * @param buffer Фрагмент, содержимое которого должно быть записано.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_write_span
    (
        am_handle handle,
        const Span span
    )
{
    am_bool result;

    assert (handle != AM_BAD_HANDLE);

    result = file_write
        (
            handle,
            span.ptr,
            span.len
        );

    return result;
}

/**
 * Запись в файл текста.
 *
 * @param handle Файловый дескриптор.
 * @param text Текст в нативной кодировке.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_write_text
    (
        am_handle handle,
        const char *text
    )
{
    am_bool result;

    assert (handle != AM_BAD_HANDLE);

    result = file_write
        (
            handle,
            text,
            strlen (text)
        );

    return result;
}

/*=========================================================*/

/* Файловый поток */

MAGNA_API am_ssize_t MAGNA_CALL file_read_function
    (
        Stream *stream,
        am_byte *buffer,
        am_size_t length
    )
{
    am_handle handle;

    assert (stream != NULL);

    handle = (am_handle) stream->data;
    assert (handle != AM_BAD_HANDLE);

    return file_read (handle, buffer, length);
}

MAGNA_API am_ssize_t MAGNA_CALL file_write_function
    (
        Stream *stream,
        const am_byte *buffer,
        am_size_t length
    )
{
    am_handle handle;

    assert (stream != NULL);

    handle = (am_handle) stream->data;
    assert (handle != AM_BAD_HANDLE);

    return file_write (handle, buffer, length) ? length : -1;
}

MAGNA_API am_ssize_t MAGNA_CALL file_seek_function
    (
        Stream *stream,
        am_size_t position
    )
{
    am_handle handle;

    assert (stream != NULL);

    handle = (am_handle) stream->data;
    assert (handle != AM_BAD_HANDLE);

    return file_seek (handle, position) ? (am_ssize_t) position : -1;
}

MAGNA_API am_ssize_t MAGNA_CALL file_tell_function
    (
        Stream *stream
    )
{
    am_handle handle;

    assert (stream != NULL);

    handle = (am_handle) stream->data;
    assert (handle != AM_BAD_HANDLE);

    return (am_ssize_t) file_tell (handle);
}

MAGNA_API am_bool MAGNA_CALL file_close_function
    (
        Stream *stream
    )
{
    am_handle handle;

    assert (stream != NULL);

    handle = (am_handle) stream->data;
    if (handle != AM_BAD_HANDLE) {
        file_close (handle);
        stream->data = (void*) AM_BAD_HANDLE;
    }

    return AM_TRUE;
}

static am_bool initialize
    (
        Stream *stream,
        am_handle handle
    )
{
    if (handle == AM_BAD_HANDLE) {
        return AM_FALSE;
    }

    if (!stream_init (stream)) {
        file_close (handle);
        return AM_FALSE;
    }

    stream->data = (void*) handle;
    stream->readFunction  = file_read_function;
    stream->writeFunction = file_write_function;
    stream->seekFunction  = file_seek_function;
    stream->tellFunction  = file_tell_function;
    stream->closeFunction = file_close_function;

    return AM_TRUE;
}

MAGNA_API am_bool MAGNA_CALL file_stream_create
    (
        Stream *stream,
        const char *filename
    )
{
    assert (stream != NULL);

    if (!initialize (stream, file_create (filename))) {
        return AM_FALSE;
    }

    return AM_TRUE;
}

MAGNA_API am_bool MAGNA_CALL file_stream_open_read
    (
        Stream *stream,
        const char *filename
    )
{
    assert (stream != NULL);

    if (!initialize (stream, file_open_read (filename))) {
        return AM_FALSE;
    }

    return AM_TRUE;
}

MAGNA_API am_bool MAGNA_CALL file_stream_open_write
    (
        Stream *stream,
        const char *filename
    )
{
    assert (stream != NULL);

    if (!initialize (stream, file_open_write (filename))) {
        return AM_FALSE;
    }

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
