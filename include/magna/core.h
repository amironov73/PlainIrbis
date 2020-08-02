// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef MAGNA_CORE_H
#define MAGNA_CORE_H

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

//=========================================================

#if !defined(MAGNA_WINDOWS) && !defined (MAGNA_UNIX)

    #if defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(_WINDOWS)

        #define MAGNA_WINDOWS

    #else

        #define MAGNA_UNIX

        #if defined(__FreeBSD__)

           #define MAGNA_FREEBSD

	    #endif

        #if defined(__ANDROID__)

            #define MAGNA_ANDROID

        #endif

        #if defined(__APPLE__) || defined(__APPLE_CC__) || defined (__OSX__)

            #define MAGNA_APPLE

        #endif

#endif

#endif

//=========================================================

#ifdef MAGNA_DYNAMIC_LIBRARY

#if defined(_MSC_VER)

        // Microsoft
        #define MAGNA_EXPORT __declspec(dllexport)
        #define MAGNA_IMPORT __declspec(dllimport)
        #define MAGNA_HIDDEN

#elif defined(__GNUC__)

        //  GCC
        #define MAGNA_EXPORT __attribute__((visibility("default")))
        #define MAGNA_IMPORT
        #define MAGNA_HIDDEN __attribute__((visibility("hidden")))

#else

        //  do nothing and hope for the best?
        #define MAGNA_EXPORT
        #define MAGNA_IMPORT
        #define MAGNA_HIDDEN
        #pragma warning Unknown dynamic link import/export semantics.

    #endif

#else

    #define MAGNA_EXPORT
    #define MAGNA_IMPORT
    #define MAGNA_HIDDEN

#endif

//=========================================================

#ifndef MAGNA_API

    #ifdef MAGNA_LIBRARY

        #define MAGNA_API MAGNA_EXPORT

    #else

        #define MAGNA_API MAGNA_IMPORT

    #endif // MAGNA_LIBRARY

#endif // IRBIS_API

// для функций: int __fastcall func (int arg).
#ifndef MAGNA_CALL

    #if defined(_MSC_VER)

        // Microsoft
        #define MAGNA_CALL __fastcall

    #elif defined(__GNUC__)

        //  GCC
        #define MAGNA_CALL

    #else

        //  do nothing and hope for the best?
        #define MAGNA_CALL

    #endif

#endif // IRBIS_CALL

//=========================================================

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char      am_byte;
typedef short     am_int16;
typedef int       am_int32;
typedef long long am_int64;

//=========================================================

MAGNA_API int magna_version (void);

//=========================================================

#ifdef _M_IX86

MAGNA_API int MAGNA_CALL delphi_call_1 (void *function, int arg1);
MAGNA_API int MAGNA_CALL delphi_call_2 (void *function, int arg1, int arg2);
MAGNA_API int MAGNA_CALL delphi_call_3 (void *function, int arg1, int arg2,
                                        int arg3);
MAGNA_API int MAGNA_CALL delphi_call_4 (void *function, int arg1, int arg2,
                                        int arg3, int arg4);
MAGNA_API int MAGNA_CALL delphi_call_5 (void *function, int arg1, int arg2,
                                        int arg3, int arg4, int arg5);
MAGNA_API int MAGNA_CALL delphi_call_6 (void *function, int arg1, int arg2,
                                        int arg3, int arg4, int arg5, int arg6);

#endif

//=========================================================

typedef struct
{
    char *ptr;
    int len;

} Span;

//=========================================================

typedef void* (*Cloner)    (void*);
typedef void  (*Liberator) (void*);

//=========================================================

typedef struct
{
    void **ptr;
    size_t len;
    size_t capacity;

    Cloner cloner;
    Liberator liberator;

} Array;

MAGNA_API Array* MAGNA_CALL array_clone      (Array *array);
MAGNA_API void   MAGNA_CALL array_copy       (Array *target, Array *source);
MAGNA_API void   MAGNA_CALL array_concat     (Array *target, Array *source);
MAGNA_API void   MAGNA_CALL array_create     (Array *array,  size_t capacity);
MAGNA_API void   MAGNA_CALL array_free       (Array *array);
MAGNA_API void*  MAGNA_CALL array_get        (Array *array,  size_t index);
MAGNA_API void   MAGNA_CALL array_grow       (Array *array,  size_t newSize);
MAGNA_API void*  MAGNA_CALL array_pop_back   (Array *array);
MAGNA_API void*  MAGNA_CALL array_pop_front  (Array *array);
MAGNA_API void   MAGNA_CALL array_push_back  (Array *array,  void *item);
MAGNA_API void   MAGNA_CALL array_push_front (Array *array,  void *item);
MAGNA_API void   MAGNA_CALL array_set        (Array *array,  size_t index, void *item);
MAGNA_API void   MAGNA_CALL array_truncate   (Array *array,  size_t newSize);

//=========================================================

typedef struct
{
    char *ptr;
    size_t position;
    size_t capacity;

} Buffer;

MAGNA_API size_t  MAGNA_CALL buffer_calculate_size (size_t newSize);
MAGNA_API Buffer* MAGNA_CALL buffer_clone  (Buffer *buffer);
MAGNA_API void    MAGNA_CALL buffer_copy   (Buffer *target, Buffer *source);
MAGNA_API void    MAGNA_CALL buffer_concat (Buffer *target, Buffer *source);
MAGNA_API void    MAGNA_CALL buffer_create (Buffer *buffer, char *data, size_t length);
MAGNA_API void    MAGNA_CALL buffer_free   (Buffer *buffer);
MAGNA_API void    MAGNA_CALL buffer_grow   (Buffer *buffer, size_t newSize);
MAGNA_API void    MAGNA_CALL buffer_putc   (Buffer *buffer, char c);
MAGNA_API void    MAGNA_CALL buffer_puts   (Buffer *buffer, char *str);
MAGNA_API void    MAGNA_CALL buffer_write  (Buffer *target, char *data, size_t length);

//=========================================================

#endif // MAGNA_CORE_H
