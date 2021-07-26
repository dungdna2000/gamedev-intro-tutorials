#pragma once
/*
	Debug utilities
*/


#include <windows.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>

#define _W(x)  __W(x)
#define __W(x)  L##x

#define VA_PRINTS(s) {				\
		va_list argp;				\
		va_start(argp, fmt);		\
		vswprintf_s(s, fmt, argp);	\
		va_end(argp);				\
}

//
// Print formatted string (similar to printf) to debug console
//
void DebugOut(wchar_t *fmt, ...);

//
// Print formatted string (similar to printf) to a target window
// This function is very useful to view live value of variables
// To set target window, use SetDebugWindow
//
void DebugOutTitle(wchar_t *fmt, ...);

//
// Set target window for DebugOutTitle
//
void SetDebugWindow(HWND hwnd);