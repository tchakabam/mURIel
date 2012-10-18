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

using namespace std;

	Url::Url(const string& uri)
	{
			init(uri);
			m_range.b_None = true;
	}

	Url::Url(const string& uri, const ByteRange& range)
	{
			init(uri);
			m_range = range;
	}

	Url::Url(const Url& url) {
			scheme = url.getScheme();
			port = url.getPort();
			host = url.getHost();
			path = url.getPath();
			query = url.getQuery();
			fragment = url.getFragment();
			m_range = url.getRange();
			m_Listeners.clear();
	}

	/*
	 *
	 *
	 * Parses URI according to IETF standard
	 *
	 */

	void Url::init(const UrlElement & uri) {

				//SCHEME: //////////////////////////////////////////////////////////////
				/* Do we have a scheme ? */
				size_t pHasScheme = uri.find("://");
				size_t scheme_end = 0;
				if (pHasScheme != string::npos) {

					scheme_end = uri.find_first_of(":", 0);

				}
				scheme = uri.substr(0, scheme_end);

				//HOST: /////////////////////////////////////////////////////////
				size_t host_begin = 0;
				if (scheme_end != 0) {
					host_begin = uri.find_first_not_of("/", scheme_end+1);
				}
				size_t host_end = uri.find_first_of("/", host_begin+1);

				host = uri.substr(host_begin, host_end - host_begin);

				//PATH: ///////////////////////////////////////////////////////////////

				size_t path_begin = uri.find_first_of("/?#", host_begin+1);
				size_t path_end = uri.find_first_of("?#", host_begin+1);

				if (path_begin == string::npos) {
					path = "/";
				} else if(path_end == string::npos) {
					path_end = uri.length();
				}

				path = uri.substr(path_begin, path_end-path_begin);

				//PORT: ////////////////////////////////////////////////////////////////////////////

				size_t pPort = host.find(":", 0);
				if (pPort != string::npos ) {
						string hostCopy = host;
						port = hostCopy.substr(pPort + 1, hostCopy.length() - pPort - 1);
						host = hostCopy.substr(0, hostCopy.length() - port.length() -1); //rectify host
				}

				//////////////////////////////////////////////////////////////////////////

				//TODO: URI.cpp to be continued to support URI queries (?) and URI fragments (#)...
				/*
				 *
				 * TODO: support for query and fragment parts in URIs
				 */
				/*
				//query begin
				size_t query_begin = uri.find_first_of("?", host_begin+1);


				size_t query_end = uri.find_first_of("#", host_begin+1);

				//fragment begin
				size_t fragment_begin = uri.find_first_of("#", host_begin+1);

				size_t fragment_end = uri.length()-1;
*/
				query = "";
				fragment = "";


	}

	void Url::enroll(UrlListener* const ul) {
		m_Listeners.push_back(ul);
	}

	void Url::remove(Part p) {
		switch(p) {
		case FULL:
			init("");
			break;
		}
	}
	void Url::appendTo(Part p, const UrlElement  &e) {
		UrlElement url;
		switch(p) {
		case FULL:
			url = toFull();
			url.append(e);
			init(url);
			break;
		case SCHEME:
			scheme.append(e);
			break;
		case HOST:
			host.append(e);
			break;
		case PORT:
			port.append(e);
			break;
		case PATH:
			path.append(e);
			break;
		case QUERY:
			query.append(e);
			break;
		case FRAGMENT:
			fragment.append(e);
			break;
		case FILENAME:
			path.append(e);
			break;
		case DIRECTORY:
			url = toDirectory();
			url.append(e);
			url.append(toFile());
			path = url;
			break;
		default:
			break;
		}

		fire(p);
	}
	void Url::alter(Part p, const UrlElement & e) {
		switch(p) {
		case FULL:
			init(e);
			break;
		case SCHEME:
			scheme = e;
			break;
		case HOST:
			host = e;
			break;
		case PORT:
			port = e;
			break;
		case PATH:
			path = e;
			break;
		case QUERY:
			query = e;
			break;
		case FRAGMENT:
			fragment = e;
			break;
		default:
			break;
		}

		fire(p);
	}

	bool Url::isUsingRange() const {return ! m_range.b_None;}

	ByteRange & Url::range() {
			return m_range;
	}

	short int Url::toPortAsInt() const { return (short int) atoi(port.c_str()); }

    /*
     *
     * gives the whole URI concatenated in a string leaving optional or not present params out so
     * it could be passed to a standard HTTP or FTP client for example
     */
	UrlElement Url::toFull() const{

			stringstream stream;
									     stream << scheme << "://" << host;
			if ( port.length() != 0 )    stream << ":" << port;
										 stream << path;
			if ( query.length() != 0)    stream << "?" << query;
			if ( fragment.length() != 0) stream << "#" << fragment;

			return stream.str();
    }

	UrlElement Url::toBase(bool bWithTrailingBackslash, bool bTruncAtHost) const {
    	string url = toFull();
    	size_t file_begin = url.find_last_of('/');
    	if(file_begin == url.length()) return "";
    	if(bWithTrailingBackslash) {
    		return url.substr(0, file_begin+1);
    	} else {
    		return url.substr(0, file_begin);
    	}

    }

    /*
     *
     * gives only the file part of the path
     *
     */
    UrlElement Url::toFile() const {
			size_t file_begin = path.find_last_of('/');
			if(file_begin == path.length()) return "";
			return path.substr(file_begin+1, path.length() - file_begin - 1);
    }

    /*
     *
     * gives only the directory part of the path
     *
     */
    UrlElement Url::toDirectory() const {
		size_t file_begin = path.find_last_of('/');
		return path.substr(0, file_begin+1);
    }

    /*
     *
     * Get accessors
     *
     */
    const ByteRange & Url::getRange() const {
    	return m_range;
    }

    const UrlElement& Url::getQuery() const
    {
    	return this->query;
    }

    const UrlElement& Url::getPort() const
    {
    	return this->port;
    }

    const UrlElement& Url::getPath() const
    {
    	return this->path;
    }

    const UrlElement& Url::getFragment() const
    {
    	return this->fragment;
    }

    const UrlElement& Url::getHost() const
    {
    	return this->host;
    }

    const UrlElement& Url::getScheme() const
    {
    	return this->scheme;
    }

	void Url::fire(Part p) {
		std::list<UrlListener*>::iterator it = m_Listeners.begin();
		while(it!=m_Listeners.end()) {
			(*it++)->onChanged(this, p);
		}
	}






