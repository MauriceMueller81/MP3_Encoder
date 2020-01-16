/*
 * COutputEncodedFile.h
 *
 *  Created on: Jan 16, 2020
 *      Author: maurice
 */

#ifndef COUTPUTENCODEDFILE_H_
#define COUTPUTENCODEDFILE_H_

#include <string>
#include <fstream>

namespace std {

class COutputEncodedFile {
public:
	COutputEncodedFile();
	COutputEncodedFile(const string & filename );
	virtual ~COutputEncodedFile();
	void writeToMP3File( char * mp3buffer, const int MP3size);
	string getOutputFileName(void);

private:
	void initializeOutputFilename(const string & ofn);
	void setOutputFileName(const string & fn);
	string outputfilename;
	ofstream mp3Output;
};

} /* namespace std */

#endif /* COUTPUTENCODEDFILE_H_ */
