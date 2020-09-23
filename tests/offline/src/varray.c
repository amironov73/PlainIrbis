// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

typedef struct
{
    int first;
    int second;

} Canary;

static int liberationCounter = 0;

static void canary_liberator (void *ptr)
{
    (void) ptr;
    ++liberationCounter;
}

static int cloningCounter = 0;

static void* canary_cloner (void *ptr)
{
    Canary *c = ptr;

    c->first *= 10;
    c->second *= 20;

    return ptr;
}

TESTER(varray_init_1)
{
    ValueArray array;

    varray_init (&array, sizeof (Canary));

    CHECK (array.itemSize == sizeof (Canary));
    CHECK (array.offset == 0);
    CHECK (array.len == 0);
    CHECK (array.capacity == 0);
    CHECK (array.ptr == NULL);
    CHECK (array.liberator == NULL);

    varray_free (&array);
}

TESTER(varray_create_1)
{
    ValueArray array;

    CHECK (varray_create (&array, sizeof (Canary), 4));
    CHECK (array.itemSize == sizeof (Canary));
    CHECK (array.offset == 0);
    CHECK (array.len == 0);
    CHECK (array.capacity == 4);
    CHECK (array.ptr != NULL);
    CHECK (array.liberator == NULL);

    varray_free (&array);

    CHECK (array.len == 0);
    CHECK (array.capacity == 0);
    CHECK (array.offset == 0);
    CHECK (array.ptr == NULL);
}

TESTER(varray_free_1)
{
    ValueArray array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };

    liberationCounter = 0;
    varray_init (&array, sizeof (Canary));
    array.liberator = canary_liberator;

    CHECK (varray_push_back (&array, &c1));
    CHECK (varray_push_back (&array, &c2));

    varray_free (&array);

    CHECK (liberationCounter == 2);
}

TESTER(varray_push_back_1)
{
    ValueArray array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    const Canary *p1, *p2;

    varray_init (&array, sizeof (Canary));

    CHECK (varray_push_back (&array, &c1));
    CHECK (varray_push_back (&array, &c2));
    CHECK (array.len == 2);

    p1 = (const Canary *) varray_get (&array, 0);
    CHECK (p1 != NULL);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    p2 = (const Canary *) varray_get (&array, 1);
    CHECK (p2 != NULL);
    CHECK (p2->first == c2.first);
    CHECK (p2->second == c2.second);

    varray_free (&array);
}

TESTER(varray_push_back_2)
{
    ValueArray array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    const Canary *p1, *p2;

    CHECK (varray_create (&array, sizeof (Canary), 4));
    CHECK (varray_push_back (&array, &c1));
    CHECK (varray_push_back (&array, &c2));
    CHECK (array.len == 2);

    p1 = (const Canary *) varray_get (&array, 0);
    CHECK (p1 != NULL);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    p2 = (const Canary *) varray_get (&array, 1);
    CHECK (p2 != NULL);
    CHECK (p2->first == c2.first);
    CHECK (p2->second == c2.second);

    varray_free (&array);
}

TESTER(varray_pop_back_1)
{
    ValueArray array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    Canary *p1, *p2;

    varray_init (&array, sizeof (Canary));

    CHECK (varray_push_back (&array, &c1));
    CHECK (varray_push_back (&array, &c2));
    CHECK (array.len == 2);

    p1 = (Canary *) varray_pop_back (&array);
    CHECK (p1 != NULL);
    CHECK (array.len == 1);
    CHECK (p1->first == c2.first);
    CHECK (p1->second == c2.second);

    p2 = (Canary *) varray_pop_back (&array);
    CHECK (p2 != NULL);
    CHECK (array.len == 0);
    CHECK (p2->first == c1.first);
    CHECK (p2->second == c1.second);

    varray_free (&array);
}

TESTER(varray_pop_front_1)
{
    ValueArray array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    Canary *p1, *p2;

    varray_init (&array, sizeof (Canary));

    CHECK (varray_push_back (&array, &c1));
    CHECK (varray_push_back (&array, &c2));
    CHECK (array.len == 2);

    p1 = (Canary *) varray_pop_front (&array);
    CHECK (p1 != NULL);
    CHECK (array.len == 1);
    CHECK (array.offset == 1);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    p2 = (Canary *) varray_pop_front (&array);
    CHECK (p2 != NULL);
    CHECK (array.len == 0);
    CHECK (array.offset == 0);
    CHECK (p2->first == c2.first);
    CHECK (p2->second == c2.second);

    varray_free (&array);
}

TESTER(varray_push_front_1)
{
    ValueArray array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    Canary *p1, *p2;

    varray_init (&array, sizeof (Canary));

    CHECK (varray_push_front (&array, &c1));
    CHECK (varray_push_front (&array, &c2));
    CHECK (array.len == 2);

    p1 = (Canary *) varray_pop_front (&array);
    CHECK (p1 != NULL);
    CHECK (array.len == 1);
    CHECK (array.offset == 1);
    CHECK (p1->first == c2.first);
    CHECK (p1->second == c2.second);

    p2 = (Canary *) varray_pop_front (&array);
    CHECK (p2 != NULL);
    CHECK (array.len == 0);
    CHECK (array.offset == 0);
    CHECK (p2->first == c1.first);
    CHECK (p2->second == c1.second);

    varray_free (&array);
}

TESTER(varray_truncate_1)
{
    ValueArray array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };

    varray_init (&array, sizeof (Canary));

    CHECK (varray_push_back (&array, &c1));
    CHECK (varray_push_back (&array, &c2));
    CHECK (array.len == 2);

    varray_truncate (&array, 1);
    CHECK (array.len == 1);
    CHECK (array.offset == 0);

    varray_truncate (&array, 0);
    CHECK (array.len == 0);
    CHECK (array.offset == 0);

    varray_free (&array);
}

TESTER(varray_clone_1)
{
    ValueArray target, source;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };

    varray_init (&target, sizeof (Canary));
    varray_init (&source, sizeof (Canary));

    CHECK (varray_push_back (&source, &c1));
    CHECK (varray_push_back (&source, &c2));
    CHECK (source.len == 2);

    CHECK (varray_clone (&target, &source, NULL));
    CHECK (target.len == source.len);

    varray_free (&target);
    varray_free (&source);
}

TESTER(varray_clone_2)
{
    ValueArray target, source;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    Canary *p1, *p2;

    varray_init (&target, sizeof (Canary));
    varray_init (&source, sizeof (Canary));

    CHECK (varray_push_back (&source, &c1));
    CHECK (varray_push_back (&source, &c2));
    CHECK (source.len == 2);

    CHECK (varray_clone (&target, &source, canary_cloner));
    CHECK (target.len == source.len);

    p1 = (Canary*) varray_get (&target, 0);
    CHECK (p1 != NULL);
    CHECK (p1->first == c1.first * 10);
    CHECK (p1->second == c1.second * 20);

    p2 = (Canary*) varray_get (&target, 1);
    CHECK (p2 != NULL);
    CHECK (p2->first == c2.first * 10);
    CHECK (p2->second == c2.second * 20);

    varray_free (&target);
    varray_free (&source);
}

TESTER(varray_copy_1)
{
    ValueArray target, source;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };

    varray_init (&target, sizeof (Canary));
    varray_init (&source, sizeof (Canary));

    CHECK (varray_push_back (&source, &c1));
    CHECK (varray_push_back (&source, &c2));
    CHECK (source.len == 2);

    CHECK (varray_copy (&target, &source));
    CHECK (target.len == source.len);

    varray_free (&target);
    varray_free (&source);
}

TESTER(varray_concat_1)
{
    ValueArray target, source;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };

    varray_init (&target, sizeof (Canary));
    varray_init (&source, sizeof (Canary));

    CHECK (varray_push_back (&source, &c1));
    CHECK (varray_push_back (&source, &c2));
    CHECK (source.len == 2);

    CHECK (varray_concat (&target, &source));
    CHECK (target.len == source.len);

    varray_free (&target);
    varray_free (&source);
}

TESTER(varray_set_1)
{
    ValueArray array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    const Canary *p1;

    varray_init (&array, sizeof (Canary));

    CHECK (varray_push_back (&array, &c1));

    varray_set (&array, 0, &c2);
    p1 = (Canary*) varray_get (&array, 0);

    CHECK (p1 != NULL);
    CHECK (p1->first == c2.first);
    CHECK (p1->second == c2.second);

    varray_free (&array);

}

TESTER(varray_emplace_back_1)
{
    ValueArray array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    Canary *p1, *p2, *p3, *p4;

    varray_init (&array, sizeof (Canary));

    p1 = varray_emplace_back (&array);
    CHECK (p1 != NULL);
    *p1 = c1;

    p2 = varray_emplace_back (&array);
    CHECK (p2 != NULL);
    *p2 = c2;

    p3 = varray_get (&array, 0);
    CHECK (p3 != NULL);
    CHECK (p3->first == c1.first);
    CHECK (p3->second == c1.second);

    p4 = varray_get (&array, 1);
    CHECK (p4 != NULL);
    CHECK (p4->first == c2.first);
    CHECK (p4->second == c2.second);

    varray_free (&array);
}

TESTER(varray_remove_index_1)
{
    ValueArray array;
    Canary c1 = {1 ,2}, c2 = {3, 4}, c3 = {5, 6};
    Canary *p1, *p2;

    varray_init (&array, sizeof (Canary));

    CHECK (varray_push_back (&array, &c1));
    CHECK (varray_push_back (&array, &c2));
    CHECK (varray_push_back (&array, &c3));
    CHECK (array.len == 3);

    varray_remove_index (&array, 0);
    CHECK (array.len == 2);

    p1 = (Canary*) varray_get (&array, 0);
    CHECK (p1 != NULL);
    CHECK (p1->first == c2.first);

    p2 = (Canary*) varray_get (&array, 1);
    CHECK (p2 != NULL);
    CHECK (p2->first == c3.first);

    varray_free (&array);
}

TESTER(varray_remove_index_2)
{
    ValueArray array;
    Canary c1 = {1 ,2}, c2 = {3, 4}, c3 = {5, 6};
    Canary *p1, *p2;

    varray_init (&array, sizeof (Canary));

    CHECK (varray_push_back (&array, &c1));
    CHECK (varray_push_back (&array, &c2));
    CHECK (varray_push_back (&array, &c3));
    CHECK (array.len == 3);

    varray_remove_index (&array, 1);
    CHECK (array.len == 2);

    p1 = (Canary*) varray_get (&array, 0);
    CHECK (p1 != NULL);
    CHECK (p1->first == c1.first);

    p2 = (Canary*) varray_get (&array, 1);
    CHECK (p2 != NULL);
    CHECK (p2->first == c3.first);

    varray_free (&array);
}

TESTER(varray_remove_index_3)
{
    ValueArray array;
    Canary c1 = {1 ,2}, c2 = {3, 4}, c3 = {5, 6};
    Canary *p1, *p2;

    varray_init (&array, sizeof (Canary));

    CHECK (varray_push_back (&array, &c1));
    CHECK (varray_push_back (&array, &c2));
    CHECK (varray_push_back (&array, &c3));
    CHECK (array.len == 3);

    varray_remove_index (&array, 2);
    CHECK (array.len == 2);

    p1 = (Canary*) varray_get (&array, 0);
    CHECK (p1 != NULL);
    CHECK (p1->first == c1.first);

    p2 = (Canary*) varray_get (&array, 1);
    CHECK (p2 != NULL);
    CHECK (p2->first == c2.first);

    varray_free (&array);
}

TESTER(varray_grow_1)
{
    ValueArray array;
    Canary c1 = {1 ,2}, c2 = {3, 4}, c3 = {5, 6};

    varray_init (&array, sizeof (Canary));

    CHECK (varray_push_back (&array, &c1));
    CHECK (varray_push_back (&array, &c2));
    CHECK (varray_push_back (&array, &c3));
    CHECK (array.len == 3);

    varray_remove_index (&array, 0);

    CHECK (array.len == 2);
    CHECK (array.offset == 1);
    CHECK (varray_grow (&array, 3));
    CHECK (array.offset == 0);

    varray_free (&array);
}
