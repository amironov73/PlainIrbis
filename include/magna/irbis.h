// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef MAGNA_IRBIS_H
#define MAGNA_IRBIS_H

#include "magna/core.h"

//=========================================================

typedef struct
{
    char *value;
    char code;

} SubField;

//=========================================================

typedef struct
{
    char *value;
    unsigned int tag;

} Field;

//=========================================================

typedef struct
{
    unsigned int mfn;
    unsigned int status;
    unsigned int version;
    char *database;

} MarcRecord;

//=========================================================

typedef struct
{
    char *host;
    int port;
    char *username;
    char *password;
    char *database;
    int clientId;
    int queryId;
    int lastError;
    // char *serverVersion;
    int interval;

} Connection;

MAGNA_API void MAGNA_CALL connection_init (Connection *connection);
MAGNA_API void MAGNA_CALL connection_free (Connection *connection);

#endif // MAGNA_IRBIS_H
