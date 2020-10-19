// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

static am_byte utf8text[] = {208, 163, 32, 208, 191, 208, 190, 208,
    191, 208, 176, 32, 208, 177, 209, 139, 208, 187, 208, 176,
    32, 209, 129, 208, 190, 208, 177, 208, 176, 208, 186, 208,
    176, 44, 32, 208, 190, 208, 189, 32, 208, 181, 209, 145,
    32, 208, 187, 209, 142, 208, 177, 208, 184, 208, 187};
static am_byte cp1251text[] = {211, 32, 239, 238, 239, 224, 32, 225,
    251, 235, 224, 32, 241, 238, 225, 224, 234, 224, 44, 32, 238,
    237, 32, 229, 184, 32, 235, 254, 225, 232, 235};

/*
TESTER(cp1251_buffer_from_utf8_1)
{
    Buffer utf8buffer, cp1251buffer, workBuffer = BUFFER_INIT;

    buffer_static (&utf8buffer, utf8text, sizeof (utf8text));
    buffer_static (&cp1251buffer, cp1251text, sizeof (cp1251text));
    cp1251_buffer_from_utf8 (&workBuffer, &utf8buffer);

    CHECK (buffer_compare (&cp1251buffer, &workBuffer) == 0);
}

TESTER(cp1251_buffer_from_to_1)
{
    Buffer utf8buffer, cp1251buffer, workBuffer = BUFFER_INIT;

    buffer_static (&utf8buffer, utf8text, sizeof (utf8text));
    buffer_static (&cp1251buffer, cp1251text, sizeof (cp1251text));
    cp1251_buffer_to_utf8 (&workBuffer, &cp1251buffer);

    CHECK (buffer_compare (&utf8buffer, &workBuffer) == 0);
}

*/
