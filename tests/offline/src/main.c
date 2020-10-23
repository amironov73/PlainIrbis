// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#define TESTER_MAIN
#include "magna/tester.h"
#include "magna/irbis.h"

#include "offline.h"

#include <assert.h>

#define STR(x)   #x
#define SHOW_DEFINE(x) printf ("%s=%s\n", #x, STR(x))

am_bool where_test_data
    (
        Buffer *path
    )
{
    am_bool result = AM_FALSE;
    Buffer candidate = BUFFER_INIT;
    Buffer appendix;
    const char *testData = "data";
    const char *text;
    Span directory;

    assert (path != NULL);

    if (!path_get_current_directory (path)) {
        return AM_FALSE;
    }

    buffer_static (&appendix, CBTEXT (testData), strlen (testData));

    while (AM_TRUE) {
        if (buffer_is_empty(path)) {
            break;
        }

        buffer_clear (&candidate);
        if (candidate.start != NULL) {
            mem_clear(candidate.start, buffer_capacity(&candidate));
        }
        if (!path_combine (&candidate, path, &appendix, NULL)) {
            break;
        }

        text = (char*) buffer_to_text (&candidate);
        if (text == NULL) {
            break;
        }

        if (directory_exist (text)) {
            result = buffer_copy (path, &candidate);
            break;
        }

        directory = path_get_directory (path);
        path->current = path->start + span_length (directory);
        mem_clear (path->current, path->end - path->current);
    }

    buffer_destroy (&candidate);

    return result;
}

TESTER(magna_get_version)
{
    int version = magna_get_version ();
    CHECK (version == 0x0001);
}

TESTER(magna_require_version)
{
    CHECK (magna_require_version (0x0001));
}

TESTER(magna_on_windows)
{
    am_bool rc = magna_on_windows ();

#ifdef MAGNA_WINDOWS
    CHECK (rc);
#else
    CHECK (!rc);
#endif
}

/*=========================================================*/

int main (int argc, const char **argv)
{
    Buffer tdp = BUFFER_INIT;
    Buffer td = BUFFER_INIT;

    SHOW_DEFINE (CMAKE_C_COMPILER_ID);
    SHOW_DEFINE (CMAKE_SYSTEM_NAME);

    if (where_test_data (&tdp)) {
        printf ("test data=");
        buffer_to_console (&tdp);
        printf ("\n");
        buffer_destroy (&tdp);
    }

    if (path_get_temporary_directory (&td)) {
        printf ("temp dir=");
        buffer_to_console (&td);
        printf ("\n");
        buffer_destroy (&td);
    }

    printf ("memory available       = %lld\n", mem_avail_physical());
    printf ("size of size_t         = %d\n", (int) SIZEOF_SIZE_T);
    printf ("sizeof (void*)         = %d\n", (int) sizeof (void*));
    printf ("sizeof (Span)          = %d\n", (int) sizeof (Span));
    printf ("sizeof (ChainSpan)     = %d\n", (int) sizeof (ChainSpan));
    printf ("sizeof (Buffer)        = %d\n", (int) sizeof (Buffer));
    printf ("sizeof (Array)         = %d\n", (int) sizeof (Array));
    printf ("sizeof (Vector)        = %d\n", (int) sizeof (Vector));
    printf ("sizeof (TextNavigator) = %d\n", (int) sizeof (TextNavigator));
    printf ("sizeof (SubField)      = %d\n", (int) sizeof (SubField));
    printf ("sizeof (MarcField)     = %d\n", (int) sizeof (MarcField));
    printf ("sizeof (MarcRecord)    = %d\n", (int) sizeof (MarcRecord));
    printf ("sizeof (Query)         = %d\n", (int) sizeof (Query));
    printf ("sizeof (Response)      = %d\n", (int) sizeof (Response));
    printf ("sizeof (Connection)    = %d\n", (int) sizeof (Connection));

    int result = tester_main (argc, argv);

    return result;
}
