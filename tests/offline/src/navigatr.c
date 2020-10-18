// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(nav_init_1)
{
    am_byte data[] = { 1, 2, 3 };
    TextNavigator nav;

    CHECK (nav_init (&nav, data, sizeof (data)) == &nav);
    CHECK (nav.data == data);
    CHECK (nav.length == sizeof (data));
    CHECK (nav.position == 0);
    CHECK (nav.line = 1);
    CHECK (nav.column = 1);
}

TESTER(nav_from_span_1)
{
    am_byte data[] = { 1, 2, 3 };
    Span span = span_init (data, sizeof (data));
    TextNavigator nav;

    CHECK (nav_from_span (&nav, span) == &nav);
    CHECK (nav.data == data);
    CHECK (nav.length == sizeof (data));
    CHECK (nav.position == 0);
    CHECK (nav.line = 1);
    CHECK (nav.column = 1);
}

TESTER(nav_from_buffer_1)
{
    am_byte data[] = { 1, 2, 3 };
    Buffer buffer;
    TextNavigator nav;

    buffer_static (&buffer, data, sizeof (data));
    buffer.position = sizeof (data);

    CHECK (nav_from_buffer (&nav, &buffer) == &nav);
    CHECK (nav.data == data);
    CHECK (nav.length == sizeof (data));
    CHECK (nav.position == 0);
    CHECK (nav.line = 1);
    CHECK (nav.column = 1);
}

TESTER(nav_from_text_1)
{
    am_byte *text = BTEXT ("Hello, world");
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav.data == text);
    CHECK (nav.length == strlen (CCTEXT (text)));
    CHECK (nav.position == 0);
    CHECK (nav.line = 1);
    CHECK (nav.column = 1);
}

TESTER(nav_end_1)
{
    am_byte *text = BTEXT ("Hello, world");
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (*nav_end (&nav) == 0);
}

TESTER(nav_current_1)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (*nav_current (&nav) == 'H');
}

TESTER(nav_eot_1)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_eot (&nav));
}

TESTER(nav_eot_2)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (!nav_eot (&nav));

    nav_move (&nav, 100);
    CHECK (nav_eot (&nav));
}

TESTER(nav_at_1)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_at (&nav, 0) == 'H');
    CHECK (nav_at (&nav, 1) == 'e');
    CHECK (nav_at (&nav, 100) == NAV_EOT);
}

TESTER(nav_front_1)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_front (&nav) == NAV_EOT);
}

TESTER(nav_front_2)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_front (&nav) == 'H');
}

TESTER(nav_back_1)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_back (&nav) == NAV_EOT);
}

TESTER(nav_back_2)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_back (&nav) == 'd');
}

TESTER(nav_look_ahead_1)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_look_ahead (&nav, 1) == NAV_EOT);
}

TESTER(nav_look_ahead_2)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_look_ahead (&nav, 1) == 'e');
    CHECK (nav_look_ahead (&nav, 2) == 'l');
}

TESTER(nav_look_behind_1)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_look_behind (&nav, 1) == NAV_EOT);
}

TESTER(nav_look_behind_2)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_read (&nav) == 'H');
    CHECK (nav_look_behind (&nav, 1) == 'H');
    CHECK (nav_look_behind (&nav, 2) == NAV_EOT);
}

TESTER(nav_move_1)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_move (&nav, 3) == &nav);
    CHECK (nav_read (&nav) == 'l');
}

TESTER(nav_peek_1)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_peek (&nav) == 'H');
    CHECK (nav_peek (&nav) == 'H');
}

TESTER(nav_peek_2)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_peek (&nav) == NAV_EOT);
    CHECK (nav_peek (&nav) == NAV_EOT);
}

TESTER(nav_peek_no_crlf_1)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_peek_no_crlf (&nav) == 'H');
    CHECK (nav_peek_no_crlf (&nav) == 'H');
}

TESTER(nav_peek_no_crlf_2)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_peek_no_crlf (&nav) == NAV_EOT);
    CHECK (nav_peek_no_crlf (&nav) == NAV_EOT);
}

TESTER(nav_peek_no_crlf_3)
{
    char *text = "\r\nHello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_peek_no_crlf (&nav) == 'H');
    CHECK (nav_peek_no_crlf (&nav) == 'H');
}

TESTER(nav_read_1)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav.column == 1);
    CHECK (nav.line == 1);
    CHECK (nav_read (&nav) == 'H');
    CHECK (nav.column == 2);
    CHECK (nav_read (&nav) == 'e');
    CHECK (nav.column == 3);
    CHECK (nav_read (&nav) == 'l');
    CHECK (nav.column == 4);
    CHECK (nav_read (&nav) == 'l');
    CHECK (nav.column == 5);
    CHECK (nav_read (&nav) == 'o');
    CHECK (nav.column == 6);
    CHECK (nav.line == 1);
}

TESTER(nav_read_2)
{
    char *text = "\r\nHello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav.column == 1);
    CHECK (nav.line == 1);
    CHECK (nav_read (&nav) == '\r');
    CHECK (nav.column == 2);
    CHECK (nav_read (&nav) == '\n');
    CHECK (nav.column == 0);
    CHECK (nav.line == 2);
    CHECK (nav_read (&nav) == 'H');
    CHECK (nav.column == 1);
    CHECK (nav_read (&nav) == 'e');
    CHECK (nav.column == 2);
    CHECK (nav_read (&nav) == 'l');
    CHECK (nav.column == 3);
    CHECK (nav.line == 2);
}

TESTER(nav_read_3)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_read (&nav) == NAV_EOT);
    CHECK (nav.column == 1);
    CHECK (nav.line == 1);
    CHECK (nav_read (&nav) == NAV_EOT);
    CHECK (nav.column == 1);
    CHECK (nav.line == 1);
}

TESTER(nav_read_no_crlf_1)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav.column == 1);
    CHECK (nav.line == 1);
    CHECK (nav_read_no_crlf (&nav) == 'H');
    CHECK (nav.column == 2);
    CHECK (nav_read_no_crlf (&nav) == 'e');
    CHECK (nav.column == 3);
    CHECK (nav_read_no_crlf (&nav) == 'l');
    CHECK (nav.column == 4);
    CHECK (nav_read_no_crlf (&nav) == 'l');
    CHECK (nav.column == 5);
    CHECK (nav_read_no_crlf (&nav) == 'o');
    CHECK (nav.column == 6);
    CHECK (nav.line == 1);
}

TESTER(nav_read_no_crlf_2)
{
    char *text = "\r\nHello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav.column == 1);
    CHECK (nav.line == 1);
    CHECK (nav_read_no_crlf (&nav) == 'H');
    CHECK (nav.line == 2);
    CHECK (nav.column == 1);
    CHECK (nav_read_no_crlf (&nav) == 'e');
    CHECK (nav.column == 2);
    CHECK (nav_read_no_crlf (&nav) == 'l');
    CHECK (nav.column == 3);
    CHECK (nav_read_no_crlf (&nav) == 'l');
    CHECK (nav.column == 4);
    CHECK (nav_read_no_crlf (&nav) == 'o');
    CHECK (nav.column == 5);
    CHECK (nav.line == 2);
}

TESTER(nav_read_no_crlf_3)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_read_no_crlf (&nav) == NAV_EOT);
    CHECK (nav.column == 1);
    CHECK (nav.line == 1);
    CHECK (nav_read_no_crlf (&nav) == NAV_EOT);
    CHECK (nav.column == 1);
    CHECK (nav.line == 1);
}

TESTER(nav_peek_string_1)
{
    am_byte *text = BTEXT ("Hello, world");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_peek_string (&nav, 5);
    CHECK (span.start == text);
    CHECK (span_length (span) == 5);

    span = nav_peek_string (&nav, 5);
    CHECK (span.start == text);
    CHECK (span_length (span) == 5);
}


TESTER(nav_peek_string_2)
{
    am_byte *text = BTEXT ("He\r\nllo, world");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_peek_string (&nav, 5);
    CHECK (span.start == text);
    CHECK (span_length (span) == 2);

    span = nav_peek_string (&nav, 5);
    CHECK (span.start == text);
    CHECK (span_length (span) == 2);
}

TESTER(nav_peek_string_3)
{
    am_byte *text = BTEXT ("He");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_peek_string (&nav, 5);
    CHECK (span.start == text);
    CHECK (span_length (span) == 2);

    span = nav_peek_string (&nav, 5);
    CHECK (span.start == text);
    CHECK (span_length (span) == 2);
}

TESTER(nav_peek_string_4)
{
    am_byte *text = BTEXT ("");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_peek_string (&nav, 5);
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);

    span = nav_peek_string (&nav, 5);
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);
}

TESTER(nav_peek_to_1)
{
    am_byte *text = BTEXT ("Hello, world");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_peek_to (&nav, 'o');
    CHECK (span.start == text);
    CHECK (span_length (span) == 5);

    span = nav_peek_to (&nav, 'o');
    CHECK (span.start == text);
    CHECK (span_length (span) == 5);
}

TESTER(nav_peek_to_2)
{
    am_byte *text = BTEXT ("Hello, world");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_peek_to (&nav, '?');
    CHECK (span.start == text);
    CHECK (span_length (span) == 12);

    span = nav_peek_to (&nav, '?');
    CHECK (span.start == text);
    CHECK (span_length (span) == 12);
}

TESTER(nav_peek_to_3)
{
    am_byte *text = BTEXT ("");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_peek_to (&nav, '?');
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);

    span = nav_peek_to (&nav, '?');
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);
}

TESTER(nav_until_to_1)
{
    am_byte *text = BTEXT ("Hello, world");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_peek_until (&nav, ',');
    CHECK (span.start == text);
    CHECK (span_length (span) == 5);

    span = nav_peek_until (&nav, ',');
    CHECK (span.start == text);
    CHECK (span_length (span) == 5);
}

TESTER(nav_until_to_2)
{
    am_byte *text = BTEXT ("Hello, world");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_peek_until (&nav, '?');
    CHECK (span.start == text);
    CHECK (span_length (span) == 12);

    span = nav_peek_until (&nav, '?');
    CHECK (span.start == text);
    CHECK (span_length (span) == 12);
}

TESTER(nav_until_to_3)
{
    am_byte *text = BTEXT ("");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_peek_until (&nav, '?');
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);

    span = nav_peek_until (&nav, '?');
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);
}

TESTER(nav_read_line_1)
{
    am_byte *text = BTEXT ("");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_read_line (&nav);
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);

    span = nav_read_line (&nav);
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);
}

TESTER(nav_read_line_2)
{
    am_byte *text = BTEXT ("Hello, world");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_read_line (&nav);
    CHECK (span.start == text);
    CHECK (span_length (span) == 12);

    span = nav_read_line (&nav);
    CHECK (span.start == text + 12);
    CHECK (span_length (span) == 0);
}

TESTER(nav_read_line_3)
{
    am_byte *text = BTEXT ("Hello\r\nworld");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_read_line (&nav);
    CHECK (span.start == text);
    CHECK (span_length (span) == 5);

    span = nav_read_line (&nav);
    CHECK (span.start == text + 7);
    CHECK (span_length (span) == 5);
}

TESTER(nav_is_control_1)
{
    am_byte *text = BTEXT ("H\te\r\nworld");
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (!nav_is_control (&nav));

    (void) nav_read (&nav);
    CHECK (nav_is_control (&nav));

    (void) nav_read (&nav);
    CHECK (!nav_is_control (&nav));

    (void) nav_read (&nav);
    CHECK (nav_is_control (&nav));
}

TESTER(nav_is_control_2)
{
    am_byte *text = BTEXT ("");
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (!nav_is_control (&nav));

    (void) nav_read (&nav);
    CHECK (!nav_is_control (&nav));
}

TESTER(nav_is_digit_1)
{
    am_byte *text = BTEXT ("H1e23world");
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (!nav_is_digit (&nav));

    (void) nav_read (&nav);
    CHECK (nav_is_digit (&nav));

    (void) nav_read (&nav);
    CHECK (!nav_is_digit (&nav));

    (void) nav_read (&nav);
    CHECK (nav_is_digit (&nav));
}

TESTER(nav_is_digit_2)
{
    am_byte *text = BTEXT ("");
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (!nav_is_digit (&nav));

    (void) nav_read (&nav);
    CHECK (!nav_is_digit (&nav));
}

TESTER(nav_is_letter_1)
{
    am_byte *text = BTEXT ("H1e23world");
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_is_letter (&nav));

    (void) nav_read (&nav);
    CHECK (!nav_is_letter (&nav));

    (void) nav_read (&nav);
    CHECK (nav_is_letter (&nav));

    (void) nav_read (&nav);
    CHECK (!nav_is_letter (&nav));
}

TESTER(nav_is_letter_2)
{
    am_byte *text = BTEXT ("");
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (!nav_is_letter (&nav));

    (void) nav_read (&nav);
    CHECK (!nav_is_letter (&nav));
}

TESTER(nav_is_whitespace_1)
{
    am_byte *text = BTEXT ("H e\t\vworld");
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (!nav_is_whitespace (&nav));

    (void) nav_read (&nav);
    CHECK (nav_is_whitespace (&nav));

    (void) nav_read (&nav);
    CHECK (!nav_is_whitespace (&nav));

    (void) nav_read (&nav);
    CHECK (nav_is_whitespace (&nav));
}

TESTER(nav_is_whitespace_2)
{
    am_byte *text = BTEXT ("");
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (!nav_is_whitespace (&nav));

    (void) nav_read (&nav);
    CHECK (!nav_is_whitespace (&nav));
}

TESTER(nav_read_integer_1)
{
    am_byte *text = BTEXT ("");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_read_integer (&nav);
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);

    span = nav_read_integer (&nav);
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);
}

TESTER(nav_read_integer_2)
{
    am_byte *text = BTEXT ("123");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_read_integer (&nav);
    CHECK (span.start == text);
    CHECK (span_length (span) == 3);

    span = nav_read_integer (&nav);
    CHECK (span.start == text + 3);
    CHECK (span_length (span) == 0);
}

TESTER(nav_read_integer_3)
{
    am_byte *text = BTEXT ("123?");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_read_integer (&nav);
    CHECK (span.start == text);
    CHECK (span_length (span) == 3);

    span = nav_read_integer (&nav);
    CHECK (span.start == text + 3);
    CHECK (span_length (span) == 0);
}

TESTER(nav_read_integer_4)
{
    am_byte *text = BTEXT ("123?456!");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_read_integer (&nav);
    CHECK (span.start == text);
    CHECK (span_length (span) == 3);

    (void) nav_read (&nav);
    span = nav_read_integer (&nav);
    CHECK (span.start == text + 4);
    CHECK (span_length (span) == 3);
}

TESTER(nav_extract_integer_1)
{
    am_byte *text = BTEXT ("");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_extract_integer (&nav);
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);

    span = nav_extract_integer (&nav);
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);
}

TESTER(nav_extract_integer_2)
{
    am_byte *text = BTEXT ("123");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_extract_integer (&nav);
    CHECK (span.start == text);
    CHECK (span_length (span) == 3);

    span = nav_extract_integer (&nav);
    CHECK (span.start == text + 3);
    CHECK (span_length (span) == 0);
}

TESTER(nav_extract_integer_3)
{
    am_byte *text = BTEXT ("?123!");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_extract_integer (&nav);
    CHECK (span.start == text + 1);
    CHECK (span_length (span) == 3);

    span = nav_extract_integer (&nav);
    CHECK (span.start == text + 5);
    CHECK (span_length (span) == 0);
}

TESTER(nav_extract_integer_4)
{
    am_byte *text = BTEXT ("123?456!");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_extract_integer (&nav);
    CHECK (span.start == text);
    CHECK (span_length (span) == 3);

    span = nav_extract_integer (&nav);
    CHECK (span.start == text + 4);
    CHECK (span_length (span) == 3);
}

TESTER(nav_read_string_1)
{
    am_byte *text = BTEXT ("");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_read_string (&nav, 10);
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);

    span = nav_read_string (&nav, 10);
    CHECK (span.start == text);
    CHECK (span_length (span) == 0);
}

TESTER(nav_read_string_2)
{
    am_byte *text = BTEXT ("1234567890");
    TextNavigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_read_string (&nav, 3);
    CHECK (span.start == text);
    CHECK (span_length (span) == 3);

    span = nav_read_string (&nav, 10);
    CHECK (span.start == text + 3);
    CHECK (span_length (span) == 7);
}








