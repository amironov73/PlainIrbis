// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/irbis.h"

/*=========================================================*/

int main (int argc, char **argv)
{
/*    char buffer [128];
    int bufSize = sizeof (buffer);
    void *space;*/
    Connection connection;
    am_mfn maxMfn;

    (void) argc;
    (void) argv;

    srand (time (NULL));

    printf ("Library version: %d\n", magna_get_version());
    connection_create       (&connection);
    connection_set_host     (&connection, "localhost");
    connection_set_username (&connection, "librarian");
    connection_set_password (&connection, "secret");
    connection_set_database (&connection, "IBIS");
    connection.workstation = CATALOGER;
    connection.port = 6666;

    if (!connection_connect (&connection)) {
        fputs ("Connection failed", stderr);
        connection_destroy (&connection);
        return 1;
    }

    maxMfn = connection_get_max_mfn (&connection, NULL);
    printf ("Max MFN=%u\n", maxMfn);

    connection_disconnect (&connection);
    connection_destroy (&connection);

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
