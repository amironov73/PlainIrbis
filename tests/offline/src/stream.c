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

/*
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
*/

TESTER(stream_copy_4)
{
    Stream target, source;

    CHECK (memory_stream_create (&target));
    CHECK (broken_stream_open (&source));
    CHECK (!stream_copy (&target, &source));
    CHECK (stream_close (&target));
    CHECK (!stream_close (&source));
}

/*
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
*/

TESTER(memory_stream_to_span_1)
{
    Stream memory;
    am_byte data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    Span span;

    CHECK (memory_stream_open (&memory, data, sizeof (data)));
    CHECK (stream_seek (&memory, sizeof (data)));

    span = memory_stream_to_span (&memory);

    CHECK (span.start == data);
    CHECK (span_length (span) == sizeof (data));
    CHECK (stream_close (&memory));
}

TESTER(memory_stream_to_text_1)
{
    Stream memory;
    const char *text = "Hello World";
    ssize_t length = strlen (text);
    am_byte *data;

    CHECK (memory_stream_create (&memory));
    CHECK (stream_write (&memory, CBTEXT (text), length));

    data = memory_stream_to_text (&memory);

    CHECK (data != NULL);
    CHECK (((ssize_t) strlen (CCTEXT (data))) == length);
    CHECK (strcmp (CCTEXT (data), text) == 0);
    CHECK (stream_close (&memory));
}

TESTER(texter_init_1)
{
    Stream memory;
    StreamTexter texter;

    CHECK (memory_stream_create (&memory));
    CHECK (texter_init (&texter, &memory, 0));
    CHECK (texter.stream == &memory);
    CHECK (texter.position == 0);
    CHECK (buffer_is_empty (&texter.buffer));
    CHECK (texter.buffer.start != NULL);

    texter_destroy(&texter);
}

TESTER(texter_init_2)
{
    Stream memory;
    StreamTexter texter;
    am_byte *text = BTEXT ("Hello World");
    ssize_t length = strlen (CCTEXT (text));

    CHECK (memory_stream_open (&memory, text, length));
    CHECK (texter_init (&texter, &memory, 0));
    CHECK (texter.stream == &memory);
    CHECK (texter.position == 0);
    CHECK (buffer_is_empty (&texter.buffer));
    CHECK (texter.buffer.start != NULL);

    texter_destroy(&texter);
}

TESTER(texter_read_byte_1)
{
    Stream memory;
    StreamTexter texter;
    int chr;

    CHECK (memory_stream_create (&memory));
    CHECK (texter_init (&texter, &memory, 0));

    chr = texter_read_byte (&texter);
    CHECK (chr == 0);
    CHECK (texter.eot);

    texter_destroy(&texter);
}

/*
TESTER(texter_read_byte_2)
{
    Stream memory;
    StreamTexter texter;
    am_byte *text = BTEXT ("Hello World");

    CHECK (memory_stream_open (&memory, text, strlen (CCTEXT (text))));
    CHECK (texter_init (&texter, &memory, 0));

    CHECK (texter_read_byte (&texter) == 'H');
    CHECK (texter_read_byte (&texter) == 'e');
    CHECK (texter_read_byte (&texter) == 'l');
    CHECK (texter_read_byte (&texter) == 'l');
    CHECK (texter_read_byte (&texter) == 'o');
    CHECK (texter_read_byte (&texter) == ' ');
    CHECK (texter_read_byte (&texter) == 'W');
    CHECK (texter_read_byte (&texter) == 'o');
    CHECK (texter_read_byte (&texter) == 'r');
    CHECK (texter_read_byte (&texter) == 'l');
    CHECK (texter_read_byte (&texter) == 'd');
    CHECK (texter_read_byte (&texter) == 0);
    CHECK (texter.eot);

    texter_destroy(&texter);
}

TESTER(texter_read_line_1)
{
    Stream memory;
    StreamTexter texter;
    Buffer line = BUFFER_INIT;

    CHECK (memory_stream_create (&memory));
    CHECK (texter_init (&texter, &memory, 0));

    CHECK (texter_read_line (&texter, &line) == 0);
    CHECK (buffer_position (&line) == 0);
    CHECK (texter.eot);

    texter_destroy(&texter);
    buffer_destroy(&line);
}

TESTER(texter_read_line_2)
{
    Stream memory;
    StreamTexter texter;
    am_byte *text = BTEXT ("Hello World");
    Buffer line = BUFFER_INIT;
    size_t length = strlen (CCTEXT (text));

    CHECK (memory_stream_open (&memory, text, length));
    CHECK (texter_init (&texter, &memory, 0));

    CHECK (((size_t) texter_read_line (&texter, &line)) == length);
    CHECK (buffer_position (&line) == length);

    texter_destroy(&texter);
    buffer_destroy(&line);
}

TESTER(texter_read_line_3)
{
    Stream memory;
    StreamTexter texter;
    am_byte *text = BTEXT ("Hello\r\nWorld!");
    Buffer line = BUFFER_INIT;
    size_t length = strlen (CCTEXT (text));

    CHECK (memory_stream_open (&memory, text, length));
    CHECK (texter_init (&texter, &memory, 0));

    CHECK (texter_read_line (&texter, &line) == 5);
    CHECK (buffer_position (&line) == 5);

    buffer_clear (&line);
    CHECK (texter_read_line (&texter, &line) == 6);
    CHECK (buffer_position (&line) == 6);

    texter_destroy(&texter);
    buffer_destroy(&line);
}

TESTER(texter_read_line_4)
{
    Stream memory;
    StreamTexter texter;
    am_byte *text = BTEXT ("Hello\nWorld!");
    Buffer line = BUFFER_INIT;
    size_t length = strlen (CCTEXT (text));

    CHECK (memory_stream_open (&memory, text, length));
    CHECK (texter_init (&texter, &memory, 0));

    CHECK (texter_read_line (&texter, &line) == 5);
    CHECK (buffer_position (&line) == 5);

    buffer_clear (&line);
    CHECK (texter_read_line (&texter, &line) == 6);
    CHECK (buffer_position (&line) == 6);

    texter_destroy(&texter);
    buffer_destroy(&line);
}

TESTER(texter_read_line_5)
{
    Stream memory;
    StreamTexter texter;
    am_byte *text = BTEXT ("Hello\rWorld!");
    Buffer line = BUFFER_INIT;
    size_t length = strlen (CCTEXT (text));

    CHECK (memory_stream_open (&memory, text, length));
    CHECK (texter_init (&texter, &memory, 0));

    CHECK (texter_read_line (&texter, &line) == 5);
    CHECK (buffer_position (&line) == 5);

    buffer_clear (&line);
    CHECK (texter_read_line (&texter, &line) == 6);
    CHECK (buffer_position (&line) == 6);

    texter_destroy(&texter);
    buffer_destroy(&line);
}

TESTER(texter_read_line_6)
{
    Stream memory;
    StreamTexter texter;
    am_byte *text = BTEXT ("Hello\r");
    Buffer line = BUFFER_INIT;
    size_t length = strlen (CCTEXT (text));

    CHECK (memory_stream_open (&memory, text, length));
    CHECK (texter_init (&texter, &memory, 0));

    CHECK (texter_read_line (&texter, &line) == 5);
    CHECK (buffer_position (&line) == 5);

    texter_destroy(&texter);
    buffer_destroy(&line);
}
*/
