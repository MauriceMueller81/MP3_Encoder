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
	int openWaveFile( const string & filename );
	int parseWaveFile( const string & PathToFilename );

	virtual ~CInputWaveFile();

	void get_samples( unsigned int offset, unsigned int size, std::vector<char> & samples ) const;

    int16_t get_channels() const;
    int32_t get_samples_per_sec() const;
    int32_t get_avg_bytes_per_sec() const;
    int32_t get_data_size() const;
    int32_t get_blockAlign()const;
    int32_t get_bitspersample()const;

    string getWaveFileName();

    //Chunks
    struct chunk_t
    {
        char ID[4]; //"data" = 0x61746164
        unsigned long size;  //Chunk data bytes
    };

    struct chunkHdr
    {
       char id[4];
       unsigned int size;
       unsigned int pos;
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

    //unsigned int optv;

private:
    void setFilename(const string &path);

    bool isChunkID(const chunkHdr &c, char id1, char id2, char id3, char id4);
    void readHeaderIDandSize(ifstream &f, chunkHdr &c, chunkHdr *parent);
    void skip(ifstream &f, streamsize size, chunkHdr *parent);
    void readHeaderToBuffer(ifstream &f, void *buffer, streamsize size, chunkHdr *parent);

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
