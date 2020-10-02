/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

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

 */

#include <assert.h>

/*=========================================================*/

/**
 * Клонирование (глубокая копия) подполя.
 *
 * @param target Указатель на неинициализированное подполе, в которое должен быть помещен результат.
 * @param source Подполе, подлежащее клонированию.
 * @return Указатель на подполе, в которое помещен клон.
 */
MAGNA_API SubField* MAGNA_CALL subfield_clone
    (
        SubField *target,
        const SubField *source
    )
{
    assert (target != NULL);
    assert (source != NULL);

    target->code = source->code;
    buffer_clone (&target->value, &source->value);

    return target;
}

/**
 * Декодирование подполя из текстового представления.
 *
 * @param subfield
 * @param text Текстовое представление подполя (без символа-разделителя).
 * @return
 */
MAGNA_API SubField* MAGNA_CALL subfield_decode
    (
        SubField *subfield,
        Span text
    )
{
    assert (subfield != NULL);

    if (text.len != 0) {
        subfield->code = text.ptr[0];
        if (--text.len != 0) {
            ++text.ptr;
            buffer_from_span(&subfield->value, text);
        }
    }

    return subfield;
}

/**
 * Проверка, не пустое ли подполе.
 *
 * @param subfield Подполе подлежащее проверке.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL subfield_empty
    (
        const SubField *subfield
    )
{
    assert (subfield != NULL);

    return subfield->code == 0
        || subfield->value.position == 0;
}

/**
 * Инициализация подполя.
 *
 * @param subfield Указатель на неинициализированное подполе.
 * @param code Код подполя.
 * @param value Указатель на строку с данными.
 * @return Указатель на подполе.
 */
MAGNA_API SubField* MAGNA_CALL subfield_create
    (
        SubField *subfield,
        char code,
        Span value
    )
{
    assert (subfield != NULL);

    subfield->code = code;
    buffer_create (&subfield->value, value.ptr, value.len);
    subfield->value.position = value.len;

    return subfield;
}

/**
 * Инициализация подполя.
 *
 * @param subfield Указатель на неинициализированное подполе.
 * @param code Код подполя.
 * @param value Указатель на строку с данными.
 * @return Указатель на подполе.
 */
MAGNA_API SubField* MAGNA_CALL subfield_init
    (
        SubField *subfield,
        char code,
        Span value
    )
{
    assert (subfield != NULL);

    subfield->code = code;
    buffer_from_span (&subfield->value, value);
    subfield->value.position = value.len;

    return subfield;
}

/**
 * Освобождение ресурсов, занятых подполем.
 *
 * @param subfield Подполе, подлежащее освобождению.
 */
MAGNA_API void MAGNA_CALL subfield_free
    (
        SubField *subfield
    )
{
    assert (subfield != NULL);

    buffer_destroy(&subfield->value);
}

/**
 * Конвертация подполя в текстовое представление.
 *
 * @param subfield
 * @param buffer
 * @return
 */
MAGNA_API Buffer* MAGNA_CALL subfield_to_string
    (
        const SubField *subfield,
        Buffer *buffer
    )
{
    assert (subfield != NULL);
    assert (buffer != NULL);

    return buffer_putc (buffer, '^')
        && buffer_putc (buffer, subfield->code)
        && buffer_concat (buffer, &subfield->value)
        ? buffer
        : NULL;
}

/**
 * Верификация подполя.
 *
 * @param subfield
 * @return
 */
MAGNA_API am_bool MAGNA_CALL subfield_verify
    (
        const SubField *subfield
    )
{
    assert (subfield != NULL);

    return subfield->code != 0
        && subfield->value.position != 0;
}
