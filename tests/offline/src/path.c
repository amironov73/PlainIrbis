// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(path_get_current_directory_1)
{
    Buffer buffer = BUFFER_INIT;
    am_bool rc = path_get_current_directory (&buffer);

    CHECK (rc);
    CHECK (buffer_position (&buffer) != 0);

    buffer_destroy(&buffer);
}

TESTER(path_get_executable_1)
{
    Buffer buffer = BUFFER_INIT;
    am_bool rc = path_get_executable (&buffer);

    CHECK (rc);
    CHECK (buffer_position (&buffer) != 0);

    buffer_destroy(&buffer);
}
