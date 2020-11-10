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
 * \struct Connection
 *      \brief Структура, хранящая все необходимые настройки
 *      для обмена данными с сервером ИРБИС64.
 *
 * \details Перед использованием структура `Connection`
 *  должна быть проинициализирована с помощью `connection_create`.
 *
 * Структура `Connection` владеет собственной памятью.
 * Для освобождения ресурсов используйте `connection_destroy`.
 *
 * \var Connection;:host
 *      \brief Имя или адрес хоста с сервером ИРБИС64.
 *      \details Значение по умолчанию "127.0.0.1".
 *
 * \var Connection::port
 *      \brief Номер порта на сервере ИРБИС64.
 *      \details По умолчанию `6666`.
 *
 * \var Connection::username
 *      \brief Имя пользователя системы ИРБИС64 (логин).
 *      \details Нечувствителен к регистру символов.
 *      По умолчанию пустая строка.
 *
 * \var Connection::password
 *      \brief Пароль пользователя системы ИРБИС64.
 *      \details Чувствителен к регистру символов.
 *      По умолчанию пустая строка.
 *
 * \var Connection::database
 *      \brief Имя текущей базы данных.
 *      \details Нечувствителен к регистру символов.
 *      По умолчанию "IBIS".
 *      Значение не должно быть пустым
 *      и должно представлять имя какой-либо из баз данных,
 *      имеющихся на сервере.
 *
 * \var Connection::workstation
 *      \brief Тип АРМ.
 *      \details По умолчанию 'C' (АРМ Каталогизатор).
 *      Значение должно быть кодом одного из разрешенных
 *      для данного пользователя АРМов.
 *
 * \var Connection::serverVersion
 *      \bfief Версия сервера (присылается при входе в систему).
 *      \details При некоторых сценариях сервер не присылвает свою версию.
 *
 * \var Connection::clientId
 *      \brief Идентификатор клиента -- случайное целое число.
 *
 * \var Connection::queryId
 *      \brief Порядковый номер запроса к серверу (нумерация с 1).
 *
 * \var Connection::lastError
 *      \brief Код ошибки последней выполненной операции.
 *      \details Значение больше или равное 0 означает отсутствие ошибки.
 *      Впрочем, некоторые отрицательные значения тоже могут означать
 *      отсутствие ошибки.
 *
 * \var Connection::interval
 *      \brief Рекомендуемый интервал подтверждения активности в минутах.
 *      \details Присылается сервером при регистрации в системе.
 *
 * \var Connection::connected
 *      \brief Признак активного подключения (устанавливается автоматически).
 *
 * \code
 * Connection connection;
 *
 * // Создаем подключение
 * if (!connection_create (&connection)) {
 *      fputs ("Can't create connection\n", stderr);
 *      return 1;
 *  }
 *
 * // Настраиваем параметры подключения
 * connection_set_host     (&connection, CBTEXT ("myhost"));
 * connection_set_username (&connection, CBTEXT ("librarian"));
 * connection_set_password (&connection, CBTEXT ("secret"));
 * connection_set_database (&connection, CBTEXT ("IStU"));
 * connection.workstation = CATALOGER;
 * connection.port = 6667; // Нестандартный порт
 *
 * // Пытаемся подключиться к серверу
 * if (!irbis_connect (&connection)) {
 *      fputs ("Connection failed\n", stderr);
 *      connection_destroy (&connection);
 *      return 1;
 *  }
 *
 * // Некие действия по обмену данными с сервером
 *
 * // Отключаемся от сервера и освобождаем ресурсы, занятые подключением.
 * irbis_disconnect (&connection);
 * connection_destroy (&connection);
 *
 * \endcode
 */

/*=========================================================*/

/**
 * Инициализация структуры.
 * Выделяет память в куче.
 *
 * @param connection Структура, подлежащая инициализации.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_create
    (
        Connection *connection
    )
{
    assert (connection != NULL);

    mem_clear (connection, sizeof (Connection));
    connection->port = 6666;
    connection->workstation = CATALOGER;

    return buffer_assign_text (&connection->host, CBTEXT ("127.0.0.1"))
        && buffer_assign_text (&connection->database, CBTEXT ("IBIS"));
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

    /* Если забыли отключиться, отключимся сейчас. */
    connection_disconnect (connection);

    buffer_destroy (&connection->host);
    buffer_destroy (&connection->username);
    buffer_destroy (&connection->password);
    buffer_destroy (&connection->database);
    buffer_destroy (&connection->serverVersion);
    mem_clear (connection, sizeof (*connection));

    /* В принципе, после этого подключение
     * можно вновь инициализировать и снова использовать.
     */
}

/*=========================================================*/

/**
 * Настройка подключения: задание имени (или адреса) удаленного хоста.
 * Выполняется до установки соединения с сервером.
 *
 * @param connection Подключение.
 * @param host Имя или адрес удаленного хоста.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_set_host
    (
        Connection *connection,
        const am_byte *host
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
 * @param username Имя (логин)  пользователя в системе ИРБИС64. Не чувствительно к регистру символов.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_set_username
    (
        Connection *connection,
        const am_byte *username
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
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_set_password
    (
        Connection *connection,
        const am_byte *password
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
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_set_database
    (
        Connection *connection,
        const am_byte *database
    )
{
    assert (connection != NULL);
    assert (database != NULL);

    return buffer_assign_text (&connection->database, database);
}

/*=========================================================*/

/**
 * Прерывает программу при обнаружении ошибки.
 *
 * @param connection Активное подключение.
 */
MAGNA_API void MAGNA_CALL connection_abort_on_error
    (
        const Connection *connection
    )
{
    assert (connection != NULL);

    if (connection->lastError < 0) {
        fputs ("\n\n", stderr);
        fputs ("ERROR: ", stderr);
        fputs (irbis_describe_error (connection->lastError), stderr);
        fputs ("\n\n", stderr);
        abort ();
    }
}

/*=========================================================*/

/**
 * Актуализация всех неактуализированных записей в указанной базе данных.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_actualize_database
    (
        Connection *connection,
        const am_byte *database
    )
{
    assert (connection != NULL);
    assert (database != NULL);

    return connection_actualize_record (connection, database, 0);
}

/**
 * Актуализация записи с указанным MFN.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных.
 * @param mfn MFN, подлежащий актуализации.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_actualize_record
    (
        Connection *connection,
        const am_byte *database,
        am_mfn mfn
    )
{
    Query query;
    Response response;
    am_bool result = AM_FALSE;

    assert (connection != NULL);
    assert (database != NULL);

    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    response_init (&response);
    if (!query_create (&query, connection, CBTEXT (ACTUALIZE_RECORD))) {
        return AM_FALSE;
    }

    if (!query_add_ansi (&query, CBTEXT (database))
        || !query_add_int32 (&query, mfn)) {
        goto DONE;
    }

    if (!connection_execute (connection, &query, &response)) {
        goto DONE;
    }

    result = response_get_return_code (&response) >= 0;

    DONE:
    query_destroy (&query);
    response_destroy (&response);

    return result;
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
 * @return Признак успешного подключения.
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
 * @return Признак успешного завершения операции.
 * Если подключение не удалось, код ошибки можно
 * посмотреть в `connection-&gt;lastError`.
 */
MAGNA_API am_bool MAGNA_CALL connection_connect
    (
        Connection *connection
    )
{
    Query query;               /* клиентский запрос */
    Response response;         /* ответ сервера */
    am_bool result = AM_FALSE; /* признак успеха */

    LOG_ENTER;
    assert (connection != NULL);

    if (connection->connected) {
        LOG_LEAVE;
        return AM_TRUE;
    }

    AGAIN:
    connection->clientId = 100000 + (random_get() % 900000);
    connection->queryId = 1;
    response_init (&response);

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
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_create_database
    (
        Connection *connection,
        const am_byte *database,
        const am_byte *description,
        am_bool readerAccess
    )
{
    am_bool result;              /* признак успеха */
    Response response;           /* ответ сервера */
    const am_byte *accessString; /* строка для ИРБИС-протокола */

    if (!database) {
        database = B2B (&connection->database);
    }

    accessString = CBTEXT (readerAccess ? "1" : "0");
    result = connection_execute_simple
        (
            connection,
            &response,
            CBTEXT (CREATE_DATABASE),
            3,
            database,
            description,
            accessString
        );

    response_destroy (&response);

    return result;
}

/**
 * Создание словаря в указанной базе данных на сервере.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_create_dictionary
    (
        Connection *connection,
        const am_byte *database
    )
{
    am_bool result;    /* признак успеха */
    Response response; /* ответ сервера */

    if (!database) {
        database = B2B (&connection->database);
    }

    result = connection_execute_simple
        (
            connection,
            &response,
            CBTEXT (CREATE_DICTIONARY),
            1,
            database
        );

    response_destroy (&response);

    return result;
}

/**
 * Удаление базы данных на сервере.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_delete_database
    (
        Connection *connection,
        const am_byte *database
    )
{
    am_bool result;    /* признак успеха */
    Response response; /* ответ сервера */

    if (!database) {
        database = B2B (&connection->database);
    }

    result = connection_execute_simple
        (
            connection,
            &response,
            CBTEXT (DELETE_DATABASE),
            1,
            database
        );

    response_destroy (&response);

    return result;
}

/**
 * Удаление файла на сервере.
 *
 * @param connection Активное подключение.
 * @param fileName Имя базы файла.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_delete_file
    (
        Connection *connection,
        const am_byte *fileName
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
 * @return Признак успешного завершения операции.
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
 * @return Признак успешного завершения операции.
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
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_disconnect
    (
        Connection *connection
    )
{
    am_bool result;    /* признак успеха */
    Response response; /* ответ сервера */

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
        const am_byte *database
    )
{
    am_mfn result = 0; /* результат: максимальный MFN или код ошибки */
    Response response; /* ответ сервера */

    if (!database) {
        database = B2B (&connection->database);
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
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_execute
    (
        Connection *connection,
        Query *query,
        Response *response
    )
{
    am_bool result = AM_FALSE;    /* признак успеха */
    Buffer header = BUFFER_INIT;  /* заголовок пакета с запросом (длина пакета в байтах) */
    const am_byte *hostname;      /* имя хоста */
    am_int32 sockfd = -1;         /* сокет */

    assert (connection != NULL);
    assert (query != NULL);
    assert (response != NULL);

    response_init (response);
    response->connection = connection;
    hostname = buffer_to_text (&connection->host);
    if (hostname == NULL) {
        /* TODO: document the error code */
        connection->lastError = 100501;
        goto DONE;
    }

    sockfd = tcp4_connect (hostname, connection->port);
    if (sockfd == -1) {
        goto DONE;
    }

    connection->lastError = 0;
    if (!query_encode (query, &header)) {
        goto DONE;
    }

    if (!tcp4_send_buffer (sockfd, &header)
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

    buffer_destroy (&header);

    return result;
}

/**
 * Упрощенное исполнение запроса к серверу.
 *
 * @param connection Активное подключение.
 * @param command Код команды, посылаемой на сервер.
 * @param ... Строки запроса (будут закодированы в ANSI).
 * @return Признак успешного выполнения операции.
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
    int index;                 /* индекс цикла */
    va_list args;              /* бродилка по аргументам */
    const am_byte *line;       /* строка */
    am_bool result = AM_FALSE; /* признак успеха */
    Query query;               /* клиенский запрос */

    assert (connection != NULL);
    assert (response != NULL);
    assert (command != NULL);

    if (!connection_check (connection)
        || !query_create (&query, connection, command)) {
        return AM_FALSE;
    }

    if (argCount != 0) {
        va_start (args, argCount);
        for (index = 0; index < argCount; ++index) {
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

    result = response_get_return_code (response) >= 0;

    DONE:
    query_destroy (&query);

    return result;
}

/**
 * Форматирование указанного MFN.
 *
 * @param connection Активное подключение.
 * @param format Формат.
 * @param mfn MFN записи.
 * @param output Буфер для размещения результата.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_format_mfn
    (
        Connection *connection,
        const am_byte *format,
        am_mfn mfn,
        Buffer *output
    )
{
    Query query;
    Response response;
    const char *database;
    Span text;
    am_bool result = AM_FALSE;

    assert (connection != NULL);
    assert (format != NULL);
    assert (mfn > 0);
    assert (output != NULL);

    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    response_init (&response);
    if (!query_create (&query, connection, CBTEXT (FORMAT_RECORD))) {
        return AM_FALSE;
    }

    database = CCTEXT (buffer_to_text (&connection->database));
    if (database == NULL) {
        goto DONE;
    }

    if (!query_add_ansi (&query, CBTEXT (database))
        || !query_add_format (&query, CBTEXT (format))
        || !query_add_int32 (&query, 1)
        || !query_add_int32 (&query, mfn)) {
        goto DONE;
    }

    if (!connection_execute (connection, &query, &response)) {
        goto DONE;
    }

    result = response_get_return_code (&response) >= 0;

    if (result) {
        text = response_remaining_utf_text (&response);
        if (span_is_empty (text)) {
            result = AM_FALSE;
        }
        else {
            if (!buffer_write (output, text.start, span_length (text))) {
                result = AM_FALSE;
            }
        }
    }

    DONE:
    query_destroy (&query);
    response_destroy (&response);

    return result;
}

/**
 * Пустая операция (используется для периодического
 * подтверждения подключения клиента).
 *
 * @param connection Активное подключение.
 * @return Признак успешного заврешения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_get_server_version
    (
        Connection *connection,
        ServerVersion *version
    )
{
    am_bool result;    /* признак успеха */
    Response response; /* ответ сервера */

    assert (connection != NULL);
    assert (version != NULL);

    result = connection_execute_simple
        (
            connection,
            &response,
            CBTEXT (SERVER_INFO),
            0
        )

        && version_parse_response (version, &response);

    response_destroy (&response);

    return result;
}

/**
 * Пустая операция (используется для периодического
 * подтверждения подключения клиента).
 *
 * @param connection Активное подключение.
 * @return Признак успешного заврешения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_no_operation
    (
        Connection *connection
    )
{
    am_bool result;    /* признак успеха */
    Response response; /* ответ сервера */

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
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_parse_string
    (
        Connection *connection,
        Span connectionString
    )
{
    Navigator navigator;
    Span item, parts[2], key, value;

    assert (connection != NULL);
    assert (connection != NULL);

    nav_from_span (&navigator, connectionString);
    while (AM_TRUE) {
        /* Разделителем служит точка с запятой.  */
        item = span_trim (nav_read_to (&navigator, ';'));
        if (span_is_empty (item)) {
            /* Пустые фрагменты игнорируются. */
            break;
        }

        if (span_split_n_by_char (item, parts, 2, '=') != 2) {
            /* Фрагмент должен иметь вид `КЛЮЧ=значение` */
            return AM_FALSE;
        }

        /* Пробелы вокруг ключа и значения игнорируются */
        key = span_trim (parts[0]);
        value = span_trim (parts[1]);
        if (span_is_empty (key) || span_is_empty (value)) {
            /* Пустой ключ или значение не допускается */
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
            connection->workstation = toupper (value.start[0]);
        }
        else {
            /* Неизвестный ключ - ошибка. */
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/**
 * Расформатирование таблицы в RTF.
 *
 * @param connection Активное подключение.
 * @param definition Определение таблицы.
 * @param output Буфер для размещения результата
 * @return Признак успешного заверешния операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_print_table
    (
        Connection *connection,
        TableDefinition *definition,
        Buffer *output
    )
{
    Query query;
    Response response;
    am_bool result = AM_FALSE;

    assert (connection != NULL);
    assert (definition != NULL);

    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    response_init (&response);
    if (!query_create (&query, connection, CBTEXT (PRINT))) {
        return AM_FALSE;
    }

    if (!table_definition_encode (definition, connection, &query)) {
        goto DONE;
    }

    if (!connection_execute (connection, &query, &response)) {
        goto DONE;
    }

    result = table_definition_decode (definition, &response, output);

    DONE:
    query_destroy (&query);
    response_destroy (&response);

    return result;
}

/**
 * Чтение постингов для указанных терминов из поискового словаря.
 *
 * @param connection Активное подключение.
 * @param parameters Параметры постингов.
 * @param postings Массив для заполнения прочитанными постингами.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_read_postings
    (
        Connection *connection,
        const PostingParameters *parameters,
        Array *postings
    )
{
    Query query;
    Response response;
    am_bool result = AM_FALSE;

    assert (connection != NULL);
    assert (parameters != NULL);
    assert (postings != NULL);

    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    response_init (&response);
    if (!query_create (&query, connection, CBTEXT (READ_POSTINGS))) {
        return AM_FALSE;
    }

    if (!posting_parameters_encode (parameters, connection, &query)) {
        goto DONE;
    }

    if (!connection_execute (connection, &query, &response)) {
        goto DONE;
    }

    if (!response_check (&response, -202, -203, -204, 0)) {
        goto DONE;
    }

    result = posting_parse_response (postings, &response);

    DONE:
    query_destroy (&query);
    response_destroy (&response);

    return result;
}

/**
 * Чтение записи с сервера. Запись разделяется на отдельные строки.
 *
 * @param connection Активное соединение.
 * @param mfn MFN записи.
 * @param record Проинициализированная структура.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_read_raw_record
    (
        Connection *connection,
        am_mfn mfn,
        RawRecord *record
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

    response_init (&response);
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

    if (!response_check (&response, -201, -600, -602, -603, 0)) {
        goto DONE;
    }

    if (!raw_record_parse_single (record, &response)) {
        goto DONE;
    }

    result = AM_TRUE;

    DONE:
    query_destroy (&query);
    response_destroy (&response);

    return result;
}

/**
 * Чтение записи с сервера.
 *
 * @param connection Активное соединение.
 * @param mfn MFN записи.
 * @param record Проинициализированная структура.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_read_record
    (
        Connection *connection,
        am_mfn mfn,
        MarcRecord *record
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

    response_init (&response);
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

    if (!response_check (&response, -201, -600, -602, -603, 0)) {
        goto DONE;
    }

    if (!record_parse_single (record, &response)) {
        goto DONE;
    }

    result = AM_TRUE;

    DONE:
    query_destroy (&query);
    response_destroy (&response);

    return result;
}

/**
 * Чтение записи с сервера. Запись никак не раскодируется и возвращается
 * в виде текста.
 *
 * @param connection Активное соединение.
 * @param mfn MFN записи.
 * @param buffer Буфер для размещения записи.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_read_record_text
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

    response_init (&response);
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

    if (!response_check (&response, -201, -600, -602, -603, 0)) {
        goto DONE;
    }

    line = response_remaining_utf_text (&response);
    if (span_is_empty (line)) {
        goto DONE;
    }

    if (!buffer_write (buffer, line.start, span_length (line))) {
        goto DONE;
    }

    result = AM_TRUE;

    DONE:
    query_destroy (&query);
    response_destroy (&response);

    return result;
}

/**
 * Чтение терминов поискового словаря.
 *
 * @param connection Активное подключение.
 * @param parameters Параметры.
 * @param terms Массив для заполнения прочитанными терминами.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_read_terms
    (
        Connection *connection,
        const TermParameters *parameters,
        Array *terms
    )
{
    Query query;
    Response response;
    const am_byte *command;
    am_bool result = AM_FALSE;

    assert (connection != NULL);
    assert (parameters != NULL);
    assert (terms != NULL);

    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    command = CBTEXT (parameters->reverseOrder ? READ_TERMS_REVERSE : READ_TERMS);

    response_init (&response);
    if (!query_create (&query, connection, command)) {
        return AM_FALSE;
    }

    if (!term_parameters_encode (parameters, connection, &query)) {
        goto DONE;
    }

    if (!connection_execute (connection, &query, &response)) {
        goto DONE;
    }

    if (!response_check (&response, -202, -203, -204, 0)) {
        goto DONE;
    }

    result = term_parse_response (terms, &response);

    DONE:
    query_destroy (&query);
    response_destroy (&response);

    return result;
}

/**
 * Чтение текстового файла с сервера.
 *
 * @param connection Активное подключение.
 * @param specification Спецификация.
 * @param buffer Инициализированный буфер для размещения результата.
 * @return Признак успешного выполнения операции.
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

    response_init (&response);
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
 * Реорганизация словаря указанной базы данных.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных. `NULL` означает "текущая база данных".
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_reload_dictionary
    (
        Connection *connection,
        const am_byte *database
    )
{
    am_bool result;
    Response response;

    if (!database) {
        database = B2B (&connection->database);
    }

    result = connection_execute_simple
        (
            connection,
            &response,
            CBTEXT (RELOAD_DICTIONARY),
            1,
            database
        );

    response_destroy (&response);

    return result;
}

/**
 * Реорганизация мастер-файла указанной базы данных.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных. `NULL` означает "текущая база данных".
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_reload_master_file
    (
        Connection *connection,
        const am_byte *database
    )
{
    am_bool result;
    Response response;

    if (!database) {
        database = B2B (&connection->database);
    }

    result = connection_execute_simple
        (
            connection,
            &response,
            CBTEXT (RELOAD_MASTER_FILE),
            1,
            database
        );

    response_destroy (&response);

    return result;
}

/**
 * Реорганизация мастер-файла указанной базы данных.
 *
 * @param connection Активное подключение.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_restart_server
    (
        Connection *connection
    )
{
    am_bool result;
    Response response;

    result = connection_execute_simple
        (
            connection,
            &response,
            CBTEXT (RESTART_SERVER),
            0
        );

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
        const am_byte *expression
    )
{
    am_int32 result = -1;
    SearchParameters parameters;
    Response response;

    assert (connection != NULL);
    assert (expression != NULL);

    if (search_parameters_create (&parameters, expression)) {
        if (connection_search_ex (connection, &parameters, &response)) {
            result = response_read_int32 (&response);
        }

        response_destroy (&response);
    }

    return result;
}

/**
 * Расширенный поиск.
 *
 * @param connection Активное подключение.
 * @param parameters Параметры поиска.
 * @param response Ответ сервера.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_search_ex
    (
        Connection *connection,
        const SearchParameters *parameters,
        Response *response
    )
{
    am_bool result = AM_FALSE;
    Query query;

    assert (connection != NULL);
    assert (parameters != NULL);
    assert (response != NULL);

    response_init (response);
    if (!connection_check (connection)
        || !search_parameters_verify (parameters)
        || !query_create (&query, connection, CBTEXT (SEARCH))) {
        return AM_FALSE;
    }

    if (!search_parameters_encode (parameters, connection, &query)
        || !connection_execute (connection, &query, response)) {
        goto DONE;
    }

    result = response_get_return_code (response) >= 0;

    DONE:
    query_destroy (&query);

    return result;
}

MAGNA_API am_bool MAGNA_CALL connection_search_mfn
    (
        Connection *connection,
        SearchParameters *parameters
    )
{
    am_bool result = AM_FALSE;
    Response response;

    assert (connection != NULL);
    assert (parameters != NULL);

    return result;
}

MAGNA_API am_bool MAGNA_CALL connection_search_simple
    (
        Connection *connection,
        Int32Array *array,
        const am_byte *expression
    )
{
    am_bool result = AM_FALSE;   /* признак успеха */
    SearchParameters parameters; /* параметры поиска */
    Response response;           /* ответ сервера */

    assert (connection != NULL);
    assert (expression != NULL);

    if (search_parameters_create (&parameters, expression)) {
        if (connection_search_ex (connection, &parameters, &response)) {
            (void) response_read_int32 (&response);
            result = found_decode_response_mfn (array, &response);
        }

        response_destroy (&response);
    }

    return result;
}

/**
 * Формирование строки подключения по текущим настройкам.
 *
 * @param connection Подключение (не обязательно активное).
 * @param output Буфер для размещения результата.
 * @return Признак успешного завершения операции.
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

/**
 * Опустошение указанной базы данных.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных. `NULL` означает "текущая база данных".
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_truncate_database
    (
        Connection *connection,
        const am_byte *database
    )
{
    am_bool result;    /* признак успеха */
    Response response; /* ответ сервера */

    if (!database) {
        database = B2B (&connection->database);
    }

    result = connection_execute_simple
        (
            connection,
            &response,
            CBTEXT (EMPTY_DATABASE),
            1,
            database
        );

    response_destroy (&response);

    return result;
}

/**
 * Восстановление записи по ее MFN.
 *
 * @param connection Активное подключение.
 * @param mfn MFN восстанавливаемой записи.
 * @return Признак успешного выполнения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_undelete_record
    (
        Connection *connection,
        am_mfn mfn
    )
{
    assert (connection != NULL);
    assert (mfn > 0u);

    return AM_FALSE;
}

/**
 * Разблокирование указанной базы данных.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных. `NULL` означает "текущая база данных".
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_unlock_database
    (
        Connection *connection,
        const am_byte *database
    )
{
    am_bool result;    /* результат */
    Response response; /* ответ сервера */

    if (!database) {
        database = B2B (&connection->database);
        assert (database != NULL);
    }

    result = connection_execute_simple
        (
            connection,
            &response,
            CBTEXT (UNLOCK_DATABASE),
            1,
            database
        );

    response_destroy (&response);

    return result;
}

/**
 * Разблокирование нескольких записей в указанной базе данных.
 *
 * @param connection Активное подключение.
 * @param database Имя базы данных (`NULL` означает текущую базу).
 * @param records Массив MFN записей.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_unlock_records
    (
        Connection *connection,
        const am_byte *database,
        Int32Array *records
    )
{
    assert (connection != NULL);
    assert (records != NULL);

    if (!database) {
        database = B2B (&connection->database);
        assert (database != NULL);
    }

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Обновление строк серверного INI-файла.
 *
 * @param connection Активное подключение.
 * @param lines Измененные строки (допускается пустой массив).
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_update_ini_file
    (
        Connection *connection,
        const Array *lines
    )
{
    assert (connection != NULL);
    assert (lines != NULL);

    return AM_FALSE;
}

/**
 * Обновление списка пользователей на сервере.
 *
 * @param connection Активное подключение.
 * @param users Массив пользователей.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_update_user_list
    (
        Connection *connection,
        const Array *users
    )
{
    assert (connection != NULL);
    assert (users != NULL);

    return AM_FALSE;
}

/**
 * Сохранение записи на сервере.
 *
 * @param connection Активное подключение.
 * @param record Запись, подлежащая сохранению.
 * @param reparse Перегружать запись согласно полученной от сервера версии?
 * @return Признак успешного выполнения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_write_raw_record
    (
        Connection *connection,
        RawRecord *record,
        am_bool reparse
    )
{
    assert (connection != NULL);
    assert (record != NULL);

    return AM_FALSE;
}

/**
 * Сохранение записи на сервере.
 *
 * @param connection Активное подключение.
 * @param record Запись, подлежащая сохранению.
 * @param reparse Перегружать запись согласно полученной от сервера версии?
 * @return Новый максимальный MFN либо код ошибки.
 */
MAGNA_API am_int32 MAGNA_CALL connection_write_record
    (
        Connection *connection,
        MarcRecord *record,
        am_bool reparse
    )
{
    Query query;             /* клиентский запрос */
    Response response;       /* ответ сервера */
    am_int32 result = -1;    /* результат: новый макс. MFN либо ошибка */
    const am_byte *database; /* имя базы данных */

    assert (connection != NULL);
    assert (record != NULL);

    if (!connection_check (connection)) {
        return result;
    }

    response_init (&response);
    if (!query_create (&query, connection, CBTEXT (UPDATE_RECORD))) {
        return result;
    }

    database = choose_string
        (
            B2B (&record->database),
            B2B (&connection->database),
            NULL
        );
    if (!query_add_ansi (&query, database)
        || !query_add_uint32 (&query, 0)
        || !query_add_uint32 (&query, 1)
        || !record_encode (record, IRBIS_DELIMITER, &query.buffer)) {
        goto DONE;
    }

    if (!connection_execute (connection, &query, &response)) {
        goto DONE;
    }

    if (!response_check (&response, 0)) {
        result = response.returnCode;
        goto DONE;
    }

    result = response.returnCode;

    if (reparse) {
        /* TODO: implement */
    }

    DONE:
    query_destroy (&query);
    response_destroy (&response);

    return AM_FALSE;
}

/**
 * Сохранение текстового файла на сервере.
 *
 * @param connection Активное подключение.
 * @param specification Спецификация (включая текст для сохранения).
 * @return Признак успешного выполнения операции.
 */
MAGNA_API am_bool MAGNA_CALL connection_write_text_file
    (
        Connection *connection,
        const Specification *specification
    )
{
    Query query;
    Response response;
    am_bool result = AM_FALSE;

    assert (connection != NULL);
    assert (specification != NULL);

    if (!connection_check (connection)) {
        return AM_FALSE;
    }

    response_init (&response);
    if (!query_create (&query, connection, CBTEXT (READ_DOCUMENT))) {
        return AM_FALSE;
    }

    if (!query_add_specification (&query, specification)) {
        goto DONE;
    }

    if (!connection_execute (connection, &query, &response)) {
        goto DONE;
    }

    result = response_check (&response, 0);

    DONE:
    query_destroy (&query);
    response_destroy (&response);

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
