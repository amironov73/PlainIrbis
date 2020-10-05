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
#include <stdarg.h>
#include <ctype.h>

/*=========================================================*/

/**
 * \file connect.c
 *
 * Работа с подключением к серверу ИРБИС64.
 *
 * Перед использованием структура `Connection` должна быть
 * проинициализирована. Для инициализации используйте
 * `connection_init`.
 *
 * Структура `Connection` владеет собственной памятью.
 * Для освобождения ресурсов используйте `connection_free`.
 */

/*=========================================================*/

/**
 * Инициализация структуры.
 *
 * @param connection Структура, подлежащая инициализации.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_create
    (
        Connection *connection
    )
{
    am_bool result;

    assert (connection != NULL);

    mem_clear (connection, sizeof (Connection));
    result = buffer_assign_text (&connection->host, "127.0.0.1")
        && buffer_assign_text (&connection->database, "IBIS");
    connection->port = 6666;
    connection->workstation = CATALOGER;

    return result;
}

/*=========================================================*/

/**
 * Освобождение ресурсов, занятых подключением.
 *
 * @param connection Структура, подлежащая очистке.
 * @warning После освобождения структура больше непригодна
 * для использования.
 */
MAGNA_API void MAGNA_CALL connection_destroy
    (
        Connection *connection
    )
{
    assert (connection != NULL);

    buffer_destroy (&connection->host);
    buffer_destroy (&connection->username);
    buffer_destroy (&connection->password);
    buffer_destroy (&connection->database);
    buffer_destroy (&connection->serverVersion);
    mem_clear (connection, sizeof (Connection));
}

/*=========================================================*/

/**
 * Настройка подключения: задание имени (или адреса) удаленного хоста.
 * Выполняется до установки соединения с сервером.
 *
 * @param connection Подключение.
 * @param host Имя или адрес удаленного хоста.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_set_host
    (
        Connection *connection,
        const char *host
    )
{
    assert (connection != NULL);
    assert (host != NULL);
    assert (!connection->connected);

    return buffer_assign_text (&connection->host, host);
}

/**
 * Настройка подключения: задание имени (логина) пользователя.
 * Выполняется до установки соединения с сервером.
 *
 * @param connection Подключение.
 * @param username Имя (логин)  пользователя. Не чувствительно к регистру символов.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_set_username
    (
        Connection *connection,
        const char *username
    )
{
    assert (connection != NULL);
    assert (username != NULL);
    assert (!connection->connected);

    return buffer_assign_text (&connection->username, username);
}

/**
 * Настройка подключения: задание пользовательского пароля.
 * Выполняется до установки соединения с сервером.
 *
 * @param connection Подключение.
 * @param password Пароль пользователя. Чувствителен к регистру символов.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_set_password
    (
        Connection *connection,
        const char *password
    )
{
    assert (connection != NULL);
    assert (password != NULL);
    assert (!connection->connected);

    return buffer_assign_text (&connection->password, password);
}

/**
 * Настройка подключения: задания имени текущей базы данных.
 * Может выполняться как до, так и после установки соединения с сервером.
 *
 * @param connection Подключения.
 * @param database Имя базы данных. Не чувствительно к регистру символов.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_set_database
    (
        Connection *connection,
        const char *database
    )
{
    assert (connection != NULL);
    assert (database != NULL);

    return buffer_assign_text (&connection->database, database);
}

/*=========================================================*/

/**
 * Актуализация всех неактуализированных записей в указанной базе данных.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_actualize_database
    (
        Connection *connection,
        const char *database
    )
{
    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    return AM_FALSE;
}

/**
 * Актуализация записи с указанным MFN.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных.
 * @param mfn MFN, подлежащий актуализации.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_actualize_record
    (
        Connection *connection,
        const char *database,
        am_mfn mfn
    )
{
    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    return AM_FALSE;
}

/**
 * Проверка, активно ли подключение.
 *
 * @param connection Подключение для проверки.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL connection_check
    (
        Connection *connection
    )
{
    assert (connection != NULL);

    if (!connection->connected) {
        connection->lastError = 100500;
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * Синоним для функции `connection_connect`.
 *
 * @param connection Неактивное подключение.
 * @return Признак успешности подключения.
 */
MAGNA_API am_bool MAGNA_CALL irbis_connect
    (
        Connection *connection
    )
{
    return connection_connect (connection);
}

/**
 * Подключение к серверу ИРБИС64.
 * Если подключене уже было установлено, функция ничего не делает.
 *
 * @param connection Неактивное подключение.
 * @return Признак успешности завершения операции.
 * Если подключение не удалось, код ошибки можно
 * посмотреть в `connection-&gt;lastError`.
 */
MAGNA_API am_bool MAGNA_CALL connection_connect
    (
        Connection *connection
    )
{
    Query query;
    Response response;
    am_bool result = AM_FALSE;

    LOG_ENTER;
    assert (connection != NULL);

    if (connection->connected) {
        LOG_LEAVE;
        return AM_TRUE;
    }

    AGAIN:
    connection->clientId = 100000 + (rand() % 900000);
    connection->queryId = 1;
    response_null (&response);

    if (!query_create (&query, connection, CBTEXT(REGISTER_CLIENT))) {
        LOG_LEAVE;
        return AM_FALSE;
    }

    if (!query_add_ansi_buffer (&query, &connection->username)
        || !query_add_ansi_buffer (&query, &connection->password)) {
        goto DONE;
    }

    if (!connection_execute (connection, &query, &response)) {
        goto DONE;
    }

    if (response_get_return_code (&response) == -3337) {
        /* Клиент с данным идентификатором уже зарегистрирован */
        /* Повторяем регистрацию с новым идентификатором */
        query_destroy (&query);
        response_destroy (&response);
        goto AGAIN;
    }

    if (response.returnCode < 0) {
        /* Нам вернули код ошибки */
        goto DONE;
    }

    connection->interval = response_read_int32 (&response);
    connection->connected = AM_TRUE;

    result = AM_TRUE;

    DONE:
    query_destroy (&query);
    response_destroy (&response);
    LOG_LEAVE;

    return result;
}

/**
 * Создание базы данных на сервере.
 *
 * @param connection Активное подключение.
 * @param database Имя создаваемой базы данных.
 * @param description Описание в свободной форме (опционально).
 * @param readerAccess Читатель будет иметь доступ?
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_create_database
    (
        Connection *connection,
        const char *database,
        const char *description,
        am_bool readerAccess
    )
{
    assert (database != NULL);

    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    return AM_FALSE;
}

/**
 * Создание словаря в указанной базе данных на сервере.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_create_dictionary
    (
        Connection *connection,
        const char *database
    )
{
    assert (database != NULL);

    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    return AM_FALSE;
}

/**
 * Удаление базы данных на сервере.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_delete_database
    (
        Connection *connection,
        const char *database
    )
{
    assert (database != NULL);

    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    return AM_FALSE;
}

/**
 * Удаление файла на сервере.
 *
 * @param connection Активное подключение.
 * @param fileName Имя базы файла.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_delete_file
    (
        Connection *connection,
        const char *fileName
    )
{
    assert (fileName != NULL);

    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    return AM_FALSE;
}

/**
 * Удаление записи по её MFN (в текущей базе данных).
 *
 * @param connection Активное подключение.
 * @param mfn MFN записи, подлежащей удалению.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_delete_record
    (
        Connection *connection,
        am_mfn mfn
    )
{
    assert (mfn > 0);

    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    return AM_FALSE;
}

/**
 * Синоним для `connection_disconnect`.
 *
 * @param connection Активное подключение.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL irbis_disconnect
    (
        Connection *connection
    )
{
    return connection_disconnect (connection);
}

/**
 * Отключение от сервера ИРБИС64.
 * Повторные попытки отключения уже отключенного
 * экземпляра клиента игнорируются.
 * Если при отключении был увеличен счетчик
 * использования лицензий, он соответственно
 * уменьшается.
 *
 * @param connection Активное подключение.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_disconnect
    (
        Connection *connection
    )
{
    am_bool result;
    Response response;

    LOG_ENTER;
    assert (connection != NULL);

    if (!connection->connected) {
        return AM_TRUE;
    }

    result = connection_execute_simple
        (
            connection,
            &response,
            CBTEXT (UNREGISTER_CLIENT),
            1,
            B2T (&connection->username)
        );

    connection->connected = AM_FALSE;
    response_destroy(&response);
    LOG_LEAVE;

    return result;
}

/**
 * Получение максимального MFN для указанной базы данных.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных. NULL означает текущую базу данных.
 * @return Максимальный MFN + 1 в случае успеха либо код ошибки.
 */
MAGNA_API am_mfn MAGNA_CALL connection_get_max_mfn
    (
        Connection *connection,
        const char *database
    )
{
    am_mfn result = 0;
    Response response;

    if (!database) {
        database = B2T (&connection->database);
    }

    if (connection_execute_simple
        (
            connection,
            &response,
            CBTEXT (GET_MAX_MFN),
            1,
            database
        )) {
        result = response.returnCode;
    }

    response_destroy(&response);

    return result;
}

/**
 * Отправка клиентского запроса на сервер ИРБИС64
 * и получение ответа от него.
 *
 * @param connection Активное подключение.
 * @param query Клиентский запрос.
 * @param response Ответ сервера.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_execute
    (
        Connection *connection,
        Query *query,
        Response *response
    )
{
    am_bool result = 0;
    Buffer prefix = BUFFER_INIT;
    const am_byte *hostname;
    am_int32 sockfd = -1;

    assert (connection != NULL);
    assert (query != NULL);
    assert (response != NULL);

    if (!response_create (connection, response)) {
        goto DONE;
    }

    hostname = buffer_to_text (&connection->host);
    if (hostname == NULL) {
        /* TODO: document the code */
        connection->lastError = 100501;
        goto DONE;
    }

    connection->lastError = 0;
    if (!query_encode (query, &prefix)) {
        goto DONE;
    }

    sockfd = tcp4_connect (hostname, connection->port);
    if (sockfd == -1) {
        goto DONE;
    }

    if (!tcp4_send_buffer (sockfd, &prefix)
        || !tcp4_send_buffer (sockfd, &query->buffer)) {
        goto DONE;
    }

    if (!tcp4_receive_all (sockfd, &response->answer)) {
        goto DONE;
    }

    tcp4_disconnect (sockfd);
    sockfd = -1;
    nav_from_buffer (&response->navigator, &response->answer);

    response->command = response_read_ansi (response);
    response->clientId = response_read_int32 (response);
    response->queryId = response_read_int32 (response);
    response->answerSize = response_read_int32 (response);
    response->serverVersion = response_read_ansi(response);
    response_get_line (response);
    response_get_line (response);
    response_get_line (response);
    response_get_line (response);
    response_get_line (response);

    result = AM_TRUE;

    DONE:
    if (sockfd != -1) {
        tcp4_disconnect (sockfd);
    }

    buffer_destroy (&prefix);

    return result;
}

/**
 * Упрощенное исполнение запроса к серверу.
 *
 * @param connection Активное подключение.
 * @param command Код команды, посылаемой на сервер.
 * @param ... Строки запроса (будут закодированы в ANSI).
 * @return Признак успешности выполнения операции.
 */
MAGNA_API am_bool connection_execute_simple
    (
        Connection *connection,
        Response *response,
        const am_byte *command,
        int argCount,
        ...
    )
{
    int i;
    va_list args;
    const am_byte *line;
    am_bool result = AM_FALSE;
    Query query;

    assert (connection != NULL);
    assert (response != NULL);
    assert (command != NULL);

    if (!connection_check (connection)
        || !query_create (&query, connection, command)) {
        return AM_FALSE;
    }

    if (argCount != 0) {
        va_start (args, argCount);
        for (i = 0; i < argCount; ++i) {
            line = va_arg (args, const am_byte*);
            if (!query_add_ansi (&query, line)) {
                va_end (args);
                goto DONE;
            }
        }
        va_end (args);
    }

    if (!connection_execute (connection, &query, response)) {
        goto DONE;
    }

    if (response_get_return_code (response) >= 0) {
        result = AM_TRUE;
    }

    DONE:
    query_destroy (&query);

    return result;
}

/**
 * Пустая операция (используется для периодического
 * подтверждения подключения клиента).
 *
 * @param connection Активное подключение.
 * @return Признак успешности заврешения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_no_operation
    (
        Connection *connection
    )
{
    am_bool result;
    Response response;

    assert (connection != NULL);

    result = connection_execute_simple
        (
            connection,
            &response,
            CBTEXT (NOP),
            0
        );

    response_destroy (&response);

    return result;
}

/**
 * Разбор строки подключения.
 *
 * @param connection Неактивное подключение.
 * @param connectionString Строка с параметрами подключения.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_parse_string
    (
        Connection *connection,
        Span connectionString
    )
{
    TextNavigator navigator;
    Span item, parts[2], key, value;

    assert (connection != NULL);
    assert (connection != NULL);

    nav_from_span (&navigator, connectionString);
    while (AM_TRUE) {
        item = span_trim (nav_read_to (&navigator, ';'));
        if (span_is_empty (item)) {
            break;
        }

        if (span_split_n_by_char (item, parts, 2, '=') != 2) {
            return AM_FALSE;
        }

        key = span_trim (parts[0]);
        value = span_trim (parts[1]);
        if (span_is_empty (key) || span_is_empty (value)) {
            return AM_FALSE;
        }

        if (span_compare_ignore_case (key, TEXT_SPAN ("host")) == 0
            || span_compare_ignore_case (key, TEXT_SPAN ("server")) == 0
            || span_compare_ignore_case (key, TEXT_SPAN ("address")) == 0) {
            if (!buffer_assign_span (&connection->host, value)) {
                return AM_FALSE;
            }
        }
        else if (span_compare_ignore_case (key, TEXT_SPAN ("port")) == 0) {
            connection->port = (am_int16) span_to_int32 (value);
        }
        else if (span_compare_ignore_case (key, TEXT_SPAN ("user")) == 0
            || span_compare_ignore_case (key, TEXT_SPAN ("username")) == 0
            || span_compare_ignore_case (key, TEXT_SPAN ("name")) == 0
            || span_compare_ignore_case (key, TEXT_SPAN ("login")) == 0) {
            if (!buffer_assign_span (&connection->username, value)) {
                return AM_FALSE;
            }
        }
        else if (span_compare_ignore_case (key, TEXT_SPAN ("password")) == 0
            || span_compare_ignore_case (key, TEXT_SPAN ("pwd")) == 0) {
            if (!buffer_assign_span (&connection->password, value)) {
                return AM_FALSE;
            }
        }
        else if (span_compare_ignore_case (key, TEXT_SPAN ("database")) == 0
                 || span_compare_ignore_case (key, TEXT_SPAN ("db")) == 0
                 || span_compare_ignore_case (key, TEXT_SPAN ("catalog")) == 0) {
            if (!buffer_assign_span (&connection->database, value)) {
                return AM_FALSE;
            }
        }
        else if (span_compare_ignore_case (key, TEXT_SPAN ("arm")) == 0
                 || span_compare_ignore_case (key, TEXT_SPAN ("workstation")) == 0) {
            connection->workstation = toupper (value.ptr[0]);
        }
        else {
            /* Неизвестный ключ */
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/**
 * Чтение текстового файла с сервера.
 *
 * @param connection Активное подключение.
 * @param specification Спецификация.
 * @param buffer Инициализированный буфер для размещения результата.
 * @return Признак успешности выполнения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_read_text_file
    (
        Connection *connection,
        const Specification *specification,
        Buffer *buffer
    )
{
    Query query;
    Response response;
    am_bool result = AM_FALSE;
    Span line;

    assert (specification != NULL);
    assert (buffer != NULL);

    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    response_null (&response);
    if (!query_create (&query, connection, CBTEXT (READ_DOCUMENT))) {
        return AM_FALSE;
    }

    if (!query_add_specification (&query, specification)) {
        goto DONE;
    }

    if (!connection_execute (connection, &query, &response)) {
        goto DONE;
    }

    line = response_get_line (&response);
    if (span_is_empty (line)) {
        goto DONE;
    }

    if (!irbis_to_client (buffer, line)) {
        goto DONE;
    }

    result = AM_TRUE;

    DONE:
    query_destroy (&query);
    response_destroy (&response);

    return result;
}

/**
 * Число найденных записей.
 *
 * @param connection
 * @param expression
 * @return Число найденных записей.
 */
MAGNA_API am_int32 MAGNA_CALL connection_search_count
    (
        Connection *connection,
        const char *expression
    )
{
    assert (connection != NULL);
    assert (expression != NULL);

    return -1;
}

/**
 * Расширенный поиск.
 *
 * @param connection
 * @param parameters
 * @param response
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_search_ex
    (
        Connection *connection,
        const SearchParameters *parameters,
        Response *response
    )
{
    assert (connection != NULL);
    assert (parameters != NULL);
    assert (response != NULL);

    return AM_FALSE;
}

/**
 * Формирование строки подключения по текущим настройкам.
 *
 * @param connection Подключение (не обязательно активное).
 * @param output Буфер для размещения результата.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_to_string
    (
        const Connection *connection,
        Buffer *output
    )
{
    assert (connection != NULL);
    assert (output != NULL);

    return buffer_puts (output, CBTEXT ("host="))
        && buffer_concat (output, &connection->host)
        && buffer_putc (output, ';')
        && buffer_puts (output, CBTEXT ("port="))
        && buffer_put_uint32 (output, connection->port)
        && buffer_putc (output, ';')
        && buffer_puts (output, CBTEXT ("username="))
        && buffer_concat (output, &connection->username)
        && buffer_putc (output, ';')
        && buffer_puts (output, CBTEXT ("password="))
        && buffer_concat (output, &connection->password)
        && buffer_putc (output, ';')
        && buffer_puts (output, CBTEXT ("database="))
        && buffer_concat (output, &connection->database)
        && buffer_putc (output, ';')
        && buffer_puts (output, CBTEXT ("workstation="))
        && buffer_putc (output, connection->workstation)
        && buffer_putc (output, ';');
}

MAGNA_API am_bool MAGNA_CALL connection_read_raw_record
    (
        Connection *connection,
        am_mfn mfn,
        Buffer *buffer
    )
{
    Query query;
    Response response;
    am_bool result = AM_FALSE;
    Span line;

    assert (connection != NULL);
    assert (mfn > 0);

    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    response_null (&response);
    if (!query_create (&query, connection, CBTEXT (READ_RECORD))) {
        return AM_FALSE;
    }

    if (!query_add_ansi_buffer (&query, &connection->database)
        || !query_add_int32 (&query, mfn)) {
        goto DONE;
    }

    if (!connection_execute (connection, &query, &response)) {
        goto DONE;
    }

    line = response_remaining_utf_text (&response);
    if (span_is_empty (line)) {
        goto DONE;
    }

    if (!buffer_write (buffer, line.ptr, line.len)) {
        goto DONE;
    }

    result = AM_TRUE;

    DONE:
    query_destroy (&query);
    response_destroy (&response);

    return result;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
