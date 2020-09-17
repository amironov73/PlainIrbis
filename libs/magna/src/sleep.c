// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#ifdef MAGNA_WINDOWS

#include <windows.h>

#endif

#if defined(MAGNA_UNIX)

#include <unistd.h>

#endif

/*=========================================================*/

/**
 * \file sleep.c
 *
 * Засыпание программы.
 */

/*=========================================================*/

/**
 * Засыпание текущего потока на указанное количество миллисекунд.
 *
 * @param interval Количество миллисекунд. 0 означает простую передачу остатака кванта времени системе.
 */
MAGNA_API void MAGNA_CALL magna_sleep
    (
        unsigned interval
    )
{
#ifdef MAGNA_WINDOWS

    Sleep (interval);

#endif

#ifdef MAGNA_UNIX

    usleep (interval * 1000);

#endif
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
