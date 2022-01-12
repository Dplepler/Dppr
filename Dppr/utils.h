#include <windows.h>
#include <stdio.h>

void createNote(const char* filen, const char* filem, BOOL showf);

static inline int popup(const char* popupt, const char* popupm) {
	return MessageBoxA(NULL, popupm, popupt, MB_YESNO);
}
