#include "WAVE.h"



WAVE_t initWAVE_t()
{
	WAVE_t w={{ '\x0',-1,'\x0' },{ '\x0',-1,-1,-1,-1,-1,-1,-1 },{ '\x0',-1,NULL },{ -1,-1,-1,-1,-1 },0};
	union {
		uint32_t i;
		char c[4];
	} e = { 0x10000000 };
	if (e.c[0])w.myEndian = 1;
	return w;
}

//warning:reead only non-negtive int
long int readWAVE(FILE * f, size_t sizeInByte, int EndianFlag)
{
	char* p = NULL; int i = 0; long int x = 0;
	if (sizeInByte > sizeof(long int)) { printf("Overflow when loading file\n"); exit(-1); }
	p = (char*)malloc(sizeof(char)*sizeInByte);
	fread(p, sizeInByte, 1, f);
	if (!EndianFlag)for (i = sizeInByte - 1; i <= 0; i--) { x = x * 256 + p[i]; }
	else for (i = 0; i < (int)sizeInByte; i++) { x = x * 256 + p[i]; }
	free(p);
	return x;
}

void loadWAVEFile(WAVE_t * w, FILE * f)
{
	int numChannels = 0, numSamples = 0, i = 0, j = 0, t = 0, sign = 1; unsigned char* p = NULL; unsigned char p_temp; int int_temp = 0;
	if (w->myEndian)printf("warning: your system may be big-endian,use loadWAVEFile2 instead.\n");
	//RIFF chunk
	fread(w->RIFF.ckID, 4, 1, f);
	fread(&w->RIFF.cksize, sizeof(uint32_t), 1, f);
	fread(w->RIFF.WAVEID, 4, 1, f);
	//fmt chunk
	fread(w->fmt.ckID, 4, 1, f);
	fread(&w->fmt.cksize, sizeof(uint32_t), 1, f);
	fread(&w->fmt.wFormatTag, sizeof(uint16_t), 1, f);
	fread(&w->fmt.nChannels, sizeof(uint16_t), 1, f);
	fread(&w->fmt.nSamplesPerSec, sizeof(uint32_t), 1, f);
	fread(&w->fmt.nAvgBytesPerSec, sizeof(uint32_t), 1, f);
	fread(&w->fmt.nBlockAlign, sizeof(uint16_t), 1, f);
	fread(&w->fmt.wBitsPerSample, sizeof(uint16_t), 1, f);
	//data chunk
	fread(w->DATA.ckID, 4, 1, f);
	fread(&w->DATA.cksize, sizeof(uint32_t), 1, f);
	//WAVEParams cal
	numChannels = w->WAVEParams.numChannels = (int)w->fmt.nChannels;
	w->WAVEParams.sampleRate = (int)w->fmt.nSamplesPerSec;
	w->WAVEParams.containerLengthInByte = (int)w->fmt.nAvgBytesPerSec / (w->WAVEParams.sampleRate*w->WAVEParams.numChannels);
	w->WAVEParams.sampleLengthInByte = (int)w->fmt.wBitsPerSample / 8 ;
	numSamples = w->WAVEParams.numSamples = (int)w->DATA.cksize / (w->WAVEParams.containerLengthInByte*w->WAVEParams.numChannels);
	//load raw data
	if (sizeof(int) < w->WAVEParams.sampleLengthInByte) { printf("error: int type is not enough to store the data;you should change the data type\n"); exit(-1); }
	w->DATA.data = CreateIntMat(numChannels, numSamples);
	ZeroIntMat(w->DATA.data);
	p = (unsigned char*)malloc(sizeof(unsigned char)*w->WAVEParams.containerLengthInByte);
	for (i = 1; i <= numSamples; i++)
		for (j = 1; j <= numChannels; j++) {
			fread(p, w->WAVEParams.containerLengthInByte, 1, f);
			p_temp = p[w->WAVEParams.sampleLengthInByte - 1];
			if (p_temp >> 7)sign = -1; else sign = 1;
			if (sign == -1) { for (t = 0; t < w->WAVEParams.sampleLengthInByte; t++)p[t] = ~p[t];}
			int_temp = 0; memcpy(&int_temp, p, w->WAVEParams.sampleLengthInByte);
			if (sign == -1)int_temp += 1;
	//		for (t = w->WAVEParams.sampleLengthInByte - 1; t >= 0; t--) w->DATA.data[j][i] = (int)(w->DATA.data[j][i] * 256 + (int)(unsigned int)p[t]);
			w->DATA.data[j][i] = sign*int_temp;
	}
	free(p);
}

//this function for both little and big endian is not finished. 
//Most systems are little endian. When WAVE_t.myEndian==0,feel free to use loadWAVEFile.
void loadWAVEFile2(WAVE_t * w, FILE * f)
{
	//RIFF chunk
	fread(w->RIFF.ckID, 4, 1, f);
	w->RIFF.cksize = (uint32_t)readWAVE(f, sizeof(uint32_t), 0);
	fread(w->RIFF.WAVEID, 4, 1, f);
	//fmt chunk
	fread(w->fmt.ckID, 4, 1, f);
	w->fmt.cksize = (uint32_t)readWAVE(f, sizeof(uint32_t), 0);
	w->fmt.wFormatTag = (uint16_t)readWAVE(f, sizeof(uint16_t), 0);
	w->fmt.nChannels = (uint16_t)readWAVE(f, sizeof(uint16_t), 0);
	w->fmt.nSamplesPerSec = (uint32_t)readWAVE(f, sizeof(uint32_t), 0);
	w->fmt.nAvgBytesPerSec = (uint32_t)readWAVE(f, sizeof(uint32_t), 0);
	w->fmt.nBlockAlign = (uint16_t)readWAVE(f, sizeof(uint16_t), 0);
	w->fmt.wBitsPerSample = (uint16_t)readWAVE(f, sizeof(uint16_t), 0);
	//data chunk

}

int WAVEParamsCheck(WAVEParams_t w1, WAVEParams_t w2)
{
	if (w1.containerLengthInByte == w2.containerLengthInByte&&w1.numChannels == w2.numChannels&&w1.numSamples == w2.numSamples&&w1.sampleLengthInByte == w2.sampleLengthInByte&&w1.sampleRate == w2.sampleRate)return 1;
	else return 0;
}

void print_WAVE(WAVE_t w)
{
	printf("Sample Rate : %d\n", w.WAVEParams.sampleRate);
	printf("Number of channels : %d\n", w.WAVEParams.numChannels);
	printf("Each sample's size in byte : %d\n", w.WAVEParams.sampleLengthInByte);
	printf("Each container's size in byte : %d\n", w.WAVEParams.containerLengthInByte);
	printf("Number of samples : %d\n", w.WAVEParams.numSamples);
}

void free_WAVE(WAVE_t * w)
{
	FreeMatrix(w->DATA.data);
	*w = initWAVE_t();
}
