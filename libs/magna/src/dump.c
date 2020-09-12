/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 * \file number.c
 *
 * Текст, содержащий фрагменты с числами.
 */

/*=========================================================*/

/**
 * Дамп данных в указанный файл.
 *
 * @param file
 * @param bytes
 * @param length
 */
MAGNA_API void MAGNA_CALL dump_bytes
    (
        FILE *file,
        const am_byte *bytes,
        am_size_t length
    )
{
    am_size_t offset, i, run;

    assert (bytes != NULL);

    for (offset = 0; offset < length; offset += 16) {
        fprintf (file, "%08lX: ", (unsigned long) offset);
        run = min (length - offset, 16);
        for (i = 0; i < run; ++i) {
            fprintf (file, " %02X", bytes [offset + i]);
        }
        fputs ("\n", file);
    }
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
