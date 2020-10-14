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

    menu_entry_destroy (&entry);
}

TESTER(menu_entry_to_string_1)
{
    MenuEntry entry;
    Buffer text = BUFFER_INIT;

    menu_entry_init (&entry);

    CHECK (menu_entry_to_string (&entry, &text));
    CHECK (buffer_is_empty(&text));

    menu_entry_destroy (&entry);
    buffer_destroy (&text);
}

TESTER(menu_entry_to_string_2)
{
    MenuEntry entry;
    Buffer text = BUFFER_INIT;

    menu_entry_init (&entry);

    buffer_assign_text (&entry.code, CBTEXT ("A"));

    CHECK (menu_entry_to_string (&entry, &text));
    CHECK (buffer_compare_text (&text, CBTEXT ("A")) == 0);

    menu_entry_destroy (&entry);
    buffer_destroy (&text);
}

TESTER(menu_entry_to_string_3)
{
    MenuEntry entry;
    Buffer text = BUFFER_INIT;

    menu_entry_init (&entry);
    buffer_assign_text (&entry.code, CBTEXT ("A"));
    buffer_assign_text (&entry.comment, CBTEXT ("first letter"));

    CHECK (menu_entry_to_string (&entry, &text));
    CHECK (buffer_compare_text (&text, CBTEXT ("A - first letter")) == 0);

    menu_entry_destroy (&entry);
    buffer_destroy (&text);
}

TESTER(menu_init_1)
{
    MenuFile menu;

    menu_init (&menu);
    CHECK (menu.entries.len == 0);

    menu_destroy (&menu);
}

TESTER(menu_append_1)
{
    MenuFile menu;

    menu_init (&menu);

    CHECK (menu_append (&menu, span_null(), span_null()));
    CHECK (menu.entries.len == 1);

    CHECK (menu_append (&menu, span_null(), span_null()));
    CHECK (menu.entries.len == 2);

    menu_destroy (&menu);
}

TESTER(menu_get_entry_1)
{
    MenuFile menu;
    const MenuEntry *entry;

    menu_init (&menu);
    entry = menu_get_entry
        (
            &menu,
            TEXT_SPAN ("first")
        );
    CHECK (entry == NULL);

    menu_destroy (&menu);
}

TESTER(menu_get_comment_1)
{
    MenuFile menu;
    Span comment;
    const am_byte *defaultValue = CBTEXT ("default");

    menu_init (&menu);
    comment = menu_get_comment
        (
            &menu,
            TEXT_SPAN ("first"),
            span_from_text (defaultValue)
        );
    CHECK (comment.ptr == defaultValue);

    menu_destroy (&menu);
}

TESTER(menu_parse_1)
{
    Stream memory;
    StreamTexter texter;
    MenuFile menu;

    menu_init (&menu);

    CHECK (memory_stream_create (&memory));
    CHECK (texter_init (&texter, &memory, 0));

    CHECK (menu_parse (&menu, &texter));
    CHECK (menu.entries.len == 0);

    texter_destroy (&texter);
    menu_destroy (&menu);
}

TESTER(menu_parse_2)
{
    Stream memory;
    StreamTexter texter;
    MenuFile menu;
    am_byte *text = BTEXT ("A\r\nfirst letter\r\nB\r\nsecond letter\r\n*****\r\n\r\n");
    const MenuEntry *entry;
    Span comment, defaultValue = TEXT_SPAN ("default");

    menu_init (&menu);

    CHECK (memory_stream_open (&memory, text, strlen (CCTEXT (text))));
    CHECK (texter_init (&texter, &memory, 0));

    CHECK (menu_parse (&menu, &texter));
    CHECK (menu.entries.len == 2);

    entry = menu_get_entry (&menu, TEXT_SPAN ("A"));
    CHECK (entry != NULL);
    CHECK (buffer_compare_text (&entry->code, CBTEXT ("A")) == 0);

    entry = menu_get_entry (&menu, TEXT_SPAN ("b"));
    CHECK (entry != NULL);
    CHECK (buffer_compare_text (&entry->code, CBTEXT ("B")) == 0);

    entry = menu_get_entry (&menu, TEXT_SPAN ("?"));
    CHECK (entry == NULL);

    comment = menu_get_comment (&menu, TEXT_SPAN("A"), defaultValue);
    CHECK (span_compare (comment, TEXT_SPAN ("first letter")) == 0);

    comment = menu_get_comment (&menu, TEXT_SPAN ("b"), defaultValue);
    CHECK (span_compare (comment, TEXT_SPAN ("second letter")) == 0);

    comment = menu_get_comment (&menu, TEXT_SPAN ("?"), defaultValue);
    CHECK (span_compare (comment, defaultValue) == 0);

    texter_destroy (&texter);
    menu_destroy (&menu);
}

TESTER(menu_to_stream_1)
{
    Stream memory;
    MenuFile menu;
    am_byte *text;

    menu_init (&menu);

    CHECK (memory_stream_create (&memory));
    CHECK (menu_to_stream (&menu, &memory));
    text = memory_stream_to_text (&memory);
    CHECK (text != NULL);
    CHECK (strcmp (CCTEXT (text), "*****\n") == 0);

    menu_destroy (&menu);
    stream_close (&memory);
}
