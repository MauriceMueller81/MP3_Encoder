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
#include </usr/local/include/lame/lame.h>


#include "CLameEncoder.h"
#include "CInputWaveFile.h"
#include "COutputEncodedFile.h"
#include "encoderdef.h"


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
	if(optv > 1)
		printf("closing lame lib\n");
	lame_close(psLame);
}

bool CLameEncoder::initLame(){

	bool ret = false;

	psLame = lame_init();

	if(psLame != NULL)
		printf("init lame version %s successful\n", get_lame_version());
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
	printf("current quality is set to %d\n", quality);
	// set quality to good
	if(lame_set_quality(psLame,LAME_QUAL_GOOD ) < 0 )
		printf("set quality failure\n");
	else
	{
		// re read out the new quality level
		quality = lame_get_quality(psLame);
		printf("current quality is set to %d\n", quality);
		printf("set quality to level good\n");
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
bool CLameEncoder::encode( const string & filename )
{
    unsigned int sample_rate    = 0;
    unsigned int byte_rate      = 0;
    unsigned int channels       = 0;

    // create object of class CInputWave in automatic memory
	CInputWaveFile  pcm( filename );

	// create object of class COutputEncodedFile in automatic memory
	COutputEncodedFile mp3(filename);

	byte_rate = pcm.get_avg_bytes_per_sec();
	sample_rate = pcm.get_samples_per_sec();
	channels = pcm.get_channels();

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

    unsigned offset = 0;

    std::vector<char> pcm_buffer;
    pcm_buffer.reserve( sizeof(short int) * PCM_SIZE * 2 );

    int calcMP3Size =  (pcm_buffer.size()/2)* (byte_rate/sample_rate) + 4*1152*(byte_rate)/sample_rate + 512;

    printf("calculated Buffer size = %d default is %d\n", calcMP3Size, MP3_SIZE);

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

    int k = ( channels == 1 ) ? 1 : 2;
    unsigned size = PCM_SIZE * k * sizeof( short int );
    unsigned read = pcm_buffer.size();
    unsigned read_shorts = read / 2;  // need number of 'short int' read
    int writeResult = 0;

    while( true )
    {
    	// read pcm data from pcm file and write to pcm_buffer
    	pcm.get_samples( offset, size, pcm_buffer );
    	read = pcm_buffer.size();
    	offset += read;
    	read_shorts = read / 4;
    	if(optv > 2 )
    		printf("read = %d --- read_shorts = %d\n",read, read_shorts );
    	if( read > 0 )
    	{
    		if( channels == 1 )
    		{
    			//writeResult = lame_encode_buffer(psLame, reinterpret_cast<short int*>( &pcm_buffer[0] ), NULL, read_shorts, mp3_buffer, /*MP3_SIZE*/0 );
    			writeResult = lame_encode_buffer( psLame, (short int*)(&pcm_buffer[0]) , NULL, 2* read_shorts, mp3_buffer, MP3_SIZE );
    		}
    		else
    		{

    			//writeResult = lame_encode_buffer_interleaved( psLame, reinterpret_cast<short int*>( &pcm_buffer[0] ), read_shorts, mp3_buffer, /*MP3_SIZE*/0 );
    			writeResult = lame_encode_buffer_interleaved( psLame, (short int*)( &pcm_buffer[0] ), read_shorts, mp3_buffer, MP3_SIZE );
    		}
    		// clean the tmp buffer
    		pcm_buffer.clear();
    		if (writeResult >= 0)
    			mp3.writeToMP3File( reinterpret_cast<char*>( mp3_buffer ) , writeResult );
    		else
    			printf("error encoding input file error code: %d\n",writeResult );
    	}

    	if( read < size )
    	{
    		writeResult = lame_encode_flush(psLame, mp3_buffer, MP3_SIZE );

    		if (writeResult >= 0)
    			mp3.writeToMP3File( reinterpret_cast<char*>( mp3_buffer ) , writeResult );
    		else
    			printf("error encoding input file error code: %d\n",writeResult );
    		// break the while loop when finished
    		break;
    	}

    }
    if(optv > 1)
    	printf("encoding of file %s finished\n", pcm.getWaveFileName().c_str() );
    return true;
}
} /* namespace std */
