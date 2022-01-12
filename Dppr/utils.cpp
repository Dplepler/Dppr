#include "utils.h"


void createNote(const char* filen, const char* filem, BOOL showf) {

	HANDLE note = CreateFileA(filen, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD wb;

	WriteFile(note, filem, strlen(filem), &wb, NULL);
	CloseHandle(note);

	if (showf) ShellExecuteA(NULL, NULL, "notepad", ".\\note.txt", NULL, SW_SHOWDEFAULT);
}