// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(chunked_init_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (buffer.chunkSize = 1024);
    CHECK (buffer.first    == NULL);
    CHECK (buffer.current  == NULL);
    CHECK (buffer.last     == NULL);
    CHECK (buffer.position == 0);

    chunked_free (&buffer);
}

TESTER(chunked_init_2)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 0);

    CHECK (buffer.chunkSize = 4096);
    CHECK (buffer.first    == NULL);
    CHECK (buffer.current  == NULL);
    CHECK (buffer.last     == NULL);
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

TESTER(chunked_eof_2)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_write_byte (&buffer, 1);

    CHECK (!chunked_eof (&buffer));
    CHECK (!chunked_empty (&buffer));

    chunked_free (&buffer);
}

TESTER(chunked_eof_3)
{
    am_byte waste [1024];
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_write (&buffer, waste, sizeof (waste));
    chunked_write_byte (&buffer, 1);

    CHECK (!chunked_eof (&buffer));
    CHECK (!chunked_empty (&buffer));

    chunked_free (&buffer);
}

TESTER(chunked_eof_4)
{
    am_byte waste [1024];
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_write (&buffer, waste, sizeof (waste));
    chunked_write_byte (&buffer, 1);
    chunked_rewind (&buffer);

    CHECK (!chunked_eof (&buffer));
    CHECK (!chunked_empty (&buffer));

    chunked_free (&buffer);
}

TESTER(chunked_peek_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_rewind (&buffer);

    CHECK (chunked_peek (&buffer) < 0);

    chunked_free (&buffer);
}

TESTER(chunked_peek_2)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_write_byte (&buffer, 1);
    chunked_write_byte (&buffer, 2);
    chunked_write_byte (&buffer, 3);
    chunked_rewind (&buffer);

    CHECK (chunked_peek (&buffer) == 1);
    CHECK (chunked_peek (&buffer) == 1);

    chunked_free (&buffer);
}

TESTER(chunked_peek_3)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_grow (&buffer, 1024);
    chunked_rewind (&buffer);

    CHECK (chunked_peek (&buffer) < 0);
    CHECK (chunked_peek (&buffer) < 0);

    chunked_free (&buffer);
}

TESTER(chunked_position_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_rewind (&buffer);

    CHECK (chunked_position (&buffer) == 0);

    chunked_free (&buffer);
}

TESTER(chunked_position_2)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_write_byte (&buffer, 1);
    chunked_write_byte (&buffer, 2);
    chunked_write_byte (&buffer, 3);
    chunked_rewind (&buffer);

    chunked_read_byte (&buffer);
    CHECK (chunked_position (&buffer) == 1);

    chunked_read_byte (&buffer);
    CHECK (chunked_position (&buffer) == 2);

    chunked_read_byte (&buffer);
    CHECK (chunked_position (&buffer) == 3);

    chunked_free (&buffer);
}

TESTER(chunked_position_3)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_grow (&buffer, 2 * 1024);
    chunked_write_byte (&buffer, 1);
    chunked_write_byte (&buffer, 2);
    chunked_write_byte (&buffer, 3);
    chunked_rewind (&buffer);

    chunked_read_byte (&buffer);
    CHECK (chunked_position (&buffer) == 1);

    chunked_read_byte (&buffer);
    CHECK (chunked_position (&buffer) == 2);

    chunked_read_byte (&buffer);
    CHECK (chunked_position (&buffer) == 3);

    chunked_free (&buffer);
}

TESTER(chunked_position_4)
{
    am_byte waste [1024];
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_grow (&buffer, 2 * 1024);
    chunked_write (&buffer, waste, sizeof (waste));
    chunked_write_byte (&buffer, 1);
    chunked_write_byte (&buffer, 2);
    chunked_write_byte (&buffer, 3);
    chunked_rewind (&buffer);

    chunked_read_byte (&buffer);
    CHECK (chunked_position (&buffer) == 1);

    chunked_read_byte (&buffer);
    CHECK (chunked_position (&buffer) == 2);

    chunked_read_byte (&buffer);
    CHECK (chunked_position (&buffer) == 3);

    chunked_free (&buffer);
}

TESTER(chunked_position_5)
{
    am_byte waste [1024];
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_grow (&buffer, 2 * 1024);
    chunked_write (&buffer, waste, sizeof (waste));
    chunked_write_byte (&buffer, 1);
    chunked_write_byte (&buffer, 2);
    chunked_write_byte (&buffer, 3);
    chunked_rewind (&buffer);

    CHECK (chunked_read_raw (&buffer, waste, sizeof (waste)) == sizeof (waste));

    chunked_read_byte (&buffer);
    CHECK (chunked_position (&buffer) == 1025);

    chunked_read_byte (&buffer);
    CHECK (chunked_position (&buffer) == 1026);

    chunked_read_byte (&buffer);
    CHECK (chunked_position (&buffer) == 1027);

    chunked_free (&buffer);
}

TESTER(chunked_read_byte_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_rewind (&buffer);

    CHECK (chunked_read_byte (&buffer) < 0);
    CHECK (chunked_read_byte (&buffer) < 0);

    chunked_free (&buffer);
}

TESTER(chunked_read_byte_2)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_write_byte (&buffer, 1);
    chunked_write_byte (&buffer, 2);
    chunked_write_byte (&buffer, 3);
    chunked_rewind (&buffer);

    CHECK (chunked_read_byte (&buffer) == 1);
    CHECK (chunked_read_byte (&buffer) == 2);
    CHECK (chunked_read_byte (&buffer) == 3);
    CHECK (chunked_read_byte (&buffer) < 0);

    chunked_free (&buffer);
}

TESTER(chunked_read_1)
{
    ChunkedBuffer chunked;
    Buffer buffer;

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);
    chunked_rewind (&chunked);

    CHECK (chunked_read (&chunked, &buffer, 10) == 0);

    chunked_free (&chunked);
    buffer_free (&buffer);
}

TESTER(chunkd_read_2)
{
    ChunkedBuffer chunked;
    Buffer buffer;
    am_byte waste[1024];

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);

    chunked_write (&chunked, waste, sizeof (waste));
    chunked_write (&chunked, waste, sizeof (waste));
    chunked_rewind (&chunked);

    CHECK (chunked_read (&chunked, &buffer, sizeof (waste)) == sizeof (waste));
    CHECK (buffer.position == sizeof (waste));
    CHECK (chunked_read (&chunked, &buffer, sizeof (waste)) == sizeof (waste));
    CHECK (buffer.position == sizeof (waste) * 2);
    CHECK (chunked_read (&chunked, &buffer, sizeof (waste)) == 0);
    CHECK (buffer.position == sizeof (waste) * 2);

    chunked_free (&chunked);
    buffer_free (&buffer);
}

TESTER(chunked_read_raw_1)
{
    ChunkedBuffer chunked;
    am_byte buffer [10];

    chunked_init (&chunked, 1024);
    chunked_rewind (&chunked);

    CHECK (chunked_read_raw (&chunked, buffer, 10) == 0);

    chunked_free (&chunked);
}

TESTER(chunked_read_raw_2)
{
    ChunkedBuffer chunked;
    am_byte waste [1024];

    chunked_init (&chunked, 1024);
    chunked_write (&chunked, waste, sizeof (waste));
    chunked_write (&chunked, waste, sizeof (waste));
    chunked_rewind (&chunked);

    CHECK (chunked_read_raw (&chunked, waste, sizeof (waste)) == sizeof (waste));
    CHECK (chunked_read_raw (&chunked, waste, sizeof (waste)) == sizeof (waste));
    CHECK (chunked_read_raw (&chunked, waste, sizeof (waste)) == 0);

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

TESTER(chunked_read_line_2)
{
    ChunkedBuffer chunked;
    Buffer buffer;
    const char *text = "Hello, world!";

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);

    chunked_write_text (&chunked, text);
    chunked_rewind (&chunked);

    CHECK (chunked_read_line (&chunked, &buffer) == strlen (text));
    CHECK (chunked_read_line (&chunked, &buffer) == 0);

    chunked_free (&chunked);
    buffer_free (&buffer);
}

TESTER(chunked_read_line_3)
{
    ChunkedBuffer chunked;
    Buffer buffer;
    const char *text1 = "Hello, world!", *text2 = "Again";

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);

    chunked_write_text (&chunked, text1);
    chunked_write_byte (&chunked, '\r');
    chunked_write_byte (&chunked, '\n');
    chunked_write_text (&chunked, text2);
    chunked_rewind (&chunked);

    CHECK (chunked_read_line (&chunked, &buffer) == strlen (text1));
    CHECK (chunked_read_line (&chunked, &buffer) == strlen (text2));
    CHECK (chunked_read_line (&chunked, &buffer) == 0);

    chunked_free (&chunked);
    buffer_free (&buffer);
}

TESTER(chunked_read_line_4)
{
    ChunkedBuffer chunked;
    Buffer buffer;
    const char *text1 = "Hello, world!", *text2 = "Again";

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);

    chunked_write_text (&chunked, text1);
    chunked_write_byte (&chunked, '\n');
    chunked_write_text (&chunked, text2);
    chunked_rewind (&chunked);

    CHECK (chunked_read_line (&chunked, &buffer) == strlen (text1));
    CHECK (chunked_read_line (&chunked, &buffer) == strlen (text2));
    CHECK (chunked_read_line (&chunked, &buffer) == 0);

    chunked_free (&chunked);
    buffer_free (&buffer);
}

TESTER(chunked_remaining_size_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    chunked_rewind (&buffer);

    CHECK (chunked_remaining_size (&buffer) == 0);

    chunked_free (&buffer);
}

TESTER(chunked_read_remaining_1)
{
    ChunkedBuffer chunked;
    Buffer buffer;

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);
    chunked_rewind (&chunked);

    CHECK (chunked_read_remaining (&chunked, &buffer) == 0);
    CHECK (chunked_read_remaining (&chunked, &buffer) == 0);

    chunked_free (&chunked);
    buffer_free (&buffer);
}

TESTER(chunked_read_remaining_2)
{
    ChunkedBuffer chunked;
    Buffer buffer;
    const char *text1 = "Hello, world!", *text2 = "Again";

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);

    chunked_write_text (&chunked, text1);
    chunked_write_byte (&chunked, '\n');
    chunked_write_text (&chunked, text2);
    chunked_rewind (&chunked);

    CHECK (chunked_read_line (&chunked, &buffer) == strlen (text1));
    CHECK (chunked_read_remaining (&chunked, &buffer) == strlen (text2));
    buffer.position = 0;
    CHECK (chunked_read_remaining (&chunked, &buffer) == strlen (text2));

    chunked_free (&chunked);
    buffer_free (&buffer);
}

TESTER(chunked_read_remaining_3)
{
    ChunkedBuffer chunked;
    Buffer buffer;
    am_byte waste [1024];

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);

    chunked_write (&chunked, waste, sizeof (waste));
    chunked_write (&chunked, waste, sizeof (waste));
    chunked_write (&chunked, waste, sizeof (waste));
    chunked_rewind (&chunked);

    CHECK (chunked_read_raw (&chunked, waste, sizeof (waste)) == sizeof (waste));
    CHECK (chunked_read_remaining (&chunked, &buffer) == sizeof (waste) * 2);

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

TESTER(chunked_size_2)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);
    CHECK (chunked_size (&buffer) == 0);

    chunked_write_byte (&buffer, 1);
    CHECK (chunked_size (&buffer) == 1);
    CHECK (chunked_capacity (&buffer) == buffer.chunkSize);

    chunked_write_byte (&buffer, 2);
    CHECK (chunked_size (&buffer) == 2);
    CHECK (chunked_capacity (&buffer) == buffer.chunkSize);

    chunked_free (&buffer);
}

TESTER(chunked_size_3)
{
    ChunkedBuffer buffer;
    am_byte waste [1024];

    chunked_init (&buffer, 1024);
    CHECK (chunked_size (&buffer) == 0);

    chunked_write (&buffer, waste, sizeof (waste));
    CHECK (chunked_size (&buffer) == sizeof (waste));
    CHECK (chunked_capacity (&buffer) == buffer.chunkSize);

    chunked_write_byte (&buffer, 1);
    CHECK (chunked_size (&buffer) == sizeof (waste) + 1);
    CHECK (chunked_capacity (&buffer) == buffer.chunkSize * 2);

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

TESTER(chunked_at_3)
{
    ChunkedBuffer buffer;
    am_byte waste[1024], *ptr;

    chunked_init (&buffer, 1024);
    chunked_write (&buffer, waste, sizeof (waste));
    chunked_write_byte (&buffer, 123);

    ptr = chunked_at (&buffer, 1024);
    CHECK (ptr != NULL);
    CHECK (*ptr == 123);

    chunked_free (&buffer);
}

TESTER(chunked_at_4)
{
    ChunkedBuffer buffer;
    am_byte waste[1024], *ptr;
    am_size_t position;

    chunked_init (&buffer, 1024);
    chunked_write (&buffer, waste, sizeof (waste) - 2);
    chunked_write_byte (&buffer, 123);
    chunked_write (&buffer, waste, sizeof (waste));

    position = sizeof (waste) - 2;
    ptr = chunked_at (&buffer, position);
    CHECK (ptr != NULL);
    CHECK (*ptr == 123);

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

TESTER(chunked_all_2)
{
    ChunkedBuffer chunked;
    Buffer buffer;

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);
    chunked_write_byte (&chunked, 1);
    chunked_write_byte (&chunked, 2);
    chunked_write_byte (&chunked, 3);

    CHECK (chunked_all (&chunked, &buffer) == 3);
    CHECK (buffer.position == 3);

    chunked_free (&chunked);
    buffer_free (&buffer);
}

TESTER(chunked_all_3)
{
    ChunkedBuffer chunked;
    Buffer buffer;
    am_byte waste [1024];
    am_size_t expected;

    chunked_init (&chunked, 1024);
    buffer_init (&buffer);
    chunked_write (&chunked, waste, sizeof (waste));
    chunked_write (&chunked, waste, sizeof (waste));
    chunked_write_byte (&chunked, 1);
    chunked_write_byte (&chunked, 2);
    chunked_write_byte (&chunked, 3);

    expected = sizeof (waste) * 2 + 3;
    CHECK (chunked_all (&chunked, &buffer) == expected);
    CHECK (buffer.position == expected);

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

TESTER(chunked_write_1)
{
    ChunkedBuffer buffer;
    am_byte waste [1024];

    chunked_init (&buffer, 512);

    CHECK (chunked_write (&buffer, waste, sizeof (waste)));

    chunked_free (&buffer);
}

TESTER(chunked_write_2)
{
    ChunkedBuffer buffer;
    am_byte waste [1024];

    chunked_init (&buffer, 512);

    CHECK (chunked_write (&buffer, waste, 0));

    chunked_free (&buffer);
}

TESTER(chunked_write_utf8_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (chunked_write_utf8 (&buffer, 0x2661));
    CHECK (chunked_size (&buffer) == 3);
    CHECK (chunked_capacity (&buffer) == 1024);

    chunked_free (&buffer);
}

TESTER(chunked_new_line_1)
{
    ChunkedBuffer buffer;

    chunked_init (&buffer, 1024);

    CHECK (chunked_new_line (&buffer));
    CHECK (chunked_size (&buffer) == 1);

    chunked_free (&buffer);
}

TESTER(chunked_stream_1)
{
    Stream stream;

    CHECK (chunked_stream_create (&stream, 1024));
    CHECK (stream_close (&stream));
}

TESTER(chunked_stream_2)
{
    Stream stream;
    am_byte data[16];

    CHECK (chunked_stream_create (&stream, 1024));
    CHECK (stream_write (&stream, data, sizeof (data)));
    CHECK (chunked_stream_rewind (&stream));
    CHECK (stream_read (&stream, data, sizeof (data)) == sizeof (data));
    CHECK (stream_close (&stream));
}
