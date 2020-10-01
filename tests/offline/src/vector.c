// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

typedef struct
{
    int first;
    int second;

} Canary;

static void *canary_cloner (void *ptr)
{
    Canary *source = (Canary*) ptr;
    Canary *result = (Canary*) mem_alloc (sizeof (Canary));
    result->first  = source->first;
    result->second = source->second;

    return result;
}

static void canary_liberator (void *ptr)
{
    free (ptr); /* trivial */
}

TESTER(vector_create_1)
{
    Vector v1;

    vector_create (&v1, 10);
    v1.cloner = canary_cloner;
    v1.liberator = canary_liberator;

    CHECK (v1.capacity == 10);
    CHECK (v1.len == 0);

    vector_destroy (&v1);
}

TESTER(vector_push_back_1)
{
    Vector v1;
    Canary c1 = { 1, 2 };
    Canary *p1;

    vector_create (&v1, 10);

    CHECK (v1.len == 0);

    vector_push_back (&v1, &c1);

    CHECK (v1.len == 1);

    p1 = (Canary*) vector_get (&v1, 0);

    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    vector_destroy (&v1);
}

TESTER(vector_push_front_1)
{
    Vector v1;
    Canary c1 = { 1, 2 };
    Canary *p1;

    vector_create (&v1, 10);

    CHECK (v1.len == 0);

    vector_push_front (&v1, &c1);

    CHECK (v1.len == 1);

    p1 = (Canary*) vector_get (&v1, 0);

    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    vector_destroy (&v1);
}

TESTER(vector_pop_back_1)
{
    Vector v1;
    Canary c1 = { 1, 2 };
    Canary *p1;

    vector_create (&v1, 10);

    CHECK (v1.len == 0);

    vector_push_back (&v1, &c1);

    CHECK (v1.len == 1);

    p1 = (Canary*) vector_pop_back (&v1);

    CHECK (v1.len == 0);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    vector_destroy (&v1);
}

TESTER(vector_pop_front_1)
{
    Vector v1;
    Canary c1 = { 1, 2 };
    Canary *p1;

    vector_create (&v1, 10);

    CHECK (v1.len == 0);

    vector_push_back (&v1, &c1);

    CHECK (v1.len == 1);

    p1 = (Canary*) vector_pop_front (&v1);

    CHECK (v1.len == 0);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    vector_destroy (&v1);
}

TESTER(vector_set_1)
{
    Vector  v1;
    Canary c1 = { 1, 2 };
    Canary *p1;

    vector_create (&v1, 10);

    CHECK (v1.len == 0);

    vector_push_back (&v1, NULL);

    CHECK (v1.len == 1);

    vector_set (&v1, 0, &c1);
    p1 = (Canary*) vector_get (&v1, 0);

    CHECK (p1 ->first == c1.first);
    CHECK (p1 ->second == c1.second);

    vector_destroy (&v1);
}

TESTER(vector_clone_1)
{
    Vector v1, v2;
    Canary c1 = { 1, 2 };
    Canary *p1;

    vector_create (&v1, 10);

    CHECK (v1.len == 0);

    vector_create (&v2, 10);

    CHECK (v2.len == 0);

    vector_push_back (&v1, &c1);

    CHECK (v1.len == 1);
    CHECK (v2.len == 0);

    vector_clone (&v2, &v1);

    CHECK (v2.len == 1);

    p1 = (Canary*) vector_get (&v2, 0);

    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    vector_destroy (&v1);
    vector_destroy (&v2);
}

TESTER(vector_clone_2)
{
    Vector v1, v2;
    Canary c1 = { 1, 2 };
    Canary *p1;

    vector_create (&v1, 10);
    v1.cloner = canary_cloner;

    CHECK (v1.len == 0);

    vector_create (&v2, 10);

    CHECK (v2.len == 0);

    vector_push_back(&v1, &c1);

    CHECK (v1.len == 1);
    CHECK (v2.len == 0);

    vector_clone (&v2, &v1);
    v2.liberator = canary_liberator;

    CHECK (v2.len == 1);

    p1 = (Canary*) vector_get (&v2, 0);

    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    vector_destroy (&v1);
    vector_destroy (&v2);
}

TESTER(vector_copy_1)
{
    Vector v1, v2;
    Canary c1 = { 1, 2 };
    Canary *p1;

    vector_create(&v1, 10);

    CHECK (v1.len == 0);

    vector_create (&v2, 10);

    CHECK (v2.len == 0);

    vector_push_back (&v1, &c1);

    CHECK (v1.len == 1);
    CHECK (v2.len == 0);

    vector_copy (&v2, &v1);

    CHECK (v2.len == 1);

    p1 = (Canary*) vector_get (&v2, 0);

    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    vector_destroy (&v1);
    vector_destroy (&v2);
}

TESTER(vector_concat_1)
{
    Vector v1, v2;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    Canary *p1;

    vector_create (&v1, 10);

    CHECK (v1.len == 0);

    vector_create (&v2, 10);

    CHECK (v2.len == 0);

    vector_push_back (&v1, &c1);

    CHECK (v1.len == 1);

    vector_push_back (&v2, &c2);

    CHECK (v2.len == 1);

    vector_concat (&v1, &v2);

    CHECK (v1.len == 2);

    p1 = (Canary*) vector_get (&v1, 0);

    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    p1 = (Canary*) vector_get (&v1, 1);

    CHECK (p1->first == c2.first);
    CHECK (p1->second == c2.second);

    vector_destroy (&v1);
    vector_destroy (&v2);
}
