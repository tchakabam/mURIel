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
#ifndef CONFIG_H_
#define CONFIG_H_

#include <muriel/Url.h>

//////////////// ADD MORE SCHEMES HERE //////////////////

INIT_BEGIN
ADD_SCHEME(UrlScheme::FTP, "ftp");
ADD_SCHEME(UrlScheme::HTTP, "http");
ADD_SCHEME(UrlScheme::HTTPS, "https");
ADD_SCHEME(UrlScheme::RTSP, "rtsp");
ADD_SCHEME(UrlScheme::FILE, "file");
ADD_SCHEME(UrlScheme::DVD, "dvd");
ADD_SCHEME(UrlScheme::VCD, "vcd");
ADD_SCHEME(UrlScheme::BLURAY, "blueray");
ADD_SCHEME(UrlScheme::MAIL, "mailto");
ADD_SCHEME(UrlScheme::ABOUT, "about");
INIT_END

#endif /* CONFIG_H_ */
