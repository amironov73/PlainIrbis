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

/* TODO: implement */

#else

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#endif

#include <assert.h>

/*=========================================================*/

/*
 * \file thread.c
 *
 * Работа с потоками.
 */

/*=========================================================*/

/**
 * Создание потока.
 *
 * @param start
 * @return
 */
MAGNA_API am_handle MAGNA_CALL thread_create
    (
        void *start
    )
{
#ifdef MAGNA_WINDOWS

    LPSECURITY_ATTRIBUTES securityAttributes = NULL;
    SIZE_T stackSize = 0; /* use defaults */
    DWORD creationFlags = 0;
    DWORD threadId;
    am_handle result;

    result.pointer = CreateThread
        (
            securityAttributes,
            stackSize,
            (LPTHREAD_START_ROUTINE) start,
            NULL,
            creationFlags,
            &threadId
        );

    return result;

#elif defined(MAGNA_MSDOS)

    return -1;

#else

    am_handle result;

    result.value = AM_BAD_HANDLE;

    return result;

#endif
}

/**
 * Ожидание потока.
 *
 * @param handle
 * @param timeout
 * @return
 */
MAGNA_API am_bool MAGNA_CALL thread_join
    (
        am_handle handle,
        am_int32 timeout
    )
{
    assert (handle_is_good(handle));

#ifdef MAGNA_WINDOWS

    WaitForSingleObject (handle.pointer, timeout);

#endif

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
