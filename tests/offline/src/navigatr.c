/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

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
    Span span = { data, sizeof (data) };
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
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav.data == text);
    CHECK (nav.length == strlen (text));
    CHECK (nav.position == 0);
    CHECK (nav.line = 1);
    CHECK (nav.column = 1);
}

TESTER(nav_end_1)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (*nav_end (&nav) == 0);
}

TESTER(nav_current_1)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (*nav_current (&nav) == 'H');
}

TESTER(nav_eot_1)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_eot (&nav));
}

TESTER(nav_eot_2)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (!nav_eot (&nav));
}

TESTER(nav_at_1)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_at (&nav, 0) == 'H');
    CHECK (nav_at (&nav, 1) == 'e');
    CHECK (nav_at (&nav, 100) == NAV_EOT);
}

TESTER(nav_front_1)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_front (&nav) == NAV_EOT);
}

TESTER(nav_front_2)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_front (&nav) == 'H');
}

TESTER(nav_back_1)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_back (&nav) == NAV_EOT);
}

TESTER(nav_back_2)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_back (&nav) == 'd');
}

TESTER(nav_look_ahead_1)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_look_ahead (&nav, 1) == NAV_EOT);
}

TESTER(nav_look_ahead_2)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_look_ahead (&nav, 1) == 'e');
    CHECK (nav_look_ahead (&nav, 2) == 'l');
}

TESTER(nav_look_behind_1)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_look_behind (&nav, 1) == NAV_EOT);
}

TESTER(nav_look_behind_2)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_read (&nav) == 'H');
    CHECK (nav_look_behind (&nav, 1) == 'H');
    CHECK (nav_look_behind (&nav, 2) == NAV_EOT);
}

TESTER(nav_move_1)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_move (&nav, 3) == &nav);
    CHECK (nav_read (&nav) == 'l');
}

TESTER(nav_peek_1)
{
    char *text = "Hello, world";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_peek (&nav) == 'H');
    CHECK (nav_peek (&nav) == 'H');
}

TESTER(nav_peek_2)
{
    char *text = "";
    TextNavigator nav;

    CHECK (nav_from_text (&nav, text) == &nav);
    CHECK (nav_peek (&nav) == NAV_EOT);
    CHECK (nav_peek (&nav) == NAV_EOT);
}

