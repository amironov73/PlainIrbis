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
#include <ctype.h>

/*=========================================================*/

/**
    \file subfield.c

    Подполе записи. Состоит из кода и значения.

    > **ПОДПОЛЕ** (Subfield) – определенная единица информации в пределах поля.
    >
    > **ЭЛЕМЕНТ ДАННЫХ** (Data Element) – наименьшая единица библиографической
    > записи, подлежащая точной идентификации. В переменном поле элемент данных
    > идентифицируется идентификатором подполя, с помощью которого образуется
    > подполе. В маркере записи (Record Label), справочнике (Directory)
    > и в подполях фиксированной длины элементы данных, состоящие из кодов,
    > идентифицируются соответствующими позициями своих символов.
    >
    > **ИДЕНТИФИКАТОР ПОДПОЛЯ** (Subfield Identifier) или **КОД ПОДПОЛЯ**
    > (Subfield code) – код, идентифицирующий отдельные подполя внутри переменного
    > поля. Состоит из двух символов. Первый символ – разделитель (Delimiter),
    > всегда один и тот же уникальный символ, установленный по ISO 2709,
    > второй символ – код подполя (Subfield code), который может быть цифровым
    > или буквенным.
    >
    > **ПОДПОЛЕ ФИКСИРОВАННОЙ ДЛИНЫ** (Fixed Length Subfield) – подполе
    > постоянной длины, все случаи применения которого определены положениями
    > формата. Подполе фиксированной длины может быть определено как содержащее
    > один или более элементов данных. Подполя фиксированной длины могут
    > присутствовать в фиксированных полях, например, поле 100,
    > подполе `$a`, и в переменных полях, например, поле 200, подполе `$z`.
    >
    > *Стандарт RUSMARC*

    В стандарте RUSMARC принято ссылаться на подполя `$a`, `$b` и т. д.
    В документации ИРБИС64 принято обозначение `^a`, `^b` и т. д.
    Мы будем придерживаться последнего обозначения.

    Стандарт допускает лишь как правило алфавитно-цифровые код
    подполей `A-Z, 0-9`, но в ИРБИС64 бывают подполя с экзотическими
    кодами вроде `!`, `(` и др.

    Стандарт не допускает пустого значения поля, но на практике в ИРБИС64
    такие подполя могут встречаться (возникают при сбоях и при нарушении
    технологии).

    Коды подполей не чувствительны к регистру. Как правило, ИРБИС64
    приводит коды к верхнему регистру, но это не точно. :)

    ИРБИС64 трактует код подполя `*` как "данные до первого разделителя
    либо значение первого по порядку подполя" (смотря по тому,
    что присутствует в записи).

    Подполе владеет собственной памятью, для освобождения которой
    необходимо вызвать `subfield_destroy`.

 */

#include <assert.h>
#include <ctype.h>

/*=========================================================*/

/**
 * Присвоение значения подполю.
 *
 * @param subfield Проинициализированное поле, значение которого необходимо задать.
 * @param value Значение подполя (может быть пустым).
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL subfield_assign
    (
        SubField *subfield,
        Span value
    )
{
    assert (subfield != NULL);

    if (!buffer_assign_span (&subfield->value, value)) {
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * Клонирование (глубокая копия) подполя.
 *
 * @param target Указатель на неинициализированное подполе,
 * в которое должен быть помещен результат.
 * @param source Подполе, подлежащее клонированию.
 * @return Указатель на подполе, в которое помещен клон.
 */
MAGNA_API am_bool MAGNA_CALL subfield_clone
    (
        SubField *target,
        const SubField *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    target->code = source->code;

    return buffer_clone (&target->value, &source->value);
}

/**
 * Декодирование подполя из текстового представления.
 *
 * @param subfield Указатель на инициализированную структуру.
 * @param text Текстовое представление подполя (без символа-разделителя).
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL subfield_decode
    (
        SubField *subfield,
        Span text
    )
{
    assert (subfield != NULL);

    if (!span_is_empty (text)) {
        subfield->code = subfield_normalize_code (text.start [0]);
        ++text.start;
        if (!span_is_empty (text)) {
            if (!buffer_assign_span (&subfield->value, text)) {
                return AM_FALSE;
            }
        }
    }

    return AM_TRUE;
}

/**
 * Проверка, не пустое ли подполе.
 *
 * @param subfield Подполе подлежащее проверке.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL subfield_is_empty
    (
        const SubField *subfield
    )
{
    assert (subfield != NULL);

    return subfield->code == 0
        || buffer_is_empty (&subfield->value);
}

/**
 * Инициализация подполя.
 *
 * @param subfield Указатель на неинициализированное подполе.
 * @param code Код подполя.
 * @param value Спан с данными (может быть пустым).
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL subfield_create
    (
        SubField *subfield,
        am_byte code,
        Span value
    )
{
    assert (subfield != NULL);

    subfield->code = code;
    if (!buffer_create (&subfield->value, value.start, span_length (value))) {
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * Инициализация подполя.
 *
 * @param subfield Указатель на неинициализированное подполе.
 * @param code Код подполя.
 * @param value Указатель на строку с данными.
 * @return Указатель на подполе.
 * @warning Инициализированное таким образом поле нельзя
 * освобождать при помощи `subfield_destroy`.
 */
MAGNA_API SubField* MAGNA_CALL subfield_init
    (
        SubField *subfield,
        am_byte code,
        Span value
    )
{
    assert (subfield != NULL);

    subfield->code = code;
    buffer_from_span (&subfield->value, value);

    return subfield;
}

/**
 * Освобождение ресурсов, занятых подполем.
 *
 * @param subfield Подполе, подлежащее освобождению.
 */
MAGNA_API void MAGNA_CALL subfield_destroy
    (
        SubField *subfield
    )
{
    assert (subfield != NULL);

    buffer_destroy (&subfield->value);
}

/**
 * Конвертация подполя в текстовое представление.
 *
 * @param subfield Подполе.
 * @param output Буфер для размещения результата.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL subfield_to_string
    (
        const SubField *subfield,
        Buffer *output
    )
{
    assert (subfield != NULL);
    assert (output != NULL);

    return buffer_putc (output, '^')
        && buffer_putc (output, subfield->code)
        && buffer_concat (output, &subfield->value);
}

/**
 * Проверка, является ли указанный код подполя валидным.
 *
 * @param code Проверяемый код подполя.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL subfield_code_is_valid
    (
        am_byte code
    )
{
    return (code >= '!') && (code <= '~');
}

/**
 * Нормализация кода подполя (приведение к нижнему регистру).
 *
 * @param code Код подполя, подлежащий нормализации.
 * @return Нормализованный код подполя.
 */
MAGNA_API am_byte MAGNA_CALL subfield_normalize_code
    (
        am_byte code
    )
{
    return (am_byte) (((unsigned) (tolower (code))) & 0xFFu);
}

/**
 * Верификация подполя.
 *
 * @param subfield Подполе, подлежащее верификации.
 * @return Результат верификации.
 * @remarks Валидным подполем считается:
 * 1) имеющее код из диапазона разрешенных,
 * 2) имеющее непустое значение.
 */
MAGNA_API am_bool MAGNA_CALL subfield_verify
    (
        const SubField *subfield
    )
{
    assert (subfield != NULL);

    return subfield_code_is_valid (subfield->code)
        && !subfield_is_empty (subfield);
}

MAGNA_API void MAGNA_CALL subfield_to_console
    (
        const SubField *subfield
    )
{
    assert (subfield != NULL);

    putc ('^', stdout);
    putc (subfield->code, stdout);
    buffer_to_console (&subfield->value);
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
