// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(mem_avail_physical_1)
{
    am_size_t rc = mem_avail_physical();

    CHECK (rc != 0);
}

TESTER(mem_avail_virtual_1)
{
    am_size_t rc = mem_avail_virtual();

    CHECK (rc != 0);
}

TESTER(mem_total_installed_1)
{
    am_size_t rc = mem_total_installed();

    CHECK (rc != 0);
}

TESTER(mem_total_virtual_1)
{
    am_size_t rc = mem_total_virtual();

    CHECK (rc != 0);
}
