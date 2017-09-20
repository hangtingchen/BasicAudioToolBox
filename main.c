#include<stdio.h>
#include<stdlib.h>
#include "hmath.h"
#include "WAVE.h"
#include"sigProcess.h"

//this is an example to use WAVE.c
int main() {
	FILE* ftemp = fopen("test.dat","w");

	//HMATH
	//Vector
	printf("Testing the hmath\n");
	printf("v0\n"); IntVec v0 = CreateIntVec(4); for (int i = 1; i <= VectorSize(v0); i++)v0[i] = i; ShowIntVec(v0);
	printf("v1\n"); Vector v1 = CreateVector(4); for (int i = 1; i <= VectorSize(v0); i++)v1[i] = (double)v0[i]; ShowVector(v1);
	printf("v2\n"); Vector v2 = CreateVector(4); CopyVector(v1, v2); ShowVector(v2);
	WriteVector(ftemp,v1); WriteVectorE(ftemp,v2);
	//Matrix
	printf("m0\n"); IntMat m0 = CreateIntMat(2, 2); ZeroIntMat(m0); for (int i = 1; i <= NumRows(m0); i++)for (int j = 1; j <= NumCols(m0); j++)m0[i][j] = i + j; ShowIntMat(m0);
	printf("m1\n"); Matrix m1 = CreateMatrix(2, 2); ZeroMatrix(m1); for (int i = 1; i <= NumRows(m1); i++)for (int j = 1; j <= NumCols(m1); j++)m1[i][j] = (double)(i + j); ShowMatrix(m1);
	printf("m2\n"); Matrix m2 = CreateMatrix(2, 2); ZeroMatrix(m2); CopyMatrix(m1, m2); ShowMatrix(m2);
	WriteMatrix(ftemp, m1);
	//STriMatrix
	printf("sm1\n"); STriMat sm1 = CreateSTriMat(2); for (int i = 1; i <= STriMatSize(sm1); i++)for (int j = 1; j <= i; j++)sm1[i][j] = i==j?(double)(i + j):(double)(i-j); ShowSTriMat(sm1);
	WriteSTriMat(ftemp, sm1);
	//inverse matrix and reshape
	printf("sm2\n"); STriMat sm2 = CreateSTriMat(2); double det=CovInvert(sm1, sm2); ShowSTriMat(sm2);
	printf("det\t"); printf("%f\t%f\t%f\n",det, CovDet(sm1), CovDet(sm2));
	printf("m3\n"); Matrix m3 = NULL; reshape(&m3, v2, 2, 2, 1); ShowMatrix(m3);
	//free all
	FreeIntVec(v0); FreeVector(v1); FreeVector(v2); FreeIntMat(m0); FreeMatrix(m1); FreeMatrix(m2); FreeSTriMat(sm1); FreeSTriMat(sm2); FreeMatrix(m3);
	fclose(ftemp);
	

	//FFT
	printf("v3\n"); Vector v3 = CreateVector(8); for (int i = 1; i <= 4; i++) { v3[2 * i - 1] = (double)i; v3[2 * i] = 0.0; }
	printf("Doing FFT\n"); 
	printf("original signal\n"); XFFT xf; VectorToXFFT(&xf, v3); ShowXFFT(xf);
	printf("after fft\n"); FFT(v3, 0); VectorToXFFT(&xf, v3); ShowXFFT(xf);
	FreeVector(v3); FreeXFFT(&xf);


	//WAVE
	FILE* f2=NULL;FILE* f=NULL;
	//load WAVE file to INTMATRIX
	WAVE_t w = initWAVE_t();
	//be sure to open the WAVE file in mode "rb"
	f = fopen("a001_0_30.wav", "rb");
	loadWAVEFile(&w, f);
	print_WAVE(w);
	fclose(f);
	//write INTMATRIX to the file
	f2 = fopen("matrix1.dat", "w");
	WriteIntMat(f2, w.DATA.data);
	fclose(f2);
	free_WAVE(&w);

	system("pause");
	return 0;
}