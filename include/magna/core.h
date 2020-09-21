// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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

#ifndef MAGNA_INLINE

//#ifdef _MSC_VER
//#if _MSC_VER >= 1910
//#define MAGNA_INLINE inline
//#endif
//#endif

#ifdef __GNUC__
#define MAGNA_INLINE __attribute__ ((gnu_inline)) inline
#endif

#endif

#ifndef MAGNA_INLINE
#define MAGNA_INLINE /* nothing */
#endif

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#if _MSC_VER >= 1910
#pragma warning(disable: 5045)
#endif
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

#define AM_FALSE 0
#define AM_TRUE  1

/*=========================================================*/

/* Простая обертка над Win32 HANDLE */

typedef union {
    void *pointer;
    int  value;
} am_handle;

#ifdef MAGNA_WINDOWS

#define AM_BAD_HANDLE ((void*)-1)

#else

#define AM_BAD_HANDLE (-1)

#endif

extern MAGNA_API MAGNA_INLINE am_bool   MAGNA_CALL handle_is_good      (am_handle handle);
extern MAGNA_API MAGNA_INLINE am_handle MAGNA_CALL handle_from_pointer (void *pointer);
extern MAGNA_API MAGNA_INLINE am_handle MAGNA_CALL handle_from_value   (int value);

/*=========================================================*/

/* Свалка всех типов в одном union */

typedef union {
    void     *pointer;
    am_byte   byte;
    am_int16  int16;
    am_int32  int32;
    am_int64  int64;
    am_uint16 uint16;
    am_uint32 uint32;
    am_uint64 uint64;
    am_size_t size;

    struct {
        am_byte byte0;
        am_byte byte1;
        am_byte byte2;
        am_byte byte3;
        am_byte byte4;
        am_byte byte5;
        am_byte byte6;
        am_byte byte7;
    } eightBytes;

    struct {
        am_uint16 word0;
        am_uint16 word1;
        am_uint16 word2;
        am_uint16 word3;
    } fourWords;

    struct {
        am_uint32 dword0;
        am_uint32 dword1;
    } twoDwords;
} am_mix;

/*=========================================================*/

MAGNA_API am_int32            magna_get_version     (void);
MAGNA_API am_bool  MAGNA_CALL magna_require_version (am_int32 version);
MAGNA_API am_bool             magna_on_windows      (void);

/*=========================================================*/

/* Опережающее объявление */

typedef struct MagnaArray       Array;
typedef struct MagnaBuffer      Buffer;
typedef struct MangaEnumerator  Enumerator;
typedef struct MagnaMemoryChunk MemoryChunk;
typedef struct MagnaSpan        Span;
typedef struct MagnaSpanArray   SpanArray;
typedef struct MagnaStream      Stream;

/*=========================================================*/

/* Работа с ошибками */

MAGNA_API void display_error (void);
MAGNA_API void print_error   (void);

/*=========================================================*/

/* Мелкие утилиты */

MAGNA_API MAGNA_INLINE am_int32  MAGNA_CALL min_int32        (am_int32  first, am_int32  second);
MAGNA_API MAGNA_INLINE am_int32  MAGNA_CALL max_int32        (am_int32  first, am_int32  second);
MAGNA_API MAGNA_INLINE am_int32  MAGNA_CALL min_uint32       (am_uint32 first, am_uint32 second);
MAGNA_API MAGNA_INLINE am_uint32 MAGNA_CALL max_uint32       (am_uint32 first, am_uint32 second);
MAGNA_API MAGNA_INLINE am_int64  MAGNA_CALL min_int64        (am_int64  first, am_int64  second);
MAGNA_API MAGNA_INLINE am_int64  MAGNA_CALL max_int64        (am_int64  first, am_int64  second);
MAGNA_API MAGNA_INLINE am_uint64 MAGNA_CALL min_uint64       (am_uint64 first, am_uint64 second);
MAGNA_API MAGNA_INLINE am_uint64 MAGNA_CALL max_uint64       (am_uint64 first, am_uint64 second);
MAGNA_API MAGNA_INLINE am_size_t MAGNA_CALL min_size_t       (am_size_t first, am_size_t second);
MAGNA_API MAGNA_INLINE am_size_t MAGNA_CALL max_size_t       (am_size_t first, am_size_t second);
MAGNA_API MAGNA_INLINE int       MAGNA_CALL magna_sign_int   (int value);
MAGNA_API MAGNA_INLINE int       MAGNA_CALL magna_sign_int32 (am_int32 value);
MAGNA_API MAGNA_INLINE int       MAGNA_CALL magna_sign_int64 (am_int64 value);

MAGNA_API const char* choose_string (const char *first, ...);
MAGNA_API Span        choose_span   (Span first, ...);
MAGNA_API Buffer*     choose_buffer (const Buffer *first, ...);

/*=========================================================*/

/* Ввод-вывод */

extern MAGNA_API MAGNA_INLINE am_uint16 MAGNA_CALL magna_ntohs     (am_uint16 value);
extern MAGNA_API MAGNA_INLINE am_uint32 MAGNA_CALL magna_ntohl     (am_uint32 value);
extern MAGNA_API              am_uint64 MAGNA_CALL irbis_decode_64 (am_uint32 junior, am_uint32 senior);
extern MAGNA_API              void      MAGNA_CALL irbis_encode_64 (am_uint32 *buffer, am_uint64 value);

/*=========================================================*/

/* Работа с памятью */

MAGNA_API void*     MAGNA_CALL mem_alloc           (am_size_t size);
MAGNA_API void*     MAGNA_CALL mem_alloc_ex        (am_size_t size);
MAGNA_API void      MAGNA_CALL mem_clear           (void *ptr, am_size_t size);
MAGNA_API void      MAGNA_CALL mem_copy            (void *destination, const void *source, am_size_t size);
MAGNA_API void      MAGNA_CALL mem_free            (void *ptr);
MAGNA_API am_uint64            mem_avail_physical  (void);
MAGNA_API am_uint64            mem_avail_virtual   (void);
MAGNA_API am_uint64            mem_total_installed (void);
MAGNA_API am_uint64            mem_total_virtual   (void);

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

/* Невладеющий непрерывный фрагмент памяти */

struct MagnaSpan
{
    am_byte *ptr;
    am_size_t len;

};

#define SPAN_INIT { NULL, 0 }

extern MAGNA_API              int        MAGNA_CALL span_compare          (Span first, Span second);
extern MAGNA_API              am_bool    MAGNA_CALL span_ends_with        (Span span, Span suffix);
extern MAGNA_API              Span       MAGNA_CALL span_fill             (Span span, am_byte value);
extern MAGNA_API              am_byte*   MAGNA_CALL span_find_byte        (Span span, am_byte value);
extern MAGNA_API MAGNA_INLINE Span       MAGNA_CALL span_from_text        (const char *str);
extern MAGNA_API MAGNA_INLINE Span       MAGNA_CALL span_init             (const am_byte *ptr, am_size_t len);
extern MAGNA_API              am_ssize_t MAGNA_CALL span_index_of         (Span span, am_byte value);
extern MAGNA_API MAGNA_INLINE am_bool    MAGNA_CALL span_is_empty         (Span span);
extern MAGNA_API              am_ssize_t MAGNA_CALL span_last_index_of    (Span span, am_byte value);
extern MAGNA_API MAGNA_INLINE Span                  span_null             (void);
extern MAGNA_API MAGNA_INLINE Span       MAGNA_CALL span_slice            (Span span, am_ssize_t start, am_ssize_t length);
extern MAGNA_API              am_bool    MAGNA_CALL span_split_by_char    (Span span, SpanArray *array, am_byte value);
extern MAGNA_API              am_bool    MAGNA_CALL span_split_by_chars   (Span span, SpanArray *array, const am_byte *values, am_size_t valueCount);
extern MAGNA_API              am_size_t  MAGNA_CALL span_split_n_by_char  (Span span, Span *array, am_size_t arraySize, am_byte value);
extern MAGNA_API              am_size_t  MAGNA_CALL span_split_n_by_chars (Span span, Span *array, am_size_t arraySize, const am_byte *values, am_size_t valueCount);
extern MAGNA_API              am_bool    MAGNA_CALL span_starts_with      (Span span, Span prefix);
extern MAGNA_API              Span       MAGNA_CALL span_trim_start       (Span span);
extern MAGNA_API              Span       MAGNA_CALL span_trim_end         (Span span);
extern MAGNA_API              Span       MAGNA_CALL span_trim             (Span span);
extern MAGNA_API              char*      MAGNA_CALL span_to_string        (Span span);
extern MAGNA_API              am_uint32  MAGNA_CALL span_to_uint_32       (Span span);
extern MAGNA_API              am_uint64  MAGNA_CALL span_to_uint_64       (Span span);
extern MAGNA_API              am_byte*   MAGNA_CALL span_to_vector        (Span span);
extern MAGNA_API              Span       MAGNA_CALL span_toupper          (Span span);
extern MAGNA_API              Span       MAGNA_CALL span_tolower          (Span span);

/*=========================================================*/

typedef void* (*Cloner)    (void*);
typedef void  (*Liberator) (void*);

/*=========================================================*/

/* Массив указателей на объекты */

struct MagnaArray
{
    void **ptr;
    am_size_t len;
    am_size_t capacity;

    Cloner cloner;
    Liberator liberator;

};

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

struct MagnaSpanArray
{
    Span *ptr;
    am_size_t len;
    am_size_t capacity;

};

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

struct MagnaBuffer
{
    am_byte *ptr;
    am_size_t position;
    am_size_t capacity;

};

#define BUFFER_INIT { NULL, 0, 0 }

MAGNA_API am_bool        MAGNA_CALL buffer_ansi_to_utf8              (Buffer *target, const Buffer *source);
MAGNA_API am_bool        MAGNA_CALL buffer_assign                    (Buffer *buffer, const am_byte *data, am_size_t length);
MAGNA_API am_bool        MAGNA_CALL buffer_assign_span               (Buffer *buffer, const Span span);
MAGNA_API am_bool        MAGNA_CALL buffer_assign_text               (Buffer *buffer, const char *text);
MAGNA_API am_size_t      MAGNA_CALL buffer_calculate_size            (am_size_t newSize);
MAGNA_API Buffer*        MAGNA_CALL buffer_clear                     (Buffer *buffer);
MAGNA_API Buffer*        MAGNA_CALL buffer_clone                     (Buffer *target, const Buffer *source);
MAGNA_API int            MAGNA_CALL buffer_compare                   (const Buffer *first, const Buffer *second);
MAGNA_API int            MAGNA_CALL buffer_compare_span              (const Buffer *buffer, Span span);
MAGNA_API int            MAGNA_CALL buffer_compare_span_ignore_case  (const Buffer *buffer, Span span);
MAGNA_API int            MAGNA_CALL buffer_compare_text              (const Buffer *buffer, const am_byte *text);
MAGNA_API int            MAGNA_CALL buffer_compare_text_ignore_case  (const Buffer *buffer, const am_byte *text);
MAGNA_API am_bool        MAGNA_CALL buffer_concat                    (Buffer *target, const Buffer *source);
MAGNA_API am_bool        MAGNA_CALL buffer_copy                      (Buffer *target, const Buffer *source);
MAGNA_API am_bool        MAGNA_CALL buffer_create                    (Buffer *buffer, const am_byte *data, am_size_t length);
MAGNA_API am_bool        MAGNA_CALL buffer_empty                     (const Buffer *buffer);
MAGNA_API am_byte*       MAGNA_CALL buffer_find_text                 (const Buffer *buffer, const am_byte *text);
MAGNA_API void           MAGNA_CALL buffer_free                      (Buffer *buffer);
MAGNA_API Buffer*        MAGNA_CALL buffer_from_span                 (Buffer *buffer, Span span);
MAGNA_API Buffer*        MAGNA_CALL buffer_from_text                 (Buffer *buffer, const char *text);
MAGNA_API am_bool        MAGNA_CALL buffer_grow                      (Buffer *buffer, am_size_t newSize);
MAGNA_API Buffer*        MAGNA_CALL buffer_init                      (Buffer *buffer);
MAGNA_API am_bool        MAGNA_CALL buffer_new_line                  (Buffer *buffer);
MAGNA_API Buffer*        MAGNA_CALL buffer_null                      (Buffer *buffer);
MAGNA_API am_bool        MAGNA_CALL buffer_put_uint_32               (Buffer *buffer, am_uint32 value);
MAGNA_API am_bool        MAGNA_CALL buffer_put_uint_64               (Buffer *buffer, am_uint64 value);
MAGNA_API am_bool        MAGNA_CALL buffer_putc                      (Buffer *buffer, char c);
MAGNA_API am_bool        MAGNA_CALL buffer_puts                      (Buffer *buffer, const char *str);
MAGNA_API am_size_t      MAGNA_CALL buffer_read                      (Buffer *buffer, am_byte *data, am_size_t length);
MAGNA_API am_bool        MAGNA_CALL buffer_replace_text              (Buffer *buffer, const am_byte *from, const am_byte *to);
MAGNA_API Buffer*        MAGNA_CALL buffer_static                    (Buffer *buffer, const am_byte *data, am_size_t length);
MAGNA_API Buffer*        MAGNA_CALL buffer_swap                      (Buffer *first, Buffer *second);
MAGNA_API const am_byte* MAGNA_CALL buffer_to_text                   (Buffer *buffer);
MAGNA_API Span           MAGNA_CALL buffer_to_span                   (const Buffer *buffer);
MAGNA_API am_bool        MAGNA_CALL buffer_utf8_to_ansi              (Buffer *target, const Buffer *source);
MAGNA_API am_bool        MAGNA_CALL buffer_write                     (Buffer *target, const am_byte *data, am_size_t length);

/*=========================================================*/

/* Блок памяти */

struct MagnaMemoryChunk
{
    am_byte *data;
    MemoryChunk *next;

};

/* Буфер, состоящий из блоков */

typedef struct
{
    MemoryChunk *first, *current, *last;
    am_size_t chunkSize, position, read;

} ChunkedBuffer;

MAGNA_API am_ssize_t       MAGNA_CALL chunked_all            (const ChunkedBuffer *chunked, Buffer *buffer);
MAGNA_API am_bool          MAGNA_CALL chunked_ansi_to_utf8   (ChunkedBuffer *target, const Buffer *source);
MAGNA_API am_byte*         MAGNA_CALL chunked_at             (const ChunkedBuffer *chunked, am_size_t offset);
MAGNA_API am_size_t        MAGNA_CALL chunked_capacity       (const ChunkedBuffer *chunked);
MAGNA_API am_bool          MAGNA_CALL chunked_empty          (const ChunkedBuffer *chunked);
MAGNA_API am_bool          MAGNA_CALL chunked_eof            (const ChunkedBuffer *chunked);
MAGNA_API void             MAGNA_CALL chunked_free           (ChunkedBuffer *chunked);
MAGNA_API am_bool          MAGNA_CALL chunked_grow           (ChunkedBuffer *chunked, am_size_t newSize);
MAGNA_API ChunkedBuffer*   MAGNA_CALL chunked_init           (ChunkedBuffer *chunked, am_size_t chunkSize);
MAGNA_API am_bool          MAGNA_CALL chunked_new_line       (ChunkedBuffer *chunked);
MAGNA_API int              MAGNA_CALL chunked_peek           (ChunkedBuffer *chunked);
MAGNA_API am_size_t        MAGNA_CALL chunked_position       (const ChunkedBuffer *chunked);
MAGNA_API int              MAGNA_CALL chunked_read_byte      (ChunkedBuffer *chunked);
MAGNA_API am_ssize_t       MAGNA_CALL chunked_read           (ChunkedBuffer *chunked, Buffer *buffer, am_size_t count);
MAGNA_API am_ssize_t       MAGNA_CALL chunked_read_line      (ChunkedBuffer *chunked, Buffer *buffer);
MAGNA_API am_ssize_t       MAGNA_CALL chunked_read_raw       (ChunkedBuffer *chunked, am_byte *buffer, am_size_t count);
MAGNA_API am_ssize_t       MAGNA_CALL chunked_read_remaining (const ChunkedBuffer *chunked, Buffer *buffer);
MAGNA_API int              MAGNA_CALL chunked_read_utf8      (ChunkedBuffer *chunked);
MAGNA_API am_size_t        MAGNA_CALL chunked_remaining_size (const ChunkedBuffer *chunked);
MAGNA_API ChunkedBuffer*   MAGNA_CALL chunked_rewind         (ChunkedBuffer *chunked);
MAGNA_API am_size_t        MAGNA_CALL chunked_size           (const ChunkedBuffer *chunked);
MAGNA_API am_bool          MAGNA_CALL chunked_utf8_to_ansi   (ChunkedBuffer *target, const Buffer *source);
MAGNA_API am_bool          MAGNA_CALL chunked_write          (ChunkedBuffer *chunked, const am_byte *data, am_size_t dataSize);
MAGNA_API am_bool          MAGNA_CALL chunked_write_byte     (ChunkedBuffer *chunked, am_byte value);
MAGNA_API am_bool          MAGNA_CALL chunked_write_text     (ChunkedBuffer *chunked, const char *text);
MAGNA_API am_bool          MAGNA_CALL chunked_write_utf8     (ChunkedBuffer *chunked, unsigned chr);

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

extern MAGNA_API              int            MAGNA_CALL nav_at               (const TextNavigator *nav, am_size_t position);
extern MAGNA_API              int            MAGNA_CALL nav_back             (const TextNavigator *nav);
extern MAGNA_API MAGNA_INLINE const am_byte* MAGNA_CALL nav_current          (const TextNavigator *nav);
extern MAGNA_API MAGNA_INLINE const am_byte* MAGNA_CALL nav_end              (const TextNavigator *nav);
extern MAGNA_API MAGNA_INLINE am_bool        MAGNA_CALL nav_eot              (const TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_extract_integer  (TextNavigator *nav);
extern MAGNA_API MAGNA_INLINE TextNavigator* MAGNA_CALL nav_from_buffer      (TextNavigator *nav, const Buffer *buffer);
extern MAGNA_API MAGNA_INLINE TextNavigator* MAGNA_CALL nav_from_span        (TextNavigator *nav, Span span);
extern MAGNA_API MAGNA_INLINE TextNavigator* MAGNA_CALL nav_from_text        (TextNavigator *nav, const char *text);
extern MAGNA_API MAGNA_INLINE int            MAGNA_CALL nav_front            (const TextNavigator *nav);
extern MAGNA_API              TextNavigator* MAGNA_CALL nav_init             (TextNavigator* nav, const am_byte *data, am_size_t dataSize);
extern MAGNA_API              am_bool        MAGNA_CALL nav_is_control       (const TextNavigator *nav);
extern MAGNA_API              am_bool        MAGNA_CALL nav_is_digit         (const TextNavigator *nav);
extern MAGNA_API              am_bool        MAGNA_CALL nav_is_letter        (const TextNavigator *nav);
extern MAGNA_API              am_bool        MAGNA_CALL nav_is_whitespace    (const TextNavigator *nav);
extern MAGNA_API              int            MAGNA_CALL nav_look_ahead       (const TextNavigator *nav, am_size_t distance);
extern MAGNA_API              int            MAGNA_CALL nav_look_behind      (const TextNavigator *nav, am_size_t distance);
extern MAGNA_API              TextNavigator* MAGNA_CALL nav_move             (TextNavigator *nav, am_ssize_t distance);
extern MAGNA_API MAGNA_INLINE int            MAGNA_CALL nav_peek             (const TextNavigator * nav);
extern MAGNA_API              int            MAGNA_CALL nav_peek_no_crlf     (const TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_peek_string      (const TextNavigator *nav, am_size_t length);
extern MAGNA_API              Span           MAGNA_CALL nav_peek_to          (const TextNavigator *nav, am_byte stopChar);
extern MAGNA_API              Span           MAGNA_CALL nav_peek_until       (const TextNavigator *nav, am_byte stopChar);
extern MAGNA_API              int            MAGNA_CALL nav_read             (TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_read_integer     (TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_read_line        (TextNavigator *nav);
extern MAGNA_API              int            MAGNA_CALL nav_read_no_crlf     (TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_read_string      (TextNavigator *nav, am_size_t length);
extern MAGNA_API              Span           MAGNA_CALL nav_read_to          (TextNavigator *nav, am_byte stopChar);
extern MAGNA_API              Span           MAGNA_CALL nav_read_until       (TextNavigator *nav, am_byte stopChar);
extern MAGNA_API              int            MAGNA_CALL nav_read_utf8        (TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_read_while       (TextNavigator *nav, am_byte goodChar);
extern MAGNA_API              Span           MAGNA_CALL nav_read_word        (TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_recent           (const TextNavigator *nav, am_ssize_t length);
extern MAGNA_API              Span           MAGNA_CALL nav_remaining        (const TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_to_span          (const TextNavigator *nav);
extern MAGNA_API              void           MAGNA_CALL nav_skip_non_word    (TextNavigator *nav);
extern MAGNA_API              void           MAGNA_CALL nav_skip_punctuation (TextNavigator *nav);
extern MAGNA_API              void           MAGNA_CALL nav_skip_whitespace  (TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_slice            (const TextNavigator *nav, am_size_t offset, am_size_t size);

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

extern MAGNA_API              int                    MAGNA_CALL ntc_compare         (const NumberTextChunk *first, const NumberTextChunk *second);
extern MAGNA_API              void                   MAGNA_CALL ntc_free            (NumberTextChunk *chunk);
extern MAGNA_API MAGNA_INLINE am_bool                MAGNA_CALL ntc_have_prefix     (const NumberTextChunk *chunk);
extern MAGNA_API              void                   MAGNA_CALL ntc_init            (NumberTextChunk *chunk);
extern MAGNA_API              am_bool                MAGNA_CALL ntc_setup           (NumberTextChunk *chunk, Span prefix, Span number);
extern MAGNA_API              am_bool                MAGNA_CALL ntc_to_string       (const NumberTextChunk *chunk, Buffer *output);

extern MAGNA_API              am_bool                MAGNA_CALL number_append       (NumberText *number, Span prefix, am_int64 value, int length);
extern MAGNA_API              NumberTextChunk*       MAGNA_CALL number_append_chunk (NumberText *number);
extern MAGNA_API              int                    MAGNA_CALL number_compare      (const NumberText *first, const NumberText *second);
extern MAGNA_API              void                   MAGNA_CALL number_free         (NumberText *number);
extern MAGNA_API              const NumberTextChunk* MAGNA_CALL number_get_chunk    (const NumberText *number, am_size_t index);
extern MAGNA_API              NumberText*            MAGNA_CALL number_increment    (NumberText *number);
extern MAGNA_API              NumberText*            MAGNA_CALL number_increment_ex (NumberText *number, am_size_t index, am_int64 delta);
extern MAGNA_API              NumberText*            MAGNA_CALL number_init         (NumberText *number);
extern MAGNA_API MAGNA_INLINE const NumberTextChunk* MAGNA_CALL number_last         (const NumberText *number);
extern MAGNA_API              am_bool                MAGNA_CALL number_parse        (NumberText *number, Span text);
extern MAGNA_API MAGNA_INLINE am_size_t              MAGNA_CALL number_size         (const NumberText *number);
extern MAGNA_API              am_bool                MAGNA_CALL number_to_string    (const NumberText *number, Buffer *output);

/*=========================================================*/

/* Простая абстракция потока */

typedef am_ssize_t (MAGNA_CALL *ReadFunction)  (Stream*, am_byte*, am_size_t);
typedef am_ssize_t (MAGNA_CALL *WriteFunction) (Stream*, const am_byte*, am_size_t);
typedef am_ssize_t (MAGNA_CALL *SeekFunction)  (Stream*, am_size_t);
typedef am_ssize_t (MAGNA_CALL *TellFunction)  (Stream*);
typedef am_bool    (MAGNA_CALL *CloseFunction) (Stream*);

struct MagnaStream
{
    ReadFunction  readFunction;
    WriteFunction writeFunction;
    SeekFunction  seekFunction;
    TellFunction  tellFunction;
    CloseFunction closeFunction;
    void *data;
};

/* Работа с текстом в потоке */

typedef struct {
    Stream *stream;
    Buffer buffer;
    am_size_t position;
    am_bool eot;
} StreamTexter;


MAGNA_API am_bool    MAGNA_CALL stream_close      (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL stream_copy       (Stream *target, Stream *source);
MAGNA_API am_bool    MAGNA_CALL stream_init       (Stream *stream);
MAGNA_API am_ssize_t MAGNA_CALL stream_read       (Stream *stream, am_byte *buffer, am_size_t length);
MAGNA_API am_ssize_t MAGNA_CALL stream_seek       (Stream *stream, am_size_t position);
MAGNA_API am_ssize_t MAGNA_CALL stream_tell       (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL stream_write      (Stream *stream, const am_byte *buffer, am_size_t length);
MAGNA_API am_bool    MAGNA_CALL stream_write_line (Stream *stream, const char *text);

MAGNA_API am_bool    MAGNA_CALL texter_init      (StreamTexter *texter, Stream *stream, am_size_t bufsize);
MAGNA_API void       MAGNA_CALL texter_free      (StreamTexter *texter);
MAGNA_API int        MAGNA_CALL texter_read_byte (StreamTexter *texter);
MAGNA_API am_ssize_t MAGNA_CALL texter_read_line (StreamTexter *texter, Buffer *output);

MAGNA_API am_bool    MAGNA_CALL null_stream_open       (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL broken_stream_open     (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL memory_stream_create   (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL memory_stream_open     (Stream *stream, am_byte *data, am_size_t length);
MAGNA_API Span       MAGNA_CALL memory_stream_to_span  (const Stream *stream);
MAGNA_API am_byte*   MAGNA_CALL memory_stream_to_text  (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL chunked_stream_create  (Stream *stream, am_size_t chunkSize);
MAGNA_API am_bool    MAGNA_CALL chunked_stream_rewind  (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL file_stream_create     (Stream *stream, const char *filename);
MAGNA_API am_bool    MAGNA_CALL file_stream_open_read  (Stream *stream, const char *filename);
MAGNA_API am_bool    MAGNA_CALL file_stream_open_write (Stream *stream, const char *filename);

MAGNA_API am_bool    MAGNA_CALL tcp4_stream_create     (Stream *stream, const char *hostname, am_uint16 port);

/*=========================================================*/

/* Работа с файлами */

MAGNA_API am_bool    MAGNA_CALL file_close        (am_handle handle);
MAGNA_API am_bool    MAGNA_CALL file_copy         (const char *targetName, const char *sourceName);
MAGNA_API am_handle  MAGNA_CALL file_create       (const char *fileName);
MAGNA_API am_bool    MAGNA_CALL file_delete       (const char *filename);
MAGNA_API am_bool    MAGNA_CALL file_eof          (am_handle handle);
MAGNA_API am_bool    MAGNA_CALL file_exist        (const char *filename);
MAGNA_API am_handle  MAGNA_CALL file_open_read    (const char *fileName);
MAGNA_API am_handle  MAGNA_CALL file_open_write   (const char *fileName);
MAGNA_API am_ssize_t MAGNA_CALL file_read         (am_handle handle, am_byte *buffer, am_ssize_t size);
MAGNA_API am_bool    MAGNA_CALL file_read_all     (const char *fileName, Buffer *buffer);
MAGNA_API int        MAGNA_CALL file_read_byte    (am_handle handle);
MAGNA_API am_uint32  MAGNA_CALL file_read_int_32  (am_handle handle);
MAGNA_API am_uint64  MAGNA_CALL file_read_int_64  (am_handle handle);
MAGNA_API am_bool    MAGNA_CALL file_read_line    (am_handle handle, Buffer *buffer);
MAGNA_API am_bool    MAGNA_CALL file_seek         (am_handle handle, am_int64 offset);
MAGNA_API am_uint64  MAGNA_CALL file_size         (am_handle handle);
MAGNA_API am_bool    MAGNA_CALL file_sync         (am_handle handle);
MAGNA_API am_int64   MAGNA_CALL file_tell         (am_handle handle);
MAGNA_API am_bool    MAGNA_CALL file_write        (am_handle handle, const am_byte *data, am_size_t size);
MAGNA_API am_bool    MAGNA_CALL file_write_buffer (am_handle handle, const Buffer *buffer);
MAGNA_API am_bool    MAGNA_CALL file_write_byte   (am_handle handle, am_byte value);
MAGNA_API am_bool    MAGNA_CALL file_write_int_32 (am_handle handle, am_uint32 value);
MAGNA_API am_bool    MAGNA_CALL file_write_int_64 (am_handle handle, am_uint64 value);
MAGNA_API am_bool    MAGNA_CALL file_write_span   (am_handle handle, const Span span);
MAGNA_API am_bool    MAGNA_CALL file_write_text   (am_handle handle, const char *text);

MAGNA_API am_bool    MAGNA_CALL directory_create  (const char *dirname, am_bool createNew);
MAGNA_API am_bool    MAGNA_CALL directory_delete  (const char *dirname);
MAGNA_API am_bool    MAGNA_CALL directory_exist   (const char *dirname);

/*=========================================================*/

/* Работа с путями */

MAGNA_API am_bool MAGNA_CALL path_get_current_directory   (Buffer *path);
MAGNA_API am_bool MAGNA_CALL path_get_executable          (Buffer *buffer);
MAGNA_API Span    MAGNA_CALL path_get_extension           (const Buffer *path);
MAGNA_API am_bool MAGNA_CALL path_get_temporary_directory (Buffer *path);
MAGNA_API am_bool MAGNA_CALL path_set_current_directory   (const Buffer *path);

/*=========================================================*/

/* Работа со строками */

MAGNA_API am_uint32   MAGNA_CALL fastParse32      (const am_byte *text, am_size_t length);
MAGNA_API am_uint64   MAGNA_CALL fastParse64      (const am_byte *text, am_size_t length);
MAGNA_API am_bool     MAGNA_CALL same_char        (int first, int second);
MAGNA_API am_bool     MAGNA_CALL same_text        (const char *first, const char *second);
MAGNA_API char*       MAGNA_CALL str_dup          (const char *text);
MAGNA_API am_bool     MAGNA_CALL char_one_of      (am_byte one, const am_byte *many);
MAGNA_API am_bool                str_one_of       (const char *one, ...);
MAGNA_API int         MAGNA_CALL str_safe_compare (const char *first, const char *second);
MAGNA_API const char* MAGNA_CALL str_to_visible   (const char *text);

/*=========================================================*/

/* Перечисление элементов */

typedef void    (MAGNA_CALL *EnumerationCleanup) (Enumerator*);
typedef void*   (MAGNA_CALL *EnumerationNext)    (Enumerator*);
typedef am_bool (MAGNA_CALL *EnumerationAction)  (Enumerator*, void*);

struct MangaEnumerator
{
    EnumerationCleanup cleanup;
    EnumerationNext next;
    void *data;
};

MAGNA_API am_bool MAGNA_CALL enum_for_all (Enumerator *enumerator, EnumerationAction action);
MAGNA_API void*   MAGNA_CALL enum_next    (Enumerator *enumerator);

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
