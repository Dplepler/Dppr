#include "utils.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	createNote(".\\note.txt", "HI HELLO THERE", TRUE);
	//while (popup("lol", "Still using this computer?\n") == IDYES) { }

	/*makeSearch("https://www.youtube.com/watch?v=T5y_OcKDadQ");
	makeSearch("https://www.google.co.ck/search?q=How+to+be+cool+for+school");*/

	//freezeCursor(10);

	return 0;
}