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
#ifndef URI_H_
#define URI_H_

#include <vector>
#include <map>
#include <string>
#include <sstream>

#include <muriel/Types.h>
#include <muriel/Macros.h>
#include <muriel/ByteRange.h>

class UrlListener;

class Url {
	public:
		enum Scheme {
			FTP,
			HTTP,
			HTTPS,
			RTSP,
			MAIL,
			FILE,
			DVD,
			VCD,
			BLURAY,
			ABOUT,
			CALLTO,
			DAV,
			SKYPE,
			SPOTIFY,
			XMPP,
			IM,
			SIP,
			MMS,
			IMAP,
			DNS,
			SSH,
			GIT,
			CONTENT,
			CHROME,
			EDONKEY2K,
			BITTORRENT,
			MPEGTS,
			SVN_HTTP,
			SVN_SSH,
			JAVASCRIPT
		};

		enum Part {
			USER,
			SCHEME,
			PORT,
			HOST,
			PATH,
			QUERY,
			FRAGMENT,
			FILENAME,
			DIRECTORY,
			RELATIVE,
			ABSOLUTE,
			BASE,
			FULL
		};

		Url();
		//Url(const Url& url);
		Url(const UrlElement& uri);
		Url(const UrlElement& uri, const ByteRange& range);

		~Url();

		//void enroll(UrlListener* const ul);

		void remove(Part p);
		void appendTo(Part p, const UrlElement  &e);
		void alter(Part p, const UrlElement & e);

		bool isUsingRange() const;
		ByteRange & range();

		const ByteRange & getRange() const;
		const UrlElement& getScheme() const;
		const UrlElement& getPort() const;
		const UrlElement& getHost() const;
		const UrlElement& getPath() const;
		const UrlElement& getQuery() const;
		const UrlElement& getFragment() const;

		UrlElement toFile() const;
		UrlElement toDirectory() const;

		UrlElement toBase(bool bWithTrailingBackslash=true, bool bTruncAtHost=false) const;
		UrlElement toFull() const;
		UrlElement toPartial(bool bScheme, bool bHost, bool bPort, bool bDir, bool bFile, bool bQuery, bool bFragment) const;

		short int toPortAsInt() const;

	protected:
		void init(const UrlElement &uri);
		//void fire(Part p);
	private:
		UrlElement scheme;
		UrlElement port;
		UrlElement host;
		UrlElement path;
		UrlElement query;
		UrlElement fragment;
		ByteRange m_range;
		//std::vector<UrlListener*> m_Listeners;
};

static std::map<Url::Scheme, UrlElement> SchemeMap;

class UrlListener {
public:
	virtual ~UrlListener();

	virtual void onChanged(Url*const url, Url::Part e) = 0;

};

class UrlValidator {

	bool scheme();
	bool host();
	bool port();
	bool path();
	bool query();
	bool fragment();
	bool file();

	bool isAddress();
	bool isRelative();
	bool isAbsolute();
	bool isComplete();
	bool isMinimal();
	bool isAmbigious();
};

class UrlMap {

};

class UrlFactory {

};

#ifdef AUTO_INCLUDE_SCHEMES
#include <muriel/Schemes.h>
#endif

#endif /* URI_H_ */
