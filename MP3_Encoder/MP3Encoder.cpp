#include <stdio.h>
#include <string.h>
#include <iostream>
#include </usr/local/include/lame/lame.h>
#include "encoderdef.h"
#include "CApplinput.h"
#include "CInputWaveFile.h"

using namespace std;

int main(int argc, char **argv) {

	CApplinput input(argc, argv);
	CApplinput::Option* opt = input.getParamFromKey("-p");
	printf("application %s version %d started\n", input.getAppName().c_str(), version);
	const string folderpath = opt ? (*opt).second : "";
	/* user input folder check code */
	printf("user input folder path is: %s\n", folderpath.c_str());
	if(input.optv > 1)
		printf("checking input folder path if containing .wav files\n");
	if(input.checkUserInputfolder(folderpath) < 1)
	{
		//exit
		return -1;
	}

	// for every .wav file in the input folder a decoder will be set up
	for (unsigned int var = 0; var < input.getNumOfFilesInFolder(); ++var)
	{
		// read out the .wav file name
		CInputWaveFile wave(input.returnWaveFileNameFromIndex(var));
	};

	// init lame library
	lame_t psLame;

	psLame = lame_init();
	if(psLame != NULL)
		printf("init lame successful\n");
	else
	 {
		printf("init NOT successfull\n");
		return -1;
	 }

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
	}
	return 0;
}
