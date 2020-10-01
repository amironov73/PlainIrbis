// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#ifdef _MSC_VER

    #include <malloc.h>
    #define alloca _alloca

#elif defined(MAGNA_MSDOS)

    /* TODO: implement properly */

    #define alloca malloc

#else

    #if defined(__MINGW32__) || defined(__MINGW64__)

        #include <malloc.h>

    #else

        #include <alloca.h>

    #endif

#endif

/*=========================================================*/

#include <assert.h>

/**
 * \file fastbuf.c
 *
 * Буфер, накапливающий сначала данные на стеке,
 * и лишь при нехватке места использующий кучу.
 *
 * Владеет собственной памятью. Для освобождения
 * ресурсов используйте `fastbu_free`.
 */

/*=========================================================*/

/**
 * Инициализация буфера.
 *
 * @param buffer Буфер.
 * @param static_capacity Количество байт, размещаемых на стеке.
 */
MAGNA_API void MAGNA_CALL fastbuf_init
    (
        FastBuffer *buffer,
        size_t static_capacity
    )
{
    assert (buffer != NULL);
    assert (static_capacity >= 8);

    memset (buffer, 0, sizeof (FastBuffer));
    buffer->static_capacity = static_capacity;
    buffer->static_data = (am_byte*) alloca (static_capacity);
}

/**
 * Освобождение буфера.
 *
 * @param buffer Буфер.
 */
MAGNA_API void MAGNA_CALL fastbuf_free
    (
        FastBuffer *buffer
    )
{
    assert (buffer != NULL);

    if (buffer->dynamic_data) {
        free (buffer->dynamic_data);
        buffer->dynamic_data = NULL;
    }

    buffer->size = 0;
}

/**
 * Очистка буфера. Емкость не уменьшается,
 * динамический буфер (если он есть)
 * не освобождается.
 *
 * @param buffer Буфер.
 * @return Буфер.
 */
MAGNA_API FastBuffer* MAGNA_CALL fastbuf_clear
    (
        FastBuffer *buffer
    )
{
    assert (buffer != NULL);

    buffer->size = 0;

    return buffer;
}

/**
 * Проверка, не пуст ли буфер.
 *
 * @param buffer Буфер.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL fastbuf_empty
    (
        const FastBuffer *buffer
    )
{
    assert (buffer != NULL);

    return buffer->size == 0;
}

/**
 * Увеличение емкости буфера, если необходимо.
 *
 * @param buffer Буфер.
 * @param newSize Предполагаемая емкость.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL fastbuf_grow
    (
        FastBuffer *buffer,
        size_t newSize
    )
{
    assert (buffer != NULL);

    if (buffer->capacity < newSize) {
        return AM_FALSE;
    }

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Запись данных в буфер.
 *
 * @param buffer Буфер.
 * @param data Данные для записи.
 * @param dataSize Размер данных в байтах.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL fastbuf_write
    (
        FastBuffer *buffer,
        const am_byte *data,
        size_t dataSize
    )
{
    assert (buffer != NULL);
    assert (data != NULL);

    if (!fastbuf_grow (buffer, buffer->size + dataSize)) {
        return AM_FALSE;
    }

    return AM_FALSE;
}

MAGNA_API am_bool MAGNA_CALL fastbuf_putc
    (
        FastBuffer *buffer,
        char c
    )
{
    assert (buffer != NULL);

    return 0;
}

MAGNA_API am_bool MAGNA_CALL fastbuf_puts
    (
        FastBuffer *buffer,
        const char *text
    )
{
    assert (buffer != NULL);

    return 0;
}

MAGNA_API am_bool MAGNA_CALL fastbuf_put_utf8
    (
        FastBuffer *buffer,
        int c
    )
{
    assert (buffer != NULL);

    return 0;
}

MAGNA_API void MAGNA_CALL fastbuf_rewind
    (
        FastBuffer *buffer
    )
{
    assert (buffer != NULL);
}

MAGNA_API size_t MAGNA_CALL fastbuf_read
    (
        FastBuffer *buffer,
        am_byte *data,
        size_t size
    )
{
    assert (buffer != NULL);

    return 0;
}

MAGNA_API am_int32 MAGNA_CALL fastbuf_getc
    (
        FastBuffer *buffer
    )
{
    assert (buffer != NULL);

    return -1;
}

MAGNA_API am_int32 MAGNA_CALL fastbuf_getc_utf8
    (
        FastBuffer *buffer
    )
{
    assert (buffer != NULL);

    return -1;
}

MAGNA_API size_t MAGNA_CALL fastbuf_gets
    (
        FastBuffer *buffer,
        Buffer *text
    )
{
    assert (buffer != NULL);
    assert (text != NULL);

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
