// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#ifdef _MSC_VER

#include <intrin.h>

#pragma intrinsic(_rotl16, _rotl, _rotr)

#endif

#if defined(__GCC__) || defined(__GNUC__) || defined(__CLANG__)

#include <x86intrin.h>

#endif

/*=========================================================*/

/**
 * \file io.c
 *
 * \details У ИРБИС64 довольно оригинальный формат записи целых чисел на диске.
 *
 * 32-битные целые записываются в сетевом формате (Big Endian).
 * 64-битные записываются как пара 32-битных, причем сначала идет младшее,
 * затем старшее.
 *
 */

/*=========================================================*/

/**
 * Преобразование сетевого порядка байт в хостовый для 16-битного числа.
 * Работает и в обратную сторону.
 *
 * @param value Слово.
 * @return Преобразованное слово.
 */
MAGNA_API MAGNA_INLINE am_uint16 MAGNA_CALL magna_ntohs
    (
        am_uint16 value
    )
{
#undef IMPLEMENTED

#ifdef _MSC_VER

    return _rotl16 (value, 8);

#define IMPLEMENTED

#endif

#if defined(__GCC__) || defined(__GNUC__) || defined(__CLANG__)

#ifndef __APPLE__

    return __rolw (value, 8);

#define IMPLEMENTED

#endif

#endif

#ifndef IMPLEMENTED

    return ((value & 0x00FFu) << 8u) + ((value & 0xFF00u) >> 8u);

#endif
}

/**
 * Преобразование сетевого порядка байт в хостовый для 32-битного числа
 *
 * @param value Двойное слово.
 * @return Преобразованное двойное слово.
 */
MAGNA_API MAGNA_INLINE am_uint32 MAGNA_CALL magna_ntohl
    (
        am_uint32 value
    )
{
#undef IMPLEMENTED

#if defined(_MSC_VER) || defined(__GCC__) || defined(__GNUC__) || defined(__CLANG__)

#ifndef __APPLE__

    return _rotr (value & 0x00FF00FFu, 8) | _rotl (value &0xFF00FF00u, 8);

#define IMPLEMENTED

#endif

#endif

#ifndef IMPLEMENTED

    return ((value & 0x000000FFu) << 24u)
        |  ((value & 0x0000FF00u) << 8u)
        |  ((value & 0x00FF0000u) >> 8u)
        |  ((value & 0xFF000000u) >> 24u);

#endif
}

/**
 * Декодирование 64-битного числа из сетевого формата ИРБИС64 в хостовый.
 *
 * @param junior Младшая часть числа.
 * @param senior Старшая часть числа.
 * @return Хостовое представление.
 */
MAGNA_API am_uint64 MAGNA_CALL irbis_decode_64
    (
        am_uint32 junior,
        am_uint32 senior
    )
{
    return (((am_uint64) magna_ntohl (senior)) << 32u)
        | ((am_uint64) magna_ntohl (junior));
}

/**
 * Кодирование 64-битного числа из хостового представления в
 *
 * @param buffer Куда помещать результат. Сначала будет записана младшая часть, затем старшая.
 * @param value Хостовое представление.
 */
MAGNA_API void MAGNA_CALL irbis_encode_64
    (
        am_uint32 *buffer,
        am_uint64 value
    )
{
    buffer[0] = magna_ntohl ((am_uint32) (value & 0xFFFFFFFFULL));
    buffer[1] = magna_ntohl ((am_uint32) ((value >> 32u) & 0xFFFFFFFFULL));
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
