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
 * \file io.c
 *      \brief Ввод-вывод, в том числе преобразование из сетевого формата
 *      в локальный и упаковка чисел/строк.
 *      \details В `config.h` может быть определен макрос `BIG_ENDIAN`,
 *       который отключает преобразование в `magna_ntohs` и `magna_ntohl`.
 *
 * У ИРБИС64 довольно оригинальный формат записи
 * целых чисел на диске.
 *
 * 32-битные целые записываются в сетевом формате (Big Endian).
 * 64-битные записываются как пара 32-битных, причем сначала идет младшее,
 * затем старшее.
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
#ifdef MAGNA_BIG_ENDIAN

    return value;

#else

    return ((value & 0x00FFu) << 8u)
        |  ((value & 0xFF00u) >> 8u);

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
#ifdef MAGNA_BIG_ENDIAN

    return value;

#else

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

/**
 * Быстрая упаковка беззнакового 32-битного числа.
 * Использует по возможности меньше байт,
 * может обработать числа меньше 1073741823.
 *
 * ```
 *  Layout of packed UInt32 value
 *
 * bit number
 * -----------------------------------------------------------
 * | 31  30         |                                2  1  0 |
 * | length minus 1 |                                        |
 * -----------------------------------------------------------
 * ```
 *
 * @param value Значение.
 * @param bytes Байты для записи.
 * @return Количество байт, подлежащих записи.
 */
MAGNA_API int MAGNA_CALL fastpack_32
    (
        am_uint32 value,
        am_byte *bytes
    )
{
    *((am_uint32*) bytes) = magna_ntohl (value);

    if (value < 63u) /* 0x3F */ {
        return 1;
    }

    if (value < 16383u) /* 0x3FFF */ {
        bytes [2] |= 0x40u;
        return 2;
    }

    if (value < 4193303u) /* 0x3FFFFF */ {
        bytes [1] |= 0x80u;
        return 3;
    }

    if (value < 1073741823u) /* 0x3FFFFFFF */ {
        bytes [0] |= 0xC0;
        return 4;
    }

    fputs ("value is too big", stderr);
    abort ();

    return 0;
}

MAGNA_API MAGNA_INLINE unsigned int MAGNA_CALL fastlength_32
    (
        am_byte firstByte
    )
{
    return (firstByte & 0xC0) >> 6u;
}

MAGNA_API am_uint32 MAGNA_CALL fastunpack_32
    (
        const am_byte *bytes
    )
{
    unsigned int length = fastlength_32 (*bytes);
    am_uint32 result = *bytes & 0x3Fu;

    if (length == 1) {
        result = (result << 8u) + bytes[1];
    }
    else if (length == 2) {
        result = (result << 16u) + (bytes[1] << 8u) + bytes[2];
    }
    else if (length == 3){
        result = (result << 24u) + (bytes[1] << 16u)
                + (bytes[2] << 8u) + bytes[3];
    }

    return result;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
