// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef MAGNA_CORE_H
#define MAGNA_CORE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define WIN32_LEAN_AND_MEAN /* Exclude rarely-used stuff from Windows headers */

/*=========================================================*/

#if !defined(MAGNA_WINDOWS) && !defined (MAGNA_UNIX)

    #if defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(_WINDOWS)

        #define MAGNA_WINDOWS

    #elif defined(__MSDOS__) || defined(__DOS__) || defined(_DOS) || defined(__DOS) || defined(MSDOS) || defined(__386__)

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

#define MAGNA_RESTRICT __restrict

#elif defined(__GNUC__) || defined(__clang__)

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
#include <stdarg.h>

/*=========================================================*/

/*
 * Some primitive types.
 */

typedef          int       am_bool;
typedef unsigned char      am_byte;
typedef          short     am_int16;
typedef unsigned short     am_uint16;
typedef          int       am_int32;
typedef unsigned int       am_uint32;
typedef          long long am_int64;
typedef unsigned long long am_uint64;

typedef unsigned short am_wchar;

typedef am_uint32  am_mfn;
typedef am_uint32  am_flag;
typedef am_uint64  am_offset;
typedef void      *am_pointer;

#define AM_FALSE 0
#define AM_TRUE  1

#ifdef _MSC_VER

/* Visual C doesn't have `ssize_t` */

#ifdef MAGNA_64BIT

typedef long long int ssize_t;

#else

typedef int ssize_t;

#endif

#endif

#ifdef 	__WATCOMC__

/* OpenWatcom doesn't have `ssize_t` */

typedef int ssize_t;

#endif

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
extern MAGNA_API MAGNA_INLINE am_handle            handle_get_bad      (void);

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
    size_t    size;

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

MAGNA_API am_int32            magna_get_version        (void);
MAGNA_API const am_byte*      magna_get_version_string (void);
MAGNA_API am_bool  MAGNA_CALL magna_require_version    (am_int32 version);
MAGNA_API am_bool             magna_on_windows         (void);

/*=========================================================*/

/* Опережающее объявление */

typedef struct MagnaAllocatorChunk ArenaChunk;
typedef struct MagnaBuffer         Buffer;
typedef struct MangaEnumerator     Enumerator;
typedef struct MaganaListItem      ListItem;
typedef struct MagnaMemoryChunk    MemoryChunk;
typedef struct MagnaSpan           Span;
typedef struct MagnaSpanArray      SpanArray;
typedef struct MagnaStream         Stream;
typedef struct MagnaArray          Array;
typedef struct MagnaVector         Vector;

/*=========================================================*/

/* Insert signed 64-bit integer literal into the source code. */
#define MAGNA_INT64(__val) (__val##L)

/* Insert unsigned 64-bit integer literal into the source code. */
#define MAGNA_UINT64(__val) (__val##UL)

/*=========================================================*/

/* Работа с ошибками */

MAGNA_API void display_error (void);
MAGNA_API void print_error   (void);

/*=========================================================*/

/* Логирование */

#define LOG_LEVEL_TRACE   1
#define LOG_LEVEL_DEBUG   2
#define LOG_LEVEL_INFO    3
#define LOG_LEVEL_WARNING 4
#define LOG_LEVEL_ERROR   5
#define LOG_LEVEL_FATAL   6

#if 0

#define LOG_TRACE(__s)   log_message (__FILE__, __FUNCTION__, __LINE__, LOG_LEVEL_TRACE,   (__s))
#define LOG_DEBUG(__s)   log_message (__FILE__, __FUNCTION__, __LINE__, LOG_LEVEL_DEBUG,   (__s))
#define LOG_INFO(__s)    log_message (__FILE__, __FUNCTION__, __LINE__, LOG_LEVEL_INFO,    (__s))
#define LOG_WARNING(__s) log_message (__FILE__, __FUNCTION__, __LINE__, LOG_LEVEL_WARNING, (__s))
#define LOG_ERROR(__s)   log_message (__FILE__, __FUNCTION__, __LINE__, LOG_LEVEL_ERROR,   (__s))
#define LOG_FATAL(__s)   log_message (__FILE__, __FUNCTION__, __LINE__, LOG_LEVEL_FATAL,   (__s))

#else

#define LOG_TRACE(__s)   /* Nothing */
#define LOG_DEBUG(__s)   /* Nothing */
#define LOG_INFO(__s)    /* Nothing */
#define LOG_WARNING(__s) /* Nothing */
#define LOG_ERROR(__s)   /* Nothing */
#define LOG_FATAL(__s)   /* Nothing */

#endif

#define LOG_ENTER     LOG_TRACE(__FUNCTION__ ": enter")
#define LOG_LEAVE     LOG_TRACE(__FUNCTION__ ": leave")

MAGNA_API void MAGNA_CALL log_write   (const am_byte *data, size_t size);
MAGNA_API void MAGNA_CALL log_message (const char *file, const char *function, int line, int level, const char *message);

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
MAGNA_API MAGNA_INLINE size_t    MAGNA_CALL min_size_t       (size_t first, size_t second);
MAGNA_API MAGNA_INLINE size_t    MAGNA_CALL max_size_t       (size_t first, size_t second);
MAGNA_API MAGNA_INLINE int       MAGNA_CALL magna_sign_int   (int value);
MAGNA_API MAGNA_INLINE int       MAGNA_CALL magna_sign_int32 (am_int32 value);
MAGNA_API MAGNA_INLINE int       MAGNA_CALL magna_sign_int64 (am_int64 value);

MAGNA_API const am_byte* choose_string (const am_byte *first, ...);
MAGNA_API Span           choose_span   (Span first, ...);
MAGNA_API Buffer*        choose_buffer (const Buffer *first, ...);

extern MAGNA_API MAGNA_INLINE const am_byte* newline (void);

MAGNA_API am_uint32 MAGNA_CALL mersenne_get  (void);
MAGNA_API void      MAGNA_CALL mersenne_init (am_uint32 s);
MAGNA_API am_uint32 MAGNA_CALL random_get    (void);
MAGNA_API void      MAGNA_CALL random_init   (am_uint32 seed);

/*=========================================================*/

/* Ввод-вывод */

extern MAGNA_API MAGNA_INLINE am_uint16 MAGNA_CALL magna_ntohs     (am_uint16 value);
extern MAGNA_API MAGNA_INLINE am_uint32 MAGNA_CALL magna_ntohl     (am_uint32 value);

extern MAGNA_API am_uint64 MAGNA_CALL irbis_decode_64 (am_uint32 junior, am_uint32 senior);
extern MAGNA_API void      MAGNA_CALL irbis_encode_64 (am_uint32 *buffer, am_uint64 value);

extern MAGNA_API              int          MAGNA_CALL fastpack_32     (am_uint32 value, am_byte *bytes);
extern MAGNA_API MAGNA_INLINE unsigned int MAGNA_CALL fastlength_32   (am_byte firstByte);
extern MAGNA_API              am_uint32    MAGNA_CALL fastunpack_32   (const am_byte *bytes);

/*=========================================================*/

/* Работа с памятью */

typedef void* (MAGNA_CALL *AllocationHandler) (size_t size);

MAGNA_API void*             MAGNA_CALL mem_alloc            (size_t size);
MAGNA_API void*             MAGNA_CALL mem_alloc_ex         (size_t size);
MAGNA_API void              MAGNA_CALL mem_clear            (void *ptr, size_t size);
MAGNA_API void              MAGNA_CALL mem_copy             (void *MAGNA_RESTRICT destination, const void *MAGNA_RESTRICT source, size_t size);
MAGNA_API void              MAGNA_CALL mem_free             (void *ptr);
MAGNA_API void*             MAGNA_CALL mem_realloc          (void *ptr, size_t newSize);
MAGNA_API AllocationHandler MAGNA_CALL mem_set_handler      (AllocationHandler newHandler);
MAGNA_API am_bool           MAGNA_CALL mem_can_execute      (const void *ptr);
MAGNA_API am_bool           MAGNA_CALL mem_can_read         (const void *ptr, size_t count);
MAGNA_API am_bool           MAGNA_CALL mem_can_write        (void *ptr, size_t count);
MAGNA_API am_bool                      mem_is_small_machine (void);
MAGNA_API am_bool                      mem_is_huge_machine  (void);
MAGNA_API am_uint64                    mem_avail_physical   (void);
MAGNA_API am_uint64                    mem_avail_virtual    (void);
MAGNA_API am_uint64                    mem_total_installed  (void);
MAGNA_API am_uint64                    mem_total_virtual    (void);

/* Супер-пупер-простой аллокатор */

struct MagnaAllocatorChunk {
    ArenaChunk *next;
};

typedef struct {
    ArenaChunk *first, *last;
    am_byte *current;
    size_t chunkSize, remaining;

} Arena;

MAGNA_API void*     MAGNA_CALL arena_alloc    (Arena *MAGNA_RESTRICT arena, size_t length);
MAGNA_API void      MAGNA_CALL arena_destroy  (Arena *MAGNA_RESTRICT arena);
MAGNA_API am_bool   MAGNA_CALL arena_init     (Arena *MAGNA_RESTRICT arena, size_t chunkSize);
MAGNA_API size_t    MAGNA_CALL arena_total    (const Arena *MAGNA_RESTRICT arena);

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
    am_byte *start;
    am_byte *end;

};

typedef am_bool (MAGNA_CALL *SpanForEach) (am_byte, void*);
typedef am_bool (MAGNA_CALL *SpanForEachPtr) (am_byte*, void*);

#define SPAN_INIT { NULL, NULL }

extern MAGNA_API MAGNA_INLINE void       MAGNA_CALL span_assert                   (Span span);
extern MAGNA_API              int        MAGNA_CALL span_compare                  (Span first, Span second);
extern MAGNA_API              int        MAGNA_CALL span_compare_ignore_case      (Span first, Span second);
extern MAGNA_API              am_bool    MAGNA_CALL span_contains                 (Span span, am_byte value);
extern MAGNA_API              size_t     MAGNA_CALL span_count                    (Span span, am_byte value);
extern MAGNA_API              am_bool    MAGNA_CALL span_ends_with                (Span span, Span suffix);
extern MAGNA_API              Span       MAGNA_CALL span_fill                     (Span span, am_byte value);
extern MAGNA_API              am_byte*   MAGNA_CALL span_find_byte                (Span span, am_byte value);
extern MAGNA_API              am_byte*   MAGNA_CALL span_for_each                 (Span span, SpanForEach routine, void *extraData);
extern MAGNA_API              am_byte*   MAGNA_CALL span_for_each_ptr             (Span span, SpanForEachPtr routine, void *extraData);
extern MAGNA_API              am_byte*   MAGNA_CALL span_for_each_reverse         (Span span, SpanForEach routine, void *extraData);
extern MAGNA_API              am_byte*   MAGNA_CALL span_for_each_ptr_reverse     (Span span, SpanForEachPtr routine, void *extraData);
extern MAGNA_API MAGNA_INLINE Span       MAGNA_CALL span_from_text                (const am_byte *str);
extern MAGNA_API              am_uint64  MAGNA_CALL span_hex_to_uint64            (Span span);
extern MAGNA_API MAGNA_INLINE Span       MAGNA_CALL span_init                     (const am_byte *ptr, size_t length);
extern MAGNA_API              ssize_t    MAGNA_CALL span_index_of                 (Span span, am_byte value);
extern MAGNA_API MAGNA_INLINE am_bool    MAGNA_CALL span_is_empty                 (Span span);
extern MAGNA_API              ssize_t    MAGNA_CALL span_last_index_of            (Span span, am_byte value);
extern MAGNA_API MAGNA_INLINE size_t     MAGNA_CALL span_length                   (Span span);
extern MAGNA_API MAGNA_INLINE Span                  span_null                     (void);
extern MAGNA_API              Span       MAGNA_CALL span_remove_prefix            (Span span, Span prefix);
extern MAGNA_API              Span       MAGNA_CALL span_remove_prefix_and_suffix (Span span, Span prefix, Span suffix);
extern MAGNA_API              Span       MAGNA_CALL span_remove_suffix            (Span span, Span suffix);
extern MAGNA_API MAGNA_INLINE Span       MAGNA_CALL span_slice                    (Span span, ssize_t start, ssize_t length);
extern MAGNA_API              am_bool    MAGNA_CALL span_split_by_char            (Span span, SpanArray *array, am_byte value);
extern MAGNA_API              am_bool    MAGNA_CALL span_split_by_chars           (Span span, SpanArray *array, const am_byte *values, size_t valueCount);
extern MAGNA_API              size_t     MAGNA_CALL span_split_n_by_char          (Span span, Span *array, size_t arraySize, am_byte value);
extern MAGNA_API              size_t     MAGNA_CALL span_split_n_by_chars         (Span span, Span *array, size_t arraySize, const am_byte *values, size_t valueCount);
extern MAGNA_API              am_bool    MAGNA_CALL span_starts_with              (Span span, Span prefix);
extern MAGNA_API              Span       MAGNA_CALL span_trim_start               (Span span);
extern MAGNA_API              Span       MAGNA_CALL span_trim_end                 (Span span);
extern MAGNA_API              Span       MAGNA_CALL span_trim                     (Span span);
extern MAGNA_API              am_byte*   MAGNA_CALL span_to_string                (Span span);
extern MAGNA_API              am_int32   MAGNA_CALL span_to_int32                 (Span span);
extern MAGNA_API              am_int64   MAGNA_CALL span_to_int64                 (Span span);
extern MAGNA_API              am_uint32  MAGNA_CALL span_to_uint32                (Span span);
extern MAGNA_API              am_uint64  MAGNA_CALL span_to_uint64                (Span span);
extern MAGNA_API              am_byte*   MAGNA_CALL span_to_vector                (Span span);
extern MAGNA_API              Span       MAGNA_CALL span_toupper                  (Span span);
extern MAGNA_API              Span       MAGNA_CALL span_tolower                  (Span span);
extern MAGNA_API MAGNA_INLINE am_bool    MAGNA_CALL span_verify                   (Span span);

#define TEXT_SPAN(__s) (span_from_text ((const am_byte*)(__s)))
#define BTEXT(__s) ((am_byte*)(__s))
#define CBTEXT(__s) ((const am_byte*)(__s))
#define CTEXT(__s) ((char*)(__s))
#define CCTEXT(__s) ((const char*)(__s))

/*=========================================================*/

/* Невладеющая цепочка фрагментов */

typedef struct MagnaChainSpan ChainSpan;

struct MagnaChainSpan
{
    am_byte *start;
    am_byte *end;
    ChainSpan *next;
};

/* Итератор по цепочке фрагментов */

typedef struct
{
    ChainSpan *chain;
    ChainSpan *current;
    ChainSpan *end;
    am_byte *ptr;

} ChainIterator;

typedef am_bool (MAGNA_CALL *ChainWalker) (ChainSpan*, void*);

#define CHAIN_INIT { NULL, NULL, NULL }

extern MAGNA_API              am_bool       MAGNA_CALL chain_append               (ChainSpan *MAGNA_RESTRICT chain, Span span);
extern MAGNA_API              void          MAGNA_CALL chain_assert               (const ChainSpan *MAGNA_RESTRICT chain);
extern MAGNA_API              int           MAGNA_CALL chain_back                 (ChainIterator *MAGNA_RESTRICT iterator);
extern MAGNA_API              ChainSpan*    MAGNA_CALL chain_before               (const ChainSpan *chain, const ChainSpan *link);
extern MAGNA_API              am_bool       MAGNA_CALL chain_bot                  (const ChainIterator *iterator);
extern MAGNA_API              am_byte       MAGNA_CALL chain_clone                (ChainSpan *target, const ChainSpan *source);
extern MAGNA_API              int           MAGNA_CALL chain_compare              (const ChainSpan *first, const ChainSpan *second);
extern MAGNA_API              int           MAGNA_CALL chain_compare_ignore_case  (const ChainSpan *first, const ChainSpan *second);
extern MAGNA_API              int           MAGNA_CALL chain_compare_span         (const ChainSpan *chain, Span span);
extern MAGNA_API              int           MAGNA_CALL chain_compare_text         (const ChainSpan *chain, const am_byte *text);
extern MAGNA_API              void          MAGNA_CALL chain_concat               (ChainSpan *target, const ChainSpan *source);
extern MAGNA_API              void          MAGNA_CALL chain_destroy              (ChainSpan *chain);
extern MAGNA_API              am_bool       MAGNA_CALL chain_ends_with            (ChainSpan *chain, ChainSpan *suffix);
extern MAGNA_API              am_bool       MAGNA_CALL chain_eot                  (const ChainIterator *iterator);
extern MAGNA_API              void          MAGNA_CALL chain_fill                 (ChainSpan *chain, am_byte value);
extern MAGNA_API              am_byte*      MAGNA_CALL chain_for_each             (ChainSpan *chain, SpanForEach routine, void *extraData);
extern MAGNA_API              am_byte*      MAGNA_CALL chain_for_each_ptr         (ChainSpan *chain, SpanForEachPtr routine, void *extraData);
extern MAGNA_API              am_byte*      MAGNA_CALL chain_for_each_reverse     (ChainSpan *chain, SpanForEach routine, void *extraData);
extern MAGNA_API              am_byte*      MAGNA_CALL chain_for_each_ptr_reverse (ChainSpan *chain, SpanForEachPtr routine, void *extraData);
extern MAGNA_API              ChainSpan     MAGNA_CALL chain_from_text            (const am_byte *text);
extern MAGNA_API              ChainIterator MAGNA_CALL chain_get_forward_iterator (const ChainSpan *chain);
extern MAGNA_API              ChainIterator MAGNA_CALL chain_get_reverse_iterator (const ChainSpan *chain);
extern MAGNA_API              void          MAGNA_CALL chain_init                 (ChainSpan *chain, Span span);
extern MAGNA_API              am_bool       MAGNA_CALL chain_is_empty             (const ChainSpan *chain);
extern MAGNA_API              ChainSpan*    MAGNA_CALL chain_last                 (const ChainSpan *chain);
extern MAGNA_API MAGNA_INLINE size_t        MAGNA_CALL chain_length               (ChainSpan *link);
extern MAGNA_API MAGNA_INLINE ChainSpan                chain_null                 (void);
extern MAGNA_API              void          MAGNA_CALL chain_optimize             (ChainSpan *chain);
extern MAGNA_API              int           MAGNA_CALL chain_read                 (ChainIterator *MAGNA_RESTRICT iterator);
extern MAGNA_API              int           MAGNA_CALL chain_read_utf8            (ChainIterator *iterator);
extern MAGNA_API              am_bool       MAGNA_CALL chain_starts_with          (ChainSpan *chain, ChainSpan *prefix);
extern MAGNA_API              am_bool       MAGNA_CALL chain_to_buffer            (const ChainSpan *chain, Buffer *output);
extern MAGNA_API              void          MAGNA_CALL chain_to_console           (const ChainSpan *chain);
extern MAGNA_API              am_bool       MAGNA_CALL chain_to_file              (const ChainSpan *chain, am_handle handle);
extern MAGNA_API              am_int32      MAGNA_CALL chain_to_int32             (const ChainSpan *chain);
extern MAGNA_API              am_int64      MAGNA_CALL chain_to_int64             (const ChainSpan *chain);
extern MAGNA_API              am_byte*      MAGNA_CALL chain_to_string            (const ChainSpan *chain);
extern MAGNA_API              am_uint32     MAGNA_CALL chain_to_uint32            (const ChainSpan *chain);
extern MAGNA_API              am_uint64     MAGNA_CALL chain_to_uint64            (const ChainSpan *chain);
extern MAGNA_API              am_byte*      MAGNA_CALL chain_to_vector            (const ChainSpan *chain);
extern MAGNA_API              void          MAGNA_CALL chain_tolower              (ChainSpan *chain);
extern MAGNA_API              size_t        MAGNA_CALL chain_total_length         (const ChainSpan *chain);
extern MAGNA_API              void          MAGNA_CALL chain_toupper              (ChainSpan *chain);
extern MAGNA_API              void          MAGNA_CALL chain_trim                 (ChainSpan *chain);
extern MAGNA_API              void          MAGNA_CALL chain_trim_end             (ChainSpan *chain);
extern MAGNA_API              void          MAGNA_CALL chain_trim_start           (ChainSpan *chain);
extern MAGNA_API              am_bool       MAGNA_CALL chain_verify               (const ChainSpan *chain);
extern MAGNA_API              ChainSpan*    MAGNA_CALL chain_walk                 (ChainSpan *chain, ChainWalker walker, void *extraData);
extern MAGNA_API              ChainSpan*    MAGNA_CALL chain_walk_reverse         (ChainSpan *chain, ChainWalker walker, void *extraData);

/*=========================================================*/

typedef void*   (MAGNA_CALL *Cloner)     (void*);
typedef void    (MAGNA_CALL *Liberator)  (void*);
typedef int     (MAGNA_CALL *Comparer)   (const void*, const void*, const void*);
typedef am_bool (MAGNA_CALL *Outputer)   (const void*, struct MagnaBuffer*);

/*=========================================================*/

/* Массив указателей на объекты */

struct MagnaVector
{
    void **ptr;
    size_t len;
    size_t capacity;
};

#define VECTOR_INIT { NULL, 0, 0 }

extern MAGNA_API              am_bool MAGNA_CALL vector_clone      (Vector *target, const Vector *source);
extern MAGNA_API              am_bool MAGNA_CALL vector_concat     (Vector *target, const Vector *source);
extern MAGNA_API              am_bool MAGNA_CALL vector_copy       (Vector *target, const Vector *source);
extern MAGNA_API              am_bool MAGNA_CALL vector_create     (Vector *vector, size_t capacity);
extern MAGNA_API              void    MAGNA_CALL vector_destroy    (Vector *vector, Liberator liberator);
extern MAGNA_API MAGNA_INLINE void*   MAGNA_CALL vector_get        (const Vector *vector, size_t index);
extern MAGNA_API              am_bool MAGNA_CALL vector_grow       (Vector *vector, size_t newSize);
extern MAGNA_API              void*   MAGNA_CALL vector_pop_back   (Vector *vector);
extern MAGNA_API              void*   MAGNA_CALL vector_pop_front  (Vector *vector);
extern MAGNA_API              am_bool MAGNA_CALL vector_push_back  (Vector *vector, void *item);
extern MAGNA_API              am_bool MAGNA_CALL vector_push_front (Vector *vector, void *item);
extern MAGNA_API              void    MAGNA_CALL vector_set        (Vector *vector, size_t index, void *item, Liberator liberator);
extern MAGNA_API              void    MAGNA_CALL vector_truncate   (Vector *vector, size_t newSize, Liberator liberator);

/*=========================================================*/

/* Динамический массив мелких объектов */

struct MagnaArray
{
    am_byte *ptr;
    size_t itemSize;
    size_t len;
    size_t capacity;
    size_t offset;
};

#define ARRAY_INIT(__itemSize) { NULL, (__itemSize), 0, 0, 0 };

MAGNA_API              void*   MAGNA_CALL array_bsearch        (Array *array, const void *value, Comparer comparer, const void *data);
MAGNA_API              void    MAGNA_CALL array_clear          (Array *array);
MAGNA_API              am_bool MAGNA_CALL array_clone          (Array *target, const Array *source, Cloner cloner);
MAGNA_API              am_bool MAGNA_CALL array_concat         (Array *target, const Array *source);
MAGNA_API              am_bool MAGNA_CALL array_copy           (Array *target, const Array *source);
MAGNA_API              am_bool MAGNA_CALL array_create         (Array *array, size_t itemSize, size_t capacity);
MAGNA_API              void*   MAGNA_CALL array_emplace_at     (Array *array, size_t index);
MAGNA_API              void*   MAGNA_CALL array_emplace_back   (Array *array);
MAGNA_API              void*   MAGNA_CALL array_emplace_before (Array *array, size_t index);
MAGNA_API              void    MAGNA_CALL array_destroy        (Array *array, Liberator liberator);
MAGNA_API MAGNA_INLINE void*   MAGNA_CALL array_get            (const Array *array, size_t index);
MAGNA_API              am_bool MAGNA_CALL array_grow           (Array *array, size_t newSize);
MAGNA_API              void    MAGNA_CALL array_init           (Array *array, size_t itemSize);
MAGNA_API              am_bool MAGNA_CALL array_insert_at      (Array *array, size_t index, void *item);
MAGNA_API              void    MAGNA_CALL array_liberate       (Array *array, Liberator liberator);
MAGNA_API              void*   MAGNA_CALL array_pop_back       (Array *array);
MAGNA_API              void*   MAGNA_CALL array_pop_front      (Array *array);
MAGNA_API              am_bool MAGNA_CALL array_push_back      (Array *array, void *item);
MAGNA_API              am_bool MAGNA_CALL array_push_front     (Array *array, void *item);
MAGNA_API              void    MAGNA_CALL array_remove_index   (Array *array, size_t index);
MAGNA_API              void    MAGNA_CALL array_remove_item    (Array *array, void *item);
MAGNA_API              void    MAGNA_CALL array_set            (Array *array, size_t index, void *value);
MAGNA_API              void    MAGNA_CALL array_sort           (Array *array, Comparer comparer, const void *data);
MAGNA_API              void    MAGNA_CALL array_truncate       (Array *array, size_t newSize);

/*=========================================================*/

/* Динамический ассив 32-битных целых */

typedef struct
{
    am_int32 *ptr;
    size_t len;
    size_t capacity;

} Int32Array;

#define INT32_ARRAY_INIT { NULL, 0, 0 }

MAGNA_API am_bool  MAGNA_CALL int32_array_clone      (Int32Array *target, const Int32Array *source);
MAGNA_API am_bool  MAGNA_CALL int32_array_compress   (const Int32Array *array, struct MagnaBuffer *buffer);
MAGNA_API am_bool  MAGNA_CALL int32_array_concat     (Int32Array *target, const Int32Array *source);
MAGNA_API am_bool  MAGNA_CALL int32_array_copy       (Int32Array *target, const Int32Array *source);
MAGNA_API am_bool  MAGNA_CALL int32_array_create     (Int32Array *array, size_t capacity);
MAGNA_API void     MAGNA_CALL int32_array_destroy    (Int32Array *array);
MAGNA_API am_int32 MAGNA_CALL int32_array_get        (const Int32Array *array, size_t index);
MAGNA_API am_bool  MAGNA_CALL int32_array_grow       (Int32Array *array, size_t newSize);
MAGNA_API am_bool  MAGNA_CALL int32_array_shrink     (Int32Array *array);
MAGNA_API am_int32 MAGNA_CALL int32_array_pop_back   (Int32Array *array);
MAGNA_API am_int32 MAGNA_CALL int32_array_pop_front  (Int32Array *array);
MAGNA_API am_bool  MAGNA_CALL int32_array_push_back  (Int32Array *array, am_int32 value);
MAGNA_API am_bool  MAGNA_CALL int32_array_push_front (Int32Array *array, am_int32 value);
MAGNA_API void     MAGNA_CALL int32_array_set        (Int32Array *array, size_t index, am_int32 value);
MAGNA_API am_bool  MAGNA_CALL int32_array_to_console (const Int32Array *array, const am_byte *delimiter);
MAGNA_API am_bool  MAGNA_CALL int32_array_to_text    (const Int32Array *array, struct MagnaBuffer *buffer, const am_byte *delimiter);
MAGNA_API void     MAGNA_CALL int32_array_truncate   (Int32Array *array, size_t newSize);

/*=========================================================*/

/* Динамический массив фрагментов */

struct MagnaSpanArray
{
    Span *ptr;
    size_t len;
    size_t capacity;

};

#define SPAN_ARRAY_INIT { NULL, 0, 0 }

MAGNA_API am_bool MAGNA_CALL span_array_clone      (SpanArray *target, const SpanArray *source);
MAGNA_API am_bool MAGNA_CALL span_array_copy       (SpanArray *target, const SpanArray *source);
MAGNA_API am_bool MAGNA_CALL span_array_concat     (SpanArray *target, const SpanArray *source);
MAGNA_API am_bool MAGNA_CALL span_array_create     (SpanArray *array, size_t capacity);
MAGNA_API void    MAGNA_CALL span_array_destroy    (SpanArray *array);
MAGNA_API am_bool MAGNA_CALL span_array_from_text  (SpanArray *array, const char **text, size_t count);
MAGNA_API Span    MAGNA_CALL span_array_get        (const SpanArray *array, size_t index);
MAGNA_API am_bool MAGNA_CALL span_array_grow       (SpanArray *array, size_t newSize);
MAGNA_API Span    MAGNA_CALL span_array_pop_back   (SpanArray *array);
MAGNA_API Span    MAGNA_CALL span_array_pop_front  (SpanArray *array);
MAGNA_API am_bool MAGNA_CALL span_array_push_back  (SpanArray *array, Span value);
MAGNA_API am_bool MAGNA_CALL span_array_push_front (SpanArray *array, Span value);
MAGNA_API void    MAGNA_CALL span_array_set        (SpanArray *array, size_t index, Span value);
MAGNA_API am_bool MAGNA_CALL span_array_shrink     (SpanArray *array);
MAGNA_API am_bool MAGNA_CALL span_array_to_text    (const SpanArray *array, struct MagnaBuffer *buffer, const am_byte *delimiter);
MAGNA_API void    MAGNA_CALL span_array_truncate   (SpanArray *array, size_t newSize);

/*=========================================================*/

/* Простой двусвязный список. */

struct MaganaListItem
{
    ListItem *next;
    ListItem *previous;
    am_byte dataStart;
};

typedef struct
{
    ListItem *first;
    ListItem *last;
    size_t itemSize;

} LinkedList;

typedef am_bool (MAGNA_CALL *ListWalker) (LinkedList*, ListItem*, void*);

extern MAGNA_API              void      MAGNA_CALL list_clear          (LinkedList *list, Liberator liberator);
extern MAGNA_API MAGNA_INLINE void*     MAGNA_CALL list_data           (const ListItem *item);
extern MAGNA_API              void*     MAGNA_CALL list_emplace_after  (LinkedList *list, ListItem *item);
extern MAGNA_API              void*     MAGNA_CALL list_emplace_back   (LinkedList *list);
extern MAGNA_API              void*     MAGNA_CALL list_emplace_before (LinkedList *list, ListItem *item);
extern MAGNA_API              void*     MAGNA_CALL list_emplace_front  (LinkedList *list);
extern MAGNA_API              am_bool   MAGNA_CALL list_extract_back   (LinkedList *list, void *place);
extern MAGNA_API              am_bool   MAGNA_CALL list_extract_front  (LinkedList *list, void *place);
extern MAGNA_API              ListItem* MAGNA_CALL list_find_first     (const LinkedList *list, Comparer comparer, void *data, void *extra);
extern MAGNA_API              ListItem* MAGNA_CALL list_find_last      (const LinkedList *list, Comparer comparer, void *data, void *extra);
extern MAGNA_API              void      MAGNA_CALL list_init           (LinkedList *list, size_t itemSize);
extern MAGNA_API              ListItem* MAGNA_CALL list_insert_after   (LinkedList *list, ListItem *item, const void *data);
extern MAGNA_API              ListItem* MAGNA_CALL list_insert_before  (LinkedList *list, ListItem *item, const void *data);
extern MAGNA_API MAGNA_INLINE am_bool   MAGNA_CALL list_is_empty       (const LinkedList *list);
extern MAGNA_API MAGNA_INLINE ListItem* MAGNA_CALL list_item           (const void *data);
extern MAGNA_API              size_t    MAGNA_CALL list_length         (const LinkedList *list);
extern MAGNA_API              ListItem* MAGNA_CALL list_pop_back       (LinkedList *list);
extern MAGNA_API              ListItem* MAGNA_CALL list_pop_front      (LinkedList *list);
extern MAGNA_API              ListItem* MAGNA_CALL list_push_back      (LinkedList *list, const void *data);
extern MAGNA_API              ListItem* MAGNA_CALL list_push_front     (LinkedList *list, const void *data);
extern MAGNA_API              void      MAGNA_CALL list_remove         (LinkedList *list, ListItem *item, Liberator liberator);
extern MAGNA_API              void      MAGNA_CALL list_reverse        (LinkedList *list);
extern MAGNA_API              am_bool   MAGNA_CALL list_sort           (LinkedList *list, Comparer comparer, void *userData);
extern MAGNA_API              am_bool   MAGNA_CALL list_to_string      (const LinkedList *list, Buffer *buffer, Outputer outputer, const char *separator);
extern MAGNA_API              am_bool   MAGNA_CALL list_to_vector      (const LinkedList *list, Vector *vector);
extern MAGNA_API              am_bool   MAGNA_CALL list_walk_backward  (LinkedList *list, ListWalker walker, void *data);
extern MAGNA_API              am_bool   MAGNA_CALL list_walk_forward   (LinkedList *list, ListWalker walker, void *data);

/*=========================================================*/

/* Простой ассоциативный динамический массив */

typedef size_t  (MAGNA_CALL *HashFunction) (void*);
typedef am_bool (MAGNA_CALL *KeyComparer)  (void *firstKey, void *secondKey);
typedef am_bool (MAGNA_CALL *MapWalker)    (void *data, void *key, void *value);

typedef struct {
    void *key;
    void *value;

} MapEntry;

typedef struct {
    size_t len;
    size_t capacity;
    MapEntry *entries;

} MapBacket;

typedef struct {
    Array buckets;
    HashFunction hasher;
    KeyComparer comparer;
    size_t factor;

} Map;

MAGNA_API am_bool MAGNA_CALL map_create  (Map *map, size_t factor, KeyComparer comparer, HashFunction hasher);
MAGNA_API void    MAGNA_CALL map_destroy (Map *map, Liberator keyLiberator, Liberator valueLiberator);
MAGNA_API void*   MAGNA_CALL map_get     (const Map *map, void *key);
MAGNA_API am_bool MAGNA_CALL map_set     (Map *map, void *key, void *value, Liberator liberator);
MAGNA_API am_bool MAGNA_CALL map_unset   (Map *map, void *key);
MAGNA_API am_bool MAGNA_CALL map_walk    (const Map *map, MapWalker walker, void *data);

/*=========================================================*/

/* Буфер - замена строки */

struct MagnaBuffer
{
    am_byte *start;
    am_byte *current;
    am_byte *end;

};

#define BUFFER_INIT { NULL, NULL, NULL }

extern MAGNA_API              am_bool        MAGNA_CALL buffer_ansi_to_utf8              (Buffer *target, const Buffer *source);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_assign                    (Buffer *buffer, const am_byte *data, size_t length);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_assign_span               (Buffer *buffer, Span span);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_assign_text               (Buffer *buffer, const am_byte *text);
extern MAGNA_API              size_t         MAGNA_CALL buffer_calculate_size            (size_t newSize);
extern MAGNA_API MAGNA_INLINE size_t         MAGNA_CALL buffer_capacity                  (const Buffer *buffer);
extern MAGNA_API              Buffer*        MAGNA_CALL buffer_clear                     (Buffer *buffer);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_clone                     (Buffer *target, const Buffer *source);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_combine                   (Buffer *output, const am_byte *prefix, const Buffer *source, const am_byte *suffix);
extern MAGNA_API              int            MAGNA_CALL buffer_compare                   (const Buffer *first, const Buffer *second);
extern MAGNA_API              int            MAGNA_CALL buffer_compare_span              (const Buffer *buffer, Span span);
extern MAGNA_API              int            MAGNA_CALL buffer_compare_span_ignore_case  (const Buffer *buffer, Span span);
extern MAGNA_API              int            MAGNA_CALL buffer_compare_text              (const Buffer *buffer, const am_byte *text);
extern MAGNA_API              int            MAGNA_CALL buffer_compare_text_ignore_case  (const Buffer *buffer, const am_byte *text);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_concat                    (Buffer *target, const Buffer *source);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_copy                      (Buffer *target, const Buffer *source);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_create                    (Buffer *buffer, const am_byte *data, size_t length);
extern MAGNA_API              void           MAGNA_CALL buffer_destroy                   (Buffer *buffer);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_is_empty                  (const Buffer *buffer);
extern MAGNA_API              am_byte*       MAGNA_CALL buffer_find_text                 (const Buffer *buffer, const am_byte *text);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_from_span                 (Buffer *buffer, Span span);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_from_text                 (Buffer *buffer, const am_byte *text);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_grow                      (Buffer *buffer, size_t newSize);
extern MAGNA_API              void           MAGNA_CALL buffer_init                      (Buffer *buffer);
extern MAGNA_API MAGNA_INLINE size_t         MAGNA_CALL buffer_length                    (const Buffer *buffer);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_new_line                  (Buffer *buffer);
extern MAGNA_API MAGNA_INLINE size_t         MAGNA_CALL buffer_position                  (const Buffer *buffer);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_put_uint32                (Buffer *buffer, am_uint32 value);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_put_uint64                (Buffer *buffer, am_uint64 value);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_putc                      (Buffer *buffer, am_byte c);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_puts                      (Buffer *buffer, const am_byte *str);
extern MAGNA_API              void           MAGNA_CALL buffer_remove_at                 (Buffer *buffer, size_t index, size_t size);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_replace_text              (Buffer *buffer, const am_byte *from, const am_byte *to);
extern MAGNA_API MAGNA_INLINE size_t         MAGNA_CALL buffer_size                      (const Buffer *buffer);
extern MAGNA_API              void           MAGNA_CALL buffer_static                    (Buffer *buffer, const am_byte *data, size_t length);
extern MAGNA_API              Buffer*        MAGNA_CALL buffer_swap                      (Buffer *first, Buffer *second);
extern MAGNA_API              void           MAGNA_CALL buffer_to_console                (const Buffer *buffer);
extern MAGNA_API              void           MAGNA_CALL buffer_to_file                   (const Buffer *buffer, am_handle handle);
extern MAGNA_API              const am_byte* MAGNA_CALL buffer_to_text                   (Buffer *buffer);
extern MAGNA_API MAGNA_INLINE Span           MAGNA_CALL buffer_to_span                   (const Buffer *buffer);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_utf8_to_ansi              (Buffer *target, const Buffer *source);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_write                     (Buffer *target, const am_byte *data, size_t length);
extern MAGNA_API              am_bool        MAGNA_CALL buffer_write_span                (Buffer *buffer, Span span);

#define B2T(__s) ((char*)buffer_to_text (__s))
#define B2B(__s) ((am_byte*)buffer_to_text (__s))

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
    size_t chunkSize, position, read;

} ChunkedBuffer;

MAGNA_API ssize_t          MAGNA_CALL chunked_all            (const ChunkedBuffer *chunked, Buffer *buffer);
MAGNA_API am_bool          MAGNA_CALL chunked_ansi_to_utf8   (ChunkedBuffer *target, const Buffer *source);
MAGNA_API am_byte*         MAGNA_CALL chunked_at             (const ChunkedBuffer *chunked, size_t offset);
MAGNA_API size_t           MAGNA_CALL chunked_capacity       (const ChunkedBuffer *chunked);
MAGNA_API am_bool          MAGNA_CALL chunked_empty          (const ChunkedBuffer *chunked);
MAGNA_API am_bool          MAGNA_CALL chunked_eof            (const ChunkedBuffer *chunked);
MAGNA_API void             MAGNA_CALL chunked_free           (ChunkedBuffer *chunked);
MAGNA_API am_bool          MAGNA_CALL chunked_grow           (ChunkedBuffer *chunked, size_t newSize);
MAGNA_API ChunkedBuffer*   MAGNA_CALL chunked_init           (ChunkedBuffer *chunked, size_t chunkSize);
MAGNA_API am_bool          MAGNA_CALL chunked_new_line       (ChunkedBuffer *chunked);
MAGNA_API int              MAGNA_CALL chunked_peek           (ChunkedBuffer *chunked);
MAGNA_API size_t           MAGNA_CALL chunked_position       (const ChunkedBuffer *chunked);
MAGNA_API int              MAGNA_CALL chunked_read_byte      (ChunkedBuffer *chunked);
MAGNA_API ssize_t          MAGNA_CALL chunked_read           (ChunkedBuffer *chunked, Buffer *buffer, size_t count);
MAGNA_API ssize_t          MAGNA_CALL chunked_read_line      (ChunkedBuffer *chunked, Buffer *buffer);
MAGNA_API ssize_t          MAGNA_CALL chunked_read_raw       (ChunkedBuffer *chunked, am_byte *buffer, size_t count);
MAGNA_API ssize_t          MAGNA_CALL chunked_read_remaining (const ChunkedBuffer *chunked, Buffer *buffer);
MAGNA_API int              MAGNA_CALL chunked_read_utf8      (ChunkedBuffer *chunked);
MAGNA_API size_t           MAGNA_CALL chunked_remaining_size (const ChunkedBuffer *chunked);
MAGNA_API ChunkedBuffer*   MAGNA_CALL chunked_rewind         (ChunkedBuffer *chunked);
MAGNA_API size_t           MAGNA_CALL chunked_size           (const ChunkedBuffer *chunked);
MAGNA_API am_bool          MAGNA_CALL chunked_utf8_to_ansi   (ChunkedBuffer *target, const Buffer *source);
MAGNA_API am_bool          MAGNA_CALL chunked_write          (ChunkedBuffer *chunked, const am_byte *data, size_t dataSize);
MAGNA_API am_bool          MAGNA_CALL chunked_write_byte     (ChunkedBuffer *chunked, am_byte value);
MAGNA_API am_bool          MAGNA_CALL chunked_write_text     (ChunkedBuffer *chunked, const char *text);
MAGNA_API am_bool          MAGNA_CALL chunked_write_utf8     (ChunkedBuffer *chunked, unsigned chr);

/*=========================================================*/

/* Буфер, хранящий часть данных на стеке */

typedef struct
{
    am_byte *static_data;
    am_byte *dynamic_data;
    size_t capacity;
    size_t size;
    size_t static_capacity;

} FastBuffer;

MAGNA_API FastBuffer*   MAGNA_CALL fastbuf_clear     (FastBuffer *buffer);
MAGNA_API am_bool       MAGNA_CALL fastbuf_empty     (const FastBuffer *buffer);
MAGNA_API void          MAGNA_CALL fastbuf_free      (FastBuffer *buffer);
MAGNA_API am_int32      MAGNA_CALL fastbuf_getc      (FastBuffer *buffer);
MAGNA_API am_int32      MAGNA_CALL fastbuf_getc_utf8 (FastBuffer *buffer);
MAGNA_API size_t        MAGNA_CALL fastbuf_gets      (FastBuffer *buffer, Buffer *text);
MAGNA_API am_bool       MAGNA_CALL fastbuf_grow      (FastBuffer *buffer, size_t newSize);
MAGNA_API void          MAGNA_CALL fastbuf_init      (FastBuffer *buffer, size_t static_capacity);
MAGNA_API am_bool       MAGNA_CALL fastbuf_puts      (FastBuffer *buffer, const char *text);
MAGNA_API am_bool       MAGNA_CALL fastbuf_putc      (FastBuffer *buffer, char c        );
MAGNA_API am_bool       MAGNA_CALL fastbuf_put_utf8  (FastBuffer *buffer, int c);
MAGNA_API size_t        MAGNA_CALL fastbuf_read      (FastBuffer *buffer, am_byte *data, size_t size);
MAGNA_API void          MAGNA_CALL fastbuf_rewind    (FastBuffer *buffer);
MAGNA_API am_bool       MAGNA_CALL fastbuf_write     (FastBuffer *buffer, const am_byte *data, size_t dataSize);

/*=========================================================*/

/* Навигация по тексту */

typedef struct
{
    const am_byte *data;
    size_t column, length, line, position;

} TextNavigator;

/**
 * Признак достигнутого конца данных.
 */
#define NAV_EOT (-1)

extern MAGNA_API              int            MAGNA_CALL nav_at               (const TextNavigator *nav, size_t position);
extern MAGNA_API              int            MAGNA_CALL nav_back             (const TextNavigator *nav);
extern MAGNA_API MAGNA_INLINE const am_byte* MAGNA_CALL nav_current          (const TextNavigator *nav);
extern MAGNA_API MAGNA_INLINE const am_byte* MAGNA_CALL nav_end              (const TextNavigator *nav);
extern MAGNA_API MAGNA_INLINE am_bool        MAGNA_CALL nav_eot              (const TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_extract_integer  (TextNavigator *nav);
extern MAGNA_API MAGNA_INLINE TextNavigator* MAGNA_CALL nav_from_buffer      (TextNavigator *nav, const Buffer *buffer);
extern MAGNA_API MAGNA_INLINE TextNavigator* MAGNA_CALL nav_from_span        (TextNavigator *nav, Span span);
extern MAGNA_API MAGNA_INLINE TextNavigator* MAGNA_CALL nav_from_text        (TextNavigator *nav, const am_byte *text);
extern MAGNA_API MAGNA_INLINE int            MAGNA_CALL nav_front            (const TextNavigator *nav);
extern MAGNA_API              TextNavigator* MAGNA_CALL nav_init             (TextNavigator* nav, const am_byte *data, size_t dataSize);
extern MAGNA_API              am_bool        MAGNA_CALL nav_is_control       (const TextNavigator *nav);
extern MAGNA_API              am_bool        MAGNA_CALL nav_is_digit         (const TextNavigator *nav);
extern MAGNA_API              am_bool        MAGNA_CALL nav_is_letter        (const TextNavigator *nav);
extern MAGNA_API              am_bool        MAGNA_CALL nav_is_whitespace    (const TextNavigator *nav);
extern MAGNA_API              int            MAGNA_CALL nav_look_ahead       (const TextNavigator *nav, size_t distance);
extern MAGNA_API              int            MAGNA_CALL nav_look_behind      (const TextNavigator *nav, size_t distance);
extern MAGNA_API              TextNavigator* MAGNA_CALL nav_move             (TextNavigator *nav, ssize_t distance);
extern MAGNA_API MAGNA_INLINE int            MAGNA_CALL nav_peek             (const TextNavigator * nav);
extern MAGNA_API              int            MAGNA_CALL nav_peek_no_crlf     (const TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_peek_string      (const TextNavigator *nav, size_t length);
extern MAGNA_API              Span           MAGNA_CALL nav_peek_to          (const TextNavigator *nav, am_byte stopChar);
extern MAGNA_API              Span           MAGNA_CALL nav_peek_until       (const TextNavigator *nav, am_byte stopChar);
extern MAGNA_API              int            MAGNA_CALL nav_read             (TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_read_integer     (TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_read_irbis       (TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_read_line        (TextNavigator *nav);
extern MAGNA_API              int            MAGNA_CALL nav_read_no_crlf     (TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_read_string      (TextNavigator *nav, size_t length);
extern MAGNA_API              Span           MAGNA_CALL nav_read_to          (TextNavigator *nav, am_byte stopChar);
extern MAGNA_API              Span           MAGNA_CALL nav_read_until       (TextNavigator *nav, am_byte stopChar);
extern MAGNA_API              int            MAGNA_CALL nav_read_utf8        (TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_read_while       (TextNavigator *nav, am_byte goodChar);
extern MAGNA_API              Span           MAGNA_CALL nav_read_word        (TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_recent           (const TextNavigator *nav, ssize_t length);
extern MAGNA_API              Span           MAGNA_CALL nav_remaining        (const TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_to_span          (const TextNavigator *nav);
extern MAGNA_API              void           MAGNA_CALL nav_skip_non_word    (TextNavigator *nav);
extern MAGNA_API              void           MAGNA_CALL nav_skip_punctuation (TextNavigator *nav);
extern MAGNA_API              void           MAGNA_CALL nav_skip_whitespace  (TextNavigator *nav);
extern MAGNA_API              Span           MAGNA_CALL nav_slice            (const TextNavigator *nav, size_t offset, size_t size);

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
extern MAGNA_API              void                   MAGNA_CALL ntc_destroy         (NumberTextChunk *chunk);
extern MAGNA_API MAGNA_INLINE am_bool                MAGNA_CALL ntc_have_prefix     (const NumberTextChunk *chunk);
extern MAGNA_API              void                   MAGNA_CALL ntc_init            (NumberTextChunk *chunk);
extern MAGNA_API              am_bool                MAGNA_CALL ntc_setup           (NumberTextChunk *chunk, Span prefix, Span number);
extern MAGNA_API              am_bool                MAGNA_CALL ntc_to_string       (const NumberTextChunk *chunk, Buffer *output);

extern MAGNA_API              am_bool                MAGNA_CALL number_append       (NumberText *number, Span prefix, am_int64 value, int length);
extern MAGNA_API              NumberTextChunk*       MAGNA_CALL number_append_chunk (NumberText *number);
extern MAGNA_API              int                    MAGNA_CALL number_compare      (const NumberText *first, const NumberText *second);
extern MAGNA_API              void                   MAGNA_CALL number_destroy      (NumberText *number);
extern MAGNA_API              const NumberTextChunk* MAGNA_CALL number_get_chunk    (const NumberText *number, size_t index);
extern MAGNA_API              NumberText*            MAGNA_CALL number_increment    (NumberText *number);
extern MAGNA_API              NumberText*            MAGNA_CALL number_increment_ex (NumberText *number, size_t index, am_int64 delta);
extern MAGNA_API              void                   MAGNA_CALL number_init         (NumberText *number);
extern MAGNA_API MAGNA_INLINE const NumberTextChunk* MAGNA_CALL number_last         (const NumberText *number);
extern MAGNA_API              am_bool                MAGNA_CALL number_parse        (NumberText *number, Span text);
extern MAGNA_API MAGNA_INLINE size_t                 MAGNA_CALL number_size         (const NumberText *number);
extern MAGNA_API              am_bool                MAGNA_CALL number_to_string    (const NumberText *number, Buffer *output);

/*=========================================================*/

/* Простая абстракция потока */

typedef ssize_t (MAGNA_CALL *ReadFunction)  (Stream*, am_byte*, size_t);
typedef ssize_t (MAGNA_CALL *WriteFunction) (Stream*, const am_byte*, size_t);
typedef ssize_t (MAGNA_CALL *SeekFunction)  (Stream*, size_t);
typedef ssize_t (MAGNA_CALL *TellFunction)  (Stream*);
typedef am_bool (MAGNA_CALL *CloseFunction) (Stream*);

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
    size_t position;
    am_bool eot;

} StreamTexter;

MAGNA_API am_bool    MAGNA_CALL stream_close        (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL stream_copy         (Stream *target, Stream *source);
MAGNA_API am_bool    MAGNA_CALL stream_init         (Stream *stream);
MAGNA_API ssize_t    MAGNA_CALL stream_read         (Stream *stream, am_byte *buffer, size_t length);
MAGNA_API ssize_t    MAGNA_CALL stream_seek         (Stream *stream, size_t position);
MAGNA_API ssize_t    MAGNA_CALL stream_tell         (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL stream_write        (Stream *stream, const am_byte *buffer, size_t length);
MAGNA_API am_bool    MAGNA_CALL stream_write_buffer (Stream *stream, const Buffer *buffer);
MAGNA_API am_bool    MAGNA_CALL stream_write_char   (Stream *stream, am_byte value);
MAGNA_API am_bool    MAGNA_CALL stream_write_text   (Stream *stream, const char *text);

MAGNA_API am_bool    MAGNA_CALL texter_init         (StreamTexter *texter, Stream *stream, size_t bufsize);
MAGNA_API void       MAGNA_CALL texter_destroy      (StreamTexter *texter);
MAGNA_API int        MAGNA_CALL texter_read_byte    (StreamTexter *texter);
MAGNA_API ssize_t    MAGNA_CALL texter_read_line    (StreamTexter *texter, Buffer *output);

MAGNA_API am_bool    MAGNA_CALL null_stream_open       (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL broken_stream_open     (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL memory_stream_create   (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL memory_stream_open     (Stream *stream, am_byte *data, size_t length);
MAGNA_API Span       MAGNA_CALL memory_stream_to_span  (const Stream *stream);
MAGNA_API am_byte*   MAGNA_CALL memory_stream_to_text  (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL chunked_stream_create  (Stream *stream, size_t chunkSize);
MAGNA_API am_bool    MAGNA_CALL chunked_stream_rewind  (Stream *stream);
MAGNA_API am_bool    MAGNA_CALL file_stream_create     (Stream *stream, const char *filename);
MAGNA_API am_bool    MAGNA_CALL file_stream_open_read  (Stream *stream, const char *filename);
MAGNA_API am_bool    MAGNA_CALL file_stream_open_write (Stream *stream, const char *filename);

MAGNA_API am_bool    MAGNA_CALL tcp4_stream_create     (Stream *stream, const char *hostname, am_uint16 port);

/*=========================================================*/

/* Работа с файлами */

MAGNA_API am_bool    MAGNA_CALL file_close             (am_handle handle);
MAGNA_API am_bool    MAGNA_CALL file_copy              (const char *targetName, const char *sourceName);
MAGNA_API am_handle  MAGNA_CALL file_create            (const char *fileName);
MAGNA_API am_handle  MAGNA_CALL file_create_insist     (const char *fileName, unsigned int delay, unsigned int retryLimit);
MAGNA_API am_bool    MAGNA_CALL file_delete            (const char *filename);
MAGNA_API am_bool    MAGNA_CALL file_eof               (am_handle handle);
MAGNA_API am_bool    MAGNA_CALL file_exist             (const char *filename);
MAGNA_API am_handle  MAGNA_CALL file_open_read         (const char *fileName);
MAGNA_API am_handle  MAGNA_CALL file_open_read_insist  (const char *fileName, unsigned int delay, unsigned int retryLimit);
MAGNA_API am_handle  MAGNA_CALL file_open_write        (const char *fileName);
MAGNA_API am_handle  MAGNA_CALL file_open_write_insist (const char *fileName, unsigned int delay, unsigned int retryLimit);
MAGNA_API ssize_t    MAGNA_CALL file_read              (am_handle handle, am_byte *buffer, ssize_t size);
MAGNA_API am_bool    MAGNA_CALL file_read_all          (const char *fileName, Buffer *buffer);
MAGNA_API int        MAGNA_CALL file_read_byte         (am_handle handle);
MAGNA_API am_uint32  MAGNA_CALL file_read_int32        (am_handle handle);
MAGNA_API am_uint64  MAGNA_CALL file_read_int64        (am_handle handle);
MAGNA_API am_bool    MAGNA_CALL file_read_line         (am_handle handle, Buffer *buffer);
MAGNA_API am_bool    MAGNA_CALL file_seek              (am_handle handle, am_int64 offset);
MAGNA_API am_uint64  MAGNA_CALL file_size              (am_handle handle);
MAGNA_API am_bool    MAGNA_CALL file_sync              (am_handle handle);
MAGNA_API am_int64   MAGNA_CALL file_tell              (am_handle handle);
MAGNA_API am_bool    MAGNA_CALL file_texter            (StreamTexter *texter, Stream *stream, const char *filename);
MAGNA_API am_bool    MAGNA_CALL file_touch             (const char *filename);
MAGNA_API am_bool    MAGNA_CALL file_write             (am_handle handle, const am_byte *data, size_t size);
MAGNA_API am_bool    MAGNA_CALL file_write_buffer      (am_handle handle, const Buffer *buffer);
MAGNA_API am_bool    MAGNA_CALL file_write_byte        (am_handle handle, am_byte value);
MAGNA_API am_bool    MAGNA_CALL file_write_int32       (am_handle handle, am_uint32 value);
MAGNA_API am_bool    MAGNA_CALL file_write_int64       (am_handle handle, am_uint64 value);
MAGNA_API am_bool    MAGNA_CALL file_write_span        (am_handle handle, Span span);
MAGNA_API am_bool    MAGNA_CALL file_write_text        (am_handle handle, const am_byte *text);

extern MAGNA_API MAGNA_INLINE am_handle file_stdin  (void);
extern MAGNA_API MAGNA_INLINE am_handle file_stdout (void);
extern MAGNA_API MAGNA_INLINE am_handle file_stderr (void);

MAGNA_API am_bool    MAGNA_CALL directory_create       (const char *dirname, am_bool createNew);
MAGNA_API am_bool    MAGNA_CALL directory_delete       (const char *dirname);
MAGNA_API am_bool    MAGNA_CALL directory_exist        (const char *dirname);

/*=========================================================*/

/* Работа с путями */

MAGNA_API am_bool MAGNA_CALL path_add_trailing_slash      (Buffer *path);
MAGNA_API am_bool MAGNA_CALL path_append                  (Buffer *path, Span element);
MAGNA_API am_bool            path_combine                 (Buffer *output, ...);
MAGNA_API void    MAGNA_CALL path_convert_slashes         (Buffer *path);
MAGNA_API am_bool MAGNA_CALL path_get_current_directory   (Buffer *path);
MAGNA_API Span    MAGNA_CALL path_get_directory           (const Buffer *path);
MAGNA_API am_bool MAGNA_CALL path_get_executable          (Buffer *buffer);
MAGNA_API Span    MAGNA_CALL path_get_extension           (const Buffer *path);
MAGNA_API Span    MAGNA_CALL path_get_extension           (const Buffer *path);
MAGNA_API Span    MAGNA_CALL path_get_filename            (const Buffer *path);
MAGNA_API char               path_get_slash               (void);
MAGNA_API am_bool MAGNA_CALL path_get_temporary_directory (Buffer *path);
MAGNA_API am_bool MAGNA_CALL path_set_current_directory   (const Buffer *path);
MAGNA_API void    MAGNA_CALL path_trim_trailing_slashes   (Buffer *path);

/*=========================================================*/

/* Работа со строками */

MAGNA_API am_uint32   MAGNA_CALL fastParse32          (const am_byte *text, size_t length);
MAGNA_API am_uint64   MAGNA_CALL fastParse64          (const am_byte *text, size_t length);
MAGNA_API am_bool     MAGNA_CALL same_char            (int first, int second);
MAGNA_API am_bool     MAGNA_CALL same_text            (const char *first, const char *second);
MAGNA_API char*       MAGNA_CALL str_dup              (const char *text);
MAGNA_API am_bool     MAGNA_CALL char_one_of          (am_byte one, const am_byte *many);
MAGNA_API am_bool                str_one_of           (const char *one, ...);
MAGNA_API int         MAGNA_CALL str_safe_compare     (const char *first, const char *second);
MAGNA_API const char* MAGNA_CALL str_to_visible       (const char *text);
MAGNA_API am_bool     MAGNA_CALL irbis_to_client      (Buffer *output, Span input);

/*=========================================================*/

/* Перечисление элементов */

typedef void    (MAGNA_CALL *EnumerationCleanup) (Enumerator*);
typedef void*   (MAGNA_CALL *EnumerationNext)    (Enumerator*);
typedef am_bool (MAGNA_CALL *EnumerationAction)  (Enumerator*, void*);

struct MangaEnumerator
{
    EnumerationCleanup cleanup;
    EnumerationNext next;
    am_mix data1;
    am_mix data2;
};

MAGNA_API am_bool MAGNA_CALL enum_for_all (Enumerator *enumerator, EnumerationAction action);
MAGNA_API void*   MAGNA_CALL enum_next    (Enumerator *enumerator);

/*=========================================================*/

/* Работа с кодировками */

/**
 * Кодировка символов.
 */
typedef struct
{
    const char *name;
    unsigned (MAGNA_CALL *char_to_unicode) (am_byte);
    am_byte (MAGNA_CALL *unicode_to_char) (unsigned);

} Encoding;

/**
 * Хелпер для чтения UTF-8.
 */
typedef struct
{
    const am_byte *position;
    unsigned int value;

} UtfHelper;

extern Encoding cp1251_encoding, cp866_encoding, koi8r_encoding;

MAGNA_API am_bool   MAGNA_CALL encoding_register  (const Encoding *encoding);
MAGNA_API Encoding* MAGNA_CALL encoding_get       (const char *name);
MAGNA_API Encoding*            encoding_ansi      (void);

MAGNA_API am_bool   MAGNA_CALL buffer_putc_utf8   (Buffer *buffer, unsigned int chr);
MAGNA_API size_t    MAGNA_CALL utf8_code_points   (const am_byte *data, size_t dataLength);
MAGNA_API ssize_t   MAGNA_CALL search_for_unicode (const am_wchar *array, ssize_t left, ssize_t right, am_wchar value);
MAGNA_API UtfHelper MAGNA_CALL utf8_read          (const am_byte *text);
MAGNA_API am_bool   MAGNA_CALL ansi2utf           (Buffer *output, Span input);
MAGNA_API am_bool   MAGNA_CALL utf2ansi           (Buffer *output, Span input);

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

/* Форматированный вывод текста */

typedef am_bool (MAGNA_CALL *OutputFunction) (void *output, void *data, size_t size);

MAGNA_API am_bool MAGNA_CALL format_generic (void *output, OutputFunction handler, Span format, va_list args);

/*=========================================================*/

/* Работа с сетью */

MAGNA_API am_int32   MAGNA_CALL tcp4_connect            (const am_byte *hostname, am_uint16 port);
MAGNA_API am_bool    MAGNA_CALL tcp4_disconnect         (am_int32 handle);
MAGNA_API am_bool               tcp4_initialize         (void);
MAGNA_API ssize_t    MAGNA_CALL tcp4_receive_all        (am_int32 handle, Buffer *buffer);
MAGNA_API ssize_t    MAGNA_CALL tcp4_receive_with_limit (am_int32 handle, Buffer *buffer, ssize_t limit);
MAGNA_API ssize_t    MAGNA_CALL tcp4_send               (am_int32 handle, const am_byte *data, ssize_t dataLength);
MAGNA_API am_bool    MAGNA_CALL tcp4_send_buffer        (am_int32 handle, const Buffer *buffer);

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

typedef void*   (MAGNA_CALL *Action)  (void *data);
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
