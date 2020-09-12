/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#ifndef MAGNA_CORE_H
#define MAGNA_CORE_H

#define WIN32_LEAN_AND_MEAN /* Exclude rarely-used stuff from Windows headers */

/*=========================================================*/

#if !defined(MAGNA_WINDOWS) && !defined (MAGNA_UNIX)

    #if defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(_WINDOWS)

        #define MAGNA_WINDOWS

    #elif defined(__MSDOS__)

        #define MAGNA_MSDOS

    #else

        #define MAGNA_UNIX

        #if defined(__linux__)

            #define MAGNA_LINUX

        #endif

        #if defined(__FreeBSD__)

           #define MAGNA_FREEBSD

        #endif

        #if defined(__ANDROID__)

            #define MAGNA_ANDROID

        #endif

        #if defined(__APPLE__) || defined(__apple__) || defined(__APPLE_CC__) || defined (__OSX__)

            /* DARWIN -- OSX and iOS */

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

    #if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__) || defined(__powerpc64__) || defined(__LP64__)

        #define MAGNA_64BIT

    #elif defined(__TURBOC__)

        /* TODO: implement properly */

        #define MAGNA_16BIT

    #else

        #define MAGNA_32BIT

    #endif

#endif

/* Sanity check */
#if !defined(MAGNA_64BIT) && !defined(MAGNA_32BIT) && !defined(MAGNA_16BIT)
#error Unsupported architecture
#endif

/*=========================================================*/

/* https://ru.wikipedia.org/wiki/Restrict

  restrict — ключевое слово в языке программирования C,
  введённое стандартом C99 и используемое в объявлениях
  указателей.

    char * restrict p1;
    int ** restrict p2;
    float * restrict p3, * restrict p4;

  Ключевое слово restrict позволяет программисту сообщить компилятору,
  что объявляемый указатель указывает на блок памяти, на который
  не указывает никакой другой указатель. Гарантию того, что на один
  блок памяти не будет указывать более одного указателя, даёт
  программист. При этом оптимизирующий компилятор может генерировать
  более эффективный код.

  Использование ключевого слова restrict при объявлении других
  объектов (не указателей) не определено стандартом.

  При использовании ключевого слова restrict программа, написанная
  на «умном» C, может сравниться по скорости с программой, написанной
  на «глупом» Fortran.

  В языке C++ нет ключевого слова restrict (не описано в стандарте),
  но разработчики разных компиляторов C++ добавили аналогичные
  по назначению ключевые слова, например:

  __restrict и __restrict__ у GNU Compiler Collection[2];
  __restrict и __declspec(restrict) у Visual C++;
  __restrict__ у Clang.

 */

#if defined (__STDC_VERSION) && __STDC_VERSION__ >= 199901L

#define MAGNA_RESTRICT restrict

#elif defined(_MSC_VER)

#define MAGNA_RESTRICT __declspec(restrict)

#elif defined(__GNUC__)

#define MAGNA_RESTRICT __restrict__

#else

#define MAGNA_RESTRICT

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
#include <time.h>

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

#if defined(MAGNA_32BIT)

typedef unsigned int am_size_t;
typedef          int am_ssize_t;

#elif defined(MAGNA_64BIT)

#include <stdint.h>

typedef uint64_t am_size_t;
typedef int64_t  am_ssize_t;

#else

typedef unsigned int am_size_t;
typedef unsigned int am_ssize_t;

#endif

typedef am_uint32  am_mfn;
typedef am_uint32  am_flag;
typedef am_uint64  am_offset;
typedef void      *am_pointer;

#ifdef MAGNA_WINDOWS

typedef void *am_handle;

#define AM_BAD_HANDLE ((void*)-1)

#else

typedef int  am_handle;

#define AM_BAD_HANDLE (-1)

#endif

#define AM_FALSE 0
#define AM_TRUE  1

/*=========================================================*/

MAGNA_API am_int32            magna_get_version     (void);
MAGNA_API am_bool  MAGNA_CALL magna_require_version (am_int32 version);
MAGNA_API am_bool             magna_on_windows      (void);

/*=========================================================*/

/* Опережающее объявление */
struct MagnaArray;
struct MagnaSpanArray;
struct MagnaBuffer;

/*=========================================================*/

/* Работа с ошибками */
MAGNA_API void display_error (void);
MAGNA_API void print_error   (void);

/*=========================================================*/

/* Работа с памятью */

MAGNA_API void* MAGNA_CALL mem_alloc    (am_size_t size);
MAGNA_API void* MAGNA_CALL mem_alloc_ex (am_size_t size);
MAGNA_API void  MAGNA_CALL mem_clear    (void *ptr, am_size_t size);
MAGNA_API void  MAGNA_CALL mem_copy     (void *destination, void *source, am_size_t size);
MAGNA_API void  MAGNA_CALL mem_free     (void *ptr);

/*=========================================================*/

/* Работа с Delphi DLL */

#ifdef _M_IX86

MAGNA_API int MAGNA_CALL delphi_call_1 (void *function, int arg1);
MAGNA_API int MAGNA_CALL delphi_call_2 (void *function, int arg1, int arg2);
MAGNA_API int MAGNA_CALL delphi_call_3 (void *function, int arg1, int arg2, int arg3);
MAGNA_API int MAGNA_CALL delphi_call_4 (void *function, int arg1, int arg2, int arg3, int arg4);
MAGNA_API int MAGNA_CALL delphi_call_5 (void *function, int arg1, int arg2, int arg3, int arg4, int arg5);
MAGNA_API int MAGNA_CALL delphi_call_6 (void *function, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6);

#endif

/*=========================================================*/

/* Невладеющий фрагмент памяти */

typedef struct
{
    am_byte *ptr;
    am_size_t len;

} Span;

#define SPAN_INIT { NULL, 0 }

MAGNA_API int                    MAGNA_CALL span_compare          (Span first, Span second);
MAGNA_API am_bool                MAGNA_CALL span_ends_with        (Span span, Span suffix);
MAGNA_API Span                   MAGNA_CALL span_fill             (Span span, am_byte value);
MAGNA_API am_byte*               MAGNA_CALL span_find_byte        (Span span, am_byte value);
MAGNA_API Span                   MAGNA_CALL span_from_text        (const char *str);
MAGNA_API Span                   MAGNA_CALL span_init             (const am_byte *ptr, am_size_t len);
MAGNA_API am_ssize_t             MAGNA_CALL span_index_of         (Span span, am_byte value);
MAGNA_API am_bool                MAGNA_CALL span_is_empty         (Span span);
MAGNA_API am_ssize_t             MAGNA_CALL span_last_index_of    (Span span, am_byte value);
MAGNA_API Span                   MAGNA_CALL span_slice            (Span span, am_ssize_t start, am_ssize_t length);
MAGNA_API struct MagnaSpanArray* MAGNA_CALL span_split_by_char    (Span span, struct MagnaSpanArray *array, am_byte value);
MAGNA_API struct MagnaSpanArray* MAGNA_CALL span_split_by_chars   (Span span, struct MagnaSpanArray *array, const am_byte *values, am_size_t valueCount);
MAGNA_API am_size_t              MAGNA_CALL span_split_n_by_char  (Span span, Span *array, am_size_t arraySize, am_byte value);
MAGNA_API am_size_t              MAGNA_CALL span_split_n_by_chars (Span span, Span *array, am_size_t arraySize, const am_byte *values, am_size_t valueCount);
MAGNA_API am_bool                MAGNA_CALL span_starts_with      (Span span, Span prefix);
MAGNA_API Span                   MAGNA_CALL span_trim_start       (Span span);
MAGNA_API Span                   MAGNA_CALL span_trim_end         (Span span);
MAGNA_API Span                   MAGNA_CALL span_trim             (Span span);
MAGNA_API char*                  MAGNA_CALL span_to_string        (Span span);
MAGNA_API am_uint32              MAGNA_CALL span_to_uint_32       (Span span);
MAGNA_API am_uint64              MAGNA_CALL span_to_uint_64       (Span span);
MAGNA_API am_byte*               MAGNA_CALL span_to_vector        (Span span);
MAGNA_API Span                   MAGNA_CALL span_toupper          (Span span);
MAGNA_API Span                   MAGNA_CALL span_tolower          (Span span);

/*=========================================================*/

typedef void* (*Cloner)    (void*);
typedef void  (*Liberator) (void*);

/*=========================================================*/

/* Массив указателей на объекты */

typedef struct MagnaArray
{
    void **ptr;
    am_size_t len;
    am_size_t capacity;

    Cloner cloner;
    Liberator liberator;

} Array;

#define ARRAY_INIT { NULL, 0, 0, NULL, NULL }

MAGNA_API am_bool MAGNA_CALL array_clone      (Array *target, const Array *source);
MAGNA_API am_bool MAGNA_CALL array_concat     (Array *target, const Array *source);
MAGNA_API am_bool MAGNA_CALL array_copy       (Array *target, const Array *source);
MAGNA_API am_bool MAGNA_CALL array_create     (Array *array, am_size_t capacity);
MAGNA_API void    MAGNA_CALL array_free       (Array *array);
MAGNA_API void*   MAGNA_CALL array_get        (const Array *array, am_size_t index);
MAGNA_API am_bool MAGNA_CALL array_grow       (Array *array, am_size_t newSize);
MAGNA_API void*   MAGNA_CALL array_pop_back   (Array *array);
MAGNA_API void*   MAGNA_CALL array_pop_front  (Array *array);
MAGNA_API am_bool MAGNA_CALL array_push_back  (Array *array,  void *item);
MAGNA_API am_bool MAGNA_CALL array_push_front (Array *array,  void *item);
MAGNA_API void    MAGNA_CALL array_set        (Array *array, am_size_t index, void *item);
MAGNA_API void    MAGNA_CALL array_truncate   (Array *array, am_size_t newSize);

/*=========================================================*/

/* Динамический ассив 32-битных целых */

typedef struct
{
    am_int32 *ptr;
    am_size_t len;
    am_size_t capacity;

} Int32Array;

#define INT32_ARRAY_INIT { NULL, 0, 0 }

MAGNA_API am_bool  MAGNA_CALL int32_array_clone      (Int32Array *target, const Int32Array *source);
MAGNA_API am_bool  MAGNA_CALL int32_array_compress   (const Int32Array *array, struct MagnaBuffer *buffer);
MAGNA_API am_bool  MAGNA_CALL int32_array_concat     (Int32Array *target, const Int32Array *source);
MAGNA_API am_bool  MAGNA_CALL int32_array_copy       (Int32Array *target, const Int32Array *source);
MAGNA_API am_bool  MAGNA_CALL int32_array_create     (Int32Array *array, am_size_t capacity);
MAGNA_API void     MAGNA_CALL int32_array_free       (Int32Array *array);
MAGNA_API am_int32 MAGNA_CALL int32_array_get        (const Int32Array *array, am_size_t index);
MAGNA_API am_bool  MAGNA_CALL int32_array_grow       (Int32Array *array, am_size_t newSize);
MAGNA_API am_bool  MAGNA_CALL int32_array_shrink     (Int32Array *array);
MAGNA_API am_int32 MAGNA_CALL int32_array_pop_back   (Int32Array *array);
MAGNA_API am_int32 MAGNA_CALL int32_array_pop_front  (Int32Array *array);
MAGNA_API am_bool  MAGNA_CALL int32_array_push_back  (Int32Array *array, am_int32 value);
MAGNA_API am_bool  MAGNA_CALL int32_array_push_front (Int32Array *array, am_int32 value);
MAGNA_API void     MAGNA_CALL int32_array_set        (Int32Array *array, am_size_t index, am_int32 value);
MAGNA_API am_bool  MAGNA_CALL int32_array_to_text    (const Int32Array *array, struct MagnaBuffer *buffer, const am_byte *delimiter);
MAGNA_API void     MAGNA_CALL int32_array_truncate   (Int32Array *array, am_size_t newSize);

/*=========================================================*/

/* Динамический массив фрагментов */

typedef struct MagnaSpanArray
{
    Span *ptr;
    am_size_t len;
    am_size_t capacity;

} SpanArray;

#define SPAN_ARRAY_INIT { NULL, 0, 0 }

MAGNA_API am_bool MAGNA_CALL span_array_clone      (SpanArray *target, const SpanArray *source);
MAGNA_API am_bool MAGNA_CALL span_array_copy       (SpanArray *target, const SpanArray *source);
MAGNA_API am_bool MAGNA_CALL span_array_concat     (SpanArray *target, const SpanArray *source);
MAGNA_API am_bool MAGNA_CALL span_array_create     (SpanArray *array, am_size_t capacity);
MAGNA_API void    MAGNA_CALL span_array_free       (SpanArray *array);
MAGNA_API am_bool MAGNA_CALL span_array_from_text  (SpanArray *array, const char **text, am_size_t count);
MAGNA_API Span    MAGNA_CALL span_array_get        (const SpanArray *array, am_size_t index);
MAGNA_API am_bool MAGNA_CALL span_array_grow       (SpanArray *array, am_size_t newSize);
MAGNA_API Span    MAGNA_CALL span_array_pop_back   (SpanArray *array);
MAGNA_API Span    MAGNA_CALL span_array_pop_front  (SpanArray *array);
MAGNA_API am_bool MAGNA_CALL span_array_push_back  (SpanArray *array, Span value);
MAGNA_API am_bool MAGNA_CALL span_array_push_front (SpanArray *array, Span value);
MAGNA_API void    MAGNA_CALL span_array_set        (SpanArray *array, am_size_t index, Span value);
MAGNA_API am_bool MAGNA_CALL span_array_shrink     (SpanArray *array);
MAGNA_API am_bool MAGNA_CALL span_array_to_text    (const SpanArray *array, struct MagnaBuffer *buffer, const am_byte *delimiter);
MAGNA_API void    MAGNA_CALL span_array_truncate   (SpanArray *array, am_size_t newSize);

/*=========================================================*/

/* Буфер - замена строки */
typedef struct MagnaBuffer
{
    am_byte *ptr;
    am_size_t position;
    am_size_t capacity;

} Buffer;

#define BUFFER_INIT { NULL, 0, 0 }

MAGNA_API am_bool        MAGNA_CALL buffer_ansi_to_utf8   (Buffer *target, const Buffer *source);
MAGNA_API am_bool        MAGNA_CALL buffer_assign         (Buffer *buffer, const am_byte *data, am_size_t length);
MAGNA_API am_bool        MAGNA_CALL buffer_assign_span    (Buffer *buffer, const Span span);
MAGNA_API am_bool        MAGNA_CALL buffer_assign_text    (Buffer *buffer, const char *text);
MAGNA_API am_size_t      MAGNA_CALL buffer_calculate_size (am_size_t newSize);
MAGNA_API Buffer*        MAGNA_CALL buffer_clone          (Buffer *target, const Buffer *source);
MAGNA_API int            MAGNA_CALL buffer_compare        (const Buffer *first, const Buffer *second);
MAGNA_API int            MAGNA_CALL buffer_compare_text   (const Buffer *buffer, const am_byte *text);
MAGNA_API am_bool        MAGNA_CALL buffer_concat         (Buffer *target, const Buffer *source);
MAGNA_API am_bool        MAGNA_CALL buffer_copy           (Buffer *target, const Buffer *source);
MAGNA_API am_bool        MAGNA_CALL buffer_create         (Buffer *buffer, am_byte *data, am_size_t length);
MAGNA_API am_byte*       MAGNA_CALL buffer_find_text      (const Buffer *buffer, const am_byte *text);
MAGNA_API void           MAGNA_CALL buffer_free           (Buffer *buffer);
MAGNA_API Buffer*        MAGNA_CALL buffer_from_span      (Buffer *buffer, Span span);
MAGNA_API Buffer*        MAGNA_CALL buffer_from_text      (Buffer *buffer, const char *text);
MAGNA_API am_bool        MAGNA_CALL buffer_grow           (Buffer *buffer, am_size_t newSize);
MAGNA_API am_bool        MAGNA_CALL buffer_new_line       (Buffer *buffer);
MAGNA_API Buffer*        MAGNA_CALL buffer_null           (Buffer *buffer);
MAGNA_API am_bool        MAGNA_CALL buffer_put_uint_32    (Buffer *buffer, am_uint32 value);
MAGNA_API am_bool        MAGNA_CALL buffer_putc           (Buffer *buffer, char c);
MAGNA_API am_bool        MAGNA_CALL buffer_puts           (Buffer *buffer, const char *str);
MAGNA_API am_bool        MAGNA_CALL buffer_replace_text   (Buffer *buffer, const am_byte *from, const am_byte *to);
MAGNA_API Buffer*        MAGNA_CALL buffer_static         (Buffer *buffer, am_byte *data, am_size_t newSize);
MAGNA_API Buffer*        MAGNA_CALL buffer_swap           (Buffer *first, Buffer *second);
MAGNA_API const am_byte* MAGNA_CALL buffer_to_text        (Buffer *buffer);
MAGNA_API Span           MAGNA_CALL buffer_to_span        (const Buffer *buffer);
MAGNA_API am_bool        MAGNA_CALL buffer_utf8_to_ansi   (Buffer *target, const Buffer *source);
MAGNA_API am_bool        MAGNA_CALL buffer_write          (Buffer *target, const am_byte *data, am_size_t length);

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
    am_size_t chunkSize, position, read;

} ChunkedBuffer;

MAGNA_API Buffer*          MAGNA_CALL chunked_all            (const ChunkedBuffer *chunked, Buffer *buffer);
MAGNA_API am_size_t        MAGNA_CALL chunked_capacity       (const ChunkedBuffer *chunked);
MAGNA_API am_bool          MAGNA_CALL chunked_eof            (const ChunkedBuffer *chunked);
MAGNA_API void             MAGNA_CALL chunked_free           (ChunkedBuffer *chunked);
MAGNA_API am_bool          MAGNA_CALL chunked_grow           (ChunkedBuffer *chunked, am_size_t newSize);
MAGNA_API ChunkedBuffer*   MAGNA_CALL chunked_init           (ChunkedBuffer *chunked, am_size_t chunkSize);
MAGNA_API int              MAGNA_CALL chunked_peek           (const ChunkedBuffer *chunked);
MAGNA_API am_size_t        MAGNA_CALL chunked_position       (const ChunkedBuffer *chunked);
MAGNA_API int              MAGNA_CALL chunked_read_byte      (ChunkedBuffer *chunked);
MAGNA_API am_size_t        MAGNA_CALL chunked_read           (ChunkedBuffer *chunked, Buffer *buffer, am_size_t count);
MAGNA_API am_size_t        MAGNA_CALL chunked_read_line      (ChunkedBuffer *chunked, Buffer *buffer);
MAGNA_API am_size_t        MAGNA_CALL chunked_read_remaining (ChunkedBuffer *chunked, Buffer *buffer);
MAGNA_API am_size_t        MAGNA_CALL chunked_remaining_size (const ChunkedBuffer *chunked);
MAGNA_API ChunkedBuffer*   MAGNA_CALL chunked_rewind         (ChunkedBuffer *chunked);
MAGNA_API am_size_t        MAGNA_CALL chunked_size           (const ChunkedBuffer *chunked);
MAGNA_API am_bool          MAGNA_CALL chunked_write          (ChunkedBuffer *chunked, am_byte *data, am_size_t dataSize);
MAGNA_API am_bool          MAGNA_CALL chunked_write_byte     (ChunkedBuffer *chunked, am_byte value);
MAGNA_API am_bool          MAGNA_CALL chunked_write_text     (ChunkedBuffer *chunked, const char *text);

/*=========================================================*/

/* Буфер, хранящий часть данных на стеке */

typedef struct
{
    am_byte *static_data;
    am_byte *dynamic_data;
    am_size_t capacity;
    am_size_t size;
    am_size_t static_capacity;
} FastBuffer;

MAGNA_API FastBuffer*   MAGNA_CALL fastbuf_clear     (FastBuffer *buffer);
MAGNA_API am_bool       MAGNA_CALL fastbuf_empty     (const FastBuffer *buffer);
MAGNA_API void          MAGNA_CALL fastbuf_free      (FastBuffer *buffer);
MAGNA_API am_int32      MAGNA_CALL fastbuf_getc      (FastBuffer *buffer);
MAGNA_API am_int32      MAGNA_CALL fastbuf_getc_utf8 (FastBuffer *buffer);
MAGNA_API am_size_t     MAGNA_CALL fastbuf_gets      (FastBuffer *buffer, Buffer *text);
MAGNA_API am_bool       MAGNA_CALL fastbuf_grow      (FastBuffer *buffer, am_size_t newSize);
MAGNA_API void          MAGNA_CALL fastbuf_init      (FastBuffer *buffer, am_size_t static_capacity);
MAGNA_API am_bool       MAGNA_CALL fastbuf_puts      (FastBuffer *buffer, const char *text);
MAGNA_API am_bool       MAGNA_CALL fastbuf_putc      (FastBuffer *buffer, char c        );
MAGNA_API am_bool       MAGNA_CALL fastbuf_put_utf8  (FastBuffer *buffer, int c);
MAGNA_API am_size_t     MAGNA_CALL fastbuf_read      (FastBuffer *buffer, am_byte *data, am_size_t size);
MAGNA_API void          MAGNA_CALL fastbuf_rewind    (FastBuffer *buffer);
MAGNA_API am_bool       MAGNA_CALL fastbuf_write     (FastBuffer *buffer, const am_byte *data, am_size_t dataSize);

/*=========================================================*/

/* Навигация по тексту */

typedef struct
{
    const am_byte *data;
    am_size_t column, length, line, position;

} TextNavigator;

/**
 * Признак достигнутого конца данных.
 */
#define NAV_EOT (-1)

MAGNA_API int            MAGNA_CALL nav_at               (const TextNavigator *nav, am_size_t position);
MAGNA_API int            MAGNA_CALL nav_back             (const TextNavigator *nav);
MAGNA_API const am_byte* MAGNA_CALL nav_current          (const TextNavigator *nav);
MAGNA_API const am_byte* MAGNA_CALL nav_end              (const TextNavigator *nav);
MAGNA_API am_bool        MAGNA_CALL nav_eot              (const TextNavigator *nav);
MAGNA_API Span           MAGNA_CALL nav_extract_integer  (TextNavigator *nav);
MAGNA_API TextNavigator* MAGNA_CALL nav_from_buffer      (TextNavigator *nav, const Buffer *buffer);
MAGNA_API TextNavigator* MAGNA_CALL nav_from_span        (TextNavigator *nav, Span span);
MAGNA_API TextNavigator* MAGNA_CALL nav_from_text        (TextNavigator *nav, const char *text);
MAGNA_API int            MAGNA_CALL nav_front            (const TextNavigator *nav);
MAGNA_API TextNavigator* MAGNA_CALL nav_init             (TextNavigator* nav, const am_byte *data, am_size_t dataSize);
MAGNA_API am_bool        MAGNA_CALL nav_is_control       (const TextNavigator *nav);
MAGNA_API am_bool        MAGNA_CALL nav_is_digit         (const TextNavigator *nav);
MAGNA_API am_bool        MAGNA_CALL nav_is_letter        (const TextNavigator *nav);
MAGNA_API am_bool        MAGNA_CALL nav_is_whitespace    (const TextNavigator *nav);
MAGNA_API int            MAGNA_CALL nav_look_ahead       (const TextNavigator *nav, am_size_t distance);
MAGNA_API int            MAGNA_CALL nav_look_behind      (const TextNavigator *nav, am_size_t distance);
MAGNA_API TextNavigator* MAGNA_CALL nav_move             (TextNavigator *nav, am_ssize_t distance);
MAGNA_API int            MAGNA_CALL nav_peek             (const TextNavigator * nav);
MAGNA_API int            MAGNA_CALL nav_peek_no_crlf     (const TextNavigator *nav);
MAGNA_API Span           MAGNA_CALL nav_peek_string      (const TextNavigator *nav, am_size_t length);
MAGNA_API Span           MAGNA_CALL nav_peek_to          (const TextNavigator *nav, am_byte stopChar);
MAGNA_API Span           MAGNA_CALL nav_peek_until       (const TextNavigator *nav, am_byte stopChar);
MAGNA_API int            MAGNA_CALL nav_read             (TextNavigator *nav);
MAGNA_API Span           MAGNA_CALL nav_read_integer     (TextNavigator *nav);
MAGNA_API Span           MAGNA_CALL nav_read_line        (TextNavigator *nav);
MAGNA_API int            MAGNA_CALL nav_read_no_crlf     (TextNavigator *nav);
MAGNA_API Span           MAGNA_CALL nav_read_string      (TextNavigator *nav, am_size_t length);
MAGNA_API Span           MAGNA_CALL nav_read_to          (TextNavigator *nav, am_byte stopChar);
MAGNA_API Span           MAGNA_CALL nav_read_until       (TextNavigator *nav, am_byte stopChar);
MAGNA_API int            MAGNA_CALL nav_read_utf8        (TextNavigator *nav);
MAGNA_API Span           MAGNA_CALL nav_read_while       (TextNavigator *nav, am_byte goodChar);
MAGNA_API Span           MAGNA_CALL nav_read_word        (TextNavigator *nav);
MAGNA_API Span           MAGNA_CALL nav_recent           (const TextNavigator *nav, am_ssize_t length);
MAGNA_API Span           MAGNA_CALL nav_remaining        (const TextNavigator *nav);
MAGNA_API Span           MAGNA_CALL nav_to_span          (const TextNavigator *nav);
MAGNA_API void           MAGNA_CALL nav_skip_non_word    (TextNavigator *nav);
MAGNA_API void           MAGNA_CALL nav_skip_punctuation (TextNavigator *nav);
MAGNA_API void           MAGNA_CALL nav_skip_whitespace  (TextNavigator *nav);
MAGNA_API Span           MAGNA_CALL nav_slice            (const TextNavigator *nav, am_size_t offset, am_size_t size);

/*=========================================================*/

/* Текст, содержащий фрагменты с числами */

typedef struct {
    Buffer prefix;
    am_int64 value;
    int length;
    am_bool haveValue;

} NumberTextChunk;

typedef struct {
    Array  chunks;

} NumberText;

/*=========================================================*/

/* Работа с файлами */

MAGNA_API am_bool    MAGNA_CALL file_close        (am_handle handle);
MAGNA_API am_handle  MAGNA_CALL file_create       (const char *fileName);
MAGNA_API am_bool    MAGNA_CALL file_eof          (am_handle handle);
MAGNA_API am_handle  MAGNA_CALL file_open_read    (const char *fileName);
MAGNA_API am_handle  MAGNA_CALL file_open_write   (const char *fileName);
MAGNA_API am_ssize_t MAGNA_CALL file_read         (am_handle handle, am_byte *buffer, am_ssize_t size);
MAGNA_API am_ssize_t MAGNA_CALL file_read_all     (am_handle handle, Buffer *buffer);
MAGNA_API int        MAGNA_CALL file_read_byte    (am_handle handle);
MAGNA_API am_uint32  MAGNA_CALL file_read_int_32  (am_handle handle);
MAGNA_API am_uint64  MAGNA_CALL file_read_int_64  (am_handle handle);
MAGNA_API am_bool    MAGNA_CALL file_read_line    (am_handle handle, Buffer *buffer);
MAGNA_API am_bool    MAGNA_CALL file_seek         (am_handle handle, am_int64 offset);
MAGNA_API am_uint64  MAGNA_CALL file_size         (am_handle handle);
MAGNA_API am_int64   MAGNA_CALL file_tell         (am_handle handle);
MAGNA_API am_bool    MAGNA_CALL file_write        (am_handle handle, const am_byte *data, am_size_t size);
MAGNA_API am_bool    MAGNA_CALL file_write_buffer (am_handle handle, const Buffer *buffer);
MAGNA_API am_bool    MAGNA_CALL file_write_byte   (am_handle handle, am_byte value);
MAGNA_API am_bool    MAGNA_CALL file_write_int_32 (am_handle handle, am_uint32 value);
MAGNA_API am_bool    MAGNA_CALL file_write_int_64 (am_handle handle, am_uint64 value);
MAGNA_API am_bool    MAGNA_CALL file_write_span   (am_handle handle, const Span span);
MAGNA_API am_bool    MAGNA_CALL file_write_text   (am_handle handle, const char *text);

/*=========================================================*/

/* Работа с путями */

MAGNA_API am_bool MAGNA_CALL path_get_current_directory (Buffer *path);
MAGNA_API Span    MAGNA_CALL path_get_extension         (const Buffer *path);
MAGNA_API am_bool MAGNA_CALL path_set_current_directory (const Buffer *path);
MAGNA_API am_bool MAGNA_CALL path_to_executable         (Buffer *buffer);

/*=========================================================*/

/* Работа со строками */

MAGNA_API am_uint32   MAGNA_CALL fastParse32      (const am_byte *text, am_size_t length);
MAGNA_API am_bool     MAGNA_CALL same_char        (int first, int second);
MAGNA_API am_bool     MAGNA_CALL same_text        (const char *first, const char *second);
MAGNA_API char*       MAGNA_CALL str_dup          (const char *text);
MAGNA_API am_bool     MAGNA_CALL char_one_of      (am_byte one, const am_byte *many);
MAGNA_API am_bool                str_one_of       (const char *one, ...);
MAGNA_API int         MAGNA_CALL str_safe_compare (const char *first, const char *second);
MAGNA_API const char* MAGNA_CALL str_to_visible   (const char *text);

/*=========================================================*/

/* Работа с кодировками */

typedef struct
{
    const char *name;
    unsigned (MAGNA_CALL *char_to_unicode) (am_byte);
    am_byte (MAGNA_CALL *unicode_to_char) (unsigned);

} Encoding;

extern Encoding cp1251_encoding, cp866_encoding, koi8r_encoding;

MAGNA_API am_bool   MAGNA_CALL encoding_register (const Encoding *encoding);
MAGNA_API Encoding* MAGNA_CALL encoding_get      (const char *name);
MAGNA_API Encoding*            encoding_ansi     (void);

MAGNA_API am_bool    MAGNA_CALL buffer_putc_utf8 (Buffer *buffer, unsigned chr);
MAGNA_API am_size_t  MAGNA_CALL utf8_code_points (const am_byte *data, am_size_t dataLength);
MAGNA_API am_ssize_t MAGNA_CALL search_for_unicode (am_wchar *array, am_ssize_t left, am_ssize_t right, am_wchar value);

MAGNA_API unsigned MAGNA_CALL cp1251_char_to_unicode  (am_byte chr);
MAGNA_API am_bool  MAGNA_CALL cp1251_char_to_utf8     (Buffer *buffer, am_byte chr);
MAGNA_API am_bool  MAGNA_CALL cp1251_buffer_to_utf8   (Buffer *target, const Buffer *source);
MAGNA_API am_byte  MAGNA_CALL cp1251_unicode_to_char  (unsigned uchr);
MAGNA_API am_bool  MAGNA_CALL cp1251_buffer_from_utf8 (Buffer *target, const Buffer *source);

MAGNA_API unsigned MAGNA_CALL cp866_char_to_unicode  (am_byte chr);
MAGNA_API am_byte  MAGNA_CALL cp866_unicode_to_char  (unsigned uchr);
MAGNA_API am_bool  MAGNA_CALL cp866_char_to_utf8     (Buffer *buffer, am_byte chr);
MAGNA_API am_bool  MAGNA_CALL cp866_buffer_to_utf8   (Buffer *target, const Buffer *source);
MAGNA_API am_bool  MAGNA_CALL cp866_buffer_from_utf8 (Buffer *target, const Buffer *source);

MAGNA_API unsigned MAGNA_CALL koi8r_char_to_unicode  (am_byte chr);
MAGNA_API am_byte  MAGNA_CALL koi8r_unicode_to_char  (unsigned uchr);
MAGNA_API am_bool  MAGNA_CALL koi8r_char_to_utf8     (Buffer *buffer, am_byte chr);
MAGNA_API am_bool  MAGNA_CALL koi8r_buffer_to_utf8   (Buffer *target, const Buffer *source);
MAGNA_API am_bool  MAGNA_CALL koi8r_buffer_from_utf8 (Buffer *target, const Buffer *source);

/*=========================================================*/

/* Работа с сетью */

MAGNA_API am_int32   MAGNA_CALL tcp4_connect            (const char *hostname, am_uint16 port);
MAGNA_API am_bool    MAGNA_CALL tcp4_disconnect         (am_int32 handle);
MAGNA_API am_bool               tcp4_initialize         (void);
MAGNA_API am_ssize_t MAGNA_CALL tcp4_receive_all        (am_int32 handle, Buffer *buffer);
MAGNA_API am_ssize_t MAGNA_CALL tcp4_receive_with_limit (am_int32 handle, Buffer *buffer, am_ssize_t limit);
MAGNA_API am_ssize_t MAGNA_CALL tcp4_send               (am_int32 handle, const am_byte *data, am_ssize_t dataLength);

/*=========================================================*/

/* Работа с датой */

MAGNA_API am_bool  MAGNA_CALL date_decode  (const struct tm *date, int *year, int *month, int *day);
MAGNA_API am_bool  MAGNA_CALL date_init    (struct tm *date, int year, int month, int day);
MAGNA_API am_bool  MAGNA_CALL date_parse   (struct tm *date, Span text);
MAGNA_API am_byte* MAGNA_CALL date_to_text (const struct tm *date, am_byte *buffer);
MAGNA_API am_bool  MAGNA_CALL date_today   (struct tm *date);

/*=========================================================*/

/* Настойчивое исполнение */

typedef struct MagnaRetryManager RetryManager;

typedef void* (MAGNA_CALL *Action) (void *data);
typedef am_bool (MAGNA_CALL *Handler) (RetryManager *manager);

struct MagnaRetryManager
{
    Handler onError;
    Handler onSuccess;
    unsigned sleepInterval;
    unsigned retryLimit;
    void *data;
};

MAGNA_API void  MAGNA_CALL retry_init   (RetryManager *manager);
MAGNA_API void* MAGNA_CALL retry_action (RetryManager *manager, Action action, void *data);

/*=========================================================*/

/* Работа с потоками */

MAGNA_API am_handle MAGNA_CALL thread_create (void *start);
MAGNA_API am_bool   MAGNA_CALL thread_join   (am_handle handle, am_int32 timeout);

/*=========================================================*/

/* Прочие функции */

MAGNA_API void beep (void);
MAGNA_API void MAGNA_CALL magna_sleep (unsigned interval);

#ifdef MAGNA_WINDOWS

MAGNA_API void capsoff (void);

#endif

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/

#endif
