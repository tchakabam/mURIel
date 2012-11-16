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

	Url::Url(const string& uri, bool bForceAbsolute)
	{
			init(uri, bForceAbsolute);
			m_range.b_None = true;
	}

	Url::Url(const string& uri, const ByteRange& range, bool bForceAbsolute)
	{
			init(uri, bForceAbsolute);
			m_range = range;
	}

	/*
	 *
	 *
	 * Parses URI according to IETF standard
	 *
	 */

	void Url::init(const UrlElement & uri, bool bForceAbsolute) {

				//SCHEME: //////////////////////////////////////////////////////////////
				/* Do we have a scheme ? */
				MURIEL_LOG("Parsing: %s", uri.c_str());

				size_t pHasScheme = uri.find("://");
				size_t scheme_end = 0;
				bool bScheme = false;

				if (pHasScheme != string::npos) {

					scheme_end = uri.find_first_of(":", 0);
					bScheme = true;

					scheme = uri.substr(0, scheme_end);

					MURIEL_LOG("Scheme: %s", scheme.c_str());
				}

				//HOST: /////////////////////////////////////////////////////////
				size_t host_begin = 0;
				size_t host_end;
				bool bHost = false;
				if( ( uri.at(0) != '/' && scheme.size() > 0 ) //in this case it should be absolute
									|| ( bForceAbsolute ) ) {

					if(bScheme) {
						host_begin = uri.find_first_not_of("/", scheme_end+1);
					} else {
						host_begin = 0;
					}

					host_end = uri.find_first_of("/", host_begin+1);

					if (host_end != string::npos) {

						host = uri.substr(host_begin, host_end - host_begin);

						bHost = true;

						MURIEL_LOG("Host: %s", host.c_str());
					}
				}

				//PATH: ///////////////////////////////////////////////////////////////

				size_t path_begin;
				size_t path_end = uri.find_first_of("?#", host_begin+1);

				if(bHost) path_begin = uri.find_first_of("/?#", host_begin+1);
				else path_begin = 0;

				if (path_begin == string::npos) {
					path = "/";
				} else if(path_end == string::npos) {
					path_end = uri.length();
				}

				path = uri.substr(path_begin, path_end-path_begin);

				MURIEL_LOG("Path: %s", path.c_str());

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

	Url::~Url() {
		MURIEL_LOG("Destroying: %u", this);
	}

/*
	void Url::enroll(UrlListener* const ul) {
		m_Listeners.push_back(ul);
	}
*/

	void Url::remove(Part p) {
		switch(p) {
		case FULL:
			init("");
			break;
		case FILENAME:
			path = toDirectory();
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

		//fire(p);
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

		//fire(p);
	}

	bool Url::makeAbsolute (const Url& base) {
		if (! scheme.empty()) {
			// url is already absolute, nothing to do
			return true;
		}
		scheme = base.scheme;

		if (! host.empty()) {
			return true;
		}
		host = base.host;
		port = base.port;

		if (! path.empty()) {
		       if (path.at(0) == '/') {
			       // path is already absolute, keep it
			       return true;
		       }
		       if (base.path.at(0) != '/') {
			       // base path is not absolute, handle it like a '/' and make the
			       // current path absolute.
			       path.insert (0, 1, '/');
			       return false;
		       }
		       // relative path, resolve it

		       // get start of base's last path segment
		       int baseLastSeg = base.path.find_last_of ('/');
		       // will never return string::npos, since we checked for a leading '/' already.

		       // handle initial '.' and '..' path segments
		       const char *rel_path = path.c_str();
		       int offset = 0;
		       bool too_few_base_segments = false;
		       char c;

		       while ((c = rel_path[offset]) && c == '.' && baseLastSeg > 0)
		       {
			       // first segment begins with a '.'
			       bool shift_segment = false;
			       int num_skip = 1;

			       c = rel_path[offset+1];

			       if (c == '.' )
			       {
				       c = rel_path[offset+2];
				       shift_segment = true;
				       num_skip += 1;
			       }

			       if (c == '/')
			       {
				       // this was either a './' or a '../' prefix
				       num_skip += 1;
			       }
			       else if (c == 0)
			       {
				       // end of path segment
			       }
			       else
			       {
				       // not a special segment.
				       // take the remainig path as it is
				       num_skip = 0;
				       break;
			       }

			       if (shift_segment)
			       {
				       if (baseLastSeg <= 0) {
					       too_few_base_segments = true;
					       break;
				       }
				       // skip last segment (this will reduce baseLastSeg)
				       baseLastSeg = base.path.find_last_of ('/', baseLastSeg-1);
				       // will never return string::npos, since we checked for a leading '/' already.
			       }

			       offset += num_skip;
		       }

		       // replace the prefix of '..' and '.' segments by the remaining segments of the base path (including the '/')
		       path.replace (0, offset, base.path, 0, baseLastSeg + 1);

		       return ! too_few_base_segments;
		}

		if (! query.empty()) {
			// we already have a query part
			return true;
		}
		query = base.query;

		if (! fragment.empty()) {
			// we already have a fragment part
			return true;
		}
		fragment = base.fragment;

		return true;
	}

	char Url::getAnalysis() {

		if( isAbsolute() ) return MURIEL_ABSOLUTE;

		if( isRelative() ) return MURIEL_RELATIVE;

		if ( isMalformed() ) return MURIEL_MALFORMED;

		return MURIEL_ERROR;
	}

	bool Url::isAbsolute() {

		return scheme.length() > 0
				&& host.length() > 0;

	}

	bool Url::isRelative() {

		return host.length() == 0 || scheme.length() == 0;

	}

	bool Url::isMalformed() {

		return (host.length() == 0 && scheme.length() != 0)
				|| (host.length() != 0 && scheme.length() == 0)
				|| (host.length() == 0 && scheme.length() == 0 && path.length() == 0);
	}

	bool Url::isUsingRange() const {return ! m_range.b_None;}

	ByteRange & Url::range() {
			return m_range;
	}

	short int Url::toPortAsInt() const { return (short int) atoi(port.c_str()); }

	void Url::swap (Url& other)
	{
		scheme.swap(other.scheme);
		port.swap(other.port);
		host.swap(other.host);
		path.swap(other.path);
		query.swap(other.query);
		fragment.swap(other.fragment);

		ByteRange tmp(m_range);
		m_range = other.m_range;
		other.m_range = tmp;
	}

    /*
     *
     * gives the whole URI concatenated in a string leaving optional or not present params out so
     * it could be passed to a standard HTTP or FTP client for example
     */
	UrlElement Url::toFull() const{

			stringstream stream;

			if(scheme.length() != 0) {
									     stream << scheme << "://" ;
			}

			stream << host;

			if ( port.length() != 0 )  {  stream << ":" << port; }

			stream << path;

			if ( query.length() != 0)  {  stream << "?" << query; }
			if ( fragment.length() != 0) { stream << "#" << fragment; }

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
/*
	void Url::fire(Part p) {
		std::vector<UrlListener*>::iterator it = m_Listeners.begin();
		while(it!=m_Listeners.end()) {
			(*it++)->onChanged(this, p);
		}
	}
*/





