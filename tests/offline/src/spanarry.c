// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(span_array_init_1)
{
    SpanArray a1 = SPAN_ARRAY_INIT;

    CHECK (a1.len == 0);
    CHECK (a1.capacity == 0);
    CHECK (a1.ptr == NULL);
}

TESTER(span_array_create_1)
{
    SpanArray a1;

    span_array_create(&a1, 10);
    CHECK (a1.capacity == 10);
    CHECK (a1.len == 0);
    span_array_free (&a1);
}


TESTER(span_array_push_back_1)
{
    SpanArray a1;
    Span c1 = { BTEXT ("Hello"), 5 }, c2 = { BTEXT ("world"), 5 };
    Span g1, g2;

    span_array_create (&a1, 10);
    CHECK (a1.len == 0);
    span_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    span_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    g1 = span_array_get (&a1, 0);
    CHECK (g1.ptr == c1.ptr);
    g2 = span_array_get (&a1, 1);
    CHECK (g2.ptr == c2.ptr);
    span_array_free (&a1);
}


TESTER(span_array_push_front_1)
{
    SpanArray a1;
    Span c1 = { BTEXT ("Hello"), 5 }, c2 = { BTEXT ("world"), 5 };
    Span g1, g2;

    span_array_create (&a1, 10);
    CHECK (a1.len == 0);
    span_array_push_front (&a1, c1);
    CHECK (a1.len == 1);
    g1 = span_array_get (&a1, 0);
    CHECK (g1.ptr == c1.ptr);
    span_array_push_front (&a1, c2);
    g2 = span_array_get (&a1, 0);
    CHECK (g2.ptr == c2.ptr);
    span_array_free (&a1);
}

TESTER(span_array_pop_back_1)
{
    SpanArray a1;
    Span c1 = { BTEXT ("Hello"), 5 }, c2 = { BTEXT ("world"), 5 };
    Span g1, g2;

    span_array_create (&a1, 10);
    CHECK (a1.len == 0);
    span_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    span_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    g1 = span_array_pop_back (&a1);
    CHECK (a1.len == 1);
    CHECK (g1.ptr == c2.ptr);
    g2 = span_array_pop_back (&a1);
    CHECK (a1.len == 0);
    CHECK (g2.ptr == c1.ptr);
    span_array_free (&a1);
}

TESTER(span_array_pop_front_1)
{
    SpanArray a1;
    Span c1 = { BTEXT ("Hello"), 5 }, c2 = { BTEXT ("world"), 5 };
    Span g1, g2;

    span_array_create (&a1, 10);
    CHECK (a1.len == 0);
    span_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    span_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    g1 = span_array_pop_front (&a1);
    CHECK (a1.len == 1);
    CHECK (g1.ptr == c1.ptr);
    g2 = span_array_pop_front (&a1);
    CHECK (a1.len == 0);
    CHECK (g2.ptr == c2.ptr);
    span_array_free (&a1);
}

TESTER(span_array_set_1)
{
    SpanArray  a1;
    Span c1 = { BTEXT ("Hello"), 5 }, c2 = { BTEXT ("world"), 5 }, c3 = { BTEXT ("!"), 1 };
    Span g1, g2;

    span_array_create (&a1, 10);
    CHECK (a1.len == 0);
    span_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    span_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    span_array_set (&a1, 0, c3);
    g1 = span_array_get (&a1, 0);
    CHECK (g1.ptr == c3.ptr);
    g2 = span_array_get (&a1, 1);
    CHECK (g2.ptr == c2.ptr);
    span_array_free (&a1);
}

TESTER(span_array_clone_1)
{
    SpanArray a1, a2;
    Span c1 = { BTEXT ("Hello"), 5 }, c2 = { BTEXT ("world"), 5 };
    Span g1, g2;

    span_array_create (&a1, 10);
    CHECK (a1.len == 0);
    span_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    span_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    span_array_clone (&a2, &a1);
    CHECK (a1.len == a2.len);
    g1 = span_array_get (&a2, 0);
    CHECK (g1.ptr == c1.ptr);
    g2 = span_array_get (&a2, 1);
    CHECK (g2.ptr == c2.ptr);
    span_array_free (&a1);
    span_array_free (&a2);
}

TESTER(span_array_clone_2)
{
    SpanArray a1, a2;

    span_array_create (&a1, 10);
    CHECK (a1.len == 0);
    span_array_clone (&a2, &a1);
    CHECK (a1.len == a2.len);
    span_array_free (&a1);
    span_array_free (&a2);
}

TESTER(span_array_copy_1)
{
    SpanArray a1, a2;
    Span c1 = { BTEXT ("Hello"), 5 }, c2 = { BTEXT ("world"), 5 };
    Span g1, g2;

    span_array_create (&a1, 10);
    CHECK (a1.len == 0);
    span_array_create (&a2, 10);
    CHECK (a2.len == 0);
    span_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    span_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    span_array_copy (&a2, &a1);
    CHECK (a2.len == a1.len);
    g1 = span_array_get (&a2, 0);
    CHECK (g1.ptr == c1.ptr);
    g2 = span_array_get (&a2, 1);
    CHECK (g2.ptr == c2.ptr);
    span_array_free (&a1);
    span_array_free (&a2);
}

TESTER(span_array_concat_1)
{
    SpanArray a1, a2;
    Span c1 = { BTEXT ("Hello"), 5 }, c2 = { BTEXT ("world"), 5 };
    Span g1, g2;

    span_array_create (&a1, 10);
    CHECK (a1.len == 0);
    span_array_create (&a2, 10);
    CHECK (a2.len == 0);
    span_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    span_array_push_back (&a2, c2);
    CHECK (a2.len == 1);
    span_array_concat (&a1, &a2);
    CHECK (a1.len == 2);
    CHECK (a2.len == 1);
    g1 = span_array_get (&a1, 0);
    CHECK (g1.ptr == c1.ptr);
    g2 = span_array_get (&a1, 1);
    CHECK (g2.ptr == c2.ptr);
    span_array_free (&a1);
    span_array_free (&a2);
}

TESTER(span_array_shrink_1)
{
    SpanArray a1;
    Span c1 = { BTEXT ("Hello"), 5 }, c2 = { BTEXT ("world"), 5 };
    Span g1, g2;

    span_array_create (&a1, 10);
    CHECK (a1.len == 0);
    span_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    span_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    span_array_shrink (&a1);
    CHECK (a1.len == 2);
    g1 = span_array_get (&a1, 0);
    CHECK (g1.ptr == c1.ptr);
    g2 = span_array_get (&a1, 1);
    CHECK (g2.ptr == c2.ptr);
    span_array_free (&a1);
}

TESTER(span_array_shrink_2)
{
    SpanArray a1;

    span_array_create (&a1, 10);
    span_array_shrink (&a1);
    CHECK (a1.capacity == 0);
    span_array_free (&a1);
}

TESTER(span_array_truncate_1)
{
    SpanArray a1;
    Span c1 = { BTEXT ("Hello"), 5 }, c2 = { BTEXT ("world"), 5 };
    Span g1;

    span_array_create (&a1, 10);
    CHECK (a1.len == 0);
    span_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    span_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    span_array_truncate (&a1, 1);
    CHECK (a1.len == 1);
    g1 = span_array_get (&a1, 0);
    CHECK (g1.ptr == c1.ptr);
    span_array_truncate (&a1, 0);
    CHECK (a1.len == 0);
    span_array_free (&a1);
}

TESTER(span_array_grow_1)
{
    SpanArray a1;

    span_array_create (&a1, 10);
    CHECK (a1.capacity == 10);
    span_array_grow (&a1, 100);
    CHECK (a1.capacity >= 100);
    span_array_free (&a1);
}

TESTER(span_array_grow_2)
{
    SpanArray a1;

    span_array_create (&a1, 10);
    CHECK (a1.capacity == 10);
    span_array_grow (&a1, 11);
    CHECK (a1.capacity >= 11);
    span_array_free (&a1);
}

TESTER(span_array_grow_3)
{
    SpanArray a1;
    Span c1 = { BTEXT ("Hello"), 5 }, c2 = { BTEXT ("world"), 5 };

    span_array_create (&a1, 10);
    CHECK (a1.capacity == 10);
    span_array_push_back (&a1, c1);
    span_array_push_back (&a1, c2);
    span_array_grow (&a1, 11);
    CHECK (a1.capacity >= 11);
    CHECK (a1.len == 2);
    span_array_free (&a1);
}
