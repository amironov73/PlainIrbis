// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"
#include "magna/irbis.h"

TESTER(subfield_ini1_1)
{
    SubField subfield = SUBFIELD_INIT;

    CHECK (subfield.code == 0);
    CHECK (buffer_is_empty (&subfield.value));
    CHECK (subfield_is_empty (&subfield));

    subfield_destroy (&subfield);
}

TESTER(subfield_init_2)
{
    SubField subfield;
    Span text = TEXT_SPAN ("Hello, world");

    subfield_init (&subfield, 'a', text);

    CHECK (subfield.code == 'a');
    CHECK (buffer_compare_span (&subfield.value, text) == 0);
    CHECK (!subfield_is_empty (&subfield));

    /* subfield_destroy (&subfield); не надо */
}

TESTER(subfield_create_1)
{
    SubField subfield;
    Span text = TEXT_SPAN ("Hello, world");

    subfield_create (&subfield, 'a', text);

    CHECK (subfield.code == 'a');
    CHECK (buffer_compare_span (&subfield.value, text) == 0);

    subfield_destroy (&subfield);
}

TESTER(subfield_assign_1)
{
    SubField subfield = SUBFIELD_INIT;
    Span value = TEXT_SPAN ("Hello, world");

    subfield.code = 'a';
    CHECK (subfield_assign (&subfield, value));
    CHECK (subfield.code == 'a');
    CHECK (buffer_compare_span (&subfield.value, value) == 0);
    CHECK (!subfield_is_empty (&subfield));

    subfield_destroy (&subfield);
}

TESTER(subfield_clone_1)
{
    SubField sf1, sf2 = SUBFIELD_INIT;

    CHECK (subfield_create (&sf1, 'a', TEXT_SPAN ("Hello, world")));
    CHECK (subfield_clone (&sf2, &sf1));
    CHECK (sf1.code == sf2.code);
    CHECK (buffer_compare (&sf1.value, &sf2.value) == 0);

    subfield_destroy (&sf1);
    subfield_destroy (&sf2);
}

TESTER(subfield_is_empty_1)
{
    SubField subfield = SUBFIELD_INIT;

    subfield.code = 'a';
    CHECK (subfield_is_empty (&subfield));

    subfield_destroy (&subfield);
}

TESTER(subfield_is_empty_2)
{
    SubField subfield = SUBFIELD_INIT;

    buffer_static (&subfield.value, CBTEXT ("Hello"), 5);
    CHECK (subfield_is_empty (&subfield));

    /* subfield_destroy (&subfield); не надо */
}

TESTER(subfield_decode_1)
{
    SubField subfield = SUBFIELD_INIT;

    CHECK (subfield_decode (&subfield, span_null()));
    CHECK (subfield_is_empty (&subfield));
    CHECK (subfield.code == 0);
    CHECK (buffer_is_empty (&subfield.value));

    subfield_destroy (&subfield);

}

TESTER(subfield_decode_2)
{
    SubField subfield = SUBFIELD_INIT;

    CHECK (subfield_decode (&subfield, TEXT_SPAN ("a")));
    CHECK (subfield_is_empty (&subfield));
    CHECK (subfield.code == 'a');
    CHECK (buffer_is_empty (&subfield.value));

    subfield_destroy (&subfield);
}

TESTER(subfield_decode_3)
{
    SubField subfield = SUBFIELD_INIT;

    CHECK (subfield_decode (&subfield, TEXT_SPAN ("aHello")));
    CHECK (!subfield_is_empty (&subfield));
    CHECK (subfield.code == 'a');
    CHECK (!buffer_is_empty (&subfield.value));
    CHECK (buffer_compare_text (&subfield.value, CBTEXT ("Hello")) == 0);

    subfield_destroy (&subfield);
}

TESTER(subfield_to_string_1)
{
    SubField subfield = SUBFIELD_INIT;
    Buffer output = BUFFER_INIT;
    Span expected = span_init (BTEXT ("^\0"), 2);

    CHECK (subfield_to_string (&subfield, &output));
    CHECK (buffer_compare_span (&output, expected) == 0);

    subfield_destroy (&subfield);
    buffer_destroy (&output);
}

TESTER(subfield_to_string_2)
{
    SubField subfield = SUBFIELD_INIT;
    Buffer output = BUFFER_INIT;
    Span expected = TEXT_SPAN ("^a");

    subfield.code = 'a';
    CHECK (subfield_to_string (&subfield, &output));
    CHECK (buffer_compare_span (&output, expected) == 0);

    subfield_destroy (&subfield);
    buffer_destroy (&output);
}

TESTER(subfield_to_string_3)
{
    SubField subfield = SUBFIELD_INIT;
    Buffer output = BUFFER_INIT;
    Span expected = TEXT_SPAN ("^aHello, world");

    subfield_init (&subfield, 'a', TEXT_SPAN ("Hello, world"));
    CHECK (subfield_to_string (&subfield, &output));
    CHECK (buffer_compare_span (&output, expected) == 0);

    /* subfield_destroy (&subfield); не надо */
    buffer_destroy (&output);
}

TESTER(subfield_code_is_valid_1)
{
    CHECK (!subfield_code_is_valid (0));
    CHECK (!subfield_code_is_valid (' '));
    CHECK (subfield_code_is_valid ('A'));
    CHECK (subfield_code_is_valid ('a'));
    CHECK (subfield_code_is_valid ('Z'));
    CHECK (subfield_code_is_valid ('z'));
    CHECK (subfield_code_is_valid ('0'));
    CHECK (subfield_code_is_valid ('9'));
    CHECK (subfield_code_is_valid ('!'));
    CHECK (subfield_code_is_valid ('~'));
}

TESTER(subfield_normalize_code_1)
{
    CHECK (subfield_normalize_code (0) == 0);
    CHECK (subfield_normalize_code (' ') == ' ');
    CHECK (subfield_normalize_code ('A') == 'a');
    CHECK (subfield_normalize_code ('a') == 'a');
    CHECK (subfield_normalize_code ('Z') == 'z');
    CHECK (subfield_normalize_code ('z') == 'z');
    CHECK (subfield_normalize_code ('0') == '0');
    CHECK (subfield_normalize_code ('9') == '9');
    CHECK (subfield_normalize_code ('!') == '!');
    CHECK (subfield_normalize_code ('~') == '~');
}

TESTER(subfield_verify_1)
{
    SubField subfield = SUBFIELD_INIT;

    CHECK (!subfield_verify (&subfield));

    subfield_destroy (&subfield);
}

TESTER(subfield_verify_2)
{
    SubField subfield = SUBFIELD_INIT;

    subfield.code = 'a';
    CHECK (!subfield_verify (&subfield));

    subfield_destroy (&subfield);
}

TESTER(subfield_verify_3)
{
    SubField subfield = SUBFIELD_INIT;

    subfield_init (&subfield, ' ', span_null());
    CHECK (!subfield_verify (&subfield));

    subfield_destroy (&subfield);
}

TESTER(subfield_verify_4)
{
    SubField subfield = SUBFIELD_INIT;

    subfield_init (&subfield, 'a', span_null());
    CHECK (!subfield_verify (&subfield));

    subfield_destroy (&subfield);
}

TESTER(subfield_verify_5)
{
    SubField subfield = SUBFIELD_INIT;

    subfield_init (&subfield, 'a', TEXT_SPAN ("Hello"));
    CHECK (subfield_verify (&subfield));

    /* subfield_destroy (&subfield); не надо */
}
