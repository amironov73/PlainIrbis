// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"
#include "magna/irbis.h"

TESTER(upc12_compute_check_digit_1)
{
    const am_byte *text = "041689300494";
    Span span = span_from_text (text);

    CHECK (upc12_compute_check_digit (span) == '4');
}

TESTER(upc12_check_control_digit_1)
{
    const am_byte *text = "041689300494";
    Span span = span_from_text (text);

    CHECK (upc12_check_control_digit (span));
}
