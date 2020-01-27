/*
 * CEncoderLogger.h
 *
 *  Created on: Jan 25, 2020
 *      Author: maurice
 */

#ifndef CENCODERLOGGER_H_
#define CENCODERLOGGER_H_

namespace std {

class CEncoderLogger {
public:
	CEncoderLogger();
	virtual ~CEncoderLogger();
	static void ErrorLog( const char* errmsg, const char* fmt...);
	static void EncoderLog(const char* msg, const char* fmt...);
	static void ErrorLog(const char* errmsg);

	unsigned int optv;
};

} /* namespace std */

#endif /* CENCODERLOGGER_H_ */
