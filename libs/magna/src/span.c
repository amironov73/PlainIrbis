// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>
#include <ctype.h>

/*=========================================================*/

/**
 * @file span.c
 *
 * Невладеющий указатель на участок памяти.
 * Освобождать ресурсы, занимаемые спаном, не нужно.
 *
 * \struct Span
 *      \brief Невладеющий указатель на участок памяти.
 *
 * \var Span::start
 *      \brief Указатель на первый байт участка памяти.
 *
 * \var Span::end
 *      \brief Указатель за последним байтом участком памяти.
 *      \details Если `start == end`, значит, спан пустой.
 */

/*=========================================================*/

/**
 * Простая инициализация спана.
 * Не выделяет памяти в куче.
 *
 * @param ptr Указатель на начало фрагмента памяти.
 * @param length Длина фрагмента памяти.
 * @return Инициализированный спан.
 */
MAGNA_API Span MAGNA_CALL span_init
    (
        const am_byte *ptr,
        size_t length
    )
{
    Span result = { (am_byte*) ptr, (am_byte*) (ptr + length) };

    return result;
}

/**
 * Сверхпростая инициализация -- создает пустой спан.
 * Не выделяет памяти в куче.
 *
 * @return Пустой спан.
 */
MAGNA_API MAGNA_INLINE Span span_null (void)
{
    Span result = { NULL, NULL };

    return result;
}

/**
 * Инициализация строкой, завершающейся нулём.
 * Не выделяет памяти в куче.
 *
 * @param str Указатель на начало строки.
 * @return Инициализированный спан. Завершающий нуль не включается.
 */
MAGNA_API Span MAGNA_CALL span_from_text
    (
        const am_byte *str
    )
{
    Span result;

    assert (str != NULL);

    result.start = (am_byte*) str;
    result.end   = (am_byte*) (str + strlen (CCTEXT (str)));

    return result;
}

/**
 * Вычисление длины спана, т. е. количества байтов в нём.
 *
 * @param span Спан.
 * @return Результат вычисления (возможно, 0).
 */
MAGNA_API MAGNA_INLINE size_t MAGNA_CALL span_length
    (
        Span span
    )
{
    return (size_t) (span.end - span.start);
}

/**
 * Удаление пробельных символов из начала спана.
 * Не производит никаких изменений в данных,
 * на которые ссылается спан.
 *
 * @param span Спан.
 * @return Тот же спан, возможно, укороченный.
 */
MAGNA_API Span MAGNA_CALL span_trim_start
    (
        Span span
    )
{
    Span result = span;

    while (result.start != result.end) {
        if (!isspace (*result.start)) {
            break;
        }

        ++result.start;
    }

    return result;
}

/**
 * Удаление пробельных символов из конца спана.
 * Не производит никаких изменений в данных,
 * на которые ссылается спан.
 *
 * @param span Спан.
 * @return Тот же спан, возможно, укороченный.
 */
MAGNA_API Span MAGNA_CALL span_trim_end
    (
        Span span
    )
{
    Span result = span;

    while (result.start != result.end) {
        if (!isspace (result.end [-1])) {
            break;
        }

        --result.end;
    }

    return result;
}

/**
 * Удаление пробельных символов из начала и конца спана.
 * Не производит никаких изменений в данных,
 * на которые ссылается спан.
 *
 * @param span Спан.
 * @return Тот же спан, возможно, укороченный.
 */
MAGNA_API Span MAGNA_CALL span_trim
    (
        Span span
    )
{
    return span_trim_end
        (
            span_trim_start (span)
        );
}

/**
 * Проход по каждому символу в фрагменте в прямом направлении.
 *
 * @param span Спан.
 * @param routine Функция-обработчик. Возвращает
 * `AM_FALSE`, если выполнение должно быть прервано.
 * @param extraData Произвольные пользовательские данные (допускается `NULL`).
 * @return Символ, на котором выполнение было прервано, либо `NULL`.
 */
MAGNA_API am_byte* MAGNA_CALL span_for_each
    (
        Span span,
        SpanForEach routine,
        void *extraData
    )
{
    am_byte *ptr;

    assert (routine != NULL);

    for (ptr = span.start; ptr != span.end; ++ptr) {
        if (!routine (*ptr, extraData)) {
            return ptr;
        }
    }

    return NULL;
}

/**
 * Проход по каждому символу в фрагменте в прямом направлении.
 *
 * @param span Спан.
 * @param routine Функция-обработчик. Возвращает
 * `AM_FALSE`, если выполнение должно быть прервано.
 * @param extraData Произвольные пользовательские данные (допускается `NULL`).
 * @return Символ, на котором выполнение было прервано, либо `NULL`.
 */
MAGNA_API am_byte* MAGNA_CALL span_for_each_ptr
    (
        Span span,
        SpanForEachPtr routine,
        void *extraData
    )
{
    am_byte *ptr;

    assert (routine != NULL);

    for (ptr = span.start; ptr != span.end; ++ptr) {
        if (!routine (ptr, extraData)) {
            return ptr;
        }
    }

    return NULL;
}

/**
 * Проход по каждому символу в фрагменте в обратном направлении.
 *
 * @param span Спан.
 * @param routine Функция-обработчик. Возвращает
 * `AM_FALSE`, если выполнение должно быть прервано.
 * @param extraData Произвольные пользовательские данные (допускается `NULL`).
 * @return Символ, на котором выполнение было прервано, либо `NULL`.
 */
MAGNA_API am_byte* MAGNA_CALL span_for_each_reverse
    (
        Span span,
        SpanForEach routine,
        void *extraData
    )
{
    am_byte *ptr, *prev;

    assert (routine != NULL);

    for (ptr = span.end; ptr != span.start; ptr = prev) {
        prev = ptr - 1;
        if (!routine (*prev, extraData)) {
            return prev;
        }
    }

    return AM_FALSE;
}

/**
 * Проход по каждому символу в фрагменте в прямом направлении.
 *
 * @param span Спан.
 * @param routine Функция-обработчик. Возвращает
 * `AM_FALSE`, если выполнение должно быть прервано.
 * @param extraData Произвольные пользовательские данные (допускается `NULL`).
 * @return Символ, на котором выполнение было прервано, либо `NULL`.
 */
MAGNA_API am_byte* MAGNA_CALL span_for_each_ptr_reverse
    (
        Span span,
        SpanForEachPtr routine,
        void *extraData
    )
{
    am_byte *ptr, *prev;

    assert (routine != NULL);

    for (ptr = span.end; ptr != span.start; ptr = prev) {
        prev = ptr - 1;
        if (!routine (prev, extraData)) {
            return prev;
        }
    }

    return NULL;
}

/**
 * Преобразование спана в целое число со знаком.
 *
 * @param span Спан.
 * @return Результат разбора. Мусор на входе -- мусор на выходе!
 */
MAGNA_API am_int32 MAGNA_CALL span_to_int32
    (
        Span span
    )
{
    am_int32 result = 0;
    am_byte *ptr = span.start;
    am_bool sign = AM_FALSE;

    while (ptr != span.end) {
        if (*ptr == '+') {
            ++ptr;
            continue;
        }

        if (*ptr != '-') {
            break;
        }

        sign = !sign;
        ++ptr;
    }

    while (ptr != span.end) {
        result = result * 10 + (*ptr++ - '0');
    }

    return sign ? -result : result;
}

/**
 * Преобразование спана в целое число со знаком.
 *
 * @param span Спан.
 * @return Результат разбора. Мусор на входе -- мусор на выходе!
 */
MAGNA_API am_int64 MAGNA_CALL span_to_int64
    (
        Span span
    )
{
    am_int64 result = 0;
    am_byte *ptr = span.start;
    am_bool sign = AM_FALSE;

    while (ptr != span.end) {
        if (*ptr == '+') {
            ++ptr;
            continue;
        }

        if (*ptr != '-') {
            break;
        }

        sign = !sign;
        ++ptr;
    }

    while (ptr != span.end) {
        result = result * 10 + (*ptr++ - '0');
    }

    return sign ? -result : result;
}

/**
 * Преобразование спана в целое число без знака.
 *
 * @param span Спан.
 * @return Результат разбора. Мусор на входе -- мусор на выходе!
 */
MAGNA_API am_uint32 MAGNA_CALL span_to_uint32
    (
        Span span
    )
{
    am_uint32 result = 0;
    am_byte *ptr = span.start;

    while (ptr != span.end) {
        result = result * 10 + (*ptr++ - '0');
    }

    return result;
}

/**
 * Преобразование спана в целое число без знака.
 *
 * @param span Спан.
 * @return Результат разбора. Мусор на входе -- мусор на выходе!
 */
MAGNA_API am_uint64 MAGNA_CALL span_to_uint64
    (
        Span span
    )
{
    am_uint64 result = 0;
    am_byte *ptr = span.start;

    while (ptr != span.end) {
        result = result * 10 + (*ptr++ - '0');
    }

    return result;
}

/**
 * Перевод спана в верхний регистр (в кодировке ASCII).
 *
 * @param span Спан.
 * @return Тот же спан.
 */
MAGNA_API Span MAGNA_CALL span_toupper
    (
        Span span
    )
{
    am_byte *ptr = span.start;

    while (ptr != span.end) {
        *ptr = (am_byte) toupper (*ptr);
        ++ptr;
    }

    return span;
}

/**
 * Перевод спана в нижний регистр (в кодировке ASCII).
 *
 * @param span Спан.
 * @return Тот же спан.
 */
MAGNA_API Span MAGNA_CALL span_tolower
    (
        Span span
    )
{
    am_byte *ptr = span.start;

    while (ptr != span.end) {
        *ptr = (am_byte) tolower (*ptr);
        ++ptr;
    }

    return span;
}

/**
 * Проверка: спан пустой?
 *
 * @param span Проверяемый спан.
 * @return Результат проверки.
 */
MAGNA_API MAGNA_INLINE am_bool MAGNA_CALL span_is_empty
    (
        Span span
    )
{
    return (span.start == span.end) || (span.start == NULL);
}

/**
 * Содержит ли спан указанный элемент?
 *
 * @param span Спан.
 * @param value Искомый элемент.
 * @return Указатель на найденный байт либо `NULL`.
 */
MAGNA_API am_byte* MAGNA_CALL span_find_byte
    (
        Span span,
        am_byte value
    )
{
    am_byte *ptr = span.start;

    while (ptr != span.end) {
        if (*ptr == value) {
            return ptr;
        }

        ++ptr;
    }

    return NULL;
}

/**
 * Заполнение спана указанным значением.
 *
 * @param span Спан.
 * @param value Значение-заполнитель.
 * @return Тот же спан.
 */
MAGNA_API Span MAGNA_CALL span_fill
    (
        Span span,
        am_byte value
    )
{
    am_byte *ptr = span.start;

    while (ptr != span.end) {
        *ptr++ = value;
    }

    return span;
}

/**
 * Поиск указанного значения в спане с начала.
 *
 * @param span Спан.
 * @param value Искомое значение.
 * @return Найденный индекс либо -1.
 */
MAGNA_API ssize_t MAGNA_CALL span_index_of
    (
        Span span,
        am_byte value
    )
{
    ssize_t index = 0;
    am_byte *ptr = span.start;

    while (ptr != span.end) {
        if (*ptr == value) {
            return index;
        }

        ++index;
        ++ptr;
    }

    return (ssize_t) -1;
}

/**
 * Поиск указанного значения в спане с конца.
 *
 * @param span Спан.
 * @param value Искомое значение.
 * @return Найденный индекс либо -1.
 */
MAGNA_API ssize_t MAGNA_CALL span_last_index_of
    (
        Span span,
        am_byte value
    )
{
    am_byte *ptr = span.end;
    ssize_t index = span.end - span.start;

    while (ptr != span.start) {
        --index;
        --ptr;
        if (*ptr == value) {
            return index;
        }
    }

    return (ssize_t) -1;
}

/**
 * Срез спана. Данные, на которые указывает спан, никак не меняются.
 *
 * @param span Спан.
 * @param start Смещение от начала.
 * @param length Длина среза.
 * @return Обрезанный спан (возможно, пустой).
 */
MAGNA_API Span MAGNA_CALL span_slice
    (
        Span span,
        ssize_t start,
        ssize_t length
    )
{
    Span result = span;
    ssize_t previousLength = (ssize_t) (span.end - span.start);

    if (length == -1) {
        length = previousLength - start;
    }

    if (length + start > previousLength) {
        length = previousLength - start;
    }

    if (length < 0) {
        length = 0;
    }

    result.end = (result.start += start) + length;

    return result;
}

/**
 * Преобразование спана в строку.
 * Выделяет память под строку в куче.
 *
 * @param span Спан для преобразования.
 * @return Размещенная в куче копия строки (с завершающим нулем).
 */
MAGNA_API am_byte* MAGNA_CALL span_to_string
    (
        Span span
    )
{
    size_t length = span_length (span);
    am_byte *result = (am_byte*) mem_alloc (length + 1);

    if (result == NULL) {
        return result;
    }

    if (length != 0) {
        mem_copy (result, span.start, length);
    }

    result [length] = 0;

    return result;
}

/**
 * Преобразование спана в вектор байтов.
 * Выделяет память под вектор в куче.
 *
 * @param span Спан для преобразования.
 * @return Размещенный в куче вектор байтов (копия данных).
 */
MAGNA_API am_byte* MAGNA_CALL span_to_vector
    (
        Span span
    )
{
    size_t length = span_length (span);
    am_byte *result = mem_alloc (length);

    if (length != 0) {
        mem_copy (result, span.start, length);
    }

    return result;
}

/**
 * Совпадает ли начало спана с другим спаном?
 *
 * @param span Проверяемый спан.
 * @param prefix Предполагаемый префикс.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL span_starts_with
    (
        Span span,
        Span prefix
    )
{
    size_t spanLength = span_length (span);
    size_t prefixLength = span_length (prefix);
    am_bool result = prefixLength <= spanLength;
    size_t index;

    if (result) {
        for (index = 0; index < prefixLength; ++index) {
            if (span.start [index] != prefix.start [index]) {
                result = AM_FALSE;
                break;
            }
        }
    }

    return result;
}

/**
 * Совпадает ли конец спана с другим спаном?
 *
 * @param span Проверяемый спан.
 * @param suffix Предполагаемый суффикс.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL span_ends_with
    (
        Span span,
        Span suffix
    )
{
    size_t spanLength = span_length (span);
    size_t suffixLength = span_length (suffix);
    am_bool result = suffixLength <= spanLength;
    size_t index;

    if (result) {
        for (index = 0; index < suffixLength; ++index) {
            if (span.start [spanLength - index - 1]
                    != suffix.start [suffixLength - index - 1]) {
                result = AM_FALSE;
                break;
            }
        }
    }

    return result;
}

/**
 * Побайтовое сравнение двух спанов.
 *
 * @param first Первый спан.
 * @param second Второй спан.
 * @return Результат сравнения: &lt; 0, если первый спан меньше,
 * &gt; 0, если первый спан больше и = 0, если содержимое спанов совпадает.
 */
MAGNA_API int MAGNA_CALL span_compare
    (
        Span first,
        Span second
    )
{
    size_t firstLength = span_length (first);
    size_t secondLength = span_length (second);
    int result;
    size_t index;

    for (index = 0; ; ++index) {
        if (index == firstLength) {
            if (index == secondLength) {
                return 0;
            }

            return -1;
        }

        if (index == secondLength) {
            return 1;
        }

        result = first.start [index] - second.start [index];
        if (result) {
            return result;
        }
    }
}

/**
 * Побайтовое сравнение двух спанов без учета регистра символов.
 *
 * @param first Первый спан.
 * @param second Второй спан.
 * @return Результат сравнения: &lt; 0, если первый спан меньше,
 * &gt; 0, если первый спан больше и = 0, если содержимое спанов совпадает.
 */
MAGNA_API int MAGNA_CALL span_compare_ignore_case
    (
        Span first,
        Span second
    )
{
    size_t firstLength = span_length (first);
    size_t secondLength = span_length (second);
    int result;
    size_t index;

    for (index = 0; ; ++index) {
        if (index == firstLength) {
            if (index == secondLength) {
                return 0;
            }

            return -1;
        }

        if (index == secondLength) {
            return 1;
        }

        result = toupper (first.start [index]) - toupper (second.start [index]);
        if (result) {
            return result;
        }
    }
}

/**
 * Спан содержит указанное значение?
 *
 * @param span Спан.
 * @param value Искомое значение.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL span_contains
    (
        Span span,
        am_byte value
    )
{
    const am_byte *ptr;

    for (ptr = span.start; ptr != span.end; ++ptr) {
        if (*ptr == value) {
            return AM_TRUE;
        }
    }

    return AM_FALSE;
}

/**
 * Подсчитывает количество вхождений указанного символа.
 *
 * @param span Спан.
 * @param value Искуомое значение.
 * @return Количество вхождений.
 */
MAGNA_API size_t MAGNA_CALL span_count
    (
        Span span,
        am_byte value
    )
{
    size_t result = 0;
    const am_byte *ptr;

    for (ptr = span.start; ptr != span.end; ++ptr) {
        if (*ptr == value) {
            ++result;
        }
    }

    return result;
}

/**
 * Разбиение по указанному символу.
 *
 * @param span Спан для разбиения.
 * @param array Инициализированный динамический массив для помещения результата.
 * @param value Символ-разделитель.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL span_split_by_char
    (
        Span span,
        SpanArray *array,
        am_byte value
    )
{
    am_byte *left, *right;
    am_bool found;
    Span item;

    assert (array != NULL);

    left = span.start;
    while (left < span.end) {
        found = AM_FALSE;
        for (right = left; right != span.end; ++right) {
            if (*right == value) {
                if (left != right) {
                    item.start = left;
                    item.end = right;
                    if (!span_array_push_back (array, item)) {
                        return AM_FALSE;
                    }
                }

                found = AM_TRUE;
                break;
            }
        }

        if (!found) {
            item.start = left;
            item.end = span.end;
            if (!span_array_push_back (array, item)) {
                return AM_FALSE;
            }

            break;
        }

        left = right + 1;
    }

    return AM_TRUE;
}

/**
 * Разбиение по указанным символам.
 *
 * @param span Спан для разбиения.
 * @param array Инициализированный динамический массив для помещения результата.
 * @param values Указатель на символы-разделители.
 * @param valueCount Количество символов-разделителей.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL span_split_by_chars
    (
        Span span,
        SpanArray *array,
        const am_byte *values,
        size_t valueCount
    )
{
    am_byte *left, *right;
    size_t index;
    Span item;
    am_byte c;
    am_bool found = AM_FALSE;

    assert (array != NULL);
    assert (values != NULL);

    left = span.start;
    while (left < span.end) {
        for (right = left; right != span.end; ++right) {
            c = *right;
            found = AM_FALSE;
            for (index = 0; index < valueCount; ++index) {
                if (c == values [index]) {
                    found = AM_TRUE;
                    break;
                }
            }

            if (found) {
                if (left != right) {
                    item.start = left;
                    item.end = right;
                    if (!span_array_push_back (array, item)) {
                        return AM_FALSE;
                    }
                }
                break;
            }
        }

        if (!found) {
            item.start = left;
            item.end = span.end;
            if (!span_array_push_back (array, item)) {
                return AM_FALSE;
            }

            break;
        }

        left = right + 1;
    }

    return AM_TRUE;
}

static am_byte* find_one
    (
        am_byte *from,
        const am_byte *to,
        am_byte value
    )
{
    am_byte *ptr;

    for (ptr = from; ptr < to; ++ptr) {
        if (*ptr == value) {
            return ptr;
        }
    }

    return NULL;
}

static am_byte* find_many
    (
        am_byte *from,
        const am_byte *to,
        am_byte *values,
        size_t valueCount
    )
{
    am_byte *ptr1, *ptr2 = values + valueCount, *ptr3;

    for (ptr1 = from; ptr1 < to; ++ptr1) {
        ptr3 = find_one (values, ptr2, *ptr1);
        if (ptr3) {
            return ptr1;
        }
    }

    return NULL;
}

/**
 * Разбиение по указанному символу.
 *
 * @param span Спан для разбиения.
 * @param array Статический массив для размещения результатов.
 * @param arraySize Размер статического массива в элементах.
 * @param value Символ-разделитель.
 * @return Количество получившихся фрагметов.
 */
MAGNA_API size_t MAGNA_CALL span_split_n_by_char
    (
        Span span,
        Span *array,
        size_t arraySize,
        am_byte value
    )
{
    size_t count;
    am_byte *ptr = span.start, *found;
    Span *item;

    assert (array != NULL);

    for (count = 0; count < arraySize - 1; ) {
        found = find_one (ptr, span.end, value);
        if (!found) {
            break;
        }

        if (found == ptr) {
            ++ptr;
            continue;
        }

        item = array + count;
        item->start = ptr;
        item->end = found;
        ptr = found + 1;
        ++count;
    }

    if (ptr != span.end) {
        item = array + count;
        item->start = ptr;
        item->end = span.end;
        ++count;
    }

    return count;
}

/**
 * Разбиение по указанным символам.
 *
 * @param span Спан для разбиения.
 * @param array Статический массив для размещения результата.
 * @param arraySize Размер статического массива в элементах.
 * @param values Символы-разделители.
 * @param valueCount Количество символов-разделителей.
 * @return Количество получившихся фрагментов.
 */
MAGNA_API size_t MAGNA_CALL span_split_n_by_chars
    (
        Span span,
        Span *array,
        size_t arraySize,
        const am_byte *values,
        size_t valueCount
    )
{
    size_t count;
    am_byte *ptr = span.start, *found;
    Span *item;

    assert (array != NULL);

    for (count = 0; count < arraySize - 1; ) {
        found = find_many (ptr, span.end, (am_byte*) values, valueCount);
        if (!found) {
            break;
        }

        if (found == ptr) {
            ++ptr;
            continue;
        }

        item = array + count;
        item->start = ptr;
        item->end = found;
        ptr = found + 1;
        ++count;
    }

    if (ptr != span.end) {
        item = array + count;
        item->start = ptr;
        item->end = span.end;
        ++count;
    }

    return count;
}

/**
 * Разбор целого числа, записанного в шестнадцатиричной форме.
 *
 * @param span Спан с числом.
 * @return Результат разбора.
 * @warning Мусор на входе - мусор на выходе!
 */
MAGNA_API am_uint64 MAGNA_CALL span_hex_to_uint64
    (
        Span span
    )
{
    am_uint64 result = 0;
    const am_byte *ptr;
    am_byte c;

    for (ptr = span.start; ptr != span.end; ++ptr) {
        c = (am_byte) toupper (*ptr);
        if (c <= '9') {
            result = result * 16 + c - '0';
        }
        else {
            result = result * 16 + c + 10 - 'A';
        }
    }

    return result;
}

/**
 * Проверка: валидный ли спан.
 *
 * @param span Спан для проверки.
 * @return Результат проверки.
 */
MAGNA_API MAGNA_INLINE am_bool MAGNA_CALL span_verify
    (
        Span span
    )
{
    return span.start <= span.end;
}

/**
 * Проверка, валидный ли спан, с помощью `assert`.
 *
 * @param span Спан для проверки.
 */
MAGNA_API MAGNA_INLINE void MAGNA_CALL span_assert
    (
        Span span
    )
{
    assert (span.start <= span.end);
}

/**
 * Удаляет префикс, если тот присутствует.
 *
 * @param span Спан.
 * @param prefix Префикс, подлежащий удалению.
 * @return Спан без префикса (возможно, пустой).
 */
MAGNA_API Span MAGNA_CALL span_remove_prefix
    (
        Span span,
        Span prefix
    )
{
    Span result;

    if (span_starts_with (span, prefix)) {
        result.start = span.start + span_length (prefix);
        result.end = span.end;

        return result;
    }

    return span;
}

/**
 * Удаляет суффикс, если тот присутствует.
 *
 * @param span Спан.
 * @param suffix Суффикс, подлежащий удалению.
 * @return Спан без суффикса (возможно, пустой).
 */
MAGNA_API Span MAGNA_CALL span_remove_suffix
    (
        Span span,
        Span suffix
    )
{
    Span result;

    if (span_ends_with (span, suffix)) {
        result.start = span.start;
        result.end = span.end - span_length (suffix);

        return result;
    }

    return span;
}

/**
 * Удаляет префикс и суффикс, если те одновременно присутствуют.
 *
 * @param span Спан.
 * @param prefix Префикс.
 * @param suffix Суффикс.
 * @return Спан без префикса и суффикса (возможно, пустой).
 */
MAGNA_API Span MAGNA_CALL span_remove_prefix_and_suffix
    (
        Span span,
        Span prefix,
        Span suffix
    )
{
    Span result;

    if (span_length (span) >= (span_length (prefix) + span_length (suffix))
        && span_starts_with (span, prefix)
        && span_ends_with (span, suffix)) {
        result.start = span.start + span_length (prefix);
        result.end = span.end - span_length (suffix);

        return result;
    }

    return span;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
