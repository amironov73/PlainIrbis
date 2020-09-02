/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/tester.h"

TESTER(buffer_create_1)
{
    am_byte data[7] = { 1, 2, 3, 4, 5, 6, 7 };
    Buffer buffer;

    buffer_create (&buffer, data, sizeof (data));
    CHECK (buffer.ptr != NULL);
    CHECK (buffer.position == sizeof (data));
    CHECK (buffer.capacity >= sizeof (data));

    buffer_free (&buffer);
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

    buffer_free (&buffer);
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
    buffer_free (&buffer);
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
    buffer_copy (&target, &source);
    CHECK (target.position == source.position);
    CHECK (memcmp (target.ptr, source.ptr, source.position) == 0);
    buffer_free (&target);
}

TESTER(buffer_concat_1)
{
    am_byte data1[] = "Hello, ", data2[] = "world!";
    Buffer buffer1, buffer2, buffer3 = BUFFER_INIT;

    buffer_static (&buffer1, data1, sizeof (data1) - 1);
    buffer_static (&buffer2, data2, sizeof (data2) - 1);
    buffer_concat (&buffer3, &buffer1);
    buffer_concat (&buffer3, &buffer2);
    CHECK (buffer_compare_text (&buffer3, "Hello, world!"));
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
}

TESTER(buffer_puts_1)
{
    Buffer buffer = BUFFER_INIT;

    buffer_puts (&buffer, "Hello,");
    buffer_puts (&buffer, " ");
    buffer_puts (&buffer, "world!");
    CHECK (buffer_compare_text (&buffer, "Hello, world!") == 0);
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
}
