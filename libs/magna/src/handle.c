// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#ifdef MAGNA_WINDOWS

    #ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4668)
    #endif

    #include <windows.h>

    #ifdef _MSC_VER
    #pragma warning(pop)
    #endif

#endif

#include <assert.h>

/*=========================================================*/

/**
 * \file handle.c
 *
 * Простая абстракция над Win32 HANDLE.
 */

/*=========================================================*/

/**
 * Проверка, валидный ли дескриптор.
 *
 * @param handle Проверяемый дескриптор.
 * @return Результат проверки.
 */
MAGNA_API MAGNA_INLINE am_bool MAGNA_CALL handle_is_good
    (
        am_handle handle
    )
{
#ifdef MAGNA_WINDOWS

    return (handle.pointer != INVALID_HANDLE_VALUE);

#else

    return (handle.value != -1);

#endif
}

/**
 * Получение заведомо неверного файлового дескриптора.
 *
 * @return Файловый дескриптор.
 */
MAGNA_API MAGNA_INLINE am_handle handle_get_bad (void)
{
    am_handle result;

#ifdef MAGNA_WINDOWS

    result.pointer = AM_BAD_HANDLE;

#else

    result.value = AM_BAD_HANDLE;

#endif


    return result;
}

/**
 * Конструирование дескриптора из целого числа.
 *
 * @param value Целочисленный индекс.
 * @return Сконструированный дескриптор.
 */
MAGNA_API MAGNA_INLINE am_handle MAGNA_CALL handle_from_value
    (
        int value
    )
{
    am_handle result;

    result.value = value;

    return result;
}

/**
 * Конструирование дескриптора из указателя.
 *
 * @param pointer Указатель.
 * @return Сконструированный дескриптор.
 */
MAGNA_API MAGNA_INLINE am_handle MAGNA_CALL handle_from_pointer
    (
        void *pointer
    )
{
    am_handle result;

    result.pointer = pointer;

    return result;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
