// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(buffer_create_1)
{
    am_byte data[7] = { 1, 2, 3, 4, 5, 6, 7 };
    Buffer buffer;

    buffer_create (&buffer, data, sizeof (data));
    CHECK (buffer.ptr != NULL);
    CHECK (buffer.position == sizeof (data));
    CHECK (buffer.capacity >= sizeof (data));

    buffer_destroy(&buffer);
    CHECK (buffer.ptr == NULL);
    CHECK (buffer.capacity == 0);
    CHECK (buffer.position == 0);
}

TESTER(buffer_create_2)
{
    Buffer buffer;

    buffer_create (&buffer, NULL, 0);
    CHECK (buffer.ptr == NULL);
    CHECK (buffer.position == 0);
    CHECK (buffer.capacity == 0);

    buffer_destroy(&buffer);
    CHECK (buffer.ptr == NULL);
    CHECK (buffer.capacity == 0);
    CHECK (buffer.position == 0);
}

TESTER(buffer_static_1)
{
    am_byte data[7] = { 1, 2, 3, 4, 5, 6, 7 };
    Buffer buffer;

    buffer_static (&buffer, data, sizeof (data));
    CHECK (buffer.ptr == data);
    CHECK (buffer.position == 0);
    CHECK (buffer.capacity == sizeof (data));
}

TESTER(buffer_static_2)
{
    Buffer buffer;

    buffer_static (&buffer, NULL, 0);
    CHECK (buffer.ptr == NULL);
    CHECK (buffer.position == 0);
    CHECK (buffer.capacity == 0);
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
    CHECK (buffer_replace_text (&buffer, "собака", "СБЕРКНИЖКА"));
    CHECK (buffer_compare_text (&buffer, "У попа была СБЕРКНИЖКА. Он ее любил") == 0);
    buffer_destroy(&buffer);
}

TESTER(buffer_find_text_1)
{
    am_byte data[] = "У попа была собака. Он ее любил";
    Buffer buffer;
    const am_byte *ptr;

    buffer_static (&buffer, data, sizeof (data) - 1);
    buffer.position = buffer.capacity;
    ptr = buffer_find_text (&buffer, "собака");
    CHECK ((ptr - data) == 21);
}

TESTER(buffer_copy_1)
{
    am_byte data[] = "У попа была собака. Он ее любил";
    Buffer target = BUFFER_INIT, source;

    buffer_static (&source, data, sizeof (data) - 1);
    source.position = source.capacity;

    CHECK (buffer_copy (&target, &source));
    CHECK (target.position == source.position);
    CHECK (memcmp (target.ptr, source.ptr, source.position) == 0);

    buffer_destroy(&target);
}

TESTER(buffer_copy_2)
{
    am_byte data1[] = "Съешь ещё этих мягких французских булок";
    am_byte data2[] = "У попа была собака. Он ее любил";
    Buffer target = BUFFER_INIT, source;

    buffer_from_text (&target, data1);
    buffer_static (&source, data2, sizeof (data2) - 1);
    source.position = source.capacity;

    CHECK (buffer_copy (&target, &source));
    CHECK (target.position == source.position);
    CHECK (memcmp (target.ptr, source.ptr, source.position) == 0);

    buffer_destroy(&target);
}

TESTER(buffer_concat_1)
{
    am_byte data1[] = "Hello, ", data2[] = "world!";
    Buffer buffer1, buffer2, buffer3 = BUFFER_INIT;

    buffer_static (&buffer1, data1, sizeof (data1) - 1);
    buffer1.position = buffer1.capacity;
    buffer_static (&buffer2, data2, sizeof (data2));
    buffer2.position = buffer2.capacity;
    buffer_concat (&buffer3, &buffer1);
    buffer_concat (&buffer3, &buffer2);

    CHECK (buffer_compare_text (&buffer3, "Hello, world!"));

    buffer_destroy(&buffer3);
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

    CHECK (buffer_compare_text (&buffer, "Hello!") == 0);

    buffer_destroy(&buffer);
}

TESTER(buffer_puts_1)
{
    Buffer buffer = BUFFER_INIT;

    buffer_puts (&buffer, "Hello,");
    buffer_puts (&buffer, " ");
    buffer_puts (&buffer, "world!");

    CHECK (buffer_compare_text (&buffer, "Hello, world!") == 0);

    buffer_destroy(&buffer);
}

TESTER(buffer_from_span_1)
{
    am_byte data[] = { 1, 2, 3, 4, 5, 6, 7 };
    Span span = span_init (data, sizeof (data));
    Buffer buffer;

    buffer_from_span (&buffer, span);

    CHECK (buffer.ptr == span.ptr);
    CHECK (buffer.position == 0);
    CHECK (buffer.capacity == span.len);
}

TESTER(buffer_assign_1)
{
    am_byte data[] = { 1, 2, 3, 4, 5, 6, 7 };
    Buffer buffer = BUFFER_INIT;

    buffer_assign (&buffer, data, sizeof (data));

    CHECK (buffer.position == sizeof (data));
    CHECK (buffer.ptr[0] == 1);
    CHECK (buffer.ptr[6] == 7);

    buffer_destroy(&buffer);
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

    CHECK (buffer_clone (&target, &source) == &target);
    CHECK (target.ptr == source.ptr);
    CHECK (target.capacity == source.capacity);
}

TESTER(buffer_clone_2)
{
    am_byte data[] = { 1, 2, 3, 4, 5, 6, 7 };
    Buffer target, source;

    buffer_static (&source, data, sizeof (data));
    source.position = source.capacity;
    CHECK (buffer_clone (&target, &source) == &target);
    CHECK (memcmp (data, target.ptr, sizeof (data)) == 0);
    CHECK (target.capacity == source.capacity);

    buffer_destroy(&target);
}

TESTER(buffer_from_text_1)
{
    Buffer buffer = BUFFER_INIT;

    CHECK (buffer_from_text (&buffer, NULL) == &buffer);
    CHECK (buffer.ptr[0] == 0);
    CHECK (buffer.position == 0);

    buffer_destroy(&buffer);
}

TESTER(buffer_from_text_2)
{
    const char *text = "Hello World";
    Buffer buffer = BUFFER_INIT;

    CHECK (buffer_from_text (&buffer, text) == &buffer);
    CHECK (buffer_compare_text (&buffer, text) == 0);

    buffer_destroy(&buffer);
}

TESTER(buffer_compare_text_1)
{
    const char *text1 = "Hello World", *text2 = "World Hello",
        *text3 = "Again", *text4 = "Hello World2";
    Buffer buffer = BUFFER_INIT;

    CHECK (buffer_from_text (&buffer, text1) == &buffer);
    CHECK (buffer_compare_text (&buffer, text1) == 0);
    CHECK (buffer_compare_text (&buffer, text2) < 0);
    CHECK (buffer_compare_text (&buffer, text3) > 0);
    CHECK (buffer_compare_text (&buffer, text4) < 0);

    buffer_destroy(&buffer);
}

TESTER(buffer_compare_1)
{
    const char *text1 = "Hello World", *text2 = "Goodbye";
    Buffer buffer1 = BUFFER_INIT, buffer2 = BUFFER_INIT;

    CHECK (buffer_from_text (&buffer1, text1) == &buffer1);
    CHECK (buffer_from_text (&buffer2, text2) == &buffer2);
    CHECK (buffer_compare (&buffer1, &buffer1) == 0);
    CHECK (buffer_compare (&buffer2, &buffer2) == 0);
    CHECK (buffer_compare (&buffer1, &buffer2) > 0);

    buffer_destroy(&buffer1);
    buffer_destroy(&buffer2);
}

TESTER(buffer_compare_2)
{
    const char *text1 = "Hello World", *text2 = "World Hello";
    Buffer buffer1 = BUFFER_INIT, buffer2 = BUFFER_INIT;

    CHECK (buffer_from_text (&buffer1, text1) == &buffer1);
    CHECK (buffer_from_text (&buffer2, text2) == &buffer2);
    CHECK (buffer_compare (&buffer1, &buffer2) < 0);

    buffer_destroy(&buffer1);
    buffer_destroy(&buffer2);
}

TESTER(buffer_read_1)
{
    am_byte data [10];
    Buffer buffer = BUFFER_INIT;

    CHECK (buffer_read (&buffer, data, sizeof (data)) == 0);
}

TESTER(buffer_read_2)
{
    am_byte data [10];
    Buffer buffer = BUFFER_INIT;

    CHECK (buffer_from_text (&buffer, "Hello, World!") == &buffer);
    buffer.position = 0;

    CHECK (buffer_read (&buffer, data, sizeof (data)) == sizeof (data));
    CHECK (data[0] == 'H');
    CHECK (data[1] == 'e');
    CHECK (data[2] == 'l');
    CHECK (data[3] == 'l');
    CHECK (data[4] == 'o');
    CHECK (data[5] == ',');
    CHECK (data[6] == ' ');
    CHECK (data[7] == 'W');
    CHECK (data[8] == 'o');
    CHECK (data[9] == 'r');

    buffer_destroy(&buffer);
}

TESTER(buffer_assign_text_1)
{
    const char *text1 = "Hello World", *text2 = "World Hello";
    Buffer buffer = BUFFER_INIT;

    CHECK (buffer_assign_text (&buffer, NULL));
    CHECK (buffer.ptr[0] == 0);
    CHECK (buffer.position == 0);
    CHECK (buffer_assign_text (&buffer, text1));
    CHECK (buffer_compare_text (&buffer, text1) == 0);
    CHECK (buffer_assign_text (&buffer, text2));
    CHECK (buffer_compare_text (&buffer, text2) == 0);

    buffer_destroy(&buffer);
}
