/*
 * CLameEncoder.cpp
 *
 *  Created on: Jan 15, 2020
 *      Author: maurice
 */
#include <fstream>                  // std::ofstream
#include <stdexcept>                // std::logic_error
#include <stdio.h>
#include <stdint.h>     			// int32_t, int16_t


#if defined(__cplusplus)
extern "C"{
#ifdef WINDOWS
#include "../include/lame.h"
#else
#include </usr/local/include/lame/lame.h>
#endif
}
#endif

#include "../include/CLameEncoder.h"
#include "../include/CInputWaveFile.h"
#include "../include/COutputEncodedFile.h"
#include "../include/encoderdef.h"
#include "../include/CEncoderLogger.h"


namespace std {


static void LameDebugCB(const char *format, va_list ap)
{
    (void) vfprintf(stdout, format, ap);
    printf("debug callback from lame \n");
}

CLameEncoder::CLameEncoder() {

	if(!this->initLame())
		printf("init NOT successfull\n");
}

CLameEncoder::~CLameEncoder() {
	closeLameLib();
}
void CLameEncoder::closeLameLib() {
	if(optv > VL_LOW)
		printf("closing lame lib\n");
	lame_close(psLame);
}

bool CLameEncoder::initLame(){

	bool ret = false;

	psLame = lame_init();

	if(psLame != NULL)
	{
		if(optv > VL_LOW)
			printf("init lame version %s successful\n", get_lame_version());
	}
		else
	 {
		printf("init NOT successfull\n");
		return ret;
	 }

	//lame_set_errorf(psLame, (void*) LameDebugCB( const char *format, va_list ap));
	lame_set_errorf(psLame, &LameDebugCB);
	//lame_set_errorf(psLame, NULL);
	lame_set_debugf(psLame, &LameDebugCB);
	lame_set_msgf(psLame, &LameDebugCB);

	// configure lame encoder
	// read out the quality level
	int quality = lame_get_quality(psLame);
	if(optv > VL_LOW)
		printf("current quality is set to %d\n", quality);
	// set quality to good
	if(lame_set_quality(psLame,LAME_QUAL_GOOD ) < 0 )
		printf("set quality failure\n");
	else
	{
		// re read out the new quality level
		quality = lame_get_quality(psLame);
		if(optv > VL_LOW)
		{
			printf("current quality is set to %d\n", quality);
			printf("set quality to level good\n");
		}
		ret = true;
	}

	/*
	 * OPTIONAL:
	 * print internal lame configuration to message handler
	 */
	if (optv>1)
	{
		lame_print_config(psLame);
		lame_print_internals( psLame);
	}
	return ret;
}
void CLameEncoder::setOutputFilename(const string name)
{
	if(name != "")
		outputFileName = name;
}
/*
 * print out lame encoder configuration
 * */
void CLameEncoder::showLameConfig()
{

	// show only in Dubug mode:
	if(optv > VL_LOW)
	{
		printf( "get number of samples: %d\n", lame_get_num_samples(psLame));
		printf( "current out sample rate: %d\n", lame_get_out_samplerate(psLame));
		// show curent lame encoder configuration
		printf( "current bitrate: %d\n", lame_get_brate(psLame));
		// get compression ration
		printf( "current compression: %.2f\n", lame_get_compression_ratio(psLame));
		// get scale factor
		printf( "current scale: %.2f\n",lame_get_scale(psLame));
	}
}
/*
 * encode a given file with path
 * */
bool CLameEncoder::encode( const string & filename )
{
	bool bResult = true;

    unsigned int sample_rate    = 0;
    unsigned int byte_rate      = 0;
    unsigned int channels       = 0;
    unsigned int blockalign		= 0;

    // create object of class CInputWave in automatic memory
	CInputWaveFile  pcm;
	if( pcm.parseWaveFile/*openWaveFile*/( filename ) < 0 )
	{
		bResult = false;
		closeLameLib();
		return bResult;
	}

    if(optv > VL_NO)
    	printf("start encoding of %s\n", pcm.getWaveFileName().c_str() );

	// create object of class COutputEncodedFile in automatic memory
	COutputEncodedFile mp3(filename);

	setOutputFilename(mp3.getOutputFileName());

	// get information of wav file header
	byte_rate = pcm.get_avg_bytes_per_sec();
	sample_rate = pcm.get_samples_per_sec();
	channels = pcm.get_channels();
	blockalign = pcm.get_blockAlign();
	// ToDo: check the MP3 Size
    /*
     * input pcm data, output (maybe) mp3 frames.
     * This routine handles all buffering, resampling and filtering for you.
     *
     * return code     number of bytes output in mp3buf. Can be 0
     *                 -1:  mp3buf was too small
     *                 -2:  malloc() problem
     *                 -3:  lame_init_params() not called
     *                 -4:  psycho acoustic problems
     *
     * The required mp3buf_size can be computed from num_samples,
     * samplerate and encoding rate, but here is a worst case estimate:
     *
     * mp3buf_size in bytes = 1.25*num_samples + 7200
     *
     * I think a tighter bound could be:  (mt, March 2000)
     * MPEG1:
     *    num_samples*(bitrate/8)/samplerate + 4*1152*(bitrate/8)/samplerate + 512
     * MPEG2:
     *    num_samples*(bitrate/8)/samplerate + 4*576*(bitrate/8)/samplerate + 256
     *
     * but test first if you use that!
     *
     * set mp3buf_size = 0 and LAME will not check if mp3buf_size is
     * large enough.
     *
     * NOTE:
     * if gfp->num_channels=2, but gfp->mode = 3 (mono), the L & R channels
     * will be averaged into the L channel before encoding only the L channel
     * This will overwrite the data in buffer_l[] and buffer_r[].
     *
    */

    unsigned char mp3_buffer[MP3_SIZE *2 ];

    lame_set_in_samplerate(this->psLame, sample_rate);
    lame_set_brate(psLame,byte_rate);

    if( channels == 1 )
    {
    	lame_set_num_channels(psLame, 1);
    	lame_set_mode(psLame, MONO);
    }
    else
    {
    	lame_set_num_channels(psLame, channels);
    	lame_set_mode(psLame, STEREO);
    }
    lame_set_VBR(psLame, vbr_default);
    lame_init_params(psLame);

    this->showLameConfig();

    unsigned offset = 0;
    // this is the tempbuffer for the encoder input
    std::vector<char> pcm_buffer;
    // this will be for 16 bit input only!
    //pcm_buffer.reserve( sizeof(short int) * PCM_SIZE * 2 );
    // we will use BlockAllign for calclulation number of bytes per sample for all channels
    pcm_buffer.reserve( blockalign * PCM_SIZE  );

    int calcMP3Size =  (pcm_buffer.size()/2)* (byte_rate/sample_rate) + 4*1152*(byte_rate)/sample_rate + 512;

    if(optv > VL_LOW)
    	printf("calculated Buffer size = %d default is %d\n", calcMP3Size, MP3_SIZE);

    //unsigned size = PCM_SIZE * k * sizeof( short int );
    // size : number of Bytes from all channels in PCMBuffer
    unsigned size = PCM_SIZE * blockalign;
    unsigned BytesInPCMBuffer = pcm_buffer.size();
    unsigned NumOfSamplesPerChannel = BytesInPCMBuffer / 2;  // need number of 'short int' read
    int writeResult = 0;

    while( true )
    {
    	// read size bytes of data from wav file and write to pcm_buffer
    	pcm.get_samples( offset, size, pcm_buffer );
    	BytesInPCMBuffer = pcm_buffer.size();
    	offset += BytesInPCMBuffer;
    	NumOfSamplesPerChannel = (BytesInPCMBuffer / blockalign);
    	if( BytesInPCMBuffer > 0 )
    	{
    		if( channels == 1 )
    		{
    			// encode mono files
    			if(blockalign == 2)
    				writeResult = lame_encode_buffer( psLame, reinterpret_cast<short int*>(&pcm_buffer[0]) , NULL, NumOfSamplesPerChannel, mp3_buffer, MP3_SIZE );
    			else
    			{
    				writeResult = lame_encode_buffer_int( psLame, reinterpret_cast<int*>(&pcm_buffer[0]) , NULL, NumOfSamplesPerChannel, mp3_buffer, MP3_SIZE );
    			}
    		}
    		else
    		{
    			// if 24 or 32 bit are used
    			if(blockalign > 4)
    				writeResult = lame_encode_buffer_interleaved_int( psLame, reinterpret_cast<int*>( &pcm_buffer[0] ), NumOfSamplesPerChannel, mp3_buffer, MP3_SIZE );
    			//	writeResult = lame_encode_buffer_interleaved( psLame, reinterpret_cast<short int*>( &pcm_buffer[0] ), NumOfSamplesPerChannel, mp3_buffer, MP3_SIZE );
    			else // for 16 bit and 8 bit
    				writeResult = lame_encode_buffer_interleaved( psLame, reinterpret_cast<short int*>( &pcm_buffer[0] ), NumOfSamplesPerChannel, mp3_buffer, MP3_SIZE );

    		}
    		// clean the tmp buffer
    		pcm_buffer.clear();
    		if (writeResult >= 0)
    			mp3.writeToMP3File( reinterpret_cast<char*>( mp3_buffer ) , writeResult );
    		else
    			printf("error encoding input file error code: %d\n",writeResult );
    	}

    	if( BytesInPCMBuffer < size )
    	{
    		writeResult = lame_encode_flush(psLame, mp3_buffer, MP3_SIZE );

    		if (writeResult >= 0)
    			mp3.writeToMP3File( reinterpret_cast<char*>( mp3_buffer ) , writeResult );
    		else
    			printf("error encoding input file error code: %d\n",writeResult );
    		// break the while loop when finished
    		break;
    	}

    }// end of while

    // set the mp3 tags
    //lame_mp3_tags_fid(psLame, output);

    if(optv > VL_NO)
    	printf("encoding of file %s finished\n", pcm.getWaveFileName().c_str() );
    return true;
}
} /* namespace std */
