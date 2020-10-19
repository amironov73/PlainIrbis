// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(mem_avail_physical_1)
{
    am_uint64 rc = mem_avail_physical ();

    CHECK (rc != 0);
}

TESTER(mem_avail_virtual_1)
{
    am_uint64 rc = mem_avail_virtual ();

    CHECK (rc != 0);
}

TESTER(mem_total_installed_1)
{
    am_uint64 rc = mem_total_installed ();

    CHECK (rc != 0);
}

TESTER(mem_total_virtual_1)
{
    am_uint64 rc = mem_total_virtual ();

    CHECK (rc != 0);
}

TESTER(arena_init_1)
{
    Arena allocator;

    CHECK (arena_init (&allocator, 0));
    CHECK (allocator.chunkSize != 0);
    CHECK (allocator.remaining == allocator.chunkSize - sizeof (ArenaChunk));
    CHECK (allocator.first != NULL);
    CHECK (allocator.first == allocator.last);

    arena_destroy (&allocator);
}

TESTER(allocator_alloc_1)
{
    Arena allocator;
    void *pointer1, *pointer2, *pointer3, *pointer4;

    CHECK (arena_init (&allocator, 16));

    pointer1 = arena_alloc (&allocator, 8);
    CHECK (pointer1 != NULL);

    pointer2 = arena_alloc (&allocator, 8);
    CHECK (pointer2 != NULL);

    pointer3 = arena_alloc (&allocator, 8);
    CHECK (pointer3 != NULL);

    pointer4 = arena_alloc (&allocator, 8);
    CHECK (pointer4 != NULL);

    CHECK (pointer1 != pointer2);
    CHECK (pointer2 != pointer3);
    CHECK (pointer3 != pointer4);
    CHECK (allocator.first != allocator.last);

    arena_destroy (&allocator);
}

TESTER(arena_total_1)
{
    Arena allocator;

    CHECK (arena_init (&allocator, 16));
    (void) arena_alloc (&allocator, 8);
    (void) arena_alloc (&allocator, 8);
    (void) arena_alloc (&allocator, 8);
    (void) arena_alloc (&allocator, 8);

    CHECK (arena_total (&allocator) > 16);

    arena_destroy (&allocator);
}

TESTER(mem_is_small_machine_1)
{
    am_bool small, huge;

    small = mem_is_small_machine ();
    huge  = mem_is_huge_machine  ();

    CHECK (small != huge);
}

static int dummy_function (int arg1, int arg2)
{
    return arg1 + arg2;
}

TESTER(mem_can_execute_1)
{
    CHECK (mem_can_execute ((void*) dummy_function));
}

TESTER(mem_can_read_1)
{
    int junk [100];

    CHECK (mem_can_read (junk, sizeof (junk)));
    CHECK (!mem_can_read (NULL, 1));
}

TESTER(mem_can_write_1)
{
    int junk [100];

    CHECK (mem_can_write (junk, sizeof (junk)));
    CHECK (!mem_can_write (NULL, 1));
}
