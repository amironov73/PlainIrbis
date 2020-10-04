// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 * \file delphi.c
 *
 * Работа с 32-битными DLL, созданными Delphi.
 * Работает только в 32-битной версии библиотеки,
 * собранной MSVC.
 */

/*=========================================================*/

#if defined(_M_IX86) && !defined(__MINGW32__)

/**
 * Вызов функции с одним 32-битным аргументом.
 *
 * @param function Указатель на функцию.
 * @param arg1 Аргумент.
 * @return Значение, возвращенное функцией (если имеет смысл).
 */
MAGNA_API int MAGNA_CALL delphi_call_1
    (
        void *function,
        int arg1
    )
{
    int result;

    (void) arg1;
    assert (function != NULL);

    _asm {
        mov eax, arg1
        call function
        mov result, eax
    }

    return result;
}

/**
 * Вызов функции с двумя 32-битными аргументами.
 *
 * @param function Указатель на функцию.
 * @param arg1 Первый аргумент.
 * @param arg2 Второй аргумент.
 * @return Значение, возвращенное функцией (если имеет смысл).
 */
MAGNA_API int MAGNA_CALL delphi_call_2
    (
        void *function,
        int arg1,
        int arg2
    )
{
    int result;

    (void) arg1;
    (void) arg2;
    assert (function != NULL);

    _asm {
        mov eax, arg1
        mov edx, arg2
        call function
        mov result, eax
    }

    return result;
}

/**
 * Вызов функции с тремя 32-битными аргументами.
 *
 * @param function Указатель на функцию.
 * @param arg1 Первый аргумент.
 * @param arg2 Второй аргумент.
 * @param arg3 Третий аргумент.
 * @return Значение, возвращенное функцией (если имеет смысл).
 */
MAGNA_API int MAGNA_CALL delphi_call_3
    (
        void *function,
        int arg1,
        int arg2,
        int arg3
    )
{
    int result;

    (void) arg1;
    (void) arg2;
    (void) arg3;
    assert (function != NULL);

    _asm {
        mov eax, arg1
        mov edx, arg2
        mov ecx, arg3
        call function
        mov result, eax
    }

    return result;
}

/**
 * Вызов функции с четырьмя 32-битными аргументами.
 *
 * @param function Указатель на функцию.
 * @param arg1 Первый аргумент.
 * @param arg2 Второй аргумент.
 * @param arg3 Третий аргумент.
 * @param arg4 Четвертый аргумент.
 * @return Значение, возвращенное функцией (если имеет смысл).
 */
MAGNA_API int MAGNA_CALL delphi_call_4
    (
        void *function,
        int arg1,
        int arg2,
        int arg3,
        int arg4
    )
{
    int result;

    (void) arg1;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    assert (function != NULL);

    _asm {
        mov eax, arg1
        mov edx, arg2
        mov ecx, arg3
        push arg4
        call function
        mov result, eax
    }

    return result;
}

/**
 * Вызов функции с пятью 32-битными аргументами.
 *
 * @param function Указатель на функцию.
 * @param arg1 Первый аргумент.
 * @param arg2 Второй аргумент.
 * @param arg3 Третий аргумент.
 * @param arg4 Четвертый аргумент.
 * @param arg5 Пятый аргумент.
 * @return Значение, возвращенное функцией (если имеет смысл).
 */
MAGNA_API int MAGNA_CALL delphi_call_5
    (
        void *function,
        int arg1,
        int arg2,
        int arg3,
        int arg4,
        int arg5
    )
{
    int result;

    (void) arg1;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    (void) arg5;
    assert (function != NULL);

    _asm {
        mov eax, arg1
        mov edx, arg2
        mov ecx, arg3
        push arg4
        push arg5
        call function
        mov result, eax
    }

    return result;
}

/**
 * Вызов функции с шестью 32-битными аргументами.
 *
 * @param function Указатель на функцию.
 * @param arg1 Первый аргумент.
 * @param arg2 Второй аргумент.
 * @param arg3 Третий аргумент.
 * @param arg4 Четвертый аргумент.
 * @param arg5 Пятый аргумент.
 * @param arg6 Шестой аргумент.
 * @return Значение, возвращенное функцией (если имеет смысл).
 */
MAGNA_API int MAGNA_CALL delphi_call_6
    (
        void *function,
        int arg1,
        int arg2,
        int arg3,
        int arg4,
        int arg5,
        int arg6
    )
{
    int result;

    (void) arg1;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    (void) arg5;
    (void) arg6;
    assert (function != NULL);

    _asm {
        mov eax, arg1
        mov edx, arg2
        mov ecx, arg3
        push arg4
        push arg5
        push arg6
        call function
        mov result, eax
    }

    return result;
}

#endif

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
