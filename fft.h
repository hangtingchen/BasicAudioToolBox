#pragma once
/*代码取自一个完全忘了的网站，和此toolbox的接口定义在sigProcess中*/

#ifndef _FFT_H
#define _FFT_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <stdlib.h>
#include <stdio.h>
#include"hmath.h"
typedef struct{
double real;
double imag;
}complex;

#define PI 3.1415926535897932384626433832795028841971

void conjugate_complex(int n, complex in[], complex out[]);
void c_plus(complex a, complex b, complex *c);//复数加  
void c_mul(complex a, complex b, complex *c);//复数乘  
void c_sub(complex a, complex b, complex *c); //复数减法  
void c_div(complex a, complex b, complex *c); //复数除法  
void fft(int N, complex f[]);//傅立叶变换 输出也存在数组f中  
void ifft(int N, complex f[]); // 傅里叶逆变换  
void c_abs(complex f[], double out[], int n);//复数数组取模  

void FFT(Vector s, int invert);
/*
When called s holds nn complex values stored in the
sequence   [ r1 , i1 , r2 , i2 , .. .. , rn , in ] where
n = VectorSize(s) DIV 2, n must be a power of 2. On exit s
holds the fft (or the inverse fft if invert == 1)
*/

#ifdef __cplusplus
}
#endif // __

#endif