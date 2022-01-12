#include <windows.h>
#include <stdio.h>

static inline int popup(const char* popupt, const char* popupm) {
	return MessageBoxA(NULL, popupm, popupt, MB_YESNO);
}

static inline void makeSearch(const char* search) {
	ShellExecuteA(NULL, "open", (LPCSTR)search, NULL, NULL, SW_SHOWDEFAULT);
}

void createNote(const char* filen, const char* filem, BOOL showf);



