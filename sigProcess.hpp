#pragma once
#ifndef _SIGPROCESS_HPP_
#define _SIGPROCESS_HPP_


#include<cmath>
#include<cstdio>
#include<cstdlib>
#include"hmath.hpp"

namespace hsigProcess {

/*circle shift*/
void circshift(hmath::Vector v, int shift);

/*寻找第一个大于thre的数值*/
int find(hmath::Vector v, double thre, int FrontOrEndFlag);

/*pad signal*/
void pad_signal(hmath::Vector* yP, hmath::Vector x, int Npad);

/*unpad signal*/
void unpad_signal(hmath::Vector* yP, hmath::Vector x, int res, int target_sz );

/*frame the origin int signal according to frame size and frame shift size*/
/*return a double hmath::matrix,each row contains a frame*/
hmath::Matrix frameRawSignal(hmath::IntVec v, int wlen, int inc,double preEmphasiseCoefft,int enableHamWindow);


/*---------预处理----------------*/
/*zero mean a complete speech waveform nSamples long*/
void ZeroMean(hmath::IntVec data);

/*预加重,k一般取0.9-1，如果取k=0,不做任何预加重*/
void PreEmphasise(hmath::Vector s, double k);

/*-----------------计算谱中心以及子带能量---------------*/
/*计算谱中心,相对位置*/
double calBrightness(hmath::Vector fftx);

/*计算子带能量,输出子带能量占总能量的比值*/
void calSubBankE(hmath::Vector fftx, hmath::Vector subBankEnergy);

/*计算过零率，返回过零次数除以一帧的采样点的个数*/
double zeroCrossingRate(hmath::Vector s, int frameSize);

/*向量差分系数
参数分别为向量，需要差分的每帧的采样点数，帧数，每次移动的步长，差分系数和原信号的距离，0，0，是否简单差分*/
void Regress(double* data, int vSize, int n, int step, int offset, int delwin, int head, int tail, int simpleDiffs);

/*矩阵差分*/
void RegressMat(hmath::Matrix* m,int delwin, int regressOrder);

/*n标准化log数值*/
void NormaliseLogEnergy(double *data, int n, int step, double silFloor, double escale);

/*Z-norm，输入参数参考Regress函数*/
void ZNormalize(double *data, int vSize, int n, int step);

/* 生成Ham窗 */
hmath::Vector GenHamWindow(int frameSize);

/*在信号s上应用Ham窗*/
void Ham(hmath::Vector s, hmath::Vector hamWin, int hamWinSize);

}

#endif // !_SIGPROCESS_H_
