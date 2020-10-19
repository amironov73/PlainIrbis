// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(buffer_create_1)
{
    am_byte data[7] = { 1, 2, 3, 4, 5, 6, 7 };
    Buffer buffer;

    buffer_create (&buffer, data, sizeof (data));
    CHECK (buffer.start != NULL);
    CHECK (buffer_length (&buffer) == sizeof (data));
    CHECK (buffer_capacity (&buffer) >= sizeof (data));

    buffer_destroy (&buffer);
    CHECK (buffer.start   == NULL);
    CHECK (buffer.current == NULL);
    CHECK (buffer.end     == NULL);
}

TESTER(buffer_create_2)
{
    Buffer buffer;

    buffer_create (&buffer, NULL, 0);
    CHECK (buffer.start   == NULL);
    CHECK (buffer.current == NULL);
    CHECK (buffer.end     == NULL);

    buffer_destroy (&buffer);
    CHECK (buffer.start   == NULL);
    CHECK (buffer.current == NULL);
    CHECK (buffer.end     == NULL);
}

TESTER(buffer_static_1)
{
    am_byte data[7] = { 1, 2, 3, 4, 5, 6, 7 };
    Buffer buffer;

    buffer_static (&buffer, data, sizeof (data));
    CHECK (buffer.start == data);
    CHECK (buffer_length (&buffer) == sizeof (data));
    CHECK (buffer_capacity (&buffer) == sizeof (data));
}

TESTER(buffer_static_2)
{
    Buffer buffer;

    buffer_static (&buffer, NULL, 0);
    CHECK (buffer.start   == NULL);
    CHECK (buffer.current == NULL);
    CHECK (buffer.end     == NULL);
}

TESTER(buffer_replace_1)
{
    am_byte data[] = "У попа была собака. Он ее любил";
    Buffer buffer;

    CHECK (buffer_create
        (
            &buffer,
            data,
            sizeof (data) - 1 /* Без завершающего нуля */
        ));
    CHECK (buffer_replace_text (&buffer, CBTEXT ("собака"), CBTEXT ("СБЕРКНИЖКА")));
    CHECK (buffer_compare_text (&buffer, CBTEXT ("У попа была СБЕРКНИЖКА. Он ее любил")) == 0);
    buffer_destroy (&buffer);
}

TESTER(buffer_find_text_1)
{
    am_byte data[] = "У попа была собака. Он ее любил";
    Buffer buffer;
    const am_byte *ptr;

    buffer_static (&buffer, data, sizeof (data) - 1);
    ptr = buffer_find_text (&buffer, CBTEXT ("собака"));
    CHECK ((ptr - data) == 21);
}

TESTER(buffer_copy_1)
{
    am_byte data[] = "У попа была собака. Он ее любил";
    Buffer target = BUFFER_INIT, source;

    buffer_static (&source, data, sizeof (data) - 1);

    CHECK (buffer_copy (&target, &source));
    CHECK (buffer_length (&target) == buffer_length (&source));
    CHECK (memcmp (target.start, source.start, buffer_position (&source)) == 0);

    buffer_destroy (&target);
}

TESTER(buffer_copy_2)
{
    am_byte data1[] = "Съешь ещё этих мягких французских булок";
    am_byte data2[] = "У попа была собака. Он ее любил";
    Buffer target = BUFFER_INIT, source;

    buffer_from_text (&target, data1);
    buffer_static (&source, data2, sizeof (data2) - 1);

    CHECK (buffer_copy (&target, &source));
    CHECK (buffer_position (&target) == buffer_position (&source));
    CHECK (memcmp (target.start, source.start, buffer_position (&source)) == 0);

    buffer_destroy (&target);
}

TESTER(buffer_concat_1)
{
    am_byte data1[] = "Hello, ", data2[] = "world!";
    Buffer buffer1, buffer2, buffer3 = BUFFER_INIT;

    buffer_static (&buffer1, data1, sizeof (data1) - 1);
    buffer_static (&buffer2, data2, sizeof (data2));
    buffer_concat (&buffer3, &buffer1);
    buffer_concat (&buffer3, &buffer2);

    CHECK (buffer_compare_text (&buffer3, CBTEXT ("Hello, world!")));

    buffer_destroy (&buffer3);
}

TESTER(buffer_putc_1)
{
    Buffer buffer = BUFFER_INIT;

    buffer_putc (&buffer, 'H');
    buffer_putc (&buffer, 'e');
    buffer_putc (&buffer, 'l');
    buffer_putc (&buffer, 'l');
    buffer_putc (&buffer, 'o');
    buffer_putc (&buffer, '!');

    CHECK (buffer_compare_text (&buffer, CBTEXT ("Hello!")) == 0);

    buffer_destroy (&buffer);
}

TESTER(buffer_puts_1)
{
    Buffer buffer = BUFFER_INIT;

    buffer_puts (&buffer, CBTEXT ("Hello,"));
    buffer_puts (&buffer, CBTEXT (" "));
    buffer_puts (&buffer, CBTEXT ("world!"));

    CHECK (buffer_compare_text (&buffer, CBTEXT ("Hello, world!")) == 0);

    buffer_destroy (&buffer);
}

TESTER(buffer_from_span_1)
{
    am_byte data[] = { 1, 2, 3, 4, 5, 6, 7 };
    Span span = span_init (data, sizeof (data));
    Buffer buffer;

    buffer_from_span (&buffer, span);

    CHECK (buffer_length (&buffer) == span_length (span));
    CHECK (memcmp (buffer.start, span.start, span_length (span)) == 0);

    buffer_destroy (&buffer);
}

TESTER(buffer_assign_1)
{
    am_byte data[] = { 1, 2, 3, 4, 5, 6, 7 };
    Buffer buffer = BUFFER_INIT;

    buffer_assign (&buffer, data, sizeof (data));

    CHECK (buffer_position (&buffer) == sizeof (data));
    CHECK (buffer.start[0] == 1);
    CHECK (buffer.start[6] == 7);

    buffer_destroy (&buffer);
}

TESTER(buffer_calculate_size_1)
{
    CHECK (buffer_calculate_size (0) == 8);
    CHECK (buffer_calculate_size (1) == 8);
    CHECK (buffer_calculate_size (8) == 8);
    CHECK (buffer_calculate_size (9) == 16);
    CHECK (buffer_calculate_size (100) == 128);
}

TESTER(buffer_clone_1)
{
    Buffer target, source = BUFFER_INIT;

    CHECK (buffer_clone (&target, &source));
    CHECK (target.start == source.start);
    CHECK (buffer_capacity (&target) == buffer_capacity (&source));
}

TESTER(buffer_clone_2)
{
    am_byte data[] = { 1, 2, 3, 4, 5, 6, 7 };
    Buffer target, source;

    buffer_static (&source, data, sizeof (data));
    CHECK (buffer_clone (&target, &source));
    CHECK (memcmp (data, target.start, sizeof (data)) == 0);
    CHECK (buffer_capacity (&target) == buffer_capacity (&source));

    buffer_destroy (&target);
}

TESTER(buffer_from_text_1)
{
    Buffer buffer = BUFFER_INIT;

    CHECK (buffer_from_text (&buffer, NULL));
    CHECK (buffer.start[0] == 0);
    CHECK (buffer.current == buffer.start);

    buffer_destroy (&buffer);
}

TESTER(buffer_from_text_2)
{
    const char *text = "Hello World";
    Buffer buffer = BUFFER_INIT;

    CHECK (buffer_from_text (&buffer, CBTEXT (text)));
    CHECK (buffer_compare_text (&buffer, CBTEXT (text)) == 0);

    buffer_destroy (&buffer);
}

TESTER(buffer_compare_text_1)
{
    const char *text1 = "Hello World", *text2 = "World Hello",
        *text3 = "Again", *text4 = "Hello World2";
    Buffer buffer = BUFFER_INIT;

    CHECK (buffer_from_text (&buffer, CBTEXT (text1)));
    CHECK (buffer_compare_text (&buffer, CBTEXT (text1)) == 0);
    CHECK (buffer_compare_text (&buffer, CBTEXT (text2)) < 0);
    CHECK (buffer_compare_text (&buffer, CBTEXT (text3)) > 0);
    CHECK (buffer_compare_text (&buffer, CBTEXT (text4)) < 0);

    buffer_destroy (&buffer);
}

TESTER(buffer_compare_1)
{
    const char *text1 = "Hello World", *text2 = "Goodbye";
    Buffer buffer1 = BUFFER_INIT, buffer2 = BUFFER_INIT;

    CHECK (buffer_from_text (&buffer1, CBTEXT (text1)));
    CHECK (buffer_from_text (&buffer2, CBTEXT (text2)));
    CHECK (buffer_compare (&buffer1, &buffer1) == 0);
    CHECK (buffer_compare (&buffer2, &buffer2) == 0);
    CHECK (buffer_compare (&buffer1, &buffer2) > 0);

    buffer_destroy (&buffer1);
    buffer_destroy (&buffer2);
}

TESTER(buffer_compare_2)
{
    const char *text1 = "Hello World", *text2 = "World Hello";
    Buffer buffer1 = BUFFER_INIT, buffer2 = BUFFER_INIT;

    CHECK (buffer_from_text (&buffer1, CBTEXT (text1)));
    CHECK (buffer_from_text (&buffer2, CBTEXT (text2)));
    CHECK (buffer_compare (&buffer1, &buffer2) < 0);

    buffer_destroy (&buffer1);
    buffer_destroy (&buffer2);
}

TESTER(buffer_assign_text_1)
{
    const char *text1 = "Hello World", *text2 = "World Hello";
    Buffer buffer = BUFFER_INIT;

    CHECK (buffer_assign_text (&buffer, NULL));
    CHECK (buffer.start[0] == 0);
    CHECK (buffer.current == buffer.start);
    CHECK (buffer_assign_text (&buffer, CBTEXT (text1)));
    CHECK (buffer_compare_text (&buffer, CBTEXT (text1)) == 0);
    CHECK (buffer_assign_text (&buffer, CBTEXT (text2)));
    CHECK (buffer_compare_text (&buffer, CBTEXT (text2)) == 0);

    buffer_destroy (&buffer);
}
