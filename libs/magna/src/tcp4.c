/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/* ReSharper disable StringLiteralTypo */
/* ReSharper disable IdentifierTypo */
/* ReSharper disable CommentTypo */

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#ifdef MAGNA_WINDOWS

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>

static WSADATA wsaData;

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define HOSTENT struct hostent
#define closesocket(__x) close(__x)

#endif

/*=========================================================*/

#include <assert.h>

/*=========================================================*/

/**
 * \file tcp4.c
 *
 * Простая обертка над TCP/IP version 4.
 */

/*=========================================================*/

static am_bool initialized = AM_FALSE;

/**
 * Инициализация сетевой подсистемы.
 * По факту нужна только в Windows.
 *
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool tcp4_initialize (void)
{
#ifdef MAGNA_WINDOWS

    if (!initialized) {
        if (WSAStartup(0x0202, &wsaData)) {
            return AM_FALSE;
        }

        initialized = AM_TRUE;
    }

#endif

    return AM_TRUE;
}

/**
 * Подключение к указанному серверу.
 *
 * @param hostname Имя хоста либо в виде "1.2.3.4", либо в виде "myserver.com"
 * @param port Номер порта на сервере.
 * @return Дескриптор сокета либо -1.
 */
MAGNA_API am_int32 MAGNA_CALL tcp4_connect
    (
        const char *hostname,
        am_uint16 port
    )
{
    am_int32 result;
    struct sockaddr_in destinationAddress;
    unsigned long inaddr;
    HOSTENT *hostentry;

    assert (hostname != NULL);

    if (!tcp4_initialize ()) {
        return -1;
    }

    result = (am_int32) socket (AF_INET, SOCK_STREAM, 0);
    if (result < 0) {
        return result;
    }

    memset (&destinationAddress, 0, sizeof (destinationAddress));
    destinationAddress.sin_family = AF_INET;
    destinationAddress.sin_port = htons (port);
    inaddr = inet_addr (hostname);
    if (inaddr != INADDR_NONE) {
        destinationAddress.sin_addr.s_addr = inaddr;
    }
    else {
        hostentry = gethostbyname (hostname);
        if (hostentry) {
            ((unsigned long*)&destinationAddress.sin_addr)[0] =
                    ((unsigned long **)hostentry->h_addr_list)[0][0];
        }
        else {
            return -1;
        }
    }

    if (connect
        (
            result,
            (struct sockaddr*) &destinationAddress,
            sizeof(destinationAddress)
        ))
    {
        closesocket (result);

        return -1;
    }

    return result;
}

/**
 * Отключение от сервера.
 *
 * @param handle Дескриптор сокета.
 * @return Признак успешности завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL tcp4_disconnect
    (
        am_int32 handle
    )
{
    assert (handle >= 0);

    closesocket (handle);

    /* TODO: handle return code */

    return AM_TRUE;
}

/**
 * Отсылка данных на сервер.
 *
 * @param handle Дескриптор сокета.
 * @param data Указатель на данные.
 * @param dataLength Объем данных в байтах.
 * @return Число успешно отосланных байт.
 */
MAGNA_API am_ssize_t MAGNA_CALL tcp4_send
    (
        am_int32 handle,
        const am_byte *data,
        am_ssize_t dataLength
    )
{
    assert (handle >= 0);
    assert (data != NULL);

    return send (handle, data, dataLength, 0);
}

/**
 * Чтение данных из сокета.
 *
 * @param handle Дескриптор сокета.
 * @param buffer Буфер, в который должны быть помещены данные.
 * @param limit Предельный объем вычитываемых данных в байтах.
 * @return Количество прочитанных байт, отрицательное в случае ошибки.
 */
MAGNA_API am_ssize_t MAGNA_CALL tcp4_receive_with_limit
    (
        am_int32 handle,
        Buffer *buffer,
        am_ssize_t limit
    )
{
    am_ssize_t result;

    assert (handle >= 0);
    assert (buffer != NULL);

    if (!buffer_grow (buffer, limit)) {
        return -1;
    }

    result = recv (handle, buffer->ptr + buffer->position, limit, 0);
    if (result > 0) {
        buffer->position += result;
    }

    return result;
}

/**
 * Вычитывание всех доступных данных из сокета.
 *
 * @param handle Дескриптор сокета.
 * @param buffer Буфер, в который должны быть помещены данные.
 * @return Количество прочитанных байт, отрицательное в случае ошибки.
 */
MAGNA_API am_ssize_t MAGNA_CALL tcp4_receive_all
    (
        am_int32 handle,
        Buffer *buffer
    )
{
    am_ssize_t result = 0, rc;
    am_byte temp[1024];

    assert (buffer != NULL);

    while (AM_TRUE) {
        rc = recv (handle, temp, sizeof (temp), 0);
        if (rc < 0) {
            return -1;
        }

        if (rc == 0) {
            break;
        }

        if (!buffer_write (buffer, temp, rc)) {
            return -1;
        }

        result += rc;
    }

    return result;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
