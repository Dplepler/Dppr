#include "utils.h"


void createNote(const char* filen, const char* filem, BOOL showf) {

	HANDLE note = CreateFileA(filen, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD wb;

	WriteFile(note, filem, strlen(filem), &wb, NULL);
	CloseHandle(note);

	if (showf) ShellExecuteA(NULL, NULL, "notepad", ".\\note.txt", NULL, SW_SHOWDEFAULT);
}

void delay(unsigned int millisecs)
{
	// Storing start time
	clock_t start_time = clock();

	// looping till required time is not achieved
	while (clock() < start_time + millisecs) { }
}

void freezeCursor(clock_t duration) {

	POINT pos;
	GetCursorPos(&pos);

	clock_t start = clock();
	while ((clock() - start) / CLOCKS_PER_SEC < duration) {
		SetCursorPos(pos.x, pos.y);
		delay(10);
	}
		
}