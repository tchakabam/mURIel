/*
 *
 *     Copyright (c) 2011, 2012 Stephan Hesse <tchakabam@gmail.com>
 *
 *     Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 *     documentation files (the "Software"), to deal in the Software without restriction, including without limitation
 *     the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 *     and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *     OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *     LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *     CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *	   mURIeL: a library for multiple URI/URLs
 *
 */

#ifndef MACROS_H_
#define MACROS_H_

////////////// CONFIG PARAMS //////////////////////

//#define DEBUG_LOG_ENABLED

#define AUTO_INCLUDE_SCHEMES //disable this for not using the scheme map

//#define AUTO_INITIALIZE //use this to build the scheme map automatically when the first instance of a Url is created

#define MURIEL_VERSION 401  // dec  MMmmss (mayor, minor, sub)

////////////////// DEBUG LOG ///////////////////////

#ifdef DEBUG_LOG_ENABLED
#include <iostream>
#include <stdio.h>
#define MURIEL_LOG(msg, ...) {std::cout << "{ mURIeL DeBug } " << __FILE__ << "::" << __FUNCTION__ << "():" << __LINE__ << " [ "; printf(msg, __VA_ARGS__); std::cout << " ]\n";}
#else
#define MURIEL_LOG(msg, ...) {}
#endif

//////////////// SCHEME CONFIG TOOL /////////////////// (see Schemes.h)

#define ADD_SCHEME(id, string) { SchemeMap.insert(std::make_pair(id, string)); }
#define INIT_BEGIN static void InitMuriel() {
#define INIT_END }
#define MURIEL_CONFIGURE() InitMuriel();
#define MURIEL_CLEAR() { SchemeMap.clear(); }

/* Return codes */
#define MURIEL_ABSOLUTE 'a'
#define MURIEL_RELATIVE 'r'
#define MURIEL_MALFORMED 'm'
#define MURIEL_ERROR 'e'

#endif /* MACROS_H_ */
