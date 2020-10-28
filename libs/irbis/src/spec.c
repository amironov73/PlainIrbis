// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/irbis.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>
#include <stdarg.h>
#include <ctype.h>

/*=========================================================*/

/**
 * \file spec.c
 *
 * Спецификация файла на сервере.
 *
 *  Путь на файл `path.database.filename`
 *
 *  `path` – код путей:
 *  0 – общесистемный путь.
 *  1 – путь размещения сведений о базах данных сервера ИРБИС64
 *  2 – путь на мастер-файл базы данных.
 *  3 – путь на словарь базы данных.
 *  10 – путь на параметрию базы данных.
 *
 *  `database` – имя базы данных
 *
 *  `filename` – имя требуемого файла с расширением
 *
 *  В случае чтения ресурса по пути 0 и 1 имя базы данных не задается.
 *
 *  В общем случае может выглядеть так:
 *
 *  `path.database.@filename`
 *
 *  или
 *
 *  `path.database.&filename&content`
 *
 *  где `@` означает двоичный файл
 *  `&` означает наличие содержимого и отделяет имя файла от его содержимого
 */

/*=========================================================*/

/**
 * Инициализация структуры указанными значениями.
 * Выделяет память в куче для сохранения копий значений.
 *
 * @param spec Указатель на структуру, подлежащую инициализации.
 * @param path Код ИРБИС-пути.
 * @param database Имя базы данных (в некоторых сценариях должно быть пустым).
 * @param filename Имя файла (не должно быть пустым.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL spec_create
    (
        Specification *spec,
        int path,
        const am_byte *database,
        const am_byte *filename
    )
{
    assert (spec != NULL);
    assert (filename != NULL);

    mem_clear (spec, sizeof (*spec));
    spec->path = path;
    if (database != NULL) {
        if (!buffer_from_text (&spec->database, CBTEXT (database))) {
            return AM_FALSE;
        }
    }

    return buffer_from_text (&spec->filename, CBTEXT (filename));
}

/**
 * Простая инцииализация структуры.
 * Не выделяет память в куче.
 *
 * @param spec Указатель на структуру, подлежащую инициализации.
 */
MAGNA_API void MAGNA_CALL spec_init
    (
        Specification *spec
    )
{
    assert (spec != NULL);

    mem_clear (spec, sizeof (*spec));
}

/**
 * Освобождение ресурсов, занимаемых структурой.
 *
 * @param spec Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL spec_destroy
    (
        Specification *spec
    )
{
    assert (spec != NULL);

    buffer_destroy (&spec->database);
    buffer_destroy (&spec->filename);
    buffer_destroy (&spec->content);
    mem_clear (spec, sizeof (*spec));
}

/**
 * Разбор текстового представления спецификации файла.
 *
 * @param spec Указатель на инициализированную структуру.
 * @param buffer
 * @return
 */
MAGNA_API am_bool MAGNA_CALL spec_parse
    (
        Specification *spec,
        const Buffer *buffer
    )
{
    assert (spec != NULL);
    assert (buffer != NULL);

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Текстовое представление спецификации файла.
 *
 * @param spec Спецификация.
 * @param output Буфер для размещения результата.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL spec_to_string
    (
        const Specification *spec,
        Buffer *output
    )
{
    assert (spec != NULL);
    assert (output != NULL);

    if (!buffer_put_uint32 (output, spec->path)
        || !buffer_putc (output, '.')) {
        return AM_FALSE;
    }

    if (spec->path > 1) {
        if (!buffer_concat (output, &spec->database)) {
            return AM_FALSE;
        }
    }

    if (!buffer_putc (output, '.')) {
        return AM_FALSE;
    }

    if (spec->binary) {
        if (!buffer_putc (output, '@')) {
            return AM_FALSE;
        }
    }

    if (!buffer_concat (output, &spec->filename)) {
        return AM_FALSE;
    }

    if (!buffer_is_empty (&spec->content)) {
        if (!buffer_putc (output, '&')
            || !buffer_concat (output, &spec->content)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

MAGNA_API am_bool MAGNA_CALL spec_verify
    (
        const Specification *spec
    )
{
    assert (spec != NULL);

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
