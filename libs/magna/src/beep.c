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

/*=========================================================*/

/*!
 * \file beep.c
 *
 * Простой звуковой сигнал (для отладки).
 */

/*=========================================================*/

/**
 * Sounds a beep in the speaker or sound card.
 */
void beep (void)
{
#ifdef MAGNA_WINDOWS

    /*MessageBeep(0xFFFFFFFF);*/
    Beep (750, 300);

#endif
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
