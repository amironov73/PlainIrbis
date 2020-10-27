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

/* Пути к файлам */

#define PATH_SYSTEM    0
#define PATH_DATA      1
#define PATH_MASTER    2
#define PATH_INVERTED  3
#define PATH_PARAMETER 10
#define PATH_FULLTEXT  11
#define PATH_RESOURCE  12

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

/* Опережающее объявление */

struct IrbisConnection;
typedef struct IrbisConnection Connection;

struct IrbisQuery;
typedef struct IrbisQuery Query;

struct IrbisResponse;
typedef struct IrbisResponse Response;

/*=========================================================*/

/* Работа с ошибками */

MAGNA_API const char* MAGNA_CALL irbis_describe_error (am_int32 code);

/*=========================================================*/

/* Подполе записи */

typedef struct
{
    Buffer value;
    am_byte code;

} SubField;

#define SUBFIELD_INIT { BUFFER_INIT, 0 }

MAGNA_API am_bool   MAGNA_CALL subfield_assign         (SubField *subField, Span value);
MAGNA_API am_bool   MAGNA_CALL subfield_clone          (SubField *target, const SubField *source);
MAGNA_API am_bool   MAGNA_CALL subfield_code_is_valid  (am_byte code);
MAGNA_API am_bool   MAGNA_CALL subfield_create         (SubField *subfield, am_byte code, Span value);
MAGNA_API am_bool   MAGNA_CALL subfield_decode         (SubField *subfield, Span span);
MAGNA_API am_bool   MAGNA_CALL subfield_is_empty       (const SubField *subfield);
MAGNA_API void      MAGNA_CALL subfield_destroy        (SubField *subfield);
MAGNA_API SubField* MAGNA_CALL subfield_init           (SubField *subfield, am_byte code, Span value);
MAGNA_API am_byte   MAGNA_CALL subfield_normalize_code (am_byte code);
MAGNA_API void      MAGNA_CALL subfield_to_console     (const SubField *subfield);
MAGNA_API am_bool   MAGNA_CALL subfield_to_string      (const SubField *subfield, Buffer *output);
MAGNA_API am_bool   MAGNA_CALL subfield_verify         (const SubField *subfield);

/*=========================================================*/

/* Поле записи */

typedef struct
{
    Buffer value;
    Array subfields;
    am_uint32 tag;

} MarcField;

MAGNA_API am_bool    MAGNA_CALL field_add                      (MarcField *field, am_byte code, Span value);
MAGNA_API MarcField* MAGNA_CALL field_clear                    (MarcField *field);
MAGNA_API am_bool    MAGNA_CALL field_clone                    (MarcField *target, const MarcField *source);
MAGNA_API void       MAGNA_CALL field_create                   (MarcField *field);
MAGNA_API am_bool    MAGNA_CALL field_decode                   (MarcField *field, Span span);
MAGNA_API am_bool    MAGNA_CALL field_decode_body              (MarcField *field, Span span);
MAGNA_API void       MAGNA_CALL field_destroy                  (MarcField *field);
MAGNA_API Vector*    MAGNA_CALL field_get_embedded_fields      (const MarcField *field, Vector *array);
MAGNA_API SubField*  MAGNA_CALL field_get_first_subfield       (const MarcField *field, am_byte code);
MAGNA_API Span       MAGNA_CALL field_get_first_subfield_value (const MarcField *field, am_byte code);
MAGNA_API SubField*  MAGNA_CALL field_get_subfield_by_index    (const MarcField *field, size_t index);
MAGNA_API am_bool    MAGNA_CALL field_insert_at                (MarcField *field, size_t index, am_byte code, Span value);
MAGNA_API am_bool    MAGNA_CALL field_is_empty                 (const MarcField *field);
MAGNA_API MarcField* MAGNA_CALL field_remove_at                (MarcField *field, size_t index);
MAGNA_API MarcField* MAGNA_CALL field_remove_subfield          (MarcField *field, am_byte code);
MAGNA_API am_bool    MAGNA_CALL field_set_subfield             (MarcField *field, am_byte code, Span value);
MAGNA_API am_bool    MAGNA_CALL field_set_value                (MarcField *field, Span value);
MAGNA_API void       MAGNA_CALL field_to_console               (const MarcField *field);
MAGNA_API am_bool    MAGNA_CALL field_to_string                (const MarcField *field, Buffer *buffer);
MAGNA_API am_bool    MAGNA_CALL field_verify                   (const MarcField *field);

/*=========================================================*/

/* Раскодированная запись */

typedef struct
{
    Array fields;
    Buffer database;
    void *data;
    am_mfn mfn;
    am_flag status;
    am_uint32 version;

} MarcRecord;

MAGNA_API MarcField*  MAGNA_CALL record_add          (MarcRecord *record, am_uint32 tag, const am_byte *value);
MAGNA_API void        MAGNA_CALL record_clear        (MarcRecord *record);
MAGNA_API MarcRecord* MAGNA_CALL record_clone        (MarcRecord *target, const MarcRecord *source);
MAGNA_API void        MAGNA_CALL record_destroy      (MarcRecord *record);
MAGNA_API am_bool     MAGNA_CALL record_decode_lines (MarcRecord *record, Vector *lines);
MAGNA_API am_bool     MAGNA_CALL record_encode       (const MarcRecord *record, const char *delimiter, Buffer *buffer);
MAGNA_API Span        MAGNA_CALL record_fm           (const MarcRecord *record, am_uint32 tag, am_byte code);
MAGNA_API am_bool     MAGNA_CALL record_fma          (const MarcRecord *record, Vector *array, am_uint32 tag, am_byte code);
MAGNA_API MarcField*  MAGNA_CALL record_get          (const MarcRecord *record, size_t index);
MAGNA_API MarcField*  MAGNA_CALL record_get_field    (const MarcRecord *record, am_uint32 tag, size_t occurrence);
MAGNA_API void        MAGNA_CALL record_init         (MarcRecord *record);
MAGNA_API am_bool     MAGNA_CALL record_parse_single (MarcRecord *record, Response *response);
MAGNA_API am_bool     MAGNA_CALL record_set_field    (MarcRecord *record, am_uint32 tag, Span value);
MAGNA_API void        MAGNA_CALL record_to_console   (const MarcRecord *record);

/*=========================================================*/

/* Сырая запись */

typedef struct
{
    Array fields;
    Buffer database;
    void *data;
    am_mfn mfn;
    am_flag status;
    am_uint32 version;

} RawRecord;

MAGNA_API void    MAGNA_CALL raw_record_clear         (RawRecord *record);
MAGNA_API void    MAGNA_CALL raw_record_destroy       (RawRecord *record);
MAGNA_API Buffer* MAGNA_CALL raw_record_get           (const RawRecord *record, size_t index);
MAGNA_API void    MAGNA_CALL raw_record_init          (RawRecord *record);
MAGNA_API am_bool MAGNA_CALL raw_record_parse_single  (RawRecord *record, Response *response);
MAGNA_API am_bool MAGNA_CALL raw_record_to_string     (const RawRecord *record, Buffer *output, const am_byte *delimiter);

/*=========================================================*/

/* Таблица алфавитных символов */

typedef struct
{
    am_byte *characters;
    size_t charCount;

} AlphabetTable;

#define APLHABET_TABLE "ISISACW.TAB"

MAGNA_API am_bool              MAGNA_CALL alpha_create    (AlphabetTable *table, const am_byte *characters, size_t charCount);
MAGNA_API const AlphabetTable*            alpha_default   (void);
MAGNA_API void                 MAGNA_CALL alpha_destroy   (AlphabetTable *table);
MAGNA_API am_bool              MAGNA_CALL alpha_is_good   (const AlphabetTable *table, am_byte character);
MAGNA_API am_bool              MAGNA_CALL alpha_parse     (AlphabetTable *table, Span text);
MAGNA_API am_bool              MAGNA_CALL alpha_split     (const AlphabetTable *table, SpanArray *output, Span text);
MAGNA_API Span                 MAGNA_CALL alpha_trim      (const AlphabetTable *table, Span text);
MAGNA_API am_bool              MAGNA_CALL alpha_to_string (const AlphabetTable *table, Buffer *output);

/*=========================================================*/

/* Таблица преобразования в верхний регистр */

typedef struct
{
    am_byte xlat[256];

} UpperCaseTable;

#define UPPERCASE_TABLE "ISISUCW.TAB"

MAGNA_API am_bool MAGNA_CALL upper_init (UpperCaseTable *table, const am_byte *characters);

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

MAGNA_API am_bool MAGNA_CALL spec_init      (Specification *spec, int path, const char *database, const char *filename);
MAGNA_API am_bool MAGNA_CALL spec_parse     (Specification *spec, Buffer *buffer);
MAGNA_API am_bool MAGNA_CALL spec_to_string (const Specification *spec, Buffer *buffer);
MAGNA_API am_bool MAGNA_CALL spec_verify    (const Specification *spec);

/*=========================================================*/

/* Определение таблицы для метода `connection_print_table` */

typedef struct
{
    Buffer database;        /* Имя базы данных. */
    Buffer table;           /* Имя таблицы. */
    Buffer headers;         /* Заголовки таблицы. */
    Buffer mode;            /* Режим таблицы. */
    Buffer searchQuery;     /* Поисковый запрос по словарю. */
    Buffer sequentialQuery; /* Запрос для последовательного поиска. */
    Int32Array mfnList;     /* Список MFN, по которым строится таблица. */
    am_int32 minMfn;        /* Минимальный MFN, 0 = нет. */
    am_int32 maxMfn;        /* Максимальный MFN, 0 = нет. */

} TableDefinition;

MAGNA_API am_bool MAGNA_CALL table_definition_decode  (const TableDefinition *table, Response *response, Buffer *output);
MAGNA_API void    MAGNA_CALL table_definition_destroy (TableDefinition *table);
MAGNA_API am_bool MAGNA_CALL table_definition_encode  (TableDefinition *table, Connection *connection, Query *query);
MAGNA_API void    MAGNA_CALL table_definition_init    (TableDefinition *table);

/*=========================================================*/

/* Клиентский запрос */

struct IrbisQuery
{
    Buffer buffer; /* Буфер для накопления данных запроса. */
};

MAGNA_API am_bool MAGNA_CALL query_add_ansi          (Query *query, const am_byte *text);
MAGNA_API am_bool MAGNA_CALL query_add_ansi_buffer   (Query *query, const Buffer *text);
MAGNA_API am_bool MAGNA_CALL query_add_format        (Query *query, const am_byte *text);
MAGNA_API am_bool MAGNA_CALL query_add_int32         (Query *query, am_int32 value);
MAGNA_API am_bool MAGNA_CALL query_add_specification (Query *query, const Specification *specification);
MAGNA_API am_bool MAGNA_CALL query_add_uint32        (Query *query, am_uint32 value);
MAGNA_API am_bool MAGNA_CALL query_add_utf           (Query *query, const am_byte *text);
MAGNA_API am_bool MAGNA_CALL query_add_utf_buffer    (Query *query, const Buffer *text);
MAGNA_API am_bool MAGNA_CALL query_create            (Query *query, Connection *connection, const am_byte *command);
MAGNA_API void    MAGNA_CALL query_destroy           (Query *query);
MAGNA_API am_bool MAGNA_CALL query_encode            (const Query *query, Buffer *prefix);
MAGNA_API am_bool MAGNA_CALL query_new_line          (Query *query);

/*=========================================================*/

/* Ответ сервера */

struct IrbisResponse
{
    Buffer answer;           /* Содержимое ответа, помещенное в буфер. */
    Span command;            /* Код команды, присланный в ответе. */
    am_int32 clientId;       /* Идентификатор клиента, присланный в ответе. */
    am_int32 queryId;        /* Идентификатор запроса, присланный в ответе. */
    am_int32 returnCode;     /* Код ошибки (не всегда доступен). */
    am_int32 answerSize;     /* Размер ответа сервера в байтах (может высылаться сервером). */
    Span serverVersion;      /* Интервал автоматического подтверждения в минутах. */
    Connection *connection;  /* Указатель на подключение */
    TextNavigator navigator; /* Навигатор для считывания строк */

};

MAGNA_API am_bool             response_check                 (Response *response, ...);
MAGNA_API void     MAGNA_CALL response_destroy               (Response *response);
MAGNA_API am_bool  MAGNA_CALL response_eot                   (const Response *response);
MAGNA_API Span     MAGNA_CALL response_get_line              (Response *response);
MAGNA_API am_int32 MAGNA_CALL response_get_return_code       (Response *response);
MAGNA_API void     MAGNA_CALL response_init                  (Response *response);
MAGNA_API Span     MAGNA_CALL response_read_ansi             (Response *response);
MAGNA_API am_int32 MAGNA_CALL response_read_int32            (Response *response);
MAGNA_API Span     MAGNA_CALL response_read_utf              (Response *response);
MAGNA_API am_bool  MAGNA_CALL response_remaining_ansi_lines  (Response *response, SpanArray *array);
MAGNA_API Span     MAGNA_CALL response_remaining_ansi_text   (Response *response);
MAGNA_API am_bool  MAGNA_CALL response_remaining_utf_lines   (Response *response, SpanArray *array);
MAGNA_API Span     MAGNA_CALL response_remaining_utf_text    (Response *response);

/*=========================================================*/

/* Инфорация о пользователе системы */

typedef struct
{
    Buffer number;
    Buffer name;
    Buffer password;
    Buffer cataloger;
    Buffer reader;
    Buffer circulation;
    Buffer acquisitions;
    Buffer provision;
    Buffer administrator;

} UserInfo;

MAGNA_API void    MAGNA_CALL userinfo_destroy       (UserInfo *userinfo);
MAGNA_API void    MAGNA_CALL userinfo_destroy_array (Array *users);
MAGNA_API void    MAGNA_CALL userinfo_init          (UserInfo *userinfo);
MAGNA_API void    MAGNA_CALL userinfo_init_array    (Array *users);
MAGNA_API am_bool MAGNA_CALL userinfo_parse_lines   (UserInfo *userinfo, const Span *lines);

/*=========================================================*/

/* Информация о версии сервера */

typedef struct {
    Buffer organization;
    Buffer version;
    am_uint32 maxClients;
    am_uint32 connected;

} ServerVersion;

MAGNA_API void    MAGNA_CALL version_destroy        (ServerVersion *version);
MAGNA_API void    MAGNA_CALL version_init           (ServerVersion *version);
MAGNA_API am_bool MAGNA_CALL version_parse_response (ServerVersion *version, Response *response);
MAGNA_API am_bool MAGNA_CALL version_to_string      (const ServerVersion *version, Buffer *output);

/*=========================================================*/

/* Меню */

/* Пара строк в MNU-файле. */
typedef struct
{
    Buffer code;
    Buffer comment;

} MenuEntry;

MAGNA_API void    MAGNA_CALL menu_entry_init      (MenuEntry *entry);
MAGNA_API void    MAGNA_CALL menu_entry_destroy   (MenuEntry *entry);
MAGNA_API am_bool MAGNA_CALL menu_entry_to_string (const MenuEntry *entry, Buffer *output);

/* MNU-файл. состоит из пар строк (см. MenuEntry). */
typedef struct
{
    Array entries;

} MenuFile;

MAGNA_API void             MAGNA_CALL menu_init        (MenuFile *menu);
MAGNA_API void             MAGNA_CALL menu_destroy     (MenuFile *menu);
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

MAGNA_API void    MAGNA_CALL ini_line_destroy   (IniLine *line);
MAGNA_API void    MAGNA_CALL ini_line_init      (IniLine *line);
MAGNA_API am_bool MAGNA_CALL ini_line_set_key   (IniLine *line, Span key);
MAGNA_API am_bool MAGNA_CALL ini_line_set_value (IniLine *line, Span value);
MAGNA_API am_bool MAGNA_CALL ini_line_to_string (const IniLine *line, Buffer *output);
MAGNA_API am_bool MAGNA_CALL ini_line_verify    (const IniLine *line);

/* Секция INI-файла. Состоит из строк (см. IniLine). Имеет опциональное имя. */
typedef struct
{
    Buffer name;
    Array lines;

} IniSection;

MAGNA_API void           MAGNA_CALL ini_section_clear        (IniSection *section);
MAGNA_API void           MAGNA_CALL ini_section_destroy      (IniSection *section);
MAGNA_API const IniLine* MAGNA_CALL ini_section_get_line     (const IniSection *section, Span key);
MAGNA_API Span           MAGNA_CALL ini_section_get_value    (const IniSection *section, Span key, Span defaultValue);
MAGNA_API am_bool        MAGNA_CALL ini_section_init         (IniSection *section);
MAGNA_API am_bool        MAGNA_CALL ini_section_is_modified  (const IniSection *section);
MAGNA_API void           MAGNA_CALL ini_section_not_modified (IniSection *section);

/* INI-файл. Состоит из секций. */
typedef struct
{
    Array sections;
} IniFile;

MAGNA_API void    MAGNA_CALL ini_file_destroy      (IniFile *file);
MAGNA_API am_bool MAGNA_CALL ini_file_init         (IniFile *file);
MAGNA_API am_bool MAGNA_CALL ini_file_is_modified  (const IniFile *file);
MAGNA_API void    MAGNA_CALL ini_file_not_modified (IniFile *file);

/*=========================================================*/

/* Работа с терминами словаря */

/* Информация об одном термине поискового словаря */
typedef struct
{
    Buffer text;
    am_int32 count;

} TermInfo;

MAGNA_API void    MAGNA_CALL term_destroy        (TermInfo *term);
MAGNA_API void    MAGNA_CALL term_destroy_array  (Array *terms);
MAGNA_API void    MAGNA_CALL term_init           (TermInfo *term);
MAGNA_API void    MAGNA_CALL term_init_array     (Array *terms);
MAGNA_API am_bool MAGNA_CALL term_parse_line     (TermInfo *term, Span line);
MAGNA_API am_bool MAGNA_CALL term_parse_response (Array *terms, Response *response);
MAGNA_API am_bool MAGNA_CALL term_to_string      (const TermInfo *term, Buffer *output);

/* Параметры извлечения терминов из поискового словаря */
typedef struct
{
    Buffer database;
    Buffer startTerm;
    Buffer format;
    am_uint32 number;
    am_bool reverseOrder;

} TermParameters;

MAGNA_API void MAGNA_CALL term_parameters_destroy (TermParameters *parameters);
MAGNA_API void MAGNA_CALL term_parameters_init    (TermParameters *parameters);

/* Постинг термина */
typedef struct
{
    Buffer text;
    am_uint32 mfn;
    am_uint32 tag;
    am_uint32 occurrence;
    am_uint32 count;

} TermPosting;

MAGNA_API void    MAGNA_CALL posting_destroy        (TermPosting *posting);
MAGNA_API void    MAGNA_CALL posting_destroy_array  (Array *postings);
MAGNA_API void    MAGNA_CALL posting_init           (TermPosting *posting);
MAGNA_API void    MAGNA_CALL posting_init_array     (Array *postings);
MAGNA_API am_bool MAGNA_CALL posting_parse_line     (TermPosting *posting, Span line);
MAGNA_API am_bool MAGNA_CALL posting_parse_response (Array *postings, Response *response);
MAGNA_API am_bool MAGNA_CALL posting_to_string      (const TermPosting *posting, Buffer *output);

/*=========================================================*/

/* Поиск */

/* Построитель запросов */
typedef struct
{
    Buffer buffer;

} Search;

MAGNA_API am_bool MAGNA_CALL search_create  (Search *search, Span text);
MAGNA_API void    MAGNA_CALL search_destroy (Search *search);
MAGNA_API am_bool MAGNA_CALL search_equals  (Search *search, Span prefix, Span value);

/* Параметры для поиска записей */
typedef struct
{
    Buffer expression;  /* Выражение для поиска по словарю (быстрый поиск) */
    Buffer database;    /* Имя базы данных (опционально). */
    Buffer format;      /* Опциональный формат для найденных записей. */
    Buffer sequential;  /* Выражение для последовательного поиска (опционально). */
    Buffer filter;      /* Фильтр для записей (фильтрация на клиенте) (опционально). */
    am_mfn firstRecord; /* Индекс первой из возвращаемых записей (по умолчанию 1). */
    am_mfn minMfn;      /* Минимальный MFN (опционально). */
    am_mfn maxMfn;      /* Максимальный MFN (опционально). */
    am_mfn number;      /* Количество возвращаемых записей (0 = все). */

} SearchParameters;

MAGNA_API am_bool MAGNA_CALL search_parameters_create  (SearchParameters *parameters, const am_byte *expression);
MAGNA_API void    MAGNA_CALL search_parameters_destroy (SearchParameters *parameters);
MAGNA_API am_bool MAGNA_CALL search_parameters_encode  (const SearchParameters *parameters, Connection *connection, Query *query);
MAGNA_API void    MAGNA_CALL search_parameters_init    (SearchParameters *parameters);
MAGNA_API am_bool MAGNA_CALL search_parameters_verify  (const SearchParameters *parameters);

/* Сценарий поиска */
typedef struct
{
    Buffer name;             /* Наименование поискового атрибута (автор, заглавие и т. п.). */
    Buffer prefix;           /* Префикс соответствующих терминов в поисковом словаре. */
    Buffer menuName;         /* Имя файла справочника (меню). */
    Buffer oldFormat;        /* Имя формата (без расширения). */
    Buffer correction;       /* Способ корректировки по словарю. */
    Buffer truncation;       /* Исходное положение переключателя "усечение". */
    Buffer hint;             /* Текст подсказки/предупреждения. */
    Buffer modByDicAuto;     /* Параметр пока не задействован. */
    Buffer logic;            /* Применимые логические операторы. */
    Buffer advance;          /* Правила автоматического расширения поиска. */
    Buffer format;           /* Имя формата показа документов. */
    am_int32 dictionaryType; /* Тип словаря для соответствующего поиска. */

} SearchScenario;

MAGNA_API void    MAGNA_CALL scenario_init    (SearchScenario *scenario);
MAGNA_API void    MAGNA_CALL scenario_destroy (SearchScenario *scenario);

/* Одна строка из списка найденных записей */

typedef struct
{
    Buffer description; /* Опциональный текст. */
    am_mfn mfn;         /* MFN найденной записи. */

} FoundLine;

MAGNA_API void    MAGNA_CALL found_array_init          (Array *array);
MAGNA_API void    MAGNA_CALL found_array_destroy       (Array *array);
MAGNA_API am_bool MAGNA_CALL found_decode_line         (FoundLine *found, Span line);
MAGNA_API am_bool MAGNA_CALL found_decode_response     (Array *array, Response *response);
MAGNA_API am_bool MAGNA_CALL found_decode_response_mfn (Int32Array *array, Response *response);
MAGNA_API void    MAGNA_CALL found_destroy             (FoundLine *found);
MAGNA_API void    MAGNA_CALL found_init                (FoundLine *found);

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
    am_byte workstation;

};

MAGNA_API am_bool  MAGNA_CALL connection_actualize_database (Connection *connection, const am_byte *database);
MAGNA_API am_bool  MAGNA_CALL connection_actualize_record   (Connection *connection, const am_byte *database, am_mfn mfn);
MAGNA_API am_bool  MAGNA_CALL connection_check              (Connection *connection);
MAGNA_API am_bool  MAGNA_CALL connection_create             (Connection *connection);
MAGNA_API am_bool  MAGNA_CALL connection_connect            (Connection *connection);
MAGNA_API am_bool  MAGNA_CALL connection_create_database    (Connection *connection, const am_byte *database, const am_byte *description, am_bool readerAccess);
MAGNA_API am_bool  MAGNA_CALL connection_create_dictionary  (Connection *connection, const am_byte *database);
MAGNA_API am_bool  MAGNA_CALL connection_delete_database    (Connection *connection, const am_byte *database);
MAGNA_API am_bool  MAGNA_CALL connection_delete_file        (Connection *connection, const am_byte *fileName);
MAGNA_API am_bool  MAGNA_CALL connection_delete_record      (Connection *connection, am_mfn mfn);
MAGNA_API am_bool  MAGNA_CALL connection_disconnect         (Connection *connection);
MAGNA_API am_bool  MAGNA_CALL connection_execute            (Connection *connection, Query *query, Response *response);
MAGNA_API am_bool             connection_execute_simple     (Connection *connection, Response *response, const am_byte *command, int argCount, ...);
MAGNA_API void     MAGNA_CALL connection_destroy            (Connection *connection);
MAGNA_API am_bool  MAGNA_CALL connection_format_mfn         (Connection *connection, const am_byte *format, am_mfn mfn, Buffer *output);
MAGNA_API am_mfn   MAGNA_CALL connection_get_max_mfn        (Connection *connection, const am_byte *database);
MAGNA_API am_bool  MAGNA_CALL connection_get_server_version (Connection *connection, ServerVersion *version);
MAGNA_API am_bool  MAGNA_CALL connection_no_operation       (Connection *connection);
MAGNA_API am_bool  MAGNA_CALL connection_parse_string       (Connection *connection, Span connectionString);
MAGNA_API am_bool  MAGNA_CALL connection_print_table        (Connection *connection, TableDefinition *definition, Buffer *output);
MAGNA_API am_bool  MAGNA_CALL connection_read_raw_record    (Connection *connection, am_mfn mfn, RawRecord *record);
MAGNA_API am_bool  MAGNA_CALL connection_read_record        (Connection *connection, am_mfn mfn, MarcRecord *record);
MAGNA_API am_bool  MAGNA_CALL connection_read_record_text   (Connection *connection, am_mfn mfn, Buffer *buffer);
MAGNA_API am_bool  MAGNA_CALL connection_read_text_file     (Connection *connection, const Specification *specification, Buffer *buffer);
MAGNA_API am_bool  MAGNA_CALL connection_reload_dictionary  (Connection *connection, const am_byte *database);
MAGNA_API am_bool  MAGNA_CALL connection_reload_master_file (Connection *connection, const am_byte *database);
MAGNA_API am_bool  MAGNA_CALL connection_restart_server     (Connection *connection);
MAGNA_API am_int32 MAGNA_CALL connection_search_count       (Connection *connection, const am_byte *expression);
MAGNA_API am_bool  MAGNA_CALL connection_search_ex          (Connection *connection, const SearchParameters *parameters, Response *response);
MAGNA_API am_bool  MAGNA_CALL connection_search_simple      (Connection *connection,  Int32Array *array, const am_byte *expression);
MAGNA_API am_bool  MAGNA_CALL connection_set_database       (Connection *connection, const am_byte *database);
MAGNA_API am_bool  MAGNA_CALL connection_set_host           (Connection *connection, const am_byte *host);
MAGNA_API am_bool  MAGNA_CALL connection_set_password       (Connection *connection, const am_byte *password);
MAGNA_API am_bool  MAGNA_CALL connection_set_username       (Connection *connection, const am_byte *username);
MAGNA_API am_bool  MAGNA_CALL connection_to_string          (const Connection *connection, Buffer *output);
MAGNA_API am_bool  MAGNA_CALL connection_truncate_database  (Connection *connection, const am_byte *database);
MAGNA_API am_bool  MAGNA_CALL connection_unlock_database    (Connection *connection, const am_byte *database);

/* Синонимы */

MAGNA_API am_bool  MAGNA_CALL irbis_connect                 (Connection *connection);
MAGNA_API am_bool  MAGNA_CALL irbis_disconnect              (Connection *connection);


/*=========================================================*/

/* Различные книжные идентификаторы */

/* EAN-8 и EAN-13 */

MAGNA_API am_byte MAGNA_CALL ean13_compute_check_digit (Span text);
MAGNA_API am_bool MAGNA_CALL ean13_check_control_digit (Span text);
MAGNA_API am_byte MAGNA_CALL ean8_compute_check_digit  (Span text);
MAGNA_API am_bool MAGNA_CALL ean8_check_control_digit  (Span text);

/* UPC-12 */

MAGNA_API am_byte MAGNA_CALL upc12_compute_check_digit (Span text);
MAGNA_API am_bool MAGNA_CALL upc12_check_control_digit (Span text);

/* ISBN */

MAGNA_API am_bool MAGNA_CALL isbn_check_978           (Span isbn);
MAGNA_API am_bool MAGNA_CALL isbn_check_control_digit (Span isbn);

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
