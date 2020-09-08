/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/tester.h"

TESTER(cp866_buffer_from_utf8_1)
{
    am_byte utf8text[] = {208, 163, 32, 208, 191, 208, 190, 208,
        191, 208, 176, 32, 208, 177, 209, 139, 208, 187, 208, 176,
        32, 209, 129, 208, 190, 208, 177, 208, 176, 208, 186, 208,
        176, 44, 32, 208, 190, 208, 189, 32, 208, 181, 209, 145,
        32, 208, 187, 209, 142, 208, 177, 208, 184, 208, 187};
    am_byte cp866text[] = {147, 32, 175, 174, 175, 160, 32, 161, 235,
        171, 160, 32, 225, 174, 161, 160, 170, 160, 44, 32, 174, 173,
        32, 165, 241, 32, 171, 238, 161, 168, 171};
    Buffer utf8buffer, cp866buffer, workBuffer = BUFFER_INIT;

    buffer_static (&utf8buffer, utf8text, sizeof (utf8text));
    buffer_static (&cp866buffer, cp866text, sizeof (cp866text));
    cp866_buffer_from_utf8 (&workBuffer, &utf8buffer);

    CHECK (buffer_compare (&cp866buffer, &workBuffer) == 0);
}

TESTER(cp866_buffer_from_to_1)
{
    am_byte utf8text[] = {208, 163, 32, 208, 191, 208, 190, 208,
        191, 208, 176, 32, 208, 177, 209, 139, 208, 187, 208, 176,
        32, 209, 129, 208, 190, 208, 177, 208, 176, 208, 186, 208,
        176, 44, 32, 208, 190, 208, 189, 32, 208, 181, 209, 145,
        32, 208, 187, 209, 142, 208, 177, 208, 184, 208, 187};
    am_byte cp866text[] = {147, 32, 175, 174, 175, 160, 32, 161, 235,
        171, 160, 32, 225, 174, 161, 160, 170, 160, 44, 32, 174, 173,
        32, 165, 241, 32, 171, 238, 161, 168, 171};
    Buffer utf8buffer, cp866buffer, workBuffer = BUFFER_INIT;

    buffer_static (&utf8buffer, utf8text, sizeof (utf8text));
    buffer_static (&cp866buffer, cp866text, sizeof (cp866text));
    cp866_buffer_to_utf8 (&workBuffer, &cp866buffer);

    CHECK (buffer_compare (&utf8buffer, &workBuffer) == 0);
}
