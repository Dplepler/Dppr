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
		delay(5);
	}	
}

DWORD WINAPI FlashLEDs(LPVOID p) {

	bool switchl = TRUE;

	while (true) {

		INPUT inputs[1];
		inputs[0].type = INPUT_KEYBOARD;
		inputs[0].ki.wVk = VK_CAPITAL;
		inputs[0].ki.dwFlags = switchl ? 0 : KEYEVENTF_KEYUP;

		/*inputs[1].type = INPUT_KEYBOARD;
		inputs[1].ki.wVk = VK_F8;
		inputs[1].ki.dwFlags = switchl ? KEYEVENTF_EXTENDEDKEY : KEYEVENTF_KEYUP | KEYEVENTF_EXTENDEDKEY;*/

		SendInput(1, inputs, sizeof(INPUT) * 1);

		delay(10000);
	}
}
