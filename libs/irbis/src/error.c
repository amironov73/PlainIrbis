// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/irbis.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

/**
 * Получение описания по коду ошибки, возвращенному сервером.
 *
 * @param code Код ошибки.
 * @return Словесное описание ошибки.
 */
MAGNA_API const char* MAGNA_CALL irbis_describe_error
    (
        am_int32 code
    )
{
    if (code >= 0) {
        return "Нет ошибки";
    }

    switch (code)
    {
        case -100:
            return "Заданный MFN вне пределов БД";

        case -101:
            return "Ошибочный размер полки";

        case -102:
            return "Ошибочный номер полки";

        case -140:
            return "MFN вне пределов БД";

        case -141:
            return "Ошибка чтения";

        case -200:
            return "Указанное поле отсутствует";

        case -201:
            return "Предыдущая версия записи отсутствует";

        case -202:
            return "Заданный термин не найден (термин не существует)";

        case -203:
            return "Последний термин в списке";

        case -204:
            return "Первый термин в списке";

        case -300:
        case -301:
            return "База данных монопольно заблокирована";

        case -400:
            return "Ошибка при открытии файлов MST или XRF (ошибка файла данных)";

        case -401:
            return "Ошибка при открытии файлов IFP (ошибка файла индекса)";

        case -402:
            return "Ошибка при записи";

        case -403:
            return "Ошибка при актуализации";

        case -600:
            return "Запись логически удалена";

        case -601:
            return "Запись физически удалена";

        case -602:
            return "Запись заблокирована на ввод";

        case -603:
            return "Запись логически удалена";

        case -605:
            return "Запись физически удалена";

        case -607:
            return "Ошибка autoin.gbl";

        case -608:
            return "Ошибка версии записи";

        case -700:
            return "Ошибка создания резервной копии";

        case -701:
            return "Ошибка восстановления из резервной копии";

        case -702:
            return "Ошибка сортировки";

        case -703:
            return "Ошибочный термин";

        case -704:
            return "Ошибка создания словаря";

        case -705:
            return "Ошибка загрузки словаря";

        case -800:
            return "Ошибка в параметрах глобальной корректировки";

        case -801:
            return "ERR_GBL_REP";

        case -802:
            return "ERR_GBL_MET";

        case -1111:
            return "Ошибка исполнения сервера (SERVER_EXECUTE_ERROR)";

        case -2222:
            return "Ошибка в протоколе (WRONG_PROTOCOL)";

        case -3333:
            return "Незарегистрированный клиент (ошибка входа на сервер) (клиент не в списке)";

        case -3334:
            return "Клиент не выполнил вход на сервер (клиент не используется)";

        case -3335:
            return "Неправильный уникальный идентификатор клиента";

        case -3336:
            return "Нет доступа к командам АРМ";

        case -3337:
            return "Клиент уже зарегистрирован";

        case -3338:
            return "Недопустимый клиент";

        case -4444:
            return "Неверный пароль";

        case -5555:
            return "Файл не существует";

        case -6666:
            return "Сервер перегружен. Достигнуто максимальное число потоков обработки";

        case -7777:
            return "Не удалось запустить/прервать поток администратора (ошибка процесса)";

        case -8888:
            return "Общая ошибка";

        case -100001:
            return "Ошибка создания сокета";

        case -100002:
            return "Сбой сети";

        case -100003:
            return "Не подключен к серверу";

        default: return "Неизвестная ошибка";
    }
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
