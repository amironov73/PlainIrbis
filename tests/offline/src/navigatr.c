// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(nav_init_1)
{
    am_byte data[] = { 1, 2, 3 };
    Navigator nav;
    NavigatorPosition position;

    CHECK (nav_init (&nav, data, sizeof (data)) == &nav);
    CHECK (nav.data == data);
    CHECK (nav.length == sizeof (data));
    CHECK (nav.position == 0);

    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);
}

TESTER(nav_from_span_1)
{
    am_byte data[] = { 1, 2, 3 };
    Span span = span_init (data, sizeof (data));
    Navigator nav;
    NavigatorPosition position;

    CHECK (nav_from_span (&nav, span) == &nav);
    CHECK (nav.data == data);
    CHECK (nav.length == sizeof (data));
    CHECK (nav.position == 0);

    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);
}

TESTER(nav_from_buffer_1)
{
    am_byte data[] = { 1, 2, 3 };
    Buffer buffer;
    Navigator nav;
    NavigatorPosition position;

    buffer_static (&buffer, data, sizeof (data));

    CHECK (nav_from_buffer (&nav, &buffer) == &nav);
    CHECK (nav.data == data);
    CHECK (nav.length == sizeof (data));
    CHECK (nav.position == 0);

    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);
}

TESTER(nav_from_text_1)
{
    am_byte *text = BTEXT ("Hello, world");
    Navigator nav;
    NavigatorPosition position;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav.data == text);
    CHECK (nav.length == strlen (CCTEXT (text)));
    CHECK (nav.position == 0);

    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);
}

TESTER(nav_end_1)
{
    am_byte *text = BTEXT ("Hello, world");
    Navigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (*nav_end (&nav) == 0);
}

TESTER(nav_current_1)
{
    char *text = "Hello, world";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (*nav_current (&nav) == 'H');
}

TESTER(nav_eot_1)
{
    char *text = "";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_eot (&nav));
}

TESTER(nav_eot_2)
{
    char *text = "Hello, world";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (!nav_eot (&nav));

    nav_move (&nav, 100);
    CHECK (nav_eot (&nav));
}

TESTER(nav_at_1)
{
    char *text = "Hello, world";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_at (&nav, 0) == 'H');
    CHECK (nav_at (&nav, 1) == 'e');
    CHECK (nav_at (&nav, 100) == NAV_EOT);
}

TESTER(nav_front_1)
{
    char *text = "";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_front (&nav) == NAV_EOT);
}

TESTER(nav_front_2)
{
    char *text = "Hello, world";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_front (&nav) == 'H');
}

TESTER(nav_back_1)
{
    char *text = "";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_back (&nav) == NAV_EOT);
}

TESTER(nav_back_2)
{
    char *text = "Hello, world";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_back (&nav) == 'd');
}

TESTER(nav_look_ahead_1)
{
    char *text = "";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_look_ahead (&nav, 1) == NAV_EOT);
}

TESTER(nav_look_ahead_2)
{
    char *text = "Hello, world";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_look_ahead (&nav, 1) == 'e');
    CHECK (nav_look_ahead (&nav, 2) == 'l');
}

TESTER(nav_look_behind_1)
{
    char *text = "";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_look_behind (&nav, 1) == NAV_EOT);
}

TESTER(nav_look_behind_2)
{
    char *text = "Hello, world";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_read (&nav) == 'H');
    CHECK (nav_look_behind (&nav, 1) == 'H');
    CHECK (nav_look_behind (&nav, 2) == NAV_EOT);
}

TESTER(nav_move_1)
{
    char *text = "Hello, world";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_move (&nav, 3) == &nav);
    CHECK (nav_read (&nav) == 'l');
}

TESTER(nav_move_2)
{
    char *text = "Hello, world";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_move (&nav, 3) == &nav);
    CHECK (nav_read (&nav) == 'l');
    CHECK (nav_move (&nav, -10) == &nav);
    CHECK (nav.position == 0);
}

TESTER(nav_peek_1)
{
    char *text = "Hello, world";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_peek (&nav) == 'H');
    CHECK (nav_peek (&nav) == 'H');
}

TESTER(nav_peek_2)
{
    char *text = "";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_peek (&nav) == NAV_EOT);
    CHECK (nav_peek (&nav) == NAV_EOT);
}

TESTER(nav_peek_no_crlf_1)
{
    char *text = "Hello, world";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_peek_no_crlf (&nav) == 'H');
    CHECK (nav_peek_no_crlf (&nav) == 'H');
}

TESTER(nav_peek_no_crlf_2)
{
    char *text = "";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_peek_no_crlf (&nav) == NAV_EOT);
    CHECK (nav_peek_no_crlf (&nav) == NAV_EOT);
}

TESTER(nav_peek_no_crlf_3)
{
    char *text = "\r\nHello, world";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_peek_no_crlf (&nav) == 'H');
    CHECK (nav_peek_no_crlf (&nav) == 'H');
}

TESTER(nav_read_1)
{
    char *text = "Hello, world";
    Navigator nav;
    NavigatorPosition position;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);

    CHECK (nav_read (&nav) == 'H');
    position = nav_position (&nav);
    CHECK (position.column == 2);
    CHECK (position.line == 1);

    CHECK (nav_read (&nav) == 'e');
    position = nav_position (&nav);
    CHECK (position.column == 3);
    CHECK (position.line == 1);

    CHECK (nav_read (&nav) == 'l');
    position = nav_position (&nav);
    CHECK (position.column == 4);
    CHECK (position.line == 1);

    CHECK (nav_read (&nav) == 'l');
    position = nav_position (&nav);
    CHECK (position.column == 5);
    CHECK (position.line == 1);

    CHECK (nav_read (&nav) == 'o');
    position = nav_position (&nav);
    CHECK (position.column == 6);
    CHECK (position.line == 1);
}

TESTER(nav_read_2)
{
    char *text = "\r\nHello, world";
    Navigator nav;
    NavigatorPosition position;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);

    CHECK (nav_read (&nav) == '\r');
    position = nav_position (&nav);
    CHECK (position.column == 2);
    CHECK (position.line == 1);

    CHECK (nav_read (&nav) == '\n');
    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 2);

    CHECK (nav_read (&nav) == 'H');
    position = nav_position (&nav);
    CHECK (position.column == 2);
    CHECK (position.line == 2);

    CHECK (nav_read (&nav) == 'e');
    position = nav_position (&nav);
    CHECK (position.column == 3);
    CHECK (position.line == 2);

    CHECK (nav_read (&nav) == 'l');
    position = nav_position (&nav);
    CHECK (position.column == 4);
    CHECK (position.line == 2);
}

TESTER(nav_read_3)
{
    char *text = "";
    Navigator nav;
    NavigatorPosition position;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);

    CHECK (nav_read (&nav) == NAV_EOT);
    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);

    CHECK (nav_read (&nav) == NAV_EOT);
    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);
}

TESTER(nav_read_no_crlf_1)
{
    char *text = "Hello, world";
    Navigator nav;
    NavigatorPosition position;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);

    CHECK (nav_read_no_crlf (&nav) == 'H');
    position = nav_position (&nav);
    CHECK (position.column == 2);
    CHECK (position.line == 1);

    CHECK (nav_read_no_crlf (&nav) == 'e');
    position = nav_position (&nav);
    CHECK (position.column == 3);
    CHECK (position.line == 1);

    CHECK (nav_read_no_crlf (&nav) == 'l');
    position = nav_position (&nav);
    CHECK (position.column == 4);
    CHECK (position.line == 1);

    CHECK (nav_read_no_crlf (&nav) == 'l');
    position = nav_position (&nav);
    CHECK (position.column == 5);
    CHECK (position.line == 1);

    CHECK (nav_read_no_crlf (&nav) == 'o');
    position = nav_position (&nav);
    CHECK (position.column == 6);
    CHECK (position.line == 1);
}

TESTER(nav_read_no_crlf_2)
{
    char *text = "\r\nHello, world";
    Navigator nav;
    NavigatorPosition position;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);

    CHECK (nav_read_no_crlf (&nav) == 'H');
    position = nav_position (&nav);
    CHECK (position.column == 2);
    CHECK (position.line == 2);

    CHECK (nav_read_no_crlf (&nav) == 'e');
    position = nav_position (&nav);
    CHECK (position.column == 3);
    CHECK (position.line == 2);

    CHECK (nav_read_no_crlf (&nav) == 'l');
    position = nav_position (&nav);
    CHECK (position.column == 4);
    CHECK (position.line == 2);

    CHECK (nav_read_no_crlf (&nav) == 'l');
    position = nav_position (&nav);
    CHECK (position.column == 5);
    CHECK (position.line == 2);

    CHECK (nav_read_no_crlf (&nav) == 'o');
    position = nav_position (&nav);
    CHECK (position.column == 6);
    CHECK (position.line == 2);
}

TESTER(nav_read_no_crlf_3)
{
    char *text = "";
    Navigator nav;
    NavigatorPosition position;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);

    CHECK (nav_read_no_crlf (&nav) == NAV_EOT);
    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);

    CHECK (nav_read_no_crlf (&nav) == NAV_EOT);
    position = nav_position (&nav);
    CHECK (position.column == 1);
    CHECK (position.line == 1);
}

TESTER(nav_peek_string_1)
{
    am_byte *text = BTEXT ("Hello, world");
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;

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
    Navigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (!nav_is_control (&nav));

    (void) nav_read (&nav);
    CHECK (!nav_is_control (&nav));
}

TESTER(nav_is_digit_1)
{
    am_byte *text = BTEXT ("H1e23world");
    Navigator nav;

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
    Navigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (!nav_is_digit (&nav));

    (void) nav_read (&nav);
    CHECK (!nav_is_digit (&nav));
}

TESTER(nav_is_letter_1)
{
    am_byte *text = BTEXT ("H1e23world");
    Navigator nav;

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
    Navigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (!nav_is_letter (&nav));

    (void) nav_read (&nav);
    CHECK (!nav_is_letter (&nav));
}

TESTER(nav_is_whitespace_1)
{
    am_byte *text = BTEXT ("H e\t\vworld");
    Navigator nav;

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
    Navigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (!nav_is_whitespace (&nav));

    (void) nav_read (&nav);
    CHECK (!nav_is_whitespace (&nav));
}

TESTER(nav_read_integer_1)
{
    am_byte *text = BTEXT ("");
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
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
    Navigator nav;
    Span span;

    CHECK (nav_from_text (&nav, text) == &nav);

    span = nav_read_string (&nav, 3);
    CHECK (span.start == text);
    CHECK (span_length (span) == 3);

    span = nav_read_string (&nav, 10);
    CHECK (span.start == text + 3);
    CHECK (span_length (span) == 7);
}

TESTER(nav_read_utf8_1)
{
    char *text = "";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
}

TESTER(nav_read_utf8_2)
{
    char *text = "Hello, world!";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    CHECK (nav_read_utf8 (&nav) == 72);
    CHECK (nav_read_utf8 (&nav) == 101);
    CHECK (nav_read_utf8 (&nav) == 108);
    CHECK (nav_read_utf8 (&nav) == 108);
    CHECK (nav_read_utf8 (&nav) == 111);
    CHECK (nav_read_utf8 (&nav) == 44);
    CHECK (nav_read_utf8 (&nav) == 32);
    CHECK (nav_read_utf8 (&nav) == 119);
    CHECK (nav_read_utf8 (&nav) == 111);
    CHECK (nav_read_utf8 (&nav) == 114);
    CHECK (nav_read_utf8 (&nav) == 108);
    CHECK (nav_read_utf8 (&nav) == 100);
    CHECK (nav_read_utf8 (&nav) == 33);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
}

TESTER(nav_read_utf8_3)
{
    // Privet, mir!
    am_byte data[] = { 208, 159, 209, 128, 208, 184, 208, 178,
        208, 181, 209, 130, 44, 32, 208, 188, 208, 184, 209, 128, 33 };
    Navigator nav;

    CHECK (nav_init (&nav, data, sizeof (data)) == &nav);
    CHECK (nav_read_utf8 (&nav) == 1055);
    CHECK (nav_read_utf8 (&nav) == 1088);
    CHECK (nav_read_utf8 (&nav) == 1080);
    CHECK (nav_read_utf8 (&nav) == 1074);
    CHECK (nav_read_utf8 (&nav) == 1077);
    CHECK (nav_read_utf8 (&nav) == 1090);
    CHECK (nav_read_utf8 (&nav) == 44);
    CHECK (nav_read_utf8 (&nav) == 32);
    CHECK (nav_read_utf8 (&nav) == 1084);
    CHECK (nav_read_utf8 (&nav) == 1080);
    CHECK (nav_read_utf8 (&nav) == 1088);
    CHECK (nav_read_utf8 (&nav) == 33);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
}

TESTER(nav_read_utf8_4)
{
    // Privet, mir!
    am_byte data[] = { 208, 159, 209 };
    Navigator nav;

    CHECK (nav_init (&nav, data, sizeof (data)) == &nav);
    CHECK (nav_read_utf8 (&nav) == 1055);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
}

TESTER(nav_read_utf8_5)
{
    // Kon'nichiwa
    am_byte data[] = { 227, 129, 147, 227, 130, 147, 227, 129,
        171, 227, 129, 161, 227, 129, 175, 33 };
    Navigator nav;

    CHECK (nav_init (&nav, data, sizeof (data)) == &nav);
    CHECK (nav_read_utf8 (&nav) == 12371);
    CHECK (nav_read_utf8 (&nav) == 12435);
    CHECK (nav_read_utf8 (&nav) == 12395);
    CHECK (nav_read_utf8 (&nav) == 12385);
    CHECK (nav_read_utf8 (&nav) == 12399);
    CHECK (nav_read_utf8 (&nav) == 33);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
}

TESTER(nav_read_utf8_6)
{
    // Kon'nichiwa
    am_byte data[] = { 227, 129, 147, 227, 130 };
    Navigator nav;

    CHECK (nav_init (&nav, data, sizeof (data)) == &nav);
    CHECK (nav_read_utf8 (&nav) == 12371);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
}

TESTER(nav_read_utf8_7)
{
    // Kon'nichiwa
    am_byte data[] = { 227, 129, 147, 227 };
    Navigator nav;

    CHECK (nav_init (&nav, data, sizeof (data)) == &nav);
    CHECK (nav_read_utf8 (&nav) == 12371);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
}

TESTER(nav_read_utf8_8)
{
    // Ancient Greek Numbers
    am_byte data[] = { 240, 144, 133, 128, 240, 144, 133, 129 };
    Navigator nav;

    CHECK (nav_init (&nav, data, sizeof (data)) == &nav);
    CHECK (nav_read_utf8 (&nav) == 65856);
    CHECK (nav_read_utf8 (&nav) == 65857);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
}

TESTER(nav_read_utf8_9)
{
    // Ancient Greek Numbers
    am_byte data[] = { 240, 144, 133, 128, 240, 144, 133 };
    Navigator nav;

    CHECK (nav_init (&nav, data, sizeof (data)) == &nav);
    CHECK (nav_read_utf8 (&nav) == 65856);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
}

TESTER(nav_read_utf8_10)
{
    // Ancient Greek Numbers
    am_byte data[] = { 240, 144, 133, 128, 240, 144 };
    Navigator nav;

    CHECK (nav_init (&nav, data, sizeof (data)) == &nav);
    CHECK (nav_read_utf8 (&nav) == 65856);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
}

TESTER(nav_read_utf8_11)
{
    // Ancient Greek Numbers
    am_byte data[] = { 240, 144, 133, 128, 240 };
    Navigator nav;

    CHECK (nav_init (&nav, data, sizeof (data)) == &nav);
    CHECK (nav_read_utf8 (&nav) == 65856);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
    CHECK (nav_read_utf8 (&nav) == NAV_EOT);
}

TESTER(nav_skip_punctuation_1)
{
    char *text = "";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    nav_skip_punctuation (&nav);
    CHECK (nav_eot (&nav));
}

TESTER(nav_skip_punctuation_2)
{
    char *text = ",.H";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    nav_skip_punctuation (&nav);
    CHECK (nav_read (&nav) == 'H');
    CHECK (nav_eot (&nav));
}

TESTER(nav_skip_whitespace_1)
{
    char *text = "";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    nav_skip_whitespace (&nav);
    CHECK (nav_eot (&nav));
}

TESTER(nav_skip_whitespace_2)
{
    char *text = "\t \r\nH";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    nav_skip_whitespace (&nav);
    CHECK (nav_read (&nav) == 'H');
    CHECK (nav_eot (&nav));
}

TESTER(nav_skip_non_word_1)
{
    char *text = "";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    nav_skip_non_word (&nav);
    CHECK (nav_eot (&nav));
}

TESTER(nav_skip_non_word_2)
{
    char *text = ", (+H";
    Navigator nav;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    nav_skip_non_word (&nav);
    CHECK (nav_read (&nav) == 'H');
    CHECK (nav_eot (&nav));
}

TESTER(nav_recent_1)
{
    char *text = "";
    Navigator nav;
    Span recent;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    recent = nav_recent (&nav, 10);
    CHECK (span_is_empty (recent));
}

TESTER(nav_recent_2)
{
    char *text = "Hello, world";
    Navigator nav;
    Span word, recent;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    word = nav_read_word (&nav);
    recent = nav_recent (&nav, 10);
    CHECK (span_compare (word, recent) == 0);
}

TESTER(nav_to_span_1)
{
    char *text = "";
    Navigator nav;
    Span span;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    span = nav_to_span (&nav);
    CHECK (span_is_empty (span));
}

TESTER(nav_to_span_2)
{
    char *text = "Hello";
    Navigator nav;
    Span span;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    span = nav_to_span (&nav);
    CHECK (span_length (span) == strlen (text));
}

TESTER(nav_remaining_1)
{
    char *text = "";
    Navigator nav;
    Span span;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    span = nav_remaining (&nav);
    CHECK (span_is_empty (span));
}

TESTER(nav_remaining_2)
{
    char *text = "Hello";
    Navigator nav;
    Span span;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    span = nav_remaining (&nav);
    CHECK (span_length (span) == strlen (text));
}

TESTER(nav_read_while_1)
{
    char *text = "";
    Navigator nav;
    Span span;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    span = nav_read_while (&nav, 'c');
    CHECK (span_is_empty (span));
}

TESTER(nav_read_while_2)
{
    char *text = "Hello";
    Navigator nav;
    Span span;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    span = nav_read_while (&nav, 'c');
    CHECK (span_is_empty (span));
}

TESTER(nav_read_while_3)
{
    char *text = "cccHello";
    Navigator nav;
    Span span;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    span = nav_read_while (&nav, 'c');
    CHECK (span_length (span) == 3);
}

TESTER(nav_read_until_1)
{
    char *text = "";
    Navigator nav;
    Span span;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    span = nav_read_until (&nav, 'c');
    CHECK (span_is_empty (span));
}

TESTER(nav_read_until_2)
{
    char *text = "Hello";
    Navigator nav;
    Span span;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    span = nav_read_until (&nav, 'c');
    CHECK (span_length (span) == strlen (text));
}

TESTER(nav_read_until_3)
{
    char *text = "Helloccc";
    Navigator nav;
    Span span;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    span = nav_read_until (&nav, 'c');
    CHECK (span_length (span) == 5);
}

TESTER(nav_read_irbis_1)
{
    char *text = "";
    Navigator nav;
    Span line;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    line = nav_read_irbis (&nav);
    CHECK (span_is_empty (line));
}

TESTER(nav_read_irbis_2)
{
    char *text = "Hello";
    Navigator nav;
    Span line;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    line = nav_read_irbis (&nav);
    CHECK (span_length (line) == 5);
    CHECK (nav_eot (&nav));
}

TESTER(nav_read_irbis_3)
{
    char *text = "Hello\x1F\x1Eworld!";
    Navigator nav;
    Span line;

    CHECK (nav_from_text (&nav, CBTEXT (text)) == &nav);
    line = nav_read_irbis (&nav);
    CHECK (span_length (line) == 5);
    line = nav_read_irbis (&nav);
    CHECK (span_length (line) == 6);
    CHECK (nav_eot (&nav));
}

