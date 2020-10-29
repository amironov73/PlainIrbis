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
 * \file fst.c
 *
 * Работа с FST-файлами (ТВП).
 *
 * ТВП состоит из набора строк, каждая из которых содержит
 * следующие три параметра, разделенные знаком пробел:
 *  * формат выборки данных, представленный на языке форматирования системы,
 *  * идентификатор поля(ИП),
 *  * метод индексирования(МИ).
 */

/*=========================================================*/
/**
 * \struct FstLine
 *      \brief Строка FST-файла.
 *      \details Владеет своей памятью
 *      (кроме поля `userData`).
 *      Для освобождения ресурсов необходимо вызвать
 *      `fst_line_destroy`.
 *
 * \var FstLine::format
 *      \brief Формат.
 *
 * \var FstLine::lineNumber
 *      \brief Номер строки.
 *
 * \var FstLine::tag
 *      \brief Метка поля.
 *
 * \var FstLine::method
 *      \brief Метод индексирования.
 *
 * \var FstLine::userData
 *      \brief Произвольные пользовательские данные.
 *      \details Памятью, на которую ссылается это поле,
 *      распоряжается сам пользователь структуры.
 */

/**
 * Простая инициализация структуры.
 * Не выделяет памяти в куче.
 *
 * @param line Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL fst_line_init
    (
        FstLine *line
    )
{
    assert (line != NULL);

    mem_clear (line, sizeof (*line));
}

/**
 * Освобождение ресурсов, занятых структурой.
 *
 * @param line Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL fst_line_destroy
    (
        FstLine *line
    )
{
    assert (line != NULL);

    buffer_destroy (&line->format);
    mem_clear (line, sizeof (*line));
}

/**
 * Инициализация массива FST-строк.
 *
 * @param lines Указатель на неинициализированную структуру массива.
 */
MAGNA_API void MAGNA_CALL fst_line_array_init
    (
        Array *lines
    )
{
    assert (lines != NULL);

    array_init (lines, sizeof (FstLine));
}

/**
 * Освобождение ресурсов, занятых массивом FST-строк.
 *
 * @param lines Указатель на массив, подлежащий освобождению.
 */
MAGNA_API void MAGNA_CALL fst_line_array_destroy
    (
        Array *lines
    )
{
    assert (lines != NULL);

    array_destroy (lines, (Liberator) fst_line_destroy);
}

/*=========================================================*/

/**
 * \struct FstFile
 *      \brief Обертка над FST-файлом (ТВП).
 *      \details Владеет собственной памятью.
 *      Для освобождения ресурсов необходимо вызвать
 *      `fst_file_destroy`.
 *
 * \var FstFile::lines
 *      \brief Динамический массив FST-строк.
 *      См. `FstLine`.
 */

/**
 * Простая иницициализация структуры.
 * Не выделяет память в куче.
 *
 * @param fst Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL fst_file_init
    (
        FstFile *fst
    )
{
    assert (fst != NULL);

    fst_line_array_init (&fst->lines);
}

/**
 * Освобождение ресурсов, занимаемых структурой.
 *
 * @param fst Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL fst_file_destroy
    (
        FstFile *fst
    )
{
    assert (fst != NULL);

    fst_line_array_destroy (&fst->lines);
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
