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
 * \file worksht.c
 *
 * Работа с WS/WSS-файлами.
 *
 * РЛ полей содержит таблицу  с элементами для ввода/корректировки,
 * имеющую следующие столбцы:
 *
 * Метка - числовая метка поля.
 * Вводится произвольно или выбирается из таблицы полного
 * описания полей, вызываемой по кнопке или команде <F2>.
 * Таблицу полного описания полей предварительно следует загрузить.
 *
 * Наименование - наименование поля.
 * Вводится произвольно или выбирается из таблицы полного
 * описания полей, вызываемой по кнопке или команде <F2>.
 * Таблицу полного описания полей предварительно следует загрузить.
 *
 * Повторяемость поля.
 * Выбирается из предлагаемого списка (1 - повторяющееся,
 * 0 - неповторяющееся).
 *
 * Индекс контекстной помощи
 * - ссылка на текст в Инструкции каталогизатора.
 *
 * Режим ввода
 * - использование расширенных средств ввода. Выбирается
 * из предлагаемого списка. Знак «!» перед значением расширенного
 * средства ввода определяет его обязательное использование.
 * Если указать в качестве режима ввода «!0», то это будет
 * означать запрет на корректировку соответствующего поля.
 *
 * Доп.инф
 * - дополнительная информация для Расширенных средств ввода
 * (АРМ Каталогизатор).
 * Значение и структура данного параметра зависит от значения
 * предыдущего параметра – РЕЖИМ ВВОДА
 *
 * ФЛК
 * - формат ФЛК поля (Приложение 4 п. 12). Указывается в виде
 * непосредственного формата или в виде имени предварительно
 * созданного формата (без расширения) с предшествующим символом «@».
 *
 * Подсказка
 * - текст помощи (инструкции), сопровождающий ввод в поле.
 *
 * Значение по умолчанию (статическое)
 * - значение поля по умолчанию при создании новой записи
 * (статическое значение по умолчанию). Указывается непосредственно
 * или через параметр инициализационного файла в виде:
 * @<SECTION>,<NAME>,<DEFAULT>, где:
 * <SECTION>  - секция инициализационного файла;
 * <NAME>        - имя параметра;
 * <DEFAULT>  - значение параметра по умолчанию.
 *
 * Доп.инф. [резерв]
 * - используется при определенных режимах ввода (1, 2, 3, 6, 10, 11).
 * Определяет правила объединения данных при групповом вводе
 * в одно поле. Может иметь вид:
 * RXXX   - вставлять разделители XXX справа от каждого отобранного
 * элемента, кроме последнего;
 * LXXX    - вставлять разделители XXX слева от каждого отобранного
 * элемента;
 * DXXYY - каждый отобранный элемент заключать слева разделителями
 * XX и справа - YY.
 * Если параметр остается пустым - групповой ввод в одно поле запрещен.
 *
 * Столбцы РЛ подполей:
 * 1. Разделитель - односимвольный идентификатор подполя
 * (латиница или цифра, нет разницы между строчными
 * и прописными буквами).
 * 2. Наименование - название подполя.
 * 3. Повторяемость - единственное допустимое значение
 * 0 - неповторяющееся.
 * 4. Индекс контекстной помощи- то же, что и для РЛ полей.
 * 5. Режим ввода - то же, что и для РЛ полей.
 * 6. Доп.инф. - то же, что и для РЛ полей.
 * 7. ФЛК - не используется.
 * 8. Умолчание - не используется.
 * 9. Подсказка - то же, что и для РЛ полей.
 * 10. [резерв] - то же, что и для РЛ полей.
 *
 */

/*=========================================================*/

/* Элемент рабочего листа (соответствует одной строке ввода). */

typedef struct
{
    Buffer title;
    Buffer help;
    Buffer mode;
    Buffer info;
    Buffer verification;
    Buffer hint;
    Buffer defaultValue;
    Buffer reserved;
    void *userData;
    am_uint32 tag;
    am_bool repeatable;

} WorksheetItem;

MAGNA_API void MAGNA_CALL worksheet_item_init
    (
        WorksheetItem *item
    )
{
    assert (item != NULL);

    mem_clear (item, sizeof (*item));
}

MAGNA_API void MAGNA_CALL worksheet_item_destroy
    (
        WorksheetItem *item
    )
{
    assert (item != NULL);

    buffer_destroy (&item->title);
    buffer_destroy (&item->help);
    buffer_destroy (&item->mode);
    buffer_destroy (&item->info);
    buffer_destroy (&item->verification);
    buffer_destroy (&item->hint);
    buffer_destroy (&item->defaultValue);
    buffer_destroy (&item->reserved);
    mem_clear (item, sizeof (*item));
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
