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

#ifdef  MAGNA_WINDOWS

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>
#include <io.h>

#pragma comment (lib, "ws2_32.lib") // for ntohl/htonl

#else

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>

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

    if (!GetCurrentDirectoryA (FILENAME_MAX, temporary)) {
        return AM_FALSE;
    }

#else

    if (!getcwd (temporary, sizeof (temporary))) {
        return AM_FALSE;
    }

#endif

    buffer_assign_text (path, temporary);

    return AM_TRUE;
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

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
