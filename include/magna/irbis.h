// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef MAGNA_IRBIS_H
#define MAGNA_IRBIS_H

#include "magna/core.h"

//=========================================================

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4820)
#endif

//=========================================================

typedef struct _SubField
{
    char *value;
    char code;

} SubField;

//=========================================================

typedef struct _MarcField
{
    char *value;
    unsigned int tag;

} MarcField;

//=========================================================

typedef struct _MarcRecord
{
    unsigned int mfn;
    unsigned int status;
    unsigned int version;
    char *database;

} MarcRecord;

//=========================================================

typedef struct _Connection
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

//=========================================================

#ifdef _M_IX86

MAGNA_API void  MAGNA_CALL irbis64_dll_version (char *buffer, int bufSize);
MAGNA_API void  MAGNA_CALL irbis64_dll_main_ini_init (char *path);
MAGNA_API void* irbis64_dll_init ();
MAGNA_API void  MAGNA_CALL irbis64_dll_set_options (int cacheable,
                                                    int precompiled,
                                                    int firstBreak);
MAGNA_API int   MAGNA_CALL irbis64_dll_init_deposit_path (char *path);
MAGNA_API int   MAGNA_CALL irbis64_dll_uatab_init (char *uctab,
                                                   char *lctab,
                                                   char *actab,
                                                   char *exeDir,
                                                   char *dataPath);
MAGNA_API void  MAGNA_CALL irbis64_dll_close (void *space);
MAGNA_API int   MAGNA_CALL irbis64_dll_init_mst (void *space,
                                                 char *path,
                                                 int shelfSize);
MAGNA_API void  MAGNA_CALL irbis64_dll_close_mst (void *space);
MAGNA_API int   MAGNA_CALL irbis64_dll_init_term (void *space,
                                                  char *path);
MAGNA_API void  MAGNA_CALL irbis64_dll_close_term (void *space);
MAGNA_API int   MAGNA_CALL irbis64_dll_new_record (void *space, int shelf);
MAGNA_API int   MAGNA_CALL irbis64_dll_init_new_db (char *path);
MAGNA_API int   MAGNA_CALL irbis64_dll_is_db_locked (void *space);
MAGNA_API int   MAGNA_CALL irbis64_dll_record (void *space,
                                               int shelf,
                                               int mfn);
MAGNA_API int   MAGNA_CALL irbis64_dll_read_version (void *space,
                                                     int mfn);
MAGNA_API int   MAGNA_CALL irbis64_dll_max_mfn (void *space);

#endif

//=========================================================

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // MAGNA_IRBIS_H
