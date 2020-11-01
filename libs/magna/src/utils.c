// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <stdarg.h>

/*=========================================================*/

/**
 * \file utils.c
 *
 * Разнообразные мелкие утилиты.
 */

/*=========================================================*/

/**
 * Выбор первой не пустой строки.
 *
 * @param first Строка для затравки.
 * @param ... Прочие строки.
 * @return Первая не пустая строка.
 * @warning Если все строки пустые, неопределенное поведение.
 */
MAGNA_API const am_byte* choose_string
    (
        const am_byte *first,
        ...
    )
{
    va_list args;
    const am_byte *next;

    if (first != NULL && *first) {
        return first;
    }

    va_start (args, first);

    for (;;) {
        next = va_arg (args, const am_byte*);
        if (next != NULL && *next) {
            va_end (args);
            return next;
        }
    }
}

/**
 * Выбор первого не пустого спана.
 *
 * @param first Спан для затравки.
 * @param ... Прочие спаны.
 * @return Первый не пустой спан.
 * @warning Если все спаны пустые, неопределенное поведение.
 */
MAGNA_API Span choose_span
    (
        Span first,
        ...
    )
{
    va_list args;
    Span next;

    if (!span_is_empty (first)) {
        return first;
    }

    va_start (args, first);

    for (;;) {
        next = va_arg (args, Span);
        if (!span_is_empty (next)) {
            va_end (args);
            return next;
        }
    }
}

/**
 * Выдает специфичный для платформы перевод строки.
 *
 * @return ASCIIZ с переводом строки.
 */
MAGNA_API MAGNA_INLINE const am_byte* newline (void)
{
#if defined(MAGNA_WINDOWS)

    return CBTEXT ("\r\n");

#else

    return CBTEXT ("\n");

#endif
}

/**
 * Выбор первого не пустого буфера.
 *
 * @param first Буфер для затравки.
 * @param ... Прочие буферы.
 * @return Первый не пустой буфер.
 * @warning Если все буферы пустые, неопределенное поведение.
 */
MAGNA_API Buffer* choose_buffer
    (
        const Buffer *first,
        ...
    )
{
    va_list args;
    const Buffer *next;

    if ((first != NULL) && !buffer_is_empty (first)) {
        return (Buffer*) first;
    }

    va_start (args, first);

    for (;;) {
        next = va_arg (args, const Buffer*);
        if (!buffer_is_empty (next)) {
            va_end (args);
            return (Buffer*) next;
        }
    }
}

/**
 * Вычисление минимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение минимума.
 */
MAGNA_API am_int32 MAGNA_CALL min_int32
    (
        am_int32 first,
        am_int32 second
    )
{
    return first < second ? first : second;
}

/**
 * Вычисление максимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение максимума.
 */
MAGNA_API am_int32 MAGNA_CALL max_int32
    (
        am_int32 first,
        am_int32 second
    )
{
    return first > second ? first : second;
}

/**
 * Вычисление минимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение минимума.
 */
MAGNA_API am_int32 MAGNA_CALL min_uint32
    (
        am_uint32 first,
        am_uint32 second
    )
{
    return first < second ? first : second;
}

/**
 * Вычисление максимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение максимума.
 */
MAGNA_API am_uint32 MAGNA_CALL max_uint32
    (
        am_uint32 first,
        am_uint32 second
    )
{
    return first > second ? first : second;
}

/**
 * Вычисление минимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение минимума.
 */
MAGNA_API am_int64 MAGNA_CALL min_int64
    (
        am_int64 first,
        am_int64 second
    )
{
    return first < second ? first : second;
}

/**
 * Вычисление максимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение максимума.
 */
MAGNA_API am_int64 MAGNA_CALL max_int64
    (
        am_int64 first,
        am_int64 second
    )
{
    return first > second ? first : second;
}

/**
 * Вычисление минимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение минимума.
 */
MAGNA_API am_uint64 MAGNA_CALL min_uint64
    (
        am_uint64 first,
        am_uint64 second
    )
{
    return first < second ? first : second;
}

/**
 * Вычисление максимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение максимума.
 */
MAGNA_API am_uint64 MAGNA_CALL max_uint64
    (
        am_uint64 first,
        am_uint64 second
    )
{
    return first > second ? first : second;
}

/**
 * Вычисление минимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение минимума.
 */
MAGNA_API size_t MAGNA_CALL min_size_t
    (
        size_t first,
        size_t second
    )
{
    return first < second ? first : second;
}

/**
 * Вычисление максимального из двух целых чисел.
 *
 * @param first Первое число.
 * @param second Второе число.
 * @return Значение максимума.
 */
MAGNA_API size_t MAGNA_CALL max_size_t
    (
        size_t first,
        size_t second
    )
{
    return first > second ? first : second;
}

/**
 * Знак целого числа.
 *
 * @param value Изучаемое значение.
 * @return -1, если отрицательное, 1 если положительное, иначе 0.
 */
MAGNA_API MAGNA_INLINE int MAGNA_CALL magna_sign_int
    (
        int value
    )
{
    return value > 0 ? 1 : (value < 0) ? -1 : 0;
}

/**
 * Знак целого числа.
 *
 * @param value Изучаемое значение.
 * @return -1, если отрицательное, 1 если положительное, иначе 0.
 */
MAGNA_API MAGNA_INLINE int MAGNA_CALL magna_sign_int32
    (
        am_int32 value
    )
{
    return value > 0 ? 1 : (value < 0) ? -1 : 0;
}

/**
 * Знак целого числа.
 *
 * @param value Изучаемое значение.
 * @return -1, если отрицательное, 1 если положительное, иначе 0.
 */
MAGNA_API MAGNA_INLINE int MAGNA_CALL magna_sign_int64
    (
        am_int64 value
    )
{
    return value > 0 ? 1 : (value < 0) ? -1 : 0;
}

/*=========================================================*/

/*
 * https://ru.wikipedia.org/wiki/%D0%9B%D0%B8%D0%BD%D0%B5%D0%B9%D0%BD%D1%8B%D0%B9_%D0%BA%D0%BE%D0%BD%D0%B3%D1%80%D1%83%D1%8D%D0%BD%D1%82%D0%BD%D1%8B%D0%B9_%D0%BC%D0%B5%D1%82%D0%BE%D0%B4
 *
 * Линейный конгруэнтный метод — один из методов генерации
 * псевдослучайных чисел. Применяется в простых случаях
 * и не обладает криптографической стойкостью.
 * Входит в стандартные библиотеки различных компиляторов.
 *
 */

static am_uint32 random_next = 1u;

MAGNA_API am_uint32 MAGNA_CALL random_get (void)
{
    if (random_next == 1u) {
        random_init ((am_uint32) time (NULL));
    }

    random_next = random_next * 1103515245u + 12345u;

    return (am_uint32) (random_next >> 16u) & 0xFFFFu;
}

MAGNA_API void MAGNA_CALL random_init
    (
        am_uint32 seed
    )
{
    random_next = seed;
}

/*=========================================================*/

/*
 * https://ru.wikipedia.org/wiki/%D0%92%D0%B8%D1%85%D1%80%D1%8C_%D0%9C%D0%B5%D1%80%D1%81%D0%B5%D0%BD%D0%BD%D0%B0
 *
 * Вихрь Мерсе́нна (англ. Mersenne twister, MT) — генератор псевдослучайных чисел (ГПСЧ),
 * разработанный в 1997 году японскими учёными Макото Мацумото (яп. 松本 眞)
 * и Такудзи Нисимура (яп. 西村 拓士). Вихрь Мерсенна основывается на свойствах
 * простых чисел Мерсенна (отсюда название) и обеспечивает быструю генерацию
 * высококачественных по критерию случайности псевдослучайных чисел.
 *
 * Вихрь Мерсенна лишён многих недостатков, присущих другим ГПСЧ,
 * таких как малый период, предсказуемость, легко выявляемые статистические
 * закономерности.
 *
 * Тем не менее, этот генератор не является криптостойким, что ограничивает
 * его использование в криптографии.
 *
 * Существуют по меньшей мере два общих варианта алгоритма, различающихся
 * только величиной используемого простого числа Мерсенна, наиболее распространённым
 * из которых является алгоритм MT19937, период которого составляет 2^19937 − 1.
 */

/*
 * Реализация заимствована:
 * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/CODES/mt19937ar.c
 */

/* Period parameters */
#define MERSENNE_N 624
#define MERSENNE_M 397
#define MATRIX_A   0x9908b0dfUL /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static am_uint32 mt [MERSENNE_N]; /* the array for the state vector  */
static int mti= MERSENNE_N + 1;   /* mti==N+1 means mt[N] is not initialized */

/**
 * Задание стартового значения ГСЧ.
 *
 * @param s Стартовое значение.
 */
MAGNA_API void MAGNA_CALL mersenne_init
    (
        am_uint32 s
    )
{
    mt [0]= s & 0xffffffffUL;
    for (mti = 1; mti < MERSENNE_N; ++mti) {
        mt [mti] = (1812433253UL * (mt [mti-1] ^ (mt [mti-1] >> 30)) + mti);

        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt [mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/**
 * Генерация псевдослучайного числа в диапазоне от 0 до 0xFFFFFFF.
 *
 * @return Очередное псевдослучайное число.
 */
MAGNA_API am_uint32 MAGNA_CALL mersenne_get (void)
{
    am_uint32 y;
    static am_uint32 mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= MERSENNE_N) { /* generate N words at one time */
        int kk;

        if (mti == MERSENNE_N + 1)   /* if init_genrand() has not been called, */
            mersenne_init ((am_uint32) time (NULL)); /* a default initial seed is used */

        for (kk=0; kk < MERSENNE_N - MERSENNE_M; kk++) {
            y = (mt [kk] & UPPER_MASK) | (mt [kk+1] & LOWER_MASK);
            mt [kk] = mt [kk + MERSENNE_M] ^ (y >> 1) ^ mag01 [y & 0x1UL];
        }

        for (; kk < MERSENNE_N - 1; kk++) {
            y = (mt [kk] & UPPER_MASK) | (mt [kk+1] & LOWER_MASK);
            mt [kk] = mt [kk + (MERSENNE_M - MERSENNE_N)] ^ (y >> 1) ^ mag01 [y & 0x1UL];
        }

        y = (mt [MERSENNE_N - 1] & UPPER_MASK) | (mt [0] & LOWER_MASK);
        mt [MERSENNE_N - 1] = mt [MERSENNE_M - 1] ^ (y >> 1) ^ mag01 [y & 0x1UL];

        mti = 0;
    }

    y = mt [mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
