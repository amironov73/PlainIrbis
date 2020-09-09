/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/tester.h"

am_byte utf8text[] = {208, 163, 32, 208, 191, 208, 190, 208, 191,
    208, 176, 32, 208, 177, 209, 139, 208, 187, 208, 176, 32, 209,
    129, 208, 190, 208, 177, 208, 176, 208, 186, 208, 176, 44, 32,
    208, 190, 208, 189, 32, 208, 181, 209, 145, 32, 208, 187, 209,
    142, 208, 177, 208, 184, 208, 187};
am_byte koi8rtext[] = {245, 32, 208, 207, 208, 193, 32, 194, 217,
    204, 193, 32, 211, 207, 194, 193, 203, 193, 44, 32, 207, 206,
    32, 197, 163, 32, 204, 192, 194, 201, 204};

TESTER(koi8r_buffer_from_utf8_1)
{
    Buffer utf8buffer, koi8rbuffer, workBuffer = BUFFER_INIT;

    buffer_static (&utf8buffer, utf8text, sizeof (utf8text));
    buffer_static (&koi8rbuffer, koi8rtext, sizeof (koi8rtext));
    koi8r_buffer_from_utf8 (&workBuffer, &utf8buffer);

    CHECK (buffer_compare (&koi8rbuffer, &workBuffer) == 0);
}

TESTER(koi8r_buffer_from_to_1)
{
    Buffer utf8buffer, koi8rbuffer, workBuffer = BUFFER_INIT;

    buffer_static (&utf8buffer, utf8text, sizeof (utf8text));
    buffer_static (&koi8rbuffer, koi8rtext, sizeof (koi8rtext));
    koi8r_buffer_to_utf8 (&workBuffer, &koi8rbuffer);

    CHECK (buffer_compare (&utf8buffer, &workBuffer) == 0);
}
