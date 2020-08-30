/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

#include <assert.h>

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

MAGNA_API Specification* MAGNA_CALL spec_init
    (
        Specification *spec,
        int path,
        const char *database,
        const char *filename
    )
{
    assert (spec != NULL);

    memset (spec, 0, sizeof (Specification));

    if (database != NULL) {
        buffer_from_text (&spec->database, database);
    }

    buffer_from_text (&spec->filename, filename);

    return spec;
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

MAGNA_API Buffer* MAGNA_CALL spec_to_string
    (
        const Specification *spec,
        Buffer *buffer
    )
{
    assert (spec != NULL);
    assert (buffer != NULL);

    return buffer;
}

MAGNA_API am_bool MAGNA_CALL spec_verify
    (
        const Specification *spec
    )
{
    assert (spec != NULL);

    return AM_FALSE;
}
