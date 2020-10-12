// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#define TESTER_MAIN
#include "magna/tester.h"
#include "magna/irbis.h"

#include "offline.h"

#include <assert.h>

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
    appendix.position = appendix.capacity;

    while (AM_TRUE) {
        if (buffer_is_empty(path)) {
            break;
        }

        buffer_clear (&candidate);
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
        path->position = directory.len;
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

    if (where_test_data (&tdp)) {
        printf ("test data=");
        fwrite (tdp.ptr, 1, tdp.position, stdout);
        printf ("\n");
        buffer_destroy (&tdp);
    }

    if (path_get_temporary_directory (&td)) {
        printf ("temp dir=");
        fwrite (td.ptr, 1, td.position, stdout);
        printf ("\n");
        buffer_destroy (&td);
    }

    printf ("memory available=%lld\n", mem_avail_physical());
    printf ("sizeof (void*)      = %d\n", (int) sizeof (void*));
    printf ("sizeof (Buffer)     = %d\n", (int) sizeof (Buffer));
    printf ("sizeof (Array)      = %d\n", (int) sizeof (Array));
    printf ("sizeof (SubField)   = %d\n", (int) sizeof (SubField));
    printf ("sizeof (MarcField)  = %d\n", (int) sizeof (MarcField));
    printf ("sizeof (MarcRecord) = %d\n", (int) sizeof (MarcRecord));
    printf ("sizeof (Connection) = %d\n", (int) sizeof (Connection));

    int result = tester_main (argc, argv);

    return result;
}
