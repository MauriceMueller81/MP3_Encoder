#include <stdio.h>
#include <string.h>
#include <iostream>
#include </usr/local/include/lame/lame.h>
#include "encoderdef.h"
#include "CApplinput.h"
#include "CLameEncoder.h"


using namespace std;

int main(int argc, char **argv) {

	// create object of class CApplinput in automatic memory
	CApplinput input(argc, argv);
	// get the input options
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

	string sInputPath = "";

	// create object of CLameEncoder class in automatic memory
	// init lame library
	CLameEncoder encoder;
	// for every .wav file in the input folder a decoder will be set up
	for (unsigned int var = 0; var < input.getNumOfFilesInFolder(); ++var)
	{
		// read out the .wav file name
		//CInputWaveFile wave(folderpath + "/" + input.returnWaveFileNameFromIndex(var));
		// encode wave file
		sInputPath = folderpath + "/" + input.returnWaveFileNameFromIndex(var);

		encoder.encode(sInputPath);

	};
}
