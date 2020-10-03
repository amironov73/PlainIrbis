// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#ifdef MAGNA_WINDOWS

#include <windows.h>

#endif

#include <assert.h>

/*=========================================================*/

/**
 * @file mapfile.c
 *
 * Работа с файлами, отображаемыми в память.
 *
 */

/*=========================================================*/

typedef struct
{
    am_handle file;
    am_uint64 size;

} MapFile;


MAGNA_API am_bool MAGNA_CALL mapfile_open_read
    (
        MapFile *mapfile,
        const char *fileName
    )
{
#ifdef MAGNA_WINDOWS

    DWORD desiredAccess = GENERIC_READ;
    DWORD shareMode = 0;
    LPSECURITY_ATTRIBUTES securityAttributes = NULL;
    DWORD creationDisposition = OPEN_EXISTING;
    DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE templateFile = NULL;
    HANDLE result;

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

    if (result == INVALID_HANDLE_VALUE) {
        return AM_FALSE;
    }

    mapfile->file.pointer = result;
    mapfile->size = file_size (mapfile->file);

    return AM_TRUE;

#elif defined (MAGNA_UNIX)

    /* TODO: implement */

    return AM_FALSE;

#else

    return AM_FALSE;

#endif
}

MAGNA_API am_bool MAGNA_CALL mapfile_open_write
    (
        MapFile *mapfile,
        const char *fileName
    )
{
#ifdef MAGNA_WINDOWS

    DWORD desiredAccess = GENERIC_READ|GENERIC_WRITE;
    DWORD shareMode = 0;
    LPSECURITY_ATTRIBUTES securityAttributes = NULL;
    DWORD creationDisposition = OPEN_EXISTING;
    DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE templateFile = NULL;
    HANDLE result;

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

    if (result == INVALID_HANDLE_VALUE) {
        return AM_FALSE;
    }

    mapfile->file.pointer = result;
    mapfile->size = file_size (mapfile->file);

    return AM_TRUE;

#elif defined (MAGNA_UNIX)

    /* TODO: implement */

    return AM_FALSE;

#else

    return AM_FALSE;

#endif
}

MAGNA_API void MAGNA_CALL mapfile_close
    (
        MapFile *mapfile
    )
{
    assert (mapfile != NULL);
    assert (handle_is_good (mapfile->file));

#ifdef MAGNA_WINDOWS

    CloseHandle (mapfile->file.pointer);
    mapfile->file.pointer = AM_BAD_HANDLE;

#elif defined (MAGNA_UNIX)

    /* TODO: implement */

#endif
}

MAGNA_API Span MAGNA_CALL mapfile_view
    (
        MapFile *mapFile,
        size_t offset,
        size_t size
    )
{
    return span_null();
}

MAGNA_API void MAGNA_CALL mapfile_unview
    (
        Span span
    )
{
    /* TODO: implement */
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
