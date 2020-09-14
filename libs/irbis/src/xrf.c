/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
   \file xrf.c

    Из официальной документации: http://sntnarciss.ru/irbis/spravka/wtcp006002000.htm

    ```
    Каждая ссылка состоит из 3-х полей:
    Число бит Параметр
    32        XRF_LOW – младшее слово в 8 байтовом смещении на запись;
    32        XRF_HIGH– старшее слово в 8 байтовом смещении на запись;
    32        XRF_FLAGS – Индикатор записи в виде битовых флагов
              следующего содержания:
                BIT_LOG_DEL(1)  - логически удаленная запись;
                BIT_PHYS_DEL(2) - физически удаленная запись;
                BIT_ABSENT(4)  - несуществующая запись;
                BIT_NOTACT_REC(8)- неактуализированная запись;
                BIT_LOCK_REC(64)- заблокированная запись.
    ```

    Файл перекрестных ссылок XRF представляет собой
    таблицу ссылок на записи файла документов.

    Нулевая ссылка соответствует записи файла документов
    с номером 1, первая – 2  и т. д.

 */

/*=========================================================*/

/* Размер записи в XRF-файле */
#define XRF_RECORD_SIZE 12

typedef struct IrbisXrfFile   XrfFile;
typedef struct IrbisXrfRecord XrfRecord;

struct IrbisXrfRecord
{
    am_uint32 low;
    am_uint32 high;
    am_uint32 flags;
};

struct IrbisXrfFile
{
    am_handle handle;
};

MAGNA_API am_bool MAGNA_CALL xrf_is_deleted
    (
        const XrfRecord *record
    )
{
    return (record->flags & (LOGICALLY_DELETED | PHYSICALLY_DELETED)) != 0;
}

MAGNA_API am_bool MAGNA_CALL xrf_is_locked
    (
        const XrfRecord *record
    )
{
    return (record->flags & LOCKED_RECORD) != 0;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
