/*
 * Macros.h
 *
 *  Created on: Oct 1, 2012
 *
 *      @author: Stephan Hesse <disparat@googlemail.com>
 *
 *      copyright (c) 2011, 2012, Some rights reserved
 *
 *		Dual license:
 *		This piece of software may be used, modified and distributed freely for
 *		non-commercial use. For commercially aimed, profit-bound usage, extension,
 *		static linking and non-share-alike or open source distribution, please contact
 *		the author.
 *
 *		mURIeL: a library for multiple URI/URLs
 *
 *		version: 0.4
 */

#ifndef MACROS_H_
#define MACROS_H_

////////////// CONFIG PARAMS //////////////////////

#define DEBUG_LOG_ENABLED

#define AUTO_INCLUDE_SCHEMES //disable this for not using the scheme map

//#define AUTO_INITIALIZE //use this to build the scheme map automatically when the first instance of a Url is created

#define MURIEL_VERSION 0.4

////////////////// DEBUG LOG ///////////////////////

#ifdef DEBUG_LOG_ENABLED
#include <iostream>
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

#endif /* MACROS_H_ */
