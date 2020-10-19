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

static am_bool MAGNA_CALL canary_walker_1 (LinkedList *list, ListItem *item, void *data)
{
    int *counter;
    Canary *canary;

    (void) list;
    counter = (int*)data;
    canary = (Canary*) list_data (item);
    *counter = *counter * 10 + canary->first;

    return AM_TRUE;
}

static am_bool MAGNA_CALL canary_walker_2 (LinkedList *list, ListItem *item, void *data)
{
    (void) list;
    (void) item;
    (void) data;

    return AM_FALSE;
}

TESTER(list_init_1)
{
    LinkedList list;

    list_init (&list, sizeof (Canary));

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
    CHECK (list.itemSize == sizeof (Canary));
    CHECK (list_is_empty (&list));
    CHECK (list_length (&list) == 0);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 0);
}

TESTER(list_push_back_1)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    ListItem *i1, *i2;
    Canary *p1, *p2;

    list_init (&list, sizeof (Canary));

    i1 = list_push_back (&list, &c1);
    CHECK (i1 != NULL);

    i2 = list_push_back (&list, &c2);
    CHECK (i2 != NULL);

    CHECK (list.first == i1);
    CHECK (i1->previous == NULL);
    CHECK (i1->next == i2);
    CHECK (i2->previous == i1);
    CHECK (i2->next == NULL);
    CHECK (list.last == i2);

    p1 = list_data (i1);
    CHECK (p1 != NULL);
    CHECK (p1->first == c1.first);
    CHECK (p1->second == c1.second);

    p2 = list_data (i2);
    CHECK (p2 != NULL);
    CHECK (p2->first == c2.first);
    CHECK (p2->second == c2.second);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 2);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 2);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_emplace_back_1)
{
    LinkedList list;
    Canary *p1, *p2;
    ListItem *i1, *i2;

    list_init (&list, sizeof (Canary));

    p1 = list_emplace_back (&list);
    CHECK (p1 != NULL);
    p1->first = 1;
    p1->second = 2;

    p2 = list_emplace_back (&list);
    CHECK (p2 != NULL);
    p2->first = 3;
    p2->second = 4;

    i1 = list_item (p1);
    CHECK (i1 != NULL);

    i2 = list_item (p2);
    CHECK (i2 != NULL);

    CHECK (list.first == i1);
    CHECK (i1->previous == NULL);
    CHECK (i1->next == i2);
    CHECK (i2->previous == i1);
    CHECK (i2->next == NULL);
    CHECK (list.last == i2);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 2);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 2);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_push_front_1)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    ListItem *i1, *i2;
    Canary *p1, *p2;

    list_init (&list, sizeof (Canary));

    i1 = list_push_front (&list, &c2);
    CHECK (i1 != NULL);

    i2 = list_push_front (&list, &c1);
    CHECK (i2 != NULL);

    CHECK (list.first == i2);
    CHECK (i2->previous == NULL);
    CHECK (i2->next == i1);
    CHECK (i1->previous == i2);
    CHECK (i1->next == NULL);
    CHECK (list.last == i1);

    p1 = list_data (i1);
    CHECK (p1 != NULL);
    CHECK (p1->first == c2.first);
    CHECK (p1->second == c2.second);

    p2 = list_data (i2);
    CHECK (p2 != NULL);
    CHECK (p2->first == c1.first);
    CHECK (p2->second == c1.second);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 2);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 2);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_emplace_front_1)
{
    LinkedList list;
    Canary *p1, *p2;
    ListItem *i1, *i2;

    list_init (&list, sizeof (Canary));

    p1 = list_emplace_front (&list);
    CHECK (p1 != NULL);
    p1->first = 1;
    p1->second = 2;

    p2 = list_emplace_front (&list);
    CHECK (p2 != NULL);
    p2->first = 3;
    p2->second = 4;

    i1 = list_item (p1);
    CHECK (i1 != NULL);

    i2 = list_item (p2);
    CHECK (i2 != NULL);

    CHECK (list.first == i2);
    CHECK (i2->previous == NULL);
    CHECK (i2->next == i1);
    CHECK (i1->previous == i2);
    CHECK (i1->next == NULL);
    CHECK (list.last == i1);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 2);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 2);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_remove_1)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 }, c3 = { 5, 6 };
    ListItem *p1, *p2, *p3;

    list_init (&list, sizeof (Canary));

    p1 = list_push_back (&list, &c1);
    p2 = list_push_back (&list, &c2);
    p3 = list_push_back (&list, &c3);

    list_remove (&list, p2, canary_liberator);
    mem_free (p2);

    CHECK (p1->next == p3);
    CHECK (p3->previous == p1);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 2);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 2);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_remove_2)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 }, c3 = { 5, 6 };
    ListItem *p1, *p2, *p3;

    list_init (&list, sizeof (Canary));

    p1 = list_push_back (&list, &c1);
    p2 = list_push_back (&list, &c2);
    p3 = list_push_back (&list, &c3);

    list_remove (&list, p1, canary_liberator);
    mem_free (p1);

    CHECK (p2->previous == NULL);
    CHECK (p3->previous == p2);
    CHECK (list.first == p2);
    CHECK (list.last == p3);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 2);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 2);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_remove_3)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 }, c3 = { 5, 6 };
    ListItem *p1, *p2, *p3;

    list_init (&list, sizeof (Canary));

    p1 = list_push_back (&list, &c1);
    p2 = list_push_back (&list, &c2);
    p3 = list_push_back (&list, &c3);

    list_remove (&list, p3, canary_liberator);
    mem_free (p3);

    CHECK (p2->next == NULL);
    CHECK (p1->next == p2);
    CHECK (list.first == p1);
    CHECK (list.last == p2);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 2);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 2);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_remove_4)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 }, c3 = { 5, 6 };
    ListItem *p1, *p2, *p3;

    list_init (&list, sizeof (Canary));

    p1 = list_push_back (&list, &c1);
    p2 = list_push_back (&list, &c2);
    p3 = list_push_back (&list, &c3);

    list_remove (&list, p1, canary_liberator);
    mem_free (p1);
    list_remove (&list, p3, canary_liberator);
    mem_free (p3);

    CHECK (p2->next == NULL);
    CHECK (p2->previous == NULL);
    CHECK (list.first == p2);
    CHECK (list.last == p2);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 1);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 1);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_remove_5)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 }, c3 = { 5, 6 };
    ListItem *p1, *p2, *p3;

    list_init (&list, sizeof (Canary));

    p1 = list_push_back (&list, &c1);
    p2 = list_push_back (&list, &c2);
    p3 = list_push_back (&list, &c3);

    list_remove (&list, p1, canary_liberator);
    mem_free (p1);
    list_remove (&list, p2, canary_liberator);
    mem_free (p2);
    list_remove (&list, p3, canary_liberator);
    mem_free (p3);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);

    CHECK (list_is_empty (&list));
    CHECK (list_length (&list) == 0);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 0);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_insert_before_1)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 }, c3 = { 5, 6 };
    ListItem *p1, *p2, *p3;

    list_init (&list, sizeof (Canary));

    p1 = list_push_back (&list, &c1);
    p2 = list_push_back (&list, &c2);
    p3 = list_insert_before (&list, p2, &c3);

    CHECK (p2->previous == p3);
    CHECK (p3->next == p2);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 3);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 3);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_insert_before_2)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 }, c3 = { 5, 6 };
    ListItem *p1, *p2, *p3;

    list_init (&list, sizeof (Canary));

    p1 = list_push_back (&list, &c1);
    p2 = list_push_back (&list, &c2);
    p3 = list_insert_before (&list, p1, &c3);

    CHECK (p1->previous == p3);
    CHECK (p3->next == p1);
    CHECK (list.first == p3);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 3);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 3);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_insert_after_1)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 }, c3 = { 5, 6 };
    ListItem *p1, *p2, *p3;

    list_init (&list, sizeof (Canary));

    p1 = list_push_back (&list, &c1);
    p2 = list_push_back (&list, &c2);
    p3 = list_insert_after (&list, p1, &c3);

    CHECK (p1->next == p3);
    CHECK (p3->previous == p1);
    CHECK (p2->previous == p3);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 3);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 3);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_insert_after_2)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 }, c3 = { 5, 6 };
    ListItem *p2, *p3;

    list_init (&list, sizeof (Canary));

    (void) list_push_back (&list, &c1);
    p2 = list_push_back (&list, &c2);
    p3 = list_insert_after (&list, p2, &c3);

    CHECK (p2->next == p3);
    CHECK (p3->previous == p2);
    CHECK (list.last == p3);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 3);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 3);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_emplace_before_1)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    ListItem *p2, *p3;
    Canary *d3;

    list_init (&list, sizeof (Canary));

    (void) list_push_back (&list, &c1);
    p2 = list_push_back (&list, &c2);
    d3 = (Canary*) list_emplace_before (&list, p2);
    p3 = list_item (d3);

    CHECK (p2->previous == p3);
    CHECK (p3->next == p2);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 3);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 3);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_emplace_before_2)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    ListItem *p1, *p3;
    Canary *d3;

    list_init (&list, sizeof (Canary));

    p1 = list_push_back (&list, &c1);
    (void) list_push_back (&list, &c2);
    d3 = (Canary*) list_emplace_before (&list, p1);
    p3 = list_item (d3);

    CHECK (p1->previous == p3);
    CHECK (p3->next == p1);
    CHECK (list.first == p3);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 3);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 3);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_emplace_after_1)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    ListItem *p1, *p2, *p3;
    Canary *d3;

    list_init (&list, sizeof (Canary));

    p1 = list_push_back (&list, &c1);
    p2 = list_push_back (&list, &c2);
    d3 = (Canary*) list_emplace_after (&list, p2);
    p3 = list_item (d3);

    CHECK (p2->next == p3);
    CHECK (p3->previous == p2);
    CHECK (list.first == p1);
    CHECK (list.last == p3);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 3);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 3);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_emplace_after_2)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    ListItem *p1, *p2, *p3;
    Canary *d3;

    list_init (&list, sizeof (Canary));

    p1 = list_push_back (&list, &c1);
    p2 = list_push_back (&list, &c2);
    d3 = (Canary*) list_emplace_after (&list, p1);
    p3 = list_item (d3);

    CHECK (list.first == p1);
    CHECK (p1->next == p3);
    CHECK (p3->previous == p1);
    CHECK (p3->next == p2);

    CHECK (!list_is_empty (&list));
    CHECK (list_length (&list) == 3);

    liberationCounter = 0;
    list_clear (&list, canary_liberator);
    CHECK (liberationCounter == 3);

    CHECK (list.first == NULL);
    CHECK (list.last == NULL);
}

TESTER(list_walk_forward_1)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    int count = 0;
    am_bool rc;

    list_init (&list, sizeof (Canary));

    list_push_back (&list, &c1);
    list_push_back (&list, &c2);

    rc = list_walk_forward (&list, canary_walker_1, &count);
    CHECK (rc);
    CHECK (count == 13);

    list_clear (&list, NULL);
}

TESTER(list_walk_forward_2)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    int count = 0;
    am_bool rc;

    list_init (&list, sizeof (Canary));

    list_push_back (&list, &c1);
    list_push_back (&list, &c2);

    rc = list_walk_forward (&list, canary_walker_2, &count);
    CHECK (rc == AM_FALSE);

    list_clear (&list, NULL);
}

TESTER(list_walk_backward_1)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    int count = 0;
    am_bool rc;

    list_init (&list, sizeof (Canary));

    list_push_back (&list, &c1);
    list_push_back (&list, &c2);

    rc = list_walk_backward (&list, canary_walker_1, &count);
    CHECK (rc);
    CHECK (count == 31);

    list_clear (&list, NULL);
}

TESTER(list_walk_backward_2)
{
    LinkedList list;
    Canary c1 = { 1, 2 }, c2 = { 3, 4 };
    int count = 0;
    am_bool rc;

    list_init (&list, sizeof (Canary));

    list_push_back (&list, &c1);
    list_push_back (&list, &c2);

    rc = list_walk_backward (&list, canary_walker_2, &count);
    CHECK (rc == AM_FALSE);

    list_clear (&list, NULL);
}
