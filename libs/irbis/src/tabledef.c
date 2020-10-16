// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/irbis.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
   \file tabledef.c

   Определение таблицы для метода `connection_print_table`.

   \struct TableDefinition
        \brief Определение таблицы для метода `connection_print_table`.
        \details Владеет всей своей памятью. Для освобождения
        ресурсов используйте `table_definition_destroy`.

   \var TableDefinition::database
        \brief Имя базы данных.

   \var TableDefinition::table
        \brief Имя таблицы.

   \var TableDefinition::headers
        \brief Заголовки таблицы.

   \var TableDefinition::mode
        \brief Режим таблицы.

   \var TableDefinition::searchQuery
        \brief Поисковый запрос по словарю.

   \var TableDefinition::sequentialQuery
        \brief Запрос для последовательного поиска.

   \var TableDefinition::mfnList
        \brief Список MFN, по которым строится таблица.

   \var TableDefinition::minMfn
        \brief Минимальный MFN, 0 = нет.

   \var TableDefinition::maxMfn
        \brief Максимальный MFN, 0 = нет.

 */

/*=========================================================*/

/**
 * Простая инициализация структуры.
 * Не выделяет память в куче.
 *
 * @param table Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL table_definition_init
    (
        TableDefinition *table
    )
{
    assert (table != NULL);

    mem_clear (table, sizeof (*table));
}

/**
 * Освобождение ресурсов, занятых структурой.
 *
 * @param table Структура, подлежащая освобождению.
 */
MAGNA_API void MAGNA_CALL table_definition_destroy
    (
        TableDefinition *table
    )
{
    assert (table != NULL);

    buffer_destroy (&table->database);
    buffer_destroy (&table->table);
    buffer_destroy (&table->headers);
    buffer_destroy (&table->mode);
    buffer_destroy (&table->searchQuery);
    buffer_destroy (&table->searchQuery);
    int32_array_destroy (&table->mfnList);
    mem_clear (table, sizeof (*table));
}

/**
 * Кодирование определения таблицы в запрос к серверу.
 *
 * @param table Определение таблицы.
 * @param connection Активное подключение.
 * @param query Запрос к серверу.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL table_definition_encode
    (
        TableDefinition *table,
        Connection *connection,
        Query *query
    )
{
    const am_byte *database;

    assert (table != NULL);
    assert (connection != NULL);
    assert (query != NULL);
    assert (!buffer_is_empty (&table->table));

    database = buffer_is_empty (&table->database)
        ? B2B (&connection->database)
        : B2B (&table->database);

    return query_add_ansi (query, database)
        && query_add_ansi_buffer (query, &table->table)
        && query_new_line (query) /* instead of headers */
        && query_add_ansi_buffer (query, &table->mode)
        && query_add_utf_buffer (query, &table->searchQuery)
        && query_add_uint32 (query, table->minMfn)
        && query_add_uint32 (query, table->maxMfn)
        && query_add_utf_buffer (query, &table->sequentialQuery);
}

/**
 * Декодирование таблицы из ответа сервера.
 *
 * @param table Описание таблицы.
 * @param response Ответ сервера.
 * @param output Буфер для размещения результата.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL table_definition_decode
    (
        const TableDefinition *table,
        Response *response,
        Buffer *output
    )
{
    Span text;

    assert (table != NULL);
    assert (response != NULL);
    assert (output != NULL);

    (void) table;
    text = response_remaining_utf_text (response);
    if (span_is_empty (text)) {
        return AM_FALSE;
    }

    return buffer_assign_span (output, text);
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
