// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"

extern "C"
{
    #include "magna/core.h"
};

TEST_CASE("span_init_1", "[magna]")
{
    Span span = span_init (NULL, 0);

    CHECK (span.ptr == NULL);
    CHECK (span.len == 0);
}

TEST_CASE("span_init_2", "[magna]")
{
    am_byte data[] = { 1, 2, 3 };
    Span span = span_init (data, sizeof (data));

    CHECK (span.ptr == data);
    CHECK (span.len == sizeof (data));
}

