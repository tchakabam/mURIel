/*
 * Url.cpp
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

#include <muriel/Url.h>

/* sys specific */
#include <stdlib.h>
#include <malloc.h>

using namespace std;

	ByteRange::ByteRange(const string& s)
	{
		b_None = ! parse_range(s, &u_From, &u_To);
	}

	//creates a standard HTTP request parameter string
	UrlElement ByteRange::toString() const {
		if(!b_None) {
			stringstream ss;
			ss << u_From << "-" << u_To;
			MURIEL_LOG("Result: %s", ss.str().c_str());
			return ss.str();
		}
		else  {
			MURIEL_LOG("Error! (%s)", "No range");
			return "";
		}
	}

	//generates any string format
	UrlElement ByteRange::toFormat(const string & format, bool bInvert) const {
		if(!b_None) {
			//we need to go on the heap to use sprintf
			char* sz = (char*)malloc(format.length()+1);
			if(!bInvert) {
				sprintf(sz, format.c_str(), u_From, u_To);
			} else {
				sprintf(sz, format.c_str(), u_To, u_From);
			}
			//put this back on the stack
			UrlElement res(sz);
			free(sz); //and free
			return res;
		}
		else return "";
	}

    bool ByteRange::parse_range(const string& s, size_t* from, size_t* to) {
    	size_t pos = s.find('-', 0);
    	if(pos>=0) {
			*to = atol(s.substr(pos+1,s.size()-pos-1).c_str());
			*from = atol(s.substr(0,pos).c_str());
			MURIEL_LOG("Result: %d-%d", *from, *to);
			return true;
    	} else {
    		*to = -1;
    		*from = -1;
    		MURIEL_LOG("Error! (%s)", s.c_str());
    		return false;
    	}

    }

