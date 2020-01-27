/*
 * CApplinput.cpp
 *
 *  Created on: Jan 10, 2020
 *      Author: maurice
 */

#ifdef WINDOWS
#include <windows.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <iterator>
#include <dirent.h>
#include <algorithm>

#include "CApplinput.h"
#include "encoderdef.h"

using namespace std;

CApplinput::CApplinput(int argc, char* argv[]) :
        argc_(argc),
        argv_(argv) {
    appName_ = argv_[0];
    InputFolderName = "";
    uiNumberOfFilesInFolder = 0;

    this->parseInputParameter();
}

CApplinput::~CApplinput() {
}

void CApplinput::printUsage()
{
#ifdef WINDOWS
#else
	cout << "MP3_Encoder useage:" << endl;
	cout <<"MP3_Encoder [ -v <verbosity> optional -p <path> ]" << endl;
	cout << "OPTIONS:" << endl;
	cout << " -v <verbosity> "<< endl;
	cout << "verbosity level. can be low, debug, no. default is no output" << endl;
	cout << "example: MP3_Encoder -v debug - prints all output information" << endl;
	cout << " -p <path> "<< endl;
	cout << "path to folder containing encoder iput files" << endl;
	cout << "example: MP3_Encoder -p /home/Music" << endl;
	cout << "function:" << endl;
	cout << "encodes all *.wav files in folder /home/Music" << endl;
	cout << "wave files must be in 16bit format and have a canonical header" << endl;


#endif
}
string CApplinput::getAppName() const {
    return appName_;
}
string CApplinput::getInputFolderName() const {
    return InputFolderName;
}
unsigned int CApplinput::getNumOfFilesInFolder() const { // @suppress("Member declaration not found")
	return uiNumberOfFilesInFolder;
}
void CApplinput::parseInputParameter() {
    typedef pair<string, string> Option;
    Option* option = new pair<string, string>();
    for (const char* const * i = this->begin() + 1; i != this->end(); i++) {
        const string p = *i;
        if (option->first == "" && p[0] == '-') {
            option->first = p;
            if (i == this->last()) {
                options_.insert(Option(option->first, option->second));
            }
            continue;
        } else if (option->first != "" && p[0] == '-') {
            option->second = "null"; /* or leave empty? */
            options_.insert(Option(option->first, option->second));
            option->first = p;
            option->second = "";
            if (i == this->last()) {
                options_.insert(Option(option->first, option->second));
            }
            continue;
        } else if (option->first != "") {
            option->second = p;
            options_.insert(Option(option->first, option->second));
            option->first = "";
            option->second = "";
            continue;
        }
    }
}

void CApplinput::setApplicationVerbosity(const string verb)
{

	if(verb == "low")
		optv = VL_LOW;
	else if (verb == "debug")
		optv = VL_DEBUG;
	else if (verb == "no")
		optv = VL_NO;
	else
		optv = VL_NO; 	// default level is no printf information
}
unsigned int CApplinput::checkUserInputfolder(const string pathname)
{
	unsigned int num = 0;
	struct stat info;

	if( stat( pathname.c_str(), &info ) != 0 )
	    printf( "cannot access %s\n", pathname.c_str() );
	else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows
		{
			if (optv > VL_LOW)
				printf( "%s is a directory\n", pathname.c_str() );
			// save the folder name
			setFolderName(pathname);
		}
	else
	    printf( "%s is no directory\n", pathname.c_str() );

	// parse the folder
	num = getWaveFilesInDirectory(pathname, FilesInFolder);
    copy(FilesInFolder.begin(), FilesInFolder.end(),ostream_iterator<string>(cout, "\n"));
    this->setNumOfFilesInFolder(num);
	return num;
}

string CApplinput::returnWaveFileNameFromIndex( const unsigned int index)
{
	return FilesInFolder[index];
}
/* Returns a list of files in a directory (except the ones that begin with a dot) */

int CApplinput::getWaveFilesInDirectory(const string &directory, vector<string> &out)
{
	int ret = 0;
	string tmpstr;
#ifdef WINDOWS
    std::string pattern(directory);
    pattern.append("\\*");
    WIN32_FIND_DATA data;
    HANDLE hFind;
    // ToDO add check for .wav extension
    if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
        	out.push_back(data.cFileName);
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
#else
	DIR* dirp = opendir(directory.c_str());
	    struct dirent * dp;
	    while ((dp = readdir(dirp)) != NULL) {
	    	tmpstr= dp->d_name;
	    	if(checkFileIfWaveFile(tmpstr))
	    	{
	    		ret++;
	    		out.push_back(dp->d_name);
	    		if(optv > VL_LOW)
	    			printf("current file: %s\n", dp->d_name);
	    	}
	    }
	    closedir(dirp);
#endif
	    return ret;
}
// GetFilesInDirectory
bool CApplinput::checkFileIfWaveFile(const string fn)
{
	bool bWaveFlag = false;
	  if(fn.substr(fn.find_last_of(".") + 1) == "wav")
		  bWaveFlag =true;

	return bWaveFlag;
}
void CApplinput::setFolderName(const std::string name )
{
	InputFolderName = name;
}
void CApplinput::setNumOfFilesInFolder(const unsigned int number){
	uiNumberOfFilesInFolder = number;
}
void CApplinput::printOptions() const {
    std::map<std::string, std::string>::const_iterator m = options_.begin();
    int i = 0;
    if (options_.empty()) {
        cout << "No parameters\n";
    }
    for (; m != options_.end(); m++, ++i) {
        cout << "Parameter [" << i << "] [" << (*m).first << " " << (*m).second
                << "]\n";
    }
}

const char* const *CApplinput::begin() const {
    return argv_;
}

const char* const *CApplinput::end() const {
    return argv_ + argc_;
}

const char* const *CApplinput::last() const {
    return argv_ + argc_ - 1;
}

bool CApplinput::hasKey(const std::string& key) const {
    return options_.find(key) != options_.end();
}

CApplinput::Option* CApplinput::getParamFromKey(const string& key) const
{
    const Options::const_iterator i = options_.find(key);
    CApplinput::Option* o = 0;
    if (i != options_.end()) {
        o = new CApplinput::Option((*i).first, (*i).second);
    }
    return o;
}


