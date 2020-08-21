/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#define TESTER_MAIN
#include "magna/tester.h"

/*=========================================================*/

TESTER (first_test)
{
    int x = 1, y = 2;

    CHECK (x == y);
}

TESTER (second_test)
{
    int x = 1, y = x;

    CHECK (x == y);
}

TESTER (third_test)
{
    int x = 1, y = x;

    CHECK (x == y);
}

/*=========================================================*/

int main (int argc, const char **argv)
{
    int result = tester_main (argc, argv);

    return result;
}
