// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/irbis.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

/*!
 * \file dll.c
 *
 * Обёртка над оригинальной irbis64.dll.
 *
 */

#if defined(_M_IX86) && !defined(__MINGW32__) && defined(MAGNA_WINDOWS)

#include <windows.h>
#include <assert.h>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#pragma warning(disable: 4152)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

/*=========================================================*/

static HANDLE hdll = NULL;

/*
 * Загрузка irbis64.dll в память.
 * В случае неудачи аварийно завершает программу.
 */
static void load_irbis64_dll(void)
{
    if (hdll == NULL) {
        hdll = LoadLibrary ("irbis64.dll");
        if (hdll == NULL) {
            fputs ("Failed to load irbis64.dll\n", stderr);
            abort ();
        }
    }
}

/*
 * Поиск указанной функции в irbis64.dll.
 * Принимает ранее найденный адрес функции,
 * чтобы не искать заново.
 * В случае наудачи аварийно завершает программу.
 */
static void* get_irbis64_function
    (
        char *functionName,
        void *previous
    )
{
    void *result;

    assert (functionName != NULL);

    if (previous != NULL) {
        return previous;
    }

    load_irbis64_dll ();
    result = GetProcAddress (hdll, functionName);
    if (result == NULL) {
        fputs ( "Failed to get function '", stderr );
        fputs ( functionName, stderr );
        fputs ("'\n", stderr );
        abort ();
    }

    return result;
}

/*=========================================================*/

static void *IrbisDLLVersion = NULL;

/**
 * Получение версии irbis64.dll.
 * Типичный результат выглядит так: "2012.1".
 *
 * @param buffer Буфер для размещения результата.
 * @param bufSize Размер буфера в байтах.
 */
MAGNA_API void MAGNA_CALL irbis64_dll_version
    (
        char *buffer,
        int bufSize
    )
{
    assert (buffer != NULL);
    assert (bufSize > 0);

    IrbisDLLVersion = get_irbis64_function
        (
            "IrbisDLLVersion",
            IrbisDLLVersion
        );

    delphi_call_2
        (
            IrbisDLLVersion, (int) buffer,
            bufSize
        );
}

/*=========================================================*/

static void *IrbisMainIniInit = NULL;

/**
 * Инициализация библиотеки указанием INI-файла.
 *
 * @param path Путь до INI-файла.
 */
MAGNA_API void MAGNA_CALL irbis64_dll_main_ini_init
    (
        char *path
    )
{
    assert (path != NULL);

    IrbisMainIniInit = get_irbis64_function
        (
            "Irbis_MainIni_Init", // sic!
            IrbisMainIniInit
        );

    delphi_call_1
        (
            IrbisMainIniInit,
            (int) path
        );
}

/*=========================================================*/

static void *IrbisSetOptions = NULL;

/**
 * Установка опций.
 *
 * @param cacheable Форматы кешируются.
 * @param precompiled UNIFOR6 прекомпилируется.
 * @param firstBreak Останавливаться на первой ошибке.
 */
MAGNA_API void MAGNA_CALL irbis64_dll_set_options
    (
        int cacheable,
        int precompiled,
        int firstBreak
    )
{
    IrbisSetOptions = get_irbis64_function
        (
            "irbis_set_options", // sic!
            IrbisSetOptions
        );

    delphi_call_3
        (
            IrbisSetOptions,
            cacheable,
            precompiled,
            firstBreak
        );
}

/*=========================================================*/

static void *IrbisInit = NULL;

/**
 * Инициализация рабочего пространства.
 * Функция должна вызываться до остальных
 * (кроме irbis64_dll_version).
 *
 * Размещенное пространство должно быть освобождено
 * при помощи irbis64_dll_close.
 *
 * @return Указатель на размещенное пространство.
 */
MAGNA_API void* irbis64_dll_init (void)
{
    FARPROC function;

    IrbisInit = get_irbis64_function
        (
            "Irbisinit", // sic!
            IrbisInit
        );

    function = (FARPROC) IrbisInit;

    return (void*) function ();
}

/*=========================================================*/

static void *IrbisInitDepositPath = NULL;

/**
 * Задание пути до папки Deposit.
 *
 * @param path Путь до папки.
 * @return Стандартный код возврата.
 */
MAGNA_API int MAGNA_CALL irbis64_dll_init_deposit_path
    (
        char *path
    )
{
    assert (path != NULL);

    IrbisInitDepositPath = get_irbis64_function
        (
            "irbis_init_DepositPath",
            IrbisInitDepositPath
        );

    return delphi_call_1
        (
            IrbisInitDepositPath,
            (int) path
        );
}

/*=========================================================*/

static void *IrbisUatabInit = NULL;

/**
 * Инициализация таблиц символов.
 *
 * @param uctab
 * @param lctab
 * @param actab
 * @param exeDir
 * @param dataPath
 * @return
 */
MAGNA_API int MAGNA_CALL irbis64_dll_uatab_init
    (
        char *uctab,
        char *lctab,
        char *actab,
        char *exeDir,
        char *dataPath
    )
{
    IrbisUatabInit = get_irbis64_function
        (
            "IrbisUatabInit",
            IrbisUatabInit
        );

    return delphi_call_5
        (
            IrbisUatabInit,
            (int) uctab,
            (int) lctab,
            (int) actab,
            (int) exeDir,
            (int) dataPath
        );
}

/*=========================================================*/

static void *IrbisClose = NULL;

/**
 * Закрывает открытые файлы в рабочем пространстве,
 * освобождает выделенную память.
 *
 * @param space Ранее инициализированное пространство.
 */
MAGNA_API void MAGNA_CALL irbis64_dll_close
    (
        void *space
    )
{
    assert (space != NULL);

    IrbisClose = get_irbis64_function
        (
            "Irbisclose", // sic!
            IrbisClose
        );

    delphi_call_1
        (
            IrbisClose,
            (int) space
        );
}

/*=========================================================*/

static void *IrbisInitMst = NULL;

/**
 * Открывает в указанном рабочем пространстве мастер-файл
 * (MST и XRF) на чтение/запись.
 *
 * @param space Рабочее пространство.
 * @param path Путь на мастер-файл без расширения.
 * @param shelfSize Требуемое количество полок.
 * @return Стандартный код возврата.
 */
MAGNA_API int MAGNA_CALL irbis64_dll_init_mst
    (
        void *space,
        char *path,
        int shelfSize
    )
{
    assert (space != NULL);
    assert (path != NULL);
    assert (shelfSize > 0);

    IrbisInitMst = get_irbis64_function
        (
            "Irbisinitmst", // sic!
            IrbisInitMst
        );

    return delphi_call_3
        (
            IrbisInitMst,
            (int) space,
            (int) path,
            shelfSize
        );
}

/*=========================================================*/

static void *IrbisCloseMst = NULL;

/**
 * Закрывает в рабочем пространстве мастер-файл.
 *
 * @param space Рабочее пространство.
 */
MAGNA_API void MAGNA_CALL irbis64_dll_close_mst
    (
        void *space
    )
{
    assert (space != NULL);

    IrbisClose = get_irbis64_function
        (
            "Irbisclosemst", // sic!
            IrbisCloseMst
        );

    delphi_call_1
        (
            IrbisClose,
            (int) space
        );
}

/*=========================================================*/

static void *IrbisInitTerm = NULL;

/**
 * Открывает в указанном рабочем пространстве инверсный файл
 * на чтение/запись.
 *
 * @param space Рабочее пространство.
 * @param path Путь к инверсному файлу без расширения.
 * @return Стандартный код возврата.
 */
MAGNA_API int MAGNA_CALL irbis64_dll_init_term
    (
        void *space,
        char *path
    )
{
    assert (space != NULL);
    assert (path != NULL);

    IrbisInitTerm = get_irbis64_function
        (
            "Irbisinitterm", // sic!
            IrbisInitTerm
        );

    return delphi_call_2
        (
            IrbisInitTerm,
            (int) space,
            (int) path
        );
}

/*=========================================================*/

static void *IrbisCloseTerm = NULL;

/**
 * Закрывает в рабочем пространстве инверсный файл.
 *
 * @param space Рабочее пространство.
 */
MAGNA_API void MAGNA_CALL irbis64_dll_close_term
    (
        void *space
    )
{
    assert (space != NULL);

    IrbisCloseTerm = get_irbis64_function
        (
            "Irbiscloseterm", // sic!
            IrbisCloseTerm
        );

    delphi_call_1
        (
            IrbisCloseTerm,
            (int) space
        );
}

/*=========================================================*/

static void *IrbisNewRec = NULL;

MAGNA_API int  MAGNA_CALL irbis64_dll_new_record
    (
        void *space,
        int shelf
    )
{
    assert (space != NULL);

    IrbisNewRec = get_irbis64_function
        (
            "IrbisNewRec",
            IrbisNewRec
        );

    return delphi_call_2
        (
            IrbisNewRec,
            (int) space,
            shelf
        );
}

/*=========================================================*/

static void *IrbisInitNewDb;

/**
 * Создание по указанному пути пяти пустых (нулевой длины)
 * файлов базы данных: IFP, L01, MST, N01 и XRF.
 *
 * Если файлы существуют, они будут усечены до нулевой длины.
 *
 * @param path Путь без расширения (может содержать директорию).
 *
 * @return Стандартный код возврата.
 */
MAGNA_API int MAGNA_CALL irbis64_dll_init_new_db
    (
        char *path
    )
{
    assert (path != NULL);

    IrbisInitNewDb = get_irbis64_function
        (
            "IrbisinitNewDB", // sic!
            IrbisInitNewDb
        );

    return delphi_call_1
        (
            IrbisInitNewDb,
            (int) path
        );
}

/*=========================================================*/

static void *IrbisRecord = NULL;

/**
 * Чтение записи без блокировки.
 *
 * @param space Рабочее пространство.
 * @param shelf Номер полки.
 * @param mfn MFN записи.
 *
 * @return Стандартный код возврата.
 */
MAGNA_API int MAGNA_CALL irbis64_dll_record
    (
        void *space,
        int shelf,
        int mfn
    )
{
    assert (space != NULL);
    assert (shelf >= 0);
    assert (mfn > 0);

    IrbisRecord = get_irbis64_function
        (
            "IrbisRecord",
            IrbisRecord
        );

    return delphi_call_3
        (
            IrbisRecord,
            (int) space,
            shelf,
            mfn
        );
}

/*=========================================================*/

static void *IrbisReadVersion = NULL;

/**
 * Чтение версии записи (без блокировки).
 *
 * @param space Рабочее пространство.
 * @param mfn MFN записи.
 *
 * @return Стандартный код возврата.
 */
MAGNA_API int MAGNA_CALL irbis64_dll_read_version
    (
        void *space,
        int mfn
    )
{
    assert (space != NULL);
    assert (mfn > 0);

    IrbisReadVersion = get_irbis64_function
        (
            "IrbisReadVersion",
            IrbisReadVersion
        );

    return delphi_call_2
        (
            IrbisReadVersion,
            (int) space,
            mfn
        );
}

/*=========================================================*/

static void *IrbisIsDBLocked = NULL;

/**
 * Проверка, не заблокирована ли база данных.
 *
 * @param space Рабочее пространство.
 *
 * @return Стандартный код возврата.
 */
MAGNA_API int MAGNA_CALL irbis64_dll_is_db_locked
    (
        void *space
    )
{
    assert (space != NULL);

    IrbisIsDBLocked = get_irbis64_function
        (
            "IrbisIsDBLocked",
            IrbisIsDBLocked
        );

    return delphi_call_1
        (
            IrbisIsDBLocked,
            (int) space
        );
}

/*=========================================================*/

static void *IrbisMaxMfn = NULL;

/**
 * Получение максимального MFN для текущей базы данных
 *
 * @param space Рабочее пространство.
 * @return Стандартный код возврата.
 */
MAGNA_API int MAGNA_CALL irbis64_dll_max_mfn
    (
        void *space
    )
{
    assert (space != NULL);

    IrbisMaxMfn = get_irbis64_function
        (
            "Irbismaxmfn", // sic!
            IrbisMaxMfn
        );

    return delphi_call_1
        (
            IrbisMaxMfn,
            (int) space
        );
}

/*=========================================================*/

static void *IrbisRecordBack = NULL;

/**
 * Чтение предыдущей версии записи.
 *
 * @param space
 * @param shelf
 * @param mfn
 * @param step
 * @return
 */
MAGNA_API int MAGNA_CALL irbis64_dll_record_back
    (
        void *space,
        int shelf,
        int mfn,
        int step
    )
{
    assert (space != NULL);
    assert (shelf >= 0);
    assert (mfn > 0);

    IrbisRecordBack = get_irbis64_function
        (
            "IrbisRecordBack", // sic!
            IrbisRecordBack
        );

    return delphi_call_4
        (
            IrbisMaxMfn,
            (int) space,
            shelf,
            mfn,
            step
        );
}

/*=========================================================*/

static void *IrbisRecordLock0 = NULL;

/**
 * Чтение с блокировкой записи.
 *
 * @param space
 * @param shelf
 * @param mfn
 * @return
 */
MAGNA_API int MAGNA_CALL irbis64_dll_record_lock_0
    (
        void *space,
        int shelf,
        int mfn
    )
{
    assert (space != NULL);
    assert (shelf >= 0);
    assert (mfn > 0);

    IrbisRecordLock0 = get_irbis64_function
        (
            "IrbisRecLock0", // sic!
            IrbisRecordLock0
        );

    return delphi_call_3
        (
            IrbisRecordLock0,
            (int) space,
            shelf,
            mfn
        );
}

/*=========================================================*/

static void *IrbisRecordUnlock0 = NULL;

/**
 *
 * @param space
 * @param mfn
 * @return
 */
MAGNA_API int MAGNA_CALL irbis64_dll_record_unlock_0
    (
        void *space,
        int mfn
    )
{
    assert (space != NULL);
    assert (mfn > 0);

    IrbisRecordUnlock0 = get_irbis64_function
        (
            "IrbisRecUnLock0", // sic!
            IrbisRecordUnlock0
        );

    return delphi_call_2
        (
            IrbisRecordUnlock0,
            (int) space,
            mfn
        );
}

/*=========================================================*/

static void *IrbisRecordUpdate0 = NULL;

/**
 *
 * @param space
 * @param shelf
 * @param keepLock
 * @return
 */
MAGNA_API int MAGNA_CALL irbis64_dll_record_update_0
    (
        void *space,
        int shelf,
        int keepLock
    )
{
    assert (space != NULL);
    assert (shelf >= 0);

    IrbisRecordUpdate0 = get_irbis64_function
        (
            "IrbisRecUpdate0", // sic!
            IrbisRecordUpdate0
        );

    return delphi_call_3
        (
            IrbisRecordUpdate0,
            (int) space,
            shelf,
            keepLock
        );
}

/*=========================================================*/

static void *IrbisRecordIfUpdate0 = NULL;

/**
 *
 * @param space
 * @param shelf
 * @param mfn
 * @return
 */
MAGNA_API int MAGNA_CALL irbis_dll_record_if_update_0
    (
        void *space,
        int shelf,
        int mfn
    )
{
    assert (space != NULL);
    assert (shelf >= 0);
    assert (mfn > 0);

    IrbisRecordIfUpdate0 = get_irbis64_function
        (
            "IrbisRecIfUpdate0", // sic!
            IrbisRecordIfUpdate0
        );

    return delphi_call_3
        (
            IrbisRecordIfUpdate0,
            (int) space,
            shelf,
            mfn
        );
}

/*=========================================================*/

static void *IrbisDbEmptyTime = NULL;

/**
 *
 * @param space
 * @param seconds
 * @return
 */
MAGNA_API int MAGNA_CALL irbis_dll_db_empty_time
    (
        void *space,
        int seconds
    )
{
    assert (space != NULL);
    assert (seconds > 0);

    IrbisDbEmptyTime = get_irbis64_function
        (
            "IrbisDBEmptyTime", // sic!
            IrbisDbEmptyTime
        );

    return delphi_call_2
        (
            IrbisDbEmptyTime,
            (int) space,
            seconds
        );
}

/*=========================================================*/

static void *IrbisLockDbTime = NULL;

/*=========================================================*/

static void *IrbisUnlockDbTime = NULL;

/*=========================================================*/

static void *IrbisRecordUpdateTime = NULL;

/*=========================================================*/

static void *IrbisRecordIfUpdateFullTextTime = NULL;

/*=========================================================*/

static void *IrbisSetFullTextActualizedBitTime = NULL;

/*=========================================================*/

static void *IrbisRecordLockTime = NULL;

MAGNA_API int MAGNA_CALL irbis_dll_record_lock_time
    (
        void *space,
        int shelf,
        int mfn,
        int seconds
    )
{
    assert (space != NULL);
    assert (shelf >= 0);
    assert (mfn > 0);
    assert (seconds > 0);

    IrbisRecordLockTime = get_irbis64_function
        (
            "IrbisRecLockTime", // sic!
            IrbisRecordLockTime
        );

    return delphi_call_4
        (
            IrbisRecordLockTime,
            (int) space,
            shelf,
            mfn,
            seconds
        );
}

/*=========================================================*/

static void *IrbisRecordUnlockTime = NULL;

MAGNA_API int MAGNA_CALL irbis_dll_record_unlock_time
    (
        void *space,
        int mfn,
        int seconds
    )
{
    assert (space != NULL);
    assert (mfn > 0);
    assert (seconds > 0);

    IrbisRecordUnlockTime = get_irbis64_function
        (
            "IrbisRecUnLockTime", // sic!
            IrbisRecordUnlockTime
        );

    return delphi_call_3
        (
            IrbisRecordUnlockTime,
            (int) space,
            mfn,
            seconds
        );
}

/*==========================================================*/

static void *IrbisNotActList = NULL;

/*==========================================================*/

static void *IrbisIsReallyLocked = NULL;

/**
 * Без чтения, только проверка флага в XRF.
 *
 * @param space
 * @param mfn
 * @return
 */
MAGNA_API int MAGNA_CALL irbis_dll_is_really_locked
    (
        void *space,
        int mfn
    )
{
    assert (space != NULL);
    assert (mfn > 0);

    IrbisIsReallyLocked = get_irbis64_function
        (
            "IrbisIsRealyLocked", // sic!
            IrbisIsReallyLocked
        );

    return delphi_call_2
        (
            IrbisIsReallyLocked,
            (int) space,
            mfn
        );
}

/*=========================================================*/

static void *IrbisIsReallyActualized = NULL;

/**
 * Без чтения, только проверка флага в XRF
 *
 * @param space
 * @param mfn
 * @return
 */
MAGNA_API int MAGNA_CALL irbis_dll_is_really_actualized
    (
        void *space,
        int mfn
    )
{
    assert (space != NULL);
    assert (mfn > 0);

    IrbisIsReallyActualized = get_irbis64_function
        (
            "IrbisIsRealyActualized", // sic!
            IrbisIsReallyActualized
        );

    return delphi_call_2
        (
            IrbisIsReallyActualized,
            (int) space,
            mfn
        );
}

/*=========================================================*/

static void *IrbisIsLocked = NULL;

/**
 * Запись на полке заблокирована?
 *
 * @param space
 * @param shelf
 * @return
 */
MAGNA_API int MAGNA_CALL irbis_dll_is_locked
    (
        void *space,
        int shelf
    )
{
    assert (space != NULL);
    assert (shelf >= 0);

    IrbisIsLocked = get_irbis64_function
        (
            "IrbisIsLocked", // sic!
            IrbisIsLocked
        );

    return delphi_call_2
        (
            IrbisIsLocked,
            (int) space,
            shelf
        );
}

/*=========================================================*/

static void *IrbisIsDeleted = NULL;

/**
 * Запись на полке удалена?
 *
 * @param space
 * @param shelf
 * @return
 */
MAGNA_API int MAGNA_CALL irbis_dll_is_deleted
    (
        void *space,
        int shelf
    )
{
    assert (space != NULL);
    assert (shelf >= 0);

    IrbisIsDeleted = get_irbis64_function
        (
            "IrbisIsDeleted", // sic!
            IrbisIsDeleted
        );

    return delphi_call_2
        (
            IrbisIsDeleted,
            (int) space,
            shelf
        );
}

/*=========================================================*/

static void *IrbisIsActualized = NULL;

/**
 * Запись на полке актуализирована?
 *
 * @param space
 * @param shelf
 * @return
 */
MAGNA_API int MAGNA_CALL irbis_dll_is_actualized
    (
        void *space,
        int shelf
    )
{
    assert (space != NULL);
    assert (shelf >= 0);

    IrbisIsActualized = get_irbis64_function
        (
            "IrbisIsActualized", // sic!
            IrbisIsActualized
        );

    return delphi_call_2
        (
            IrbisIsActualized,
            (int) space,
            shelf
        );
}

/*=========================================================*/

static void *IrbisRecordDelete = NULL;

/**
 * Удаление записи на полке.
 *
 * @param space
 * @param shelf
 * @return
 */
MAGNA_API int MAGNA_CALL irbis_dll_record_delete
    (
        void *space,
        int shelf
    )
{
    assert (space != NULL);
    assert (shelf >= 0);

    IrbisRecordDelete = get_irbis64_function
        (
            "Irbisrecdel", // sic!
            IrbisRecordDelete
        );

    return delphi_call_2
        (
            IrbisRecordDelete,
            (int) space,
            shelf
        );
}

/*=========================================================*/

static void *IrbisRecordUndelete = NULL;

/**
 * Восстановление записи на полке.
 *
 * @param space
 * @param shelf
 * @return
 */
MAGNA_API int MAGNA_CALL irbis_dll_record_undelete
    (
        void *space,
        int shelf
    )
{
    assert (space != NULL);
    assert (shelf >= 0);

    IrbisRecordUndelete = get_irbis64_function
        (
            "Irbisrecundelete", // sic!
            IrbisRecordUndelete
        );

    return delphi_call_2
        (
            IrbisRecordUndelete,
            (int) space,
            shelf
        );
}

/*=========================================================*/

#endif

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
