#pragma once
#ifndef  _FILEIO_H
#define _FILEIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include<stdio.h>

/*一个计算文件行数的函数，计算结束后，指针返回文件头处*/
int numRowInFile(FILE* f);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ! _FILEIO_H
