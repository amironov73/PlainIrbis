// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <ctype.h>
#include <assert.h>

/*=========================================================*/

/**
 * \file number.c
 *
 * Текст, содержащий фрагменты с числами.
 *
 * Владеет собственной памятью. Для освобождения ресурсов
 * используйте `number_free`.
 */

/*=========================================================*/

/**
 * Инициализация чанка.
 *
 * @param chunk Указатель на неинициализированный чанк.
 */
MAGNA_API void MAGNA_CALL ntc_init
    (
        NumberTextChunk *chunk
    )
{
    assert (chunk != NULL);

    (void) buffer_init (&chunk->prefix);
    chunk->value = 0;
    chunk->length = 0;
    chunk->haveValue = AM_FALSE;
}

/**
 * Освобождение ресурсов, занятых чанком.
 *
 * @param chunk Указатель на чанк.
 */
MAGNA_API void MAGNA_CALL ntc_free
    (
        NumberTextChunk *chunk
    )
{
    assert (chunk != NULL);

    buffer_free (&chunk->prefix);
}

/**
 * Сравнение чанков.
 *
 * @param first Первый чанк.
 * @param second Второй чанк.
 * @return Результат сравнения.
 */
MAGNA_API int MAGNA_CALL ntc_compare
    (
        const NumberTextChunk *first,
        const NumberTextChunk *second
    )
{
    int result;

    assert (first != NULL);
    assert (second != NULL);

    result = buffer_compare (&first->prefix, &second->prefix);
    if (result == 0) {
        result = first->haveValue && second->haveValue
            ? magna_sign_int64 (first->value - second->value)
            : first->haveValue - second->haveValue;
    }

    return result;
}

/**
 * У чанка есть префикс?
 *
 * @param chunk Чанк для проверки.
 * @return Результат проверки.
 */
MAGNA_API MAGNA_INLINE am_bool MAGNA_CALL ntc_have_prefix
    (
        const NumberTextChunk *chunk
    )
{
    assert (chunk != NULL);

    return chunk->prefix.position != 0;
}

/**
 * Первоначальная настройка чанка.
 *
 * @param chunk Указатель на чанк.
 * @param prefix Префиксная часть
 * @param number Значение.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL ntc_setup
    (
        NumberTextChunk *chunk,
        Span prefix,
        Span number
    )
{
    am_bool result = !span_is_empty (prefix);

    assert (chunk != NULL);

    buffer_assign_span (&chunk->prefix, prefix);
    if (span_is_empty (number)) {
        chunk->haveValue = AM_FALSE;
        chunk->value = 0;
    }
    else {
        result = AM_TRUE;
        chunk->haveValue = AM_TRUE;
        chunk->value = span_to_uint_64 (number);
    }
    chunk->length = 0;

    return result;
}

/**
 * Преобразование чанка в текст.
 *
 * @param chunk Указатель на чанк.
 * @param output Буфер для результата.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL ntc_to_string
    (
        const NumberTextChunk *chunk,
        Buffer *output
    )
{
    am_bool result;

    assert (chunk != NULL);
    assert (output != NULL);

    result = buffer_concat (output, &chunk->prefix);
    if (result && chunk->haveValue) {
        /* TODO: implement length */
        result = buffer_put_uint_64 (output, chunk->value);
    }

    return result;
}

/**
 * Получение чанка по его индексу.
 *
 * @param number Текст.
 * @param index Индекс.
 * @return Указатель на чанк либо `NULL`, если индекс неверен.
 */
MAGNA_API const NumberTextChunk* MAGNA_CALL number_get_chunk
    (
        const NumberText *number,
        size_t index
    )
{
    assert (number != NULL);

    if (index >= number->chunks.len) {
        return NULL;
    }

    return (NumberTextChunk*) number->chunks.ptr [index];
}

/**
 * Указатель на последний фрагмент в тексте.
 *
 * @param number Текст.
 * @return Указатель на последний фрагмент либо `NULL`,
 * если текст пустой.
 */
MAGNA_API MAGNA_INLINE const NumberTextChunk* MAGNA_CALL number_last
    (
        const NumberText *number
    )
{
    size_t length;

    assert (number != NULL);

    length = number->chunks.len;

    return (length != 0)
        ? number->chunks.ptr [length-1]
        : NULL;
}

/**
 * Инициализация фрагментированного текста для последующего построения.
 *
 * @param number Указатель на ненициализированную структуру.
 * @return Указатель на инициализированную структуру.
 */
MAGNA_API NumberText* MAGNA_CALL number_init
    (
        NumberText *number
    )
{
    assert (number != NULL);

    vector_create(&number->chunks, 2);

    return number;
}

/**
 * Освобождение ресурсов, занятых текстом.
 *
 * @param number Текст, подлежащий освобождению.
 */
MAGNA_API void MAGNA_CALL number_free
    (
        NumberText *number
    )
{
    size_t index;
    NumberTextChunk *chunk;

    assert (number != NULL);

    for (index = 0; index < number->chunks.len; ++index) {
        chunk = (NumberTextChunk*) number->chunks.ptr [index];
        ntc_free (chunk);
        mem_free (chunk);
    }

    vector_destroy(&number->chunks);
}

/**
 * Добавление чанка к тексту в конец.
 *
 * @param number Текст.
 * @return Указатель на добавленный чанк либо `NULL`.
 */
MAGNA_API NumberTextChunk* MAGNA_CALL number_append_chunk
    (
        NumberText *number
    )
{
    NumberTextChunk *result;

    assert (number != NULL);

    result = (NumberTextChunk*) mem_alloc (sizeof (NumberTextChunk));
    if (result == NULL) {
        return result;
    }

    ntc_init (result);
    if (vector_push_back(&number->chunks, result)) {
        mem_free (result);
        result = NULL;
        return result;
    }

    return result;
}

/**
 * Добавление фрагмента в конец.
 *
 * @param number Текст.
 * @param prefix Префикс (может быть пустым).
 * @param value Значение.
 * @param length Длина (допускается 0).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL number_append
    (
        NumberText *number,
        Span prefix,
        am_int64 value,
        int length
    )
{
    NumberTextChunk *chunk;

    assert (number != NULL);

    chunk = number_append_chunk (number);
    if (chunk == NULL) {
        return AM_FALSE;
    }

    if (!buffer_assign_span (&chunk->prefix, prefix)) {
        return AM_FALSE;
    }

    chunk->haveValue = AM_TRUE;
    chunk->value = value;
    chunk->length = length;

    return AM_TRUE;
}

/**
 * Количество фрагментов в тексте.
 *
 * @param number Исследуемый текст.
 * @return Количество фрагментов.
 */
MAGNA_API MAGNA_INLINE size_t MAGNA_CALL number_size
    (
        const NumberText *number
    )
{
    assert (number != NULL);

    return number->chunks.len;
}

/**
 * Разбор текста.
 *
 * @param number Указатель на неинициализированную
 * @param text
 * @return
 */
MAGNA_API am_bool MAGNA_CALL number_parse
    (
        NumberText *number,
        Span text
    )
{
    NumberTextChunk *chunk, temp;
    Span prefix, num;
    am_byte *ptr, *end, c;
    am_bool textPart;

    assert (number != NULL);

    end = (ptr = text.ptr) + text.len;
    num = span_null ();
    prefix = span_init (ptr, 0);
    textPart = AM_TRUE;
    for (; ptr < end; ++ptr) {
        c = *ptr;
        if (textPart) {
            if (isdigit (c)) {
                num = span_init (ptr, 1);
                textPart = AM_FALSE;
            }
            else {
                ++prefix.len;
            }
        }
        else {
            if (isdigit (c)) {
                ++num.len;
            }
            else {
                chunk = mem_alloc (sizeof (NumberTextChunk));
                if (chunk == NULL) {
                    return AM_FALSE;
                }

                ntc_init (chunk);
                if (!ntc_setup (chunk, prefix, num)
                    || vector_push_back(&number->chunks, chunk)) {
                    return AM_FALSE;
                }

                textPart = AM_TRUE;
                num = span_null ();
                prefix = span_init (ptr, 1);
            }
        }
    }

    ntc_init (&temp);
    if (!ntc_setup (&temp, prefix, num)) {
        ntc_free (&temp);
    }
    else {
        chunk = mem_alloc (sizeof (NumberTextChunk));
        if (chunk == NULL) {
            ntc_free (&temp);
            return AM_FALSE;
        }

        mem_copy (chunk, &temp, sizeof (temp));
        if (!vector_push_back(&number->chunks, chunk)) {
            ntc_free (&temp);
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/**
 * Инкремент последнего фрагмента в тексте на единицу.
 * Если текст пустой, инкремента не происходит.
 *
 * @param number Текст.
 * @return Тот же текст.
 */
MAGNA_API NumberText* MAGNA_CALL number_increment
    (
        NumberText *number
    )
{
    NumberTextChunk *chunk;

    assert (number != NULL);

    chunk = (NumberTextChunk*) number_last (number);
    if (chunk != NULL) {
        ++chunk->value;
    }

    return number;
}

/**
 * Инкремент указанного фрагмента в тексте на заданное значение.
 * Если указан несуществующий индекс фрагмента,
 * то инкремента не происходит.
 *
 * @param number Текст.
 * @param index Индекс фрагмента (нумерация с 0).
 * @param delta Приращение.
 * @return Тот же текст.
 */
MAGNA_API NumberText* MAGNA_CALL number_increment_ex
    (
        NumberText *number,
        size_t index,
        am_int64 delta
    )
{
    NumberTextChunk *chunk;

    assert (number != NULL);

    chunk = (NumberTextChunk*) number_get_chunk (number, index);
    if (chunk != NULL) {
        chunk->value += delta;
    }

    return number;
}

/**
 * Преобразование в текст.
 *
 * @param number Текст с фрагментами.
 * @param output Буфер для размещения результата.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL number_to_string
    (
        const NumberText *number,
        Buffer *output
    )
{
    size_t index;
    const NumberTextChunk *chunk;

    assert (number != NULL);
    assert (output != NULL);

    for (index = 0; index < number->chunks.len; ++index) {
        chunk = number_get_chunk (number, index);
        if (!ntc_to_string (chunk, output)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/**
 * Сравнение фрагментированных текстов.
 *
 * @param first Первый текст.
 * @param second Второй текст.
 * @return Результат сравнения: < 0, если первый текст меньше,
 * >0, если первый текст больше, =0 если тексты совпадают.
 */
MAGNA_API int MAGNA_CALL number_compare
    (
        const NumberText *first,
        const NumberText *second
    )
{
    size_t index;
    const NumberTextChunk *firstChunk, *secondChunk;
    int result;

    assert (first != NULL);
    assert (second != NULL);

    for (index = 0; ; ++index) {
        if (index >= number_size (first)) {
            if (index >= number_size (second)) {
                return 0;
            }
            return -1;
        }
        else {
            if (index > number_size (second)) {
                return 1;
            }

            firstChunk = number_get_chunk (first, index);
            secondChunk = number_get_chunk (second, index);
            result = ntc_compare (firstChunk, secondChunk);
            if (result != 0) {
                return result;
            }
        }
    }
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
