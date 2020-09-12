/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#ifdef _MSC_VER

#include <windows.h>

#endif

#include <assert.h>

/*=========================================================*/

/**
 * \file retry.c
 *
 * Повторение выполнения кода.
 */

/*=========================================================*/

/**
 * Инициализация менеджера настойчивости.
 *
 * @param manager
 */
MAGNA_API void MAGNA_CALL retry_init
    (
        RetryManager *manager
    )
{
    assert (manager != NULL);

    memset (manager, 0, sizeof (RetryManager));
    manager->sleepInterval = 100;
    manager->retryLimit = 5;
}

/**
 * Настойчивое исполнение.
 *
 * @param manager
 * @param action
 * @return
 */
MAGNA_API void* MAGNA_CALL retry_action
    (
        RetryManager *manager,
        Action action,
        void *data
    )
{
    void *result = NULL;
    int count;

    assert (manager != NULL);
    assert (action != NULL);

    for (count = 0; count < manager->retryLimit; ++count) {

#ifdef _MSC_VER
        __try
        {
#endif

            result = action(data);

#ifdef _MSC_VER
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            result = NULL;
        }
#endif


        if (result) {
            if (manager->onSuccess != NULL) {
                manager->onSuccess (manager);
            }

            break;
        }

        if (manager->onError != NULL) {
            if (!manager->onError (manager)) {
                break;
            }
        }

        if (manager->sleepInterval != 0
            && count < (manager->retryLimit - 1)) {
            magna_sleep (manager->sleepInterval);
        }
    }

    return result;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
