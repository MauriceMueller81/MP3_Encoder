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
	CApplinput::Option* opt_path = input.getParamFromKey("-p");
	CApplinput::Option* opt_verb = input.getParamFromKey("-v");

	// get application information
	string tmp = input.getAppName();
	const char * applicationName = (tmp.substr(tmp.find_last_of("/")+1) ).c_str();
	// print name and version
	printf("application %s version %.2f started\n", applicationName, version);

	// check user input path if it is set
	const string folderpath = opt_path ? (*opt_path).second : "";
	if ((folderpath == "")||(folderpath=="null"))
	{
		//
		printf("false user input: no input path set\n");
		input.printUsage();
		return -1;
	}
	const string verbosity = opt_verb ? (*opt_verb).second : "";
	if(verbosity != "")
		input.setApplicationVerbosity(verbosity);

	input.printOptions();
	printf("user input verbosity: %s\n", verbosity.c_str());
	printf("user input folder path is: %s\n", folderpath.c_str());

	/* user input folder check code */

	if(input.optv > 1)
		printf("checking input folder path if containing .wav files\n");
	if(input.checkUserInputfolder(folderpath) < 1)
	{
		input.printUsage();
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
		// encode wave file
		sInputPath = folderpath + "/" + input.returnWaveFileNameFromIndex(var);
		encoder.encode(sInputPath);

	};
}
