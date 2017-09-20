#include"fileIO.h"

int numRowInFile(FILE* f) {
	int c; int i = 0;
	while ((c = fgetc(f)) != EOF)
	{
		if (c == '\n')
			i++;
	}
	fseek(f, 0, SEEK_SET);
	printf("the num of rows in file is %d\n", i);
	return i;
}