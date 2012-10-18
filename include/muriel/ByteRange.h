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

#ifndef BYTERANGE_H_
#define BYTERANGE_H_

class ByteRange {

	public:
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
