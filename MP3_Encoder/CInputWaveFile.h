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

namespace std {

class CInputWaveFile {
public:
	CInputWaveFile();
	CInputWaveFile( const string & filename );

	virtual ~CInputWaveFile();

    int16_t get_channels() const;
    int32_t get_samples_per_sec() const;
    int32_t get_avg_bytes_per_sec() const;
    int32_t get_data_size() const;

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

    static const int optv = 2;

private:

   // int optv;
    std::vector<char>   wave_;

    RIFF        riff;
    FMTHDR      fmthdr;
    FMT         fmt;
    std::vector<char>   fmt_extra_bytes_;
    FACT        fact;
    DATA        data;
    int16_t     extra_param_length_;
    std::vector<char>   extra_param_;

};

} /* namespace std */

#endif /* CINPUTWAVEFILE_H_ */
