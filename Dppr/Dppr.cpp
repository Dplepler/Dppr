#include "utils.h"


int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Argument missing. For this program to run you have to pass an 'ok' argument.\n");
		return 1;
	}

	if (strcmp(argv[1], "ok")) return 1;

	//createNote(".\\note.txt", "HI HELLO THERE", TRUE);
	while (popup("lol", "Still using this computer?\n") == IDYES) { }

	return 0;
}