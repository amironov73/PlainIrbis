// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"
#include "offline.h"

TESTER(file_create_1)
{
    Buffer tempDirectory = BUFFER_INIT;
    Buffer fileName = BUFFER_INIT;
    Buffer tempFile = BUFFER_INIT;
    const char *fname;
    am_byte original [] = { 3, 14, 15, 9, 26, 5 }, buffer [sizeof (original)];
    am_handle handle;
    am_uint64 fileSize, readSize;

    CHECK (path_get_temporary_directory (&tempDirectory));
    CHECK (directory_exist (buffer_to_text (&tempDirectory)));
    CHECK (buffer_from_text (&fileName, "magna.tmp") != NULL);
    CHECK (path_combine (&tempFile, &tempDirectory, &fileName, NULL));

    fname = buffer_to_text (&tempFile);
    CHECK (fname != NULL);
    if (file_exist (fname)) {
        CHECK (file_delete (fname));
    }

    handle = file_create (fname);
    CHECK (handle_is_good (handle));
    CHECK (file_write (handle, original, sizeof (original)));
    fileSize = file_size (handle);
    CHECK (file_close (handle));
    CHECK (fileSize == sizeof (original));

    handle = file_open_read (fname);
    CHECK (handle_is_good (handle));
    readSize = file_read (handle, buffer, sizeof (buffer));
    CHECK (file_close (handle));
    CHECK (readSize == sizeof (buffer));

    handle = file_open_write (fname);
    CHECK (file_write (handle, original, sizeof (original)));
    CHECK (file_close (handle));

    buffer_destroy (&tempDirectory);
    buffer_destroy (&fileName);
    buffer_destroy (&tempFile);
}

TESTER(file_create_insist_1)
{
    Buffer tempDirectory = BUFFER_INIT;
    Buffer fileName = BUFFER_INIT;
    Buffer tempFile = BUFFER_INIT;
    const char *fname;
    am_byte original [] = { 3, 14, 15, 9, 26, 5 }, buffer [sizeof (original)];
    am_handle handle;
    am_uint64 fileSize, readSize;

    CHECK (path_get_temporary_directory (&tempDirectory));
    CHECK (directory_exist (buffer_to_text (&tempDirectory)));
    CHECK (buffer_from_text (&fileName, "magna.tmp") != NULL);
    CHECK (path_combine (&tempFile, &tempDirectory, &fileName, NULL));

    fname = buffer_to_text (&tempFile);
    CHECK (fname != NULL);
    if (file_exist (fname)) {
        CHECK (file_delete (fname));
    }

    handle = file_create_insist (fname, 100, 5);
    CHECK (handle_is_good (handle));
    CHECK (file_write (handle, original, sizeof (original)));
    fileSize = file_size (handle);
    CHECK (file_close (handle));
    CHECK (fileSize == sizeof (original));

    handle = file_open_read_insist (fname, 100, 5);
    CHECK (handle_is_good (handle));
    readSize = file_read (handle, buffer, sizeof (buffer));
    CHECK (file_close (handle));
    CHECK (readSize == sizeof (buffer));

    handle = file_open_write_insist (fname, 100, 5);
    CHECK (file_write (handle, original, sizeof (original)));
    CHECK (file_close (handle));

    buffer_destroy (&tempDirectory);
    buffer_destroy (&fileName);
    buffer_destroy (&tempFile);
}

TESTER(file_open_read_1)
{
    Buffer path = BUFFER_INIT;
    am_byte junk[100];
    am_handle handle;

    CHECK (where_test_data (&path));
    CHECK (path_append (&path, span_from_text ("TEST1.ISO")));

    handle = file_open_read (buffer_to_text (&path));
    CHECK (handle_is_good (handle));
    CHECK (file_tell (handle) == MAGNA_INT64 (0));
    CHECK (file_read (handle, junk, sizeof (junk)) == sizeof (junk));
    CHECK (junk[0] == 0x30);
    CHECK (junk[1] == 0x30);
    CHECK (file_tell (handle) == MAGNA_INT64 (100));

    CHECK (file_seek (handle, MAGNA_INT64 (0)));
    CHECK (file_tell (handle) == MAGNA_INT64 (0));

    CHECK (file_close (handle));

    buffer_destroy (&path);
}

TESTER(file_size_1)
{
    Buffer path = BUFFER_INIT;
    am_handle handle;

    CHECK (where_test_data (&path));
    CHECK (path_append (&path, span_from_text ("TEST1.ISO")));

    handle = file_open_read (buffer_to_text (&path));
    CHECK (handle_is_good (handle));
    CHECK (!file_eof (handle));
    CHECK (file_size (handle) == MAGNA_INT64 (78096));
    CHECK (file_close (handle));

    buffer_destroy (&path);
}

TESTER(file_read_all_1)
{
    Buffer path = BUFFER_INIT;
    Buffer all = BUFFER_INIT;

    CHECK (where_test_data (&path));
    CHECK (path_append (&path, span_from_text ("org.mnu")));

    CHECK (file_read_all (buffer_to_text (&path), &all));
    CHECK (all.position == 86);

    buffer_destroy (&path);
    buffer_destroy (&all);
}

TESTER(file_read_byte_1)
{
    Buffer path = BUFFER_INIT;
    am_handle handle;

    CHECK (where_test_data (&path));
    CHECK (path_append (&path, span_from_text ("org.mnu")));

    handle = file_open_read (buffer_to_text (&path));
    CHECK (handle_is_good (handle));
    CHECK (file_read_byte (handle) == 0x31);
    CHECK (file_close (handle));

    buffer_destroy (&path);
}

TESTER(file_read_int32_1)
{
    Buffer path = BUFFER_INIT;
    am_handle handle;

    CHECK (where_test_data (&path));
    CHECK (path_append (&path, span_from_text ("Irbis64/Datai/IBIS/ibis.xrf")));

    handle = file_open_read (buffer_to_text (&path));
    CHECK (handle_is_good (handle));
    CHECK (file_read_int32 (handle) == 0x015E34BCu);
    CHECK (file_close (handle));

    buffer_destroy (&path);
}

TESTER(file_read_int64_1)
{
    Buffer path = BUFFER_INIT;
    am_handle handle;

    CHECK (where_test_data (&path));
    CHECK (path_append (&path, span_from_text ("Irbis64/Datai/IBIS/ibis.xrf")));

    handle = file_open_read (buffer_to_text (&path));
    CHECK (handle_is_good (handle));
    CHECK (file_read_int64 (handle) == MAGNA_UINT64 (0x00000000015E34BC));
    CHECK (file_close (handle));

    buffer_destroy (&path);
}

TESTER(file_read_line_1)
{
    Buffer path = BUFFER_INIT;
    Buffer line = BUFFER_INIT;
    am_handle handle;

    CHECK (where_test_data (&path));
    CHECK (path_append (&path, span_from_text ("org.mnu")));

    handle = file_open_read (buffer_to_text (&path));
    CHECK (handle_is_good (handle));
    CHECK (file_read_line (handle, &line));
    CHECK (buffer_compare_text (&line, "1") == 0);
    buffer_clear (&line);
    CHECK (file_read_line (handle, &line));
    CHECK (buffer_compare_text (&line, "RU") == 0);
    CHECK (file_close (handle));

    buffer_destroy (&path);
    buffer_destroy (&line);
}

TESTER(file_write_byte_1)
{
    Buffer path = BUFFER_INIT;
    const char *fname;
    am_handle handle;

    CHECK (path_get_temporary_directory (&path));
    CHECK (path_append (&path, span_from_text ("writebuf.tmp")));

    fname = buffer_to_text (&path);
    CHECK (fname != NULL);
    if (file_exist (fname)) {
        CHECK (file_delete (fname));
    }

    handle = file_create (fname);
    CHECK (handle_is_good (handle));
    CHECK (file_write_byte (handle, 0x01));
    CHECK (file_write_byte (handle, 0x02));
    CHECK (file_write_byte (handle, 0x03));

    CHECK (file_close (handle));

    handle = file_open_read (fname);
    CHECK (handle_is_good (handle));
    CHECK (file_read_byte (handle) == 0x01);
    CHECK (file_read_byte (handle) == 0x02);
    CHECK (file_read_byte (handle) == 0x03);
    CHECK (file_close (handle));

    CHECK (file_delete (fname));

    buffer_destroy (&path);
}

TESTER(file_write_int32_1)
{
    Buffer path = BUFFER_INIT;
    const char *fname;
    am_handle handle;

    CHECK (path_get_temporary_directory (&path));
    CHECK (path_append (&path, span_from_text ("writebuf.tmp")));

    fname = buffer_to_text (&path);
    CHECK (fname != NULL);
    if (file_exist (fname)) {
        CHECK (file_delete (fname));
    }

    handle = file_create (fname);
    CHECK (handle_is_good (handle));
    CHECK (file_write_int32 (handle, 0x01020304));
    CHECK (file_write_int32 (handle, 0x05060708));
    CHECK (file_write_int32 (handle, 0x090A0B0C));

    CHECK (file_close (handle));

    handle = file_open_read (fname);
    CHECK (handle_is_good (handle));
    CHECK (file_read_int32 (handle) == 0x01020304);
    CHECK (file_read_int32 (handle) == 0x05060708);
    CHECK (file_read_int32 (handle) == 0x090A0B0C);
    CHECK (file_close (handle));

    CHECK (file_delete (fname));

    buffer_destroy (&path);
}

TESTER(file_write_int64_1)
{
    Buffer path = BUFFER_INIT;
    const char *fname;
    am_handle handle;

    CHECK (path_get_temporary_directory (&path));
    CHECK (path_append (&path, span_from_text ("writebuf.tmp")));

    fname = buffer_to_text (&path);
    CHECK (fname != NULL);
    if (file_exist (fname)) {
        CHECK (file_delete (fname));
    }

    handle = file_create (fname);
    CHECK (handle_is_good (handle));
    CHECK (file_write_int64 (handle, MAGNA_UINT64 (0x0102030405060708)));
    CHECK (file_write_int64 (handle, MAGNA_UINT64 (0x090A0B0C0D0E0F00)));

    CHECK (file_close (handle));

    handle = file_open_read (fname);
    CHECK (handle_is_good (handle));
    CHECK (file_read_int64 (handle) == MAGNA_UINT64 (0x0102030405060708));
    CHECK (file_read_int64 (handle) == MAGNA_UINT64 (0x090A0B0C0D0E0F00));
    CHECK (file_close (handle));

    CHECK (file_delete (fname));

    buffer_destroy (&path);
}

TESTER(file_write_buffer_1)
{
    Buffer path = BUFFER_INIT;
    Buffer output = { "Hello, world!", 13, 13 };
    const char *fname;
    am_handle handle;

    CHECK (path_get_temporary_directory (&path));
    CHECK (path_append (&path, span_from_text ("writebuf.tmp")));

    fname = buffer_to_text (&path);
    CHECK (fname != NULL);
    if (file_exist (fname)) {
        CHECK (file_delete (fname));
    }

    handle = file_create (fname);
    CHECK (handle_is_good (handle));
    CHECK (file_write_buffer (handle, &output));

    CHECK (file_close (handle));
    CHECK (file_delete (fname));

    buffer_destroy (&path);
}

TESTER(file_write_span_1)
{
    Buffer path = BUFFER_INIT;
    Span output = span_init ("Hello, world!", 13);
    const char *fname;
    am_handle handle;

    CHECK (path_get_temporary_directory (&path));
    CHECK (path_append (&path, span_from_text ("writebuf.tmp")));

    fname = buffer_to_text (&path);
    CHECK (fname != NULL);
    if (file_exist (fname)) {
        CHECK (file_delete (fname));
    }

    handle = file_create (fname);
    CHECK (handle_is_good (handle));
    CHECK (file_write_span (handle, output));

    CHECK (file_close (handle));
    CHECK (file_delete (fname));

    buffer_destroy (&path);
}

TESTER(file_write_text_1)
{
    Buffer path = BUFFER_INIT;
    const char *output = "Hello, world!";
    const char *fname;
    am_handle handle;

    CHECK (path_get_temporary_directory (&path));
    CHECK (path_append (&path, span_from_text ("writebuf.tmp")));

    fname = buffer_to_text (&path);
    CHECK (fname != NULL);
    if (file_exist (fname)) {
        CHECK (file_delete (fname));
    }

    handle = file_create (fname);
    CHECK (handle_is_good (handle));
    CHECK (file_write_text (handle, output));
    CHECK (file_sync (handle));

    CHECK (file_close (handle));
    CHECK (file_delete (fname));

    buffer_destroy (&path);
}

TESTER(directory_create_1)
{
    Buffer path = BUFFER_INIT;
    const char *name;

    CHECK (path_get_temporary_directory (&path));
    CHECK (path_append (&path, span_from_text ("magnadir")));

    name = buffer_to_text (&path);
    CHECK (name != NULL);

    if (directory_exist (name)) {
        CHECK (directory_delete (name));
    }

    CHECK (directory_create (name, AM_TRUE));
    CHECK (directory_delete (name));

    CHECK (directory_create (name, AM_FALSE));
    CHECK (directory_delete (name));

    buffer_destroy (&path);
}

TESTER(file_copy_1)
{
    Buffer spath = BUFFER_INIT, tpath = BUFFER_INIT;
    const char *sname, *tname;

    CHECK (where_test_data (&spath));
    CHECK (path_append (&spath, span_from_text ("org.mnu")));
    CHECK (path_get_temporary_directory (&tpath));
    CHECK (path_append (&tpath, span_from_text ("org.mnu")));

    sname = buffer_to_text (&spath);
    CHECK (sname != NULL);
    CHECK (file_exist (sname));
    tname = buffer_to_text (&tpath);
    CHECK (tname != NULL);
    if (file_exist (tname)) {
        CHECK (file_delete (tname));
    }

    CHECK (file_copy (tname, sname));
    CHECK (file_exist (tname));
    CHECK (file_delete (tname));

    buffer_destroy (&spath);
    buffer_destroy (&tpath);
}

TESTER(file_stream_create_1)
{
    Buffer path = BUFFER_INIT;
    Stream stream;
    const char *fname;

    CHECK (path_get_temporary_directory (&path));
    CHECK (path_append (&path, span_from_text ("stream.tmp")));

    fname = buffer_to_text (&path);
    CHECK (fname != NULL);

    if (file_exist (fname)) {
        CHECK (file_delete (fname));
    }

    CHECK (file_stream_create (&stream, fname));
    CHECK (stream_write_line (&stream, "Hello, world!"));
    CHECK (stream_close (&stream));

    CHECK (file_delete (fname));

    buffer_destroy (&path);
}

TESTER(file_stream_open_read_1)
{
    Buffer path = BUFFER_INIT;
    am_byte junk[100];
    Stream stream;
    const char *fname;

    CHECK (where_test_data (&path));
    CHECK (path_append (&path, span_from_text ("org.mnu")));

    fname = buffer_to_text (&path);
    CHECK (fname != NULL);

    CHECK (file_stream_open_read (&stream, fname));
    CHECK (stream_read (&stream, junk, sizeof (junk)) == 86);
    CHECK (stream_close (&stream));

    buffer_destroy (&path);
}

TESTER(file_stream_open_write_1)
{
    Buffer path = BUFFER_INIT;
    Stream stream;
    const char *fname;

    CHECK (path_get_temporary_directory (&path));
    CHECK (path_append (&path, span_from_text ("stream.tmp")));

    fname = buffer_to_text (&path);
    CHECK (fname != NULL);

    CHECK (file_touch (fname));

    CHECK (file_stream_open_write (&stream, fname));
    CHECK (stream_write_line (&stream, "Hello, world!"));
    CHECK (stream_close (&stream));

    CHECK (file_delete (fname));

    buffer_destroy (&path);
}

TESTER(file_texter_1)
{
    Buffer path = BUFFER_INIT;
    Stream stream;
    StreamTexter texter;
    const char *fname;

    CHECK (where_test_data (&path));
    CHECK (path_append (&path, span_from_text ("org.mnu")));

    fname = buffer_to_text (&path);
    CHECK (fname != NULL);

    CHECK (file_texter (&texter, &stream, fname));
    texter_destroy (&texter);

    buffer_destroy (&path);
}
