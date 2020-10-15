// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(ntc_init_1)
{
    NumberTextChunk chunk;

    ntc_init (&chunk);

    CHECK (chunk.prefix.ptr == NULL);
    CHECK (chunk.prefix.position == 0);
    CHECK (chunk.prefix.capacity == 0);
    CHECK (chunk.value == 0);
    CHECK (chunk.length == 0);
    CHECK (chunk.haveValue == AM_FALSE);

    ntc_destroy (&chunk);
}

TESTER(ntc_free_1)
{
    NumberTextChunk *chunk;

    chunk = mem_alloc (sizeof (NumberTextChunk));
    CHECK (chunk != NULL);

    ntc_init (chunk);
    ntc_destroy (chunk);

    mem_free (chunk);
}

TESTER(ntc_ntc_compare_1)
{
    NumberTextChunk first, second;

    ntc_init (&first);
    ntc_init (&second);

    buffer_assign_text (&first.prefix, CBTEXT ("Hello"));
    buffer_assign_text (&second.prefix, CBTEXT ("World"));
    CHECK (ntc_compare (&first, &second) < 0);

    ntc_destroy (&first);
    ntc_destroy (&second);
}

TESTER(ntc_ntc_compare_2)
{
    NumberTextChunk first, second;

    ntc_init (&first);
    ntc_init (&second);

    buffer_assign_text (&first.prefix, CBTEXT ("Hello"));
    buffer_assign_text (&second.prefix, CBTEXT ("Hello"));
    first.haveValue = AM_TRUE;
    first.value = 123;
    second.haveValue = AM_TRUE;
    second.value = 456;

    CHECK (ntc_compare (&first, &second) < 0);

    ntc_destroy (&first);
    ntc_destroy (&second);
}

TESTER(ntc_have_prefix_1)
{
    NumberTextChunk chunk;

    ntc_init (&chunk);

    CHECK (ntc_have_prefix (&chunk) == AM_FALSE);

    ntc_destroy (&chunk);
}

TESTER(ntc_setup_1)
{
    NumberTextChunk chunk;
    const char *prefix = "Hello", *number = "123";

    ntc_init (&chunk);

    CHECK (ntc_setup (&chunk, TEXT_SPAN (prefix), TEXT_SPAN (number)));

    ntc_destroy (&chunk);
}

TESTER(ntc_setup_2)
{
    NumberTextChunk chunk;
    const char *prefix = "", *number = "123";

    ntc_init (&chunk);

    CHECK (ntc_setup (&chunk, TEXT_SPAN (prefix), TEXT_SPAN (number)));

    ntc_destroy (&chunk);
}

TESTER(ntc_setup_3)
{
    NumberTextChunk chunk;
    const char *prefix = "", *number = "";

    ntc_init (&chunk);

    CHECK (!ntc_setup (&chunk, TEXT_SPAN (prefix), TEXT_SPAN (number)));

    ntc_destroy (&chunk);
}

TESTER(ntc_to_string_1)
{
    NumberTextChunk chunk;
    Buffer buffer;

    ntc_init (&chunk);
    buffer_init (&buffer);

    CHECK (ntc_to_string (&chunk, &buffer));
    CHECK (buffer.position == 0);

    ntc_destroy (&chunk);
    buffer_destroy (&buffer);
}

TESTER(ntc_to_string_2)
{
    NumberTextChunk chunk;
    Buffer buffer;

    ntc_init (&chunk);
    buffer_init (&buffer);
    buffer_assign_text (&chunk.prefix, CBTEXT ("Hello"));
    chunk.haveValue = AM_TRUE;
    chunk.value = 123;

    CHECK (ntc_to_string (&chunk, &buffer));
    CHECK (buffer.position == 8);
    CHECK (buffer_compare_text (&buffer, CBTEXT ("Hello123")) == 0);

    ntc_destroy (&chunk);
    buffer_destroy (&buffer);
}

TESTER(ntc_to_string_3)
{
    NumberTextChunk chunk;
    Buffer buffer;

    ntc_init (&chunk);
    buffer_init (&buffer);
    buffer_assign_text (&chunk.prefix, CBTEXT ("Hello"));

    CHECK (ntc_to_string (&chunk, &buffer));
    CHECK (buffer.position == 5);
    CHECK (buffer_compare_text (&buffer, CBTEXT ("Hello")) == 0);

    ntc_destroy (&chunk);
    buffer_destroy (&buffer);
}

TESTER(ntc_to_string_4)
{
    NumberTextChunk chunk;
    Buffer buffer;

    ntc_init (&chunk);
    buffer_init (&buffer);
    chunk.haveValue = AM_TRUE;
    chunk.value = 123;

    CHECK (ntc_to_string (&chunk, &buffer));
    CHECK (buffer.position == 3);
    CHECK (buffer_compare_text (&buffer, CBTEXT ("123")) == 0);

    ntc_destroy (&chunk);
    buffer_destroy (&buffer);
}

TESTER(number_get_chunk_1)
{
    NumberText number;
    const NumberTextChunk *chunk;

    number_init (&number);
    chunk = number_get_chunk (&number, 0);

    CHECK (chunk == NULL);

    number_destroy (&number);
}
