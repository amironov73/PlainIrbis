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

/*=========================================================*/

/**
 * \file dbinfo.c
 *
 * Информация о базе данных.
 */

/*=========================================================*/

#define assign(__b, __f, __c) \
    buffer_assign_span ((__b), field_get_first_subfield_value ((__f), (__c)))

#define apply(__f, __c, __b) \
    field_set_subfield((__f), (__c), buffer_to_span (__b))

typedef struct
{
    Buffer name;
    Buffer description;
    Int32Array logicallyDeletedRecords;
    Int32Array physicallyDeletedRecords;
    Int32Array nonActualizedRecords;
    Int32Array lockedRecords;
    am_mfn maxMfn;
    am_bool databaseLocked;
    am_bool readOnly;

} DatabaseInfo;

/* Разбор строки на MFN */
static am_bool parse_line
    (
        Span line,
        Int32Array *array
    )
{
    size_t count = span_count (line, '\x1E') + 1;
    TextNavigator nav;
    Span item;
    am_mfn mfn;

    if (!int32_array_grow (array, count)) {
        return AM_FALSE;
    }

    nav_from_span (&nav, line);
    while (!nav_eot (&nav)) {
        item = nav_read_to (&nav, '\x1E');
        if (!span_is_empty (item)) {
            mfn = span_to_uint32 (item);
            int32_array_push_back (array, mfn);
        }
    }

    return AM_TRUE;
}

/**
 * Инициализация структуры.
 *
 * @param dbinfo Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL dbinfo_init
    (
        DatabaseInfo *dbinfo
    )
{
    assert (dbinfo != NULL);

    mem_clear (dbinfo, sizeof (*dbinfo));
}

/**
 * Освобождение памяти, занимаемой структурой.
 *
 * @param dbinfo Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL dbinfo_destroy
    (
        DatabaseInfo *dbinfo
    )
{
    assert (dbinfo != NULL);

    buffer_destroy (&dbinfo->name);
    buffer_destroy (&dbinfo->description);
    int32_array_destroy (&dbinfo->logicallyDeletedRecords);
    int32_array_destroy (&dbinfo->physicallyDeletedRecords);
    int32_array_destroy (&dbinfo->nonActualizedRecords);
    int32_array_destroy (&dbinfo->lockedRecords);
    mem_clear (dbinfo, sizeof (*dbinfo));
}

/**
 * Разбор меню, например, `dbnam1.mnu`.
 *
 * @param menu Меню.
 * @param array Массив для размещения результата.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL dbinfo_parse_menu
    (
        const MenuFile *menu,
        Array *array
    )
{
    size_t index;
    DatabaseInfo *dbinfo;
    const MenuEntry *entry;

    assert (array != NULL);
    assert (menu != NULL);

    for (index = 0; index < menu->entries.len; ++index) {
        entry = (const MenuEntry *) array_get (&menu->entries, index);
        dbinfo = array_emplace_back (array);
        if (dbinfo == NULL) {
            return AM_FALSE;
        }

        dbinfo_init (dbinfo);
        if (!buffer_copy (&dbinfo->name, &entry->code)
            || !buffer_copy (&dbinfo->description, &entry->comment)) {
            break;
        }

        if (!buffer_is_empty (&dbinfo->name)
            && dbinfo->name.ptr[0] == '-') {
            buffer_remove_at (&dbinfo->name, 0, 1);
            dbinfo->readOnly = AM_TRUE;
        }
    }

    return AM_TRUE;
}

/**
 * Разбор ответа сервера.
 *
 * @param dbinfo Указатель на инициализированную структуру.
 * @param response Ответ сервера.
 * @return Признак успешности операции.
 */
MAGNA_API am_bool MAGNA_CALL dbinfo_parse_response
    (
        DatabaseInfo *dbinfo,
        Response *response
    )
{
    assert (dbinfo != NULL);
    assert (response != NULL);

    if (!parse_line (response_get_line (response), &dbinfo->logicallyDeletedRecords)
        || !parse_line (response_get_line (response), &dbinfo->physicallyDeletedRecords)
        || !parse_line (response_get_line (response), &dbinfo->nonActualizedRecords)
        || !parse_line (response_get_line (response), &dbinfo->lockedRecords))
    {
        return AM_FALSE;
    }

    dbinfo->maxMfn = span_to_uint32 (response_get_line (response));
    dbinfo->databaseLocked = span_to_uint32 (response_get_line (response)) != 0u;

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
