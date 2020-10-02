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
 */

/*=========================================================*/

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
    Buffer bindingIndex; // Инвентарный номер подшивки, подполе 'i'.
    MarcField *field; // Поле, из которого извлечена информация.

} Exemplar;

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

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
