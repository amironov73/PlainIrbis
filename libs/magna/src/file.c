// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#ifdef MAGNA_WINDOWS

#include <windows.h>

#elif defined (MAGNA_MSDOS)

#include <stdio.h>
#include <io.h>

typdef unsigned mode_t;

#define S_IRUSR 0
#define S_IWUSR 0
#define S_IRGRP 0
#define S_IWGRP 0

#else

#include <unistd.h>
#include <sys/types.h>

#endif

#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef S_IFDIR
#define	S_IFDIR	0040000	/* Directory.  */
#endif

#ifndef S_IFREG
#define	S_IFREG	0100000	/* Regular file.  */
#endif

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

    result.pointer = CreateFileA
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

    result.value = open (fileName, flags, mode);

#else

    result.value = open64 (fileName, flags, mode);

#endif

#endif

    return result;
}

/**
 * Настойчивое создание файла с указанным именем.
 *
 * @param fileName
 * @param delay
 * @param retryLimit
 * @return
 */
MAGNA_API am_handle MAGNA_CALL file_create_insist
    (
        const char *fileName,
        unsigned int delay,
        unsigned int retryLimit
    )
{
    am_handle result = handle_get_bad ();
    int count;

    assert (fileName != NULL);
    assert (delay > 0);
    assert (retryLimit > 0);

    for (count = 0; count < retryLimit; ++count) {
        result = file_create (fileName);
        if (handle_is_good (result)) {
            break;
        }

        magna_sleep (delay);
    }

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

    result.pointer = CreateFileA
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

    result.value = open (fileName, flags);

#else

    result.value = open64 (fileName, flags);

#endif

#endif

    return result;
}

/**
 * Настойчивое открытие файла для чтения.
 *
 * @param fileName
 * @param delay
 * @param retryLimit
 * @return
 */
MAGNA_API am_handle MAGNA_CALL file_open_read_insist
    (
        const char *fileName,
        unsigned int delay,
        unsigned int retryLimit
    )
{
    am_handle result = handle_get_bad ();
    int count;

    assert (fileName != NULL);
    assert (delay > 0);
    assert (retryLimit > 0);

    for (count = 0; count < retryLimit; ++count) {
        result = file_open_read (fileName);
        if (handle_is_good (result)) {
            break;
        }

        magna_sleep (delay);
    }

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

    result.pointer = CreateFileA
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

    result.value = open (fileName, flags);

#else

    result.value = open64 (fileName, flags);

#endif

#endif

    return result;
}

/**
 * Настойчивое открытие файла для записи.
 *
 * @param fileName
 * @param delay
 * @param retryLimit
 * @return
 */
MAGNA_API am_handle MAGNA_CALL file_open_write_insist
    (
        const char *fileName,
        unsigned int delay,
        unsigned int retryLimit
    )
{
    am_handle result = handle_get_bad ();
    int count;

    assert (fileName != NULL);
    assert (delay > 0);
    assert (retryLimit > 0);

    for (count = 0; count < retryLimit; ++count) {
        result = file_open_write (fileName);
        if (handle_is_good (result)) {
            return result;
        }

        magna_sleep (delay);
    }

    return result;
}

/**
 * Закрытие файла.
 *
 * @param handle
 * @return
 */
MAGNA_API am_bool MAGNA_CALL file_close
    (
        am_handle handle
    )
{
    assert (handle_is_good (handle));

#ifdef MAGNA_WINDOWS

    CloseHandle (handle.pointer);

#else

    close (handle.value);

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
    assert (handle_is_good (handle));

    return file_tell (handle) >= (am_int64) file_size (handle);
}

/**
 * Чтение данных из файла.
 *
 * @param handle Дескриптор файла.
 * @param buffer Буфер, в который должен быть помещен результат.
 * @param size Количество считываемых байт.
 * @return Количество прочитанных байт, либо отрицательное значение.
 */
MAGNA_API ssize_t MAGNA_CALL file_read
    (
        am_handle handle,
        am_byte *buffer,
        ssize_t size
    )
{
    size_t result;

#ifdef MAGNA_WINDOWS

    DWORD numberOfBytesRead;
    BOOL rc;

    assert (handle_is_good (handle));
    assert (buffer != NULL);

    rc = ReadFile
        (
            handle.pointer,
            buffer,
            (DWORD) size,
            &numberOfBytesRead,
            NULL
        );

    result = rc ? numberOfBytesRead : -1;

#else

    assert (handle_is_good (handle));
    assert (buffer != NULL);

    result = read (handle.value, buffer, size);

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
    ssize_t rc;

    assert (handle_is_good (handle));

    rc = file_read (handle, &result, 1);

    return (rc == 1 ? result : -1);
}

/**
 * Считывание всего файла в оперативную память.
 *
 * @param fileName Имя файла.
 * @param buffer Буфер, в который должен быть помещен результат.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_read_all
    (
        const char *fileName,
        Buffer *buffer
    )
{
    am_handle handle;
    ssize_t rc;
    size_t totalLength;
    am_byte temp [1024];

    assert (fileName != NULL);
    assert (buffer != NULL);

    handle = file_open_read (fileName);
    if (!handle_is_good (handle)) {
        return AM_FALSE;
    }

    totalLength = (size_t) file_size (handle);
    if (!buffer_grow (buffer, totalLength)) {
        file_close (handle);
        return AM_FALSE;
    }

    while (AM_TRUE) {
        rc = file_read (handle, temp, sizeof (temp));
        if (rc < 0) {
            file_close (handle);
            return AM_FALSE;
        }

        if (rc == 0) {
            break;
        }

        if (!buffer_write (buffer, temp, (size_t) rc)) {
            file_close (handle);
            return AM_FALSE;
        }
    }

    file_close (handle);

    return AM_TRUE;
}

/**
 * Чтение беззнакового 32-битного целого в сетевом формате.
 *
 * @param handle Файловый дескриптор.
 * @return Прочитанное целое.
 */
MAGNA_API am_uint32 MAGNA_CALL file_read_int32
    (
        am_handle handle
    )
{
    am_uint32 result;

    assert (handle_is_good (handle));

    (void) file_read (handle, (am_byte*) &result, sizeof (result));
    result = magna_ntohl (result);

    return result;
}

/**
 * Чтение беззнакового 64-битного целого в сетевом формате.
 *
 * @param handle Файловый дескриптор.
 * @return Прочитанное целое.
 */
MAGNA_API am_uint64 MAGNA_CALL file_read_int64
    (
        am_handle handle
    )
{
    am_uint32 numbers[2];
    am_uint64 result;

    assert (handle_is_good (handle));

    (void) file_read (handle, (am_byte*) numbers, sizeof (numbers));
    numbers[0] = magna_ntohl (numbers[0]);
    numbers[1] = magna_ntohl (numbers[1]);
    result = (((am_uint64) numbers[1]) << 32u) + ((am_uint64) numbers[0]);

    return result;
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

    assert (handle_is_good (handle));
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
            if (!buffer_putc (buffer, (char) c)) {
                return AM_FALSE;
            }
        }
        else if (c == '\n') {
            break;
        }
        else {
            if (!buffer_putc (buffer, (char) c)) {
                return AM_FALSE;
            }
        }
    }

    return AM_TRUE;
}

/**
 * Получение текущего положения указателя файла.
 *
 * @param handle Файловый дескриптор.
 * @return Смещение от начала в байтах.
 */
MAGNA_API am_int64 MAGNA_CALL file_tell
    (
        am_handle handle
    )
{
#ifdef MAGNA_WINDOWS

    LARGE_INTEGER distance;
    LARGE_INTEGER result;

    assert (handle_is_good (handle));

    distance.QuadPart = 0;
    if (!SetFilePointerEx (handle.pointer, distance, &result, FILE_CURRENT)) {
        return -1;
    }

    return result.QuadPart;

#elif defined (MAGNA_LINUX)

    assert (handle_is_good (handle));

    return lseek64 (handle.value, 0, SEEK_CUR);

#elif defined (MAGNA_UNIX)

    assert (handle_is_good (handle));

    return lseek (handle.value, 0, SEEK_CUR);

#else

    return MAGNA_INT64 (-1);

#endif
}

/**
 * Определение размера файла в байтах.
 *
 * @param handle Файловый дескриптор.
 * @return Размер файла в байтах.
 */
MAGNA_API am_uint64 MAGNA_CALL file_size
    (
        am_handle handle
    )
{
#ifdef MAGNA_WINDOWS

    LARGE_INTEGER result;

    assert (handle_is_good (handle));

    if (!GetFileSizeEx (handle.pointer, &result)) {
        return (am_uint64) (MAGNA_INT64 (-1));
    }

    return (am_uint64) result.QuadPart;

/*
#elif defined(MAGNA_LINUX)

    struct stat64 buf;

    assert (handle_is_good (handle));

    mem_clear (&buf, sizeof (buf));
    if (fstat (handle.value, &buf) < 0) {
        return MAGNA_INT64 (-1);
    }

    return buf.st_size;
*/

#elif defined(MAGNA_UNIX)

    struct stat buf;

    assert (handle_is_good (handle));

    mem_clear (&buf, sizeof (buf));
    if (fstat (handle.value, &buf) < 0) {
        return MAGNA_INT64 (-1);
    }

    return buf.st_size;

#else

    return MAGNA_INT64 (-1);

#endif
}

/**
 * Перемещение указателя файла.
 *
 * @param handle Файловый дескриптор.
 * @param offset Смещение в байтах от начала файла.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_seek
    (
        am_handle handle,
        am_int64 offset
    )
{
#ifdef MAGNA_WINDOWS

    LARGE_INTEGER largeInteger;

    assert (handle_is_good (handle));

    largeInteger.QuadPart = offset;
    if (!SetFilePointerEx (handle.pointer, largeInteger, NULL, FILE_BEGIN)) {
        return MAGNA_INT64 (-1);
    }

    return AM_TRUE;

#elif defined(MAGNA_LINUX)

    assert (handle_is_good (handle));

    return lseek64 (handle.value, offset, SEEK_SET) != ((off_t) (-1));

#elif defined(MAGNA_UNIX)

    assert (handle_is_good (handle));

    return lseek (handle.value, offset, SEEK_SET) != ((off_t) (-1));

#else

    return AM_FALSE;

#endif
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
        size_t size
    )
{
#ifdef MAGNA_WINDOWS

    DWORD written;
    BOOL rc;

    assert (handle_is_good (handle));
    assert (data != NULL);

    rc = WriteFile
        (
            handle.pointer,
            data,
            (DWORD) size,
            &written,
            NULL
        );

    if (!rc) {
        return AM_FALSE;
    }

    return ((size_t) written) == size;

#else

    am_int64 result;

    assert (handle_is_good (handle));
    assert (data != NULL);

    result = write (handle.value, data, size);

    return ((size_t)result) == size;

#endif
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
    assert (handle_is_good (handle));

    return file_write (handle, &value, 1);
}

/**
 * Запись беззнакового 32-битного целого в сетевом формате.
 *
 * @param handle Файловый дескриптор.
 * @param value Записываемое значение.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_write_int32
    (
        am_handle handle,
        am_uint32 value
    )
{
    assert (handle_is_good (handle));

    value = magna_ntohl (value);

    return file_write (handle, (am_byte*) &value, sizeof (value));
}

/**
 * Запись беззнакового 64-битного целого в сетевом формате.
 *
 * @param handle Файловый дескриптор.
 * @param value Записываемое значение.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_write_int64
    (
        am_handle handle,
        am_uint64 value
    )
{
    am_uint32 parts [2];

    assert (handle_is_good (handle));

    parts[0] = magna_ntohl ((am_uint32) (value & MAGNA_UINT64 (0xFFFFFFFF)));
    parts[1] = magna_ntohl ((am_uint32) ((value >> 32u) & MAGNA_UINT64 (0xFFFFFFFF)));

    return file_write (handle, (am_byte*) parts, sizeof (parts));
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

    assert (handle_is_good (handle));
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
        Span span
    )
{
    am_bool result;

    assert (handle_is_good (handle));

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

    assert (handle_is_good (handle));

    result = file_write
        (
            handle,
            text,
            strlen (text)
        );

    return result;
}

/**
 * Проверяет, существует ли указанный файл.
 *
 * @param filename Имя файла в нативной кодировке.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL file_exist
    (
        const char *filename
    )
{
#ifdef MAGNA_WINDOWS

    DWORD rc;

    assert (filename != NULL);

    rc = GetFileAttributes (filename);
    if (rc == INVALID_FILE_ATTRIBUTES) {
        return AM_FALSE;
    }

    return (rc & (DWORD) FILE_ATTRIBUTE_DIRECTORY) == 0u;

#elif defined (MAGNA_UNIX)

    struct stat info;

    assert (filename != NULL);

    mem_clear (&info, sizeof (info));
    if (stat (filename, &info)) {
        return AM_FALSE;
    }

    return (info.st_mode & S_IFREG) != 0;

#else

    return 0;

#endif
}

/**
 * Удаление файла с указанным именем.
 *
 * @param filename Имя файла в нативной кодировке.
 * @return Признак успешности завершения операции.
 * Если файла не существует, считается, что функция отработала успешно.
 */
MAGNA_API am_bool MAGNA_CALL file_delete
    (
        const char *filename
    )
{
    if (!file_exist (filename)) {
        return AM_TRUE;
    }

#ifdef MAGNA_WINDOWS

    return DeleteFileA (filename);

#elif defined (MAGNA_UNIX)

    return remove (filename) >= 0;

#else

    return AM_FALSE;

#endif
}

/**
 * Проверяет, существует ли указанная директория.
 *
 * @param filename Имя директории в нативной кодировке.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL directory_exist
    (
        const char *dirname
    )
{
#ifdef MAGNA_WINDOWS

    DWORD rc;

    assert (dirname != NULL);

    rc = GetFileAttributes (dirname);
    if (rc == INVALID_FILE_ATTRIBUTES) {
        return AM_FALSE;
    }

    return (rc & (DWORD) FILE_ATTRIBUTE_DIRECTORY) != 0u;

#elif defined (MAGNA_UNIX)

    struct stat info;

    assert (dirname != NULL);

    mem_clear (&info, sizeof (info));
    if (stat (dirname, &info)) {
        return AM_FALSE;
    }

    return (info.st_mode & S_IFDIR) != 0;

#else

    return 0;

#endif
}

MAGNA_API am_bool MAGNA_CALL directory_create
    (
        const char *dirname,
        am_bool createNew
    )
{
    if (directory_exist (dirname)) {
        return !createNew;
    }

#ifdef MAGNA_WINDOWS

    assert (dirname != NULL);

    return CreateDirectoryA (dirname, NULL);

#elif defined (MAGNA_UNIX)

    assert (dirname != NULL);

    return mkdir (dirname, 0755) >= 0;

#else

    return AM_FALSE;

#endif
}

MAGNA_API am_bool MAGNA_CALL directory_delete
    (
        const char *dirname
    )
{
    if (!directory_exist (dirname)) {
        return AM_TRUE;
    }

#ifdef MAGNA_WINDOWS

    assert (dirname != NULL);

    return RemoveDirectoryA (dirname);

#elif defined (MAGNA_UNIX)

    return rmdir (dirname) >= 0;

#else

    return AM_FALSE;

#endif
}

/**
 * Простое копирование файла.
 *
 * @param targetName
 * @param sourceName
 * @return
 */
MAGNA_API am_bool MAGNA_CALL file_copy
    (
        const char *targetName,
        const char *sourceName
    )
{
#ifdef MAGNA_WINDOWS

    return CopyFileA (sourceName, targetName, AM_FALSE);

#else

    am_handle targetHandle, sourceHandle;
    am_byte buffer [1024];
    size_t rc;

    assert (targetName != NULL);
    assert (sourceName != NULL);

    sourceHandle = file_open_read (sourceName);
    if (!handle_is_good (sourceHandle)) {
        return AM_FALSE;
    }

    targetHandle = file_create (targetName);
    if (!handle_is_good (targetHandle)) {
        file_close (sourceHandle);
        return AM_FALSE;
    }

    while (AM_TRUE) {
        rc = file_read (sourceHandle, buffer, sizeof (buffer));
        if (rc < 0) {
            file_close (sourceHandle);
            file_close (targetHandle);
            file_delete (targetName);
            return AM_FALSE;
        }

        if (rc == 0) {
            break;
        }

        if (!file_write (targetHandle, buffer, rc)) {
            file_close (sourceHandle);
            file_close (targetHandle);
            file_delete (targetName);
            return AM_FALSE;
        }
    }

    file_close (sourceHandle);
    file_close (targetHandle);

    return AM_TRUE;

#endif
}

/**
 * Синхронизирует состояние файла в памяти с диском.
 *
 * @param handle Файловый дескриптор.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_sync
    (
        am_handle handle
    )
{
#ifdef MAGNA_WINDOWS

    assert (handle_is_good (handle));

    return FlushFileBuffers (handle.pointer);

#elif defined (MAGNA_UNIX)

    assert (handle_is_good (handle));

    return fsync (handle.value) == 0;

#else

    return AM_FALSE;

#endif
}

/*=========================================================*/

/* Файловый поток */

MAGNA_API ssize_t MAGNA_CALL file_read_function
    (
        Stream *stream,
        am_byte *buffer,
        size_t length
    )
{
    am_handle handle;

    assert (stream != NULL);

    handle = *(am_handle*) &stream->data;
    assert (handle_is_good (handle));

    return file_read (handle, buffer, length);
}

MAGNA_API ssize_t MAGNA_CALL file_write_function
    (
        Stream *stream,
        const am_byte *buffer,
        size_t length
    )
{
    am_handle handle;

    assert (stream != NULL);

    handle = *(am_handle*) &stream->data;
    assert (handle_is_good (handle));

    return file_write (handle, buffer, length) ? length : -1;
}

MAGNA_API ssize_t MAGNA_CALL file_seek_function
    (
        Stream *stream,
        size_t position
    )
{
    am_handle handle;

    assert (stream != NULL);

    handle = *(am_handle*) &stream->data;
    assert (handle_is_good (handle));

    return file_seek (handle, position) ? (ssize_t) position : -1;
}

MAGNA_API ssize_t MAGNA_CALL file_tell_function
    (
        Stream *stream
    )
{
    am_handle handle;

    assert (stream != NULL);

    handle = *(am_handle*) &stream->data;
    assert (handle_is_good (handle));

    return (ssize_t) file_tell (handle);
}

MAGNA_API am_bool MAGNA_CALL file_close_function
    (
        Stream *stream
    )
{
    am_handle handle;

    assert (stream != NULL);

    handle = *(am_handle*) &stream->data;
    if (handle_is_good (handle)) {
        file_close (handle);
#ifdef MAGNA_WINDOWS
        handle.pointer = AM_BAD_HANDLE;
#else
        handle.value = AM_BAD_HANDLE;
#endif
        stream->data = *(void**) &handle;
    }

    return AM_TRUE;
}

static am_bool initialize
    (
        Stream *stream,
        am_handle handle
    )
{
    if (!handle_is_good (handle)) {
        return AM_FALSE;
    }

    if (!stream_init (stream)) {
        file_close (handle);
        return AM_FALSE;
    }

    stream->data = handle.pointer;
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

/**
 * Открывает файл для построчного ввода.
 *
 * @param texter Указатель на неинициализированную структуру.
 * @param stream Указатель на неинициализированную структуру.
 * @param filename Имя файла.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL file_texter
    (
        StreamTexter *texter,
        Stream *stream,
        const char *filename
    )
{
    assert (texter != NULL);
    assert (filename != NULL);

    if (!file_stream_open_read (stream, filename)) {
        return AM_FALSE;
    }

    return texter_init (texter, stream, 0);
}

MAGNA_API am_bool MAGNA_CALL file_touch
    (
        const char *filename
    )
{
    am_handle handle;

    assert (filename != NULL);

    handle = file_exist (filename)
        ? file_open_write (filename)
        : file_create (filename);
    if (!handle_is_good (handle)) {
        return AM_FALSE;
    }

    file_close (handle);

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
