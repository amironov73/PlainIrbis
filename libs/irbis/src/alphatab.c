// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/irbis.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 * \file alphatab.c
 *
 *  Таблица алфавитных символов используется системой ИРБИС
 *  при разбиении текста на слова и представляет собой список
 *  кодов символов, которые считаются алфавитными.
 *  Таблица реализована в виде текстового файла.
 *
 *  Местонахождение и имя файла по умолчанию:
 *  `<IRBIS_SERVER_ROOT>\ISISACW.TAB`.
 *
 *  Местонахождение и имя файла определяется значением
 *  параметра ACTABPATH в конфигурационном файле
 *  ИРБИС и может быть изменено.
 *
 *  Стандартное содержимое
 *
 *  ```
 *  038 064 065 066 067 068 069 070 071 072 073 074 075 076 077 078 079 080 081 082 083 084 085 086 087 088 089 090 097 098 099 100
 *  101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 128 129 130 131 132 133 134 135 136 137
 *  138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169
 *  170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201
 *  202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233
 *  234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255
 *  ```
 */

/*=========================================================*/

/* Символы по умолчанию */
static am_byte defaultCharacters[] =
{
        38,  64,  65,  66,  67,  68,  69,  70,  71,  72,
        73,  74,  75,  76,  77,  78,  79,  80,  81,  82,
        83,  84,  85,  86,  87,  88,  89,  90,  97,  98,
        99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
        109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
        119, 120, 121, 122, 128, 129, 130, 131, 132, 133,
        134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
        144, 145, 146, 147, 148, 149, 150, 151, 152, 153,
        154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
        164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
        174, 175, 176, 177, 178, 179, 180, 181, 182, 183,
        184, 185, 186, 187, 188, 189, 190, 191, 192, 193,
        194, 195, 196, 197, 198, 199, 200, 201, 202, 203,
        204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
        214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
        224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
        234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
        244, 245, 246, 247, 248, 249, 250, 251, 252, 253,
        254, 255
};

/* Таблица по умолчанию */
static AlphabetTable defaultTable =
{
    defaultCharacters,
    sizeof (defaultCharacters)
};

/*=========================================================*/

/**
 * Инициализация таблицы символов.
 *
 * @param table Указатель на неинициализированную структуру.
 * @param characters Указатель на символы.
 * @param charCount Количество символов (от 1 до 256 включительно).
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL alpha_create
    (
        AlphabetTable *table,
        const am_byte *characters,
        size_t charCount
    )
{
    assert (table != NULL);
    assert (characters != NULL);
    assert ((charCount != 0) && (charCount <= 256));

    table->charCount = charCount;
    table->characters = mem_alloc (charCount);
    if (table->characters == NULL) {
        return AM_FALSE;
    }

    mem_copy (table->characters, characters, charCount);

    return AM_TRUE;
}

/**
 * Освобождение памяти, занятой таблицей.
 *
 * @param table Таблица, подлежащая освобождению.
 */
MAGNA_API void MAGNA_CALL alpha_destroy
    (
        AlphabetTable *table
    )
{
    assert (table != NULL);

    if (table != &defaultTable) {
        mem_free (table->characters);
    }
}

/**
 * Получение таблицы по умолчанию (встроенной в библиотеку).
 * Таблица по умолчанию не может быть освобождена
 * или модифицирована.
 *
 * @return Указатель на встроенную таблицу.
 */
MAGNA_API const AlphabetTable* alpha_default (void)
{
    return &defaultTable;
}

/**
 * Проверка, является ли указанный символ буквой.
 *
 * @param table Таблица.
 * @param character Проверяемый символ.
 * @return Результат проверки.
 */
MAGNA_API am_bool MAGNA_CALL alpha_is_good
    (
        const AlphabetTable *table,
        am_byte character
    )
{
    const am_byte *ptr, *end;

    assert (table != NULL);

    /* TODO: переделать на bsearch */
    end = (ptr = table->characters) + table->charCount;
    while (ptr < end) {
        if (*ptr == character) {
            return AM_TRUE;
        }

        ++ptr;
    }

    return AM_FALSE;
}

/**
 * Разбор текста.
 *
 * @param table Указатель на неинициализированную структуру.
 * @param text Текст с кодами символов в десятиричной системе.
 * @return Признак успешности завершеня операции.
 */
MAGNA_API am_bool MAGNA_CALL alpha_parse
    (
        AlphabetTable *table,
        Span text
    )
{
    am_bool result = AM_FALSE;
    Buffer buffer = BUFFER_INIT;
    TextNavigator nav;
    Span number;
    am_int32 value;
    am_byte byteValue;

    assert (table != NULL);

    nav_from_span (&nav, text);
    if (!buffer_grow (&buffer, 256)) {
        /* У нас закончилась память */
        goto DONE;
    }

    while (!nav_eot (&nav)) {
        if (buffer_position (&buffer) >= 256) {
            /* Что-то таблица слишком большая! */
            goto DONE;
        }

        number = nav_extract_integer (&nav);
        if (!span_is_empty (number)) {
            value = span_to_uint32 (number);
            if (value == 0 || value >= 256) {
                /* Таких символов в однобайтной кодировке не бывает */
                goto DONE;
            }

            byteValue = (am_byte) value;
            if (!span_contains (buffer_to_span (&buffer), byteValue)) {
                /* Берём только значения, которых ещё нет в таблице */
                if (!buffer_putc(&buffer, (am_byte) value)) {
                    /* У нас закончилась память, беда! */
                    goto DONE;
                }
            }
        }
    }

    result = alpha_create (table, buffer.start, buffer_position (&buffer));

    DONE:
    buffer_destroy (&buffer);

    return result;
}

/**
 * Обрезает неалфавитные символы в начале и в конце фрагмента.
 *
 * @param table Таблица символов.
 * @param text Текст, подлежащий обрезке (может быть пустым).
 * @return Фрагмент, оставшийся после обрезки (возможно, пустой).
 */
MAGNA_API Span MAGNA_CALL alpha_trim
    (
        const AlphabetTable *table,
        Span text
    )
{
    assert (table != NULL);

    /* Обрезаем символы в начале */
    while (!span_is_empty (text)) {
        if (alpha_is_good (table, text.start [0])) {
            break;
        }

        ++text.start;
    }

    /* Обрезаем символы в конце */
    while (!span_is_empty (text)) {
        if (alpha_is_good (table, text.end [-1])) {
            break;
        }

        --text.end;
    }

    return text;
}

/**
 * Формирование текстового представления таблицы символов.
 *
 * @param table Таблица символов.
 * @param output Буфер для размещения результата.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL alpha_to_string
    (
        const AlphabetTable *table,
        Buffer *output
    )
{
    size_t index;
    am_byte temp[6];

    assert (table != NULL);
    assert (output != NULL);

    for (index = 0; index < table->charCount; ++index) {
        if ((index % 32) == 0) {
            if (!buffer_putc (output, '\n')) {
                return AM_FALSE;
            }
        }
        else if (index != 0) {
            if (!buffer_putc (output, ' ')) {
                return AM_FALSE;
            }
        }

        sprintf ((char*) temp, "%03u", (unsigned int) table->characters[index]);
        if (!buffer_puts (output, temp)) {
            return AM_FALSE;
        }
    }

    return buffer_putc (output, '\n');
}

/**
 * Разбиение текста на слова в соответствии с таблицей символов.
 *
 * @param table Таблица символов.
 * @param output Массив для помещения результатов.
 * @param text Текст для обработки.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL alpha_split
    (
        const AlphabetTable *table,
        SpanArray *output,
        Span text
    )
{
    Span accumulator;
    TextNavigator nav;
    int chr;

    assert (table != NULL);
    assert (output != NULL);

    nav_from_span (&nav, text);
    accumulator = span_init (text.start, 0);
    while (!nav_eot (&nav)) {
        chr = nav_read (&nav);
        if (alpha_is_good (table, (am_byte)chr)) {
            ++accumulator.end;
        }
        else {
            if (!span_is_empty (accumulator)) {
                if (!span_array_push_back(output, accumulator)) {
                    return AM_FALSE;
                }
            }

            accumulator.end = accumulator.start = text.start + nav.position;
        }
    }

    if (!span_is_empty (accumulator)) {
        if (!span_array_push_back (output, accumulator)) {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
