// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef MAGNA_FIELDS_H
#define MAGNA_FIELDS_H

#include "magna/irbis.h"

/*=========================================================*/

/* Адрес читателя */

typedef struct
{
    Buffer postcode; //< Почтовый индекс, подполе A.
    Buffer country; //< Страна/республика, подполе B.
    Buffer city; //< Город, подполе C.
    Buffer street; //< Улица, подполе D.
    Buffer building; //< Номер дома, подполе E.
    Buffer entrance; //< Номер подъезда, подполе G.
    Buffer apartment; //< Номер квартиры, подпле H.
    Buffer additional; //< Дополнительные данные, подполе F.
    am_mix userData; //< Произвольные пользовательские данные.
    MarcField *field; //< Поле, из которого извлечены данные об адресе.

} Address;

MAGNA_API am_bool MAGNA_CALL address_apply       (const Address *address, MarcField *field);
MAGNA_API void    MAGNA_CALL address_destroy     (Address *address);
MAGNA_API void    MAGNA_CALL address_init        (Address *address);
MAGNA_API am_bool MAGNA_CALL address_parse_field (Address *address, const MarcField *field);

/*=========================================================*/

/* Индивидуальный автор */

typedef struct
{
    Buffer familyName; //< Фамилия, подполе A.
    Buffer initials; //< Инициалы (сокращение), подполе B.
    Buffer fullName; //< Расширение инициалов (имя и отчество), подполе G.
    Buffer canBeInverted; //< Инвертирование имени недопустимо? Подполе 9.
    Buffer postfix; //< Неотъемлемая часть имени (отец, сын, младший, старший и т. п.), подполе 1.
    Buffer appendix; //< Дополнения к именам кроме дат (род деятельности, звание, титул и т. д.), подполе C.
    Buffer number; //< Династический номер (римские цифры), подполе D.
    Buffer dates; //< Даты жизни, подполе F.
    Buffer variant; //< Разночтение фамилии, подполе R.
    Buffer workplace; //< Место работы автора, подполе P.
    MarcField *field; //< Поле, из которого была извлечена информация.

} Author;

MAGNA_API am_bool MAGNA_CALL author_apply        (const Author *author, MarcField *field);
MAGNA_API void    MAGNA_CALL author_destroy      (Author *author);
MAGNA_API void    MAGNA_CALL author_init         (Author *author);
MAGNA_API am_bool MAGNA_CALL author_parse_field  (Author *author, const MarcField *field);
MAGNA_API am_bool MAGNA_CALL author_parse_record (const MarcRecord *record, am_uint32 tag, Array *authors);

/*=========================================================*/

/* Кодированная информация о книге/журнале */

typedef struct
{
    Buffer type; //< Тип документа, подполе T.
    Buffer kind; //< Вид документа, подполе B.
    Buffer character [6]; //< Характер документа, подполя C, 2, 3, 4, 5, 6.
    Buffer purpose [3]; //< Код целевого назначения, подполя X, Y, 9.
    Buffer restrictions; //< Возрастные ограничения, подполе Z.
    MarcField *field; //< Поле, из которого была извлечена информация.

} Codes;

MAGNA_API am_bool MAGNA_CALL codes_apply       (const Codes *codes, MarcField *field);
MAGNA_API void    MAGNA_CALL codes_destroy     (Codes *codes);
MAGNA_API void    MAGNA_CALL codes_init        (Codes *codes);
MAGNA_API am_bool MAGNA_CALL codes_parse_field (Codes *codes, const MarcField *field);

/*=========================================================*/

/* Коллективный автор */

typedef struct
{
    Buffer title;
    Buffer country;
    Buffer abbreviation;
    Buffer number;
    Buffer date;
    Buffer city;
    Buffer department;
    Buffer characteristic;
    Buffer gost;
    MarcField *field;

} Collective;

MAGNA_API am_bool MAGNA_CALL collective_apply        (const Collective *collective, MarcField *field);
MAGNA_API void    MAGNA_CALL collective_destroy      (Collective *collective);
MAGNA_API void    MAGNA_CALL collective_init         (Collective *collective);
MAGNA_API am_bool MAGNA_CALL collective_parse_field  (Collective *collective, const MarcField *field);
MAGNA_API am_bool            collective_parse_record (const MarcRecord *record, Array *collectives, ...);

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

MAGNA_API am_bool MAGNA_CALL exemplar_apply        (const Exemplar *exemplar, MarcField *field);
MAGNA_API void    MAGNA_CALL exemplar_destroy      (Exemplar *exemplar);
MAGNA_API void    MAGNA_CALL exemplar_init         (Exemplar *exemplar);
MAGNA_API am_bool MAGNA_CALL exemplar_parse_field  (Exemplar *exemplar, const MarcField *field);
MAGNA_API am_bool MAGNA_CALL exemplar_parse_record (const MarcRecord *record, Array *exemplars, am_uint32 tag);

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

#endif
