/*
 * COutputEncodedFile.cpp
 *
 *  Created on: Jan 16, 2020
 *      Author: maurice
 */

#include "COutputEncodedFile.h"
#include <string>
#include <fstream>

namespace std {

COutputEncodedFile::COutputEncodedFile() {
	// TODO Auto-generated constructor stub

}

// input is the path to input *.wav file
COutputEncodedFile::COutputEncodedFile(const string & filename )
{
	// construct the output filename by renaming it to *.mp3
	initializeOutputFilename(filename);
	printf("writing encoded data to %s\n", this->getOutputFileName().c_str());

	// setup output
	mp3Output.open( getOutputFileName().c_str(), std::ios_base::binary | std::ios_base::out  );

}
COutputEncodedFile::~COutputEncodedFile() {
	// TODO Auto-generated destructor stub
}

void COutputEncodedFile::initializeOutputFilename(const string & ofn){

	string tmp = ofn.substr(0, ofn.find_last_of(".") )+ ".mp3";
	setOutputFileName(tmp);


}
void COutputEncodedFile::setOutputFileName(const string & fn){
	outputfilename = fn;
}
void COutputEncodedFile::writeToMP3File( char * mp3buffer, const int MP3size){
	mp3Output.write( reinterpret_cast<char*>( mp3buffer ), MP3size );
}

string COutputEncodedFile::getOutputFileName(){
	return outputfilename;
}
} /* namespace std */
