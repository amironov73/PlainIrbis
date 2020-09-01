/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 5045)
#endif

/*=========================================================*/

/**
 * \file memory.c
 *
 * Работа с памятью.
 *
 */

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 *
 * @param size
 * @return
 */
MAGNA_API void* MAGNA_CALL mem_alloc
    (
        am_size_t size
    )
{
    return calloc (1, size);
}

/**
 *
 * @param size
 * @return
 */
MAGNA_API void* MAGNA_CALL mem_alloc_ex
    (
        am_size_t size
    )
{
    void* result = calloc (1, size);

    if (!result) {
        exit (1);
    }

    return result;
}

/**
 *
 * @param ptr
 */
MAGNA_API void MAGNA_CALL mem_free
    (
        void *ptr
    )
{
    free (ptr);
}

/**
 *
 * @param ptr
 * @param size
 */
MAGNA_API void MAGNA_CALL mem_clear
    (
        void *ptr,
        am_size_t size
    )
{
    assert (ptr != NULL);

    memset (ptr, 0, size);
}

/**
 *
 * @param destination
 * @param source
 * @param size
 */
MAGNA_API void MAGNA_CALL mem_copy
    (
        void *destination,
        void *source,
        am_size_t size
    )
{
    assert (destination != NULL);
    assert (source != NULL);

    memcpy (destination, source, size);
}

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
