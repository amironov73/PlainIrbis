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
    return AM_BAD_HANDLE;
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
    assert (handle != AM_BAD_HANDLE);

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
