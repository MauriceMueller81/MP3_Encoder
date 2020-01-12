/*
 * CInputWaveFile.cpp
 *
 *  Created on: Jan 12, 2020
 *      Author: maurice
 */

#include "CInputWaveFile.h"

#include <fstream>          // std::ofstream

namespace std {

CInputWaveFile::CInputWaveFile() {
	extra_param_length_ = 0;
}
CInputWaveFile::CInputWaveFile( const string & filename )
{
    fmt.wFormatTag      = 0;
    extra_param_length_ = 0;
    fact.samplesNumber  = -1;

    if(optv >1)
    	printf("extracting wave header information of file %s\n", filename.c_str());

    ifstream file( filename.c_str(), std::ios_base::binary | std::ios_base::in );

    if( file.is_open() == false )
    {
    	// ToDO : use exceptions?
        //throw std::runtime_error( strerror( errno ) );
    }

    //file.read( reinterpret_cast<char*>( &riff ), RIFF_SIZE );
    file.read( (char*)( &riff ), RIFF_SIZE );
    if(optv > 1)
    	printf("riffID:  %s size: %d Format: %s\n", riff.riffID, riff.riffSIZE, riff.riffFORMAT );

    //file.read( reinterpret_cast<char*>( &fmthdr ), FMTHDR_SIZE );
    file.read( (char*)( &fmthdr ), FMTHDR_SIZE );
    if(optv > 1)
    	printf("fmtID:  %s size: %d \n", fmthdr.fmtID, fmthdr.fmtSIZE );
    //file.read( reinterpret_cast<char*>( &fmt ), FMT_SIZE );
    file.read( (char*)( &fmt ), FMT_SIZE );
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

CInputWaveFile::~CInputWaveFile() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
