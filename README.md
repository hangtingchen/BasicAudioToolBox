# BasicAudioToolBox

A basic audio toolbox for signal processing and statistic models

## 我们建议使用masterCPlus分支（C++版本），master分支（C版本）可能未必能编译运行通过

## 文件
| 文件        | 主要用于         |
| ------------- |:-------------:|
| hmath.c | 基础数学库，包含了向量和矩阵的生成、操作和解析函数 |
| fft.c | 快速傅里叶变换，其和sigProcess中的FFT函数对接 |
| sigProcess.c | 基本的信号处理函数 |
| fileIO.c | 文件操作 |
| WAVE.c | 读取WAVE文件 |
| main.c | DEMO |
| a001_0_30.wav | 用于DEMO的音频文件 |

## 使用
### hmath
#### 向量的创建、复制、打印、输出、解析
```C
//VectorSize函数的输入可以用任何向量类型，包括IntVec
printf("v0\n"); 
IntVec v0 = CreateIntVec(4); 
for (int i = 1; i <= VectorSize(v0); i++)v0[i] = i; 
ShowIntVec(v0);

printf("v1\n"); 
Vector v1 = CreateVector(4); 
for (int i = 1; i <= VectorSize(v0); i++)v1[i] = (double)v0[i]; 
ShowVector(v1);
WriteVector(ftemp,v1);
printf("v2\n"); 

Vector v2 = CreateVector(4); 
CopyVector(v1, v2); 
ShowVector(v2);
WriteVectorE(ftemp,v2);

FreeIntVec(v0); FreeVector(v1); FreeVector(v2);
```
#### 矩阵的创建、复制、打印、输出、解析
```C
//NumRows和NumCols的输入可以是除了STriMat外的矩阵，包括IntMat
printf("m0\n"); 
IntMat m0 = CreateIntMat(2, 2); 
ZeroIntMat(m0); 
for (int i = 1; i <= NumRows(m0); i++)for (int j = 1; j <= NumCols(m0); j++)m0[i][j] = i + j; 
ShowIntMat(m0);

printf("m1\n"); 
Matrix m1 = CreateMatrix(2, 2); ZeroMatrix(m1); 
for (int i = 1; i <= NumRows(m1); i++)for (int j = 1; j <= NumCols(m1); j++)m1[i][j] = (double)(i + j); 
ShowMatrix(m1);
WriteMatrix(ftemp, m1);

printf("m2\n"); 
Matrix m2 = CreateMatrix(2, 2); 
ZeroMatrix(m2); 
CopyMatrix(m1, m2); 
ShowMatrix(m2);

FreeIntMat(m0); FreeMatrix(m1); FreeMatrix(m2);
```

#### 下三角矩阵和求逆
```C
printf("sm1\n"); 
STriMat sm1 = CreateSTriMat(2); 
for (int i = 1; i <= STriMatSize(sm1); i++)for (int j = 1; j <= i; j++)sm1[i][j] = i==j?(double)(i + j):(double)(i-j); 
ShowSTriMat(sm1);
WriteSTriMat(ftemp, sm1);

printf("sm2\n"); 
STriMat sm2 = CreateSTriMat(2); 
//CovInvert可将输入矩阵sm1的逆存储值矩阵sm2，并返回sm1行列式的对数值
double det=CovInvert(sm1, sm2); 
ShowSTriMat(sm2);
printf("det\t"); 
//CovDet返回矩阵行列式的对数值
printf("%f\t%f\t%f\n",det, CovDet(sm1), CovDet(sm2));
//CovInvert和CovDet输入只能为STriMat，可以用函数CopyMatToTri将Matrix转化为STriMat

FreeSTriMat(sm2); FreeMatrix(m3);
```

### FFT
```C
printf("v3\n"); 
Vector v3 = CreateVector(8); 
//fft的序列为[1,2,3,4],但要储存为[1,0,2,0,3,0,4,0]，也就是实部和虚部分开储存
for (int i = 1; i <= 4; i++) { v3[2 * i - 1] = (double)i; v3[2 * i] = 0.0; }
printf("Doing FFT\n"); 
printf("original signal\n"); XFFT xf; VectorToXFFT(&xf, v3); ShowXFFT(xf);
//FFT的输入参数0或者1，分别代表正变换和逆变换
printf("after fft\n"); FFT(v3, 0); VectorToXFFT(&xf, v3); ShowXFFT(xf);
FreeVector(v3); FreeXFFT(&xf);
```

### WAVE
```C
//load WAVE file to IntMat
FILE* f2=NULL;FILE* f=NULL;
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
```