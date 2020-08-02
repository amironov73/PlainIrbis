// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

#include <assert.h>

MAGNA_API Array* MAGNA_CALL array_clone (Array *array)
{
    Array *result;
    void *item;
    size_t index;

    assert (array != NULL);

    result = (Array*) malloc (sizeof (Array));
    array_create (result, array->len);
    for (index = 0; index < array->len; ++index) {
        item = array->ptr [index];
        if (array->liberator != NULL) {
            result->ptr [index] = array->cloner (item);
        }
        else {
            result->ptr [index] = item;
        }
    }

    return result;
}

MAGNA_API void MAGNA_CALL array_copy (Array *target, Array *source)
{
    assert (target != NULL);
    assert (source != NULL);

    array_truncate (target, 0);
    array_concat (target, source);
}

MAGNA_API void MAGNA_CALL array_concat (Array *target, Array *source)
{
    size_t index;

    assert (target != NULL);
    assert (source != NULL);

    array_grow (target, target->len + source->len);
    for (index = 0; index < source->len; ++index) {
        target->ptr [target->len] = source->ptr [source->len];
        ++(target->len);
    }
}

MAGNA_API void MAGNA_CALL array_create (Array *array, size_t capacity)
{
    assert (array != NULL);
    assert (capacity > 0);

    array->ptr = (void**) calloc (capacity, sizeof (void*));
    array->len = 0;
    array->capacity = capacity;
}

MAGNA_API void MAGNA_CALL array_free (Array *array)
{
    size_t index;

    assert (array != NULL);

    if (array->liberator != NULL) {
        for (index = 0; index < array->len; ++index) {
            array->liberator (array->ptr [index]);
        }
        array->len = 0;
        array->capacity = 0;
        free (array->ptr);
        array->ptr = NULL;
    }
}

MAGNA_API void* MAGNA_CALL array_get (Array *array, size_t index)
{
    assert (array != NULL);
    assert (index < array->len);

    return NULL;
}

MAGNA_API void MAGNA_CALL array_grow (Array *array, size_t newSize)
{
    void **newPtr;

    assert (array != NULL);
    assert (newSize > 0);

    if (newSize > array->capacity) {
        newPtr = calloc (newSize, sizeof (void*));
        memcpy (newPtr, array->ptr, array->len * sizeof (void*));
        free (array->ptr);
        array->ptr = newPtr;
        array->capacity = newSize;
    }
}

MAGNA_API void* MAGNA_CALL array_pop_back (Array *array)
{
    void *result;

    assert (array != NULL);
    assert (array->len != 0);

    --(array->len);
    result = array->ptr [array->len];

    return result;
}

MAGNA_API void* MAGNA_CALL array_pop_front (Array *array)
{
    size_t index;
    void *result;

    assert (array != NULL);
    assert (array->len != 0);

    result = array->ptr [0];
    --(array->len);
    for (index = 0; index < array->len; ++index) {
        array->ptr [index] = array->ptr [index + 1];
    }

    return result;
}

MAGNA_API void MAGNA_CALL array_push_back (Array *array, void *item)
{
    assert (array != NULL);

    array_grow (array, array->len + 1);
    array->ptr [array->len] = item;
    ++(array->len);
}

MAGNA_API void MAGNA_CALL array_push_front (Array *array, void *item)
{
    size_t index;

    assert (array != NULL);

    array_grow (array, array->len + 1);
    for (index = array->len; index != 0; --index) {
        array->ptr [index] = array->ptr [index - 1];
    }
    array->ptr [0] = item;
    ++(array->len);
}

MAGNA_API void MAGNA_CALL array_set (Array *array, size_t index, void *item)
{
    assert (array != NULL);
    assert (index < array->len);

    if (array->liberator != NULL) {
        array->liberator (array->ptr [index]);
    }

    array->ptr [index] = item;
}

MAGNA_API void MAGNA_CALL array_truncate (Array *array, size_t newSize)
{
    size_t index;

    assert (array != NULL);
    assert (newSize > 0);

    if (array->liberator != NULL) {
        for (index = array->len; index < newSize; ++index) {
            array->liberator (array->ptr [index]);
            array->ptr [index] = NULL;
        }
    }

    array->len = newSize;
}
