/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#ifndef MAGNA_TESTER_H
#define MAGNA_TESTER_H

#include "magna/core.h"

/*=========================================================*/

#define TESTER_MAGIC 0x0BED0EDA

typedef struct
{
    char *test_name;
    void (*run)(void);
    int magic;
} Tester;

/*=========================================================*/

#ifdef _MSC_VER

#define TESTER_SECTION(_name) __declspec(allocate("tester")) _name

#else

  #ifdef __APPLE__

    #define TESTER_SECTION(_name) _name __attribute__((used, section ("__DATA, tester"), aligned (1)))

  #else

    #define TESTER_SECTION(_name) _name __attribute__((used, section ("tester"), aligned (1)))

  #endif

#endif

#ifdef _MSC_VER

#define TESTER(name)                                     \
static void tester_ ## name ## _function (void);         \
                                                         \
static Tester tester_ ## name ## _data =                 \
{                                                        \
    #name,                                               \
    tester_ ## name ## _function,                        \
    TESTER_MAGIC                                         \
};                                                       \
                                                         \
__pragma(data_seg(push, "tester"))                       \
static __declspec(align(4)) Tester *tester_ ## name ## _pointer =             \
  &tester_ ## name ## _data;                             \
__pragma(data_seg(pop))                                  \
                                                         \
static void tester_ ## name ## _function (void)

#else

#define TESTER(name) \
static char tester_ ## name ## _name[] = {#name};        \
                     \
static void tester_ ## name ## _function (void);         \
                     \
static Tester TESTER_SECTION(tester_ ## name ## _data) = \
{                                                        \
    tester_ ## name ## _name,                            \
    tester_ ## name ## _function,                        \
    TESTER_MAGIC                                         \
};                                                       \
                                                         \
static void tester_ ## name ## _function (void)

#endif

/*=========================================================*/

void TESTER_LOG (const char *format, ...);
void TESTER_ERROR (const char *format, ...);

void assert_string    (const char *expected, const char *actual, const char *caller, int line);
void assert_int_32    (am_int32 expected, am_int32 actual, const char *caller, int line);
void assert_int_64    (am_int64 expected, am_int64 actual, const char *caller, int line);
void assert_not_null  (void *actual, const char *caller, int line);
void assert_fail      (const char *caller, int line);
void assert_check     (am_bool value, const char *condition, const char *caller, int line);

/*=========================================================*/

#define ASSERT_STR(_exp, _act)    assert_string   ((_exp), (_act),   __FILE__, __LINE__)
#define ASSERT_INT_32(_exp, _act) assert_int_32   ((_exp), (_act),   __FILE__, __LINE__)
#define ASSERT_INT_64(_exp, _act) assert_int_64   ((_exp), (_act),   __FILE__, __LINE__)
#define ASSERT_NOT_NULL(_act)     assert_not_null ((_exp),           __FILE__, __LINE__)
#define ASSERT_FAIL()             assert_fail     (                  __FILE__, __LINE__)
#define CHECK(_cond)              assert_check    ((_cond), #_cond,  __FILE__, __LINE__)

/*=========================================================*/

#ifdef TESTER_MAIN

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

static jmp_buf tester_err;

void TESTER_LOG (const char *format, ...)
{
    va_list args;
    printf ("LOG: ");
    va_start (args, format);
    vprintf (format, args);
    va_end (args);
}

void TESTER_ERROR (const char *format, ...)
{
    va_list args;
    printf ("ERROR: ");
    va_start (args, format);
    vprintf (format, args);
    va_end (args);
    longjmp(tester_err, 1);
}

void assert_string (const char *expected, const char *actual, const char *caller, int line)
{
    if ((expected == NULL && actual != NULL)
        || (expected != NULL && actual == NULL)
        || (expected != NULL && actual != NULL && strcmp (expected, actual) != 0)) {
        TESTER_ERROR ("%s:%d: expected '%s', got '%s'", caller, line, expected, actual);
    }
}

void assert_int_32 (am_int32 expected, am_int32 actual, const char *caller, int line)
{
    if (expected != actual) {
        TESTER_ERROR ("%s:%d: expected %d, got %d", caller, line, expected, actual);
    }
}

void assert_int_64 (am_int64 expected, am_int64 actual, const char *caller, int line)
{
    if (expected != actual) {
        TESTER_ERROR ("%s:%d: expected %ld, got %ld", caller, line, expected, actual);
    }
}

void assert_not_null (void *actual, const char *caller, int line)
{
    if (actual != NULL) {
        TESTER_ERROR ("%s:%d: should not be NULL", caller, line);
    }
}

void assert_fail (const char *caller, int line)
{
    TESTER_ERROR ("%s:%d: should not happen", caller, line);
}

void assert_check (am_bool value, const char *condition, const char *caller, int line)
{
    if (!value) {
        TESTER_ERROR ("%s: %d: condition failed: %s", caller, line, condition);
    }
}

TESTER(_anchor_) {}

static int tester_main (int argc, const char **argv)
{
    Tester *begin = &tester__anchor__data;
    Tester *end = begin;
    Tester *ptr;
    int rc;
    int success = 0, failure = 0;

    (void) argc;
    (void) argv;

    while (1) {
        --begin;
        if (begin->magic != TESTER_MAGIC) {
            ++begin;
            break;
        }
    }

    while (1) {
        ++end;
        if (end->magic != TESTER_MAGIC) {
            break;
        }
    }

    for (ptr = begin; ptr != end; ++ptr) {
        if (ptr->run != tester__anchor__function) {
            printf ("%-30s", ptr->test_name);
            rc = setjmp (tester_err);
            if (rc == 0) {
                ptr->run();
            }
            printf (" [%s]\n", rc ? "FAIL" : "OK");
            if (rc) {
                ++failure;
            }
            else {
                ++success;
            }
        }
    }

    printf ("\nTOTAL: success=%d, failure=%d\n", success, failure);

    return failure;
}

#endif

/*=========================================================*/

#endif
