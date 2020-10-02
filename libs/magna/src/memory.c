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

#include <windows.h>

#endif

#ifdef MAGNA_LINUX

#include <sys/time.h>
#include <sys/resource.h>

#endif

#include <assert.h>
#include <ctype.h>

/*=========================================================*/

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
    return calloc (1, size);
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
        void *destination,
        const void *source,
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
            result = (size_t) span_to_uint_64 (found);
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
    texter_free (&texter);

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
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL arena_init
    (
        Arena *arena,
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
        Arena *arena
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
        Arena *arena,
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
        const Arena *arena
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

#include "warnpop.h"

/*=========================================================*/
