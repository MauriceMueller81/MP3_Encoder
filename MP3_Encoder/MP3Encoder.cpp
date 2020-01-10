#include <stdio.h>
#include <string.h>
#include <iostream>
#include </usr/local/include/lame/lame.h>
#include "encoderdef.h"
#include "CApplinput.h"

using namespace std;

int main(int argc, char **argv) {

	CApplinput input(argc, argv);
	CApplinput::Option* opt = input.getParamFromKey("-p");
	printf("application %s started\n", input.getAppName().c_str());
	const string folderpath = opt ? (*opt).second : "";
	cout << folderpath << endl;
	/* user input folder check code */
	printf("user input folder path is: %s\n", folderpath.c_str());
	if(input.checkUserInputfolder(folderpath) < 1)
	{
		//exit
		return -1;
	}

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
