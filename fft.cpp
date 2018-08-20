#include <math.h>
#include "fft.hpp"
#include"hmath.hpp"
//精度0.0001弧度  

using namespace hmath;

void hFFT::conjugate_complex(int n, hFFT::complex in[], hFFT::complex out[])
{
	int i  = 0;
	for (i = 0; i<n; i++)
	{
		out[i].imag  = -in[i].imag;
		out[i].real  = in[i].real;
	}
}

void hFFT::c_abs(hFFT::complex f[], double out[], int n)
{
	int i  = 0;
	double t;
	for (i = 0; i<n; i++)
	{
		t  = f[i].real * f[i].real  + f[i].imag * f[i].imag;
		out[i] = sqrt(t);
	}
}


void hFFT::c_plus(hFFT::complex a, hFFT::complex b, hFFT::complex *c)
{
	c->real  = a.real  + b.real;
	c->imag  = a.imag  + b.imag;
}

void hFFT::c_sub(hFFT::complex a, hFFT::complex b, hFFT::complex *c)
{
	c->real  = a.real  - b.real;
	c->imag  = a.imag  - b.imag;
}

void hFFT::c_mul(hFFT::complex a, hFFT::complex b, hFFT::complex *c)
{
	c->real  = a.real * b.real  - a.imag * b.imag;
	c->imag  = a.real * b.imag  + a.imag * b.real;
}

void hFFT::c_div(hFFT::complex a, hFFT::complex b, hFFT::complex *c)
{
	c->real  = (a.real * b.real  + a.imag * b.imag) / (b.real * b.real  + b.imag * b.imag);
	c->imag  = (a.imag * b.real  - a.real * b.imag) / (b.real * b.real  + b.imag * b.imag);
}


void hFFT::Wn_i(int n, int i, hFFT::complex *Wn, char flag)
{
	Wn->real  = cos(2 * PI*i / n);
	if (flag  == 1)
		Wn->imag  = -sin(2 * PI*i / n);
	else if(flag  == 0)
		Wn->imag  = -sin(2 * PI*i / n);
}

//傅里叶变化  
void hFFT::fft(int N, hFFT::complex f[])
{
	hFFT::complex t, wn;//中间变量  
	int i, j, k, m, n, l, r, M;
	int la, lb, lc;
	  /*----计算分解的级数M=Log2(N)----*/  
	for (i = N, M = 1; (i = i / 2) != 1; M++);
	  /*----按照倒位序重新排列原信号----*/  
	for (i = 1, j = N / 2; i <= N - 2; i++)
	{
		if (i<j)
		{
			t = f[j];
			f[j] = f[i];
			f[i] = t;
		}
		k = N / 2;
		while (k <= j)
		{
			j = j - k;
			k = k / 2;
		}
		j = j + k;
	}

	  /*----FFT算法----*/  
	for (m = 1; m <= M; m++)
	{
		la = pow(2, m); //la=2^m代表第m级每个分组所含节点数       
		lb = la / 2;    //lb代表第m级每个分组所含碟形单元数  
						//同时它也表示每个碟形单元上下节点之间的距离  
						    /*----碟形运算----*/  
		for (l = 1; l <= lb; l++)
		{
			r = (l - 1)*pow(2, M - m);
			for (n = l - 1; n<N - 1; n = n + la) //遍历每个分组，分组总数为N/la  
			{
				lc = n + lb;  //n,lc分别代表一个碟形单元的上、下节点编号       
				Wn_i(N, r, &wn, 1);//wn=Wnr  
				c_mul(f[lc], wn, &t);//t = f[lc] * wn复数运算  
				c_sub(f[n], t, &(f[lc]));//f[lc] = f[n] - f[lc] * Wnr  
				c_plus(f[n], t, &(f[n]));//f[n] = f[n] + f[lc] * Wnr  
			}
		}
	}
}

//傅里叶逆变换  
void hFFT::ifft(int N, hFFT::complex f[])
{
	int i = 0;
	conjugate_complex(N, f, f);
	fft(N, f);
	conjugate_complex(N, f, f);
	for (i = 0; i<N; i++)
	{
		f[i].imag  = (f[i].imag) / N;
		f[i].real  = (f[i].real) / N;
	}
}

void hFFT::FFT(Vector s, int invert) {
	int n = VectorSize(s); int i;
	int nn = n / 2;
	hFFT::complex* x = (hFFT::complex*)malloc(sizeof(hFFT::complex)*nn);
	for (i = 1; i <= nn; i++) {
		x[i - 1].real = s[2 * i - 1];
		x[i - 1].imag = s[2 * i];
	}
	if (invert)ifft(nn, x);
	else fft(nn, x);
	for (i = 1; i <= nn; i++) {
		s[2 * i - 1] = x[i - 1].real;
		s[2 * i] = x[i - 1].imag;
	}
	free(x);
}
