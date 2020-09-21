// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"
#include "magna/irbis.h"

TESTER(menu_entry_init_1)
{
    MenuEntry entry;

    menu_entry_init (&entry);

    CHECK (entry.code.position == 0);
    CHECK (entry.comment.position == 0);

    menu_entry_free (&entry);
}

TESTER(menu_entry_to_string_1)
{
    MenuEntry entry;
    Buffer text = BUFFER_INIT;

    menu_entry_init (&entry);

    CHECK (menu_entry_to_string (&entry, &text));
    CHECK (buffer_empty (&text));

    menu_entry_free (&entry);
    buffer_free (&text);
}

TESTER(menu_entry_to_string_2)
{
    MenuEntry entry;
    Buffer text = BUFFER_INIT;

    menu_entry_init (&entry);

    buffer_assign_text (&entry.code, "A");

    CHECK (menu_entry_to_string (&entry, &text));
    CHECK (buffer_compare_text (&text, "A") == 0);

    menu_entry_free (&entry);
    buffer_free (&text);
}

TESTER(menu_entry_to_string_3)
{
    MenuEntry entry;
    Buffer text = BUFFER_INIT;

    menu_entry_init (&entry);

    buffer_assign_text (&entry.code, "A");
    buffer_assign_text (&entry.comment, "first letter");

    CHECK (menu_entry_to_string (&entry, &text));
    CHECK (buffer_compare_text (&text, "A - first letter") == 0);

    menu_entry_free (&entry);
    buffer_free (&text);
}

TESTER(menu_init_1)
{
    MenuFile menu;

    CHECK (menu_init (&menu));
    CHECK (menu.entries.len == 0);

    menu_free (&menu);
}

TESTER(menu_append_1)
{
    MenuFile menu;

    CHECK (menu_init (&menu));

    CHECK (menu_append (&menu, span_null(), span_null()));
    CHECK (menu.entries.len == 1);

    CHECK (menu_append (&menu, span_null(), span_null()));
    CHECK (menu.entries.len == 2);

    menu_free (&menu);
}

TESTER(menu_get_entry_1)
{
    MenuFile menu;
    const MenuEntry *entry;

    CHECK (menu_init (&menu));

    entry = menu_get_entry
        (
            &menu,
            span_from_text ("first")
        );
    CHECK (entry == NULL);

    menu_free (&menu);
}

TESTER(menu_get_comment_1)
{
    MenuFile menu;
    Span comment;
    const char *defaultValue = "default";

    CHECK (menu_init (&menu));

    comment = menu_get_comment
        (
            &menu,
            span_from_text ("first"),
            span_from_text (defaultValue)
        );
    CHECK (comment.ptr == defaultValue);

    menu_free (&menu);
}

TESTER(menu_parse_1)
{
    Stream memory;
    StreamTexter texter;
    MenuFile menu;

    CHECK (menu_init (&menu));
    CHECK (memory_stream_create (&memory));
    CHECK (texter_init (&texter, &memory, 0));

    CHECK (menu_parse (&menu, &texter));
    CHECK (menu.entries.len == 0);

    texter_free (&texter);
    menu_free (&menu);
}

TESTER(menu_parse_2)
{
    Stream memory;
    StreamTexter texter;
    MenuFile menu;
    am_byte *text = "A\r\nfirst letter\r\nB\r\nsecond letter\r\n*****\r\n\r\n";
    const MenuEntry *entry;
    Span comment, defaultValue = span_from_text ("default");

    CHECK (menu_init (&menu));
    CHECK (memory_stream_open (&memory, text, strlen (text)));
    CHECK (texter_init (&texter, &memory, 0));

    CHECK (menu_parse (&menu, &texter));
    CHECK (menu.entries.len == 2);

    entry = menu_get_entry (&menu, span_from_text ("A"));
    CHECK (entry != NULL);
    CHECK (buffer_compare_text (&entry->code, "A") == 0);

    entry = menu_get_entry (&menu, span_from_text ("b"));
    CHECK (entry != NULL);
    CHECK (buffer_compare_text (&entry->code, "B") == 0);

    entry = menu_get_entry (&menu, span_from_text ("?"));
    CHECK (entry == NULL);

    comment = menu_get_comment (&menu, span_from_text("A"), defaultValue);
    CHECK (span_compare (comment, span_from_text ("first letter")) == 0);

    comment = menu_get_comment (&menu, span_from_text("b"), defaultValue);
    CHECK (span_compare (comment, span_from_text ("second letter")) == 0);

    comment = menu_get_comment (&menu, span_from_text("?"), defaultValue);
    CHECK (span_compare (comment, defaultValue) == 0);

    texter_free (&texter);
    menu_free (&menu);
}
