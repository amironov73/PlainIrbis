/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/tester.h"

typedef struct
{
    int first;
    int second;
} Canary;

static void *canary_cloner (void *ptr)
{
    Canary *source = (Canary*) ptr;
    Canary *result = (Canary*) calloc (1, sizeof (Canary));
    result->first = source->first;
    result->second = source->second;

    return result;
}

static void canary_liberator (void *ptr)
{
    free (ptr); /* trivial */
}

TESTER(array_create_1)
{
    Array a1;

    a1.cloner = canary_cloner;
    a1.liberator = canary_liberator;
    array_create(&a1, 10);
    CHECK (a1.capacity == 10);
    CHECK (a1.len == 0);
    array_free(&a1);
}

TESTER(array_push_back_1)
{
    Array a1;
    Canary c1 = { 1, 2 };
    Canary *p1;

    array_create (&a1, 10);
    CHECK (a1.len == 0);
    array_push_back (&a1, &c1);
    CHECK (a1.len == 1);
    p1 = (Canary*) array_get(&a1, 0);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);
    array_free(&a1);
}

TESTER(array_push_front_1)
{
    Array a1;
    Canary c1 = { 1, 2 };
    Canary *p1;

    array_create (&a1, 10);
    CHECK (a1.len == 0);
    array_push_front (&a1, &c1);
    CHECK (a1.len == 1);
    p1 = (Canary*) array_get(&a1, 0);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);
    array_free(&a1);
}

TESTER(array_pop_back_1)
{
    Array a1;
    Canary c1 = { 1, 2 };
    Canary *p1;

    array_create (&a1, 10);
    CHECK (a1.len == 0);
    array_push_back (&a1, &c1);
    CHECK (a1.len == 1);
    p1 = (Canary*) array_pop_back (&a1);
    CHECK (a1.len == 0);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);
    array_free (&a1);
}

TESTER(array_pop_front_1)
{
    Array a1;
    Canary c1 = { 1, 2 };
    Canary *p1;

    array_create (&a1, 10);
    CHECK (a1.len == 0);
    array_push_back (&a1, &c1);
    CHECK (a1.len == 1);
    p1 = (Canary*) array_pop_front (&a1);
    CHECK (a1.len == 0);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);
    array_free (&a1);
}

TESTER(array_set_1)
{
    Array  a1;
    Canary c1 = { 1, 2 };
    Canary *p1;

    array_create (&a1, 10);
    CHECK (a1.len == 0);
    array_push_back (&a1, NULL);
    CHECK (a1.len == 1);
    array_set (&a1, 0, &c1);
    p1 = (Canary*) array_get (&a1, 0);
    CHECK (p1 ->first == c1.first);
    CHECK (p1 ->second == c1.second);
}

TESTER(array_clone_1)
{
    Array a1, a2;
    Canary c1 = { 1, 2 };
    Canary *p1;

    array_create (&a1, 10);
    CHECK (a1.len == 0);
    array_create (&a2, 10);
    CHECK (a2.len == 0);
    array_push_back (&a1, &c1);
    CHECK (a1.len == 1);
    CHECK (a2.len == 0);
    array_clone (&a2, &a1);
    CHECK (a2.len == 1);
    p1 = (Canary*) array_get (&a2, 0);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);
}

TESTER(array_copy_1)
{
    Array a1, a2;
    Canary c1 = { 1, 2 };
    Canary *p1;

    array_create (&a1, 10);
    CHECK (a1.len == 0);
    array_create (&a2, 10);
    CHECK (a2.len == 0);
    array_push_back (&a1, &c1);
    CHECK (a1.len == 1);
    CHECK (a2.len == 0);
    array_copy (&a2, &a1);
    CHECK (a2.len == 1);
    p1 = (Canary*) array_get (&a2, 0);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);
}

TESTER(array_concat_1)
{
    Array a1, a2;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    Canary *p1;

    array_create (&a1, 10);
    CHECK (a1.len == 0);
    array_create (&a2, 10);
    CHECK (a2.len == 0);
    array_push_back (&a1, &c1);
    CHECK (a1.len == 1);
    array_push_back (&a2, &c2);
    CHECK (a2.len == 1);
    array_concat (&a1, &a2);
    CHECK (a1.len == 2);
    p1 = (Canary*) array_get (&a1, 0);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);
    p1 = (Canary*) array_get (&a1, 1);
    CHECK (p1->first == c2.first);
    CHECK (p1->second == c2.second);
}
