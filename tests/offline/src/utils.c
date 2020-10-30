// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(min_int32_1)
{
    CHECK (min_int32 (123, 321) == 123);
    CHECK (min_int32 (321, 123) == 123);
    CHECK (min_int32 (123, 123) == 123);
}

TESTER(max_int32_1)
{
    CHECK (max_int32 (123, 321) == 321);
    CHECK (max_int32 (321, 123) == 321);
    CHECK (max_int32 (123, 123) == 123);
}

TESTER(min_uint32_1)
{
    CHECK (min_uint32 (123u, 321u) == 123u);
    CHECK (min_uint32 (321u, 123u) == 123u);
    CHECK (min_uint32 (123u, 123u) == 123u);
}

TESTER(max_uint32_1)
{
    CHECK (max_uint32 (123u, 321u) == 321u);
    CHECK (max_uint32 (321u, 123u) == 321u);
    CHECK (max_uint32 (123u, 123u) == 123u);
}

TESTER(min_int64_1)
{
    CHECK (min_int64 (123L, 641L) == 123L);
    CHECK (min_int64 (641L, 123L) == 123L);
    CHECK (min_int64 (123L, 123L) == 123L);
}

TESTER(max_int64_1)
{
    CHECK (max_int64 (123L, 641L) == 641L);
    CHECK (max_int64 (641L, 123L) == 641L);
    CHECK (max_int64 (123L, 123L) == 123L);
}

TESTER(min_uint64_1)
{
    CHECK (min_uint64 (123ul, 641ul) == 123ul);
    CHECK (min_uint64 (641ul, 123ul) == 123ul);
    CHECK (min_uint64 (123ul, 123ul) == 123ul);
}

TESTER(max_uint64_1)
{
    CHECK (max_uint64 (123ul, 641ul) == 641ul);
    CHECK (max_uint64 (641ul, 123ul) == 641ul);
    CHECK (max_uint64 (123ul, 123ul) == 123ul);
}

TESTER(min_size_t_1)
{
    CHECK (min_size_t (123, 641) == 123);
    CHECK (min_size_t (641, 123) == 123);
    CHECK (min_size_t (123, 123) == 123);
}

TESTER(max_size_t_1)
{
    CHECK (max_size_t (123, 641) == 641);
    CHECK (max_size_t (641, 123) == 641);
    CHECK (max_size_t (123, 123) == 123);
}

TESTER(choose_string_1)
{
    const am_byte *first = CBTEXT ("Hello"), *second = CBTEXT ("World");

    CHECK (choose_string (first, second) == first);

    first = NULL;

    CHECK (choose_string (first, second) == second);
}

TESTER(choose_string_2)
{
    const am_byte *first = CBTEXT (""), *second = CBTEXT (""), *third = CBTEXT ("Here");

    CHECK (choose_string (first, second, third) == third);
}

TESTER(choose_span_1)
{
    Span first = TEXT_SPAN ("Hello"), second = TEXT_SPAN ("World");
    Span choosen;

    choosen = choose_span (first, second);
    CHECK (span_compare (choosen, first) == 0);

    first.end = first.start;

    choosen = choose_span (first, second);
    CHECK (span_compare (choosen, second) == 0);
}

TESTER(choose_buffer_1)
{
    Buffer first, second;
    Buffer *choosen;

    buffer_static (&first, BTEXT ("Hello"), 5);
    buffer_static (&second, BTEXT ("World"), 5);

    choosen = choose_buffer (&first, &second);
    CHECK (choosen == &first);

    first.current = first.start;
    choosen = choose_buffer (&first, &second);
    CHECK (choosen == &second);

    choosen = choose_buffer (NULL, &second);
    CHECK (choosen == &second);
}

TESTER(mersenne_get_1)
{
    am_uint32 number1, number2, index;

    number1 = mersenne_get();
    for (index = 0; index < 256; ++index) {
        number2 = mersenne_get();
        CHECK (number1 != number2);
        number1 = number2;
    }
}

TESTER(random_get_1)
{
    am_uint32 number1, number2, index;

    number1 = random_get();
    for (index = 0; index < 256; ++index) {
        number2 = random_get();
        CHECK (number1 != number2);
        number1 = number2;
    }
}
