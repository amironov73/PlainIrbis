// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "magna/core.h"

// ReSharper disable StringLiteralTypo
// ReSharper disable IdentifierTypo
// ReSharper disable CommentTypo

/*=========================================================*/

#include "warnpush.h"

/*=========================================================*/

#ifdef MAGNA_WINDOWS

    #ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4668)
    #endif

    #define _WINSOCK_DEPRECATED_NO_WARNINGS
    #include <winsock2.h>
    #include <windows.h>

    #ifdef _MSC_VER
    #pragma warning(pop)
    #endif

    static WSADATA wsaData;

    #pragma comment (lib, "ws2_32.lib")

#elif defined(MAGNA_MSDOS)

/* TODO: implement */

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
        const am_byte *hostname,
        am_uint16 port
    )
{
#ifdef MAGNA_MSDOS

    /* TODO: implement */
    (void) hostname;
    (void) port;

    return -1;

#else

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
    inaddr = inet_addr ((const char*) hostname);
    if (inaddr != INADDR_NONE) {
        destinationAddress.sin_addr.s_addr = inaddr;
    }
    else {
        hostentry = gethostbyname ((const char*) hostname);
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

#endif
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

#ifdef MAGNA_MSDOS

    /* TODO: implement */

#else

    closesocket (handle);

    /* TODO: handle return code */

#endif

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
MAGNA_API ssize_t MAGNA_CALL tcp4_send
    (
        am_int32 handle,
        const am_byte *data,
        ssize_t dataLength
    )
{
    ssize_t result;

    assert (handle >= 0);
    assert (data != NULL);

#ifdef MAGNA_MSDOS

    /* TODO: implement */

    result = -1;

#else

    result = send (handle, (const char*) data, dataLength, 0);

#endif

    return result;
}

/**
 * Чтение данных из сокета.
 *
 * @param handle Дескриптор сокета.
 * @param buffer Буфер, в который должны быть помещены данные.
 * @param limit Предельный объем вычитываемых данных в байтах.
 * @return Количество прочитанных байт, отрицательное в случае ошибки.
 */
MAGNA_API ssize_t MAGNA_CALL tcp4_receive_with_limit
    (
        am_int32 handle,
        Buffer *buffer,
        ssize_t limit
    )
{
    ssize_t result;

    assert (handle >= 0);
    assert (buffer != NULL);

    if (!buffer_grow (buffer, buffer_length (buffer) + limit)) {
        return -1;
    }

#ifdef MAGNA_MSDOS

    /* TODO: implement */

    result = -1;

#else

    result = recv (handle, (char*) (buffer->current), limit, 0);

#endif


    if (result > 0) {
        buffer->current += result;
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
MAGNA_API ssize_t MAGNA_CALL tcp4_receive_all
    (
        am_int32 handle,
        Buffer *buffer
    )
{
    ssize_t result = 0, rc;
    am_byte temp[1024];

    assert (buffer != NULL);

    while (AM_TRUE) {
#ifdef MAGNA_MSDOS

        /* TODO: implement */
        rc = -1;

#else

        rc = recv (handle, (char*) temp, sizeof (temp), 0);

#endif

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

MAGNA_API am_bool MAGNA_CALL tcp4_send_buffer
    (
        am_int32 handle,
        const Buffer *buffer
    )
{
    ssize_t result;

    assert (buffer != NULL);

    if (buffer_is_empty (buffer)) {
        return AM_TRUE;
    }

    result = tcp4_send (handle, buffer->start, buffer_position (buffer));

    return result == ((ssize_t) buffer_position (buffer));
}

/*=========================================================*/

/* Сетевой поток (TCP) */

MAGNA_API ssize_t MAGNA_CALL tcp4_read_function
    (
        Stream *stream,
        am_byte *data,
        size_t length
    )
{
    am_int32 handle;

    assert (stream != NULL);
    assert (data != NULL);

    handle = *(am_int32*) &stream->data;
    assert (handle != -1);

#ifdef MAGNA_MSDOS

    /* TODO: implement */

    return -1;

#else

    return recv (handle, (char*) data, length, 0);

#endif
}

MAGNA_API ssize_t MAGNA_CALL tcp4_write_function
    (
        Stream *stream,
        const am_byte *data,
        size_t length
    )
{
    am_int32 handle;

    assert (stream != NULL);
    assert (data != NULL);

    handle = *(am_int32*) &stream->data;
    assert (handle != -1);

#ifdef MAGNA_MSDOS

    /* TODO: implement */

    return -1;

#else

    return send (handle, (const char*) data, length, 0);

#endif
}


MAGNA_API am_bool MAGNA_CALL tcp4_close_function
    (
        Stream *stream
    )
{
    am_int32 handle;

    assert (stream != NULL);

    handle = *(am_int32*) &stream->data;
    if (handle != -1) {
#ifdef MAGNA_MSDOS

        /* TODO: implement */

#else

        closesocket (handle);

#endif

        stream->data = NULL;
    }

    return AM_TRUE;
}

/**
 * Предполагается, что память будут записывать.
 *
 * @param stream
 * @param hostname
 * @param port
 * @return
 */
MAGNA_API am_bool MAGNA_CALL tcp4_stream_create
    (
        Stream *stream,
        const char *hostname,
        am_uint16 port
    )
{
    am_int32 handle;

    assert (stream != NULL);

    if (!stream_init (stream)) {
        return AM_FALSE;
    }

    handle = tcp4_connect (CBTEXT (hostname), port);
    if (handle == -1) {
        return AM_FALSE;
    }

    stream->data = *(void**) &handle;
    stream->readFunction  = tcp4_read_function;
    stream->writeFunction = tcp4_write_function;
    stream->closeFunction = tcp4_close_function;

    return AM_TRUE;
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/
