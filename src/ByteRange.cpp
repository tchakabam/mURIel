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

