// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/irbis.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*!
 * \file dll.c
 *
 * Обёртка над оригинальной irbis64.dll.
 *
 */

#ifdef _M_IX86

#include <windows.h>
#include <assert.h>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

static HANDLE hdll = NULL;

/*
 * Загрузка irbis64.dll в память.
 * В случае неудачи аварийно завершает программу.
 */
static void load_irbis64_dll()
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
MAGNA_API void* irbis64_dll_init ()
{
    IrbisInit = get_irbis64_function
        (
            "Irbisinit", // sic!
            IrbisInit
        );

    FARPROC function = (FARPROC) IrbisInit;

    return (void*) function ();
}

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

#endif
