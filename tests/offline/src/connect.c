// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"
#include "magna/irbis.h"

TESTER(connection_create_1)
{
    Connection connection;

    CHECK (connection_create (&connection));
    CHECK (!buffer_is_empty (&connection.host));
    CHECK (!buffer_is_empty (&connection.database));
    CHECK (buffer_is_empty (&connection.username));
    CHECK (buffer_is_empty (&connection.password));
    CHECK (connection.port == 6666);
    CHECK (connection.workstation == CATALOGER);
    CHECK (connection.connected == AM_FALSE);
    CHECK (connection.lastError == 0);

    connection_destroy (&connection);
}

TESTER(connection_set_host_1)
{
    Connection connection;
    const am_byte *value = CBTEXT ("Some");

    CHECK (connection_create (&connection));
    CHECK (connection_set_database (&connection, value));
    CHECK (buffer_compare_text (&connection.database, CBTEXT (value)) == 0);

    connection_destroy (&connection);
}

TESTER(connection_set_username_1)
{
    Connection connection;
    const am_byte *value = CBTEXT ("Some");

    CHECK (connection_create (&connection));
    CHECK (connection_set_username (&connection, value));
    CHECK (buffer_compare_text (&connection.username, CBTEXT (value)) == 0);

    connection_destroy (&connection);
}

TESTER(connection_set_password_1)
{
    Connection connection;
    const am_byte *value = CBTEXT ("Some");

    CHECK (connection_create (&connection));
    CHECK (connection_set_password (&connection, value));
    CHECK (buffer_compare_text (&connection.password, CBTEXT (value)) == 0);

    connection_destroy (&connection);
}

TESTER(connection_set_database_1)
{
    Connection connection;
    const am_byte *value = CBTEXT ("Some");

    CHECK (connection_create (&connection));
    CHECK (connection_set_database (&connection, value));
    CHECK (buffer_compare_text (&connection.database, CBTEXT (value)) == 0);

    connection_destroy (&connection);
}

TESTER(connection_check_1)
{
    Connection connection;

    CHECK (connection_create (&connection));
    CHECK (!connection_check (&connection));
    CHECK (connection.lastError == 100500);

    connection_destroy (&connection);
}

TESTER(connection_check_2)
{
    Connection connection;

    CHECK (connection_create (&connection));
    connection.connected = AM_TRUE;
    CHECK (connection_check (&connection));
    CHECK (connection.lastError == 0);
    connection.connected = AM_FALSE;

    connection_destroy (&connection);
}

TESTER(connection_disconnect_1)
{
    Connection connection;

    CHECK (connection_create (&connection));
    CHECK (connection_disconnect (&connection));

    connection_destroy (&connection);
}

TESTER(irbis_disconnect_1)
{
    Connection connection;

    CHECK (connection_create (&connection));
    CHECK (irbis_disconnect (&connection));

    connection_destroy (&connection);
}

TESTER(connection_parse_string_1)
{
    Connection connection;

    CHECK (connection_create (&connection));
    CHECK (connection_parse_string (&connection, TEXT_SPAN ("")));
    CHECK (buffer_is_empty (&connection.username));
    CHECK (buffer_is_empty (&connection.password));

    connection_destroy (&connection);
}

TESTER(connection_parse_string_2)
{
    Connection connection;

    CHECK (connection_create (&connection));
    CHECK (connection_parse_string (&connection,
        TEXT_SPAN ("user = librarian; pwd = secret ; ")));
    CHECK (buffer_compare_text (&connection.username, CBTEXT ("librarian")) == 0);
    CHECK (buffer_compare_text (&connection.password, CBTEXT ("secret")) == 0);

    connection_destroy (&connection);
}

TESTER(connection_parse_string_3)
{
    Connection connection;

    CHECK (connection_create (&connection));
    CHECK (connection_parse_string (&connection,
        TEXT_SPAN ("address = 123.0.1.2 ; port = 5555 ")));
    CHECK (buffer_compare_text (&connection.host, CBTEXT ("123.0.1.2")) == 0);
    CHECK (connection.port == 5555);

    connection_destroy (&connection);
}

TESTER(connection_parse_string_4)
{
    Connection connection;

    CHECK (connection_create (&connection));
    CHECK (connection_parse_string (&connection,
        TEXT_SPAN ("catalog = ISTU ; workstation = a ")));
    CHECK (buffer_compare_text (&connection.database, CBTEXT ("ISTU")) == 0);
    CHECK (connection.workstation == 'A');

    connection_destroy (&connection);
}

TESTER(connection_parse_string_5)
{
    Connection connection;

    CHECK (connection_create (&connection));
    CHECK (!connection_parse_string (&connection,
        TEXT_SPAN ("catalog = ISTU ; unknown= a ")));

    connection_destroy (&connection);
}

TESTER(connection_to_string_1)
{
    Connection connection;
    Buffer output = BUFFER_INIT;

    CHECK (connection_create (&connection));
    CHECK (connection_to_string (&connection, &output));
    CHECK (buffer_compare_text (&output,
        CBTEXT ("host=127.0.0.1;port=6666;username=;password=;database=IBIS;workstation=C;")) == 0);

    connection_destroy (&connection);
    buffer_destroy (&output);
}

TESTER(connection_to_string_2)
{
    Connection connection;
    Buffer output = BUFFER_INIT;

    CHECK (connection_create (&connection));
    CHECK (connection_set_username (&connection, CBTEXT ("ninja")));
    CHECK (connection_set_password (&connection, CBTEXT ("invisible")));
    CHECK (connection_to_string (&connection, &output));
    CHECK (buffer_compare_text (&output,
        CBTEXT ("host=127.0.0.1;port=6666;username=ninja;password=invisible;database=IBIS;workstation=C;")) == 0);

    connection_destroy (&connection);
    buffer_destroy (&output);
}
