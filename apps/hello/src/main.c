/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/*=========================================================*/

int main (int argc, char **argv)
{
    char buffer [128];
    int bufSize = sizeof (buffer);
    void *space;

    (void) argc;
    (void) argv;

    printf ("Library version: %d\n", magna_version());

#ifdef _M_IX86

    _mkdir ("test");

    irbis64_dll_version (buffer, bufSize);
    printf ("Library version: %s\n", buffer);

    space = irbis64_dll_init();
    printf ("Space: %p\n", space);
    irbis64_dll_init_new_db ("test\\db");
    irbis64_dll_close (space);

#endif

    return 0;
}

/*=========================================================*/
