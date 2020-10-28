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
 * Инициализация (создание пустой) строки INI-файла.
 *
 * @param line Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL ini_line_init
    (
        IniLine *line
    )
{
    assert (line != NULL);

    buffer_init (&line->key);
    buffer_init (&line->value);
    line->modified = AM_FALSE;
}

/**
 * Освобождение ресурсов, занятых строкой INI-файла.
 *
 * @param line Указатель на структуру.
 */
MAGNA_API void MAGNA_CALL ini_line_destroy
    (
        IniLine *line
    )
{
    assert (line != NULL);

    buffer_destroy(&line->key);
    buffer_destroy(&line->value);
}

/**
 * Установка ключа в строке INI-файла.
 *
 * @param line Строка.
 * @param key Новое значение ключа (может быть пустым).
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL ini_line_set_key
    (
        IniLine *line,
        Span key
    )
{
    assert (line != NULL);

    if (buffer_compare_span (&line->key, key) != 0) {
        if (!buffer_assign_span(&line->key, key)) {
            return AM_FALSE;
        }

        line->modified = AM_TRUE;
    }

    return AM_TRUE;
}

/**
 * Установка значения в строке INI-файла.
 *
 * @param line Строка.
 * @param value Новое значение (может быть пустым).
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL ini_line_set_value
    (
        IniLine *line,
        Span value
    )
{
    assert (line != NULL);

    if (buffer_compare_span (&line->value, value) != 0) {
        if (!buffer_assign_span(&line->value, value)) {
            return AM_FALSE;
        }

        line->modified = AM_TRUE;
    }

    return AM_TRUE;
}

/**
 * Преобразование в текст.
 *
 * @param line Строка INI-файла.
 * @param output Буфер для размещения результата.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL ini_line_to_string
    (
        const IniLine *line,
        Buffer *output
    )
{
    assert (line != NULL);
    assert (output != NULL);

    return buffer_concat (output, &line->key)
        && buffer_putc (output, '=')
        && buffer_concat (output, &line->value);
}

/**
 * Верификация строки INI-файла.
 *
 * @param line Строка.
 * @return Результат верификации.
 */
MAGNA_API am_bool MAGNA_CALL ini_line_verify
    (
        const IniLine *line
    )
{
    assert (line != NULL);

    return !buffer_is_empty(&line->key);
}

/*=========================================================*/

/**
 * Инициализация секции INI-файла.
 *
 * @param section Указатель на неинициализированную структуру.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL ini_section_init
    (
        IniSection *section
    )
{
    assert (section != NULL);

    buffer_init (&section->name);

    return array_create (&section->lines, sizeof (IniLine), 4);
}

/**
 * Освобождение ресурсов, занятых секцией INI-файла.
 *
 * @param section Секция.
 */
MAGNA_API void MAGNA_CALL ini_section_destroy
    (
        IniSection *section
    )
{
    assert (section != NULL);

    buffer_destroy(&section->name);
    array_destroy (&section->lines, (Liberator) ini_line_destroy);
}

/**
 * Секция модифицирована? Секция считается модифицированной,
 * если в ней модифицирована хотя бы одна строка.
 *
 * @param section Секция.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL ini_section_is_modified
    (
        const IniSection *section
    )
{
    am_bool result = AM_FALSE;
    size_t index;
    const IniLine *line;

    assert (section != NULL);

    for (index = 0; index < section->lines.len; ++index) {
        line = (const IniLine*) array_get (&section->lines, index);
        if (line->modified) {
            result = AM_TRUE;
            break;
        }
    }

    return result;
}

/**
 * Сброс флага модификации на всех строках секции.
 *
 * @param section Секция.
 */
MAGNA_API void MAGNA_CALL ini_section_not_modified
    (
        IniSection *section
    )
{
    size_t index;
    IniLine *line;

    assert (section != NULL);

    for (index = 0; index < section->lines.len; ++index) {
        line = (IniLine*) array_get (&section->lines, index);
        line->modified = AM_FALSE;
    }
}

/**
 * Удаление всех строк из секции.
 *
 * @param section Секция.
 */
MAGNA_API void MAGNA_CALL ini_section_clear
    (
        IniSection *section
    )
{
    size_t index;
    IniLine *line;

    assert (section != NULL);

    for (index = 0; index < section->lines.len; ++index) {
        line = (IniLine*) array_get (&section->lines, index);
        ini_line_destroy (line);
    }

    array_truncate (&section->lines, 0);
}

/**
 * Поиск первого вхождения строки с указанным ключом.
 *
 * @param section Секция.
 * @param key Искомый ключ. Сравнение без учета регистра.
 * @return Указатель на найденную строку либо `NULL`.
 */
MAGNA_API const IniLine* MAGNA_CALL ini_section_get_line
    (
        const IniSection *section,
        Span key
    )
{
    size_t index;
    const IniLine *line;

    assert (section != NULL);

    for (index = 0; index < section->lines.len; ++index) {
        line = (const IniLine*) array_get (&section->lines, index);
        if (buffer_compare_span_ignore_case (&line->key, key) == 0) {
            return line;
        }
    }

    return NULL;
}

/**
 * Получение значения, соответствующего запрошенному ключу.
 *
 * @param section Секция.
 * @param key Искомый ключ.
 * @param defaultValue Значение, выдаваемое, если строка с ключом не найдена.
 * @return Найденное значение либо значение по умолчанию.
 */
MAGNA_API Span MAGNA_CALL ini_section_get_value
    (
        const IniSection *section,
        Span key,
        Span defaultValue
    )
{
    const IniLine *line;

    assert (section != NULL);

    line = ini_section_get_line (section, key);

    return (line != NULL)
        ? buffer_to_span (&line->value)
        : defaultValue;
}

/**
 * Добавление строки в конец секции.
 * Дублетность не контролируется!
 *
 * @param section Секция.
 * @param key Ключ.
 * @param value Значение (возможно, пустое).
 * @return Указатель на вновь созданную запись либо `NULL`.
 */
MAGNA_API IniLine* MAGNA_CALL ini_section_append_line
    (
        IniSection *section,
        Span key,
        Span value
    )
{
    IniLine *line;

    assert (section != NULL);

    line = (IniLine*) array_emplace_back (&section->lines);
    if (line == NULL) {
        return NULL;
    }

    ini_line_init (line);
    if (!ini_line_set_key (line, key)
        || !ini_line_set_value (line, value)
        || !array_push_back (&section->lines, line)) {
        ini_line_destroy (line);
        --section->lines.len;
        return AM_FALSE;
    }

    return line;
}

/**
 * Установка значения для указанного ключа.
 *
 * @param section Секция.
 * @param key Ключ.
 * @param value Новое значение.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL ini_section_set_value
    (
        IniSection *section,
        Span key,
        Span value
    )
{
    IniLine *line;

    assert (section != NULL);

    line = (IniLine*) ini_section_get_line (section, key);
    if (line == NULL) {
        line = ini_section_append_line (section, key, value);
    }
    else {
        if (!ini_line_set_value (line, value)) {
            return AM_FALSE;
        }
    }

    return (line != NULL);
}

/**
 * Преобразование в текст.
 *
 * @param section Секция INI-файла.
 * @param output Буфер для размещения результата.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL ini_section_to_string
    (
        const IniSection *section,
        Buffer *output
    )
{
    return AM_FALSE;
}

/*=========================================================*/

/**
 * Инициализация INI-файла в памяти.
 *
 * @param file Указатель на неинициализированную структуру.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL ini_file_init
    (
        IniFile *file
    )
{
    assert (file != NULL);

    return array_create (&file->sections, sizeof (IniSection), 4);
}

/**
 * Освобождение ресурсов, занятых INI-файлом.
 *
 * @param file INI-файл.
 */
MAGNA_API void MAGNA_CALL ini_file_destroy
    (
        IniFile *file
    )
{
    assert (file != NULL);

    array_destroy (&file->sections, (Liberator) ini_section_destroy);
}

/**
 * INI-файл модифицирован? Файл считается модифицированным,
 * если в нем модифицирована хотя бы одна секция.
 *
 * @param file INI-файл.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL ini_file_is_modified
    (
        const IniFile *file
    )
{
    am_bool result = AM_FALSE;
    size_t index;
    const IniSection *section;

    assert (file != NULL);

    for (index = 0; index < file->sections.len; ++index) {
        section = (const IniSection*) array_get (&file->sections, index);
        if (ini_section_is_modified (section)) {
            result = AM_TRUE;
            break;
        }
    }

    return result;
}

/**
 * Сброс флага модификации на всех секциях файла.
 *
 * @param file INI-файл.
 */
MAGNA_API void MAGNA_CALL ini_file_not_modified
    (
        IniFile *file
    )
{
    size_t index;
    IniSection *section;

    assert (file != NULL);

    for (index = 0; index < file->sections.len; ++index) {
        section = (IniSection*) array_get (&file->sections, index);
        ini_section_not_modified (section);
    }
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
