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
 * \file map.c
 *
 * Простой ассоциативный динамический массив.
 *
 * Содержит указатели на элементы (в общем случае могут
 * быть равны `NULL`).
 *
 * Владеет только собственной памятью. Памятью элементов
 * распоряжаются сами элементы. Чтобы разрешить освобождение
 * памяти элементов, необходимо задать `liberator`.
 */

/*=========================================================*/

static am_bool MAGNA_CALL trivial_comparer
    (
        void *firstKey,
        void *secondKey
    )
{
    return firstKey == secondKey;
}

/**
 * Инициализация.
 *
 * @param map Указатель на неинициализированную структуру.
 * @param factor Количество корзин. 0 означает "определяется системой".
 * @param comparer Функция сравнения ключей (опционально).
 * @param hasher Хэш-функция (обязательно!).
 * @param keyLiberator Освободитель ключей (опционально).
 * @param valueLiberator Освободитель значений (опционально).
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL map_create
    (
        Map *map,
        size_t factor,
        KeyComparer comparer,
        HashFunction hasher
    )
{
    assert (map != NULL);
    assert (hasher != NULL);

    if (factor < 2) {
        factor = 17;
    }

    map->factor = factor;
    map->comparer = comparer != NULL ? comparer : trivial_comparer;
    map->hasher = hasher;
    if (!array_create (&map->buckets, sizeof (MapEntry), factor)) {
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * Освобождение ресурсов, занятых ассоциативным массивом.
 *
 * @param map Массив.
 */
MAGNA_API void MAGNA_CALL map_destroy
    (
        Map *map,
        Liberator keyLiberator,
        Liberator valueLiberator
    )
{
    size_t i, j;
    Vector *bucket;
    MapEntry *entry;

    assert (map != NULL);

    for (i = 0; i < map->factor; ++i) {
        bucket = array_get (&map->buckets, i);
        if (bucket != NULL) {
            for (j = 0; j < bucket->len; ++j) {
                entry = (MapEntry *) bucket->ptr [j];
                if (entry != NULL) {
                    if (keyLiberator != NULL) {
                        keyLiberator (entry->key);
                    }

                    if (valueLiberator != NULL) {
                        valueLiberator (entry->value);
                    }
                }
            }

            //vector_destroy (bucket);
            mem_free (bucket);
        }
    }

    //mem_free (map->buckets);
}

/**
 * Получение элемента из массива по ключу.
 *
 * @param map Массив.
 * @param key Ключ.
 * @return Найденный элемент либо `NULL`.
 */
MAGNA_API void* MAGNA_CALL map_get
    (
        const Map *map,
        void *key
    )
{
    size_t hash, index, j;
    Vector *bucket;
    const MapEntry *entry;

    assert (map != NULL);

    hash = map->hasher (key);
    index = hash % map->factor;
    bucket = array_get (&map->buckets, index);
    if (bucket == NULL) {
        return NULL;
    }

    for (j = 0; j < bucket->len; ++j) {
        entry = (const MapEntry*) bucket->ptr[j];
        if (entry != NULL) {
            if (map->comparer (entry->key, key)) {
                return entry->value;
            }
        }
    }

    return NULL;
}

/**
 * Установка значения элемента массива по ключу.
 *
 * @param map Массив.
 * @param key Ключ.
 * @param value Значение.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL map_set
    (
        Map *map,
        void *key,
        void *value,
        Liberator liberator
    )
{
    size_t hash, index, j;
    Vector *bucket;
    MapEntry *entry;

    assert (map != NULL);

    hash = map->hasher (key);
    index = hash % map->factor;
    bucket = array_get (&map->buckets, index);
    if (bucket == NULL) {
        bucket = mem_alloc (sizeof (Vector));
        if (bucket == NULL) {
            return AM_FALSE;
        }

        if (!vector_create(bucket, 4)) {
            return AM_FALSE;
        }
    }

    for (j = 0; j < bucket->len; ++j) {
        entry = (MapEntry *) bucket->ptr [j];
        if (entry != NULL) {
            if (map->comparer (entry->key, key)) {
                if (entry->value != value) {
                    if (liberator != NULL) {
                        liberator (entry->value);
                    }

                    entry->value = value;

                    return AM_TRUE;
                }
            }
        }
    }

    entry = (MapEntry*) mem_alloc (sizeof (MapEntry));
    if (entry == NULL) {
        return AM_FALSE;
    }

    if (!vector_push_back(bucket, entry)) {
        mem_free (entry);
        return AM_FALSE;
    }

    entry->key = key;
    entry->value = value;

    return AM_TRUE;
}

/**
 * Удаление элемента из массива по ключу.
 *
 * @param map Массив.
 * @param key Ключ.
 * @return `AM_TRUE`, если элемент был найден и удалён,
 * `AM_FALSE`, если элемент не был найден.
 */
MAGNA_API am_bool MAGNA_CALL map_unset
    (
        Map *map,
        void *key
    )
{
    assert (map != NULL);

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Прогулка по массиву.
 *
 * @param map Массив.
 * @param walker Функция.
 * @param data Произвольные пользовательские данные.
 * @return `AM_TRUE`, если прогулка дошла до конца,
 * `AM_FALSE`, если была прервана.
 */
MAGNA_API am_bool MAGNA_CALL map_walk
    (
        const Map *map,
        MapWalker walker,
        void *data
    )
{
    size_t i, j;
    const Vector *bucket;
    const MapEntry *entry;

    assert (map != NULL);
    assert (walker != NULL);

    for (i = 0; i < map->factor; ++i) {
        bucket = (const Vector*) array_get (&map->buckets, i);
        if (bucket != NULL) {
            for (j = 0; j < bucket->len; ++j) {
                entry = (const MapEntry*) bucket->ptr [j];
                if (entry != NULL) {
                    if (!walker (data, entry->key, entry->value)) {
                        return AM_FALSE;
                    }
                }
            }
        }
    }

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
