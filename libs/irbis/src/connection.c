// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/irbis.h"

void MAGNA_CALL connection_init (Connection *connection)
{
    memset (connection, 0, sizeof (Connection));
}

void MAGNA_CALL connection_free (Connection *connection)
{
    memset (connection, 0, sizeof (Connection));
}
