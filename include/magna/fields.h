// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef MAGNA_FIELDS_H
#define MAGNA_FIELDS_H

#include "magna/irbis.h"

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4820)
#endif

/*=========================================================*/

/* Адрес читателя */

typedef struct
{
    Buffer postcode;   /* Почтовый индекс, подполе A. */
    Buffer country;    /* Страна/республика, подполе B. */
    Buffer city;       /* Город, подполе C. */
    Buffer street;     /* Улица, подполе D. */
    Buffer building;   /* Номер дома, подполе E. */
    Buffer entrance;   /* Номер подъезда, подполе G. */
    Buffer apartment;  /* Номер квартиры, подпле H. */
    Buffer additional; /* Дополнительные данные, подполе F. */
    am_mix userData;   /* Произвольные пользовательские данные. */
    MarcField *field;  /* Поле, из которого извлечены данные об адресе. */

} Address;

#define ADDRESS_TAG 13

MAGNA_API am_bool MAGNA_CALL address_apply       (const Address *address, MarcField *field);
MAGNA_API void    MAGNA_CALL address_destroy     (Address *address);
MAGNA_API void    MAGNA_CALL address_init        (Address *address);
MAGNA_API am_bool MAGNA_CALL address_parse_field (Address *address, const MarcField *field);

/*=========================================================*/

/* Индивидуальный автор/редактор */

typedef struct
{
    Buffer familyName;    /* Фамилия, подполе A. */
    Buffer initials;      /* Инициалы (сокращение), подполе B. */
    Buffer fullName;      /* Расширение инициалов (имя и отчество), подполе G. */
    Buffer canBeInverted; /* Инвертирование имени недопустимо? Подполе 9. */
    Buffer postfix;       /* Неотъемлемая часть имени (отец, сын, младший, старший и т. п.), подполе 1. */
    Buffer appendix;      /* Дополнения к именам кроме дат (род деятельности, звание, титул и т. д.), подполе C. */
    Buffer number;        /* Династический номер (римские цифры), подполе D. */
    Buffer dates;         /* Даты жизни, подполе F. */
    Buffer variant;       /* Разночтение фамилии, подполе R. */
    Buffer workplace;     /* Место работы автора, подполе P. */
    MarcField *field;     /* Поле, из которого была извлечена информация. */

} Author;

MAGNA_API am_bool MAGNA_CALL author_apply         (const Author *author, MarcField *field);
MAGNA_API void    MAGNA_CALL author_array_destroy (Array *authors);
MAGNA_API void    MAGNA_CALL author_array_init    (Array *authors);
MAGNA_API void    MAGNA_CALL author_destroy       (Author *author);
MAGNA_API void    MAGNA_CALL author_init          (Author *author);
MAGNA_API am_bool MAGNA_CALL author_parse_field   (Author *author, const MarcField *field);
MAGNA_API am_bool MAGNA_CALL author_parse_record  (const MarcRecord *record, am_uint32 tag, Array *authors);

/*=========================================================*/

/* Информация о заглавии, поле 200. */

typedef struct
{
    Buffer number;    /* Обозначение и номер тома, подполе V. */
    Buffer title;     /* Собственно заглавие, подполе A. */
    Buffer specific;  /* Нехарактерное заглавие, подполе U. */
    Buffer general;   /* Общее обозначение материала, подполе B. */
    Buffer subtitle;  /* Сведения, относящиеся к заглавию, подполе E. */
    Buffer first;     /* Первые сведения об ответственности, подполе F. */
    Buffer other;     /* Последующие сведения об ответственности, подполе G. */
    MarcField *field; /* Поле, из которого была извлечена информация. */

} Title;

#define TITLE_TAG 200

MAGNA_API am_bool MAGNA_CALL title_apply       (const Title *title, MarcField *field);
MAGNA_API void    MAGNA_CALL title_destroy     (Title *title);
MAGNA_API void    MAGNA_CALL title_init        (Title *title);
MAGNA_API am_bool MAGNA_CALL title_parse_field (Title *title, const MarcField *field);
MAGNA_API void    MAGNA_CALL title_to_console  (const Title *title);
MAGNA_API am_bool MAGNA_CALL title_to_string   (const Title *title, Buffer *output);
MAGNA_API am_bool MAGNA_CALL title_verify      (const Title *title);

/*=========================================================*/

/* Кодированная информация о книге/журнале */

typedef struct
{
    Buffer type;          /* Тип документа, подполе T. */
    Buffer kind;          /* Вид документа, подполе B. */
    Buffer character [6]; /* Характер документа, подполя C, 2, 3, 4, 5, 6. */
    Buffer purpose [3];   /* Код целевого назначения, подполя X, Y, 9. */
    Buffer restrictions;  /* Возрастные ограничения, подполе Z. */
    MarcField *field;     /* Поле, из которого была извлечена информация. */

} Codes;

MAGNA_API am_bool MAGNA_CALL codes_apply       (const Codes *codes, MarcField *field);
MAGNA_API void    MAGNA_CALL codes_destroy     (Codes *codes);
MAGNA_API void    MAGNA_CALL codes_init        (Codes *codes);
MAGNA_API am_bool MAGNA_CALL codes_parse_field (Codes *codes, const MarcField *field);

/*=========================================================*/

/* Коллективный автор */

typedef struct
{
    Buffer title;          /* Наименование коллектива, подполе A. */
    Buffer country;        /* Страна, подполе S. */
    Buffer abbreviation;   /* Аббревиатура, подполе R. */
    Buffer number;         /* Номер, подполе N. */
    Buffer date;           /* Дата, подполе F. */
    Buffer city;           /* Город, подполе C. */
    Buffer department;     /* Подразделение, подполе B. */
    Buffer characteristic; /* Характерное название подразделения, подполе X. */
    Buffer gost;           /* Сокращение названия по ГОСТ, подполе 7. */
    MarcField *field;      /* Поле, из которого извлечена информация. */

} Collective;

MAGNA_API am_bool MAGNA_CALL collective_apply        (const Collective *collective, MarcField *field);
MAGNA_API void    MAGNA_CALL collective_destroy      (Collective *collective);
MAGNA_API void    MAGNA_CALL collective_init         (Collective *collective);
MAGNA_API am_bool MAGNA_CALL collective_parse_field  (Collective *collective, const MarcField *field);
MAGNA_API am_bool            collective_parse_record (const MarcRecord *record, Array *collectives, ...);

/*=========================================================*/

/* ISBN и цена */

typedef struct
{
    Buffer isbn;       /* ISBN, подполе A. */
    Buffer refinement; /* Уточнение для ISBN, подполе B. */
    Buffer erroneous;  /* Ошибочный ISBN, подполе Z. */
    Buffer price;      /* Цена общая для всех экземпляров, цифры, подполе D, */
    Buffer currency;   /* Обозначение валюты, подполе C, */
    MarcField *field;  /* Поле, из которого извлечена информация. */

} IsbnInfo;

#define ISBN_TAG 10

MAGNA_API am_bool MAGNA_CALL isbn_apply       (const IsbnInfo *isbn, MarcField *field);
MAGNA_API void    MAGNA_CALL isbn_destroy     (IsbnInfo *isbn);
MAGNA_API void    MAGNA_CALL isbn_init        (IsbnInfo *isbn);
MAGNA_API am_bool MAGNA_CALL isbn_parse_field (IsbnInfo *isbn, const MarcField *field);

/*=========================================================*/

/* Сведения о внешнем ресурсе */

typedef struct
{
    Buffer filename;     /* Имя файла, подполе A. */
    Buffer url;          /* URL, подполе I. */
    Buffer description;  /* Текст для ссылки, подполе T. */
    Buffer fileCount;    /* Количество файлов, подполе N. */
    Buffer nameTemplate; /* Имя шаблона первого файла, подполе M. */
    Buffer fileType;     /* Тип внешнего файла, подполе H. */
    Buffer textbook;     /* Признак электронного учебника, подполе K. */
    Buffer accessLevel;  /* Уровень доступа по категориям пользователей, подполе D. */
    Buffer lanOnly;      /* Ресурс доступен только в ЛВС, подполе L. */
    Buffer inputDate;    /* Дата ввода информации, подполе 1. */
    Buffer fileSize;     /* Размер фмйла, подполе 2. */
    Buffer number;       /* Номер источника копии, подполе 3. */
    Buffer lastCheck;    /* Дата последней проверки доступности, подполе 5. */
    Buffer imageSize;    /* Размеры изображения в пикселах, подполе 6. */
    Buffer issn;         /* ISSN, подполе X. */
    Buffer form;         /* Форма представления, подполе B. */
    Buffer provider;     /* Код поставщика информации, подполе F. */
    Buffer price;        /* Цена, подполе E. */
    Buffer index;        /* Шифр в БД, подполе W. */
    Buffer remarks;      /* Примечания в свободной форме, подполе P. */
    Buffer system;       /* Электронная библиотечная система, подполе S. */
    MarcField *field;    /* Поле, из которого извлечена информация. */


} ExternalResource;

#define EXTERNAL_RESOURCE_TAG 951

MAGNA_API am_bool MAGNA_CALL external_resource_apply        (const ExternalResource *resource, MarcField *field);
MAGNA_API void    MAGNA_CALL external_resource_destroy      (ExternalResource *resource);
MAGNA_API void    MAGNA_CALL external_resource_init         (ExternalResource *resource);
MAGNA_API am_bool MAGNA_CALL external_resource_parse_field  (ExternalResource *resource, const MarcField *field);
MAGNA_API am_bool MAGNA_CALL external_resource_parse_record (const MarcRecord *record, Array *resources, am_uint32 tag);

/*=========================================================*/

/* Сведения о двоичном ресурсе */

typedef struct
{
    Buffer kind;      /* Тип двоичного ресурса, подполе A. */
    Buffer resource;  /* Закодированный двоичный ресурс, подполе B. */
    Buffer title;     /* Название двоичного ресурса, подполе T. */
    Buffer view;      /* Характер просмотра, подполе P. */
    MarcField *field; /* Поле, из которого извлечена информация. */

} BinaryResource;

#define BINARY_RESOURCE_TAG 953

MAGNA_API am_bool MAGNA_CALL binary_resource_apply        (const BinaryResource *resource, MarcField *field);
MAGNA_API void    MAGNA_CALL binary_resource_destroy      (BinaryResource *resource);
MAGNA_API void    MAGNA_CALL binary_resource_init         (BinaryResource *resource);
MAGNA_API am_bool MAGNA_CALL binary_resource_parse_field  (BinaryResource *resource, const MarcField *field);
MAGNA_API am_bool MAGNA_CALL binary_resource_parse_record (const MarcRecord *record, Array *resources, am_uint32 tag);

/*=========================================================*/

/* Экземпляр */

typedef struct
{
    Buffer status;        /* Статус экзмепляра, подполе 'a'. */
    Buffer number;        /* Инвентарный номер, подполе `b`. */
    Buffer date;          /* Дата поступления, подполе 'c'. */
    Buffer place;         /* Место хранения, подполе 'd'. */
    Buffer collection;    /* Наименование коллекции, подполе 'q'. */
    Buffer shelf;         /* Расстановочный шифр (полочный индекс), подполе 'r'. */
    Buffer price;         /* Цена экземпляра, подполе 'e'. */
    Buffer barcode;       /* Штрих-код или радиометка, подполе 'h'. */
    Buffer amount;        /* Количество экземпляров, подполе '1'. */
    Buffer purpose;       /* Специальное назначение фонда, подполе 't'. */
    Buffer coefficient;   /* Коэффициент многоразового использования, подполе '='. */
    Buffer offBalance;    /* Экземпляры не на баланс, подполе '4'. */
    Buffer ksuNumber1;    /* Номер записи КСУ (поступление), подполе 'u'. */
    Buffer actNumber1;    /* Номер акта поступления, подполе 'y'. */
    Buffer channel;       /* Канал поступления, подполе 'f'. */
    Buffer onHand;        /* Число выданных экземпляров, подполе '2'. */
    Buffer actNumber2;    /* Номер акта списания, подполе 'v'. */
    Buffer writeOff;      /* Количество списываемых экземпляров, подполе 'x'. */
    Buffer completion;    /* Количество экземпляров для докомплектования, подполе 'k'. */
    Buffer actNumber3;    /* Номер акта передачи в другое подразделение, подполе 'w'. */
    Buffer moving;        /* Количество передаваемых экземпляров, подполе 'z'. */
    Buffer newPlace;      /* Новое место хранения, подполе 'm'. */
    Buffer checkDate;     /* Дата проверки фонда, подполе 's'. */
    Buffer checkedAmount; /* Число проверенных экземпляров, подполе '0'. */
    Buffer realPlace;     /* Реальное место нахождения книги, подполе '!'. */
    Buffer bindingIndex;  /* Шифр подшивки, подполе 'p'. */
    Buffer bindingNumber; /* Инвентарный номер подшивки, подполе 'i'. */
    MarcField *field;     /* Поле, из которого извлечена информация. */

} Exemplar;

#define EXEMPLAR_TAG 910

#define EXEMPLAR_FREE          "0"
#define EXEMPLAR_LOAN          "1"
#define EXEMPLAR_WAIT          "2"
#define EXEMPLAR_BINDERY       "3"
#define EXEMPLAR_LOST          "4"
#define EXEMPLAR_NOT_AVAILABLE "5"
#define EXEMPLAR_WRITTEN_OFF   "6"
#define EXEMPLAR_ON_THE_WAY    "8"
#define EXEMPLAR_RESERVED      "9"
#define EXEMPLAR_BIBLIO_NET    "C"
#define EXEMPLAR_BOUND         "P"
#define EXEMPLAR_REPRODUCTION  "R"
#define EXEMPLAR_SUMMARY       "U"

MAGNA_API am_bool MAGNA_CALL exemplar_apply         (const Exemplar *exemplar, MarcField *field);
MAGNA_API void    MAGNA_CALL exemplay_array_destroy (Array *exemplars);
MAGNA_API void MAGNA_CALL exemplar_array_init       (Array *exemplars);
MAGNA_API void    MAGNA_CALL exemplar_destroy       (Exemplar *exemplar);
MAGNA_API void    MAGNA_CALL exemplar_init          (Exemplar *exemplar);
MAGNA_API am_bool MAGNA_CALL exemplar_parse_field   (Exemplar *exemplar, const MarcField *field);
MAGNA_API am_bool MAGNA_CALL exemplar_parse_record  (const MarcRecord *record, Array *exemplars, am_uint32 tag);

/*=========================================================*/

/* Сведения о посещении/книговыдаче */

typedef struct
{
    Buffer database;    /* Имя БД каталога, подполе g. */
    Buffer index;       /* Шифр документа, подполе a. */
    Buffer inventory;   /* Инвентарный номер экземпляра, подполе b. */
    Buffer barcode;     /* Штрих-код экземпляра, подполе h. */
    Buffer sigla;       /* Место хранения экземпляра, подполе k. */
    Buffer given;       /* Дата выдачи, подполе d. */
    Buffer department;  /* Место выдачи, подполе v. */
    Buffer expected;    /* Дата предполагаемого возврата, подполе e. */
    Buffer returned;    /* Дата фактического возврата, подполе f. */
    Buffer prolong;     /* Дата продления, подполе l. */
    Buffer lost;        /* Признак утерянной книги, подполе u. */
    Buffer description; /* Краткое библиографическое описание, подполе c. */
    Buffer responsible; /* Ответственное лицо, подполе i. */
    Buffer timeIn;      /* Время начала визита в библиотеку, подполе 1. */
    Buffer timeOut;     /* Время окончания визита в библиотеку, подполе 2. */
    Buffer count;       /* Счетчик продлений, подполе 4. */
    MarcField *field;   /* Поле, из которого извлечены данные о посещении. */

} Visit;

#define VISIT_TAG 40

MAGNA_API am_bool MAGNA_CALL visit_apply        (const Visit *visit, MarcField *field);
MAGNA_API void    MAGNA_CALL visit_destroy      (Visit *visit);
MAGNA_API void    MAGNA_CALL visit_init         (Visit *visit);
MAGNA_API am_bool MAGNA_CALL visit_is_returned  (const Visit *visit);
MAGNA_API am_bool MAGNA_CALL visit_is_visit     (const Visit *visit);
MAGNA_API am_bool MAGNA_CALL visit_parse_field  (Visit *visit, const MarcField *field);
MAGNA_API am_bool MAGNA_CALL visit_parse_record (const MarcRecord *record, Array *visits, am_uint32 tag);

/*=========================================================*/

/* Поле 203 */

typedef struct
{
    Buffer contentType [9];
    Buffer access [9];
    Buffer contentDescription [11];
    MarcField *field;

} Field203;

MAGNA_API void    MAGNA_CALL field203_init        (Field203 *field);
MAGNA_API void    MAGNA_CALL field203_destroy     (Field203 *field);
MAGNA_API am_bool MAGNA_CALL field203_parse_field (Field203 *fld203, MarcField *field);

/*=========================================================*/

/* Сведения о читателе */

typedef struct
{
    Buffer familyName;  /* Фамилия, поле 10. */
    Buffer firstName;   /* Имя, поле 11. */
    Buffer patronymic;  /* Отчество, поле 12. */
    Buffer dateOfBirth; /* Дата рождения, поле 21. */
    Buffer passCard;    /* Номер пропуска, поле 22. */
    Buffer ticket;      /* Номер читательского билета (идентификатор читателя), поле 30. */
    Buffer gender;      /* Пол, поле 23. */
    Buffer category;    /* Категория, поле 50. */
    /* Address */       /* */
    Buffer workPlace;   /* Место работы, поле 15. */
    Buffer education;   /* Образование, поле 20. */
    Buffer email;       /* Электронная почта, поле 32. */
    Buffer phone;       /* Домашний телефон, поле 17. */
    MarcRecord *record; /* Запись, из которой извлечены сведения о читателе. */

} ReaderInfo;

MAGNA_API am_bool MAGNA_CALL reader_apply        (const ReaderInfo *reader, MarcRecord *record);
MAGNA_API void    MAGNA_CALL reader_destroy      (ReaderInfo *reader);
MAGNA_API void    MAGNA_CALL reader_init         (ReaderInfo *reader);
MAGNA_API am_bool MAGNA_CALL reader_parse_record (ReaderInfo *reader, const MarcRecord *record);

/*=========================================================*/

/* Работа с журналами/газетами/подшивками и статьями. */

/* Информация о статье из журнала/сборника. */

typedef struct
{
    Array authors; /* Авторы, поля 70x, */
    Title title;   /* Завглавие, поле 200. */

} ArticleInfo;

MAGNA_API void MAGNA_CALL article_array_destroy (Array *articles);
MAGNA_API void MAGNA_CALL article_array_init    (Array *articles);
MAGNA_API void MAGNA_CALL article_destroy       (ArticleInfo *article);
MAGNA_API void MAGNA_CALL article_init          (ArticleInfo *article);

/* Информация о выпуске журнала/газеты. */

typedef struct
{
    Buffer index;        /* Шифр записи. Поле 903. */
    Buffer description;  /* Библиографическое описание. */
    Buffer magazineCode; /* Шифр журнала. Поле 933. */
    Buffer year;         /* Год. Поле 934. */
    Buffer volume;       /* Том. Поле 935. */
    Buffer number;       /* Номер, часть. Поле 936. */

} IssueInfo;

MAGNA_API void MAGNA_CALL issue_array_destroy (Array *issues);
MAGNA_API void MAGNA_CALL issue_array_init    (Array *issues);
MAGNA_API void MAGNA_CALL issue_destroy       (IssueInfo *issue);
MAGNA_API void MAGNA_CALL issue_init          (IssueInfo *issue);

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/*=========================================================*/

#endif
