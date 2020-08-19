/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/*=========================================================*/

int main (int argc, char **argv)
{

    (void)argc;
    (void)argv;

#ifdef MAGNA_WINDOWS

    capsoff();

#endif

    return 0;
}

/*=========================================================*/

