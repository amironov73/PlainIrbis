/* This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com */

/*
 * Отключает предупреждения.
 * Включается в начале файла.
 */

/*=========================================================*/

#ifdef _MSC_VER

#pragma warning(push)
#pragma warning(disable: 4255)
#pragma warning(disable: 4820)

#if _MSC_VER >= 1910

/* MSVS 2017 */
#pragma warning(disable: 5045)
#pragma warning(disable: 5031)

#endif

#endif

/*=========================================================*/
