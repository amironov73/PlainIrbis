/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"
#include "version.h"

/**
 * Версия библиотеки.
 *
 * @return Версия.
 */
MAGNA_API am_int32 magna_get_version (void)
{
    return MAGNA_VERSION;
}

/**
 * Запущено на Windows?
 *
 * @return Да или нет.
 */
MAGNA_API am_bool magna_on_windows (void)
{
#ifdef MAGNA_WINDOWS

    return AM_TRUE;

#else

    return AM_FALSE;

#endif
}
