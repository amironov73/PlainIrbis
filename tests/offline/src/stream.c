// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(null_stream_open_1)
{
    Stream stream;
    am_byte data[16];

    CHECK (null_stream_open (&stream));
    CHECK (stream_read (&stream, data, sizeof (data)) == 0);
    CHECK (stream_write (&stream, data, sizeof (data)));
    CHECK (stream_seek (&stream, 123UL) == 123UL);
    CHECK (stream_tell (&stream) == 0L);
    CHECK (stream_close (&stream));
}

TESTER(null_stream_open_2)
{
    Stream stream;
    am_byte data[16];

    CHECK (null_stream_open (&stream));
    stream.closeFunction = NULL;
    CHECK (stream_close (&stream));
}

TESTER(broken_stream_open_1)
{
    Stream stream;
    am_byte data[16];

    CHECK (broken_stream_open (&stream));
    CHECK (stream_read (&stream, data, sizeof (data)) < 0);
    CHECK (!stream_write (&stream, data, sizeof (data)));
    CHECK (stream_seek (&stream, 123UL) < 0L);
    CHECK (stream_tell (&stream) < 0L);
    CHECK (!stream_close (&stream));
}

TESTER(stream_copy_1)
{
    Stream target, source;

    CHECK (null_stream_open (&target));
    CHECK (null_stream_open (&source));
    CHECK (stream_copy (&target, &source));
    CHECK (stream_close (&target));
    CHECK (stream_close (&source));
}

TESTER(stream_copy_2)
{
    Stream target, source;
    am_byte data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    CHECK (memory_stream_create (&target));
    CHECK (memory_stream_open (&source, data, sizeof(data)));
    CHECK (stream_copy (&target, &source));
    CHECK (stream_close (&target));
    CHECK (stream_close (&source));
}

TESTER(stream_copy_3)
{
    Stream target, source;
    am_byte data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    CHECK (broken_stream_open (&target));
    CHECK (memory_stream_open (&source, data, sizeof(data)));
    CHECK (!stream_copy (&target, &source));
    CHECK (!stream_close (&target));
    CHECK (stream_close (&source));
}

TESTER(stream_copy_4)
{
    Stream target, source;

    CHECK (memory_stream_create (&target));
    CHECK (broken_stream_open (&source));
    CHECK (!stream_copy (&target, &source));
    CHECK (stream_close (&target));
    CHECK (!stream_close (&source));
}

TESTER(memory_seek_function_1)
{
    Stream memory;
    am_byte data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    am_byte chr;

    CHECK (memory_stream_open (&memory, data, sizeof (data)));
    CHECK (stream_seek (&memory, 10UL) == 10L);
    CHECK (stream_read (&memory, &chr, 1) == 1);
    CHECK (chr = 11);
    CHECK (stream_tell (&memory) == 11L);
    CHECK (stream_close (&memory));
}

TESTER(memory_stream_to_span_1)
{
    Stream memory;
    am_byte data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    Span span;

    CHECK (memory_stream_open (&memory, data, sizeof (data)));
    CHECK (stream_seek (&memory, sizeof (data)));
    span = memory_stream_to_span (&memory);
    CHECK (span.ptr == data);
    CHECK (span.len == sizeof (data));
    CHECK (stream_close (&memory));
}

TESTER(memory_stream_to_text_1)
{
    Stream memory;
    const char *text = "Hello World";
    am_ssize_t length = strlen (text);
    am_byte *data;

    CHECK (memory_stream_create (&memory));
    CHECK (stream_write (&memory, text, length));
    data = memory_stream_to_text (&memory);
    CHECK (data != NULL);
    CHECK (strlen (data) == length);
    CHECK (strcmp (data, text) == 0);
    CHECK (stream_close (&memory));
}
