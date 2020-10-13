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
 * \file exemplar.c
 *
 * Сведения об экземпляре.
 *
 *  \var Exemplar::status
 *      \brief Статус экземпляра. Подполе `a`.
 *
 *  \var Exemplar::number
 *      \brief Инвентарный номер. Подполе `b`.
 *
 *  \var Exemplar::date
 *      \brief Дата поступления. Подполе `c`.
 *
 *  \var Exemplar::place
 *      \brief Место хранения. Подполе `d`.
 *
 *  \var Exemplar::collection
 *      \brief Наименование коллекции. Подполе `q`.
 *
 *  \var Exemplar::shelf
 *      \brief Расстановочный шифр (полочный индекс). Подполе `r`.
 *
 *  \var Exemplar::price
 *      \brief Цена экземпляра. Подполе `e`.
 *
 *  \var Exemplar::barcode
 *      \brief Штрих-код или радиометка. Подполе `h`.
 *
 *  \var Exemplar::amount
 *      \brief Количество экземпляров. Подполе `1`.
 *
 *  \var Exemplar::purpose
 *      \brief Специальное назначение фонда. Подполе `t`.
 *
 *  \var Exemplar::coefficient
 *      \brief Коэффициент многоразового использования. Подполе `=`.
 *
 *  \var Exemplar::offBalance
 *      \brief Экземпляры не на баланс. Подполе `4`.
 *
 *  \var Exemplar::ksuNumber1
 *      \brief Номер записи КСУ (поступление). Подполе `u`.
 *
 *  \var Exemplar::actNumber1
 *      \brief Номер акта поступления. Подполе `y`.
 *
 *  \var Exemplar::channel
 *      \brief Канал поступления. Подполе `f`.
 *
 *  \var Exemplar::onHand
 *      \brief Число выданных экземпляров. Подполе `2`.
 *
 *  \var Exemplar::actNumber2
 *      \brief Номер акта списания. Подполе `v`.
 *
 *  \var Exemplar::writeOff
 *      \brief Количество списываемых экземпляров. Подполе `x`.
 *
 *  \var Exemplar::completion
 *      \brief Количество экземпляров для докомплектования. Подполе `k`.
 *
 *  \var Exemplar::actNumber3
 *      \brief Номер акта передачи в другое подразделение. Подполе `w`.
 *
 *  \var Exemplar::moving
 *      \brief Количество передаваемых экземпляров. Подполе `z`.
 *
 *  \var Exemplar::newPlace
 *      \brief Новое место хранения. Подполе `m`.
 *
 *  \var Exemplar::checkedDate
 *      \brief Дата проверки фонда. Подполе `s`.
 *
 *  \var Exemplar::checkedAmount
 *      \brief Число проверенных экземпляров. Подполе `0`.
 *
 *  \var Exemplar::realPlace
 *      \brief Реальное место нахождения книги. Подполе `!`.
 *
 *  \var Exemplar::bindingIndex
 *      \brief Шифр подшивки. Подполе `p`.
 *
 *  \var Exemplar::bindingNumber
 *      \brief Инвентарный номер подшивки. Подполе `i`.
 *
 *  \var Exemplar::field
 *      \brief Поле, из которого извлечена информация.
 */

/*=========================================================*/

#define assign(__b, __f, __c) \
    buffer_assign_span ((__b), field_get_first_subfield_value ((__f), (__c)))

#define apply(__f, __c, __b) \
    field_set_subfield((__f), (__c), buffer_to_span (__b))

typedef struct
{
    Buffer status; // Статус экзмепляра, подполе 'a'.
    Buffer number; // Инвентарный номер, подполе `b`.
    Buffer date; // Дата поступления, подполе 'c'.
    Buffer place; // Место хранения, подполе 'd'.
    Buffer collection; // Наименование коллекции, подполе 'q'.
    Buffer shelf; // Расстановочный шифр (полочный индекс), подполе 'r'.
    Buffer price; // Цена экземпляра, подполе 'e'.
    Buffer barcode; // Штрих-код или радиометка, подполе 'h'.
    Buffer amount; // Количество экземпляров, подполе '1'.
    Buffer purpose; // Специальное назначение фонда, подполе 't'.
    Buffer coefficient; // Коэффициент многоразового использования, подполе '='.
    Buffer offBalance; // Экземпляры не на баланс, подполе '4'.
    Buffer ksuNumber1; // Номер записи КСУ (поступление), подполе 'u'.
    Buffer actNumber1; // Номер акта поступления, подполе 'y'.
    Buffer channel; // Канал поступления, подполе 'f'.
    Buffer onHand; // Число выданных экземпляров, подполе '2'.
    Buffer actNumber2; // Номер акта списания, подполе 'v'.
    Buffer writeOff; // Количество списываемых экземпляров, подполе 'x'.
    Buffer completion; // Количество экземпляров для докомплектования, подполе 'k'.
    Buffer actNumber3; // Номер акта передачи в другое подразделение, подполе 'w'.
    Buffer moving; // Количество передаваемых экземпляров, подполе 'z'.
    Buffer newPlace; // Новое место хранения, подполе 'm'.
    Buffer checkDate; // Дата проверки фонда, подполе 's'.
    Buffer checkedAmount; // Число проверенных экземпляров, подполе '0'.
    Buffer realPlace; // Реальное место нахождения книги, подполе '!'.
    Buffer bindingIndex; // Шифр подшивки, подполе 'p'.
    Buffer bindingNumber; // Инвентарный номер подшивки, подполе 'i'.
    MarcField *field; // Поле, из которого извлечена информация.

} Exemplar;

#define EXEMPLAR_TAG 910

/**
 * Инициализация структуры.
 *
 * @param exemplar Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL exemplar_init
    (
        Exemplar *exemplar
    )
{
    assert (exemplar != NULL);

    mem_clear (exemplar, sizeof (Exemplar));
}

/**
 * Освобождение ресурсов, занятых структурой.
 *
 * @param exemplar Указатель на структуру.
 */
MAGNA_API void MAGNA_CALL exemplar_destroy
    (
        Exemplar *exemplar
    )
{
    assert (exemplar != NULL);

    buffer_destroy (&exemplar->status);
    buffer_destroy (&exemplar->number);
    buffer_destroy (&exemplar->date);
    buffer_destroy (&exemplar->place);
    buffer_destroy (&exemplar->collection);
    buffer_destroy (&exemplar->shelf);
    buffer_destroy (&exemplar->price);
    buffer_destroy (&exemplar->barcode);
    buffer_destroy (&exemplar->amount);
    buffer_destroy (&exemplar->purpose);
    buffer_destroy (&exemplar->coefficient);
    buffer_destroy (&exemplar->offBalance);
    buffer_destroy (&exemplar->ksuNumber1);
    buffer_destroy (&exemplar->actNumber1);
    buffer_destroy (&exemplar->channel);
    buffer_destroy (&exemplar->onHand);
    buffer_destroy (&exemplar->actNumber2);
    buffer_destroy (&exemplar->writeOff);
    buffer_destroy (&exemplar->completion);
    buffer_destroy (&exemplar->actNumber3);
    buffer_destroy (&exemplar->moving);
    buffer_destroy (&exemplar->newPlace);
    buffer_destroy (&exemplar->checkDate);
    buffer_destroy (&exemplar->checkedAmount);
    buffer_destroy (&exemplar->realPlace);
    buffer_destroy (&exemplar->bindingIndex);
    exemplar->field = NULL;
}

/**
 * Применение сведений об экземпляре к указанному полю.
 *
 * @param exemplar Сведения об экземпляре.
 * @param field Поле для заполнения.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL exemplar_apply
    (
        const Exemplar *exemplar,
        MarcField *field
    )
{
    assert (exemplar != NULL);
    assert (field != NULL);

    return apply (field, 'a', &exemplar->status)
        && apply (field, 'b', &exemplar->number)
        && apply (field, 'c', &exemplar->date)
        && apply (field, 'd', &exemplar->place)
        && apply (field, 'q', &exemplar->collection)
        && apply (field, 'r', &exemplar->shelf)
        && apply (field, 'e', &exemplar->price)
        && apply (field, 'h', &exemplar->barcode)
        && apply (field, '1', &exemplar->amount)
        && apply (field, 't', &exemplar->purpose)
        && apply (field, '=', &exemplar->coefficient)
        && apply (field, '4', &exemplar->offBalance)
        && apply (field, 'u', &exemplar->ksuNumber1)
        && apply (field, 'y', &exemplar->actNumber1)
        && apply (field, 'f', &exemplar->channel)
        && apply (field, '2', &exemplar->onHand)
        && apply (field, 'v', &exemplar->actNumber2)
        && apply (field, 'x', &exemplar->writeOff)
        && apply (field, 'k', &exemplar->completion)
        && apply (field, 'w', &exemplar->actNumber3)
        && apply (field, 'z', &exemplar->moving)
        && apply (field, 'm', &exemplar->newPlace)
        && apply (field, 's', &exemplar->checkDate)
        && apply (field, '0', &exemplar->checkedAmount)
        && apply (field, '!', &exemplar->realPlace)
        && apply (field, 'p', &exemplar->bindingIndex)
        && apply (field, 'i', &exemplar->bindingNumber);
}

/**
 * Разбор указанного поля на сведения об экземпляре.
 *
 * @param exemplar Сведения об экземпляре, подлежащие заполнению.
 * @param field Поле для разбора.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL exemplar_parse_field
    (
        Exemplar *exemplar,
        const MarcField *field
    )
{
    assert (exemplar != NULL);
    assert (field != NULL);

    exemplar->field = (MarcField*) field;

    return assign (&exemplar->status,        field, 'a')
        && assign (&exemplar->number,        field, 'b')
        && assign (&exemplar->date,          field, 'c')
        && assign (&exemplar->place,         field, 'd')
        && assign (&exemplar->collection,    field, 'q')
        && assign (&exemplar->shelf,         field, 'r')
        && assign (&exemplar->price,         field, 'e')
        && assign (&exemplar->barcode,       field, 'h')
        && assign (&exemplar->amount,        field, '1')
        && assign (&exemplar->purpose,       field, 't')
        && assign (&exemplar->coefficient,   field, '=')
        && assign (&exemplar->offBalance,    field, '4')
        && assign (&exemplar->ksuNumber1,    field, 'u')
        && assign (&exemplar->actNumber1,    field, 'y')
        && assign (&exemplar->channel,       field, 'f')
        && assign (&exemplar->onHand,        field, '2')
        && assign (&exemplar->actNumber2,    field, 'v')
        && assign (&exemplar->writeOff,      field, 'x')
        && assign (&exemplar->completion,    field, 'k')
        && assign (&exemplar->actNumber3,    field, 'w')
        && assign (&exemplar->moving,        field, 'z')
        && assign (&exemplar->newPlace,      field, 'm')
        && assign (&exemplar->checkDate,     field, 's')
        && assign (&exemplar->checkedAmount, field, '0')
        && assign (&exemplar->realPlace,     field, '!')
        && assign (&exemplar->bindingIndex,  field, 'p')
        && assign (&exemplar->bindingNumber, field, 'i');
}

/**
 * Поиск в записи сведений об экземплярах с последующим их разбором.
 *
 * @param array Массив для размещения сведений об экземплярах.
 * @param record Запись для разбора.
 * @param tag Метка поля, как правило, 910.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL exemplar_parse_record
    (
        Array *array,
        const MarcRecord *record,
        am_uint32 tag
    )
{
    size_t index;
    const MarcField *field;
    Exemplar *exemplar;

    assert (array != NULL);
    assert (record != NULL);
    assert (tag != 0);

    for (index = 0; index < record->fields.len; ++index) {
        field = (const MarcField*) array_get (&record->fields, index);
        if (field->tag == tag) {
            exemplar = (Exemplar*) array_emplace_back (array);
            if (exemplar == NULL) {
                return AM_FALSE;
            }

            exemplar_init (exemplar);
            if (!exemplar_parse_field (exemplar, field)) {
                return AM_FALSE;
            }
        }
    }

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
