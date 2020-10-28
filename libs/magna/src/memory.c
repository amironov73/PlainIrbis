// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

/**
 * \file memory.c
 *
 * Работа с памятью.
 *
 */

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

#ifdef MAGNA_LINUX

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#endif

#ifdef MAGNA_APPLE

#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/vmmeter.h>

#endif

#include <assert.h>
#include <ctype.h>

/*=========================================================*/

static AllocationHandler allocationHandler;

/**
 * Запрос блока памяти.
 *
 * @param size Объем блока в байтах.
 * @return Указатель на блок либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL mem_alloc
    (
        size_t size
    )
{
    void *result = calloc (1, size);

    if ((result == NULL) && (allocationHandler != NULL)) {
        result = allocationHandler (size);
    }

    LOG_TRACE ("failed to allocate memory");

    return result;
}

/**
 * Запрос изменения размера блока памяти.
 *
 * @param ptr Указатель на ранее выделенный блок либо `NULL`.
 * @param newSize Запрашиваемый размер блока либо 0 (освобождение).
 * @return Указатель на блок либо `NULL` в случае неудачи.
 */
MAGNA_API void* MAGNA_CALL mem_realloc
    (
        void *ptr,
        size_t newSize
    )
{
    void *result;

    if (newSize == 0) {
        mem_free (ptr);

        return ptr;
    }

    if (ptr == NULL) {
        return mem_alloc (newSize);
    }

    result = realloc (ptr, newSize);
    if ((result == NULL) && (allocationHandler != NULL)) {
        result = allocationHandler (newSize);
    }

    LOG_TRACE ("failed to reallocate memory");

    return result;
}

/**
 * Установка обработчика нехватки памяти.
 *
 * @param newHandler Новый обработчик.
 * @return Предыдущий обработчик.
 */
MAGNA_API AllocationHandler MAGNA_CALL mem_set_handler
    (
        AllocationHandler newHandler
    )
{
    AllocationHandler oldHandler = allocationHandler;

    allocationHandler = newHandler;

    return oldHandler;
}

/**
 * Запрос блока памяти. Если запрос не может быть удовлетворен,
 * программа аварийно завершается.
 *
 * @param size Объем блока в байтах.
 * @return Указатель на блок.
 */
MAGNA_API void* MAGNA_CALL mem_alloc_ex
    (
        size_t size
    )
{
    void* result = calloc (1, size);

    if (!result) {
        exit (1);
    }

    return result;
}

/**
 * Освобождение ранее запрошенного блока.
 *
 * @param ptr Указатель на блок памяти.
 */
MAGNA_API void MAGNA_CALL mem_free
    (
        void *ptr
    )
{
    free (ptr);
}

/**
 * Заполнение памяти нулями.
 *
 * @param ptr Указатель на начало блока.
 * @param size Размер блока в байтах.
 */
MAGNA_API void MAGNA_CALL mem_clear
    (
        void *ptr,
        size_t size
    )
{
    assert (ptr != NULL);

    memset (ptr, 0, size);
}

/**
 * Копирование одного блока памяти в другой.
 *
 * @param destination Блок назначения.
 * @param source Исходный блок.
 * @param size Размер блока в байтах.
 */
MAGNA_API void MAGNA_CALL mem_copy
    (
        void *MAGNA_RESTRICT destination,
        const void *MAGNA_RESTRICT source,
        size_t size
    )
{
    assert (destination != NULL);
    assert (source != NULL);

    memcpy (destination, source, size);
}

#ifdef MAGNA_LINUX

/* Парсим /proc/meminfo в поисках строки */
static size_t parse_value
    (
        const char *prefix
    )
{
    Stream stream;
    StreamTexter texter;
    Span parts[2], sought = span_from_text (prefix), found, unit = span_null();
    size_t result = 0, nparts, multiplier;
    Buffer line = BUFFER_INIT;

    if (!file_stream_open_read (&stream, "/proc/meminfo")) {
        return 0;
    }

    if (!texter_init (&texter, &stream, 256)) {
        stream_close (&stream);
        return 0;
    }

    while (AM_TRUE) {
        buffer_clear (&line);
        if (texter_read_line (&texter, &line) <= 0) {
            break;
        }
        nparts = span_split_n_by_char (buffer_to_span (&line), parts, 2, ':');
        if (nparts != 2) {
            continue;
        }
        if (span_compare (parts[0], sought) == 0) {
            found = parts[1];
            found = span_trim_start (found);
            nparts = span_split_n_by_char (found, parts, 2, ' ');
            if (nparts != 2) {
                break;
            }

            found = parts[0];
            unit = parts[1];
            result = (size_t) span_to_uint64(found);
            break;
        }
    }

    if (span_compare (unit, span_from_text ("kB")) == 0) {
        multiplier = MAGNA_INT64 (1024);
    }
    else if (span_compare (unit, span_from_text ("MB")) == 0) {
        multiplier = MAGNA_INT64 (1024) * MAGNA_INT64 (1024);
    }
    else if (span_compare (unit, span_from_text ("GB")) == 0) {
        multiplier = MAGNA_INT64 (1024) * MAGNA_INT64 (1024)
            * MAGNA_INT64 (1024);
    }
    else {
        multiplier = 0;
    }

    result *= multiplier;

    buffer_destroy(&line);
    texter_destroy (&texter);

    return result;
}

#endif

/**
 * Общее количество инсталлированой физической памяти в компьютере.
 *
 * @return Объем физической памяти в байтах.
 */
MAGNA_API am_uint64 mem_total_installed (void)
{
#ifdef MAGNA_WINDOWS

    MEMORYSTATUSEX status;

    mem_clear (&status, sizeof (status));
    status.dwLength = sizeof (status);

    if (!GlobalMemoryStatusEx (&status)) {
        return 0;
    }

    return status.ullTotalPhys;

#elif defined(MAGNA_LINUX)

    return parse_value ("MemTotal");

#elif defined(MAGNA_APPLE)

    int mib[2];
    int64_t physical_memory;
    size_t length;

    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    length = sizeof (int64_t);
    sysctl (mib, 2, &physical_memory, &length, NULL, 0);

    return physical_memory;

#else

    return 0;

#endif
}

/**
 * Общее количество виртуальной памяти в компьютере.
 *
 * @return Объем виртуальной памяти в байтах.
 */
MAGNA_API am_uint64 mem_total_virtual (void)
{
#ifdef MAGNA_WINDOWS

    MEMORYSTATUSEX status;

    mem_clear (&status, sizeof (status));
    status.dwLength = sizeof (status);

    if (!GlobalMemoryStatusEx (&status)) {
        return 0;
    }

    return status.ullTotalPageFile;

#elif defined(MAGNA_LINUX)

    return parse_value ("MemTotal") + parse_value ("SwapTotal");

#elif defined(MAGNA_APPLE)

    int mib[2];
    struct xsw_usage swapinfo;
    size_t length;

    mib[0] = CTL_VM;
    mib[1] = VM_SWAPUSAGE;
    length = sizeof (swapinfo);
    sysctl (mib, 2, &swapinfo, &length, NULL, 0);

    return mem_total_installed() + swapinfo.xsu_total;

#else

    return 0;

#endif
}

/**
 * Объем свободной физической памяти.
 *
 * @return Объем свободной физической памяти в байтах.
 */
MAGNA_API am_uint64 mem_avail_physical (void)
{
#ifdef MAGNA_WINDOWS

    MEMORYSTATUSEX status;

    mem_clear (&status, sizeof (status));
    status.dwLength = sizeof (status);

    if (!GlobalMemoryStatusEx (&status)) {
        return 0;
    }

    return status.ullAvailPhys;

#elif defined(MAGNA_LINUX)

    am_uint64 result;

    result = parse_value ("MemAvailable");
    if (result == 0) {
        result = parse_value("MemFree");
    }

    return result;

#elif defined(MAGNA_APPLE)

    int mib[2];
    struct vmtotal vminfo;
    size_t length;

    mib[0] = CTL_VM;
    mib[1] = VM_METER;
    length = sizeof (vminfo);
    sysctl (mib, 2, &vminfo, &length, NULL, 0);

    return vminfo.t_free * 4096;

#else

    return 0;

#endif
}

/**
 * Общее количество свободной виртуальной памяти в компьютере.
 *
 * @return Объем свободной виртуальной памяти в байтах.
 */
MAGNA_API am_uint64 mem_avail_virtual (void)
{
#ifdef MAGNA_WINDOWS

    MEMORYSTATUSEX status;

    mem_clear (&status, sizeof (status));
    status.dwLength = sizeof (status);

    if (!GlobalMemoryStatusEx (&status)) {
        return 0;
    }

    return status.ullAvailPageFile;

#elif defined(MAGNA_LINUX)

    return parse_value ("MemFree") + parse_value ("SwapFree");

#elif defined(MAGNA_APPLE)

    int mib[2];
    struct xsw_usage swapinfo;
    size_t length;

    mib[0] = CTL_VM;
    mib[1] = VM_SWAPUSAGE;
    length = sizeof (swapinfo);
    sysctl (mib, 2, &swapinfo, &length, NULL, 0);

    return mem_avail_physical() + swapinfo.xsu_avail;

#else

    return 0;

#endif
}

/*=========================================================*/

/**
 * Мы выполняемся на машине с маленькой памятью?
 *
 * @return Результат проверки.
 */
MAGNA_API am_bool mem_is_small_machine (void)
{
    /* Установлено меньше 1 Мб оперативной памяти? */

    return mem_avail_physical() < (MAGNA_INT64 (1024) * MAGNA_INT64 (1024));
}

/**
 * Мы выполняемся на машине с огромной памятью?
 *
 * @return Результат проверки.
 */
MAGNA_API am_bool mem_is_huge_machine (void)
{
    /* Доступно больше 2 Гб оперативной памяти? */

    return mem_avail_physical() > (MAGNA_INT64(2) * MAGNA_INT64 (1024) * MAGNA_INT64 (1024));
}

/*=========================================================*/

#ifdef MAGNA_LINUX

/*
   Работа с файлом `/proc/self/maps`, содержащим описание регионов
   в адресном пространстве процесса.

   Типичная запись выглядит следующим образом:

   address           perms offset  dev   inode   pathname
   08048000-08056000 r-xp 00000000 03:0c 64593   /usr/sbin/gpm

   Здесь `address` — начало и конец региона адресного пространства,
   `perms` – флаги доступа: `r` – чтение, `w` – запись,
   `x` – выполнение кода, `s` – shared-регион, `p` – private-регион,
   `offset` – если регион проецируется из файла с помощью `mmap`,
   тут хранится смещение относительно начала файла.
   Ну и собственно inode файла и путь к нему.

 */

/* Одна запись в файле */
typedef struct {
    am_uint64 from, to;
    am_byte flags[4];
} MME;

static am_bool read_memory_map
    (
        Array *array
    )
{
    am_bool result = AM_FALSE;
    char line[256];
    TextNavigator nav;
    Span from, to, flags;
    MME *mme;
    FILE *file = fopen ("/proc/self/maps", "rt");

    if (!file) {
        return AM_FALSE;
    }

    mem_clear (line, sizeof (line));
    while (!feof (file)) {
        if (!fgets (line, sizeof (line) - 1, file)) {
            break;
        }

        nav_init (&nav, line, strlen (line));
        from = nav_read_to (&nav, '-');
        to = nav_read_to (&nav, ' ');
        flags = nav_read_to (&nav, ' ');
        if (span_is_empty (from) || span_is_empty (to) || span_is_empty (flags)) {
            break;
        }

        mme = (MME*) array_emplace_back (array);
        if (!mme) {
            goto DONE;
        }
        mme->from     = span_hex_to_uint64 (from);
        mme->to       = span_hex_to_uint64 (to);
        mme->flags[0] = flags.start [0];
        mme->flags[1] = flags.start [1];
        mme->flags[2] = flags.start [2];
        mme->flags[3] = flags.start [3];
    }

    result = AM_TRUE;

    DONE: fclose (file);

    return result;
}

static const MME* find_address
    (
        const Array *array,
        am_uint64 address
    )
{
    size_t index;
    const MME *mme;

    for (index = 0; index < array->len; ++index) {
        mme = (const MME*) array_get (array, index);
        if (mme->from <= address && mme->to >= address) {
            return mme;
        }
    }

    return NULL;
}

static am_bool check_memory_map
    (
        am_uint64 address,
        size_t count,
        am_byte flag
    )
{
    am_bool result;
    Array memmap = ARRAY_INIT (sizeof (MME));
    const MME *mme;
    int i;

    if (address == MAGNA_UINT64 (0)) {
        return AM_FALSE;
    }

    if (!read_memory_map (&memmap)) {
        array_destroy (&memmap, NULL);
        return AM_FALSE;
    }

    result = AM_FALSE;
    mme = find_address (&memmap, address);
    if (mme != NULL) {
        if ((address + count) < mme->to) {
          for (i = 0; i < 4; ++i) {
              if (same_char (flag, mme->flags[i])) {
                  result = AM_TRUE;
                  break;
              }
          }
        }
    }

    array_destroy (&memmap, NULL);

    return result;
}

#endif

/**
 * Можно ли исполнять код по указанному адресу?
 *
 * @param ptr Указатель на начало.
 * @param count Число байт.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL mem_can_execute
    (
        const void *ptr
    )
{
#ifdef MAGNA_WINDOWS

    return !IsBadCodePtr ((FARPROC) ptr);

#elif defined(MAGNA_LINUX)

    return check_memory_map ((am_uint64) ptr, 4, 'x');

#else

    return AM_TRUE;

#endif
}

/**
 * Можно ли читать память по указанному адресу?
 *
 * @param ptr Указатель на начало.
 * @param count Число байт.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL mem_can_read
    (
        const void *ptr,
        size_t count
    )
{
#ifdef MAGNA_WINDOWS

    return !IsBadReadPtr (ptr, count);

#elif defined(MAGNA_LINUX)

    return check_memory_map ((am_uint64) ptr, count, 'r');

#else

    if (ptr == NULL) {
        return AM_FALSE;
    }

    return AM_TRUE;

#endif
}

/**
 * Можно ли писать в память по указанному адресу?
 *
 * @param ptr Указатель на начало.
 * @param count Число байт.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL mem_can_write
    (
        void *ptr,
        size_t count
    )
{
#ifdef MAGNA_WINDOWS

    return !IsBadWritePtr (ptr, count);

#elif defined(MAGNA_LINUX)

    return check_memory_map ((am_uint64) ptr, count, 'w');

#else

    if (ptr == NULL) {
        return AM_FALSE;
    }

    return AM_TRUE;

#endif
}


/*=========================================================*/

/* Добавление чанка к аллокатору */
static am_byte append_chunk
    (
        Arena *arena
    )
{

    ArenaChunk *chunk;

    chunk = mem_alloc (arena->chunkSize);
    if (chunk == NULL) {
        return AM_FALSE;
    }

    chunk->next = NULL;

    if (arena->first == NULL) {
        arena->first = arena->last = chunk;
    }
    else {
        arena->last->next = chunk;
        arena->last = chunk;
    }

    arena->current = (am_byte*) (chunk + 1);
    arena->remaining = arena->chunkSize - sizeof (ArenaChunk);

    return AM_TRUE;
}

/**
 * Инициализация аллокатора.
 *
 * @param arena Указатель на неинициализированную структуру.
 * @param chunkSize Размер чанка. 0 означает "определяется системой".
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL arena_init
    (
        Arena *MAGNA_RESTRICT arena,
        size_t chunkSize
    )
{
    assert (arena != NULL);

    if (chunkSize < (sizeof (void*) * 2)) {
        chunkSize = 4096;
    }

    arena->first = arena->last = NULL;
    arena->chunkSize = chunkSize;

    return append_chunk (arena);
}

/**
 * Освобождение ресурсов, занятых аллокатором.
 *
 * @param arena Указатель на аллокатор.
 */
MAGNA_API void MAGNA_CALL arena_destroy
    (
        Arena *MAGNA_RESTRICT arena
    )
{
    ArenaChunk *chunk, *next;

    assert (arena != NULL);

    for (chunk = arena->first; chunk != NULL; ) {
        next = chunk->next;
        mem_free (chunk);
        chunk = next;
    }

    arena->first = arena->last = NULL;
}

/**
 * Запрос блока памяти.
 *
 * @param arena Аллокатор.
 * @param length Требуемая длина блока.
 * Запросы на блоки больше `chunkSize - sizeof (void*)`
 * не могут быть удовлетворены.
 * @return Указатель на выделенный блок либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL arena_alloc
    (
        Arena *MAGNA_RESTRICT arena,
        size_t length
    )
{
    void *result;


    assert (arena != NULL);
    assert (arena->first != NULL);
    assert (length != 0);

    if (length > (arena->chunkSize - sizeof (ArenaChunk))) {
        return NULL;
    }

    /* Округляем вверх по модулю 4 */
    length = (length + 3) & ~3;

    if (length > arena->remaining) {
        if (!append_chunk (arena)) {
            return NULL;
        }
    }

    result = (void*) arena->current;
    arena->current += length;
    arena->remaining -= length;

    return result;
}

/**
 * Вычисление общего размера памяти, занятой аллокатором.
 *
 * @param arena Аллокатор.
 * @return Общий размер занятой памяти.
 */
MAGNA_API size_t MAGNA_CALL arena_total
    (
        const Arena *MAGNA_RESTRICT arena
    )
{
    size_t result = 0;
    const ArenaChunk *chunk;

    assert (arena != NULL);

    for (chunk =  arena->first; chunk != NULL; chunk = chunk->next) {
        result += arena->chunkSize;
    }

    return result;
}

/*=========================================================*/

#ifdef MAGNA_64BIT
typedef am_uint64 am_uintptr_t;
#else
typedef am_uint32 am_uintptr_t;
#endif

typedef am_uint16 am_offset_t;
#define PTR_OFFSET_SZ sizeof(am_offset_t)

#ifndef align_up
#define align_up(num, align) \
    (((num) + ((align) - 1)) & ~((align) - 1))
#endif

/**
 *
 *
 * @param alignment
 * @param size
 * @return
 * @remarks Borrowed from
 * https://embeddedartistry.com/blog/2017/02/22/generating-aligned-memory/
 */
MAGNA_API void* MAGNA_CALL mem_aligned_alloc
    (
        size_t alignment,
        size_t size
    )
{
    /*
                              aligned_mem (returned to caller)
                              V
        +---------------+-----+---------------------------------+
        |               |  ^  |  at least size bytes            |
        +---------------+--+--+---------------------------------+
        ^                  |
        allocated_mem      value of allocated_mem
        points here        stored here

     */

    am_uint32 headerSize;
    void *result = NULL;
    void *p;

    /* We want it to be a power of two since
       align_up operates on powers of two */
    assert ((alignment & (alignment - 1)) == 0);

    if (alignment && size)
    {
        /*
         * We know we have to fit an offset value
         * We also allocate extra bytes to ensure we
         * can meet the alignment
         */
        headerSize = PTR_OFFSET_SZ + (alignment - 1);
        p = mem_alloc (size + headerSize);

        if (p)
        {
            /*
             * Add the offset size to malloc's pointer
             * (we will always store that)
             * Then align the resulting value to the
             * target alignment
             */
            result = (void*) align_up (((am_uintptr_t) p + PTR_OFFSET_SZ), alignment);

            /* Calculate the offset and store it
               behind our aligned pointer */
            *((am_offset_t*) result - 1) =
                    (am_offset_t) ((am_uintptr_t) result - (am_uintptr_t) p);

        } /* else NULL, could not malloc */
    } /* else NULL, invalid arguments */

    return result;
}

/**
 *
 * @param ptr
 */
MAGNA_API void MAGNA_CALL mem_aligned_free
    (
        void *ptr
    )
{
    am_offset_t offset;
    void *p;

    assert (ptr);

    /*
    * Walk backwards from the passed-in pointer
    * to get the pointer offset. We convert to an offset_t
    * pointer and rely on pointer math to get the data
    */
    offset = *((am_offset_t *) ptr - 1);

    /*
    * Once we have the offset, we can get our
    * original pointer and call free
    */
    p = (void*)((am_byte*) ptr - offset);

    mem_free (p);
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
