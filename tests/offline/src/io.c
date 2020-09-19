// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/tester.h"

TESTER(magna_ntohs_1)
{
    CHECK (magna_ntohs (0x0000) == 0x0000);
    CHECK (magna_ntohs (0x0102) == 0x0201);
    CHECK (magna_ntohs (0x0201) == 0x0102);
    CHECK (magna_ntohs (0xFFFF) == 0xFFFF);
}

TESTER(magna_ntohl_1)
{
    CHECK (magna_ntohl (0x00000000) == 0x00000000);
    CHECK (magna_ntohl (0x12345678) == 0x78563412);
    CHECK (magna_ntohl (0x78563412) == 0x12345678);
    CHECK (magna_ntohl (0xFFFFFFFF) == 0xFFFFFFFF);
}

TESTER(irbis_decode_64_1)
{
    CHECK (irbis_decode_64 (0x12345678, 0x9ABCDEF0) == 0xF0DEBC9A78563412ULL);
}

TESTER(irbis_encode_64_1)
{
    am_uint32 buffer [2];

    irbis_encode_64 (buffer, 0xF0DEBC9A78563412ULL);
    CHECK (buffer[0] == 0x12345678);
    CHECK (buffer[1] == 0x9ABCDEF0);
}
