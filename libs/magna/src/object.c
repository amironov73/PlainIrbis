// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 * \file object.c
 *
 * Объект.
 *
 */

/*=========================================================*/

/* Простейший объект */

typedef struct
{
    char *name;

} TypeInfo;

typedef void (*Destroyer) (void*);
typedef const TypeInfo* (*GetType) (const void*);
typedef am_bool (*GetString) (const void*, Buffer*);

#define MAGNA_OBJECT_HEADER \
    GetType get_type;       \
    GetString to_string;    \
    Destroyer destroy;      \
    void *auxData;

typedef struct
{
    MAGNA_OBJECT_HEADER

} Object;

/**
 * Инициализация пустого объекта.
 *
 * @param object Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL object_init
    (
        Object *object
    )
{
    assert (object != NULL);

    object->get_type  = NULL;
    object->to_string = NULL;
    object->destroy   = NULL;
}

/**
 * Разрушение объекта.
 *
 * @param object
 */
MAGNA_API void MAGNA_CALL object_destroy
    (
        Object *object
    )
{
    assert (object != NULL);

    if (object->destroy != NULL) {
        object->destroy (object);
        object->destroy = NULL;
    }
}

/**
 * Получение строкового представления объекта.
 *
 * @param object
 * @param buffer
 * @return
 */
MAGNA_API am_bool MAGNA_CALL object_to_string
    (
        const Object *object,
        Buffer *buffer
    )
{
    const TypeInfo *info;

    assert (object != NULL);
    assert (buffer != NULL);

    if (object->to_string != NULL) {
        return object->to_string (object, buffer);
    }

    if (object->get_type != NULL) {
        info = object->get_type (object);
        if (info != NULL) {
            return buffer_puts (buffer, info->name);
        }
    }

    return AM_TRUE;
}

/**
 * Получение типа объекта.
 *
 * @param object Объект.
 * @return Указатель на информацию о типе либо `NULL`.
 */
MAGNA_API const TypeInfo* MAGNA_CALL object_get_type
    (
        const Object *object
    )
{
    assert (object != NULL);

    if (object->to_string != NULL) {
        return object_get_type (object);
    }

    return NULL;
}


/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
