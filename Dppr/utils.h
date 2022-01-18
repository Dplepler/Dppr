#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <tchar.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#include <string.h>
#include <winioctl.h>

void printError();

static inline int popup(const char* popupt, const char* popupm) {
	return MessageBoxA(NULL, popupm, popupt, MB_YESNO);
}
//
//static inline int popupW(LPWSTR popupt, LPWSTR popupm) {
//	return MessageBoxW(NULL, popupm, popupt, MB_YESNO);
//}
//
//static inline void makeSearch(const char* search) {
//	ShellExecuteA(NULL, "open", (LPCSTR)search, NULL, NULL, SW_SHOWDEFAULT);
//}
//
//void createNote(const char* filen, const char* filem, BOOL showf);
//void delay(unsigned int millisecs);
//void freezeCursor(clock_t duration);
//
//LPWSTR getImagNameW(LPWSTR path);
//
//DWORD WINAPI FlashLEDs(LPVOID p);
//DWORD WINAPI blink(LPVOID p);


#endif