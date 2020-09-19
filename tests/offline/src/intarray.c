// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(int32_array_init_1)
{
    Int32Array a1 = INT32_ARRAY_INIT;

    CHECK (a1.len == 0);
    CHECK (a1.capacity == 0);
    CHECK (a1.ptr == NULL);
}

TESTER(int32_array_create_1)
{
    Int32Array a1;

    int32_array_create(&a1, 10);
    CHECK (a1.capacity == 10);
    CHECK (a1.len == 0);
    int32_array_free (&a1);
}

TESTER(int32_array_push_back_1)
{
    Int32Array a1;
    am_int32 c1 = 1, c2 = 2;
    am_int32 g1, g2;

    int32_array_create (&a1, 10);
    CHECK (a1.len == 0);
    int32_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    int32_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    g1 = int32_array_get (&a1, 0);
    CHECK (g1 == c1);
    g2 = int32_array_get (&a1, 1);
    CHECK (g2 == c2);
    int32_array_free (&a1);
}

TESTER(int32_array_push_front_1)
{
    Int32Array a1;
    am_int32 c1 = 1, c2 = 2;
    am_int32 g1, g2;

    int32_array_create (&a1, 10);
    CHECK (a1.len == 0);
    int32_array_push_front (&a1, c1);
    CHECK (a1.len == 1);
    g1 = int32_array_get (&a1, 0);
    CHECK (g1 == c1);
    int32_array_push_front (&a1, c2);
    g2 = int32_array_get (&a1, 0);
    CHECK (g2 == c2);
    int32_array_free (&a1);
}

TESTER(int32_array_pop_back_1)
{
    Int32Array a1;
    am_int32 c1 = 1, c2 = 2;
    am_int32 g1, g2;

    int32_array_create (&a1, 10);
    CHECK (a1.len == 0);
    int32_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    int32_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    g1 = int32_array_pop_back (&a1);
    CHECK (a1.len == 1);
    CHECK (g1 == c2);
    g2 = int32_array_pop_back (&a1);
    CHECK (a1.len == 0);
    CHECK (g2 == c1);
    int32_array_free (&a1);
}

TESTER(int32_array_pop_front_1)
{
    Int32Array a1;
    am_int32 c1 = 1, c2 = 2;
    am_int32 g1, g2;

    int32_array_create (&a1, 10);
    CHECK (a1.len == 0);
    int32_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    int32_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    g1 = int32_array_pop_front (&a1);
    CHECK (a1.len == 1);
    CHECK (g1 == c1);
    g2 = int32_array_pop_front (&a1);
    CHECK (a1.len == 0);
    CHECK (g2 == c2);
    int32_array_free (&a1);
}

TESTER(int32_array_set_1)
{
    Int32Array  a1;
    am_int32 c1 = 1, c2 = 2, c3 = 3;
    am_int32 g1, g2;

    int32_array_create (&a1, 10);
    CHECK (a1.len == 0);
    int32_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    int32_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    int32_array_set (&a1, 0, c3);
    g1 = int32_array_get (&a1, 0);
    CHECK (g1 == c3);
    g2 = int32_array_get (&a1, 1);
    CHECK (g2 == c2);
    int32_array_free (&a1);
}

TESTER(int32_array_clone_1)
{
    Int32Array a1, a2;
    am_int32 c1 = 1, c2 = 2;
    am_int32 g1, g2;

    int32_array_create (&a1, 10);
    CHECK (a1.len == 0);
    int32_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    int32_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    int32_array_clone (&a2, &a1);
    CHECK (a1.len == a2.len);
    g1 = int32_array_get (&a2, 0);
    CHECK (g1 == c1);
    g2 = int32_array_get (&a2, 1);
    CHECK (g2 == c2);
    int32_array_free (&a1);
    int32_array_free (&a2);
}

TESTER(int32_array_clone_2)
{
    Int32Array a1, a2;

    int32_array_create (&a1, 10);
    CHECK (a1.len == 0);
    int32_array_clone (&a2, &a1);
    CHECK (a1.len == a2.len);
    int32_array_free (&a1);
    int32_array_free (&a2);
}

TESTER(int32_array_copy_1)
{
    Int32Array a1, a2;
    am_int32 c1 = 1, c2 = 2;
    am_int32 g1, g2;

    int32_array_create (&a1, 10);
    CHECK (a1.len == 0);
    int32_array_create (&a2, 10);
    CHECK (a2.len == 0);
    int32_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    int32_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    int32_array_copy (&a2, &a1);
    CHECK (a2.len == a1.len);
    g1 = int32_array_get (&a2, 0);
    CHECK (g1 == c1);
    g2 = int32_array_get (&a2, 1);
    CHECK (g2 == c2);
    int32_array_free (&a1);
    int32_array_free (&a2);
}

TESTER(int32_array_concat_1)
{
    Int32Array a1, a2;
    am_int32 c1 = 1, c2 = 2;
    am_int32 g1, g2;

    int32_array_create (&a1, 10);
    CHECK (a1.len == 0);
    int32_array_create (&a2, 10);
    CHECK (a2.len == 0);
    int32_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    int32_array_push_back (&a2, c2);
    CHECK (a2.len == 1);
    int32_array_concat (&a1, &a2);
    CHECK (a1.len == 2);
    CHECK (a2.len == 1);
    g1 = int32_array_get (&a1, 0);
    CHECK (g1 == c1);
    g2 = int32_array_get (&a1, 1);
    CHECK (g2 == c2);
    int32_array_free (&a1);
    int32_array_free (&a2);
}

TESTER(int32_array_shrink_1)
{
    Int32Array a1;
    am_int32 c1 = 1, c2 = 2;
    am_int32 g1, g2;

    int32_array_create (&a1, 10);
    CHECK (a1.len == 0);
    int32_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    int32_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    int32_array_shrink (&a1);
    CHECK (a1.len == 2);
    g1 = int32_array_get (&a1, 0);
    CHECK (g1 == c1);
    g2 = int32_array_get (&a1, 1);
    CHECK (g2 == c2);
    int32_array_free (&a1);
}

TESTER(int32_array_shrink_2)
{
    Int32Array a1;

    int32_array_create (&a1, 10);
    int32_array_shrink (&a1);
    CHECK (a1.capacity == 0);
    int32_array_free (&a1);
}

TESTER(int32_array_truncate_1)
{
    Int32Array a1;
    am_int32 c1 = 1, c2 = 2;
    am_int32 g1;

    int32_array_create (&a1, 10);
    CHECK (a1.len == 0);
    int32_array_push_back (&a1, c1);
    CHECK (a1.len == 1);
    int32_array_push_back (&a1, c2);
    CHECK (a1.len == 2);
    int32_array_truncate (&a1, 1);
    CHECK (a1.len == 1);
    g1 = int32_array_get (&a1, 0);
    CHECK (g1 == c1);
    int32_array_truncate (&a1, 0);
    CHECK (a1.len == 0);
    int32_array_free (&a1);
}

TESTER(int32_array_grow_1)
{
    Int32Array a1;

    int32_array_create (&a1, 10);
    CHECK (a1.capacity == 10);
    int32_array_grow (&a1, 100);
    CHECK (a1.capacity >= 100);
    int32_array_free (&a1);
}

TESTER(int32_array_grow_2)
{
    Int32Array a1;

    int32_array_create (&a1, 10);
    CHECK (a1.capacity == 10);
    int32_array_grow (&a1, 11);
    CHECK (a1.capacity >= 11);
    int32_array_free (&a1);
}

TESTER(int32_array_grow_3)
{
    Int32Array a1;
    am_int32 c1 = 1, c2 = 2;

    int32_array_create (&a1, 10);
    CHECK (a1.capacity == 10);
    int32_array_push_back (&a1, c1);
    int32_array_push_back (&a1, c2);
    int32_array_grow (&a1, 11);
    CHECK (a1.capacity >= 11);
    CHECK (a1.len == 2);
    int32_array_free (&a1);
}

TESTER(int32_array_compress_1)
{
    am_int32 data[] = { 1, 2, 3, 6, 7 };
    Int32Array array;
    Buffer buffer = BUFFER_INIT;
    am_bool rc;
    const am_byte *text;

    array.ptr = data;
    array.len = sizeof (data) / sizeof (data[0]);
    array.capacity = array.len;
    rc = int32_array_compress (&array, &buffer);

    CHECK (rc);

    text = buffer_to_text (&buffer);

    CHECK (text != NULL);
    CHECK (strcmp (text, "1-3, 6, 7") == 0);

    buffer_free (&buffer);
}
