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
 * \file magazine.c
 *
 * Работа с журналами/газетами/подшивками и статьями.
 */

/*=========================================================*/

/**
 * \struct ArticleInfo
 *      \brief Информация о статье из журнала/сборника.
 *      \details Структура владеет собственной памятью,
 *      для освобождения ресурсов необходимо вызвать
 *      `article_destroy`.
 *
 * \var ArticleInfo::authors
 *      \brief Массив авторов.
 *
 * \var AritcleInfo::title
 *      \brief Заглавие.
 */

/**
 * Простая инициализация структуры.
 * Не выделяет память в куче.
 *
 * @param article Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL article_init
    (
        ArticleInfo *article
    )
{
    assert (article != NULL);

    mem_clear (article, sizeof (*article));
    author_array_init (&article->authors);
    title_init (&article->title);
}

/**
 * Освобождение ресурсов, занимаемых структурой.
 *
 * @param article Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL article_destroy
    (
        ArticleInfo *article
    )
{
    assert (article != NULL);

    author_array_destroy (&article->authors);
    title_destroy (&article->title);
    mem_clear (article, sizeof (*article));
}

/**
 * Инициализация массива статей.
 *
 * @param articles Указатель на неинициализированный массив.
 */
MAGNA_API void MAGNA_CALL article_array_init
    (
        Array *articles
    )
{
    assert (articles != NULL);

    array_init (articles, sizeof (ArticleInfo));
}

MAGNA_API void MAGNA_CALL article_array_destroy
    (
        Array *articles
    )
{
    assert (articles != NULL);

    array_destroy (articles, (Liberator) article_destroy);
}

/*=========================================================*/

/**
 * \struct IssueInfo
 *      \brief Информация о выпуске журнала/газеты.
 *
 * \var IssueInfo::index
 *      \brief
 *
 * \var IssueInfo::description
 *      \brief
 *
 * \var IssueInfo::magazineCode
 *      \brief
 *
 * \var IssueInfo::year
 *      \brief
 *
 * \var IssueInfo::volume
 *      \brief
 *
 * \var IssueInfo::number
 *      \brief
 *
 * \var IssueInfo::supplement
 *      \brief Дополнение к номеру. Поле 931^c.
 *
 * \var IssueInfo::worksheet
 *      \brief Рабочий лист. Поле 920.
 *
 */

/**
 * Простая инициализация структуры.
 * Не выделяет памяти в куче.
 *
 * @param issue Указатель на неинициализированную структуру.
 */
MAGNA_API void MAGNA_CALL issue_init
    (
        IssueInfo *issue
    )
{
    assert (issue != NULL);

    /* TODO: implement */
}

/**
 * Освобождение ресурсов, занятых структурой.
 *
 * @param issue Указатель на структуру, подлежащую освобождению.
 */
MAGNA_API void MAGNA_CALL issue_destroy
    (
        IssueInfo *issue
    )
{
    assert (issue != NULL);

    /* TODO: implement */
}

/**
 * Инициализация массива выпусков журнала.
 *
 * @param issues Указатель на неинициализированный массив.
 */
MAGNA_API void MAGNA_CALL issue_array_init
    (
        Array *issues
    )
{
    assert (issues != NULL);

    array_init (issues, sizeof (IssueInfo));
}

/**
 * Освобождение ресурсов, занимаемых массивов.
 *
 * @param issues Указатель на массив, подлежащий освобождению.
 */
MAGNA_API void MAGNA_CALL issue_array_destroy
    (
        Array *issues
    )
{
    assert (issues != NULL);

    array_destroy (issues, (Liberator) issue_destroy);
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
