/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

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

#define LOGICALLY_DELETED  1u  /* Запись логически удалена     */
#define PHYSICALLY_DELETED 2u  /* Запись физически удалена     */
#define ABSENT             4u  /* Запись отсутствует           */
#define NON_ACTUALIZED     8u  /* Запись не актуализирована    */
#define LAST_VERSION       32u /* Последняя версия записи      */
#define LOCKED_RECORD      64u /* Запись заблокирована на ввод */

/* Коды АРМ */

#define ADMINISTRATOR 'A' /* Адмнистратор        */
#define CATALOGER     'C' /* Каталогизатор       */
#define ACQUSITIONS   'M' /* Комплектатор        */
#define READER        'R' /* Читатель            */
#define CIRCULATION   'B' /* Книговыдача         */
#define BOOKLAND      'B' /* Книговыдача         */
#define PROVISION     'K' /* Книгообеспеченность */

/* Разделитель строк в ИРБИС. */

#define IRBIS_DELIMITER "\x1F\x1E"

/* Короткие версии разделителя строк ИРБИС. */

#define SHORT_DELIMITER "\x1E"
#define ALT_DELIMITER   "\x1F"


/*=========================================================*/

/* Подполе */
typedef struct
{
    char *value;
    char code;

} SubField;

/*=========================================================*/

/* Поле записи */

typedef struct
{
    char *value;
    unsigned int tag;

} MarcField;

/*=========================================================*/

/* Раскодированная запись */

typedef struct
{
    unsigned int mfn;
    unsigned int status;
    unsigned int version;
    char *database;

} MarcRecord;

/*=========================================================*/

/* Опережающее объявление */

struct IrbisConnection;

/*=========================================================*/

/* Клиентский запрос */

typedef struct {
    Buffer buffer;
} Query;

MAGNA_API void MAGNA_CALL query_create      (struct IrbisConnection *connection, Query *query);
MAGNA_API void MAGNA_CALL query_add_ansi    (Query *query, const char *text);
MAGNA_API void MAGNA_CALL query_add_format  (Query *query, const char *text);
MAGNA_API void MAGNA_CALL query_add_int32   (Query *query, am_int32 value);
MAGNA_API void MAGNA_CALL query_add_utf     (Query *query, const char *text);
MAGNA_API void MAGNA_CALL query_new_line    (Query *query);

/*=========================================================*/

/* Ответ сервера */

typedef struct {
    Buffer answer;
    Span command;
    am_int32 clientId;
    am_int32 queryId;
    am_int32 returnCode;
    am_int32 answerSize;
    Span serverVersion;
    struct IrbisConnection *connection;
} Response;

MAGNA_API am_int32 MAGNA_CALL response_create                (struct IrbisConnection *connection,
                                                                Response *response);
MAGNA_API am_bool             response_check                 (Response *response, ...);
MAGNA_API Span     MAGNA_CALL response_get_line              (Response *response);
MAGNA_API am_int32 MAGNA_CALL response_get_return_code       (Response *response);
MAGNA_API Span     MAGNA_CALL response_read_ansi             (Response *response);
MAGNA_API am_int32 MAGNA_CALL response_read_int32            (Response *response);
MAGNA_API Span     MAGNA_CALL response_read_utf              (Response *response);
MAGNA_API Array    MAGNA_CALL response_remaining_ansi_lines  (Response *response);
MAGNA_API Span     MAGNA_CALL response_remaining_ansi_text   (Response *response);
MAGNA_API Array    MAGNA_CALL response_remaining_utf_lines   (Response *response);
MAGNA_API Span     MAGNA_CALL response_remaining_utf_text    (Response *response);

/*=========================================================*/

/* Подключение к серверу */

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
    char *serverVersion;
    int interval;

} Connection;

MAGNA_API void MAGNA_CALL connection_init (Connection *connection);
MAGNA_API void MAGNA_CALL connection_free (Connection *connection);

/*=========================================================*/

MAGNA_API am_byte MAGNA_CALL ean13_compute_check_digit (Span text);
MAGNA_API am_bool MAGNA_CALL ean13_check_control_digit (Span text);
MAGNA_API am_byte MAGNA_CALL ean8_compute_check_digit  (Span text);
MAGNA_API am_bool MAGNA_CALL ean8_check_control_digit  (Span text);

/*=========================================================*/

#ifdef _M_IX86

MAGNA_API void  MAGNA_CALL irbis64_dll_version (char *buffer, int bufSize);
MAGNA_API void  MAGNA_CALL irbis64_dll_main_ini_init (char *path);
MAGNA_API void* irbis64_dll_init (void);
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

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/

#endif
