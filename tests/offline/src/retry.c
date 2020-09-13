/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/tester.h"

static void* MAGNA_CALL canary_action (void *data)
{
    int *ptr = (int *) data;

    if (*ptr == 2) {
        return data;
    }

    ++(*ptr);

    return NULL;
}

TESTER(retry_action_1)
{
    int counter = 0;
    void *result;
    RetryManager manager;

    retry_init (&manager);
    result = retry_action (&manager, canary_action, &counter);

    CHECK (counter == 2);
    CHECK (result == &counter);
}

TESTER(retry_action_2)
{
    int counter = 0;
    void *result;
    RetryManager manager;

    retry_init (&manager);
    manager.retryLimit = 1;
    result = retry_action (&manager, canary_action, &counter);

    CHECK (counter == 1);
    CHECK (result == NULL);
}
