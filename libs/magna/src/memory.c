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

/*=========================================================*/

/**
 * Запрос блока памяти.
 *
 * @param size Объем блока в байтах.
 * @return Указатель на блок либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL mem_alloc
    (
        am_size_t size
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
        am_size_t size
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
        am_size_t size
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
        am_size_t size
    )
{
    assert (destination != NULL);
    assert (source != NULL);

    memcpy (destination, source, size);
}

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

    return 0;

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

    return 0;

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

    return 0;

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

    return 0;

#else

    return 0;

#endif
}

/*=========================================================*/

/* Добавление чанка к аллокатору */
static am_byte append_chunk
    (
        Allocator *allocator
    )
{

    AllocatorChunk *chunk;

    chunk = mem_alloc (allocator->chunkSize);
    if (chunk == NULL) {
        return AM_FALSE;
    }

    chunk->next = NULL;

    if (allocator->first == NULL) {
        allocator->first = allocator->last = chunk;
    }
    else {
        allocator->last->next = chunk;
        allocator->last = chunk;
    }

    allocator->current = (am_byte*) (chunk + 1);
    allocator->remaining = allocator->chunkSize - sizeof (AllocatorChunk);

    return AM_TRUE;
}

/**
 * Инициализация аллокатора.
 *
 * @param allocator Указатель на неинициализированную структуру.
 * @param chunkSize Размер чанка. 0 означает "определяется системой".
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL allocator_init
    (
        Allocator *allocator,
        am_size_t chunkSize
    )
{
    assert (allocator != NULL);

    if (chunkSize < (sizeof (void*) * 2)) {
        chunkSize = 4096;
    }

    allocator->first = allocator->last = NULL;
    allocator->chunkSize = chunkSize;

    return append_chunk (allocator);
}

/**
 * Освобождение ресурсов, занятых аллокатором.
 *
 * @param allocator Указатель на аллокатор.
 */
MAGNA_API void MAGNA_CALL allocator_free
    (
        Allocator *allocator
    )
{
    AllocatorChunk *chunk, *next;

    assert (allocator != NULL);

    for (chunk = allocator->first; chunk != NULL; ) {
        next = chunk->next;
        mem_free (chunk);
        chunk = next;
    }

    allocator->first = allocator->last = NULL;
}

/**
 * Запрос блока памяти.
 *
 * @param allocator Аллокатор.
 * @param length Требуемая длина блока.
 * Запросы на блоки больше `chunkSize - sizeof (void*)`
 * не могут быть удовлетворены.
 * @return Указатель на выделенный блок либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL allocator_alloc
    (
        Allocator *allocator,
        am_size_t length
    )
{
    void *result;

    assert (allocator != NULL);
    assert (allocator->first != NULL);
    assert (length != 0);

    if (length > (allocator->chunkSize - sizeof (AllocatorChunk))) {
        return NULL;
    }

    if (length > allocator->remaining) {
        if (!append_chunk (allocator)) {
            return NULL;
        }
    }

    result = (void*) allocator->current;
    allocator->current += length;
    allocator->remaining -= length;

    return result;
}

/**
 * Вычисление общего размера памяти, занятой аллокатором.
 *
 * @param allocator Аллокатор.
 * @return Общий размер занятой памяти.
 */
MAGNA_API am_size_t MAGNA_CALL allocator_total
    (
        const Allocator *allocator
    )
{
    am_size_t result = 0;
    const AllocatorChunk *chunk;

    assert (allocator != NULL);

    for (chunk =  allocator->first; chunk != NULL; chunk = chunk->next) {
        result += allocator->chunkSize;
    }

    return result;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
