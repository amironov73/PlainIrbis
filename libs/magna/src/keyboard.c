/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

#include "magna/core.h"

/**
 * \file keyboard.c
 *
 * Работа с клавиатурой.
 *
 */

/*=========================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4255)
#pragma warning(disable: 4820)
#endif

/*=========================================================*/

#ifdef MAGNA_WINDOWS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#endif

/*=========================================================*/

#ifdef MAGNA_WINDOWS

static void press_key
    (
        BYTE vk,
        BYTE scan
    )
{
    /* Simulate a key press */
    keybd_event
        (
            vk,
            scan,
            KEYEVENTF_EXTENDEDKEY | 0,
            0
        );

    /* Simulate a key release */
    keybd_event
        (
            vk,
            scan,
            KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
            0
        );
}

static int key_pressed
    (
        BYTE virtualKey
    )
{
    BYTE keyState[256];

    GetKeyboardState (keyState);

    return (keyState [virtualKey] & 1) != 0;
}

/**
 * Toggles the Caps Lock key off.
 */
MAGNA_API void capsoff (void)
{
    /*
     * https://support.microsoft.com/en-us/help/127190/howto-toggle-the-num-lock-caps-lock-and-scroll-lock-keys
     */

    if (key_pressed (VK_CAPITAL)) {
        press_key (VK_CAPITAL, 0x35);
    }
}

/**
 * Toggles the Caps Lock key on.
 * @return
 */
MAGNA_API void capson (void)
{
    if (!key_pressed (VK_CAPITAL)) {
        press_key (VK_CAPITAL, 0x45);
    }
}

/**
 * Clears the keyboard buffer.
 */
MAGNA_API void clearkeys (void)
{
    // TODO implement
}

/**
 * Gets a character from the keyboard from a list of valid characters.
 * @param valid
 * @param defchar
 * @return
 */
MAGNA_API int MAGNA_CALL getchf (char *valid, int defchar)
{
    return defchar;
}

/**
 * Inputs a string of a specified maximum length from the keyboard.
 * @param str
 * @param maxchars
 * @return
 */
MAGNA_API int MAGNA_CALL getns (char *str, int maxchars)
{
    return 0;
}

/**
 * Getsk a key (ASCII code/scan code) from the keyboard.
 * @return
 */
MAGNA_API unsigned MAGNA_CALL getxch (void)
{
    return 0;
}

/**
 * Determines if a key has been pressed and is waiting to be got.
 * @return
 */
MAGNA_API int kbmhit (void)
{
    return 0;
}

/**
 * Places a string of characters into internal keyboard buffer.
 * @param str
 * @return
 */
MAGNA_API int MAGNA_CALL kbputs (char *str)
{
    return 0;
}

/**
 * Returns the status of the keyboard control keys.
 * @return
 */
MAGNA_API unsigned kbstat (void)
{
    return 0;
}

/**
 * Toggles the Num Lock key off.
 */
MAGNA_API void numoff (void)
{
    /*
     * https://support.microsoft.com/en-us/help/127190/howto-toggle-the-num-lock-caps-lock-and-scroll-lock-keys
     */

    if (key_pressed (VK_NUMLOCK)) {
        press_key (VK_NUMLOCK, 0x45);
    }
}

/**
 * Toggles the Num Lock key on.
 */
MAGNA_API void numon (void)
{
    if (!key_pressed (VK_NUMLOCK)) {
        press_key (VK_NUMLOCK, 0x45);
    }
}

/**
 * Clears the keyboard buffer and halts the program until a key is pressed.
 * @return
 */
MAGNA_API int waitkey (void)
{
    return 0;
}

/**
 * Clears the keyboard buffer and halts the program until a key press,
 * or the specified time limit expires.
 * @param duration
 * @return
 */
MAGNA_API int MAGNA_CALL waitkeyt (int duration)
{
    return 0;
}

#endif
