/*
 * Config.h
 *
 *  Created on: Oct 1, 2012
 *      Author: ubuntu
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <muriel/Url.h>

//////////////// ADD MORE SCHEMES HERE //////////////////

INIT_BEGIN
ADD_SCHEME(Url::FTP, "ftp");
ADD_SCHEME(Url::HTTP, "http");
ADD_SCHEME(Url::HTTPS, "https");
ADD_SCHEME(Url::RTSP, "rtsp");
ADD_SCHEME(Url::FILE, "file");
ADD_SCHEME(Url::DVD, "dvd");
ADD_SCHEME(Url::VCD, "vcd");
ADD_SCHEME(Url::BLURAY, "blueray");
ADD_SCHEME(Url::MAIL, "mailto");
ADD_SCHEME(Url::ABOUT, "about");
INIT_END

#endif /* CONFIG_H_ */
