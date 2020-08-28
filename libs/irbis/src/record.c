/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/irbis.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

#include <assert.h>

/*=========================================================*/

MAGNA_API MarcRecord* MAGNA_CALL record_init
    (
        MarcRecord *record
    )
{
    assert (record != NULL);

    return record;
}
