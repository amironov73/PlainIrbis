/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#define TESTER_MAIN
#include "magna/tester.h"

#include "offline.h"

TESTER(get_magna_version)
{
    int version = magna_version();
    CHECK (version == 0x0001);
}

TESTER(magna_on_windows)
{
    am_bool rc = magna_on_windows();

#ifdef MAGNA_WINDOWS
    CHECK (rc);
#else
    CHECK (!rc);
#endif
}

/*=========================================================*/

int main (int argc, const char **argv)
{
    int result = tester_main (argc, argv);

    return result;
}
