/*
 * CApplinput.cpp
 *
 *  Created on: Jan 10, 2020
 *      Author: maurice
 */


#include "CApplinput.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

using namespace std;

CApplinput::CApplinput(int argc, char* argv[]) :
        argc_(argc),
        argv_(argv) {
    appName_ = argv_[0];
    this->parse();
}

CApplinput::~CApplinput() {
}

std::string CApplinput::getAppName() const {
    return appName_;
}
std::string CApplinput::getInputFolderName() const {
    return InputFolderName;
}
void CApplinput::parse() {
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
unsigned int CApplinput::checkUserInputfolder(const std::string pathname)
{
	unsigned int num = 0;
	struct stat info;

	if( stat( pathname.c_str(), &info ) != 0 )
	    printf( "cannot access %s\n", pathname.c_str() );
	else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows
		{
			printf( "%s is a directory\n", pathname.c_str() );
			// save the folder name
			setFolderName(pathname);
			num =1;
		}
	else
	    printf( "%s is no directory\n", pathname.c_str() );

	// parse the folder

	return num;
}

void CApplinput::setFolderName(const std::string name )
{
	InputFolderName = name;
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

CApplinput::Option* CApplinput::getParamFromKey(
        const std::string& key) const {
    const Options::const_iterator i = options_.find(key);
    CApplinput::Option* o = 0;
    if (i != options_.end()) {
        o = new CApplinput::Option((*i).first, (*i).second);
    }
    return o;
}


