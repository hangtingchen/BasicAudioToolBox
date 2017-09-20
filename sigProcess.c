#include "sigProcess.h"

/*circle shift the signal*/
void circshift(Vector v, int shift)
{
	int i = 1; Vector v_temp = CreateVector(VectorSize(v));
	if (shift < 0)do { shift += VectorSize(v); } while (shift < 0);
	if (shift >= VectorSize(v))do { shift -= VectorSize(v); } while (shift >= VectorSize(v));
	for (i = 1; (i + shift) <= VectorSize(v); i++)v_temp[i + shift] = v[i];
	for (; i <= VectorSize(v); i++)v_temp[i + shift - VectorSize(v)] = v[i];
	CopyVector(v_temp, v);
	FreeVector(v_temp);
}

/*find the first index of abs|sample| exceeding the thre from the front or end*/
int find(Vector v, double thre, int FrontOrEndFlag)
{
	int i; int m = 0;
	if (FrontOrEndFlag == 1) {
		for (i = 1; i <= VectorSize(v); i++)if (fabs(v[i]) > thre) { m = i; break; }
		return m;
	}
	else if (FrontOrEndFlag == -1) {
		for (i = VectorSize(v); i >= 1; i--)if (fabs(v[i]) > thre) { m = i; break; }
		return m;
	}
	else
		return 0;
}

void pad_signal(Vector * yP, Vector x, int Npad)
{
	int i = 0; int j = 0;
	int orig_sz = VectorSize (x);
	int Norig = VectorSize(x);
	int end = Norig + (int)floor((double)(Npad - Norig) / 2.0);
	int end2 = (int)ceil((double)(Npad - Norig) / 2.0);
	int end3 = Norig + (int)floor((double)(Npad - Norig) / 2.0) + 1;
	IntVec ind0 = CreateIntVec(2 * Norig);
	IntVec conjugate0 = CreateIntVec(2 * Norig);
	IntVec conjugate = CreateIntVec(Npad);
	IntVec ind = CreateIntVec(Npad);
	IntVec src = CreateIntVec(end - Norig);
	IntVec dst = CreateIntVec(end - Norig);
	ZeroIntVec(ind); ZeroIntVec(conjugate);
	for (i = 1; i <= Norig; i++)ind0[i] = i;
	for (i=Norig; i >= 1; i--)ind0[2 * Norig - i + 1] = i;
	for (i = 1; i <= Norig; i++)conjugate0[i] = 0;
	for (i = Norig + 1; i <= 2 * Norig; i++)conjugate0[i] = 1;
	for (i = 1; i <= Norig; i++)ind[i] = i;
	for (i = 1; i <= VectorSize(src); i++)src[i] = (Norig + i-1) % (VectorSize(ind0)) + 1;
	for (i = 1; i <= VectorSize(dst); i++)dst[i] = Norig + i;
	for (i = 1; i <= VectorSize(src); i++)ind[dst[i]] = ind0[src[i]];
	for (i = 1; i <= VectorSize(src); i++)conjugate[dst[i]] = conjugate0[src[i]];
	FreeIntVec(src); FreeIntVec(dst);
	src = CreateIntVec(end2); dst = CreateIntVec(Npad - end3 + 1);
	for (i = 1; i <= VectorSize(src); i++) {
		if((VectorSize(ind0) - i)>=0)src[i] = ((VectorSize(ind0) - i) % (VectorSize(ind0))) + 1;
		else src[i] = ((VectorSize(ind0) - i+ VectorSize(ind0)) % (VectorSize(ind0))) + 1;
	}
	for (i = Npad, j = 1; i >= end3; i--, j++)dst[j] = i;
	for (i = 1; i <= VectorSize(src); i++)ind[dst[i]] = ind0[src[i]];
	for (i = 1; i <= VectorSize(src); i++)conjugate[dst[i]] = conjugate0[src[i]];
	*yP = CreateVector(VectorSize(ind));
	for (i = 1; i <= VectorSize(ind); i++)(*yP)[i] = x[ind[i]];
	FreeIntVec(ind0); FreeIntVec(conjugate0); FreeIntVec(conjugate); FreeIntVec(ind); FreeIntVec(src); FreeIntVec(dst);
}

void unpad_signal(Vector * yP, Vector x, int res, int target_sz)
{
	int i = 0;
	int padded_sz = VectorSize(x);
	double offset = 0;
	int offset_ds = 0;
	int target_sz_ds = 1 + floor((double)(target_sz - 1) / pow(2.0, (double)res));
	*yP = CreateVector(target_sz_ds);
	for (i = 1; i <= VectorSize(*yP); i++)(*yP)[i] = x[i];
}

/*FFT from HTK*/
/* EXPORT-> FFT: apply fft/invfft to complex s */
/*
void FFT(Vector s, int invert)
{
	int ii, jj, n, nn, limit, m, j, inc, i;
	double wx, wr, wpr, wpi, wi, theta;
	double xre, xri, x;

	n = VectorSize(s);
	nn = n / 2; j = 1;
	for (ii = 1; ii <= nn; ii++) {
		i = 2 * ii - 1;
		if (j>i) {
			xre = s[j]; xri = s[j + 1];
			s[j] = s[i];  s[j + 1] = s[i + 1];
			s[i] = xre; s[i + 1] = xri;
		}
		m = n / 2;
		while (m >= 2 && j > m) {
			j -= m; m /= 2;
		}
		j += m;
	};
	limit = 2;
	while (limit < n) {
		inc = 2 * limit; theta = 2 * pi / limit;
		if (invert) theta = -theta;
		x = sin(0.5 * theta);
		wpr = -2.0 * x * x; wpi = sin(theta);
		wr = 1.0; wi = 0.0;
		for (ii = 1; ii <= limit / 2; ii++) {
			m = 2 * ii - 1;
			for (jj = 0; jj <= (n - m) / inc; jj++) {
				i = m + jj * inc;
				j = i + limit;
				xre = wr * s[j] - wi * s[j + 1];
				xri = wr * s[j + 1] + wi * s[j];
				s[j] = s[i] - xre; s[j + 1] = s[i + 1] - xri;
				s[i] = s[i] + xre; s[i + 1] = s[i + 1] + xri;
			}
			wx = wr;
			wr = wr * wpr - wi * wpi + wr;
			wi = wi * wpr + wx * wpi + wi;
		}
		limit = inc;
	}
	if (invert) {
		for (i = 1; i <= n; i++)
			s[i] = s[i] / nn;
	}
	for (i = 3; i <= n / 2; i++) {
		if (mod(i, 2) == 0)ii = n + 4 - i;
		else ii = n + 2 - i;
		x = s[ii]; s[ii] = s[i]; s[i] = x;
	}
}
*/

void FFT(Vector s, int invert) {
	int n = VectorSize(s); int i;
	int nn = n / 2;
	complex* x = (complex*)malloc(sizeof(complex)*nn);
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

void ZeroMean(IntVec data)
{
	long i, hiClip = 0, loClip = 0;
	int *x;
	double sum = 0.0, y, mean;
	int nSamples = VectorSize(data);

	x = &data[1];
	for (i = 0; i<nSamples; i++, x++)
		sum += *x;
	mean = sum / (double)nSamples;
	x = data;
	for (i = 0; i<nSamples; i++, x++) {
		y = (double)(*x) - mean;
		if (y<-32767.0) {
			y = -32767.0; ++loClip;
		}
		if (y>32767.0) {
			y = 32767.0; ++hiClip;
		}
		*x = (int)((y>0.0) ? y + 0.5 : y - 0.5);
	}
	if (loClip>0)
		printf("ZeroMean: %d samples too -ve\n", loClip);
	if (hiClip>0)
		printf("ZeroMean: %d samples too +ve\n", hiClip);
}

double zeroCrossingRate(Vector s, int frameSize) {
	int count = 0; int i;
	for (i = 1; i < frameSize; i++)  if ((s[i] * s[i + 1]) < 0.0)count++;
	return ((double)count) / (double)(frameSize - 1);
}

/* EXPORT->PreEmphasise: pre-emphasise signal in s */
void PreEmphasise(Vector s, double k)
{
	int i;
	double preE;

	preE = k;
	if (k == 0.0)return;
	for (i = VectorSize(s); i >= 2; i--)
		s[i] -= s[i - 1] * preE;
	s[1] *= 1.0 - preE;
}

double calBrightness(Vector fftx)
{
	int i;
	double sum = 0.0;
	double te = 0.0;
	double b = 0;
	if (((int)VectorSize(fftx)) % 2 != 0)printf("something wrong in cal brightness");
	for (i = 1; i <= ((int)VectorSize(fftx)) / 2; i++) {
		sum += (fftx[2 * i - 1] * fftx[2 * i - 1] + fftx[2 * i] * fftx[2 * i])*(double)i;
		te += fftx[2 * i - 1] * fftx[2 * i - 1] + fftx[2 * i] * fftx[2 * i];
	}
	b = sum / te;
	b = b / ((double)VectorSize(fftx) / 2.0);
	return b;
}


void calSubBankE(Vector fftx, Vector subBankEnergy)
{
	int i;
	int numBank = VectorSize(subBankEnergy); int bankSize = (int)VectorSize(fftx) / (2 * numBank);
	int bankNum = 1;
	double te = 0.0;
	double sum = 0.0;
	for (i = 1; i <= (int)VectorSize(fftx) / 2; i++)te+= fftx[2 * i - 1] * fftx[2 * i - 1] + fftx[2 * i] * fftx[2 * i];
	for (i = 1; i <= (int)VectorSize(fftx) / 2; i++) {
		if (i <= bankNum*bankSize) {
			sum += fftx[2 * i - 1] * fftx[2 * i - 1] + fftx[2 * i] * fftx[2 * i];
		}
		else {
			subBankEnergy[bankNum] = sum / te;
			//printf("sum: %f\n", sum/te);
			bankNum++; sum = 0.0; i--;
		}
	}
	subBankEnergy[bankNum] = sum / te;

}

void Regress(Vector data, int vSize, int n, int step, int offset, int delwin, int head, int tail, bool simpleDiffs)
{
	double *fp, *fp1, *fp2, *back, *forw;
	double sum, sigmaT2;
	int i, t, j;

	sigmaT2 = 0.0;
	for (t = 1; t <= delwin; t++)
		sigmaT2 += t*t;
	sigmaT2 *= 2.0;
	fp = &data[1];
	for (i = 1; i <= n; i++) {
		fp1 = fp; fp2 = fp + offset;
		for (j = 1; j <= vSize; j++) {
			back = forw = fp1; sum = 0.0;
			for (t = 1; t <= delwin; t++) {
				if (head + i - t > 0)     back -= step;
				if (tail + n - i + 1 - t > 0) forw += step;
				if (!simpleDiffs) sum += t * (*forw - *back);
			}
			if (simpleDiffs)
				*fp2 = (*forw - *back) / (2 * delwin);
			else
				*fp2 = sum / sigmaT2;
			++fp1; ++fp2;
		}
		fp += step;
	}
}