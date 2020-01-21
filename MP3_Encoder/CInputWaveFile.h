/*
 * CInputWaveFile.h
 *
 *  Created on: Jan 12, 2020
 *      Author: maurice
 */

#ifndef CINPUTWAVEFILE_H_
#define CINPUTWAVEFILE_H_

#include <string>
#include <vector>       // std::vector
#include <stdint.h>     // int32_t, int16_t

#include "encoderdef.h"

namespace std {

class CInputWaveFile {
public:
	CInputWaveFile();
	CInputWaveFile( const string & filename );

	virtual ~CInputWaveFile();

	void get_samples( unsigned int offset, unsigned int size, std::vector<char> & samples ) const;

    int16_t get_channels() const;
    int32_t get_samples_per_sec() const;
    int32_t get_avg_bytes_per_sec() const;
    int32_t get_data_size() const;
    int32_t get_blockAlign()const;
    int32_t get_bitspersample()const;

    string getWaveFileName();
    // alternative wave header implementation
    void WavReader (const char* fileName, const char* fileToSave);
    //Chunks
    struct chunk_t
    {
        char ID[4]; //"data" = 0x61746164
        unsigned long size;  //Chunk data bytes
    };

    struct wav_header_t
    {
        char chunkID[4]; //"RIFF" = 0x46464952
        unsigned long chunkSize; //28 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes] + sum(sizeof(chunk.id) + sizeof(chunk.size) + chunk.size)
        char format[4]; //"WAVE" = 0x45564157
        char subchunk1ID[4]; //"fmt " = 0x20746D66
        unsigned long subchunk1Size; //16 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes]
        unsigned short audioFormat;
        unsigned short numChannels;
        unsigned long sampleRate;
        unsigned long byteRate;
        unsigned short blockAlign;
        unsigned short bitsPerSample;
        //[WORD wExtraFormatBytes;]
        //[Extra format bytes]
    };

    struct RIFF
    {
        char riffID[4];     //4
        int32_t riffSIZE;   //4
        char riffFORMAT[4]; //4
    };

    struct FMTHDR
    {
        char fmtID[4];      //4
        int32_t fmtSIZE;    //4
    };

    struct FMT
    {
        int16_t wFormatTag; //2
        int16_t nChannels;  //2
        int32_t nSamplesPerSec;     //4
        int32_t nAvgBytesPerSec;    //4
        int16_t nBlockAlign;        //2
        int16_t wBitsPerSample;     //2
    };

    struct DATA
    {
        char dataID[4];     // 4 bytes
        int32_t dataSIZE;   // 4 bytes
    };
    struct FACT
    {
        int32_t samplesNumber;
        int32_t t;
    };

public:
    static const int RIFF_SIZE      = 12;
    static const int FMTHDR_SIZE    = 8;
    static const int FMT_SIZE       = 16;
    static const int DATA_SIZE      = 8;
    static const int FACT_SIZE      = 8;

    unsigned int optv;

private:
    void setFilename(const string &path);

    vector<char>   wave_;
    RIFF        riff;
    FMTHDR      fmthdr;
    FMT         fmt;
    std::vector<char>   fmt_extra_bytes_;
    FACT        fact;
    DATA        data;
    int16_t     extra_param_length_;
    vector<char>   extra_param_;
    string filename;
};

} /* namespace std */

#endif /* CINPUTWAVEFILE_H_ */
