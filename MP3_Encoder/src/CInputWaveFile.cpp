/*
 * CInputWaveFile.cpp
 *
 *  Created on: Jan 12, 2020
 *      Author: maurice
 */


#include <iostream>
#include <fstream>          // std::ofstream
#include <stdio.h>
#include <string.h>


#include "../include/CInputWaveFile.h"
#include "../include/CEncoderLogger.h"

using namespace std;

CInputWaveFile::CInputWaveFile() {
	extra_param_length_ = 0;
}

bool CInputWaveFile::isChunkID(const chunkHdr &c, char id1, char id2, char id3, char id4)
{
    return ((c.id[0] == id1) &&
            (c.id[1] == id2) &&
            (c.id[2] == id3) &&
            (c.id[3] == id4));
}


void CInputWaveFile::readHeaderToBuffer(ifstream &f, void *buffer, streamsize size, chunkHdr *parent)
{
    if (!f.read(static_cast<char*>(buffer), size))
    {
        if (f.eof())
            throw runtime_error("Unexpected EOF while reading from file");
        throw runtime_error("Unable to read from file");
    }
    if (parent)
    	parent->pos += size;
}

void CInputWaveFile::skip(ifstream &f, streamsize size, chunkHdr *parent)
{
    if (!f.seekg(size, ios_base::cur))
        throw runtime_error("Unable to read from file");
    if (parent) parent->pos += size;
}

void CInputWaveFile::readHeaderIDandSize(ifstream &f, chunkHdr &c, chunkHdr *parent)
{
	readHeaderToBuffer(f, c.id, 4, parent);
	readHeaderToBuffer(f, &(c.size), 4, parent);
    c.pos = 0;
}
// open and parse an input filestream to a given wave audio file (path)
int CInputWaveFile::parseWaveFile( const string & PathToFilename )
{
	int iRet = 0;

    unsigned int SampleRate, ByteRate;
    unsigned short AudioFormat, NumChannels, BlockAlign, BitsPerSample, ExtraSize;

	chunkHdr riffHdr, waveHdr, chunkHdr;
    bool fmtFound = false;

    fmt.wFormatTag      = 0;
    extra_param_length_ = 0;
    fact.samplesNumber  = -1;

    // get the filename without the path and write to member
    setFilename(PathToFilename);

    if(optv > VL_LOW)
    	printf("extracting wave header information of file %s\n", PathToFilename.c_str());

    // open a input filestream
    ifstream file( PathToFilename.c_str(), ios_base::binary | ios_base::in );

    if( file.is_open() == false )
    {
    	CEncoderLogger::ErrorLog("file is not open");
    	file.close();
    	iRet = -1;
    	return iRet;
    }

    // check the RIFF header
    readHeaderIDandSize(file, riffHdr, NULL);
    if (!isChunkID(riffHdr, 'R', 'I', 'F', 'F'))
    {
    	CEncoderLogger::ErrorLog("File is not a RIFF file\n");
    	file.close();
    	iRet = -1;
    	return iRet;
    	//throw runtime_error("File is not a RIFF file");
    }

    // check the WAVE header
    readHeaderToBuffer(file, waveHdr.id, 4, &riffHdr);
    waveHdr.size = riffHdr.size - 4;
    waveHdr.pos = 0;

    if (!isChunkID(waveHdr, 'W', 'A', 'V', 'E'))
    {
    	CEncoderLogger::ErrorLog("File is not a wav file\n");
    	file.close();
    	iRet = -1;
    	return iRet;
    	//throw runtime_error("File is not a Wav file");
    }

    if(optv > VL_LOW)
    	printf("riffID:  %s size: %d Format: %s\n", riffHdr.id, riffHdr.size, waveHdr.id );


    // read WAVE chunks
    while (waveHdr.pos < waveHdr.size)
    {
        readHeaderIDandSize(file, chunkHdr, &waveHdr);
        if(optv > VL_LOW)
        	printf("Chunk:  %s size: %d \n", chunkHdr.id, chunkHdr.size);

        if (isChunkID(chunkHdr, 'f', 'm', 't', ' '))
        {
            if (fmtFound)
            {
            	CEncoderLogger::ErrorLog("More than one FMT chunk encountered\n");
            	file.close();
            	iRet = -1;
            	return iRet;
            	//throw runtime_error("More than one FMT chunk encountered");
            }
            fmtFound = true;

            readHeaderToBuffer(file, &AudioFormat, 2, &chunkHdr);
            readHeaderToBuffer(file, &NumChannels, 2, &chunkHdr);
            readHeaderToBuffer(file, &SampleRate, 4, &chunkHdr);
            readHeaderToBuffer(file, &ByteRate, 4, &chunkHdr);
            readHeaderToBuffer(file, &BlockAlign, 2, &chunkHdr);

            fmt.nAvgBytesPerSec = ByteRate;
            fmt.nBlockAlign = BlockAlign;
            fmt.nChannels = NumChannels;
            fmt.nSamplesPerSec = SampleRate;
            fmt.wFormatTag = AudioFormat;

            cout << "  Audio Format: " << AudioFormat << endl;
            cout << "  Channels: " << NumChannels << endl;
            cout << "  Sample Rate: " << SampleRate << endl;
            cout << "  Byte Rate: " << ByteRate << endl;
            cout << "  BlockAlign: " << BlockAlign << endl;

            if (chunkHdr.size >= 16)
            {
            	readHeaderToBuffer(file, &BitsPerSample, 2, &chunkHdr);
                cout << "  Bits per Sample: " << BitsPerSample << endl;
            }

            fmt.wBitsPerSample = BitsPerSample;

            if(optv > VL_LOW)
                           	printf("format Tag:  %d channels: %d  Samples per sec: %d avg bytes per sec: %d block align: %d bits per sample: %d\n"
                           			, fmt.wFormatTag, fmt.nChannels, fmt.nSamplesPerSec, fmt.nAvgBytesPerSec, fmt.nBlockAlign, fmt.wBitsPerSample );
            if (chunkHdr.size >= 18)
            {
            	readHeaderToBuffer(file, &ExtraSize, 2, &chunkHdr);
                cout << "  Extra Size: " << ExtraSize << endl;

                if (ExtraSize > 0)
                {
                    // read and process ExtraSize number of bytes as needed...
                    skip(file, ExtraSize, &chunkHdr);
                }
            }
            if (chunkHdr.pos < chunkHdr.size)
                skip(file, chunkHdr.size - chunkHdr.pos, &chunkHdr);
        }
        else if (isChunkID(chunkHdr, 'd', 'a', 't', 'a'))
        {
            if (!fmtFound)
            {
            	CEncoderLogger::ErrorLog("No FMT chunk encountered before DATA chunk\n");
            	file.close();
            	iRet = -1;
            	return iRet;
            	//throw runtime_error("No FMT chunk encountered before DATA chunk");
            }
            // read and process chunkHdr.size number of bytes as needed...

            data.dataSIZE  = chunkHdr.size;
            // this part reads all data of the wave file after resizing the buffer (vector Buffer)
            // resize vector
            wave_.resize( data.dataSIZE );
            // read all data
            file.read( & wave_[0], data.dataSIZE );
            // adjust parent position
            chunkHdr.pos += data.dataSIZE;
        }

        // read other chunks as needed...
        else
        {
            // skip an unwanted chunk
            skip(file, chunkHdr.size, &chunkHdr);
        }

        // all done with this chunk
        waveHdr.pos += chunkHdr.pos;

        // check for chunk padding
        if (chunkHdr.size % 2)
            skip(file, 1, &waveHdr);
    }
    return iRet;
}
// open a input filestream to a given wave audio file (path)
int CInputWaveFile::openWaveFile( const string & PathToFilename )
{
	int iRet = 0;

    fmt.wFormatTag      = 0;
    extra_param_length_ = 0;
    fact.samplesNumber  = -1;

    // get the filename without the path and write to member
    setFilename(PathToFilename);

    if(optv > VL_LOW)
    	printf("extracting wave header information of file %s\n", PathToFilename.c_str());

    // open a input filestream
    ifstream file( PathToFilename.c_str(), ios_base::binary | ios_base::in );

    if( file.is_open() == false )
    {
    	CEncoderLogger::ErrorLog("file is not open");
    	file.close();
    	iRet = -1;
    	return iRet;
    }
    // read the RIFF Chunk descriptor 12 bytes: Id, size Format
    file.read( reinterpret_cast<char*>( &riff ), RIFF_SIZE );

    if(optv > VL_LOW)
    	printf("riffID:  %s size: %d Format: %s\n", riff.riffID, riff.riffSIZE, riff.riffFORMAT );

    file.read( reinterpret_cast<char*>( &fmthdr ), FMTHDR_SIZE );

    if(optv > VL_LOW)
    	printf("fmtID:  %s size: %d \n", fmthdr.fmtID, fmthdr.fmtSIZE );

    // check if it is a format ID is fmt or JUNK. JUNK not supported currently
    if( ( strncmp(fmthdr.fmtID , "JUNK", 4) ) == 0)
    {
    	printf("encoding %s not possible \n", getWaveFileName().c_str());
    	printf("file is in JUNK format, currently not supported\n");
    	// so we return here for this file

    	file.close();
    	iRet = -1;
    	return iRet;

    }
    file.read( reinterpret_cast<char*>( &fmt ), FMT_SIZE );

    if(optv > VL_LOW)
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

    // this part reads all data of the wave file after resizing the buffer (vector Buffer)
    // resize vector
    wave_.resize( data.dataSIZE );
    // read all data
    file.read( & wave_[0], data.dataSIZE );
    return iRet;
}
string CInputWaveFile::getWaveFileName()
{
	return filename;
}
void CInputWaveFile::setFilename(const string &path)
{
	filename = path.substr(path.find_last_of("/") +1 );
}


void CInputWaveFile::get_samples( unsigned int offset, unsigned int size, std::vector<char> & samples ) const
{
    if( offset > ( unsigned )data.dataSIZE )
        return;

    unsigned int real_size = ( offset + size ) < ( unsigned )data.dataSIZE ? size : ( unsigned )data.dataSIZE - offset;

    samples.insert( samples.end(), & wave_[offset], & wave_[offset + real_size] );
}
int16_t CInputWaveFile::get_channels() const
{
    return fmt.nChannels;
}

int32_t CInputWaveFile::get_samples_per_sec() const
{
    return fmt.nSamplesPerSec;
}

int32_t CInputWaveFile::get_avg_bytes_per_sec() const
{
    return fmt.nAvgBytesPerSec;
}

int32_t CInputWaveFile::get_data_size() const
{
    return data.dataSIZE;
}
int32_t CInputWaveFile::get_blockAlign() const
{
    return fmt.nBlockAlign;
}
int32_t CInputWaveFile::get_bitspersample() const
{
    return fmt.wBitsPerSample;
}

CInputWaveFile::~CInputWaveFile() {
	// TODO Auto-generated destructor stub
}

//} /* namespace std */
