// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(mem_avail_physical_1)
{
    size_t rc = mem_avail_physical();

    CHECK (rc != 0);
}

TESTER(mem_avail_virtual_1)
{
    size_t rc = mem_avail_virtual();

    CHECK (rc != 0);
}

TESTER(mem_total_installed_1)
{
    size_t rc = mem_total_installed();

    CHECK (rc != 0);
}

TESTER(mem_total_virtual_1)
{
    size_t rc = mem_total_virtual();

    CHECK (rc != 0);
}

TESTER(allocator_init_1)
{
    Allocator allocator;

    CHECK (allocator_init (&allocator, 0));
    CHECK (allocator.chunkSize != 0);
    CHECK (allocator.remaining == allocator.chunkSize - sizeof (AllocatorChunk));
    CHECK (allocator.first != NULL);
    CHECK (allocator.first == allocator.last);

    allocator_free (&allocator);
}

TESTER(allocator_alloc_1)
{
    Allocator allocator;
    void *pointer1, *pointer2, *pointer3, *pointer4;

    CHECK (allocator_init (&allocator, 16));

    pointer1 = allocator_alloc (&allocator, 8);
    CHECK (pointer1 != NULL);

    pointer2 = allocator_alloc (&allocator, 8);
    CHECK (pointer2 != NULL);

    pointer3 = allocator_alloc (&allocator, 8);
    CHECK (pointer3 != NULL);

    pointer4 = allocator_alloc (&allocator, 8);
    CHECK (pointer4 != NULL);

    CHECK (pointer1 != pointer2);
    CHECK (pointer2 != pointer3);
    CHECK (pointer3 != pointer4);
    CHECK (allocator.first != allocator.last);

    allocator_free (&allocator);
}

TESTER(allocator_total_1)
{
    Allocator allocator;

    CHECK (allocator_init (&allocator, 16));
    (void) allocator_alloc (&allocator, 8);
    (void) allocator_alloc (&allocator, 8);
    (void) allocator_alloc (&allocator, 8);
    (void) allocator_alloc (&allocator, 8);

    CHECK (allocator_total (&allocator) > 16);

    allocator_free (&allocator);
}

TESTER(mem_is_small_machine_1)
{
    am_bool small, huge;

    small = mem_is_small_machine ();
    huge  = mem_is_huge_machine  ();

    CHECK (small != huge);
}
