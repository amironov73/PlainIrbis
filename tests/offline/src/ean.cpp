// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"

extern "C"
{
    #include "magna/irbis.h"
};

TEST_CASE("Ean13_computeCheckDigit_1", "[ean]")
{
    Span text = span_from_text ((char*)"4600051000057");

    CHECK (ean13_compute_check_digit (text) == '7');
}

TEST_CASE("Ean13_checkControlDigit_1", "[ean]")
{
    Span text = span_from_text ((char*)"4600051000057");

    CHECK (ean13_check_control_digit (text));
}

TEST_CASE("Ean8_computeCheckDigit_1", "[ean]")
{
    Span text = span_from_text ((char*)"46009333");

    CHECK (ean8_compute_check_digit (text) == '3');
}

TEST_CASE("Ean8_checkControlDigit_1", "[ean]")
{
    Span text = span_from_text ((char*)"46009333");

    CHECK (ean8_compute_check_digit (text));
}
