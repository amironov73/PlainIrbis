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
 * Простая объектная система.
 *
 */

/*=========================================================*/

/* Простейший объект */

typedef void (*Destroyer) (void*);
typedef am_bool (*GetString) (const void*, Buffer*);
typedef void* (*GetInterface) (const void*, const char*, void*);

typedef struct
{
    char *name;
    GetString to_string;
    GetInterface get_interface;
    Destroyer destroy;

} TypeInfo;

typedef const TypeInfo* (*GetType) (const void*);

#define MAGNA_OBJECT_HEADER \
    GetType get_type;

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
    const TypeInfo *type;

    assert (object != NULL);

    if (object->get_type != NULL) {
        type = object->get_type (object);
        if (type != NULL) {
            if (type->destroy != NULL) {
                type->destroy (object);
            }
        }
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
    const TypeInfo *type;

    assert (object != NULL);
    assert (buffer != NULL);

    if (object->get_type != NULL) {
        type = object->get_type (object);
        if (type != NULL) {
            return type->to_string (object, buffer);
        }

        return buffer_puts (buffer, type->name);
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

    if (object->get_type != NULL) {
        return object->get_type (object);
    }

    return NULL;
}

/**
 * Запрос именованного интерфейса у объекта.
 *
 * @param object Объект.
 * @param name Имя интерфейса.
 * @param auxData Произвольные пользовательские данные (могут быть `NULL`).
 * @return Запрошенный интерфейс либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL object_get_interface
    (
        const Object *object,
        const char *name,
        void *auxData
    )
{
    const TypeInfo *type;
    void *result = NULL;

    assert (object != NULL);
    assert (name != NULL);

    type = object_get_type (object);
    if (type != NULL) {
        if (type->get_interface != NULL) {
            result = type->get_interface (object, name, auxData);
        }
    }

    return result;

}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
