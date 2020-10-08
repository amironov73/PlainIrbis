// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"
#include "magna/irbis.h"

TESTER(subfield_init_1)
{
    SubField subfield;
    Span text = TEXT_SPAN ("Hello, world");

    subfield_init (&subfield, 'a', text);

    CHECK (subfield.code == 'a');
    CHECK (buffer_compare_span (&subfield.value, text) == 0);

    /* subfield_free (&subfield); Не надо */
}

TESTER(subfield_create_1)
{
    SubField subfield;
    Span text = TEXT_SPAN ("Hello, world");

    subfield_create (&subfield, 'a', text);

    CHECK (subfield.code == 'a');
    CHECK (buffer_compare_span (&subfield.value, text) == 0);

    subfield_free (&subfield);
}
