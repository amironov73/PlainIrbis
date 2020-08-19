/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/**
 * \file error.c
 *
 * Обработка и демонстрация ошибок.
 *
 */

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4255)
#pragma warning(disable: 4820)
#endif

/*=========================================================*/

#ifdef MAGNA_WINDOWS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#endif

/*=========================================================*/

MAGNA_API void display_error ()
{
#ifdef MAGNA_WINDOWS

    LPSTR message;
    DWORD errorCode = GetLastError();

    FormatMessageA
        (
            FORMAT_MESSAGE_ALLOCATE_BUFFER
            | FORMAT_MESSAGE_FROM_SYSTEM
            | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorCode,
            MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR) &message,
            0,
            NULL
        );

    MessageBoxA
        (
            NULL,
            message,
            "ERROR",
            MB_OK | MB_ICONERROR
        );
    LocalFree (message);

#endif
}

/*=========================================================*/

MAGNA_API void print_error ()
{
#ifdef MAGNA_WINDOWS

    LPSTR message;
    DWORD errorCode = GetLastError();

    FormatMessageA
        (
            FORMAT_MESSAGE_ALLOCATE_BUFFER
            | FORMAT_MESSAGE_FROM_SYSTEM
            | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorCode,
            MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR) &message,
            0,
            NULL
        );

    fputs ("ERROR: ", stderr);
    fputs (message, stderr);
    fputs ("\n", stderr);
    LocalFree (message);

#endif
}

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
