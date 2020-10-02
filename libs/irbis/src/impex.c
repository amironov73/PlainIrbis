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

/*=========================================================*/

/**
 * \file impex.c
 *
 * Импорт-экспорт записей.
 *
 */

/*=========================================================*/

typedef struct IrbisRecordImporter RecordImporter;
typedef struct IrbisRecordExporter RecordExporter;

typedef am_bool (*ImportFunction) (struct IrbisRecordImporter*, MarcRecord*, void*);
typedef am_bool (*ExporterFunction) (struct IrbisRecordImporter*, MarcRecord*, void*);

struct IrbisRecordImporter {
    char *name;
    ImportFunction import;
};

struct IrbisRecordExporter {
    char *name;
    ExporterFunction export;
};

MAGNA_API am_bool import_record_from_text()
{
    return AM_FALSE;
}

MAGNA_API am_bool export_record_to_text ()
{
    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
