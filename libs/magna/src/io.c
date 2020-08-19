/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/**
 * \file io.c
 *
 * \details У ИРБИС64 довольно оригинальный формат записи целых чисел на диске.
 *
 * 32-битные целые записываются в сетевом формате (Big Endian).
 * 64-битные записываются как пара 32-битных, причем сначала идет младшее,
 * затем старшее.
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

MAGNA_API int MAGNA_CALL read_int_32 (int handle, am_uint32 *value)
{
    return 0;
}

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
