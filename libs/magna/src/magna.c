// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 * Версия библиотеки в виде строки.
 *
 * @return Версия библиотеки.
 */
MAGNA_API const am_byte* magna_get_version_string (void)
{
    am_int32 version;       /* версия как число (чтобы не запрашивать дважды) */
    static am_byte text[6]; /* разбита на мажор и минор версия */

    version = magna_get_version ();
    sprintf
        (
            (char*) text,
            "%u.%02u",
            (version & 0xFF00u) >> 16u,
            version & 0x00FFu
        );

    return text;
}

/**
 * Требуем библиотеку не ниже указанной версии
 *
 * @param version Минимально допустимая версия.
 * @return Требование выполнено?
 */
MAGNA_API am_bool MAGNA_CALL magna_require_version
    (
        am_int32 version
    )
{
    return MAGNA_VERSION >= version;
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
