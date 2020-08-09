/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

#include <assert.h>

#ifdef _M_IX86

MAGNA_API int MAGNA_CALL delphi_call_1
    (
        void *function,
        int arg1
    )
{
    int result;

    assert (function != NULL);

    _asm {
        mov eax, arg1
        call function
        mov result, eax
    }

    return result;
}

MAGNA_API int MAGNA_CALL delphi_call_2
    (
        void *function,
        int arg1,
        int arg2
    )
{
    int result;

    assert (function != NULL);

    _asm {
        mov eax, arg1
        mov edx, arg2
        call function
        mov result, eax
    }

    return result;
}

MAGNA_API int MAGNA_CALL delphi_call_3
    (
        void *function,
        int arg1,
        int arg2,
        int arg3
    )
{
    int result;

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
