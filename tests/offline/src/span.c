// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(span_init_1)
{
    Span span = SPAN_INIT;

    CHECK (span.ptr == NULL);
    CHECK (span.len == 0);
}

TESTER(span_init_2)
{
    Span span = span_init (NULL, 0);

    CHECK (span.ptr == NULL);
    CHECK (span.len == 0);
}

TESTER(span_init_3)
{
    const am_byte data[] = { 1, 2, 3 };
    Span span = span_init (data, sizeof (data));

    CHECK (span.ptr == data);
    CHECK (span.len == sizeof (data));
}

TESTER(span_from_text_1)
{
    const char *text = "Hello";
    Span span = span_from_text (text);

    CHECK (span.ptr == text);
    CHECK (span.len == 5);
}

TESTER(span_trim_start_1)
{
    Span span1 = SPAN_INIT;
    Span span2 = span_trim_start (span1);

    CHECK (span2.ptr == NULL);
    CHECK (span2.len == 0);
}

TESTER(span_trim_start_2)
{
    const char *text = "Hello";
    Span span1 = span_from_text (text);
    Span span2 = span_trim_start (span1);

    CHECK (span2.ptr == text);
    CHECK (span2.len == 5);
}

TESTER(span_trim_start_3)
{
    const char *text = "  Hello";
    Span span1 = span_from_text (text);
    Span span2 = span_trim_start (span1);

    CHECK (span2.ptr == text + 2);
    CHECK (span2.len == 5);
}

TESTER(span_trim_end_1)
{
    Span span1 = SPAN_INIT;
    Span span2 = span_trim_end (span1);

    CHECK (span2.ptr == NULL);
    CHECK (span2.len == 0);
}

TESTER(span_trim_end_2)
{
    const char *text = "Hello";
    Span span1 = span_from_text (text);
    Span span2 = span_trim_end (span1);

    CHECK (span2.ptr == text);
    CHECK (span2.len == 5);
}

TESTER(span_trim_end_3)
{
    const char *text = "Hello  ";
    Span span1 = span_from_text (text);
    Span span2 = span_trim_end (span1);

    CHECK (span2.ptr == text);
    CHECK (span2.len == 5);
}

TESTER(span_trim_1)
{
    Span span1 = SPAN_INIT;
    Span span2 = span_trim (span1);

    CHECK (span2.ptr == NULL);
    CHECK (span2.len == 0);
}

TESTER(span_trim_2)
{
    const char *text = "Hello";
    Span span1 = span_from_text (text);
    Span span2 = span_trim (span1);

    CHECK (span2.ptr == text);
    CHECK (span2.len == 5);
}

TESTER(span_trim_3)
{
    const char *text = "  Hello  ";
    Span span1 = span_from_text (text);
    Span span2 = span_trim (span1);

    CHECK (span2.ptr == text + 2);
    CHECK (span2.len == 5);
}

TESTER(span_to_uint_32_1)
{
    Span span = SPAN_INIT;
    am_uint32 value = span_to_uint_32 (span);

    CHECK (value == 0);

}

TESTER(span_to_uint_32_2)
{
    const char *text = "123456789";
    Span span = span_from_text (text);
    am_uint32 value = span_to_uint_32 (span);

    CHECK (value == 123456789);
}

TESTER(span_to_uint_64_1)
{
    Span span = SPAN_INIT;
    am_uint64 value = span_to_uint_64 (span);

    CHECK (value == 0);

}

TESTER(span_to_uint_64_2)
{
    const char *text = "123456789";
    Span span = span_from_text (text);
    am_uint64 value = span_to_uint_64 (span);

    CHECK (value == 123456789);
}

TESTER(span_toupper_1)
{
    char *hello = str_dup ("Hello");
    Span span = span_from_text (hello);
    span_toupper (span);

    CHECK (strcmp (hello, "HELLO") == 0);

    free (hello);
}

TESTER(span_tolower_1)
{
    char *hello = str_dup ("Hello");
    Span span = span_from_text (hello);
    span_tolower (span);

    CHECK (strcmp (hello, "hello") == 0);

    free (hello);
}

TESTER(span_is_empty_1)
{
    Span span = SPAN_INIT;

    CHECK (span_is_empty (span));
}

TESTER(span_is_empty_2)
{
    const char *text = "123456789";
    Span span = span_from_text (text);

    CHECK (!span_is_empty (span));
}

TESTER(span_find_byte_1)
{
    const char *text = "123456789";
    Span span = span_from_text (text);
    am_byte *found = span_find_byte (span, '?');

    CHECK (!found);

    found = span_find_byte (span, '5');
    CHECK (*found == '5');
}

TESTER(span_fill_1)
{
    char *hello = str_dup ("Hello");
    Span span = span_from_text (hello);
    span_fill (span, '0');

    CHECK (strcmp (hello, "00000") == 0);

    free (hello);
}

TESTER(span_index_of_1)
{
    const char *text = "123456789";
    Span span = span_from_text (text);
    ssize_t found = span_index_of (span, '?');

    CHECK (found < 0);

    found = span_index_of (span, '5');

    CHECK (found == 4);
}

TESTER(span_last_index_of_1)
{
    const char *text = "123456789";
    Span span = span_from_text (text);
    ssize_t found = span_last_index_of (span, '?');

    CHECK (found < 0);

    found = span_last_index_of (span, '5');

    CHECK (found == 4);
}

TESTER(span_slice_1)
{
    Span span1 = SPAN_INIT;
    Span span2 = span_slice (span1, 5, 10);

    CHECK (span2.len == 0);
}

TESTER(span_slice_2)
{
    const char *text = "123456789";
    Span span1 = span_from_text (text);
    Span span2 = span_slice (span1, 2, 2);

    CHECK (span2.ptr[0] == '3');
    CHECK (span2.len == 2);
}

TESTER(span_slice_3)
{
    const char *text = "123456789";
    Span span1 = span_from_text (text);
    Span span2 = span_slice (span1, 7, 12);

    CHECK (span2.ptr[0] == '8');
    CHECK (span2.len == 2);
}

TESTER(span_to_string_1)
{
    const char *text = "123456789";
    Span span1 = span_from_text (text);
    Span span2 = span_slice (span1, 7, 12);
    char *str = span_to_string (span2);

    CHECK (strcmp (str, "89") == 0);

    free (str);
}

TESTER(span_to_vector_1)
{
    const char *text = "123456789";
    Span span1 = span_from_text (text);
    Span span2 = span_slice (span1, 7, 12);
    am_byte *vec = span_to_vector (span2);

    CHECK (vec[0] == '8');
    CHECK (vec[1] == '9');

    free (vec);
}

TESTER(span_compare_1)
{
    Span span = SPAN_INIT;

    CHECK (span_compare (span, span) == 0);
}

TESTER(span_compare_2)
{
    const char *text1 = "123456789", *text2 = "qwertyiop";
    Span span1 = span_from_text (text1);
    Span span2 = span_from_text (text2);

    CHECK (span_compare (span1, span2) < 0);
}

TESTER(span_compare_3)
{
    const char *text1 = "123456789";
    Span span1 = span_from_text (text1);
    Span span2 = span_from_text (text1);

    CHECK (span_compare (span1, span2) == 0);
}

TESTER(span_compare_4)
{
    const char *text1 = "1234", *text2 = "123456789";
    Span span1 = span_from_text (text1);
    Span span2 = span_from_text (text2);

    CHECK (span_compare (span1, span2) < 0);
}

TESTER(span_compare_5)
{
    const char *text1 = "123456789", *text2 = "1234";
    Span span1 = span_from_text (text1);
    Span span2 = span_from_text (text2);

    CHECK (span_compare (span1, span2) > 0);
}

TESTER(span_split_by_char_1)
{
    Span span = SPAN_INIT;
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_char (span, &parts, '?');

    CHECK (rc);
    CHECK (parts.len == 0);

    span_array_free (&parts);
}

TESTER(span_split_by_char_2)
{
    const char *text = "123456789";
    Span span = span_from_text (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_char (span, &parts, '?');

    CHECK (rc);
    CHECK (parts.len == 1);
    CHECK (parts.ptr[0].ptr == text);
    CHECK (parts.ptr[0].len == 9);

    span_array_free (&parts);
}

TESTER(span_split_by_char_3)
{
    const char *text = "123456789";
    Span span = span_from_text (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_char (span, &parts, '5');

    CHECK (rc);
    CHECK (parts.len == 2);
    CHECK (parts.ptr[0].ptr == text);
    CHECK (parts.ptr[0].len == 4);
    CHECK (parts.ptr[1].ptr == text + 5);
    CHECK (parts.ptr[1].len == 4);

    span_array_free (&parts);
}

TESTER(span_split_by_char_4)
{
    const char *text = "123,567,9";
    Span span = span_from_text (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_char (span, &parts, ',');

    CHECK (rc);
    CHECK (parts.len == 3);
    CHECK (parts.ptr[0].ptr == text);
    CHECK (parts.ptr[0].len == 3);
    CHECK (parts.ptr[1].ptr == text + 4);
    CHECK (parts.ptr[1].len == 3);
    CHECK (parts.ptr[2].ptr == text + 8);
    CHECK (parts.ptr[2].len == 1);

    span_array_free (&parts);
}

TESTER(span_split_by_char_5)
{
    const char *text = ",123,,567,9";
    Span span = span_from_text (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_char (span, &parts, ',');

    CHECK (rc);
    CHECK (parts.len == 3);
    CHECK (parts.ptr[0].ptr == text + 1);
    CHECK (parts.ptr[0].len == 3);
    CHECK (parts.ptr[1].ptr == text + 6);
    CHECK (parts.ptr[1].len == 3);
    CHECK (parts.ptr[2].ptr == text + 10);
    CHECK (parts.ptr[2].len == 1);

    span_array_free (&parts);
}

TESTER(span_split_by_chars_1)
{
    Span span = SPAN_INIT;
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_chars (span, &parts, "!?", 2);

    CHECK (rc);
    CHECK (parts.len == 0);

    span_array_free (&parts);
}

TESTER(span_split_by_chars_2)
{
    const char *text = "123456789";
    Span span = span_from_text (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_chars (span, &parts, "!?", 2);

    CHECK (rc);
    CHECK (parts.len == 1);
    CHECK (parts.ptr[0].ptr == text);
    CHECK (parts.ptr[0].len == 9);

    span_array_free (&parts);
}

TESTER(span_split_by_chars_3)
{
    const char *text = "123456789";
    Span span = span_from_text (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_chars (span, &parts, "5?", 2);

    CHECK (rc);
    CHECK (parts.len == 2);
    CHECK (parts.ptr[0].ptr == text);
    CHECK (parts.ptr[0].len == 4);
    CHECK (parts.ptr[1].ptr == text + 5);
    CHECK (parts.ptr[1].len == 4);

    span_array_free (&parts);
}

TESTER(span_split_by_chars_4)
{
    const char *text = "123.567,9";
    Span span = span_from_text (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_chars (span, &parts,",.", 2);

    CHECK (rc);
    CHECK (parts.len == 3);
    CHECK (parts.ptr[0].ptr == text);
    CHECK (parts.ptr[0].len == 3);
    CHECK (parts.ptr[1].ptr == text + 4);
    CHECK (parts.ptr[1].len == 3);
    CHECK (parts.ptr[2].ptr == text + 8);
    CHECK (parts.ptr[2].len == 1);

    span_array_free (&parts);
}

TESTER(span_split_by_chars_5)
{
    const char *text = ",123,.567.9";
    Span span = span_from_text (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_chars (span, &parts, ",.", 2);

    CHECK (rc);
    CHECK (parts.len == 3);
    CHECK (parts.ptr[0].ptr == text + 1);
    CHECK (parts.ptr[0].len == 3);
    CHECK (parts.ptr[1].ptr == text + 6);
    CHECK (parts.ptr[1].len == 3);
    CHECK (parts.ptr[2].ptr == text + 10);
    CHECK (parts.ptr[2].len == 1);

    span_array_free (&parts);
}

TESTER(span_split_n_by_char_1)
{
    Span span = SPAN_INIT;
    Span parts [2];
    size_t count = span_split_n_by_char (span, parts, 2, '?');

    CHECK (count == 0);
}

TESTER(span_split_n_by_char_2)
{
    const char *text = "123456789";
    Span span = span_from_text (text);
    Span parts [2];
    size_t count = span_split_n_by_char (span, parts, 2, '?');

    CHECK (count == 1);
    CHECK (parts[0].ptr == text);
    CHECK (parts[0].len == 9);
}

TESTER(span_split_n_by_char_3)
{
    const char *text = "123456789";
    Span span = span_from_text (text);
    Span parts [2];
    size_t count = span_split_n_by_char (span, parts, 2, '5');

    CHECK (count == 2);
    CHECK (parts[0].ptr == text);
    CHECK (parts[0].len == 4);
    CHECK (parts[1].ptr == text + 5);
    CHECK (parts[1].len == 4);
}

TESTER(span_split_n_by_char_4)
{
    const char *text = "123,567,9";
    Span span = span_from_text (text);
    Span parts [2];
    size_t count = span_split_n_by_char (span, parts, 2, ',');

    CHECK (count == 2);
    CHECK (parts[0].ptr == text);
    CHECK (parts[0].len == 3);
    CHECK (parts[1].ptr == text + 4);
    CHECK (parts[1].len == 5);
}

TESTER(span_split_n_by_char_5)
{
    const char *text = ",123,,567,9";
    Span span = span_from_text (text);
    Span parts [3];
    size_t count = span_split_n_by_char (span, parts, 3, ',');

    CHECK (count == 3);
    CHECK (parts[0].ptr == text + 1);
    CHECK (parts[0].len == 3);
    CHECK (parts[1].ptr == text + 6);
    CHECK (parts[1].len == 3);
    CHECK (parts[2].ptr == text + 10);
    CHECK (parts[2].len == 1);
}

TESTER(span_split_n_by_chars_1)
{
    Span span = SPAN_INIT;
    Span parts [2];
    size_t count = span_split_n_by_chars (span, parts, 2, "!?", 2);

    CHECK (count == 0);
}

TESTER(span_split_n_by_chars_2)
{
    const char *text = "123456789";
    Span span = span_from_text (text);
    Span parts [2];
    size_t count = span_split_n_by_chars (span, parts, 2, "!?", 2);

    CHECK (count == 1);
    CHECK (parts[0].ptr == text);
    CHECK (parts[0].len == 9);
}

TESTER(span_split_n_by_chars_3)
{
    const char *text = "123456789";
    Span span = span_from_text (text);
    Span parts [2];
    size_t count = span_split_n_by_chars (span, parts, 2, "5?", 2);

    CHECK (count == 2);
    CHECK (parts[0].ptr == text);
    CHECK (parts[0].len == 4);
    CHECK (parts[1].ptr == text + 5);
    CHECK (parts[1].len == 4);
}

TESTER(span_split_n_by_chars_4)
{
    const char *text = "123.567,9";
    Span span = span_from_text (text);
    Span parts [2];
    size_t count = span_split_n_by_chars (span, parts, 2, ",.", 2);

    CHECK (count == 2);
    CHECK (parts[0].ptr == text);
    CHECK (parts[0].len == 3);
    CHECK (parts[1].ptr == text + 4);
    CHECK (parts[1].len == 5);
}

TESTER(span_split_n_by_chars_5)
{
    const char *text = ",123,.567.9";
    Span span = span_from_text (text);
    Span parts [3];
    size_t count = span_split_n_by_chars (span, parts, 3, ",.", 2);

    CHECK (count == 3);
    CHECK (parts[0].ptr == text + 1);
    CHECK (parts[0].len == 3);
    CHECK (parts[1].ptr == text + 6);
    CHECK (parts[1].len == 3);
    CHECK (parts[2].ptr == text + 10);
    CHECK (parts[2].len == 1);
}

TESTER(span_starts_with_1)
{
    Span span1 = SPAN_INIT;
    Span span2 = span_from_text ("Hello");

    CHECK (!span_starts_with (span1, span2));
}

TESTER(span_starts_with_2)
{
    Span span1 = span_from_text ("Hello, world!");
    Span span2 = span_from_text ("Hello");

    CHECK (span_starts_with (span1, span2));
}

TESTER(span_starts_with_3)
{
    Span span1 = span_from_text ("HellO, world!");
    Span span2 = span_from_text ("Hello");

    CHECK (!span_starts_with (span1, span2));
}

TESTER(span_ends_with_1)
{
    Span span1 = SPAN_INIT;
    Span span2 = span_from_text ("world!");

    CHECK (!span_ends_with (span1, span2));
}

TESTER(span_ends_with_2)
{
    Span span1 = span_from_text ("Hello, world!");
    Span span2 = span_from_text ("world!");

    CHECK (span_ends_with (span1, span2));
}

TESTER(span_ends_with_3)
{
    Span span1 = span_from_text ("Hello, World!");
    Span span2 = span_from_text ("world!");

    CHECK (!span_ends_with (span1, span2));
}


