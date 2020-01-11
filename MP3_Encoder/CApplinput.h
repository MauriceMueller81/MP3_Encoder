/*
 * CApplinput.h
 *
 *  Created on: Jan 10, 2020
 *      Author: maurice
 */

#ifndef CAPPLINPUT_H_
#define CAPPLINPUT_H_

#include <map>
#include <string>
#include <vector>

using namespace std;

class CApplinput {

public:
    typedef std::pair<std::string, std::string> Option;
    CApplinput(int argc, char *argv[]);
    virtual ~CApplinput();
    string getAppName() const;
    string getInputFolderName() const;
    unsigned int checkUserInputfolder(const std::string pathname);

    bool hasKey(const string&) const;
    Option* getParamFromKey(const string&) const;
    void printOptions() const;

    int optv;
private:
    typedef std::map<std::string, std::string> Options;
    void parse();
    void setFolderName(const std::string name);
    int getFilesInDirectory(const string &directory, vector<string> &out);
    const char* const *begin() const;
    const char* const *end() const;
    const char* const *last() const;
    Options options_;
    int argc_;
    char** argv_;
    string appName_;
    string InputFolderName;
    unsigned int uiNumberOfFilesInFolder;
    vector<string>FilesInFolder;
};

#endif /* CAPPLINPUT_H_ */
