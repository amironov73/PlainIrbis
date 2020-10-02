// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef MAGNA_IRBIS_H
#define MAGNA_IRBIS_H

#include "magna/core.h"

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4820)
#endif

/*=========================================================*/

/* Статус записи */

#define LOGICALLY_DELETED        1u   /* Запись логически удалена       */
#define PHYSICALLY_DELETED       2u   /* Запись физически удалена       */
#define ABSENT                   4u   /* Запись отсутствует             */
#define NON_ACTUALIZED           8u   /* Запись не актуализирована      */
#define NEW_RECORD               16u  /* Свежесозданная запись          */
#define LAST_VERSION             32u  /* Последняя версия записи        */
#define LOCKED_RECORD            64u  /* Запись заблокирована на ввод   */
#define AUTOIN_ERROR             128u /* Ошибка AUTOIN.GBL              */
#define FULL_TEXT_NOT_ACTUALIZED 256u /* Полный текст не актуализирован */

/* Коды АРМ */

#define ADMINISTRATOR 'A' /* Адмнистратор        */
#define CATALOGER     'C' /* Каталогизатор       */
#define ACQUSITIONS   'M' /* Комплектатор        */
#define READER        'R' /* Читатель            */
#define CIRCULATION   'B' /* Книговыдача         */
#define BOOKLAND      'B' /* Книговыдача         */
#define PROVISION     'K' /* Книгообеспеченность */
#define JAVA_APPLET   'J' /* Апплет JAVA         */

/*=========================================================*/

/* Коды команд */

/* Получение признака монопольной блокировки базы данных */
#define EXCLUSIVE_DATABASE_LOCK "#"

/* Получение списка удаленных, неактуализированных
   и заблокированных записей */
#define RECORD_LIST "0"

/* Получение версии сервера */
#define SERVER_INFO "1"

/* Получение статистики по базе данных */
#define DATABASE_STAT "2"

/* Глобальная корректировка */
#define GLOBAL_CORRECTION "5"

/* Сохранение группы записей */
#define SAVE_RECORD_GROUP "6"

/* Печать */
#define PRINT "7"

/* Запись параметров в ini - файл, расположенный на сервере */
#define UPDATE_INI_FILE "8"

#define IMPORT_ISO "9"

/* Регистрация клиента на сервере */
#define REGISTER_CLIENT "A"

/* Разрегистрация клиента */
#define UNREGISTER_CLIENT "B"

/* Чтение записи, ее расформатирование */
#define READ_RECORD "C"

/* Сохранение записи */
#define UPDATE_RECORD "D"

/* Разблокировка записи */
#define UNLOCK_RECORD "E"

/* Актуализация записи */
#define ACTUALIZE_RECORD "F"

/* Форматирование записи или группы записей */
#define FORMAT_RECORD "G"

/* Получение терминов и ссылок словаря,
   форматирование записей */
#define READ_TERMS "H"

/* Получение ссылок для термина (списка терминов) */
#define READ_POSTINGS "I"

/* Глобальная корректировка виртуальной записи */
#define CORRECT_VIRTUAL_RECORD "J"

/* Поиск записей с опциональным форматированием */
#define SEARCH "K"

/* Получение / сохранение текстового файла,
   расположенного на сервере (группы текстовых файлов) */
#define READ_DOCUMENT "L"

#define BACKUP "M"

/* Пустая операция. Периодическое подтверждение соединения с сервером */
#define NOP "N"

/* Получение максимального MFN для базы данных */
#define GET_MAX_MFN "O"

/* Получение терминов и ссылок словаря в обратном порядке */
#define READ_TERMS_REVERSE "P"

/* Разблокирование записей */
#define UNLOCK_RECORDS "Q"

/* Полнотекстовый поиск */
#define FULL_TEXT_SEARCH "R"

/* Опустошение базы данных */
#define EMPTY_DATABASE "S"

/* Создание базы данных */
#define CREATE_DATABASE "T"

/* Разблокирование базы данных */
#define UNLOCK_DATABASE "U"

/* Чтение ссылок для заданного MFN */
#define GET_RECORD_POSTINGS "V"

/* Удаление базы данных */
#define DELETE_DATABASE "W"

/* Реорганизация мастер - файла */
#define RELOAD_MASTER_FILE "X"

/* Реорганизация словаря */
#define RELOAD_DICTIONARY "Y"

/* Создание поискового словаря заново */
#define CREATE_DICTIONARY "Z"

/* Получение статистики работы сервера */
#define GET_SERVER_STAT "+1"

/* Получение списка запущенных процессов */
#define GET_PROCESS_LIST "+3"

/* Сохранение списка пользователей */
#define SET_USER_LIST "+7"

/* Перезапуск сервера */
#define RESTART_SERVER "+8"

/* Получение списка пользователей */
#define GET_USER_LIST "+9"

/* Получение списка файлов на сервере */
#define LIST_FILES "!"

/*=========================================================*/

/* Разделитель строк в ИРБИС. */

#define IRBIS_DELIMITER "\x1F\x1E"

/* Короткие версии разделителя строк ИРБИС. */

#define SHORT_DELIMITER "\x1E"
#define ALT_DELIMITER   "\x1F"

/* Разделитель строк в MS-DOS */

#define MSDOS_DELIMITER "\r\n"

/* Признак окончания меню */

#define STOP_MARKER "*****"

/*=========================================================*/

/* Подполе записи */

typedef struct
{
    Buffer value;
    char code;

} SubField;

#define SUBFIELD_INIT { BUFFER_INIT, 0 };

MAGNA_API SubField* MAGNA_CALL subfield_clone     (SubField *target, const SubField *source);
MAGNA_API SubField* MAGNA_CALL subfield_decode    (SubField *subfield, Span span);
MAGNA_API am_bool   MAGNA_CALL subfield_empty     (const SubField *subfield);
MAGNA_API void      MAGNA_CALL subfield_free      (SubField *subfield);
MAGNA_API SubField* MAGNA_CALL subfield_init      (SubField *subfield, char code, Span value);
MAGNA_API Buffer*   MAGNA_CALL subfield_to_string (const SubField *subfield, Buffer *buffer);
MAGNA_API am_bool   MAGNA_CALL subfield_verify    (const SubField *subfield);

/*=========================================================*/

/* Поле записи */

typedef struct
{
    Buffer value;
    Vector subfields;
    am_uint32 tag;

} MarcField;

MAGNA_API SubField*  MAGNA_CALL field_add                      (MarcField *field, char code, Span value);
MAGNA_API MarcField* MAGNA_CALL field_clear                    (MarcField *field);
MAGNA_API MarcField* MAGNA_CALL field_clone                    (MarcField *target, const MarcField *source);
MAGNA_API SubField*  MAGNA_CALL subfield_create                (SubField *subfield, char code, Span value);
MAGNA_API MarcField* MAGNA_CALL field_decode                   (MarcField *field, Span span);
MAGNA_API am_bool    MAGNA_CALL field_empty                    (const MarcField *field);
MAGNA_API Vector*     MAGNA_CALL field_get_embedded_fields      (const MarcField *field, Vector *array);
MAGNA_API SubField*  MAGNA_CALL field_get_first_subfield       (const MarcField *field, char code);
MAGNA_API Span       MAGNA_CALL field_get_first_subfield_value (const MarcField *field, char code);
MAGNA_API MarcField* MAGNA_CALL field_init                     (MarcField *field, am_uint32 tag);
MAGNA_API MarcField* MAGNA_CALL field_insert_at                (MarcField *field, size_t index, const SubField *subfield);
MAGNA_API MarcField* MAGNA_CALL field_remove_at                (MarcField *field, size_t index);
MAGNA_API MarcField* MAGNA_CALL field_remove_subfield          (MarcField *field, char code);
MAGNA_API Buffer*    MAGNA_CALL field_to_string                (const MarcField *field, Buffer *buffer);
MAGNA_API am_bool    MAGNA_CALL field_verify                   (const MarcField *field);

/*=========================================================*/

/* Раскодированная запись */

typedef struct
{
    Vector fields;
    Buffer database;
    void *data;
    am_mfn mfn;
    am_flag status;
    am_int32 version;

} MarcRecord;

MAGNA_API MarcField*  MAGNA_CALL record_add          (MarcRecord *record, am_uint32 tag, const char *value);
MAGNA_API MarcRecord* MAGNA_CALL record_clone        (MarcRecord *target, const MarcRecord *source);
MAGNA_API am_bool     MAGNA_CALL record_decode_lines (MarcRecord *record, Vector *lines);
MAGNA_API am_bool     MAGNA_CALL record_encode       (const MarcRecord *record, const char *delimiter, Buffer *buffer);
MAGNA_API Span        MAGNA_CALL record_fm           (const MarcRecord *record, am_uint32 tag, char code);
MAGNA_API am_bool     MAGNA_CALL record_fma          (const MarcRecord *record, Vector *array, am_uint32 tag, char code);
MAGNA_API MarcField*  MAGNA_CALL record_get_field    (const MarcRecord *record, am_uint32 tag, size_t occurrence);

/*=========================================================*/

/* Спецификация файла на сервере */

typedef struct
{
    int path;
    Buffer database;
    Buffer filename;
    Buffer content;
    am_bool binary;

} Specification;

MAGNA_API Specification* MAGNA_CALL spec_init      (Specification *spec, int path, const char *database, const char *filename);
MAGNA_API am_bool        MAGNA_CALL spec_parse     (Specification *spec, Buffer *buffer);
MAGNA_API Buffer*        MAGNA_CALL spec_to_string (const Specification *spec, Buffer *buffer);
MAGNA_API am_bool        MAGNA_CALL spec_verify    (const Specification *spec);

/*=========================================================*/

/* Информация о версии сервера */

typedef struct {
    Buffer organization;
    Buffer version;
    am_uint32 maxClients;
    am_uint32 connected;

} ServerVersion;

/*=========================================================*/

/* Опережающее объявление */

struct IrbisConnection;
typedef struct IrbisConnection Connection;

/*=========================================================*/

/* Клиентский запрос */

typedef struct
{
    Buffer buffer;
} Query;

MAGNA_API am_bool MAGNA_CALL query_add_ansi        (Query *query, const char *text);
MAGNA_API am_bool MAGNA_CALL query_add_ansi_buffer (Query *query, const Buffer *text);
MAGNA_API am_bool MAGNA_CALL query_add_format      (Query *query, const char *text);
MAGNA_API am_bool MAGNA_CALL query_add_int_32      (Query *query, am_int32 value);
MAGNA_API am_bool MAGNA_CALL query_add_utf         (Query *query, const char *text);
MAGNA_API am_bool MAGNA_CALL query_add_utf_buffer  (Query *query, const Buffer *text);
MAGNA_API am_bool MAGNA_CALL query_create          (Connection *connection, Query *query, const char *command);
MAGNA_API am_bool MAGNA_CALL query_encode          (const Query *query, Buffer *prefix);
MAGNA_API void    MAGNA_CALL query_free            (Query *query);
MAGNA_API am_bool MAGNA_CALL query_new_line        (Query *query);

/*=========================================================*/

/* Ответ сервера */

typedef struct
{
    Buffer answer;
    Span command;
    am_int32 clientId;
    am_int32 queryId;
    am_int32 returnCode;
    am_int32 answerSize;
    Span serverVersion;
    struct IrbisConnection *connection;

} Response;

MAGNA_API am_bool  MAGNA_CALL response_create                (struct IrbisConnection *connection, Response *response);
MAGNA_API am_bool             response_check                 (Response *response, ...);
MAGNA_API void     MAGNA_CALL response_free                  (Response *response);
MAGNA_API Span     MAGNA_CALL response_get_line              (Response *response);
MAGNA_API am_int32 MAGNA_CALL response_get_return_code       (Response *response);
MAGNA_API Span     MAGNA_CALL response_read_ansi             (Response *response);
MAGNA_API am_int32 MAGNA_CALL response_read_int32            (Response *response);
MAGNA_API Span     MAGNA_CALL response_read_utf              (Response *response);
MAGNA_API am_bool  MAGNA_CALL response_remaining_ansi_lines  (Response *response, Vector *array);
MAGNA_API Span     MAGNA_CALL response_remaining_ansi_text   (Response *response);
MAGNA_API am_bool  MAGNA_CALL response_remaining_utf_lines   (Response *response, Vector *array);
MAGNA_API Span     MAGNA_CALL response_remaining_utf_text    (Response *response);

/*=========================================================*/

/* Меню */

/* Пара строк в MNU-файле. */
typedef struct
{
    Buffer code;
    Buffer comment;
} MenuEntry;

MAGNA_API void    MAGNA_CALL menu_entry_init      (MenuEntry *entry);
MAGNA_API void    MAGNA_CALL menu_entry_free      (MenuEntry *entry);
MAGNA_API am_bool MAGNA_CALL menu_entry_to_string (const MenuEntry *entry, Buffer *output);

/* MNU-файл. состоит из пар строк (см. MenuEntry). */
typedef struct
{
    Vector entries;
} MenuFile;

MAGNA_API am_bool          MAGNA_CALL menu_init        (MenuFile *menu);
MAGNA_API void             MAGNA_CALL menu_free        (MenuFile *menu);
MAGNA_API am_bool          MAGNA_CALL menu_append      (MenuFile *menu, Span code, Span comment);
MAGNA_API const MenuEntry* MAGNA_CALL menu_get_entry   (const MenuFile *menu, Span code);
MAGNA_API Span             MAGNA_CALL menu_get_comment (const MenuFile *menu, Span code, Span defaultValue);
MAGNA_API am_bool          MAGNA_CALL menu_parse       (MenuFile *menu, StreamTexter *texter);
MAGNA_API am_bool          MAGNA_CALL menu_to_stream   (const MenuFile *menu, Stream *stream);

/*=========================================================*/

/* INI-файл */

/* Строка INI-файла. Состоит из ключа и (опционально) значения. c*/
typedef struct
{
    Buffer key;
    Buffer value;
    am_bool modified;

} IniLine;

MAGNA_API void    MAGNA_CALL ini_line_free      (IniLine *line);
MAGNA_API void    MAGNA_CALL ini_line_init      (IniLine *line);
MAGNA_API am_bool MAGNA_CALL ini_line_set_key   (IniLine *line, Span key);
MAGNA_API am_bool MAGNA_CALL ini_line_set_value (IniLine *line, Span value);
MAGNA_API am_bool MAGNA_CALL ini_line_to_string (const IniLine *line, Buffer *output);
MAGNA_API am_bool MAGNA_CALL ini_line_verify    (const IniLine *line);

/* Секция INI-файла. Состоит из строк (см. IniLine). Имеет опциональное имя. */
typedef struct
{
    Buffer name;
    Vector lines;

} IniSection;

MAGNA_API void           MAGNA_CALL ini_section_clear        (IniSection *section);
MAGNA_API void           MAGNA_CALL ini_section_free         (IniSection *section);
MAGNA_API const IniLine* MAGNA_CALL ini_section_get_line     (const IniSection *section, Span key);
MAGNA_API Span           MAGNA_CALL ini_section_get_value    (const IniSection *section, Span key, Span defaultValue);
MAGNA_API am_bool        MAGNA_CALL ini_section_init         (IniSection *section);
MAGNA_API am_bool        MAGNA_CALL ini_section_is_modified  (const IniSection *section);
MAGNA_API void           MAGNA_CALL ini_section_not_modified (IniSection *section);

/* INI-файл. Состоит из секций. */
typedef struct
{
    Vector sections;
} IniFile;

MAGNA_API void    MAGNA_CALL ini_file_free         (IniFile *file);
MAGNA_API am_bool MAGNA_CALL ini_file_init         (IniFile *file);
MAGNA_API am_bool MAGNA_CALL ini_file_is_modified  (const IniFile *file);
MAGNA_API void    MAGNA_CALL ini_file_not_modified (IniFile *file);

/*=========================================================*/

/* Поиск */

/* Параметры для поиска записей */
typedef struct
{
    Buffer expression;
    Buffer database;
    Buffer format;
    Buffer sequential;
    Buffer filter;
    am_mfn firstRecord;
    am_mfn minMfn;
    am_mfn maxMfn;
    am_mfn number;

} SearchParameters;

MAGNA_API am_bool MAGNA_CALL search_init (SearchParameters *parameters);
MAGNA_API void    MAGNA_CALL search_free (SearchParameters *parameters);


/* Сценарий поиска */
typedef struct
{
    Buffer name;
    Buffer prefix;
    Buffer menuName;
    Buffer oldFormat;
    Buffer correction;
    Buffer truncation;
    Buffer hint;
    Buffer modByDicAuto;
    Buffer logic;
    Buffer advance;
    Buffer format;
    am_int32 dictionaryType;

} SearchScenario;

MAGNA_API am_bool MAGNA_CALL scenario_init (SearchScenario *scenario);
MAGNA_API void    MAGNA_CALL scenario_free (SearchScenario *scenario);

/*=========================================================*/

/* Глобальная корректировка */

/* Параметр глобальной корректировки */
typedef struct
{
    Buffer title;
    Buffer value;

} GblParameter;

/* Оператор глобальной корректировки */
typedef struct
{
    Buffer command;
    Buffer parameter1;
    Buffer parameter2;
    Buffer format1;
    Buffer format2;

} GblStatement;

/* Настройки для глобальной корректировки */
typedef struct
{
    Buffer database;
    Buffer fileName;
    Int32Array mfnList;
    Buffer searchExpression;
    Buffer sequentialSearch;
    Vector statements;
    Vector parameters;
    am_bool actualize;
    am_bool autoin;
    am_mfn firstRecord;
    am_mfn maxMfn;
    am_mfn minMfn;
    am_mfn numberOfRecords;

} GblSettings;

/* Результат глобальной корректировки */
typedef struct
{
    /* TODO: implement */
    int unused;
} GblResult;

/*=========================================================*/

/* Подключение к серверу */

struct IrbisConnection
{
    Buffer host;
    Buffer username;
    Buffer password;
    Buffer database;
    Buffer serverVersion;
    am_int32 clientId;
    am_int32 queryId;
    am_int32 lastError;
    am_int32 interval;
    am_bool connected;
    am_int16 port;
    char workstation;

};

#define CONNECTION_SET_HOST(__c,__h)     buffer_from_text (&(__c).host,     __h)
#define CONNECTION_SET_USERNAME(__c,__h) buffer_from_text (&(__c).username, __h)
#define CONNECTION_SET_PASSWORD(__c,__h) buffer_from_text (&(__c).password, __h)
#define CONNECTION_SET_DATABASE(__c,__h) buffer_from_text (&(__c).database, __h)

MAGNA_API am_bool MAGNA_CALL connection_actualize_database (Connection *connection, const char *database);
MAGNA_API am_bool MAGNA_CALL connection_actualize_record   (Connection *connection, const char *database, am_mfn mfn);
MAGNA_API am_bool MAGNA_CALL connection_check              (Connection *connection);
MAGNA_API am_bool MAGNA_CALL connection_connect            (Connection *connection);
MAGNA_API am_bool MAGNA_CALL connection_create_database    (Connection *connection, const char *database, const char *description, am_bool readerAccess);
MAGNA_API am_bool MAGNA_CALL connection_create_dictionary  (Connection *connection, const char *database);
MAGNA_API am_bool MAGNA_CALL connection_delete_database    (Connection *connection, const char *database);
MAGNA_API am_bool MAGNA_CALL connection_delete_file        (Connection *connection, const char *fileName);
MAGNA_API am_bool MAGNA_CALL connection_delete_record      (Connection *connection, am_mfn mfn);
MAGNA_API am_bool MAGNA_CALL connection_disconnect         (Connection *connection);
MAGNA_API am_bool MAGNA_CALL connection_execute            (Connection *connection, Query *query, Response *response);
MAGNA_API am_bool            connection_execute_simple     (Connection *connection, Response *response, const char *command, int argCount, ...);
MAGNA_API void    MAGNA_CALL connection_destroy            (Connection *connection);
MAGNA_API am_mfn  MAGNA_CALL connection_get_max_mfn        (Connection *connection, const char *database);
MAGNA_API am_bool MAGNA_CALL connection_create               (Connection *connection);
MAGNA_API am_bool MAGNA_CALL connection_no_operation       (Connection *connection);
MAGNA_API am_bool MAGNA_CALL connection_parse_string       (Connection *connection, Buffer *connectionString);
MAGNA_API am_bool MAGNA_CALL connection_read_text_file     (Connection *connection, const Specification *specification, Buffer *buffer);
MAGNA_API am_bool MAGNA_CALL connection_search_ex          (Connection *connection, const SearchParameters *parameters, Response *response);
MAGNA_API am_bool MAGNA_CALL connection_set_database       (Connection *connection, const char *database);
MAGNA_API am_bool MAGNA_CALL connection_set_host           (Connection *connection, const char *host);
MAGNA_API am_bool MAGNA_CALL connection_set_password       (Connection *connection, const char *password);
MAGNA_API am_bool MAGNA_CALL connection_set_username       (Connection *connection, const char *username);

/*=========================================================*/

/* Различные книжные идентификаторы */

/* EAN-8 и EAN-13 */

MAGNA_API am_byte MAGNA_CALL ean13_compute_check_digit (const Span text);
MAGNA_API am_bool MAGNA_CALL ean13_check_control_digit (const Span text);
MAGNA_API am_byte MAGNA_CALL ean8_compute_check_digit  (const Span text);
MAGNA_API am_bool MAGNA_CALL ean8_check_control_digit  (const Span text);

/* UPC-12 */

MAGNA_API am_byte MAGNA_CALL upc12_compute_check_digit (const Span text);
MAGNA_API am_bool MAGNA_CALL upc12_check_control_digit (const Span text);

/* ISBN */

MAGNA_API am_bool MAGNA_CALL isbn_check_978           (const Span isbn);
MAGNA_API am_bool MAGNA_CALL isbn_check_control_digit (const Span isbn);

/*=========================================================*/

/* Формат ISO 2709 */

/* Маркер записи */
typedef struct
{
    am_byte length[5];
    am_byte status;
    am_byte type;
    am_byte index;
    am_byte unused[2];
    am_byte indicatorLength;
    am_byte identifierLength;
    am_byte base[5];
    am_byte level;
    am_byte rules;
    am_byte related;
    am_byte lengthOfLength;
    am_byte offsetLength;
    am_byte zero[2];

} IsoMarker;

/*=========================================================*/

#ifdef _M_IX86

/* Работа с irbis64.dll */

MAGNA_API void  MAGNA_CALL irbis64_dll_close             (void *space);
MAGNA_API void  MAGNA_CALL irbis64_dll_close_mst         (void *space);
MAGNA_API void  MAGNA_CALL irbis64_dll_close_term        (void *space);
MAGNA_API void*            irbis64_dll_init              (void);
MAGNA_API int   MAGNA_CALL irbis64_dll_init_deposit_path (char *path);
MAGNA_API int   MAGNA_CALL irbis64_dll_init_mst          (void *space, char *path, int shelfSize);
MAGNA_API int   MAGNA_CALL irbis64_dll_init_new_db       (char *path);
MAGNA_API int   MAGNA_CALL irbis64_dll_init_term         (void *space, char *path);
MAGNA_API int   MAGNA_CALL irbis64_dll_is_db_locked      (void *space);
MAGNA_API void  MAGNA_CALL irbis64_dll_main_ini_init     (char *path);
MAGNA_API int   MAGNA_CALL irbis64_dll_max_mfn           (void *space);
MAGNA_API int   MAGNA_CALL irbis64_dll_new_record        (void *space, int shelf);
MAGNA_API int   MAGNA_CALL irbis64_dll_read_version      (void *space, int mfn);
MAGNA_API int   MAGNA_CALL irbis64_dll_record            (void *space, int shelf, int mfn);
MAGNA_API void  MAGNA_CALL irbis64_dll_set_options       (int cacheable, int precompiled, int firstBreak);
MAGNA_API int   MAGNA_CALL irbis64_dll_uatab_init        (char *uctab, char *lctab, char *actab, char *exeDir, char *dataPath);
MAGNA_API void  MAGNA_CALL irbis64_dll_version           (char *buffer, int bufSize);

#endif

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/

#endif
