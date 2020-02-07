/*
 * CEncoderLogger.cpp
 *
 *  Created on: Jan 25, 2020
 *      Author: maurice
 */

#include <stdarg.h>
//#include <stdio.h>
#include <string.h>
#include <iostream>
#include "../include/CEncoderLogger.h"

namespace std {

CEncoderLogger::CEncoderLogger() {
	// TODO Auto-generated constructor stub

}

CEncoderLogger::~CEncoderLogger() {
	// TODO Auto-generated destructor stub
}

void CEncoderLogger::EncoderLog(const char* msg, const char* fmt...)
{

	// 1. zerlege char* immer bis zum %
	// 2. länge von format ist anzahl der %
	// 3. umwandlung der fmt args in char und anhängen an die zerlegten chars

    va_list args;
    va_start(args, fmt);

    std::cout << msg ;

    while (*fmt != '\0') {
        if (*fmt == 'd') {
            int i = va_arg(args, int);
            std::cout << i << '\n';
        } else if (*fmt == 'c') {
            // note automatic conversion to integral type
            int c = va_arg(args, int);
            std::cout << static_cast<char>(c) << '\n';
        } else if (*fmt == 'f') {
            double d = va_arg(args, double);
            std::cout << d << '\n';
        }
        ++fmt;
    }

    va_end(args);
}
void CEncoderLogger::ErrorLog(const char* errmsg, const char* fmt...){

    va_list args;
    va_start(args, fmt);

    std::cout << errmsg;

    while (*fmt != '\0') {
        if (*fmt == 'd') {
            int i = va_arg(args, int);
            std::cout << i << '\n';
        } else if (*fmt == 'c') {
            // note automatic conversion to integral type
            int c = va_arg(args, int);
            std::cout << static_cast<char>(c) << '\n';
        } else if (*fmt == 'f') {
            double d = va_arg(args, double);
            std::cout << d << '\n';
        }
        ++fmt;
    }

    va_end(args);
}
void CEncoderLogger::ErrorLog(const char* errmsg){

    std::cout << errmsg << '\n';

}
} /* namespace std */
