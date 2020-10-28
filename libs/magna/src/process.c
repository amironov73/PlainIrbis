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

#include <windows.h>

#endif

#ifdef MAGNA_UNIX

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#endif

#include <assert.h>

/*=========================================================*/

typedef struct
{
    char *fileName;
    char *arguments;
    char **environment;
    char *directory;
    am_bool noWindow;
} ProcessStartInfo;

/**
 * Инициализация структуры.
 *
 * @param startInfo Указатель на неиницализированную структуру.
 */
MAGNA_API void MAGNA_CALL process_init
    (
        ProcessStartInfo *startInfo
    )
{
    assert (startInfo != NULL);

    mem_clear (startInfo, sizeof (ProcessStartInfo));
}

/**
 * Запуск процесса.
 *
 * @param startInfo Информация, необходимая для запуска.
 * @return Описатель процесса либо `AM_BAD_HANDLE`.
 */
MAGNA_API am_handle MAGNA_CALL process_start
    (
        ProcessStartInfo *startInfo
    )
{
#ifdef MAGNA_WINDOWS

    LPSTR lpApplicationName;
    LPSTR lpCommandLine;
    LPSECURITY_ATTRIBUTES lpProcessAttributes = NULL;
    LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
    BOOL bInheritHandles = FALSE;
    DWORD dwCreationFlags = 0;
    LPVOID lpEnvironment = NULL;
    LPCSTR lpCurrentDirectory = startInfo->directory;
    LPSTARTUPINFOA lpStartupInfo = NULL;
    PROCESS_INFORMATION processInformation;
    BOOL rc;

    assert (startInfo != NULL);

    mem_clear (&processInformation, sizeof (processInformation));
    lpApplicationName = startInfo->fileName;
    lpCommandLine = startInfo->arguments;
    if (startInfo->noWindow) {
        dwCreationFlags |= CREATE_NO_WINDOW;
    }

    /* TODO: handle startInfo->environment */

    rc = CreateProcessA
        (
            lpApplicationName,
            lpCommandLine,
            lpProcessAttributes,
            lpThreadAttributes,
            bInheritHandles,
            dwCreationFlags,
            lpEnvironment,
            lpCurrentDirectory,
            lpStartupInfo,
            &processInformation
        );
    if (!rc) {
        return handle_from_pointer (AM_BAD_HANDLE);
    }

    return handle_from_pointer (processInformation.hProcess);

#elif defined(MAGNA_UNIX)

    extern char **environ;
    pid_t result;
    char *argv[3];
    char **env = environ;

    assert (startInfo != NULL);

    result = fork ();
    if (result == -1) {
        return handle_from_value (AM_BAD_HANDLE);
    }

    if (result == 0) {
        /* Мы в потомке */
        argv[0] = startInfo->fileName;
        argv[1] = startInfo->arguments;
        argv[2] = NULL;

        execve (startInfo->fileName, argv, env);

        exit (1);
    }

    return handle_from_value (result);

#else

    assert (startInfo != NULL);

    return handle_from_value (AM_BAD_HANDLE);

#endif
}

/**
 * Ожидание завершения процесса.
 *
 * @param process Процесс.
 * @param seconds Сколько секунд ждать. &lt=0 означает "ждать вечно".
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL process_wait
    (
        am_handle process,
        int seconds
    )
{
#ifdef MAGNA_WINDOWS

    DWORD milliseconds = INFINITE;

    if (seconds > 0) {
        milliseconds = seconds * 1000;
    }

    return WaitForSingleObject (process.pointer, milliseconds);

#elif defined(MAGNA_UNIX)

    pid_t rc;

    (void)seconds;

    assert (handle_is_good (process));

    rc = waitpid (process.value, NULL, 0);

    return (rc != -1);

#else

    return AM_FALSE;

#endif
}

/**
 * Немедленное завершение процесса.
 *
 * @param process Процесс.
 * @return Признак успешного завершения операции.
 */
MAGNA_API am_bool MAGNA_CALL process_terminate
    (
        am_handle process
    )
{
#ifdef MAGNA_WINDOWS

    assert (handle_is_good (process));

    return TerminateProcess (process.pointer, 0);

#elif defined(MAGNA_UNIX)

    int rc;

    assert (handle_is_good (process));

    rc = kill (process.value, SIGKILL);

    return rc == 0;

#else

    return AM_FALSE;

#endif
}

/*=========================================================*/

#include "warnpop.h"

/*=========================================================*/

