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

/*=========================================================*/

/**
 * \file logging.c
 *
 * Простое логирование.
 *
 * Уровни логирования:
 *
 * * Trace — вывод всего подряд. На тот случай, если Debug
 *   не позволяет локализовать ошибку. В нем полезно отмечать
 *   вызовы разнообразных блокирующих и асинхронных операций.
 *
 * * Debug — журналирование моментов вызова «крупных» операций.
 *   Старт/остановка потока, запрос пользователя и т.п.
 *
 * * Info — разовые операции, которые повторяются крайне редко,
 *   но не регулярно. (загрузка конфига, плагина, запуск бэкапа).
 *
 * * Warning — неожиданные параметры вызова, странный формат запроса,
 *   использование дефолтных значений в замен не корректных.
 *   Вообще все, что может свидетельствовать о не штатном использовании.
 *
 * * Error — повод для внимания разработчиков. Тут интересно окружение
 *   конкретного места ошибки.
 *
 * * Fatal — тут и так понятно. Выводим все до чего дотянуться можем,
 *   так как дальше приложение работать не будет.
 *
 */

/*=========================================================*/

/**
 * Вывод сообщения (в сыром виде).
 *
 * @param data Указатель на данные.
 * @param size Размер данных в байтах.
 */
MAGNA_API void MAGNA_CALL log_write
    (
        const am_byte *data,
        size_t size
    )
{
    assert (data != NULL);

    fwrite (data, 1, size, stdout);
}

/**
 * Вывод сообщения.
 *
 * @param file
 * @param function
 * @param line
 * @param level
 * @param message
 */
MAGNA_API void MAGNA_CALL log_message
    (
        const char *file,
        const char *function,
        int line,
        int level,
        const char *message
    )
{
    char buf [16];

    assert (file != NULL);
    assert (message != NULL);

    (void) function;
    (void) line;

    log_write (CBTEXT (message), strlen (message));

    if (level == LOG_LEVEL_FATAL) {
        abort();
    }
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/

