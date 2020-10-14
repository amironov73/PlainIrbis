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
 * Инициализация.
 *
 * @param spec
 * @param path
 * @param database
 * @param filename
 * @return
 */
MAGNA_API am_bool MAGNA_CALL spec_init
    (
        Specification *spec,
        int path,
        const char *database,
        const char *filename
    )
{
    assert (spec != NULL);
    assert (filename != NULL);

    mem_clear (spec, sizeof (Specification));
    spec->path = path;
    if (database != NULL) {
        if (!buffer_from_text (&spec->database, CBTEXT (database))) {
            return AM_FALSE;
        }
    }

    if (!buffer_from_text (&spec->filename, CBTEXT (filename))) {
        return AM_FALSE;
    }

    return AM_TRUE;
}

MAGNA_API am_bool MAGNA_CALL spec_parse
    (
        Specification *spec,
        Buffer *buffer
    )
{
    assert (spec != NULL);
    assert (buffer != NULL);

    return AM_FALSE;
}

/**
 * Текстовое представление спецификации файла.
 *
 * @param spec Спецификация.
 * @param output Буфер для размещения результата.
 * @return Признак успешности завершения операции.
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
