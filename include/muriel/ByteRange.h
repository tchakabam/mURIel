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

#ifndef BYTERANGE_H_
#define BYTERANGE_H_

class ByteRange {

	public:
		ByteRange(const char* psz);

		ByteRange(const std::string& s);

		ByteRange(size_t from, size_t to)
		:u_From(from)
		,u_To(to)
		,b_None(false)
		{}

		ByteRange()
		:u_From(-1)
		,u_To(-1)
		,b_None(true)
		{}

		//generates any string format
		UrlElement toFormat(const std::string & format, bool bInvert=false) const;

		//creates a standard HTTP request parameter string
		UrlElement toString() const;

		size_t u_From;
		size_t u_To;
		bool   b_None;

	private:

		bool parse_range(const std::string& s, size_t* from, size_t* to);
};

#endif /* BYTERANGE_H_ */
