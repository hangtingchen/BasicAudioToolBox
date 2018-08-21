#pragma once
#ifndef  _FILEIO_HPP_
#define _FILEIO_HPP_


#include<cstdio>
#include"hmath.hpp"

namespace hfileIO {

/*A struct for saving binary file with 12 bytes in head*/
typedef struct
{
	int numFrames;
	int lengthFrame;
	short sizeFrameInByte;
	short typeFlag;
	hmath::Matrix data;
}BinaryFileStruct;

/*释放二进制文件*/
void FreeBinaryFileStruct(BinaryFileStruct* bfp);

/*一个计算文件行数的函数，计算结束后，指针返回文件头处*/
int numRowInFile(FILE* f);

/*Read binary file with 12 bytes in head*/
BinaryFileStruct readBinaryFile(FILE* f);

/*Write binary file with 12 byte in head */
void writeBinaryFile(FILE* f, BinaryFileStruct bf, hmath::Vector optionVec=nullptr);

}

#endif // ! _FILEIO_H
