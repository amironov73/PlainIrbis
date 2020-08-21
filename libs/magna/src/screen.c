/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/**
 * \file screen.c
 *
 * Работа с консолью.
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

#ifdef MAGNA_WINDOWS

static HANDLE hStdout, hScreen;
static CHAR_INFO *screen;

MAGNA_API int screen_init (void)
{
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    return 1;
}

MAGNA_API int screen_set_encoding (int encoding)
{
    SetConsoleCP((UINT) encoding);
    return 1;
}

#endif

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
