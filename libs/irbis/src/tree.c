/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

/**
 * Узел дерева TRE-файла.
 */
typedef struct
{
    Array children;
    Buffer value;
    int level;
} TreeNode;

/**
 * Дерево, хранящееся в TRE-файле.
 */
typedef struct
{
    Array roots;
} TreeFile;