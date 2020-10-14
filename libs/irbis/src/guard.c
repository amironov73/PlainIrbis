// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/irbis.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

/**
 * \file guard.c
 *
 * Как irbis_server.exe работает с многозадачностью
 *
 * При старте:
 *
 * ```
 * HANDLE hStarted = CreateEventA
 *   (
 *     NULL, // lpEventAttributes
 *     TRUE, // bManualReset
 *     FALSE, // bInitialState
 *     "IRBIS64_STARTED" // lpName
 *   );
 * ```
 *
 * Затем сразу однократно
 *
 * ```
 * HRESULT result = WaitForSingleObject
 *   (
 *     hStarted, // handle
 *     1, // milliseconds
 *   );
 * ```
 *
 * Выдаёт `WAIT_TIMEOUT` (т. е. других копий сервера в памяти нет).
 *
 * Затем сразу
 *
 * ```
 * HRESULT result = SetEvent (hStarted);
 * ```
 *
 * Возвращает `TRUE` (т. е. застолбили).
 *
 * Затем сразу
 *
 * ```
 * HANDLE hStop = CreateEventA
 *   (
 *     NULL, // lpEventAttributes
 *     TRUE, // bManualReset
 *     FALSE, // bInitialState
 *     "IRBIS64_STOP_" // lpName
 *   );
 * ```
 *
 * Затем периодически (несколько раз в секунду)
 *
 * ```
 * HRESULT result = WaitForSingleObject
 *   (
 *     hStop, // handle
 *     1, // milliseconds
 *   );
 * ```
 *
 * Возвращает `WAIT_TIMEOUT`.
 *
 * При завершении работы
 *
 * ```
 * ResetEvent (hStarted);
 * ```
 *
 * Возвращает `TRUE`.
 *
 */

#ifdef MAGNA_WINDOWS

#include <windows.h>
#include <assert.h>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#pragma warning(disable: 4152)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#define STARTED "IRBIS64_STARTED"
#define STOPPED "IRBIS64_STOP_"

static HANDLE startHandle = NULL;
static HANDLE stopHandle = NULL;

/*=========================================================*/

/**
 * Создает события, регулирующие запуск
 * и остановку сервера ИРБИС64.
 *
 */
MAGNA_API int server_guard_create (void)
{
    if (startHandle == NULL) {
        startHandle = CreateEventA
            (
                NULL,   /* lpEventAttributes */
                TRUE,   /* bManualReset */
                FALSE,  /* bInitialState */
                STARTED /* lpName */
            );
        if (startHandle == NULL) {
            return 0;
        }
    }

    if (stopHandle == NULL) {
        stopHandle = CreateEventA
            (
                NULL,   /* lpEventAttributes */
                TRUE,   /* bManualReset */
                FALSE,  /* bInitialState */
                STOPPED /* lpName */
            );
        if (stopHandle == NULL) {
            CloseHandle(stopHandle);
            return 0;
        }
    }

    return 1;
}

/*=========================================================*/

/**
 * Освобождает события, регулирующие запуск
 * и остановку сервера ИРБИС64.
 */
MAGNA_API int server_guard_close (void)
{
    if (startHandle != NULL) {
        CloseHandle (startHandle);
        startHandle = NULL;
    }

    if (stopHandle != NULL) {
        CloseHandle (stopHandle);
        stopHandle = NULL;
    }

    return 1;
}

/*=========================================================*/

/**
 * Проверяет, запущен ли другой экземпляр
 * сервера ИРБИС64.
 */
MAGNA_API int check_other_server_running (void)
{
    assert (startHandle != NULL);

    HRESULT hr = WaitForSingleObject
        (
            startHandle, /* handle */
            1 /* milliseconds */
        );

    return (hr != WAIT_TIMEOUT);
}

/*=========================================================*/

/**
 * Проверяет, был ли инициирован останов сервера.
 */
MAGNA_API int check_server_stop_requested (void)
{
    assert (stopHandle != NULL);

    HRESULT hr = WaitForSingleObject
        (
            stopHandle, /* handle */
            1 /* milliseconds */
        );

    return (hr == WAIT_OBJECT_0);
}

/*=========================================================*/

/**
 * Заявляет "Я есть сервер! Да не будет других
 * серверов перед лицом моим!"
 */
MAGNA_API void say_i_am_running (void)
{
    assert (startHandle != NULL);

    BOOL hr = SetEvent(startHandle);
    assert (hr == TRUE);
}

/*=========================================================*/

/**
 * Завявляет "Я устал, я ухожу!"
 * от имени сервера.
 */
MAGNA_API void say_i_am_stopped (void)
{
    assert (startHandle != NULL);

    BOOL hr = ResetEvent (startHandle);
    assert (hr == TRUE);
}

/*=========================================================*/

/**
 * Запрашивает останов сервера ИРБИС64.
 */
MAGNA_API void request_server_stop (void)
{
    assert (stopHandle != NULL);

    BOOL hr = SetEvent (stopHandle);
    assert (hr == TRUE);
}

/*=========================================================*/

#endif

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
