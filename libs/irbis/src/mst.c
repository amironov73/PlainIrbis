// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/direct.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 * \file mst.c
 *
    Работа с мастер-файлом напрямую.

    \class MstControlRecord64
    \brief Первая запись в файле документов - управляющая.
    \details Она формируется в момент создания базы данных
    и поддерживается автоматически. Ее содержание следующее:

    ```
    Число бит Параметр
    32        CTLMFN – резерв;
    32        NXTMFN –номер записи файла документов,
              назначаемый для следующей записи,
              создаваемой в базе данных;
    32        NXT_LOW – младшее слово смещения на свободное место
              в файле; (всегда указывает на конец файла MST)
    32        NXT_HIGH – старшее слово смещения на свободное
              место в файле
    32        MFTYPE – резерв;
    32        RECCNT – резерв;
    32        MFCXX1 – резерв;
    32        MFCXX2 – резерв;
    32        MFCXX3 – индикатор блокировки базы данных
              (0 – нет, >0 – да).
    ```

    \var MstControlRecord64::RecordSize
    \brief Размер управляющей записи.

    \var MstControlRecord64::LockFlagPosition
    \brief Позиция индикатора блокировки базы данных
    в управляющей записи.

    \var MstControlRecord64::ctlMfn
    \brief Резерв.

    \var MstControlRecord64::nextMfn
    \brief Номер записи файла документов, назначаемый
    для следующей записи, создаваемой в базе данных.

    \var MstControlRecord64::nextPosition
    \brief Смещение свободного места в файле;
    не обязательно указывает на конец файла MST,
    т. к. пара мегабайт может быть захвачена авансом,
    чтобы избежать фрагментации файла документов.

    \var MstControlRecord64::mftType
    \brief Резерв.

    \var MstControlRecord64::recCnt
    \brief Резерв.

    \var MstControlRecord64::reserv1
    \brief Резерв.

    \var irbis::MstControlRecord64::reserv2
    \brief Резерв.

    \var MstControlRecord64::locked
    \brief Индикатор блокировки базы данных в целом.

    \class MstRecordLeader64
    \brief Лидер записи.

    \var MstRecordLeader64::mfn
    \brief Номер записи в файле документов.

    \var MstRecordLeader64::length
    \brief Длина записи в байтах.
    \details В официальной документации сказано,
    что всегда четное число, но по факту это не так.

    \var MstRecordLeader64::previous
    \brief Ссылка на предыдущую версию записи.

    \var MstRecordLeader64::base
    \brief Смещение (базовый адрес) полей переменной длины
    (это общая часть лидера и справочника записи в байтах).

    \var MstRecordLeader64::nvf
    \brief Число полей в записи (т. е. число входов
    в справочнике).

    \var MstRecordLeader64::status
    \brief Статус записи (логически удаленная и т. п.).

    \var MstRecordLeader64::version
    \brief Номер версии записи.

    \class MstDictionaryEntry64
    \brief Элемент справочника MST-записи, описывающий
    поле переменной длины.

    \var MstDictionaryEntry64::tag
    \brief Метка поля.

    \var MstDictionaryEntry64::position
    \brief Смещение данных в байтах.

    \var MstDictionaryEntry64::length
    \brief Длина данных в байтах.
 *
 */

/*=========================================================*/

MAGNA_API am_bool MAGNA_CALL mst_read_control
    (
        am_handle handle,
        MstControlRecord64 *record
    )
{
    assert (handle_is_good (handle));
    assert (record != NULL);

    /* TODO implement */

    return AM_FALSE;
}

MAGNA_API am_bool MAGNA_CALL mst_read_record
    (
        am_handle handle,
        MstRecord64 *record
    )
{
    assert (handle_is_good (handle));
    assert (record != NULL);

    /* TODO: implement */

    return AM_FALSE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
