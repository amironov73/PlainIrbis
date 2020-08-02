// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C"
{
    #include "magna/irbis.h"
};

#include "offline.h"

TEST_CASE("get_magna_version", "[magna]")
{
    int version = magna_version();
    REQUIRE(version == 0);
}
