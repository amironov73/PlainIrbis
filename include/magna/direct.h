// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef MAGNA_DIRECT_H
#define MAGNA_DIRECT_H

#include "magna/irbis.h"

/*=========================================================*/

typedef struct
{
    am_uint32 low;
    am_uint32 high;
    am_uint32 flags;

} XrfRecord;

typedef struct
{
    am_handle handle;
} XrfFile;

/* Размер записи в XRF-файле */
#define XRF_RECORD_SIZE 12u

MAGNA_API am_bool MAGNA_CALL xrf_is_deleted (const XrfRecord *record);
MAGNA_API am_bool MAGNA_CALL xrf_is_locked  (const XrfRecord *record);

/*=========================================================*/

typedef struct
{
    am_uint32 ctlMfn;
    am_uint32 nextMfn;
    am_uint64 nextPosition;
    am_uint32 mftType;
    am_uint32 recCnt;
    am_uint32 reserv1;
    am_uint32 reserv2;
    am_uint32 locked;

} MstControlRecord64;

typedef struct
{
    am_uint32 tag;
    am_uint32 position;
    am_uint64 length;

} MstDictionaryEntry64;

typedef struct
{
    am_uint32 mfn;
    am_uint32 length;
    am_uint64 previous;
    am_uint32 base;
    am_uint32 nvf;
    am_uint32 status;
    am_uint32 version;

} MstRecordLeader64;

typedef struct
{
    MstRecordLeader64 leader;
    am_uint64 offset;
    Array dictionary;

} MstRecord64;

typedef struct
{
    am_handle handle;

} MstFile64;

MAGNA_API am_bool MAGNA_CALL mst_read_control (am_handle handle, MstControlRecord64 *record);
MAGNA_API am_bool MAGNA_CALL mst_read_record  (am_handle handle, MstRecord64 *record);

/*=========================================================*/

#endif
