/*
 * CLameEncoder.h
 *
 *  Created on: Jan 15, 2020
 *      Author: maurice
 */

#ifndef CLAMEENCODER_H_
#define CLAMEENCODER_H_

#include <stdio.h>
#include </usr/local/include/lame/lame.h>

namespace std {

class CLameEncoder {
public:
	CLameEncoder();
	virtual ~CLameEncoder();
    bool encode( const string & filename );
   // static void LameDebugCB(const char *format, va_list ap);

    lame_t psLame;
private:
    static int optv;
    bool initLame();

};

} /* namespace std */

#endif /* CLAMEENCODER_H_ */
