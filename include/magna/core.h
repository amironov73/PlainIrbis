/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#ifndef MAGNA_CORE_H
#define MAGNA_CORE_H

#define WIN32_LEAN_AND_MEAN /* Exclude rarely-used stuff from Windows headers */

/*=========================================================*/

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

/*=========================================================*/

#ifdef MAGNA_DYNAMIC_LIBRARY

#if defined(_MSC_VER)

        /* Microsoft */
        #define MAGNA_EXPORT __declspec(dllexport)
        #define MAGNA_IMPORT __declspec(dllimport)
        #define MAGNA_HIDDEN

#elif defined(__GNUC__)

        /*  GCC */
        #define MAGNA_EXPORT __attribute__((visibility("default")))
        #define MAGNA_IMPORT
        #define MAGNA_HIDDEN __attribute__((visibility("hidden")))

#else

        /*  do nothing and hope for the best? */
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

/*=========================================================*/

#ifndef MAGNA_API

    #ifdef MAGNA_LIBRARY

        #define MAGNA_API MAGNA_EXPORT

    #else

        #define MAGNA_API MAGNA_IMPORT

    #endif

#endif

/* для функций: int __fastcall func (int arg). */
#ifndef MAGNA_CALL

    #if defined(_MSC_VER)

        /* Microsoft */
        #define MAGNA_CALL __fastcall

    #elif defined(__GNUC__)

        /*  GCC */
        #define MAGNA_CALL

    #else

        /*  do nothing and hope for the best? */
        #define MAGNA_CALL

    #endif

#endif

/*=========================================================*/

#if !defined(MAGNA_64BIT) && !defined(MAGNA_32BIT) && !defined(MAGNA_16BIT)

    #if defined(_WIN64)

        #define MAGNA_64BIT

    #else

        #define MAGNA_32BIT

    #endif

#endif

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5045)
#endif

/*=========================================================*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*=========================================================*/

/*
 * Some primitive types.
 */

typedef          int       am_bool;
typedef          char      am_byte;
typedef          short     am_int16;
typedef unsigned short     am_uint16;
typedef          int       am_int32;
typedef unsigned int       am_uint32;
typedef          long long am_int64;
typedef unsigned long long am_uint64;

typedef unsigned short am_wchar;

typedef unsigned int am_size;
typedef          int am_ssize;

typedef am_uint32  am_mfn;
typedef am_uint32  am_flag;
typedef am_uint64  am_offset;
typedef void      *am_pointer;

#ifdef MAGNA_WINDOWS
typedef void *am_handle;
#else
typedef int  am_handle;
#endif

#define AM_FALSE 0
#define AM_TRUE  1

/*=========================================================*/

#define MAGNA_VERSION_MAJOR 0u
#define MAGNA_VERSION_MINOR 1u

MAGNA_API am_int32 magna_version    (void);
MAGNA_API am_bool  magna_on_windows (void);

/*=========================================================*/

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

/*=========================================================*/

typedef struct
{
    am_byte *ptr;
    am_size len;

} Span;

MAGNA_API Span MAGNA_CALL span_init (am_byte *ptr, am_size len);
MAGNA_API Span MAGNA_CALL span_from_text (char *str);

#define SPAN_INIT { NULL, 0 }

/*=========================================================*/

typedef void* (*Cloner)    (void*);
typedef void  (*Liberator) (void*);

/*=========================================================*/

typedef struct
{
    void **ptr;
    size_t len;
    size_t capacity;

    Cloner cloner;
    Liberator liberator;

} Array;

MAGNA_API void   MAGNA_CALL array_clone      (Array *target, Array *source);
MAGNA_API void   MAGNA_CALL array_copy       (Array *target, Array *source);
MAGNA_API void   MAGNA_CALL array_concat     (Array *target, Array *source);
MAGNA_API void   MAGNA_CALL array_create     (Array *array,  am_size capacity);
MAGNA_API void   MAGNA_CALL array_free       (Array *array);
MAGNA_API void*  MAGNA_CALL array_get        (Array *array,  am_size index);
MAGNA_API void   MAGNA_CALL array_grow       (Array *array,  am_size newSize);
MAGNA_API void*  MAGNA_CALL array_pop_back   (Array *array);
MAGNA_API void*  MAGNA_CALL array_pop_front  (Array *array);
MAGNA_API void   MAGNA_CALL array_push_back  (Array *array,  void *item);
MAGNA_API void   MAGNA_CALL array_push_front (Array *array,  void *item);
MAGNA_API void   MAGNA_CALL array_set        (Array *array,  am_size index, void *item);
MAGNA_API void   MAGNA_CALL array_truncate   (Array *array,  am_size newSize);

#define ARRAY_INIT { NULL, 0, 0, NULL, NULL }

/*=========================================================*/

typedef struct
{
    am_byte *ptr;
    am_size position;
    am_size capacity;

} Buffer;

MAGNA_API Buffer* MAGNA_CALL buffer_assign         (Buffer *buffer, const am_byte *data, am_size length);
MAGNA_API Buffer* MAGNA_CALL buffer_assign_text    (Buffer *buffer, const char *text);
MAGNA_API am_size MAGNA_CALL buffer_calculate_size (am_size newSize);
MAGNA_API Buffer* MAGNA_CALL buffer_clone          (Buffer *target, Buffer *source);
MAGNA_API void    MAGNA_CALL buffer_copy           (Buffer *target, Buffer *source);
MAGNA_API void    MAGNA_CALL buffer_concat         (Buffer *target, Buffer *source);
MAGNA_API void    MAGNA_CALL buffer_create         (Buffer *buffer, am_byte *data, am_size length);
MAGNA_API void    MAGNA_CALL buffer_free           (Buffer *buffer);
MAGNA_API Buffer* MAGNA_CALL buffer_from_text      (Buffer *buffer, const char *text);
MAGNA_API void    MAGNA_CALL buffer_grow           (Buffer *buffer, am_size newSize);
MAGNA_API void    MAGNA_CALL buffer_putc           (Buffer *buffer, char c);
MAGNA_API void    MAGNA_CALL buffer_puts           (Buffer *buffer, const char *str);
MAGNA_API void    MAGNA_CALL buffer_static         (Buffer *buffer, am_byte *data, am_size newSize);
MAGNA_API void    MAGNA_CALL buffer_write          (Buffer *target, const am_byte *data, am_size length);

#define BUFFER_INIT { NULL, 0, 0 }

/*=========================================================*/

/* Блок памяти */

typedef struct MagnaMemoryChunk
{
    am_byte *data;
    struct MagnaMemoryChunk *next;

} MemoryChunk;

/* Буфер, состоящий из блоков */
typedef struct
{
    MemoryChunk *first, *current, *last;
    am_size chunkSize, position, read;

} ChunkedBuffer;

MAGNA_API Buffer*        MAGNA_CALL chunked_all            (const ChunkedBuffer *chunked, Buffer *buffer);
MAGNA_API am_size        MAGNA_CALL chunked_capacity       (const ChunkedBuffer *chunked);
MAGNA_API am_bool        MAGNA_CALL chunked_eof            (const ChunkedBuffer *chunked);
MAGNA_API void           MAGNA_CALL chunked_free           (ChunkedBuffer *chunked);
MAGNA_API am_bool        MAGNA_CALL chunked_grow           (ChunkedBuffer *chunked, am_size newSize);
MAGNA_API ChunkedBuffer* MAGNA_CALL chunked_init           (ChunkedBuffer *chunked, am_size chunkSize);
MAGNA_API int            MAGNA_CALL chunked_peek           (const ChunkedBuffer *chunked);
MAGNA_API am_size        MAGNA_CALL chunked_position       (const ChunkedBuffer *chunked);
MAGNA_API int            MAGNA_CALL chunked_read_byte      (ChunkedBuffer *chunked);
MAGNA_API am_size        MAGNA_CALL chunked_read           (ChunkedBuffer *chunked, Buffer *buffer, am_size count);
MAGNA_API am_size        MAGNA_CALL chunked_read_line      (ChunkedBuffer *chunked, Buffer *buffer);
MAGNA_API am_size        MAGNA_CALL chunked_read_remaining (ChunkedBuffer *chunked, Buffer *buffer);
MAGNA_API am_size        MAGNA_CALL chunked_remaining_size (const ChunkedBuffer *chunked);
MAGNA_API ChunkedBuffer* MAGNA_CALL chunked_rewind         (ChunkedBuffer *chunked);
MAGNA_API am_size        MAGNA_CALL chunked_size           (const ChunkedBuffer *chunked);
MAGNA_API am_bool        MAGNA_CALL chunked_write          (ChunkedBuffer *chunked, am_byte *data, am_size dataSize);
MAGNA_API am_bool        MAGNA_CALL chunked_write_byte     (ChunkedBuffer *chunked, am_byte value);
MAGNA_API am_bool        MAGNA_CALL chunked_write_text     (ChunkedBuffer *chunked, const char *text);

/*=========================================================*/

/* Буфер, хранящий часть данных на стеке */

typedef struct
{
    am_byte *static_data;
    am_byte *dynamic_data;
    am_size capacity;
    am_size size;
    am_size static_capacity;
} FastBuffer;

MAGNA_API FastBuffer* MAGNA_CALL fastbuf_clear     (FastBuffer *buffer);
MAGNA_API am_bool     MAGNA_CALL fastbuf_empty     (const FastBuffer *buffer);
MAGNA_API void        MAGNA_CALL fastbuf_free      (FastBuffer *buffer);
MAGNA_API am_int32    MAGNA_CALL fastbuf_getc      (FastBuffer *buffer);
MAGNA_API am_int32    MAGNA_CALL fastbuf_getc_utf8 (FastBuffer *buffer);
MAGNA_API am_size     MAGNA_CALL fastbuf_gets      (FastBuffer *buffer, Buffer *text);
MAGNA_API am_bool     MAGNA_CALL fastbuf_grow      (FastBuffer *buffer, am_size newSize);
MAGNA_API void        MAGNA_CALL fastbuf_init      (FastBuffer *buffer, am_size static_capacity);
MAGNA_API am_bool     MAGNA_CALL fastbuf_puts      (FastBuffer *buffer, const char *text);
MAGNA_API am_bool     MAGNA_CALL fastbuf_putc      (FastBuffer *buffer, char c        );
MAGNA_API am_bool     MAGNA_CALL fastbuf_put_utf8  (FastBuffer *buffer, int c);
MAGNA_API am_size     MAGNA_CALL fastbuf_read      (FastBuffer *buffer, am_byte *data, am_size size);
MAGNA_API void        MAGNA_CALL fastbuf_rewind    (FastBuffer *buffer);
MAGNA_API am_bool     MAGNA_CALL fastbuf_write     (FastBuffer *buffer, const am_byte *data, am_size dataSize);

/*=========================================================*/

/* Навигация по тексту */

typedef struct
{
    const am_byte *data;
    am_size column, length, line, position;

} TextNavigator;

#define NAV_EOT (-1)

MAGNA_API TextNavigator* MAGNA_CALL nav_init         (TextNavigator* nav, const am_byte *data, am_size dataSize);
MAGNA_API const am_byte* MAGNA_CALL nav_end          (const TextNavigator *nav);
MAGNA_API const am_byte* MAGNA_CALL nav_current      (const TextNavigator *nav);
MAGNA_API am_bool        MAGNA_CALL nav_eot          (const TextNavigator *nav);
MAGNA_API int            MAGNA_CALL nav_at           (const TextNavigator *nav, am_size position);
MAGNA_API int            MAGNA_CALL nav_front        (const TextNavigator *nav);
MAGNA_API int            MAGNA_CALL nav_back         (const TextNavigator *nav);
MAGNA_API int            MAGNA_CALL nav_look_ahead   (const TextNavigator *nav, am_size distance);
MAGNA_API int            MAGNA_CALL nav_look_behind  (const TextNavigator *nav, am_size distance);
MAGNA_API TextNavigator* MAGNA_CALL nav_move         (TextNavigator *nav, am_ssize distance);
MAGNA_API int            MAGNA_CALL nav_peek         (const TextNavigator * nav);
MAGNA_API int            MAGNA_CALL nav_peek_no_crlf (const TextNavigator *nav);
MAGNA_API int            MAGNA_CALL nav_read         (TextNavigator *nav);
MAGNA_API int            MAGNA_CALL nav_read_no_crlf (TextNavigator *nav);

/*=========================================================*/

/* Работа с путями */

MAGNA_API am_bool MAGNA_CALL path_get_current_directory (Buffer *path);
MAGNA_API Span    MAGNA_CALL path_get_extension         (const Buffer *path);
MAGNA_API am_bool MAGNA_CALL path_set_current_directory (const Buffer *path);

/*=========================================================*/

MAGNA_API void beep (void);

#ifdef MAGNA_WINDOWS

MAGNA_API void capsoff (void);

#endif

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/

#endif
