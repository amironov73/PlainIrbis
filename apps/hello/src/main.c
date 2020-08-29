/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/*=========================================================*/

int main (int argc, char **argv)
{
/*    char buffer [128];
    int bufSize = sizeof (buffer);
    void *space;*/
    Connection connection;
    am_int32 maxMfn;

    (void) argc;
    (void) argv;

    printf ("Library version: %d\n", magna_version());
    connection_init (&connection);
    buffer_from_text (&connection.host, "localhost");
    connection.port = 6666;
    buffer_from_text (&connection.username, "librarian");
    buffer_from_text (&connection.password, "secret");
    buffer_from_text (&connection.database, "IBIS");

    if (!connection_connect (&connection)) {
        fputs ("Connection failed", stderr);
        connection_free (&connection);
        return 1;
    }

    maxMfn = connection_get_max_mfn (&connection, NULL);
    printf ("Max MFN=%d\n", maxMfn);

    connection_disconnect (&connection);

    connection_free (&connection);

/*
#ifdef _M_IX86

    _mkdir ("test");

    irbis64_dll_version (buffer, bufSize);
    printf ("Library version: %s\n", buffer);

    space = irbis64_dll_init();
    printf ("Space: %p\n", space);
    irbis64_dll_init_new_db ("test\\db");
    irbis64_dll_close (space);

#endif
*/

    return 0;
}

/*=========================================================*/
