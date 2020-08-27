/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

/**
 * Строка INI-файла. Состоит из ключа
 * и (опционально) значения.
 */
typedef struct
{
    Buffer key;
    Buffer value;
} IniLine;

/**
 * Секция INI-файла. Состоит из строк
 * (см. IniLine). Имеет опциональное имя.
 */

typedef struct
{
    Buffer name;
    Array lines;
} IniSection;

/**
 * INI-файл. Состоит из секций.
 */
typedef struct
{
    Array sections;
} IniFile;
