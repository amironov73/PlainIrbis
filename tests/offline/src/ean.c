/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/tester.h"
#include "magna/irbis.h"

TESTER(ean13_compute_check_digit_1)
{
    Span text = span_from_text ((char*)"4600051000057");

    CHECK (ean13_compute_check_digit (text) == '7');
}

TESTER(ean13_check_control_digit_1)
{
    Span text = span_from_text ((char*)"4600051000057");

    CHECK (ean13_check_control_digit (text));
}

TESTER(ean8_compute_check_digit_1)
{
    Span text = span_from_text ((char*)"46009333");

    CHECK (ean8_compute_check_digit (text) == '3');
}

TESTER(ean8_check_control_digit_1)
{
    Span text = span_from_text ((char*)"46009333");

    CHECK (ean8_compute_check_digit (text));
}
