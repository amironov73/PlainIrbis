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
 * Маркер конца меню.
 */
am_byte StopMarker[] = "*****";

/**
 * Разделители для кода меню.
 */
am_byte MenuSeparators[] = { ' ', '-', '=', ':' };

/*=========================================================*/

/* Откусывание от кода фрагмента, начинающегося с разделителя */
static Span trim_code
    (
        Span code
    )
{
    size_t i, j;
    am_byte chr;

    for (i = 0; i < code.len; ++i) {
        chr = code.ptr [i];
        for (j = 0; j < sizeof (MenuSeparators); j++) {
            if (chr == MenuSeparators[j]) {
                return span_slice (code, 0, i);
            }
        }
    }

    return code;
}

/**
 * Инициализация пункта меню.
 *
 * @param entry Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL menu_entry_init
    (
        MenuEntry *entry
    )
{
    assert (entry != NULL);

    buffer_null (&entry->code);
    buffer_null (&entry->comment);
}

/**
 * Освобождение ресурсов.
 *
 * @param entry Указатель на структуру.
 */
MAGNA_API void MAGNA_CALL menu_entry_free
    (
        MenuEntry *entry
    )
{
    assert (entry != NULL);

    buffer_free (&entry->code);
    buffer_free (&entry->comment);
}

/**
 * Получение текстового представления пункта меню.
 *
 * @param entry Пункт меню.
 * @param output Буфер для размещения результата.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL menu_entry_to_string
    (
        const MenuEntry *entry,
        Buffer *output
    )
{
    assert (entry != NULL);
    assert (output != NULL);

    if (!buffer_concat (output, &entry->code)) {
        return AM_FALSE;
    }

    if (entry->comment.position != 0) {
        return buffer_puts(output, " - ")
           && buffer_concat(output, &entry->comment);
    }

    return AM_TRUE;
}

/*=========================================================*/

/**
 * Инициализация меню.
 *
 * @param menu Указатель на неинициализированную структуру.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL menu_init
    (
        MenuFile *menu
    )
{
    assert (menu != NULL);

    return vector_create(&menu->entries, 4);
}

/**
 * Освобождение ресурсов, занимаемых меню.
 *
 * @param menu Меню.
 */
MAGNA_API void MAGNA_CALL menu_free
    (
        MenuFile *menu
    )
{
    size_t index;
    MenuEntry *entry;

    assert (menu != NULL);

    for (index = 0; index < menu->entries.len; ++index) {
        entry = (MenuEntry*) menu->entries.ptr [index];
        menu_entry_free (entry);
        mem_free (entry);
    }

    vector_destroy(&menu->entries);
}

/**
 * Добавление пары "код-комментарий" в конец меню.
 *
 * @param menu Меню.
 * @param code Код (может быть пустым).
 * @param comment Комментарий (может быть пустым).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL menu_append
    (
        MenuFile *menu,
        Span code,
        Span comment
    )
{
    MenuEntry *entry;

    assert (menu != NULL);

    entry = (MenuEntry*) mem_alloc (sizeof (MenuEntry));
    if (entry == NULL) {
        return AM_FALSE;
    }

    menu_entry_init (entry);
    if (!buffer_assign_span (&entry->code, code)
        || !buffer_assign_span (&entry->comment, comment)) {
        menu_entry_free (entry);
        mem_free (entry);
        return AM_FALSE;
    }

    if (!vector_push_back(&menu->entries, entry)) {
        menu_entry_free (entry);
        mem_free (entry);
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * Поиск пункта меню с указанным кодом без учета регистра символов.
 *
 * @param menu Меню.
 * @param code Искомый код.
 * @return Указатель на первый подходящий пункт меню либо `NULL`.
 */
MAGNA_API const MenuEntry* MAGNA_CALL menu_get_entry
    (
        const MenuFile *menu,
        Span code
    )
{
    size_t index;
    const MenuEntry *entry;

    assert (menu != NULL);

    for (index = 0; index < menu->entries.len; ++index) {
        entry = (const MenuEntry*) menu->entries.ptr [index];
        if (buffer_compare_span_ignore_case (&entry->code, code) == 0) {
            return entry;
        }
    }

    code = span_trim (code);

    for (index = 0; index < menu->entries.len; ++index) {
        entry = (const MenuEntry*) menu->entries.ptr [index];
        if (buffer_compare_span_ignore_case (&entry->code, code) == 0) {
            return entry;
        }
    }

    code = trim_code (code);

    for (index = 0; index < menu->entries.len; ++index) {
        entry = (const MenuEntry*) menu->entries.ptr [index];
        if (buffer_compare_span_ignore_case (&entry->code, code) == 0) {
            return entry;
        }
    }

    return NULL;
}

/**
 * Получение комментария для указанного кода без учета регистра символов.
 *
 * @param menu Меню.
 * @param code Искомый код.
 * @param defaultValue Значение, возвращаемое, если пункт меню не найден (может быть пустым).
 * @return Текст комментария (может быть пустым).
 */
MAGNA_API Span MAGNA_CALL menu_get_comment
    (
        const MenuFile *menu,
        Span code,
        Span defaultValue
    )
{
    const MenuEntry *entry;
    Span result;

    assert (menu != NULL);

    result = defaultValue;
    entry = menu_get_entry (menu, code);
    if (entry != NULL) {
        result = buffer_to_span (&entry->comment);
    }

    return result;
}

/**
 * Парсинг текстового представления.
 *
 * @param menu
 * @param texter
 * @return
 */
MAGNA_API am_bool MAGNA_CALL menu_parse
    (
        MenuFile *menu,
        StreamTexter *texter
    )
{
    am_bool result = AM_FALSE;
    Buffer line1 = BUFFER_INIT, line2 = BUFFER_INIT;
    Span span1, span2;
    ssize_t rc1, rc2;

    assert (menu != NULL);
    assert (texter != NULL);

    while (AM_TRUE) {
        buffer_clear (&line1);
        buffer_clear (&line2);
        rc1 = texter_read_line (texter, &line1);
        rc2 = texter_read_line (texter, &line2);
        if (texter->eot) {
            result = AM_TRUE;
            break;
        }

        if (rc1 < 0 || rc2 < 0) {
            break;
        }

        if (buffer_compare_text (&line1, StopMarker) == 0) {
            result = AM_TRUE;
            break;
        }

        span1 = buffer_to_span (&line1);
        span2 = buffer_to_span (&line2);
        if (!menu_append (menu, span1, span2)) {
            break;
        }
    }

    buffer_free (&line1);
    buffer_free (&line2);

    return result;
}

MAGNA_API am_bool MAGNA_CALL menu_to_stream
    (
        const MenuFile *menu,
        Stream *stream
    )
{
    assert (menu != NULL);
    assert (stream != NULL);

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
