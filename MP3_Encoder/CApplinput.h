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

class CApplinput {

public:
    typedef std::pair<std::string, std::string> Option;
    CApplinput(int argc, char *argv[]);
    virtual ~CApplinput();
    std::string getAppName() const;
    std::string getInputFolderName() const;
    unsigned int checkUserInputfolder(const std::string pathname);

    bool hasKey(const std::string&) const;
    Option* getParamFromKey(const std::string&) const;
    void printOptions() const;
private:
    typedef std::map<std::string, std::string> Options;
    void parse();
    void setFolderName(const std::string name);
    const char* const *begin() const;
    const char* const *end() const;
    const char* const *last() const;
    Options options_;
    int argc_;
    char** argv_;
    std::string appName_;
    std::string InputFolderName;
};

#endif /* CAPPLINPUT_H_ */
