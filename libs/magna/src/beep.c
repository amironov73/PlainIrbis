/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4255)
#endif

/*=========================================================*/

#ifdef MAGNA_WINDOWS
    #include <windows.h>
#endif

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

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/
