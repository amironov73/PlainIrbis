/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4820)
#pragma warning(disable: 5045)
#endif

/*=========================================================*/

/**
 * \file path.c
 *
 * Работа с путями.
 *
 */

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

#include <sys/stat.h>
#include <fcntl.h>

#ifdef MAGNA_WINDOWS

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>
#include <io.h>

#pragma comment (lib, "ws2_32.lib") // for ntohl/htonl

#elif defined(MAGNA_MSDOS)

#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#elif defined(MAGNA_APPLE)

#include <mach-o/dyld.h>
#include <unistd.h>

#else

#include <unistd.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <fcntl.h>

#endif

#include <assert.h>

/**
 * Получение текущей директории.
 *
 * @param path Буфер для результата.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL path_get_current_directory
    (
        Buffer *path
    )
{
    char temporary [FILENAME_MAX];

    assert (path != NULL);

    memset (temporary, 0, sizeof (temporary));

#ifdef MAGNA_WINDOWS

    if (!GetCurrentDirectoryA (sizeof (temporary), temporary)) {
        return AM_FALSE;
    }

#else

    if (!getcwd (temporary, sizeof (temporary))) {
        return AM_FALSE;
    }

#endif

    return buffer_assign_text (path, temporary);
}

/**
 * Смена текущей директории.
 *
 * @param path Буфер с новой директорией.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL path_set_current_directory
    (
        const Buffer *path
    )
{
    char temporary [FILENAME_MAX];

    assert (path != NULL);

    if (path->position >= FILENAME_MAX) {
        /* Слишком длинный путь */
        return AM_FALSE;
    }
    memset (temporary, 0, sizeof (temporary));
    memcpy (temporary, path->ptr, path->position);

#ifdef MAGNA_WINDOWS

    if (!SetCurrentDirectoryA (temporary)) {
        return AM_FALSE;
    }

#else

    if (chdir (temporary) < 0) {
        return AM_FALSE;
    }

#endif

    return AM_TRUE;
}

/**
 * Получение расширения для файла.
 *
 * @param path Путь к файлу (возможно, не полный).
 * @return Расширение (возможно, пустое).
 */
MAGNA_API Span MAGNA_CALL path_get_extension
    (
        const Buffer *path
    )
{
    Span result;
    char c;

    assert (path != NULL);

    if (path->position <= 2) {
        result.ptr = path->ptr;
        result.len = 0;
        return result;
    }

    result.ptr = path->ptr + path->position - 1;
    result.len = 0;

    while (result.ptr >= path->ptr) {
        c = *result.ptr;
        if (c == '.') {
            break;
        }

        if (c == '/' || c == '\\') {
            result.ptr = path->ptr;
            result.len = 0;
            break;
        }

        --result.ptr;
        ++result.len;
    }

    return result;
}

/**
 * Путь к исполняемому файлу.
 *
 * @param buffer Проинициализированный буфер.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL path_to_executable
    (
        Buffer *buffer
    )
{
#if defined(MAGNA_WINDOWS)

    char temp [MAX_PATH];

    assert (buffer != NULL);

    memset (temp, 0, sizeof (temp));
    if (!GetModuleFileNameA (NULL, temp, MAX_PATH)) {
        return AM_FALSE;
    }

    return buffer_puts (buffer, temp);

#elif defined(MAGNA_LINUX)

    char temp [PATH_MAX];

    assert (buffer != NULL);

    /* readlink does not null terminate! */
    memset (temp, 0, sizeof (temp));
    if (readlink ("/proc/self/exe", temp, PATH_MAX) == -1) {
        return AM_FALSE;
    }

    return buffer_puts (buffer, temp);

#elif defined(MAGNA_APPLE)

    char path[1024];
    uint32_t size = sizeof (path);

    assert (buffer != NULL);

    if (_NSGetExecutablePath(path, &size) != 0) {
        return AM_FALSE;
    }

    return buffer_puts (buffer, path);

#else

    (void)buffer;

    return AM_FALSE;

#endif
}

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
