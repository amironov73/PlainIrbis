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
    Specification spec;
    Buffer fileContent = BUFFER_INIT;
    Buffer rawRecord = BUFFER_INIT;
    Buffer formatted = BUFFER_INIT;
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

    if (!irbis_connect (&connection)) {
        fputs ("Connection failed\n", stderr);
        connection_destroy (&connection);
        return 1;
    }

    maxMfn = connection_get_max_mfn (&connection, NULL);
    printf ("Max MFN=%u\n", maxMfn);

    connection_no_operation (&connection);
    printf ("NOP\n");

    printf ("\n\nbrief.pft:\n\n");
    spec_init (&spec, PATH_MASTER, "IBIS", "brief.pft");
    connection_read_text_file (&connection, &spec, &fileContent);
    buffer_to_console (&fileContent);
    buffer_destroy (&fileContent);

    printf ("\n\nMFN=1:\n\n");
    connection_read_raw_record (&connection, 1, &rawRecord);
    buffer_to_console (&rawRecord);
    buffer_destroy (&rawRecord);

    printf ("\n\nFormatted:\n\n");
    connection_format_mfn (&connection, "@brief", 1, &formatted);
    buffer_to_console (&formatted);
    buffer_destroy (&formatted);

    irbis_disconnect (&connection);
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
