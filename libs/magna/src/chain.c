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
 * \file chain.c
 *
 * Невладеющая цепочка фрагментов памяти.
 *
 * Владеет только собственной памятью,
 * но не фрагментом, на который указаывает.
 * В общем случае для освобождения ресурсов
 * требуется вызывать `chain_destroy`.
 *
 * \struct ChainSpan
 *      \brief Невладеющая цепочка фрагментов памяти.
 *      \details Владеет только собственной памятью,
 *      но не фрагментом, на который указывает.
 *      В общем случае для освобождения ресурсов
 *      требуется вызывать `chain_destroy`.
 *      \paragraph Первый фрагмент цепочки может располагаться
 *      на стеке или в сегменте данных, последующие фрагменты
 *      всегда располагаются в куче.
 *
 * \var ChainSpan::start
 *      \brief Указатель на первый байт фрагмента памяти.
 *
 * \var ChainSpan::end
 *      \brief Указатель сразу за последним байтом фрагмента памяти.
 *      Если `start == end`, фрагмент считается пустым.
 *
 * \var ChainSpan::next
 *      \brief Указатель на следующий фрагмент в цепочке (если есть).
 *
 * \struct ChainIterator
 *      \brief Итератор по цепочке фрагментов памяти.
 *
 * \var ChainIterator::chain
 *      \brief Указатель на начало цепочки.
 *
 * \var ChainIterator::current
 *      \brief Указатель на текущий фрагмент.
 *
 * \var ChainIterator::ptr
 *      \brief Указатель на текущий символ в фрагменте.
 */

/*=========================================================*/

/**
 * Простая инициализация цепочки.
 * Не выделяет памяти в куче.
 *
 * @param chain Указатель на цепочку, подлежащую инициализации.
 * @param span Первый фрагмент цепочки.
 */
MAGNA_API void MAGNA_CALL chain_init
    (
        ChainSpan *chain,
        Span span
    )
{
    assert (chain != NULL);

    chain->start = span.start;
    chain->end   = span.end;
    chain->next  = NULL;
}

/**
 * Сверхпростая инициализация -- создает пустую цепочку.
 * Не выделяет памяти в куче.
 *
 * @return Пустая цепочка.
 */
MAGNA_API MAGNA_INLINE ChainSpan chain_null (void)
{
    ChainSpan result = CHAIN_INIT;

    return result;
}

/**
 * Освобождение ресурсов, занятых цепочкой.
 *
 * @param chain Начало цепочки, подлежащей освобождению.
 */
MAGNA_API void MAGNA_CALL chain_destroy
    (
        ChainSpan *chain
    )
{
    ChainSpan *span, *next;

    assert (chain != NULL);

    for (span = chain->next; span != NULL; span = next) {
        next = span->next;
        mem_free (span);
    }

    chain->start = NULL;
    chain->end   = NULL;
    chain->next  = NULL;
}

/**
 * Клонирование цепочки фрагментов.
 *
 * @param target Указатель на неинициализированную структуру, в которую будет помещен клон.
 * @param source Начало клонируемой цепочки.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_byte MAGNA_CALL chain_clone
    (
        ChainSpan *target,
        const ChainSpan *source
    )
{
    ChainSpan *current, *last, *next;

    assert (target != NULL);
    assert (source != NULL);

    target->start = source->start;
    target->end   = source->end;
    target->next  = NULL;

    last = target;
    for (current = source->next; current != NULL; current = current->next) {
        next = mem_alloc (sizeof (*next));
        if (next == NULL) {
            chain_destroy (target);

            return AM_FALSE;
        }

        last->next  = next;
        next->start = current->start;
        next->end   = current->end;
        next->next  = NULL;
        last        = next;
    }

    return AM_TRUE;
}

/**
 * Получение указателя на последний фрагмент цепочки.
 *
 * @param chain Начало цепочки.
 * @return Последний фрагмент цепочки (не может быть `NULL`).
 */
MAGNA_API ChainSpan* MAGNA_CALL chain_last
    (
        const ChainSpan *chain
    )
{
    ChainSpan *result;

    assert (chain != NULL);

    for (result = (ChainSpan*) chain; result->next != NULL; ) {
        result = result->next;
    }

    return result;
}

/**
 * Получение элемента цепочки, предшествующего указанному.
 *
 * @param chain Начало цепочки.
 * @param item Исследуемый элемент цепочки.
 * @return Указатель на предыдущий элемент либо `NULL`.
 */
MAGNA_API ChainSpan* MAGNA_CALL chain_before
    (
        const ChainSpan *chain,
        const ChainSpan *item
    )
{
    ChainSpan *result;

    assert (chain != NULL);
    assert (item != NULL);

    if (item == chain) {
        return NULL;
    }

    for (result = (ChainSpan*) chain; result->next != NULL; result = result->next) {
        if (result->next == item) {
            return result;
        }
    }

    return NULL;
}

/**
 * Добавление фрагмента в конец цепочки.
 * Выделяет память в куче для добавляемого фрагмента.
 *
 * @param chain Начало цепочки.
 * @param span Добавляемый фрагмент.
 * @return Признак успеха завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL chain_append
    (
        ChainSpan *chain,
        Span span
    )
{
    ChainSpan *last, *next;

    assert (chain != NULL);

    next = (ChainSpan*) mem_alloc (sizeof (*next));
    if (next == NULL) {
        return AM_FALSE;
    }

    chain_init (next, span);
    last = chain_last (chain);
    assert (last != NULL);
    last->next = next;

    return AM_TRUE;
}

/**
 * Вычисление длины звена цепочки.
 *
 * @param link Звено цепочки (не обязательно начальное).
 * @return Длина звена в байтах.
 */
MAGNA_API MAGNA_INLINE size_t MAGNA_CALL chain_length
    (
        ChainSpan *link
    )
{
    assert (link != NULL);

    return (size_t) (link->end - link->start);
}

/**
 * Вычисление общей длины цепочки фрагментов.
 *
 * @param chain Начало цепочки.
 * @return Общая длина цепочки в байтах.
 */
MAGNA_API size_t MAGNA_CALL chain_total_length
    (
        const ChainSpan *chain
    )
{
    size_t result = 0; /* Результат */
    ChainSpan *link; /* Переменная цикла */

    assert (chain != NULL);
    for (link = (ChainSpan*) chain; link != NULL; link = link->next) {
        result += chain_length (link);
    }

    return result;
}

/**
 * Проверка, не пустая ли цепочка.
 * Цепочка считается пустой, если все ее элементы пусты.
 *
 * @param chain Начало цепочки.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL chain_is_empty
    (
        const ChainSpan *chain
    )
{
    const ChainSpan *span;

    assert (chain != NULL);

    for (span = chain; span != NULL; span = span->next) {
        if (span->start != span->end) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/**
 * Инициализация строкой, завершающейся нулём.
 * Не выделяет памяти в куче.
 *
 * @param str Указатель на начало строки.
 * @return Инициализированная цепочка. Завершающий нуль не включается.
 */
MAGNA_API ChainSpan MAGNA_CALL chain_from_text
    (
        const am_byte *text
    )
{
    ChainSpan result;

    assert (text != NULL);

    result.start = (am_byte*) text;
    result.end   = (am_byte*) (text + strlen (CCTEXT (text)));
    result.next  = NULL;

    return result;
}

/**
 * Удаление пробельных символов из начала цепочки.
 * Не производит никаких изменений в данных,
 * на которые ссылается цепочка.
 *
 * @param chain Начало цепочки.
 */
MAGNA_API void MAGNA_CALL chain_trim_start
    (
        ChainSpan *chain
    )
{
    ChainSpan *current;

    assert (chain != NULL);

    for (current = chain; current != NULL; current = current->next) {
        while (current->start != current->end) {
            if (!isspace (*current->start)) {
                return;
            }

            ++current->start;
        }
    }

    /* Возможно, потребуется оптимизация цепочки. */
    chain_optimize (chain);
}

/**
 * Удаление пробельных символов из конца цепочки.
 * Не производит никаких изменений в данных,
 * на которые ссылается цепочка.
 *
 * @param chain Начало цепочки.
 */
MAGNA_API void MAGNA_CALL chain_trim_end
    (
        ChainSpan *chain
    )
{
    ChainSpan *current;

    assert (chain != NULL);

    for (current = chain_last (chain); current != NULL;
        current = chain_before (chain, current)) {
            while (current->start != current->end) {
                if (!isspace(current->end[-1])) {
                    return;
                }

                --current->end;
            }

            current = chain_before(chain, current);
    }

    /* Возможно, потребуется оптимизация цепочки. */
    chain_optimize (chain);
}

/**
 * Удаление пробельных символов из начала и конца цепочки.
 * Не производит никаких изменений в данных,
 * на которые ссылается цепочка.
 *
 * @param chain Цепочка.
 */
MAGNA_API void MAGNA_CALL chain_trim
    (
        ChainSpan *chain
    )
{
    assert (chain != NULL);

    chain_trim_start (chain);
    chain_trim_end (chain);
}

/**
 * Проход по каждому символу в цепочке в прямом направлении.
 *
 * @param chain Начало цепочки.
 * @param routine Функция-обработчик. Возвращает
 * `AM_FALSE`, если выполнение должно быть прервано.
 * @param extraData Произвольные пользовательские данные (допускается `NULL`).
 * @return Символ, на котором выполнение было прервано, либо `NULL`.
 */
MAGNA_API am_byte* MAGNA_CALL chain_for_each
    (
        ChainSpan *chain,
        SpanForEach routine,
        void *extraData
    )
{
    ChainSpan *current;
    am_byte *ptr;

    assert (chain != NULL);
    assert (routine != NULL);

    for (current = chain; current != NULL; current = current->next) {
        for (ptr = current->start; ptr != current->end; ++ptr) {
            if (!routine (*ptr, extraData)) {
                return ptr;
            }
        }
    }

    return NULL;
}

/**
 * Проход по каждому символу в цепочке в прямом направлении.
 *
 * @param chain Начало цепочки.
 * @param routine Функция-обработчик. Возвращает
 * `AM_FALSE`, если выполнение должно быть прервано.
 * @param extraData Произвольные пользовательские данные (допускается `NULL`).
 * @return Символ, на котором выполнение было прервано, либо `NULL`.
 */
MAGNA_API am_byte* MAGNA_CALL chain_for_each_ptr
    (
        ChainSpan *chain,
        SpanForEachPtr routine,
        void *extraData
    )
{
    ChainSpan *current;
    am_byte *ptr;

    assert (chain != NULL);
    assert (routine != NULL);

    for (current = chain; current != NULL; current = current->next) {
        for (ptr = current->start; ptr != current->end; ++ptr) {
            if (!routine (ptr, extraData)) {
                return ptr;
            }
        }
    }

    return NULL;
}

/**
 * Проход по каждому символу в цепочке в обратном направлении.
 *
 * @param chain Начало цепочки.
 * @param routine Функция-обработчик. Возвращает
 * `AM_FALSE`, если выполнение должно быть прервано.
 * @param extraData Произвольные пользовательские данные (допускается `NULL`).
 * @return Символ, на котором выполнение было прервано, либо `NULL`.
 */
MAGNA_API am_byte* MAGNA_CALL chain_for_each_reverse
    (
        ChainSpan *chain,
        SpanForEach routine,
        void *extraData
    )
{
    ChainSpan *current;
    am_byte *ptr;

    assert (chain != NULL);
    assert (routine != NULL);

    for (current = chain_last (chain); current != NULL;
        current = chain_before (chain, current)) {
        for (ptr = current->end; ptr != current->start; --ptr) {
            if (!routine (ptr [-1], extraData)) {
                return ptr;
            }
        }
    }

    return NULL;
}

/**
 * Проход по каждому символу в цепочке в обратном направлении.
 *
 * @param chain Начало цепочки.
 * @param routine Функция-обработчик. Возвращает
 * `AM_FALSE`, если выполнение должно быть прервано.
 * @param extraData Произвольные пользовательские данные (допускается `NULL`).
 * @return Символ, на котором выполнение было прервано, либо `NULL`.
 */
MAGNA_API am_byte* MAGNA_CALL chain_for_each_ptr_reverse
    (
        ChainSpan *chain,
        SpanForEachPtr routine,
        void *extraData
    )
{
    ChainSpan *current;
    am_byte *ptr, *prev;

    assert (chain != NULL);
    assert (routine != NULL);

    for (current = chain; current != NULL; current = current->next) {
        for (ptr = current->end; ptr != current->start; ptr = prev) {
            prev = ptr - 1;
            if (!routine (prev, extraData)) {
                return prev;
            }
        }
    }

    return NULL;
}

/**
 * Преобразование спана в целое число со знаком.
 *
 * @param chain Начало цепочки.
 * @return Результат разбора. Мусор на входе -- мусор на выходе!
 */
MAGNA_API am_int32 MAGNA_CALL chain_to_int32
    (
        const ChainSpan *chain
    )
{
    am_int32 result = 0;
    /* am_bool sign = AM_FALSE; */

    assert (chain != NULL);

    /* TODO: implement */

    return result;
}

/**
 * Преобразование спана в целое число со знаком.
 *
 * @param chain Начало цепочки.
 * @return Результат разбора. Мусор на входе -- мусор на выходе!
 */
MAGNA_API am_int64 MAGNA_CALL chain_to_int64
    (
        const ChainSpan *chain
    )
{
    am_int64 result = 0;
    /* am_bool sign = AM_FALSE; */

    assert (chain != NULL);

    /* TODO: implement */

    return result;
}

/**
 * Преобразование спана в целое число без знака.
 *
 * @param chain Начало цепочки.
 * @return Результат разбора. Мусор на входе -- мусор на выходе!
 */
MAGNA_API am_uint32 MAGNA_CALL chain_to_uint32
    (
        const ChainSpan *chain
    )
{
    am_uint32 result = 0;
    ChainSpan *current;
    am_byte *ptr;

    assert (chain != NULL);

    for (current = (ChainSpan*) chain; current != NULL; current = current->next) {
        for (ptr = current->start; ptr != current->end; ++ptr) {
            result = result * 10 + *ptr - '0';
        }
    }

    return result;
}

/**
 * Преобразование спана в целое число без знака.
 *
 * @param chain Начало цепочки.
 * @return Результат разбора. Мусор на входе -- мусор на выходе!
 */
MAGNA_API am_uint64 MAGNA_CALL chain_to_uint64
    (
        const ChainSpan *chain
    )
{
    am_uint64 result = 0;
    ChainSpan *current;
    am_byte *ptr;

    assert (chain != NULL);

    for (current = (ChainSpan*) chain; current != NULL; current = current->next) {
        for (ptr = current->start; ptr != current->end; ++ptr) {
            result = result * 10 + *ptr - '0';
        }
    }

    return result;
}

/**
 * Перевод цепочки фрагментов в верхний регистр (в кодировке ASCII).
 *
 * @param chain Начало цепочки.
 */
MAGNA_API void MAGNA_CALL chain_toupper
    (
        ChainSpan *chain
    )
{
    ChainSpan *current;
    am_byte *ptr;

    assert (chain != NULL);

    for (current = chain; current != NULL; current = current->next) {
        for (ptr = current->start; ptr != current->end; ++ptr) {
            *ptr = toupper (*ptr);
        }
    }
}

/**
 * Перевод цепочки фрагментов в нижний регистр (в кодировке ASCII).
 *
 * @param chain Начало цепочки.
 */
MAGNA_API void MAGNA_CALL chain_tolower
    (
        ChainSpan *chain
    )
{
    ChainSpan *current;
    am_byte *ptr;

    assert (chain != NULL);

    for (current = chain; current != NULL; current = current->next) {
        for (ptr = current->start; ptr != current->end; ++ptr) {
            *ptr = tolower (*ptr);
        }
    }
}

/**
 * Заполнение цепочки фрагментов указанным значением.
 *
 * @param chain Начало цепочки.
 * @param value Значение-заполнитель.
 */
MAGNA_API void MAGNA_CALL chain_fill
    (
        ChainSpan *chain,
        am_byte value
    )
{
    ChainSpan *current;
    am_byte *ptr;

    assert (chain != NULL);

    for (current = chain; current != NULL; current = current->next) {
        for (ptr = current->start; ptr != current->end; ++ptr) {
            *ptr = value;
        }
    }
}

/**
 * Срез цепочки фрагментов.
 * Данные, на которые указывает цепочка, никак не меняются.
 *
 * @param chain Начало цепочки.
 * @param offset Смещение от начала.
 * @param length Длина среза.
 */
MAGNA_API void MAGNA_CALL chain_slice
    (
        ChainSpan *chain,
        size_t offset,
        size_t length
    )
{
    ChainSpan *link; /* текущее звено */
    ChainSpan *next; /* указатель на следующее звено */
    ChainSpan *tmp;  /* Переменная в цикле */
    am_byte *where; /* до куда домотали */
    size_t delta;   /* длина текущего звена */

    assert (chain != NULL);

    where = chain->start; /* Для успокоения линтера. */

    /* Сначала проматываем до нужного смещения. */
    for (link = chain; link != NULL; link = link->next) {
        delta = chain_length (link);
        if (offset < delta) {
            where = link->start + delta;
            goto DONE1;
        }
        else {
            offset -= delta;
        }
    }

    DONE1:
    if (link == NULL) {
        /* Домотали до самого конца, значит, нужно обнулять цепочку. */
        chain_destroy (chain);

        return;
    }

    chain->start = where;

    /* Домотали не до конца, значит, возможно,
       нужно будет откусить начальные звенья. */
    if (link != chain) {
        /* Мы вышли за пределы первого звена. Переносим начало цепочки. */

        /* Удаляем ненужные звенья цепочки. */
        for (tmp = chain->next; tmp != link; tmp = next) {
            next = tmp->next;
            mem_free (tmp);
        }

        /* Поправлем указатели. */
        chain->end  = link->end;
        chain->next = link;
    }

    /* Теперь проматываем длину. */
    /* Сначала в текущем звене. */
    delta = (size_t) (link->end - where);
    if (length < delta) {
        link->end = where + length;
        goto DONE2;
    }

    /* Потом в последующих. */
    for (; link != NULL; link = link->next) {
        delta = (size_t) (link->end - link->start);
        if (length < delta) {
            link->end = link->start + length;
            goto DONE2;
        }
        else {
            length -= delta;
        }
    }

    DONE2:

    if (link != NULL) {
        /* Домотали не до конца, значит, нужно обнулять цепочку. */
        /* Удаляем ненужные звенья цепочки. */
        for (tmp = link->next; tmp != NULL; tmp = next) {
            next = tmp->next;
            mem_free (tmp);
        }

        link->next = NULL;
    }
}

/**
 * Проход по цепочке в прямом направлении.
 *
 * @param chain Начало цепочки.
 * @param walker Функция-обработчик. Возвращает `AM_FALSE`, если необходимо прервать проход по цепочке.
 * @param extraData Указатель на звено, на котором был прерван проход по цепочке, либо `NULL`.
 * @return
 */
MAGNA_API ChainSpan* MAGNA_CALL chain_walk
    (
        ChainSpan *chain,
        ChainWalker walker,
        void *extraData
    )
{
    ChainSpan *link; /* Переменная цикла */

    assert (chain != NULL);
    assert (walker != NULL);

    for (link = chain; link != NULL; link = link->next) {
        if (!walker (link, extraData)) {
            return link;
        }
    }

    return NULL;
}

/**
 * Проход по цепочке в обратном направлении.
 *
 * @param chain Начало цепочки.
 * @param walker Функция-обработчик. Возвращает `AM_FALSE`, если необходимо прервать проход по цепочке.
 * @param extraData Указатель на звено, на котором был прерван проход по цепочке, либо `NULL`.
 * @return
 */
MAGNA_API ChainSpan* MAGNA_CALL chain_walk_reverse
    (
        ChainSpan *chain,
        ChainWalker walker,
        void *extraData
    )
{
    ChainSpan *link; /* переменная цикла */

    assert (chain != NULL);
    assert (walker != NULL);

    for (link = chain_last (chain); link != NULL; link = chain_before (chain, link)) {
        if (!walker (link, extraData)) {
            return link;
        }
    }

    return NULL;
}

/**
 * Преобразование спана в строку.
 * Выделяет память под строку в куче.
 *
 * @param span Спан для преобразования.
 * @return Размещенная в куче копия строки (с завершающим нулем).
 */
MAGNA_API am_byte* MAGNA_CALL chain_to_string
    (
        const ChainSpan *chain
    )
{
    am_byte *result; /* результат */
    am_byte *ptr;    /* временный указатель */
    ChainSpan *link; /* переменная цикла */
    size_t length;   /* общая длина строки */
    size_t delta;    /* длина звена */

    assert (chain != NULL);

    length = chain_total_length (chain) + 1;
    result = mem_alloc (length);
    if (result == NULL) {
        return NULL;
    }

    ptr = result;
    for (link = (ChainSpan*) chain; link != NULL; link = link->next) {
        delta = chain_length (link);
        mem_copy (ptr, link->start, delta);
        ptr += delta;
    }

    *ptr = 0;

    return result;
}

/**
 * Преобразование спана в вектор байтов.
 * Выделяет память под вектор в куче.
 *
 * @param span Спан для преобразования.
 * @return Размещенный в куче вектор байтов (копия данных).
 */
MAGNA_API am_byte* MAGNA_CALL chain_to_vector
    (
        const ChainSpan *chain
    )
{
    am_byte *result; /* результат */
    am_byte *ptr;    /* временный указатель */
    ChainSpan *link; /* переменная цикла */
    size_t length;   /* общая длина строки */
    size_t delta;    /* длина звена */

    assert (chain != NULL);

    length = chain_total_length (chain);
    result = mem_alloc (length);
    if (result == NULL) {
        return NULL;
    }

    ptr = result;
    for (link = (ChainSpan*) chain; link != NULL; link = link->next) {
        delta = chain_length (link);
        mem_copy (ptr, link->start, delta);
        ptr += delta;
    }

    return result;
}

/**
 * Совпадает ли начало цепочки с другой цепочкой?
 *
 * @param chain Начало проверяемой цепочки.
 * @param prefix Предполагаемый префикс.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL chain_starts_with
    (
        ChainSpan *chain,
        ChainSpan *prefix
    )
{
    assert (chain != NULL);
    assert (prefix != NULL);

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Совпадает ли конец цепочки с другой цепочкой?
 *
 * @param chain Начало проверяемой цепочки.
 * @param suffix Предполагаемый суффикс.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL chain_ends_with
    (
        ChainSpan *chain,
        ChainSpan *suffix
    )
{
    assert (chain != NULL);
    assert (suffix != NULL);

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Побайтовое сравнение двух цепочек фрагментов.
 *
 * @param first Начало первой цепочки.
 * @param second Начало второй цепочки.
 * @return Результат сравнения: &lt; 0, если первая цепочка меньше,
 * &gt; 0, если первая цепочке больше и = 0, если содержимое цепочек совпадает.
 */
MAGNA_API int MAGNA_CALL chain_compare
    (
        const ChainSpan *first,
        const ChainSpan *second
    )
{
    assert (first != NULL);
    assert (second != NULL);

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Побайтовое сравнение двух цепочек фрагментов без учета регистра символов
 * (ASCII).
 *
 * @param first Начало первой цепочки.
 * @param second Начало второй цепочки.
 * @return Результат сравнения: &lt; 0, если первая цепочка меньше,
 * &gt; 0, если первая цепочке больше и = 0, если содержимое цепочек совпадает.
 */
MAGNA_API int MAGNA_CALL chain_compare_ignore_case
    (
        const ChainSpan *first,
        const ChainSpan *second
    )
{
    assert (first != NULL);
    assert (second != NULL);

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Побайтовое сравнение цепочки фрагментов с единичным фрагментом.
 *
 * @param chain Начало цепочки.
 * @param span Единичный фрагмент.
 * @return Результат сравнения.
 */
MAGNA_API int MAGNA_CALL chain_compare_span
    (
        const ChainSpan *chain,
        Span span
    )
{
    assert (chain != NULL);

    /* TODO: implement */

    return 0;
}

/**
 * Побайтовое сравнение цепочки фрагментов со строкой символов.
 *
 * @param chain Начало цепочки.
 * @param text Строка символов
 * @return Результат сравнения.
 */
MAGNA_API int MAGNA_CALL chain_compare_text
    (
        const ChainSpan *chain,
        const am_byte *text
    )
{
    assert (chain != NULL);
    assert (text != NULL);

    /* TODO: implement */

    return 0;
}

/**
 * Запись цепочки фрагментов на консоль.
 *
 * @param chain Начало цепочки.
 */
MAGNA_API void MAGNA_CALL chain_to_console
    (
        const ChainSpan *chain
    )
{
    assert (chain != NULL);

    /* TODO: implement */
}

/**
 * Запись цепочки фрагментов в файл.
 *
 * @param chain Начало цепочки.
 * @param handle Файловый дескриптор.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL chain_to_file
    (
        const ChainSpan *chain,
        am_handle handle
    )
{
    assert (chain != NULL);
    assert (handle_is_good (handle));

    /* TODO: implement */

    return AM_FALSE;
}

/**
 * Конкатенация двух цепочек фрагментов.
 *
 * @param target Начало принимающей цепочки.
 * @param source Начало добавляемой цепочки.
 */
MAGNA_API void MAGNA_CALL chain_concat
    (
        ChainSpan *target,
        const ChainSpan *source
    )
{
    ChainSpan *last; /* последнее звено в target */

    assert (target != NULL);
    assert (source != NULL);
    assert (target != source);

    last = chain_last (source);
    last->next = (ChainSpan*) source;
}

/**
 * Оптимизация цепочки.
 * Удаляет из цепочки пустые элементы.
 *
 * @param chain Начало цепочки.
 */
MAGNA_API void MAGNA_CALL chain_optimize
    (
        ChainSpan *chain
    )
{
    assert (chain != NULL);

    /* TODO: implement */
}

/**
 * Верификация цепочки.
 * Каждый элемент цепочки должен удовлетворять условию
 * `start &lt;= end`.
 *
 * @param chain Начало цепочки.
 * @return Результат верификации.
 */
MAGNA_API am_bool MAGNA_CALL chain_verify
    (
        const ChainSpan *chain
    )
{
    assert (chain != NULL);

    while (chain != NULL) {
        if (chain->start > chain->end) {
            return AM_FALSE;
        }

        chain = chain->next;
    }

    return AM_TRUE;
}

/**
 * Получение прямого итератора по цепочке фрагментов.
 *
 * @param chain Начало цепочки.
 * @return Итератор.
 */
MAGNA_API ChainIterator MAGNA_CALL chain_get_forward_iterator
    (
        const ChainSpan *chain
    )
{
    ChainIterator result;

    assert (chain != NULL);

    result.chain   = (ChainSpan*) chain;
    result.current = (ChainSpan*) chain;
    result.ptr     = chain->start;

    return result;
}

/**
 * Получение реверсного итератора по цепочке фрагментов.
 *
 * @param chain Начало цепочки.
 * @return Итератор.
 */
MAGNA_API ChainIterator MAGNA_CALL chain_get_reverse_iterator
    (
        const ChainSpan *chain
    )
{
    ChainIterator result;

    assert (chain != NULL);

    result.chain   = (ChainSpan*) chain;
    result.current = chain_last (chain);
    result.ptr     = result.current->end;

    return result;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
