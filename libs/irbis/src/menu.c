/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

/**
 * Пара строк в MNU-файле.
 */
typedef struct
{
    Buffer code;
    Buffer comment;
} MenuEntry;

/**
 * MNU-файл. состоит из пар строк (см. MenuEntry).
 */
typedef struct
{
    Array entries;
} MenuFile;
