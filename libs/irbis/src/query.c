/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

#include <assert.h>


/**
 * Добавление строки в кодировке ANSI (плюс перевод строки).
 *
 * @param query Клиентский запрос.
 * @param text Добавляемая строка (может быть пустой).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL query_add_ansi
    (
        Query *query,
        const char *text
    )
{
    Buffer temp;

    assert (query != NULL);
    assert (text != NULL);

    buffer_from_text (&temp, text);
    if (!buffer_ansi_to_utf8 (&query->buffer, &temp)
        || !buffer_putc (&query->buffer, 0x0A)) {
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * Добавление строки формата с предварительной подготовкой.
 * Также добавляется перевод строки.
 *
 * @param query Клиентский запрос.
 * @param text Добавляемая строка формата (может быть пустой).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL query_add_format
    (
        Query *query,
        const char *text
    )
{
    assert (query != NULL);
    assert (text != NULL);

    /* TODO: implement */

    return AM_TRUE;
}

/**
 * Добавление целого числа без знака (плюс перевод строки).
 *
 * @param query Клиентский запрос
 * @param value Целое число.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL query_add_int_32
    (
        Query *query,
        am_int32 value
    )
{
    am_byte temp[16];

    assert (query != NULL);

    sprintf (temp, "%u", value);

    return query_add_utf (query, temp);
}

/**
 * Добавление строки в кодировке UTF-8 (плюс перевод строки).
 *
 * @param query Клиентский запрос.
 * @param text Добавляемая строка (может быть пустой).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL query_add_utf
    (
        Query *query,
        const char *text
    )
{
    assert (query != NULL);
    assert (text != NULL);

    if (!buffer_puts (&query->buffer, text)
        || !buffer_putc (&query->buffer, 0x0A)) {
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * Добавление символа перевода строки (например,
 * для создания в буфере пустой строки).
 *
 * @param query Клиентский запрос.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL query_new_line
    (
        Query *query
    )
{
    assert (query != NULL);

    return buffer_putc (&query->buffer, 0x0A);
}

/**
 * Создание пользовательского запроса.
 *
 * @param connection Подключение.
 * @param query Создаваемый запрос.
 * @param command Код команды (в кодировке UTF-8).
 */
MAGNA_API am_bool MAGNA_CALL query_create
    (
        struct IrbisConnection *connection,
        Query *query,
        const char *command
    )
{
    assert (connection != NULL);
    assert (query != NULL);
    assert (strlen (command) != 0);

    if (!buffer_create (&query->buffer, NULL, 16)
        || !query_add_ansi (query, command)
        || !buffer_putc (&query->buffer, connection->workstation)
        || !query_add_ansi (query, command)
        || !query_add_int_32 (query, connection->clientId)
        || !query_add_int_32 (query, connection->queryId)
        || !query_add_ansi_buffer (query, &connection->password)
        || !query_add_ansi_buffer (query, &connection->username)
        || !query_new_line (query)
        || !query_new_line (query)
        || !query_new_line (query)) {
        return AM_FALSE;
    }

    ++connection->queryId;

    return AM_TRUE;
}

/**
 * Освобождение ресурсов, занятых клиентским запросом.
 *
 * @param query Клиентский запрос.
 */
MAGNA_API void MAGNA_CALL query_free
    (
        Query *query
    )
{
    assert (query != NULL);

    buffer_free (&query->buffer);
}

/**
 * Кодирование префикса запроса.
 *
 * @param query Клиентский запрос.
 * @param prefix Инициализированный буфер для размещения префикса запроса.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL query_encode
    (
        const Query *query,
        Buffer *prefix
    )
{
    char temp[16];

    assert (query != NULL);
    assert (prefix != NULL);

    sprintf (temp, "%u\n", (unsigned int) query->buffer.position);

    return buffer_puts (prefix, temp);
}

/**
 * Добавление данных в кодировке ANSI (плюс перевод строки).
 *
 * @param query Клиентский запрос.
 * @param text Добавляемые данные.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL query_add_ansi_buffer
    (
        Query *query,
        const Buffer *text
    )
{
    assert (query != NULL);
    assert (text != NULL);

    if (!buffer_ansi_to_utf8 (&query->buffer, text)
        || !buffer_putc (&query->buffer, 0x0A)) {
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * Добавление данных в кодировке UTF-8 (плюс перевод строки).
 *
 * @param query Клиентский запрос.
 * @param text Добавляемые данные.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL query_add_utf_buffer
    (
        Query *query,
        const Buffer *text
    )
{
    assert (query != NULL);
    assert (text != NULL);

    if (!buffer_concat (&query->buffer, text)
        || !buffer_putc (&query->buffer, 0x0A)) {
        return AM_FALSE;
    }

    return AM_TRUE;
}
