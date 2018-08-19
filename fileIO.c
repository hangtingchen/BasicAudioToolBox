#include"fileIO.h"

void FreeBinaryFileStruct(BinaryFileStruct * bfp)
{
	bfp->lengthFrame = -1;
	bfp->numFrames = -1;
	bfp->sizeFrameInByte = -1;
	bfp->typeFlag = - 1;
	FreeMatrix(bfp->data);
	bfp->data = NULL;
}

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

BinaryFileStruct readBinaryFile(FILE * f)
{
	double dtemp = 0; float ftemp = 0;
	Matrix m = NULL; 
	//no matter the origin params is int or short int,set these params to int for they are no-negtive
	BinaryFileStruct bf = {0,0,0,0,NULL};
	int numDims = 0;
	int i = 0; int j = 0;

	//read information from 12 bytes in head
	fread(&(bf.numFrames), 4, 1, f);
	fread(&(bf.lengthFrame), 4, 1, f);
	fread(&(bf.sizeFrameInByte), 2, 1, f);
	fread(&(bf.typeFlag), 2, 1, f);

	//should change according to typeFlag
	numDims = bf.sizeFrameInByte / 4;
	m = CreateMatrix(bf.numFrames, numDims);
	//read data
	for (i = 1; i <= bf.numFrames; i++)for (j = 1; j <= numDims; j++) {
		fread(&ftemp, 4, 1, f);
		m[i][j] = (double)ftemp;
	}
	bf.data = m;

	return bf;
}

void writeBinaryFile(FILE* f, BinaryFileStruct bf)
{
	double dtemp = 0; float ftemp = 0;
	int i = 0; int j = 0;
	int numDims = bf.sizeFrameInByte / 4;

	fwrite(&(bf.numFrames), 4, 1, f);
	fwrite(&(bf.lengthFrame), 4, 1, f);
	fwrite(&(bf.sizeFrameInByte), 2, 1, f);
	fwrite(&(bf.typeFlag), 2, 1, f);
//	printf("%d\t%d\n", NumCols(bf.data), NumRows(bf.data));
	for (i = 1; i <= bf.numFrames; i++)for (j = 1; j <= numDims; j++) {
		ftemp = (float)bf.data[i][j];
		fwrite(&ftemp, 4, 1, f);
	}
}
