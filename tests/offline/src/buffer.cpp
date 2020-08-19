// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"

extern "C"
{
    #include "magna/core.h"
};

TEST_CASE("buffer_create_1", "[magna]")
{
    am_byte data[7] = { 1, 2, 3, 4, 5, 6, 7 };
    Buffer buffer;

    buffer_create (&buffer, data, sizeof (data));
    REQUIRE (buffer.ptr != nullptr);
    REQUIRE (buffer.position == sizeof (data));
    REQUIRE (buffer.capacity >= sizeof (data));

    buffer_free (&buffer);
    REQUIRE (buffer.ptr == NULL);
    REQUIRE (buffer.capacity == 0);
    REQUIRE (buffer.position == 0);
}

TEST_CASE("buffer_create_2", "[magna]")
{
    Buffer buffer;

    buffer_create (&buffer, NULL, 0);
    REQUIRE (buffer.ptr == nullptr);
    REQUIRE (buffer.position == 0);
    REQUIRE (buffer.capacity == 0);

    buffer_free (&buffer);
    REQUIRE (buffer.ptr == NULL);
    REQUIRE (buffer.capacity == 0);
    REQUIRE (buffer.position == 0);
}

TEST_CASE("buffer_static_1", "[magna]")
{
    am_byte data[7] = { 1, 2, 3, 4, 5, 6, 7 };
    Buffer buffer;

    buffer_static (&buffer, data, sizeof (data));
    REQUIRE (buffer.ptr == data);
    REQUIRE (buffer.position == 0);
    REQUIRE (buffer.capacity == sizeof (data));
}

TEST_CASE("buffer_static_2", "[magna]")
{
    Buffer buffer;

    buffer_static (&buffer, NULL, 0);
    REQUIRE (buffer.ptr == NULL);
    REQUIRE (buffer.position == 0);
    REQUIRE (buffer.capacity == 0);
}

