/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/tester.h"

TESTER(buffer_create_1)
{
    am_byte data[7] = { 1, 2, 3, 4, 5, 6, 7 };
    Buffer buffer;

    buffer_create (&buffer, data, sizeof (data));
    CHECK (buffer.ptr != NULL);
    CHECK (buffer.position == sizeof (data));
    CHECK (buffer.capacity >= sizeof (data));

    buffer_free (&buffer);
    CHECK (buffer.ptr == NULL);
    CHECK (buffer.capacity == 0);
    CHECK (buffer.position == 0);
}

TESTER(buffer_create_2)
{
    Buffer buffer;

    buffer_create (&buffer, NULL, 0);
    CHECK (buffer.ptr == NULL);
    CHECK (buffer.position == 0);
    CHECK (buffer.capacity == 0);

    buffer_free (&buffer);
    CHECK (buffer.ptr == NULL);
    CHECK (buffer.capacity == 0);
    CHECK (buffer.position == 0);
}

TESTER(buffer_static_1)
{
    am_byte data[7] = { 1, 2, 3, 4, 5, 6, 7 };
    Buffer buffer;

    buffer_static (&buffer, data, sizeof (data));
    CHECK (buffer.ptr == data);
    CHECK (buffer.position == 0);
    CHECK (buffer.capacity == sizeof (data));
}

TESTER(buffer_static_2)
{
    Buffer buffer;

    buffer_static (&buffer, NULL, 0);
    CHECK (buffer.ptr == NULL);
    CHECK (buffer.position == 0);
    CHECK (buffer.capacity == 0);
}

