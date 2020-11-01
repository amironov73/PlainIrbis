// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/fields.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 * \file title.c
 *
 * Заглавие книги/статьи.
 *
 * \struct Title
 *      \brief Информация о заглавии, поле 200.
 *      \details Структура владеет собственной памятью
 *      (кроме поля `field`), для освобождения ресурсов
 *      необходимо вызвать `title_destroy`.
 *
 * \var Title::number
 *      \brief Обозначение и номер тома, подполе V.
 *
 * \var Title::title
 *      \brief Собственно заглавие, подполе A.
 *
 * \var Title::specific
 *      \brief Нехарактерное заглавие, подполе U.
 *
 * \var Title::general
 *      \brief Общее обозначение материала, подполе B.
 *      \warning В современных версиях ИРБИС подполе не используется.
 *
 * \var Title::subtitle
 *      \brief Сведения, относящиеся к заглавию, подполе E.
 *
 * \var Title::first
 *      \brief Первые сведения об ответственности, подполе F.
 *
 * \var Title::other
 *      \brief Поледующие сведения об ответственности, подполе G.
 *
 * \var Title::field
 *      \brief Поле, из которого была извлечена информация.
 *      \warning Структура не владеет этим указателем!
 */

/*=========================================================*/

#define assign(__b, __f, __c) \
    buffer_assign_span ((__b), field_get_first_subfield_value ((__f), (__c)))

#define apply(__f, __c, __b) \
    field_set_subfield((__f), (__c), buffer_to_span (__b))

/*=========================================================*/

/**
 * Простая инициализация структуры.
 * Не выделяет памяти в куче.
 *
 * @param title Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL title_init
    (
            Title *title
    )
{
    assert (title != NULL);

    mem_clear (title, sizeof (*title));
}

/**
 * Освобождение ресурсов, занимаемых структурой.
 *
 * @param title Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL title_destroy
    (
            Title *title
    )
{
    assert (title != NULL);

    buffer_destroy (&title->number);
    buffer_destroy (&title->title);
    buffer_destroy (&title->specific);
    buffer_destroy (&title->general);
    buffer_destroy (&title->subtitle);
    buffer_destroy (&title->first);
    buffer_destroy (&title->other);
    mem_clear (title, sizeof (*title));
}

/**
 * Применение области заглавия к указанному полю.
 *
 * @param title Структура заглавия.
 * @param field Поле, подлежащее заполнению элементами заглавия.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL title_apply
    (
        const Title *title,
        MarcField *field
    )
{
    assert (title != NULL);
    assert (field != NULL);

    field_clear (field);
    return apply (field, 'v', &title->number)
        && apply (field, 'a', &title->title)
        && apply (field, 'u', &title->specific)
        && apply (field, 'b', &title->general)
        && apply (field, 'e', &title->subtitle)
        && apply (field, 'f', &title->first)
        && apply (field, 'g', &title->other);
}

/**
 * Разбор указанного поля на элементы заглавия.
 *
 * @param title Структура заглавия, подлежащая заполнению.
 * @param field Поле для разбора.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL title_parse_field
    (
            Title *title,
            const MarcField *field
    )
{
    assert (title != NULL);
    assert (field != NULL);

    title->field = (MarcField*) field;
    return assign (&title->number,   field, 'v')
        && assign (&title->title,    field, 'a')
        && assign (&title->specific, field, 'u')
        && assign (&title->general,  field, 'b')
        && assign (&title->subtitle, field, 'e')
        && assign (&title->first,    field, 'f')
        && assign (&title->other,    field, 'g');
}

/**
 * Верификация структуры заглавия.
 *
 * @param title Структура, подлежащая проверке.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL title_verify
    (
        const Title *title
    )
{
    assert (title != NULL);

    return !buffer_is_empty (&title->title)
        || !buffer_is_empty (&title->number);
}

/**
 * Вывод заглавия в консоль.
 *
 * @param title Структура заглавия.
 */
MAGNA_API void MAGNA_CALL title_to_console
    (
        const Title *title
    )
{
    am_bool first = AM_TRUE;

    assert (title != NULL);

    if (!buffer_is_empty (&title->number)) {
        buffer_to_console (&title->number);
        putc ('.', stdout);

        first = AM_FALSE;
    }

    if (!buffer_is_empty (&title->title)) {
        if (!first) {
            putc (' ', stdout);
        }

        buffer_to_console (&title->title);
    }

    if (!buffer_is_empty (&title->general)) {
        puts (" [");
        buffer_to_console (&title->general);
        putc (']', stdout);
    }

    if (!buffer_is_empty (&title->subtitle)) {
        puts (" : ");
        buffer_to_console (&title->subtitle);
    }

    if (!buffer_is_empty (&title->first)) {
        puts (" / ");
        buffer_to_console (&title->first);
    }

    if (!buffer_is_empty (&title->other)) {
        puts (" ; ");
        buffer_to_console (&title->other);
    }
}

/**
 * Текстовое представление заглавия.
 *
 * @param title Структура заглавия.
 * @param output Буфер для размещения результата.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL title_to_string
    (
        const Title *title,
        Buffer *output
    )
{
    am_bool first = AM_TRUE;

    assert (title != NULL);
    assert (output != NULL);

    if (!buffer_is_empty (&title->number)) {
        if (!buffer_combine (output, NULL, &title->number, CBTEXT ("."))) {
            return AM_FALSE;
        }

        first = AM_FALSE;
    }

    if (!buffer_is_empty (&title->title)) {
        if (!first) {
            if (!buffer_putc (output, ' ')) {
                return AM_FALSE;
            }
        }

        if (!buffer_concat (output, &title->title)) {
            return AM_FALSE;
        }
    }

    if (!buffer_is_empty (&title->general)) {
        if (!buffer_combine (output, CBTEXT (" ["), &title->general, CBTEXT("]"))) {
            return AM_FALSE;
        }
    }

    if (!buffer_is_empty (&title->subtitle)) {
        if (!buffer_combine (output, CBTEXT (" : "), &title->subtitle, NULL)) {
            return AM_FALSE;
        }
    }

    if (!buffer_is_empty (&title->first)) {
        if (!buffer_combine (output, CBTEXT (" / "), &title->first, NULL)) {
            return AM_FALSE;
        }
    }

    if (!buffer_is_empty (&title->other)) {
        if (!buffer_combine (output, CBTEXT (" ; "), &title->other, NULL)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
