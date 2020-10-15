// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/fields.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
   \file resource.c

   Данные о внешних ресурсах (поле 951)
   и о двоичхных ресурсах (поле 953).

   \struct ExternalResource
        \brief Данные о внешнем ресурсе (поле 951).

   \var ExternalResource::filename
        \brief Имя файла, подполе A.

   \var ExternalResource::url
        \brief URL, подполе I.

   \var ExternalResource::description
        \brief Текст для ссылки, подполе T.

   \var ExternalResource::fileCount
        \brief Количество файлов, подполе N.

   \var ExternalResource::nameTemplate
        \brief Имя шаблона первого файла, подполе M.

   \var ExternalResource::fileType
        \brief Тип внешнего файла, подполе H.

   \var ExternalResource::textbook
        \brief Признак электронного учебника, подполе K.

   \var ExternalResource::accessLevel
        \brief Уровень доступа по категориям пользователей, подполе D.
        \details Оно же -- дата начала предоставления информации.

   \var ExternalResource::lanOnly
        \brief Ресурс доступен только в ЛВС, подполе L.

   \var ExternalResource::inputDate
        \brief Дата ввода информации, подполе 1.

   \var ExternalResource::fileSize
        \brief Размер файла, подполе 2.

   \var ExternalResource::number
        \brief Номер источника копии, подполе 3.

   \var ExternalResource::lastCheck
        \brief Дата последней проверки доступности, подполе 5.

   \var ExternalResource::imageSize
        \brief Размеры изображения в пикселах, подполе 6.

   \var ExternalResource::issn
        \brief IISN, подполе X.

   \var ExternalResource::form
        \brief Форма представления, подполе B.

   \var ExternalResource::provider
        \brief Код поставщика информации, подполе F.

   \var ExternalResource::price
        \brief Цена, подполе E.

   \var ExternalResource::index
        \brief ШИфр в БД, подполе W.

   \var ExternalResource::remarks
        \brief Примечания в свободной форме, подполе P.

   \var ExternalResource::system
        \brief Электронная библиотечная система, подполе S.

   \var ExternalResource::field
        \brief Поле, из которого извлечена информация.

   \struct BinaryResource
        \brief Сведения о двоичном ресурсе, хранящемся в записи (поле 953).

   \var BinaryResource::kind
        \brief Тип двоичного ресурса, подполе A.

   \var BinaryResource::resource
        \brief Закодированный двоичный ресурс, подполе B.

   \var BinaryResource::title
        \brief Название двоичного ресурса, подполе T.

   \var BinaryResource::view
        \brief Характер просмотра, подполе P.

   \var BinaryResource::field
        \brief Поле, из которого извлечена информация.

 */

/*=========================================================*/

#define assign(__b, __f, __c) \
    buffer_assign_span ((__b), field_get_first_subfield_value ((__f), (__c)))

#define apply(__f, __c, __b) \
    field_set_subfield((__f), (__c), buffer_to_span (__b))

/*=========================================================*/

/**
 * Простая инициализация структуры.
 * Не выделяет памяти в куче.
 *
 * @param resource Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL external_resource_init
    (
        ExternalResource *resource
    )
{
    assert (resource != NULL);

    mem_clear (resource, sizeof (*resource));
}

/**
 * Освобождение памяти, занятой структурой.
 *
 * @param resource Указатель на освобождаемую структуру.
 */
MAGNA_API void MAGNA_CALL external_resource_destroy
    (
        ExternalResource *resource
    )
{
    assert (resource != NULL);

    buffer_destroy (&resource->filename);
    buffer_destroy (&resource->url);
    buffer_destroy (&resource->description);
    buffer_destroy (&resource->fileCount);
    buffer_destroy (&resource->nameTemplate);
    buffer_destroy (&resource->fileType);
    buffer_destroy (&resource->textbook);
    buffer_destroy (&resource->accessLevel);
    buffer_destroy (&resource->lanOnly);
    buffer_destroy (&resource->inputDate);
    buffer_destroy (&resource->fileSize);
    buffer_destroy (&resource->number);
    buffer_destroy (&resource->lastCheck);
    buffer_destroy (&resource->imageSize);
    buffer_destroy (&resource->issn);
    buffer_destroy (&resource->form);
    buffer_destroy (&resource->provider);
    buffer_destroy (&resource->price);
    buffer_destroy (&resource->index);
    buffer_destroy (&resource->remarks);
    buffer_destroy (&resource->system);
    mem_clear (resource, sizeof (*resource));
}

/**
 * Применение сведений о внешнем ресурсе к указанному полю.
 *
 * @param resource Сведения о внешнем ресурсе.
 * @param field Поле, подлежащее заполнению сведениями о внешнем ресурсе.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL external_resource_apply
    (
        const ExternalResource *resource,
        MarcField *field
    )
{
    assert (resource != NULL);
    assert (field != NULL);

    field_clear (field);
    return apply (field, 'a', &resource->filename)
           && apply (field, 'i', &resource->url)
           && apply (field, 't', &resource->description)
           && apply (field, 'n', &resource->fileCount)
           && apply (field, 'm', &resource->nameTemplate)
           && apply (field, 'h', &resource->fileType)
           && apply (field, 'k', &resource->textbook)
           && apply (field, 'd', &resource->accessLevel)
           && apply (field, 'L', &resource->lanOnly)
           && apply (field, '1', &resource->inputDate)
           && apply (field, '2', &resource->fileSize)
           && apply (field, '3', &resource->number)
           && apply (field, '5', &resource->lastCheck)
           && apply (field, '6', &resource->imageSize)
           && apply (field, 'x', &resource->issn)
           && apply (field, 'b', &resource->form)
           && apply (field, 'f', &resource->provider)
           && apply (field, 'e', &resource->price)
           && apply (field, 'w', &resource->index)
           && apply (field, 'p', &resource->remarks)
           && apply (field, 's', &resource->system);
}

/**
 * Разбор указанного поля на сведения о внешнем ресурсе.
 *
 * @param resource Сведения о внешнем ресурсе, подлежащие заполнению.
 * @param field Поле для разбора.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL external_resource_parse_field
    (
        ExternalResource *resource,
        const MarcField *field
    )
{
    assert (resource != NULL);
    assert (field != NULL);

    resource->field = (MarcField*) field;
    return assign (&resource->filename,        field, 'a')
           && assign (&resource->url,          field, 'i')
           && assign (&resource->description,  field, 't')
           && assign (&resource->fileCount,    field, 'n')
           && assign (&resource->nameTemplate, field, 'm')
           && assign (&resource->fileType,     field, 'h')
           && assign (&resource->textbook,     field, 'k')
           && assign (&resource->accessLevel,  field, 'd')
           && assign (&resource->lanOnly,      field, 'l')
           && assign (&resource->inputDate,    field, '1')
           && assign (&resource->fileSize,     field, '2')
           && assign (&resource->number,       field, '3')
           && assign (&resource->lastCheck,    field, '5')
           && assign (&resource->imageSize,    field, '6')
           && assign (&resource->issn,         field, 'x')
           && assign (&resource->form,         field, 'b')
           && assign (&resource->provider,     field, 'f')
           && assign (&resource->price,        field, 'e')
           && assign (&resource->index,        field, 'w')
           && assign (&resource->remarks,      field, 'p')
           && assign (&resource->system,       field, 's');
}

/**
 * Поиск в записи сведений о внешних ресурсах с последующим их разбором.
 *
 * @param record Запись для разбора.
 * @param resources Массив для размещения сведений о внешних ресурсах.
 * @param tag Метка поля, как правило, 951.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL external_resource_parse_record
    (
        const MarcRecord *record,
        Array *resources,
        am_uint32 tag
    )
{
    size_t index;
    const MarcField *field;
    ExternalResource *resource;

    assert (resources != NULL);
    assert (record != NULL);
    assert (tag != 0);

    for (index = 0; index < record->fields.len; ++index) {
        field = (const MarcField*) array_get (&record->fields, index);
        if (field->tag == tag) {
            resource = (ExternalResource*) array_emplace_back (resources);
            if (resource == NULL) {
                return AM_FALSE;
            }

            external_resource_init (resource);
            if (!external_resource_parse_field (resource, field)) {
                --resources->len;
                return AM_FALSE;
            }
        }
    }

    return AM_TRUE;
}

/*=========================================================*/

/**
 * Простая инициализация структуры.
 * Не выделяет памяти в куче.
 *
 * @param resource Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL binary_resource_init
    (
        BinaryResource *resource
    )
{
    assert (resource != NULL);

    mem_clear (resource, sizeof (*resource));
}

/**
 * Освобождение памяти, занятой структурой.
 *
 * @param resource Указатель на освобождаемую структуру.
 */
MAGNA_API void MAGNA_CALL binary_resource_destroy
    (
        BinaryResource *resource
    )
{
    assert (resource != NULL);

    buffer_destroy (&resource->kind);
    buffer_destroy (&resource->resource);
    buffer_destroy (&resource->title);
    buffer_destroy (&resource->view);
    mem_clear (resource, sizeof (*resource));
}

/**
 * Применение сведений о двоичном ресурсе к указанному полю.
 *
 * @param resource Сведения о двоичном ресурсе.
 * @param field Поле, подлежащее заполнению сведениями о двоичном ресурсе.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL binary_resource_apply
    (
        const BinaryResource *resource,
        MarcField *field
    )
{
    assert (resource != NULL);
    assert (field != NULL);

    field_clear (field);
    return apply (field, 'a', &resource->kind)
           && apply (field, 'b', &resource->resource)
           && apply (field, 't', &resource->title)
           && apply (field, 'p', &resource->view);
}

/**
 * Разбор указанного поля на сведения о двоичном ресурсе.
 *
 * @param resource Сведения о двоичном ресурсе, подлежащие заполнению.
 * @param field Поле для разбора.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL binary_resource_parse_field
    (
        BinaryResource *resource,
        const MarcField *field
    )
{
    assert (resource != NULL);
    assert (field != NULL);

    resource->field = (MarcField*) field;
    return assign (&resource->kind,        field, 'a')
           && assign (&resource->resource, field, 'b')
           && assign (&resource->title,    field, 't')
           && assign (&resource->view,     field, 'p');
}

/**
 * Поиск в записи сведений о двоичных ресурсах с последующим их разбором.
 *
 * @param record Запись для разбора.
 * @param resources Массив для размещения сведений о двоичных ресурсах.
 * @param tag Метка поля, как правило, 953.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL binary_resource_parse_record
    (
        const MarcRecord *record,
        Array *resources,
        am_uint32 tag
    )
{
    size_t index;
    const MarcField *field;
    BinaryResource *resource;

    assert (resources != NULL);
    assert (record != NULL);
    assert (tag != 0);

    for (index = 0; index < record->fields.len; ++index) {
        field = (const MarcField*) array_get (&record->fields, index);
        if (field->tag == tag) {
            resource = (BinaryResource*) array_emplace_back (resources);
            if (resource == NULL) {
                return AM_FALSE;
            }

            binary_resource_init (resource);
            if (!binary_resource_parse_field (resource, field)) {
                --resources->len;
                return AM_FALSE;
            }
        }
    }

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
