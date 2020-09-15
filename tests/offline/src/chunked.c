/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/tester.h"

TESTER(chunked_init_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (buffer.chunkSize = 1024);
    CHECK (buffer.first == NULL);
    CHECK (buffer.current == NULL);
    CHECK (buffer.last == NULL);
    CHECK (buffer.position == 0);

    chunked_free (&buffer);
}

TESTER(chunked_init_2)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 0);

    CHECK (buffer.chunkSize = 4096);
    CHECK (buffer.first == NULL);
    CHECK (buffer.current == NULL);
    CHECK (buffer.last == NULL);
    CHECK (buffer.position == 0);

    chunked_free (&buffer);
}

TESTER(chunked_eof_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (chunked_eof (&buffer));
    CHECK (chunked_empty (&buffer));

    chunked_free (&buffer);
}

TESTER(chunked_peek_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (chunked_peek (&buffer) < 0);

    chunked_free (&buffer);
}

TESTER(chunked_position_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (chunked_position (&buffer) == 0);

    chunked_free (&buffer);
}

TESTER(chunked_read_byte_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (chunked_read_byte (&buffer) < 0);

    chunked_free (&buffer);
}

TESTER(chunked_read_1)
{
    ChunkedBuffer chunked;
    Buffer buffer;

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);

    CHECK (chunked_read (&chunked, &buffer, 10) == 0);

    chunked_free (&chunked);
    buffer_free (&buffer);
}

TESTER(chunked_read_raw_1)
{
    ChunkedBuffer chunked;
    am_byte buffer [10];

    chunked_init (&chunked, 1024);

    CHECK (chunked_read_raw (&chunked, buffer, 10) == 0);

    chunked_free (&chunked);
}

TESTER(chunked_read_line_1)
{
    ChunkedBuffer chunked;
    Buffer buffer;

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);

    CHECK (chunked_read_line (&chunked, &buffer) == 0);

    chunked_free (&chunked);
    buffer_free (&buffer);
}

TESTER(chunked_remaining_size_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (chunked_remaining_size (&buffer) == 0);

    chunked_free (&buffer);
}

TESTER(chunked_read_remaining_1)
{
    ChunkedBuffer chunked;
    Buffer buffer;

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);

    CHECK (chunked_read_remaining (&chunked, &buffer) == 0);

    chunked_free (&chunked);
    buffer_free (&buffer);
}

TESTER(chunked_rewind_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (chunked_rewind (&buffer) == &buffer);

    chunked_free (&buffer);
}

TESTER(chunked_size_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (chunked_size (&buffer) == 0);
    CHECK (chunked_capacity (&buffer) == 0);

    chunked_free (&buffer);
}

TESTER(chunked_write_buffer_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (chunked_write_byte (&buffer, 1));
    CHECK (buffer.first != NULL);
    CHECK (buffer.current != NULL);
    CHECK (buffer.last != NULL);
    CHECK (buffer.first == buffer.current);
    CHECK (buffer.first == buffer.last);
    CHECK (chunked_size (&buffer) == 1);
    CHECK (chunked_capacity (&buffer) == 1024);

    chunked_free (&buffer);
}

TESTER(chunked_at_1)
{
    ChunkedBuffer buffer;
    am_byte *ptr;

    chunked_init (&buffer, 1024);

    ptr = chunked_at (&buffer, 0);
    CHECK (ptr == NULL);

    ptr = chunked_at (&buffer, 100);
    CHECK (ptr == NULL);

    chunked_free (&buffer);
}

TESTER(chunked_at_2)
{
    ChunkedBuffer buffer;
    am_byte *ptr;

    chunked_init (&buffer, 1024);

    CHECK (chunked_write_byte (&buffer, 123));

    ptr = chunked_at (&buffer, 0);
    CHECK (ptr != NULL);
    CHECK (*ptr == 123);

    ptr = chunked_at (&buffer, 100);
    CHECK (ptr == NULL);

    chunked_free (&buffer);
}

TESTER(chunked_all_1)
{
    ChunkedBuffer chunked;
    Buffer buffer;

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);

    CHECK (chunked_all (&chunked, &buffer) == 0);
    CHECK (buffer.position == 0);

    chunked_free (&chunked);
    buffer_free (&buffer);
}

TESTER(chunked_grow_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (chunked_grow (&buffer, 4096));
    CHECK (chunked_size (&buffer) == 0);
    CHECK (chunked_capacity (&buffer) == 4096);

    chunked_free (&buffer);
}

TESTER(chunked_write_text_1)
{
    ChunkedBuffer buffer;
    const char *message = "Hello World";

    chunked_init (&buffer, 1024);

    CHECK (chunked_write_text (&buffer, message));
    CHECK (chunked_size (&buffer) == strlen (message));
    CHECK (chunked_capacity (&buffer) == 1024);
    CHECK (memcmp (buffer.first->data, message, strlen (message)) == 0);

    chunked_free (&buffer);
}

TESTER(chunked_write_utf8_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (chunked_write_utf8 (&buffer, 0x2661));
    CHECK (chunked_size (&buffer) == 3);
    CHECK (chunked_capacity (&buffer) == 1024);
}
