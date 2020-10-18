// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

typedef struct
{
    int first;
    int second;

} Canary;

static int liberationCounter = 0;

static void MAGNA_CALL canary_liberator (void *ptr)
{
    (void) ptr;
    ++liberationCounter;
}

static int cloningCounter = 0;

static void* MAGNA_CALL canary_cloner (void *ptr)
{
    Canary *c = ptr;

    c->first *= 10;
    c->second *= 20;

    return ptr;
}

TESTER(array_init_1)
{
    Array array;

    array_init (&array, sizeof (Canary));

    CHECK (array.itemSize == sizeof (Canary));
    CHECK (array.offset == 0);
    CHECK (array.len == 0);
    CHECK (array.capacity == 0);
    CHECK (array.ptr == NULL);

    array_destroy (&array);
}

TESTER(array_create_1)
{
    Array array;

    CHECK (array_create (&array, sizeof (Canary), 4));
    CHECK (array.itemSize == sizeof (Canary));
    CHECK (array.offset == 0);
    CHECK (array.len == 0);
    CHECK (array.capacity == 4);
    CHECK (array.ptr != NULL);

    array_destroy (&array);

    CHECK (array.len == 0);
    CHECK (array.capacity == 0);
    CHECK (array.offset == 0);
    CHECK (array.ptr == NULL);
}

TESTER(array_liberate_1)
{
    Array array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };

    liberationCounter = 0;
    array_init (&array, sizeof (Canary));

    CHECK (array_push_back (&array, &c1));
    CHECK (array_push_back (&array, &c2));

    array_liberate (&array, canary_liberator);

    CHECK (liberationCounter == 2);
}

TESTER(array_push_back_1)
{
    Array array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    const Canary *p1, *p2;

    array_init (&array, sizeof (Canary));

    CHECK (array_push_back (&array, &c1));
    CHECK (array_push_back (&array, &c2));
    CHECK (array.len == 2);

    p1 = (const Canary *) array_get (&array, 0);
    CHECK (p1 != NULL);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    p2 = (const Canary *) array_get (&array, 1);
    CHECK (p2 != NULL);
    CHECK (p2->first == c2.first);
    CHECK (p2->second == c2.second);

    array_destroy (&array);
}

TESTER(array_push_back_2)
{
    Array array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    const Canary *p1, *p2;

    CHECK (array_create (&array, sizeof (Canary), 4));
    CHECK (array_push_back (&array, &c1));
    CHECK (array_push_back (&array, &c2));
    CHECK (array.len == 2);

    p1 = (const Canary *) array_get (&array, 0);
    CHECK (p1 != NULL);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    p2 = (const Canary *) array_get (&array, 1);
    CHECK (p2 != NULL);
    CHECK (p2->first == c2.first);
    CHECK (p2->second == c2.second);

    array_destroy (&array);
}

TESTER(array_pop_back_1)
{
    Array array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    Canary *p1, *p2;

    array_init (&array, sizeof (Canary));

    CHECK (array_push_back (&array, &c1));
    CHECK (array_push_back (&array, &c2));
    CHECK (array.len == 2);

    p1 = (Canary *) array_pop_back (&array);
    CHECK (p1 != NULL);
    CHECK (array.len == 1);
    CHECK (p1->first == c2.first);
    CHECK (p1->second == c2.second);

    p2 = (Canary *) array_pop_back (&array);
    CHECK (p2 != NULL);
    CHECK (array.len == 0);
    CHECK (p2->first == c1.first);
    CHECK (p2->second == c1.second);

    array_destroy (&array);
}

TESTER(array_pop_front_1)
{
    Array array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    Canary *p1, *p2;

    array_init (&array, sizeof (Canary));

    CHECK (array_push_back (&array, &c1));
    CHECK (array_push_back (&array, &c2));
    CHECK (array.len == 2);

    p1 = (Canary *) array_pop_front (&array);
    CHECK (p1 != NULL);
    CHECK (array.len == 1);
    CHECK (array.offset == 1);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    p2 = (Canary *) array_pop_front (&array);
    CHECK (p2 != NULL);
    CHECK (array.len == 0);
    CHECK (array.offset == 0);
    CHECK (p2->first == c2.first);
    CHECK (p2->second == c2.second);

    array_destroy (&array);
}

TESTER(array_push_front_1)
{
    Array array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    Canary *p1, *p2;

    array_init (&array, sizeof (Canary));

    CHECK (array_push_front (&array, &c1));
    CHECK (array_push_front (&array, &c2));
    CHECK (array.len == 2);

    p1 = (Canary *) array_pop_front (&array);
    CHECK (p1 != NULL);
    CHECK (array.len == 1);
    CHECK (array.offset == 1);
    CHECK (p1->first == c2.first);
    CHECK (p1->second == c2.second);

    p2 = (Canary *) array_pop_front (&array);
    CHECK (p2 != NULL);
    CHECK (array.len == 0);
    CHECK (array.offset == 0);
    CHECK (p2->first == c1.first);
    CHECK (p2->second == c1.second);

    array_destroy (&array);
}

TESTER(array_truncate_1)
{
    Array array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };

    array_init (&array, sizeof (Canary));

    CHECK (array_push_back (&array, &c1));
    CHECK (array_push_back (&array, &c2));
    CHECK (array.len == 2);

    array_truncate (&array, 1);
    CHECK (array.len == 1);
    CHECK (array.offset == 0);

    array_truncate (&array, 0);
    CHECK (array.len == 0);
    CHECK (array.offset == 0);

    array_destroy (&array);
}

TESTER(array_clone_1)
{
    Array target, source;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };

    array_init (&target, sizeof (Canary));
    array_init (&source, sizeof (Canary));

    CHECK (array_push_back (&source, &c1));
    CHECK (array_push_back (&source, &c2));
    CHECK (source.len == 2);

    CHECK (array_clone (&target, &source, NULL));
    CHECK (target.len == source.len);

    array_destroy (&target);
    array_destroy (&source);
}

TESTER(array_clone_2)
{
    Array target, source;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    Canary *p1, *p2;

    array_init (&target, sizeof (Canary));
    array_init (&source, sizeof (Canary));

    CHECK (array_push_back (&source, &c1));
    CHECK (array_push_back (&source, &c2));
    CHECK (source.len == 2);

    CHECK (array_clone (&target, &source, canary_cloner));
    CHECK (target.len == source.len);

    p1 = (Canary*) array_get (&target, 0);
    CHECK (p1 != NULL);
    CHECK (p1->first == c1.first * 10);
    CHECK (p1->second == c1.second * 20);

    p2 = (Canary*) array_get (&target, 1);
    CHECK (p2 != NULL);
    CHECK (p2->first == c2.first * 10);
    CHECK (p2->second == c2.second * 20);

    array_destroy (&target);
    array_destroy (&source);
}

TESTER(array_copy_1)
{
    Array target, source;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };

    array_init (&target, sizeof (Canary));
    array_init (&source, sizeof (Canary));

    CHECK (array_push_back (&source, &c1));
    CHECK (array_push_back (&source, &c2));
    CHECK (source.len == 2);

    CHECK (array_copy (&target, &source));
    CHECK (target.len == source.len);

    array_destroy (&target);
    array_destroy (&source);
}

TESTER(array_concat_1)
{
    Array target, source;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };

    array_init (&target, sizeof (Canary));
    array_init (&source, sizeof (Canary));

    CHECK (array_push_back (&source, &c1));
    CHECK (array_push_back (&source, &c2));
    CHECK (source.len == 2);

    CHECK (array_concat (&target, &source));
    CHECK (target.len == source.len);

    array_destroy (&target);
    array_destroy (&source);
}

TESTER(array_set_1)
{
    Array array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    const Canary *p1;

    array_init (&array, sizeof (Canary));

    CHECK (array_push_back (&array, &c1));

    array_set (&array, 0, &c2);
    p1 = (Canary*) array_get (&array, 0);

    CHECK (p1 != NULL);
    CHECK (p1->first == c2.first);
    CHECK (p1->second == c2.second);

    array_destroy (&array);

}

TESTER(array_emplace_back_1)
{
    Array array;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    Canary *p1, *p2, *p3, *p4;

    array_init (&array, sizeof (Canary));

    p1 = array_emplace_back (&array);
    CHECK (p1 != NULL);
    *p1 = c1;

    p2 = array_emplace_back (&array);
    CHECK (p2 != NULL);
    *p2 = c2;

    p3 = array_get (&array, 0);
    CHECK (p3 != NULL);
    CHECK (p3->first == c1.first);
    CHECK (p3->second == c1.second);

    p4 = array_get (&array, 1);
    CHECK (p4 != NULL);
    CHECK (p4->first == c2.first);
    CHECK (p4->second == c2.second);

    array_destroy (&array);
}

TESTER(array_remove_index_1)
{
    Array array;
    Canary c1 = {1 ,2}, c2 = {3, 4}, c3 = {5, 6};
    Canary *p1, *p2;

    array_init (&array, sizeof (Canary));

    CHECK (array_push_back (&array, &c1));
    CHECK (array_push_back (&array, &c2));
    CHECK (array_push_back (&array, &c3));
    CHECK (array.len == 3);

    array_remove_index (&array, 0);
    CHECK (array.len == 2);

    p1 = (Canary*) array_get (&array, 0);
    CHECK (p1 != NULL);
    CHECK (p1->first == c2.first);

    p2 = (Canary*) array_get (&array, 1);
    CHECK (p2 != NULL);
    CHECK (p2->first == c3.first);

    array_destroy (&array);
}

TESTER(array_remove_index_2)
{
    Array array;
    Canary c1 = {1 ,2}, c2 = {3, 4}, c3 = {5, 6};
    Canary *p1, *p2;

    array_init (&array, sizeof (Canary));

    CHECK (array_push_back (&array, &c1));
    CHECK (array_push_back (&array, &c2));
    CHECK (array_push_back (&array, &c3));
    CHECK (array.len == 3);

    array_remove_index (&array, 1);
    CHECK (array.len == 2);

    p1 = (Canary*) array_get (&array, 0);
    CHECK (p1 != NULL);
    CHECK (p1->first == c1.first);

    p2 = (Canary*) array_get (&array, 1);
    CHECK (p2 != NULL);
    CHECK (p2->first == c3.first);

    array_destroy (&array);
}

TESTER(array_remove_index_3)
{
    Array array;
    Canary c1 = {1 ,2}, c2 = {3, 4}, c3 = {5, 6};
    Canary *p1, *p2;

    array_init (&array, sizeof (Canary));

    CHECK (array_push_back (&array, &c1));
    CHECK (array_push_back (&array, &c2));
    CHECK (array_push_back (&array, &c3));
    CHECK (array.len == 3);

    array_remove_index (&array, 2);
    CHECK (array.len == 2);

    p1 = (Canary*) array_get (&array, 0);
    CHECK (p1 != NULL);
    CHECK (p1->first == c1.first);

    p2 = (Canary*) array_get (&array, 1);
    CHECK (p2 != NULL);
    CHECK (p2->first == c2.first);

    array_destroy (&array);
}

TESTER(array_grow_1)
{
    Array array;
    Canary c1 = {1 ,2}, c2 = {3, 4}, c3 = {5, 6};

    array_init (&array, sizeof (Canary));

    CHECK (array_push_back (&array, &c1));
    CHECK (array_push_back (&array, &c2));
    CHECK (array_push_back (&array, &c3));
    CHECK (array.len == 3);

    array_remove_index (&array, 0);

    CHECK (array.len == 2);
    CHECK (array.offset == 1);
    CHECK (array_grow (&array, 3));
    CHECK (array.offset == 0);

    array_destroy (&array);
}

static int MAGNA_CALL my_int32_comparer (const void *left, const void *right, const void *data)
{
    (void) data;

    return *((const am_int32*) left) - *((const am_int32*) right);
}

TESTER(array_sort_1)
{
    Array array;

    array_init (&array, sizeof (am_int32));
    *(am_int32*) array_emplace_back (&array) = 3;
    *(am_int32*) array_emplace_back (&array) = 2;
    *(am_int32*) array_emplace_back (&array) = 1;

    array_sort (&array, my_int32_comparer, NULL);

    CHECK (*(const am_int32*) array_get (&array, 0) == 1);
    CHECK (*(const am_int32*) array_get (&array, 1) == 2);
    CHECK (*(const am_int32*) array_get (&array, 2) == 3);

    array_destroy (&array);
}

TESTER(array_sort_2)
{
    Array array;

    array_init (&array, sizeof (am_int32));
    *(am_int32*) array_emplace_back (&array) = 1;
    *(am_int32*) array_emplace_back (&array) = 2;
    *(am_int32*) array_emplace_back (&array) = 3;

    array_sort (&array, my_int32_comparer, NULL);

    CHECK (*(const am_int32*) array_get (&array, 0) == 1);
    CHECK (*(const am_int32*) array_get (&array, 1) == 2);
    CHECK (*(const am_int32*) array_get (&array, 2) == 3);

    array_destroy (&array);
}

TESTER(array_sort_3)
{
    Array array;

    array_init (&array, sizeof (am_int32));
    *(am_int32*) array_emplace_back (&array) = 1;
    *(am_int32*) array_emplace_back (&array) = 2;
    *(am_int32*) array_emplace_back (&array) = 6;
    *(am_int32*) array_emplace_back (&array) = 5;
    *(am_int32*) array_emplace_back (&array) = 4;
    *(am_int32*) array_emplace_back (&array) = 3;
    *(am_int32*) array_emplace_back (&array) = 3;

    array_sort (&array, my_int32_comparer, NULL);

    CHECK (*(const am_int32*) array_get (&array, 0) == 1);
    CHECK (*(const am_int32*) array_get (&array, 1) == 2);
    CHECK (*(const am_int32*) array_get (&array, 2) == 3);
    CHECK (*(const am_int32*) array_get (&array, 3) == 3);
    CHECK (*(const am_int32*) array_get (&array, 4) == 4);
    CHECK (*(const am_int32*) array_get (&array, 5) == 5);
    CHECK (*(const am_int32*) array_get (&array, 6) == 6);

    array_destroy (&array);
}

TESTER(array_sort_4)
{
    Array array;

    array_init (&array, sizeof (am_int32));
    array_sort (&array, my_int32_comparer, NULL);

    CHECK (array.len == 0);

    array_destroy (&array);
}

TESTER(array_bsearch_1)
{
    Array array;
    am_int32 value = 123;
    const am_int32 *found;

    array_init (&array, sizeof (am_int32));
    found = (const am_int32*) array_bsearch (&array, &value, my_int32_comparer, NULL);

    CHECK (found == NULL);

    array_destroy (&array);
}

TESTER(array_bsearch_2)
{
    Array array;
    am_int32 value = 123;
    const am_int32 *found;

    array_init (&array, sizeof (am_int32));
    *(am_int32*) array_emplace_back (&array) = 1;
    *(am_int32*) array_emplace_back (&array) = 2;
    *(am_int32*) array_emplace_back (&array) = 3;
    *(am_int32*) array_emplace_back (&array) = 4;
    *(am_int32*) array_emplace_back (&array) = 5;
    *(am_int32*) array_emplace_back (&array) = 6;
    *(am_int32*) array_emplace_back (&array) = 7;

    found = (const am_int32*) array_bsearch (&array, &value, my_int32_comparer, NULL);
    CHECK (found == NULL);

    value = 2;
    found = (const am_int32*) array_bsearch (&array, &value, my_int32_comparer, NULL);
    CHECK (found != NULL);
    CHECK (*found == 2);

    value = 1;
    found = (const am_int32*) array_bsearch (&array, &value, my_int32_comparer, NULL);
    CHECK (found != NULL);
    CHECK (*found == 1);

    value = 7;
    found = (const am_int32*) array_bsearch (&array, &value, my_int32_comparer, NULL);
    CHECK (found != NULL);
    CHECK (*found == 7);

    value = 0;
    found = (const am_int32*) array_bsearch (&array, &value, my_int32_comparer, NULL);
    CHECK (found == NULL);

    array_destroy (&array);
}

