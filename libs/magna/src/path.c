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
 * Получение пути директории для временных файлов.
 *
 * @param path Буфер для размещения результата.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL path_get_temporary_directory
    (
        Buffer *path
    )
{
#ifdef MAGNA_WINDOWS

    char buffer [FILENAME_MAX];

    assert (path != NULL);

    mem_clear (buffer, sizeof (buffer));
    if (!GetTempPathA(FILENAME_MAX, buffer)) {
        return AM_FALSE;
    }

    return buffer_puts (path, buffer);


#elif defined (MAGNA_UNIX)

    const char *result;

    assert (path != NULL);

    result = getenv("TMPDIR");
    if (!result) {
        result = getenv("TEMPDIR");
    }
    if (!result) {
        result = getenv("TMP");
    }
    if (!result) {
        result = getenv("TEMP");
    }
    if (!result) {
        /* The Filesystem Hierarchy Standard version 3.0 says:
           The /tmp directory must be made available for programs
           that require temporary files. */
        result = "/tmp";
    }

    return buffer_puts (path, result);

#else

    return AM_FALSE;

#endif
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
 * Получение имени файла (с расширением, если есть).
 *
 * @param path Путь к файлу (возможно, неполный).
 * @return Имя файла (возможно, пустое).
 */
MAGNA_API Span MAGNA_CALL path_get_filename
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
        if (c == '/' || c == '\\') {
            ++result.ptr;
            --result.len;
            break;
        }

        --result.ptr;
        ++result.len;
    }

    return result;
}

/**
 * Получение директории для указанного пути.
 *
 * @param path Путь (возможно, неполный).
 * @return Директория (возможно, пустой фрагмент).
 */
MAGNA_API Span MAGNA_CALL path_get_directory
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

    /* TODO: implement */

    return result;
}

/**
 * Превращает неправильные слэши в правильные.
 *
 * @param path Путь (возможно, пустой).
 * @return Обработанный путь.
 */
MAGNA_API Buffer* MAGNA_CALL path_convert_slashes
    (
        Buffer *path
    )
{
    am_byte *ptr, *end;

    assert (path != NULL);

    for (ptr = path->ptr, end = ptr + path->position; ptr < end; ++ptr) {

#if defined(MAGNA_WINDOWS) || defined(MAGNA_MSDOS)

        if (*ptr == '/') {
            *ptr = '\\';
        }

#else

        if (*ptr == '\\') {
            *ptr = '/';
        }

#endif
    }

    return path;
}

MAGNA_API am_bool path_combine
    (
        Buffer *result,
        ...
    )
{
    assert (result != NULL);

    return AM_FALSE;
}

/**
 * Путь к исполняемому файлу.
 *
 * @param buffer Проинициализированный буфер.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL path_get_executable
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

    char temp [PATH_MAX], temp2[30], *ptr;
    pid_t pid;

    assert (buffer != NULL);

    /* readlink does not null terminate! */
    memset (temp, 0, sizeof (temp));
    if (readlink ("/proc/self/exe", temp, PATH_MAX) == -1) {
        /* Может, procfs есть, но нет /proc/self */
        pid = getpid();
        sprintf (temp2, "/proc/%ld/exe", (long) pid);
        if (readlink (temp2, temp, PATH_MAX) == -1) {

            /* When Bash invokes an external command,
              the variable ‘$_’ is set to the full pathname
              of the command and passed to that command
              in its environment.*/

            ptr = getenv("_");
            if (!ptr) {
                return AM_FALSE;
            }

            strcpy (temp, ptr);
        }
    }

    return buffer_puts (buffer, temp);

#elif defined(MAGNA_SOLARIS)

    /* See https://docs.oracle.com/cd/E19253-01/816-5168/6mbb3hrb1/index.html */

    const char *path = getexecname();

    assert (buffer != NULL);

    /* readlink("/proc/self/path/a.out", buf, bufsize); */

    if (path == NULL) {
        return AM_FALSE;
    }

    return buffer_puts (buffer, path);

#elif defined(MAGNA_FREEBSD)

    char temp [PATH_MAX];

    assert (buffer != NULL);

    /* readlink does not null terminate! */
    memset (temp, 0, sizeof (temp));
    if (readlink ("/proc/curproc/file", temp, PATH_MAX) == -1) {
        return AM_FALSE;
    }

    return buffer_puts (buffer, temp);

#elif defined(MAGNA_NETBSD)

    char temp [PATH_MAX];

    assert (buffer != NULL);

    /* readlink does not null terminate! */
    memset (temp, 0, sizeof (temp));
    if (readlink ("/proc/curproc/exe", temp, PATH_MAX) == -1) {
        return AM_FALSE;
    }

    return buffer_puts (buffer, temp);

#elif defined(MAGNA_APPLE)

    char path[1024];
    uint32_t size = sizeof (path);

    assert (buffer != NULL);

    if (_NSGetExecutablePath (path, &size) != 0) {
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
