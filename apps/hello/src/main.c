// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/irbis.h"

/*=========================================================*/

#define MAKE_STR_IMPL(__x) #__x
#define MAKE_STR(__x) MAKE_STR_IMPL(__x)

int main (int argc, char **argv)
{
/*    char buffer [128];
    int bufSize = sizeof (buffer);
    void *space;*/
    Connection connection;
    Specification spec;
    Buffer fileContent = BUFFER_INIT;
    Buffer formatted = BUFFER_INIT;
    MarcRecord record;
    am_mfn maxMfn;
    am_int32 count;
    SearchParameters parameters;
    Int32Array found;

    (void) argc;
    (void) argv;

#ifdef HAVE_CONFIG_H
    printf ("Compiler: %s\n", MAKE_STR (CMAKE_C_COMPILER_ID));
    printf ("System: %s\n", MAKE_STR (CMAKE_SYSTEM_NAME));
#endif

    printf ("Library version: %s\n", magna_get_version_string ());
    if (!connection_create (&connection)) {
        fputs ("Can't create connection\n", stderr);
        return 1;
    }

    /* Настройка параметров подключения. */
    connection_set_host     (&connection, CBTEXT ("localhost"));
    connection_set_username (&connection, CBTEXT ("librarian"));
    connection_set_password (&connection, CBTEXT ("secret"));
    connection_set_database (&connection, CBTEXT ("IBIS"));
    connection.workstation = CATALOGER;
    connection.port = 6666;

    if (!irbis_connect (&connection)) {
        fputs ("Connection failed\n", stderr);
        connection_destroy (&connection);
        return 1;
    }

    /* Получение максимального MFN. */
    maxMfn = connection_get_max_mfn (&connection, NULL);
    printf ("Max MFN=%u\n", maxMfn);

    /* Пустая операция NOP. */
    connection_no_operation (&connection);
    printf ("NOP\n");

    /* Подсчет количества записей, удовлетворяющих поисковому запросу. */
    count = connection_search_count (&connection, CBTEXT ("K=БЕТОН$"));
    printf ("Found record count=%d\n", count);

    /* Поиск записей. */
    int32_array_create (&found, 10);
    if (!search_parameters_create (&parameters, CBTEXT ("K=БЕТОН$"))) {
        fputs ("Can't create search parameters\n", stderr);
    }
    else {
        if (!connection_search_simple (&connection, &found, CBTEXT ("K=БЕТОН$"))) {
            fputs ("Error during search\n", stderr);
        }
        else {
            int32_array_to_console (&found, CBTEXT (", "));
            puts ("\n");
        }

        search_parameters_destroy (&parameters);
    }
    int32_array_destroy (&found);

    /* Чтение текстового файла с сервера. */
    if (spec_create (&spec, PATH_MASTER, CBTEXT ("IBIS"), CBTEXT ("brief.pft"))) {
        if (connection_read_text_file (&connection, &spec, &fileContent)) {
            printf ("\n\nbrief.pft:\n\n");
            buffer_to_console(&fileContent);
            printf ("\n\n");
        }

        buffer_destroy (&fileContent);
        spec_destroy (&spec);
    }

    record_init (&record);
    if (connection_read_record (&connection, 1, &record)) {
        printf ("\n\nMFN=1:\n\n");
        record_to_console (&record);
        printf ("\n\n");
    }
    record_destroy (&record);

    if (connection_format_mfn (&connection, CBTEXT ("@brief"), 1, &formatted)) {
        printf ("\n\nFormatted:\n\n");
        buffer_to_console (&formatted);
    }
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
