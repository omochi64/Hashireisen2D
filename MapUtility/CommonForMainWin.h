
#pragma once

#ifdef MAIN_WIN_DEFINE
#define EXTERN_MAIN_WIN	/*extern*/
#else
#define EXTERN_MAIN_WIN extern
#endif

#include <windows.h>

EXTERN_MAIN_WIN HWND			g_hDialog;
