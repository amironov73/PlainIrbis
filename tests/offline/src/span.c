// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(span_init_1)
{
    Span span = SPAN_INIT;

    CHECK (span.start == NULL);
    CHECK (span.end   == NULL);
    CHECK (span_verify (span));
    span_assert (span);
}

TESTER(span_init_2)
{
    Span span = span_init (NULL, 0);

    CHECK (span.start == NULL);
    CHECK (span.end   == NULL);
    CHECK (span_verify (span));
    span_assert (span);
}

TESTER(span_init_3)
{
    const am_byte data[] = { 1, 2, 3 };
    Span span = span_init (data, sizeof (data));

    CHECK (span.start == data);
    CHECK (span_length (span) == sizeof (data));
    CHECK (span_verify (span));
    span_assert (span);
}

TESTER(span_null_1)
{
    Span span = span_null();

    CHECK (span.start == NULL);
    CHECK (span.end   == NULL);
    CHECK (span_is_empty (span));
    CHECK (span_verify (span));
    span_assert (span);
}

TESTER(span_from_text_1)
{
    const am_byte *text = CBTEXT ("Hello");
    Span span = TEXT_SPAN (text);

    CHECK (span.start == text);
    CHECK (span_length (span) == 5);
    CHECK (span_verify (span));
    span_assert (span);
}

TESTER(span_trim_start_1)
{
    Span span1 = SPAN_INIT;
    Span span2 = span_trim_start (span1);

    CHECK (span2.start == NULL);
    CHECK (span2.end   == NULL);
}

TESTER(span_trim_start_2)
{
    const am_byte *text = CBTEXT ("Hello");
    Span span1 = TEXT_SPAN (text);
    Span span2 = span_trim_start (span1);

    CHECK (span2.start == text);
    CHECK (span_length (span2) == 5);
}

TESTER(span_trim_start_3)
{
    const am_byte *text = CBTEXT ("  Hello");
    Span span1 = TEXT_SPAN (text);
    Span span2 = span_trim_start (span1);

    CHECK (span2.start == text + 2);
    CHECK (span_length (span2) == 5);
}

TESTER(span_trim_end_1)
{
    Span span1 = SPAN_INIT;
    Span span2 = span_trim_end (span1);

    CHECK (span2.start == NULL);
    CHECK (span2.end   == NULL);
}

TESTER(span_trim_end_2)
{
    const am_byte *text = CBTEXT ("Hello");
    Span span1 = TEXT_SPAN (text);
    Span span2 = span_trim_end (span1);

    CHECK (span2.start == text);
    CHECK (span_length (span2) == 5);
}

TESTER(span_trim_end_3)
{
    const am_byte *text = CBTEXT ("Hello  ");
    Span span1 = TEXT_SPAN (text);
    Span span2 = span_trim_end (span1);

    CHECK (span2.start == text);
    CHECK (span_length (span2) == 5);
}

TESTER(span_trim_1)
{
    Span span1 = SPAN_INIT;
    Span span2 = span_trim (span1);

    CHECK (span2.start == NULL);
    CHECK (span2.end   == NULL);
}

TESTER(span_trim_2)
{
    const am_byte *text = CBTEXT ("Hello");
    Span span1 = TEXT_SPAN (text);
    Span span2 = span_trim (span1);

    CHECK (span2.start == text);
    CHECK (span_length (span2) == 5);
}

TESTER(span_trim_3)
{
    const am_byte *text = CBTEXT ("  Hello  ");
    Span span1 = TEXT_SPAN (text);
    Span span2 = span_trim (span1);

    CHECK (span2.start == text + 2);
    CHECK (span_length (span2) == 5);
}

TESTER(span_to_int32_1)
{
    Span span = SPAN_INIT;
    am_int32 value = span_to_int32 (span);

    CHECK (value == 0);
}

TESTER(span_to_int32_2)
{
    Span span = TEXT_SPAN ("123456789");
    am_int32 value = span_to_int32 (span);

    CHECK (value == 123456789);
}

TESTER(span_to_int32_3)
{
    Span span = TEXT_SPAN ("-123456789");
    am_int32 value = span_to_int32 (span);

    CHECK (value == -123456789);
}

TESTER(span_to_int32_4)
{
    Span span = TEXT_SPAN ("--123456789");
    am_int32 value = span_to_int32 (span);

    CHECK (value == 123456789);
}

TESTER(span_to_int32_5)
{
    Span span = TEXT_SPAN ("+123456789");
    am_int32 value = span_to_int32 (span);

    CHECK (value == 123456789);
}

TESTER(span_to_int32_6)
{
    Span span = TEXT_SPAN ("-+123456789");
    am_int32 value = span_to_int32 (span);

    CHECK (value == -123456789);
}

TESTER(span_to_int64_1)
{
    Span span = SPAN_INIT;
    am_int64 value = span_to_int64 (span);

    CHECK (value == MAGNA_INT64 (0));
}

TESTER(span_to_int64_2)
{
    Span span = TEXT_SPAN ("123456789");
    am_int64 value = span_to_int64 (span);

    CHECK (value == MAGNA_INT64 (123456789));
}

TESTER(span_to_int64_3)
{
    Span span = TEXT_SPAN ("-123456789");
    am_int64 value = span_to_int64 (span);

    CHECK (value == MAGNA_INT64 (-123456789));
}

TESTER(span_to_int64_4)
{
    Span span = TEXT_SPAN ("--123456789");
    am_int64 value = span_to_int64 (span);

    CHECK (value == MAGNA_INT64 (123456789));
}

TESTER(span_to_int64_5)
{
    Span span = TEXT_SPAN ("+123456789");
    am_int64 value = span_to_int64 (span);

    CHECK (value == MAGNA_INT64 (123456789));
}

TESTER(span_to_int64_6)
{
    Span span = TEXT_SPAN ("-+123456789");
    am_int64 value = span_to_int64 (span);

    CHECK (value == MAGNA_INT64 (-123456789));
}

TESTER(span_to_uint_32_1)
{
    Span span = SPAN_INIT;
    am_uint32 value = span_to_uint32 (span);

    CHECK (value == 0u);
}

TESTER(span_to_uint_32_2)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);
    am_uint32 value = span_to_uint32(span);

    CHECK (value == 123456789u);
}

TESTER(span_to_uint_64_1)
{
    Span span = SPAN_INIT;
    am_uint64 value = span_to_uint64(span);

    CHECK (value == MAGNA_UINT64 (0));
}

TESTER(span_to_uint_64_2)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);
    am_uint64 value = span_to_uint64(span);

    CHECK (value == MAGNA_UINT64 (123456789));
}

TESTER(span_toupper_1)
{
    am_byte *hello = BTEXT (str_dup ("Hello"));
    Span span = TEXT_SPAN (hello);
    span_toupper (span);

    CHECK (strcmp (CCTEXT (hello), "HELLO") == 0);

    mem_free (hello);
}

TESTER(span_tolower_1)
{
    am_byte *hello = BTEXT (str_dup ("Hello"));
    Span span = TEXT_SPAN (hello);
    span_tolower (span);

    CHECK (strcmp (CCTEXT (hello), "hello") == 0);

    free (hello);
}

TESTER(span_is_empty_1)
{
    Span span = SPAN_INIT;

    CHECK (span_is_empty (span));
}

TESTER(span_is_empty_2)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);

    CHECK (!span_is_empty (span));
}

TESTER(span_find_byte_1)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);
    am_byte *found = span_find_byte (span, '?');

    CHECK (!found);

    found = span_find_byte (span, '5');
    CHECK (*found == '5');
}

TESTER(span_fill_1)
{
    am_byte *hello = BTEXT (str_dup ("Hello"));
    Span span = TEXT_SPAN (hello);
    span_fill (span, '0');

    CHECK (strcmp (CCTEXT (hello), "00000") == 0);

    free (hello);
}

TESTER(span_index_of_1)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);
    ssize_t found = span_index_of (span, '?');

    CHECK (found < 0);

    found = span_index_of (span, '5');

    CHECK (found == 4);
}

TESTER(span_last_index_of_1)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);
    ssize_t found = span_last_index_of (span, '?');

    CHECK (found < 0);

    found = span_last_index_of (span, '5');
    CHECK (found == 4);
}

TESTER(span_slice_1)
{
    Span span1 = SPAN_INIT;
    Span span2 = span_slice (span1, 5, 10);

    CHECK (span_is_empty (span2));
}

TESTER(span_slice_2)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span1 = TEXT_SPAN (text);
    Span span2 = span_slice (span1, 2, 2);

    CHECK (span2.start[0] == '3');
    CHECK (span_length (span2) == 2);
}

TESTER(span_slice_3)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span1 = TEXT_SPAN (text);
    Span span2 = span_slice (span1, 7, 12);

    CHECK (span2.start[0] == '8');
    CHECK (span_length (span2) == 2);
}

TESTER(span_slice_4)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span1 = TEXT_SPAN (text);
    Span span2 = span_slice (span1, 3, -1);

    CHECK (span2.start[0] == '4');
    CHECK (span_length (span2) == 6);
}

TESTER(span_to_string_1)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span1 = TEXT_SPAN (text);
    Span span2 = span_slice (span1, 7, 12);
    am_byte *str = span_to_string (span2);

    CHECK (strcmp (CCTEXT (str), "89") == 0);

    free (str);
}

TESTER(span_to_vector_1)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span1 = TEXT_SPAN (text);
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
    const am_byte *text1 = CBTEXT ("123456789"), *text2 = CBTEXT ("qwertyiop");
    Span span1 = TEXT_SPAN (text1);
    Span span2 = TEXT_SPAN (text2);

    CHECK (span_compare (span1, span2) < 0);
}

TESTER(span_compare_3)
{
    const am_byte *text1 = CBTEXT ("123456789");
    Span span1 = TEXT_SPAN (text1);
    Span span2 = TEXT_SPAN (text1);

    CHECK (span_compare (span1, span2) == 0);
}

TESTER(span_compare_4)
{
    const am_byte *text1 = CBTEXT ("1234"), *text2 = CBTEXT ("123456789");
    Span span1 = TEXT_SPAN (text1);
    Span span2 = TEXT_SPAN (text2);

    CHECK (span_compare (span1, span2) < 0);
}

TESTER(span_compare_5)
{
    const am_byte *text1 = CBTEXT ("123456789"), *text2 = CBTEXT ("1234");
    Span span1 = TEXT_SPAN (text1);
    Span span2 = TEXT_SPAN (text2);

    CHECK (span_compare (span1, span2) > 0);
}

TESTER(span_compare_ignore_case_1)
{
    Span span = SPAN_INIT;

    CHECK (span_compare_ignore_case (span, span) == 0);
}

TESTER(span_compare_ignore_case_2)
{
    const am_byte *text1 = CBTEXT ("QWERTYIOP"), *text2 = CBTEXT ("wertyiop");
    Span span1 = TEXT_SPAN (text1);
    Span span2 = TEXT_SPAN (text2);

    CHECK (span_compare_ignore_case (span1, span2) < 0);
}

TESTER(span_compare_ignore_case_3)
{
    const am_byte *text1 = CBTEXT ("qwertyiop"), *text2 = CBTEXT ("QWERTYIOP");
    Span span1 = TEXT_SPAN (text1);
    Span span2 = TEXT_SPAN (text2);

    CHECK (span_compare_ignore_case (span1, span2) == 0);
}

TESTER(span_compare_ignore_case_4)
{
    const am_byte *text1 = CBTEXT ("qwer"), *text2 = CBTEXT ("QWERTYIOP");
    Span span1 = TEXT_SPAN (text1);
    Span span2 = TEXT_SPAN (text2);

    CHECK (span_compare_ignore_case (span1, span2) < 0);
}

TESTER(span_compare_ignore_case_5)
{
    const am_byte *text1 = CBTEXT ("wer"), *text2 = CBTEXT ("QWERTYIOP");
    Span span1 = TEXT_SPAN (text1);
    Span span2 = TEXT_SPAN (text2);

    CHECK (span_compare_ignore_case (span1, span2) > 0);
}

TESTER(span_compare_ignore_case_6)
{
    const am_byte *text1 = CBTEXT ("QWERTY"), *text2 = CBTEXT ("qwert");
    Span span1 = TEXT_SPAN (text1);
    Span span2 = TEXT_SPAN (text2);

    CHECK (span_compare_ignore_case (span1, span2) > 0);
}

TESTER(span_contains_1)
{
    Span span = SPAN_INIT;

    CHECK (!span_contains (span, '?'));
}

TESTER(span_contains_2)
{
    Span span = TEXT_SPAN ("123456789");

    CHECK (!span_contains (span, '?'));
}

TESTER(span_contains_3)
{
    Span span = TEXT_SPAN ("123456789");

    CHECK (span_contains (span, '5'));
}

TESTER(span_split_by_char_1)
{
    Span span = SPAN_INIT;
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_char (span, &parts, '?');

    CHECK (rc);
    CHECK (parts.len == 0);

    span_array_destroy (&parts);
}

TESTER(span_split_by_char_2)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_char (span, &parts, '?');

    CHECK (rc);
    CHECK (parts.len == 1);
    CHECK (parts.ptr[0].start == text);
    CHECK (span_length (parts.ptr[0]) == 9);

    span_array_destroy (&parts);
}

TESTER(span_split_by_char_3)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_char (span, &parts, '5');

    CHECK (rc);
    CHECK (parts.len == 2);
    CHECK (parts.ptr[0].start == text);
    CHECK (span_length (parts.ptr[0]) == 4);
    CHECK (parts.ptr[1].start == text + 5);
    CHECK (span_length (parts.ptr[1]) == 4);

    span_array_destroy (&parts);
}

TESTER(span_split_by_char_4)
{
    const am_byte *text = CBTEXT ("123,567,9");
    Span span = TEXT_SPAN (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_char (span, &parts, ',');

    CHECK (rc);
    CHECK (parts.len == 3);
    CHECK (parts.ptr[0].start == text);
    CHECK (span_length (parts.ptr[0]) == 3);
    CHECK (parts.ptr[1].start == text + 4);
    CHECK (span_length (parts.ptr[1]) == 3);
    CHECK (parts.ptr[2].start == text + 8);
    CHECK (span_length (parts.ptr[2]) == 1);

    span_array_destroy (&parts);
}

TESTER(span_split_by_char_5)
{
    const am_byte *text = CBTEXT (",123,,567,9");
    Span span = TEXT_SPAN (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_char (span, &parts, ',');

    CHECK (rc);
    CHECK (parts.len == 3);
    CHECK (parts.ptr[0].start == text + 1);
    CHECK (span_length (parts.ptr[0]) == 3);
    CHECK (parts.ptr[1].start == text + 6);
    CHECK (span_length (parts.ptr[1]) == 3);
    CHECK (parts.ptr[2].start == text + 10);
    CHECK (span_length (parts.ptr[2]) == 1);

    span_array_destroy (&parts);
}

TESTER(span_split_by_chars_1)
{
    Span span = SPAN_INIT;
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_chars (span, &parts, CBTEXT ("!?"), 2);

    CHECK (rc);
    CHECK (parts.len == 0);

    span_array_destroy (&parts);
}

TESTER(span_split_by_chars_2)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_chars (span, &parts, CBTEXT ("!?"), 2);

    CHECK (rc);
    CHECK (parts.len == 1);
    CHECK (parts.ptr[0].start == text);
    CHECK (span_length (parts.ptr[0]) == 9);

    span_array_destroy (&parts);
}

TESTER(span_split_by_chars_3)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_chars (span, &parts, CBTEXT ("5?"), 2);

    CHECK (rc);
    CHECK (parts.len == 2);
    CHECK (parts.ptr[0].start == text);
    CHECK (span_length (parts.ptr[0]) == 4);
    CHECK (parts.ptr[1].start == text + 5);
    CHECK (span_length (parts.ptr[1]) == 4);

    span_array_destroy (&parts);
}

TESTER(span_split_by_chars_4)
{
    const am_byte *text = CBTEXT ("123.567,9");
    Span span = TEXT_SPAN (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_chars (span, &parts,CBTEXT (",."), 2);

    CHECK (rc);
    CHECK (parts.len == 3);
    CHECK (parts.ptr[0].start == text);
    CHECK (span_length (parts.ptr[0]) == 3);
    CHECK (parts.ptr[1].start == text + 4);
    CHECK (span_length (parts.ptr[1]) == 3);
    CHECK (parts.ptr[2].start == text + 8);
    CHECK (span_length (parts.ptr[2]) == 1);

    span_array_destroy (&parts);
}

TESTER(span_split_by_chars_5)
{
    const am_byte *text = CBTEXT (",123,.567.9");
    Span span = TEXT_SPAN (text);
    SpanArray parts = SPAN_ARRAY_INIT;
    am_bool rc = span_split_by_chars (span, &parts, CBTEXT (",."), 2);

    CHECK (rc);
    CHECK (parts.len == 3);
    CHECK (parts.ptr[0].start == text + 1);
    CHECK (span_length (parts.ptr[0]) == 3);
    CHECK (parts.ptr[1].start == text + 6);
    CHECK (span_length (parts.ptr[1]) == 3);
    CHECK (parts.ptr[2].start == text + 10);
    CHECK (span_length (parts.ptr[2]) == 1);

    span_array_destroy (&parts);
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
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);
    Span parts [2];
    size_t count = span_split_n_by_char (span, parts, 2, '?');

    CHECK (count == 1);
    CHECK (parts[0].start == text);
    CHECK (span_length (parts[0]) == 9);
}

TESTER(span_split_n_by_char_3)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);
    Span parts [2];
    size_t count = span_split_n_by_char (span, parts, 2, '5');

    CHECK (count == 2);
    CHECK (parts[0].start == text);
    CHECK (span_length (parts[0]) == 4);
    CHECK (parts[1].start == text + 5);
    CHECK (span_length (parts[1]) == 4);
}

TESTER(span_split_n_by_char_4)
{
    const am_byte *text = CBTEXT ("123,567,9");
    Span span = TEXT_SPAN (text);
    Span parts [2];
    size_t count = span_split_n_by_char (span, parts, 2, ',');

    CHECK (count == 2);
    CHECK (parts[0].start == text);
    CHECK (span_length (parts[0]) == 3);
    CHECK (parts[1].start == text + 4);
    CHECK (span_length (parts[1]) == 5);
}

TESTER(span_split_n_by_char_5)
{
    const am_byte *text = CBTEXT (",123,,567,9");
    Span span = TEXT_SPAN (text);
    Span parts [3];
    size_t count = span_split_n_by_char (span, parts, 3, ',');

    CHECK (count == 3);
    CHECK (parts[0].start == text + 1);
    CHECK (span_length (parts[0]) == 3);
    CHECK (parts[1].start == text + 6);
    CHECK (span_length (parts[1]) == 3);
    CHECK (parts[2].start == text + 10);
    CHECK (span_length (parts[2]) == 1);
}

TESTER(span_split_n_by_chars_1)
{
    Span span = SPAN_INIT;
    Span parts [2];
    size_t count = span_split_n_by_chars (span, parts, 2, CBTEXT ("!?"), 2);

    CHECK (count == 0);
}

TESTER(span_split_n_by_chars_2)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);
    Span parts [2];
    size_t count = span_split_n_by_chars (span, parts, 2, CBTEXT ("!?"), 2);

    CHECK (count == 1);
    CHECK (parts[0].start == text);
    CHECK (span_length (parts[0]) == 9);
}

TESTER(span_split_n_by_chars_3)
{
    const am_byte *text = CBTEXT ("123456789");
    Span span = TEXT_SPAN (text);
    Span parts [2];
    size_t count = span_split_n_by_chars (span, parts, 2, CBTEXT ("5?"), 2);

    CHECK (count == 2);
    CHECK (parts[0].start == text);
    CHECK (span_length (parts[0]) == 4);
    CHECK (parts[1].start == text + 5);
    CHECK (span_length (parts[1]) == 4);
}

TESTER(span_split_n_by_chars_4)
{
    const am_byte *text = CBTEXT ("123.567,9");
    Span span = TEXT_SPAN (text);
    Span parts [2];
    size_t count = span_split_n_by_chars (span, parts, 2, CBTEXT (",."), 2);

    CHECK (count == 2);
    CHECK (parts[0].start == text);
    CHECK (span_length (parts[0]) == 3);
    CHECK (parts[1].start == text + 4);
    CHECK (span_length (parts[1]) == 5);
}

TESTER(span_split_n_by_chars_5)
{
    const am_byte *text = CBTEXT (",123,.567.9");
    Span span = TEXT_SPAN (text);
    Span parts [3];
    size_t count = span_split_n_by_chars (span, parts, 3, CBTEXT (",."), 2);

    CHECK (count == 3);
    CHECK (parts[0].start == text + 1);
    CHECK (span_length (parts[0]) == 3);
    CHECK (parts[1].start == text + 6);
    CHECK (span_length (parts[1]) == 3);
    CHECK (parts[2].start == text + 10);
    CHECK (span_length (parts[2]) == 1);
}

TESTER(span_starts_with_1)
{
    Span span1 = SPAN_INIT;
    Span span2 = TEXT_SPAN ("Hello");

    CHECK (!span_starts_with (span1, span2));
}

TESTER(span_starts_with_2)
{
    Span span1 = TEXT_SPAN ("Hello, world!");
    Span span2 = TEXT_SPAN ("Hello");

    CHECK (span_starts_with (span1, span2));
}

TESTER(span_starts_with_3)
{
    Span span1 = TEXT_SPAN ("HellO, world!");
    Span span2 = TEXT_SPAN ("Hello");

    CHECK (!span_starts_with (span1, span2));
}

TESTER(span_ends_with_1)
{
    Span span1 = SPAN_INIT;
    Span span2 = TEXT_SPAN ("world!");

    CHECK (!span_ends_with (span1, span2));
}

TESTER(span_ends_with_2)
{
    Span span1 = TEXT_SPAN ("Hello, world!");
    Span span2 = TEXT_SPAN ("world!");

    CHECK (span_ends_with (span1, span2));
}

TESTER(span_ends_with_3)
{
    Span span1 = TEXT_SPAN ("Hello, World!");
    Span span2 = TEXT_SPAN ("world!");

    CHECK (!span_ends_with (span1, span2));
}

TESTER(span_hex_to_uint64_1)
{
    Span span = SPAN_INIT;
    am_uint64 value = span_hex_to_uint64 (span);

    CHECK (value == MAGNA_UINT64 (0));
}

TESTER(span_hex_to_uint64_2)
{
    Span span = TEXT_SPAN ("0123456789ABC");
    am_uint64 value = span_hex_to_uint64 (span);

    CHECK (value == MAGNA_UINT64 (0x0123456789ABC));
}

TESTER(span_verify_1)
{
    Span span;

    span.start = (am_byte*) 0x2000;
    span.end   = (am_byte*) 0x1000;

    CHECK (!span_verify (span));
}

TESTER(span_count_1)
{
    Span span = span_null();

    CHECK (span_count (span, '1') == 0);
}

TESTER(span_count_2)
{
    Span span = TEXT_SPAN ("0000000");

    CHECK (span_count (span, '1') == 0);
}

TESTER(span_count_3)
{
    Span span = TEXT_SPAN ("0001000");

    CHECK (span_count (span, '1') == 1);
}

TESTER(span_count_4)
{
    Span span = TEXT_SPAN ("1111111");

    CHECK (span_count (span, '1') == 7);
}

TESTER(span_remove_prefix_1)
{
    Span span1 = TEXT_SPAN ("Hello, world");
    Span span2 = TEXT_SPAN ("(");
    Span span3 = span_remove_prefix (span1, span2);

    CHECK (span_compare (span1, span3) == 0);

}

TESTER(span_remove_prefix_2)
{
    Span span1 = TEXT_SPAN ("(Hello, world");
    Span span2 = TEXT_SPAN ("(");
    Span span3 = span_remove_prefix (span1, span2);

    CHECK (span_compare (span3, TEXT_SPAN ("Hello, world")) == 0);

}

TESTER(span_remove_suffix_1)
{
    Span span1 = TEXT_SPAN ("Hello, world");
    Span span2 = TEXT_SPAN (")");
    Span span3 = span_remove_suffix (span1, span2);

    CHECK (span_compare (span1, span3) == 0);

}

TESTER(span_remove_suffix_2)
{
    Span span1 = TEXT_SPAN ("Hello, world)");
    Span span2 = TEXT_SPAN (")");
    Span span3 = span_remove_suffix (span1, span2);

    CHECK (span_compare (span3, TEXT_SPAN ("Hello, world")) == 0);

}

TESTER(span_remove_prefix_and_suffix_1)
{
    Span span1 = TEXT_SPAN ("Hello, world");
    Span span2 = TEXT_SPAN ("(");
    Span span3 = TEXT_SPAN (")");
    Span span4 = span_remove_prefix_and_suffix (span1, span2, span3);

    CHECK (span_compare (span1, span4) == 0);
}

TESTER(span_remove_prefix_and_suffix_2)
{
    Span span1 = TEXT_SPAN ("(Hello, world");
    Span span2 = TEXT_SPAN ("(");
    Span span3 = TEXT_SPAN (")");
    Span span4 = span_remove_prefix_and_suffix (span1, span2, span3);

    CHECK (span_compare (span1, span4) == 0);
}

TESTER(span_remove_prefix_and_suffix_3)
{
    Span span1 = TEXT_SPAN ("Hello, world)");
    Span span2 = TEXT_SPAN ("(");
    Span span3 = TEXT_SPAN (")");
    Span span4 = span_remove_prefix_and_suffix (span1, span2, span3);

    CHECK (span_compare (span1, span4) == 0);
}

TESTER(span_remove_prefix_and_suffix_4)
{
    Span span1 = TEXT_SPAN ("(Hello, world)");
    Span span2 = TEXT_SPAN ("(");
    Span span3 = TEXT_SPAN (")");
    Span span4 = span_remove_prefix_and_suffix (span1, span2, span3);

    CHECK (span_compare (span4, TEXT_SPAN ("Hello, world")) == 0);
}

TESTER(span_remove_prefix_and_suffix_5)
{
    Span span1 = TEXT_SPAN ("+Hello, world+");
    Span span2 = TEXT_SPAN ("+");
    Span span3 = TEXT_SPAN ("+");
    Span span4 = span_remove_prefix_and_suffix (span1, span2, span3);

    CHECK (span_compare (span4, TEXT_SPAN ("Hello, world")) == 0);
}

TESTER(span_remove_prefix_and_suffix_6)
{
    Span span1 = TEXT_SPAN ("+");
    Span span2 = TEXT_SPAN ("+");
    Span span3 = TEXT_SPAN ("+");
    Span span4 = span_remove_prefix_and_suffix (span1, span2, span3);

    CHECK (span_compare (span1, span4) == 0);
}
