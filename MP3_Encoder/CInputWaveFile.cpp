/*
 * CInputWaveFile.cpp
 *
 *  Created on: Jan 12, 2020
 *      Author: maurice
 */

#include "CInputWaveFile.h"

#include <iostream>
#include <fstream>          // std::ofstream
#include <stdio.h>


//namespace std {
using namespace std;

CInputWaveFile::CInputWaveFile() {
	extra_param_length_ = 0;
}
CInputWaveFile::CInputWaveFile( const string & filename )
{
    fmt.wFormatTag      = 0;
    extra_param_length_ = 0;
    fact.samplesNumber  = -1;
    const char* wavefilename = filename.c_str();

    if(optv >1)
    	printf("extracting wave header information of file %s\n", wavefilename);


    // try alternative waveheader
    //WavReader(wavefilename, "test.wav");

    std::ifstream file( filename.c_str(), std::ios_base::binary | std::ios_base::in );
    //file.open( filename.c_str()/*wavefilename*/, std::ios_base::binary | std::ios_base::in );

    if( file.is_open() == false )
    {
    	// ToDO : use exceptions?
        //throw std::runtime_error( strerror( errno ) );
    }

    file.read( reinterpret_cast<char*>( &riff ), RIFF_SIZE );
    if(optv > 1)
    	printf("riffID:  %s size: %d Format: %s\n", riff.riffID, riff.riffSIZE, riff.riffFORMAT );

    file.read( reinterpret_cast<char*>( &fmthdr ), FMTHDR_SIZE );
    if(optv > 1)
    	printf("fmtID:  %s size: %d \n", fmthdr.fmtID, fmthdr.fmtSIZE );
    file.read( reinterpret_cast<char*>( &fmt ), FMT_SIZE );
    if(optv > 1)
    	printf("format Tag:  %d channels: %d  Samples per sec: %d avg bytes per sec: %d block align: %d bits per sample: %d\n"
    			, fmt.wFormatTag, fmt.nChannels, fmt.nSamplesPerSec, fmt.nAvgBytesPerSec, fmt.nBlockAlign, fmt.wBitsPerSample );

    /*unsigned*/int fmt_extra_bytes    = fmthdr.fmtSIZE - FMT_SIZE;

    if( fmt_extra_bytes > 0 )
    {
        fmt_extra_bytes_.resize( fmt_extra_bytes );

        file.read( & fmt_extra_bytes_[0], fmt_extra_bytes );
    }

    if( fmt.wFormatTag != 1 )
    {
        file.read( reinterpret_cast<char*>( &extra_param_length_ ), 2 ); //2 bytes
        if( extra_param_length_ > 0 )
        {
            extra_param_.resize( extra_param_length_ );
            file.read( & extra_param_[0], extra_param_length_ );
        }
    }

    file.read( reinterpret_cast<char*>( &data.dataID ), 4 );

    if( data.dataID[0] == 'f' && data.dataID[1] == 'a' && data.dataID[2] == 'c' && data.dataID[3] == 't' )
    {
        file.read( reinterpret_cast<char*>( &fact ), FACT_SIZE );
        file.read( reinterpret_cast<char*>( &data ), DATA_SIZE );
    }
    else
        file.read( reinterpret_cast<char*>( &data.dataSIZE ), 4 );

    wave_.resize( data.dataSIZE );

    file.read( & wave_[0], data.dataSIZE );
}
void CInputWaveFile::WavReader (const char* fileName, const char* fileToSave)
{
    FILE *fin = fopen(fileName, "rb");

    //Read WAV header
    wav_header_t header;

    fread(&header, sizeof(header), 1, fin);

    //Print WAV header
    printf("WAV File Header read:\n");
    printf("File Type: %s\n", header.chunkID);
    printf("File Size: %ld\n", header.chunkSize);
    printf("WAV Marker: %s\n", header.format);
    printf("Format Name: %s\n", header.subchunk1ID);
    printf("Format Length: %ld\n", header.subchunk1Size );
    printf("Format Type: %hd\n", header.audioFormat);
    printf("Number of Channels: %hd\n", header.numChannels);
    printf("Sample Rate: %ld\n", header.sampleRate);
    printf("Sample Rate * Bits/Sample * Channels / 8: %ld\n", header.byteRate);
    printf("Bits per Sample * Channels / 8.1: %hd\n", header.blockAlign);
    printf("Bits per Sample: %hd\n", header.bitsPerSample);

    //skip wExtraFormatBytes & extra format bytes
    //fseek(f, header.chunkSize - 16, SEEK_CUR);

    //Reading file
    chunk_t chunk;
    printf("id\t" "size\n");
    //go to data chunk
    while (true)
    {
        fread(&chunk, sizeof(chunk), 1, fin);
        printf("%c%c%c%c\t" "%li\n", chunk.ID[0], chunk.ID[1], chunk.ID[2], chunk.ID[3], chunk.size);
        if (*(unsigned int *)&chunk.ID == 0x61746164)
            break;
        //skip chunk data bytes
        fseek(fin, chunk.size, SEEK_CUR);
    }

    //Number of samples
    int sample_size = header.bitsPerSample / 8;
    int samples_count = chunk.size * 8 / header.bitsPerSample;
    printf("Samples count = %i\n", samples_count);

    short int *value = new short int[samples_count];
    // memset(value, 0, sizeof(short int) * samples_count);

    //Reading data
    for (int i = 0; i < samples_count; i++)
    {
        fread(&value[i], sample_size, 1, fin);
    }

    //Write data into the file
    FILE *fout = fopen(fileToSave, "w");
    for (int i = 0; i < samples_count; i++)
    {
        fprintf(fout, "%d\n", value[i]);
    }
    fclose(fin);
    fclose(fout);
}

CInputWaveFile::~CInputWaveFile() {
	// TODO Auto-generated destructor stub
}

//} /* namespace std */
