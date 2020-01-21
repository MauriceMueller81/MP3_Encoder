#include <stdio.h>
#include <string.h>
#include <iostream>

#ifdef WINDOWS
// include the pthread_w32 header
// do not forget to link the specific library
#else
#include <pthread.h>
#endif

#include </usr/local/include/lame/lame.h>
#include "encoderdef.h"
#include "CApplinput.h"
#include "CLameEncoder.h"


using namespace std;
// thread function
unsigned int optv;

void *thread_encoder(void *path);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

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
		printf("false user input: no input path set\n");
		input.printUsage();
		return -1;
	}
	const string verbosity = opt_verb ? (*opt_verb).second : "";
	if(verbosity != "")
		input.setApplicationVerbosity(verbosity);

	if(optv > VL_LOW)
		input.printOptions();

	/* user input folder check code */
	if(optv > VL_NO)
		printf("checking input folder path if containing .wav files\n");
	if(input.checkUserInputfolder(folderpath) < 1)
	{
		input.printUsage();
		//exit
		return -1;
	}


	string sInputPath = "";
	// handling file encoding using threads
	vector<pthread_t>  thread_vec;
	unsigned int numOfFiles = input.getNumOfFilesInFolder();
	thread_vec.reserve(numOfFiles);

	// for every .wav file in the input folder a decoder will be set up
	for (unsigned int var = 0; var < input.getNumOfFilesInFolder(); var++)
	{
		sInputPath = folderpath + "/" + input.returnWaveFileNameFromIndex(var);
		// encode wave file

	        int t = pthread_create(&thread_vec[var], NULL, thread_encoder, &sInputPath);

	        if (t != 0)
	        {
	            cout << "Error in thread creation: " << t << endl;
	        }
	}
    for(unsigned int j = 0 ; j < numOfFiles; j++)
    {
        void* status;
        int t = pthread_join(thread_vec[j], &status);
        if (t != 0)
        {
            cout << "Error in thread join: " << t << endl;
            }
    }

}

void *thread_encoder(void *path)

{

	string tmp = (*static_cast<string*>(path));

	printf("Encoder Thread number %ld\n", pthread_self());
	printf("Encoder thread path %s\n", tmp.c_str());

	// create object of CLameEncoder class in automatic memory
	// init lame library
	CLameEncoder encoder;

	encoder.encode(tmp);

	//pthread_mutex_lock( &mutex1 );

	//pthread_mutex_unlock( &mutex1 );

}
