#pragma once
#ifndef  _FILEIO_H
#define _FILEIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include<stdio.h>
#include"hmath.h"

/*A struct for saving binary file with 12 bytes in head*/
typedef struct
{
	int numFrames;
	int lengthFrame;
	short sizeFrameInByte;
	short typeFlag;
	Matrix data;
}BinaryFileStruct;

void FreeBinaryFileStruct(BinaryFileStruct* bfp);

/*一个计算文件行数的函数，计算结束后，指针返回文件头处*/
int numRowInFile(FILE* f);

/*Read binary file with 12 bytes in head*/
BinaryFileStruct readBinaryFile(FILE* f);

/*Write binary file with 12 byte in head */
void writeBinaryFile(FILE* f, BinaryFileStruct bf);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ! _FILEIO_H
