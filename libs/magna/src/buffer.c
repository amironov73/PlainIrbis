// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

#include <assert.h>

size_t buffer_calculate_size (size_t newSize)
{
    if (newSize < 8) {
        newSize = 8;
    }
    return newSize * 2;
}

Buffer* buffer_clone (Buffer *buffer)
{
    assert (buffer != NULL);

    Buffer *result = malloc (sizeof (Buffer));
    result->position = buffer->position;
    result->capacity = buffer->capacity;
    result->ptr = malloc (buffer->capacity);
    memcpy (result->ptr, buffer->ptr, buffer->position);

    return result;
}

void buffer_copy (Buffer *target, Buffer *source)
{
    assert (target != NULL);
    assert (source != NULL);
    buffer_grow (target, source->position);
    memcpy (target->ptr, source->ptr, source->position);
    target->position = source->position;
}

void buffer_concat (Buffer *target, Buffer *source)
{
    assert (target != NULL);
    assert (source != NULL);
}

void buffer_create (Buffer *buffer, char *data, size_t length)
{
    assert (buffer != NULL);
    assert (length >= 0);
    buffer->ptr = NULL;
    buffer->position = 0;
    buffer->capacity = 0;

    if (length != 0) {
        assert (data != NULL);
        buffer_grow (buffer, length);
        memcpy (buffer->ptr, data, length);
        buffer->position = length;
    }
}

void buffer_free (Buffer *buffer)
{
    assert (buffer != NULL);

    if (buffer->ptr) {
        free (buffer->ptr);
        buffer->ptr = NULL;
    }
    buffer->position = 0;
    buffer->capacity = 0;
}

void buffer_grow (Buffer *buffer, size_t newSize)
{
    char *newPtr;

    assert (buffer != NULL);
    assert (newSize >= 0);

    if (newSize > buffer->capacity) {
        newSize = buffer_calculate_size (newSize);
        newPtr = malloc (newSize);
        memcpy (newPtr, buffer->ptr, buffer->position);
        free (buffer->ptr);
        buffer->capacity = newSize;
    }
}

void buffer_putc (Buffer *buffer, char c)
{
    assert (buffer != NULL);
    buffer_grow (buffer, buffer->position + 1);
    buffer->ptr [buffer->position++] = c;
}

void buffer_puts (Buffer *buffer, char *str)
{
    assert (buffer != NULL);
    assert (str != NULL);
    size_t delta = strlen (str);
    buffer_grow (buffer, buffer->position + delta);
    memcpy (buffer->ptr + buffer->position, str, delta);
    buffer->position += delta;
}


void buffer_write (Buffer *buffer, char *data, size_t length)
{
    assert (buffer != NULL);
    assert (data != NULL);
    assert (length >= 0);
    buffer_grow (buffer, buffer->position + length);
    memcpy (buffer->ptr + buffer->position, data, length);
    buffer->position += length;
}
