#include "utils.h"

void printError() {

	char* messageBuffer = NULL;

	DWORD error = GetLastError();

	if (!error) {
		popup("No errors!", "Nice");
	}
	else {
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		popup(messageBuffer, messageBuffer);
	}
}

void createNote(const char* filen, const char* filem, BOOL showf) {

	HANDLE note = CreateFileA(filen, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD wb;

	WriteFile(note, filem, strlen(filem), &wb, NULL);
	CloseHandle(note);

	if (showf) ShellExecuteA(NULL, NULL, "notepad", ".\\note.txt", NULL, SW_SHOWDEFAULT);
}
//
//void delay(unsigned int millisecs)
//{
//	// Storing start time
//	clock_t start_time = clock();
//
//	// looping till required time is not achieved
//	while (clock() < start_time + millisecs);
//}
//
//void freezeCursor(clock_t duration) {
//
//	POINT pos;
//	GetCursorPos(&pos);
//
//	clock_t start = clock();
//	while ((clock() - start) / CLOCKS_PER_SEC < duration) {
//		SetCursorPos(pos.x, pos.y);
//		Sleep(5);
//	}
//}
//
//DWORD WINAPI FlashLEDs(LPVOID p) {
//
//
//	while (TRUE) {
//
//		/* Press CAPS key */
//		INPUT inputs[2];
//		inputs[0].type = INPUT_KEYBOARD;
//		inputs[0].ki.wVk = VK_CAPITAL;
//		inputs[0].ki.dwFlags = 0;
//
//		SendInput(1, inputs, sizeof(INPUT));
//
//		/* Release CAPS key */
//		inputs[0].type = INPUT_KEYBOARD;
//		inputs[0].ki.wVk = VK_CAPITAL;
//		inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
//
//		SendInput(1, inputs, sizeof(INPUT));
//
//		Sleep(1000);	// Delay 1 second
//	}
//
//	return 0;
//}
//
DWORD WINAPI blink(LPVOID p) {

	HWND hwnd;
	HDC hdc;
	RECT rekt;

	for (;;) {

		hwnd = GetDesktopWindow();
		hdc = GetWindowDC(hwnd);
		GetWindowRect(hwnd, &rekt);
		BitBlt(hdc, 0, 0, rekt.right - rekt.left, rekt.bottom - rekt.top, hdc, 0, 0, NOTSRCCOPY);
		ReleaseDC(hwnd, hdc);

		Sleep(750);
	}
}

LPWSTR getImagNameW(LPWSTR path) {

	LPWSTR imagen = NULL;
	size_t sizeI = lstrlenW(path);
	size_t sizeN = 0;

	for (int i = sizeI; path[i] != L'/' && path[i] != L'\\' && i >= 0; i--) {
		sizeN++;
	}

	imagen = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, ++sizeN * sizeof(WCHAR));

	for (unsigned int i = 0; i < sizeN; i++) {
		imagen[i] = path[sizeI - sizeN + i + 2];
	}

	return imagen;
}