// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

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

    #ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4668)
    #endif

    #include <windows.h>
    #include <io.h>

    #ifdef _MSC_VER
    #pragma warning(pop)
    #endif

#endif

#ifdef MAGNA_MSDOS

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <direct.h>

#endif

#ifdef MAGNA_APPLE

#include <mach-o/dyld.h>
#include <unistd.h>

#endif

#ifdef MAGNA_UNIX

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
#include <stdarg.h>

/*=========================================================*/

/**
 * Получение текущей директории.
 *
 * @param path Буфер для результата.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL path_get_current_directory
    (
        Buffer *path
    )
{
    char temporary [FILENAME_MAX];

    assert (path != NULL);

    mem_clear (temporary, sizeof (temporary));

#ifdef MAGNA_WINDOWS

    if (!GetCurrentDirectoryA (sizeof (temporary), temporary)) {
        return AM_FALSE;
    }

#else

    if (!getcwd (temporary, sizeof (temporary))) {
        return AM_FALSE;
    }

#endif

    return buffer_assign_text (path, CBTEXT (temporary));
}

/**
 * Смена текущей директории.
 *
 * @param path Буфер с новой директорией.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL path_set_current_directory
    (
        const Buffer *path
    )
{
    char temporary [FILENAME_MAX];

    assert (path != NULL);

    if (buffer_length (path) >= FILENAME_MAX) {
        /* Слишком длинный путь */
        return AM_FALSE;
    }

    mem_clear (temporary, sizeof (temporary));
    mem_copy (temporary, path->start, buffer_length (path));

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
 * @return Признак успешного завершения операции.
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

    return buffer_puts (path, CBTEXT (buffer));


#elif defined (MAGNA_UNIX)

    const char *result;

    assert (path != NULL);

    result = getenv ("TMPDIR");
    if (!result) {
        result = getenv ("TEMPDIR");
    }

    if (!result) {
        result = getenv ("TMP");
    }

    if (!result) {
        result = getenv ("TEMP");
    }

    if (!result) {
        /* The Filesystem Hierarchy Standard version 3.0 says:
           The /tmp directory must be made available for programs
           that require temporary files. */
        result = "/tmp";
    }

    return buffer_puts (path, CBTEXT (result));

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

    if (buffer_length (path) <= 2) {
        result.end = result.start = path->start;
        return result;
    }

    result.end = result.start = path->current;

    while (result.start >= path->start) {
        c = *result.start;
        if (c == '.') {
            break;
        }

        if (c == '/' || c == '\\') {
            result.end = result.start = path->start;
            break;
        }

        --result.start;
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

    if (buffer_length (path) <= 2) {
        result.end = result.start = path->start;
        return result;
    }

    result.end = result.start = path->current;

    while (result.start >= path->start) {
        c = *result.start;
        if (c == '/' || c == '\\') {
            ++result.start;
            break;
        }

        --result.start;
    }

    return result;
}

/**
 * Получение директории для указанного файла.
 *
 * @param path Путь к файлу (возможно, неполный).
 * @return Директория (возможно, пустая).
 */
MAGNA_API Span MAGNA_CALL path_get_directory
    (
        const Buffer *path
    )
{
    Span result;
    char c;
    const am_byte *ptr;

    assert (path != NULL);

    if (buffer_is_empty(path)) {
        return buffer_to_span (path);
    }

    result.start = path->start;
    ptr = path->current;
    if (ptr >= path->start) {
        for (;;) {
            c = *ptr;
            if (c == '/' || c == '\\') {
                if (ptr != path->start) {
                    --ptr;
                }
                break;
            }

            if (ptr == path->start) {
                break;
            }

            --ptr;
        }
    }

    if (ptr == path->start) {
        if (*ptr == '/' || *ptr == '\\') {
            result.end = result.start + 1;
            return result;
        }

        result.end = result.start;
    }
    else {
        result.end = result.start + (ptr - path->start) + 1;
    }

    return result;
}

/**
 * Превращает неправильные слэши в правильные.
 *
 * @param path Путь к файлу (возможно, пустой).
 */
MAGNA_API void MAGNA_CALL path_convert_slashes
    (
        Buffer *path
    )
{
    am_byte *ptr, *end;

    assert (path != NULL);

    for (ptr = path->start, end = path->current; ptr < end; ++ptr) {

#if defined (MAGNA_WINDOWS) || defined (MAGNA_MSDOS)

        if (*ptr == '/') {
            *ptr = '\\';
        }

#else

        if (*ptr == '\\') {
            *ptr = '/';
        }

#endif
    }
}

/**
 * Склеивание пути из компонентов.
 *
 * @param output Инициализированный буфер для результата.
 * @param ... Компоненты пути, последний должен быть `NULL`.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool path_combine
    (
        Buffer *output,
        ...
    )
{
    va_list args;
    Buffer *path;
    am_bool first = AM_TRUE, result = AM_FALSE;

#if defined (MAGNA_WINDOWS) || defined (MAGNA_MSDOS)
    char slash = '\\';
#else
    char slash = '/';
#endif

    assert (output != NULL);

    va_start (args, output);
    for (;;) {
        path = va_arg (args, Buffer*);
        if (path == NULL) {
            result = AM_TRUE;
            break;
        }

        if (buffer_is_empty(path)) {
            continue;
        }

        path_trim_trailing_slashes (output);

        if (!first) {
            if (!buffer_putc (output, slash)) {
                break;
            }
        }

        first = AM_FALSE;

        if (!buffer_concat (output, path)) {
            break;
        }
    }

    va_end (args);

    return result;
}

MAGNA_API char path_get_slash (void)
{
#if defined (MAGNA_WINDOWS) || defined (MAGNA_MSDOS)
    char slash = '\\';
#else
    char slash = '/';
#endif

    return slash;
}

MAGNA_API am_bool MAGNA_CALL path_add_trailing_slash
    (
        Buffer *path
    )
{
    assert (path != NULL);

    return buffer_putc (path, path_get_slash());
}

MAGNA_API am_bool MAGNA_CALL path_append
    (
        Buffer *path,
        Span element
    )
{
    assert (path != NULL);

    path_trim_trailing_slashes (path);

    if (!path_add_trailing_slash (path)) {
        return AM_FALSE;
    }

    if (!span_is_empty (element)) {
        return buffer_write (path, element.start, span_length (element));
    }

    return AM_TRUE;
}

/**
 * Удаление слэшей в конце пути.
 *
 * @param path Путь для обработки.
 */
MAGNA_API void MAGNA_CALL path_trim_trailing_slashes
    (
        Buffer *path
    )
{
    am_byte chr;

    assert (path != NULL);

    while (buffer_length (path) > 1) {
        chr = path->current [-1];
        if (chr != '/' && chr != '\\') {
            break;
        }
        --path->current;
    }
}

/**
 * Путь к исполняемому файлу.
 *
 * @param buffer Проинициализированный буфер.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL path_get_executable
    (
        Buffer *buffer
    )
{
#if defined (MAGNA_WINDOWS)

    char temp [MAX_PATH];

    assert (buffer != NULL);

    mem_clear (temp, sizeof (temp));
    if (!GetModuleFileNameA (NULL, temp, MAX_PATH)) {
        return AM_FALSE;
    }

    return buffer_puts (buffer, CBTEXT (temp));

#elif defined (MAGNA_LINUX)

    char temp [PATH_MAX], temp2[30], *ptr;
    pid_t pid;

    assert (buffer != NULL);

    /* readlink does not null terminate! */
    memset (temp, 0, sizeof (temp));
    if (readlink ("/proc/self/exe", temp, PATH_MAX) == -1) {
        /* Может, procfs есть, но нет /proc/self */
        pid = getpid ();
        sprintf (temp2, "/proc/%ld/exe", (long) pid);
        if (readlink (temp2, temp, PATH_MAX) == -1) {

            /* When Bash invokes an external command,
              the variable ‘$_’ is set to the full pathname
              of the command and passed to that command
              in its environment.*/

            ptr = getenv ("_");
            if (!ptr) {
                return AM_FALSE;
            }

            strcpy (temp, ptr);
        }
    }

    return buffer_puts (buffer, temp);

#elif defined (MAGNA_SOLARIS)

    /* See https://docs.oracle.com/cd/E19253-01/816-5168/6mbb3hrb1/index.html */

    const char *path = getexecname ();

    assert (buffer != NULL);

    /* readlink ("/proc/self/path/a.out", buf, bufsize); */

    if (path == NULL) {
        return AM_FALSE;
    }

    return buffer_puts (buffer, path);

#elif defined (MAGNA_FREEBSD)

    char temp [PATH_MAX];

    assert (buffer != NULL);

    /* readlink does not null terminate! */
    memset (temp, 0, sizeof (temp));
    if (readlink ("/proc/curproc/file", temp, PATH_MAX) == -1) {
        return AM_FALSE;
    }

    return buffer_puts (buffer, temp);

#elif defined (MAGNA_NETBSD)

    char temp [PATH_MAX];

    assert (buffer != NULL);

    /* readlink does not null terminate! */
    memset (temp, 0, sizeof (temp));
    if (readlink ("/proc/curproc/exe", temp, PATH_MAX) == -1) {
        return AM_FALSE;
    }

    return buffer_puts (buffer, temp);

#elif defined (MAGNA_APPLE)

    char path[1024];
    uint32_t size = sizeof (path);

    assert (buffer != NULL);

    if (_NSGetExecutablePath (path, &size) != 0) {
        return AM_FALSE;
    }

    return buffer_puts (buffer, CBTEXT (path));

#else

    (void)buffer;

    return AM_FALSE;

#endif
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
