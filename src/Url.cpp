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

	Url::Url() {
		//
	}

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

				if(uri.size() == 0) return;

				//SCHEME: //////////////////////////////////////////////////////////////
				/* Do we have a scheme ? */
				MURIEL_LOG("Parsing: %s", uri.c_str());

				const char *start = uri.c_str();
				const char *cstr = start;
				size_t scheme_end = 0;
				bool bScheme = false;

				if (isalpha (cstr[0])) {
					cstr += 1;
					while (isalnum (*cstr) || *cstr == '+' || *cstr == '-' || *cstr == '.') {
						cstr += 1;
					}
					if (*cstr == ':') {
						bScheme = true;
						scheme_end = std::distance (start, cstr);
						scheme = uri.substr(0, scheme_end);
						scheme_end += 1; // skip ':'

						MURIEL_LOG("Scheme: %s", scheme.c_str());
					}
				}

				//HOST: /////////////////////////////////////////////////////////
				size_t host_begin = scheme_end;
				size_t host_end = scheme_end;
				bool bHost = false;
				/*
				if( ( uri.at(0) != '/' && scheme.size() > 0 ) //in this case it should be absolute
									|| ( bForceAbsolute ) ) {
				*/
				if( host_begin+2 < uri.size() && uri.at(host_begin) == '/' && uri.at(host_begin+1) == '/' ) {

					host_begin += 2;
					host_end = uri.find_first_of("/?#", host_begin);

					if (host_end == string::npos) {
						host_end = uri.length();
					}

					host = uri.substr(host_begin, host_end - host_begin);

					bHost = true;

					MURIEL_LOG("Host: %s", host.c_str());
				}

				//PATH: ///////////////////////////////////////////////////////////////

				size_t path_begin = host_end;
				size_t path_end = host_end;

				if (path_begin < uri.size()) {
					path_end = uri.find_first_of("?#", host_end);
					
					if(path_end == string::npos) {
						path_end = uri.length();
					}
					path = uri.substr(path_begin, path_end-path_begin);

					if (path.empty() && bForceAbsolute) {
						path = "/";
					}
				}
#if 0
				if (path_begin < uri.size() && uri.at(path_begin) != '/') {
					path = "/";
				} else {
					path_end = uri.find_first_of("?#", host_end+1);
					
					if(path_end == string::npos) {
						path_end = uri.length();
					}
					path = uri.substr(path_begin, path_end-path_begin);
				}
#endif


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

				//query begin
				
				size_t query_begin = path_end;
				size_t query_end   = path_end;
				if (query_begin < uri.size() && uri.at(query_begin) == '?') {
					query_begin += 1;
					query_end = uri.find_first_of("#", query_begin);
					if(query_end == string::npos) {
						query_end = uri.length();
					}
					query = uri.substr(query_begin, query_end-query_begin);
				}

				//fragment begin
				size_t fragment_begin = query_end;
				size_t fragment_end   = query_end;
				if (fragment_begin < uri.size() && uri.at(fragment_begin) == '#') {
					fragment_begin += 1;
					fragment = uri.substr(fragment_begin);
				}
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

		if (! path.empty() || ! query.empty()) {
		       if (! path.empty() && path.at(0) == '/') {
			       // path is already absolute, keep it
			       return true;
		       }
		       if (base.path.empty() || base.path.at(0) != '/') {
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

		// only a fragment or an empty string has been given in the URI reference
		// so according to the RFC this refers the ducument currently loaded.
		// Since this case is not (yet) handled by the Url class, the parts
		// not given in the URI reference are all taken from the base URI
		// to create a URI that refers to the source of the current document.
		// This approach may lead to non-conformant results if used e.g. within
		// documents retrieved via HTTP POST.

		path = base.path;
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

			if(path[0] != '/') { stream << "/" << path; }
			else { stream << path; }

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

#ifdef ENABLE_TESTS

#include <stdio.h>

static class UrlTests {
	int m_numTests;
	int m_numFailedTests;

	void checkUrlElement (const UrlElement& elem, const char *result)
	{
		m_numTests += 1;
		if (elem != result) {
			fprintf (stderr, "Test %d: %s: failed: expected '%s' instead of '%s'\n", m_numTests, __FUNCTION__, result, elem.c_str());
			m_numFailedTests += 1;
		}
	}

	void checkAbsolute (const Url& base, const char *relUrl, const char *result, bool retval = true)
	{
		bool success = true;
		m_numTests += 1;
		Url url(relUrl, false);
		bool ret = url.makeAbsolute (base);
		UrlElement absUrl (url.toFull());
		if (absUrl != result) {
			fprintf (stderr, "Test %d: %s: failed: expected '%s' instead of '%s' (relUrl = '%s', base = '%s', ret = %d)\n", m_numTests, __FUNCTION__, result, absUrl.c_str(), relUrl, base.toFull().c_str(), (int)ret);
			Url url2(relUrl, false);
			fprintf (stderr, "\trelUrl = '%s', scheme = '%s', host = '%s', path = '%s', query = '%s', fragment = '%s'\n", url2.toFull().c_str(), url2.getScheme().c_str(), url2.getHost().c_str(), url2.getPath().c_str(), url2.getQuery().c_str(), url2.getFragment().c_str());
			success = false;
		}
		if (!!ret != !!retval) {
			fprintf (stderr, "Test %d: %s: return value mismatch: expected %d instead of %d (relUrl = '%s', base = '%s')\n", m_numTests, __FUNCTION__, (int)retval, (int)ret, relUrl, base.toFull().c_str());
			success = false;
		}

		if (! success)
			m_numFailedTests += 1;
	}

public:
	UrlTests () 
	: m_numTests (0)
	, m_numFailedTests (0)
	{
		Url base ("http://a/b/c/d;p?q");

		checkUrlElement (base.getScheme(), "http");
		checkUrlElement (base.getHost(), "a");
		checkUrlElement (base.getPath(), "/b/c/d;p");
		checkUrlElement (base.getQuery(), "q");
		checkUrlElement (base.toFull(), "http://a/b/c/d;p?q");

		Url simple ("g:h");

		checkUrlElement (simple.getScheme(), "g");
		checkUrlElement (simple.getHost(), "");
		checkUrlElement (simple.getPath(), ""); // the parser does not handle this kind of URI yet
		checkUrlElement (simple.getQuery(), "");

		// Normal Cases (RFC2396 C.1)
		checkAbsolute (base, "g:h", "g:h");     // the parser does not handle this kind of URI yet
		checkAbsolute (base, "g", "http://a/b/c/g");
		checkAbsolute (base, "./g", "http://a/b/c/g");
		checkAbsolute (base, "g/", "http://a/b/c/g/");
		checkAbsolute (base, "/g", "http://a/g");
		checkAbsolute (base, "//g", "http://g");
		checkAbsolute (base, "?y", "http://a/b/c/?y");
		checkAbsolute (base, "g?y", "http://a/b/c/g?y");
		checkAbsolute (base, "#s", "http://a/b/c/d;p?q#s");
		checkAbsolute (base, "g#s", "http://a/b/c/g#s");
		checkAbsolute (base, "g?y#s", "http://a/b/c/g?y#s");
		checkAbsolute (base, ";x", "http://a/b/c/;x");
		checkAbsolute (base, "g;x", "http://a/b/c/g;x");
		checkAbsolute (base, "g;x?y#s", "http://a/b/c/g;x?y#s");
		checkAbsolute (base, ".", "http://a/b/c/");
		checkAbsolute (base, "./", "http://a/b/c/");
		checkAbsolute (base, "..", "http://a/b/");
		checkAbsolute (base, "../", "http://a/b/");
		checkAbsolute (base, "../g", "http://a/b/g");
		checkAbsolute (base, "../..", "http://a/");
		checkAbsolute (base, "../../", "http://a/");
		checkAbsolute (base, "../../g", "http://a/g");

		// Abnormal Cases (RFC2396 C.2)
		checkAbsolute (base, "../../../g", "http://a/../g");
		checkAbsolute (base, "../../../../g", "http://a/../../g");
		checkAbsolute (base, "/./g", "http://a/./g");
		checkAbsolute (base, "/../g", "http://a/../g");
		checkAbsolute (base, "g.", "http://a/b/c/g.");
		checkAbsolute (base, ".g", "http://a/b/c/.g");
		checkAbsolute (base, "g..", "http://a/b/c/g..");
		checkAbsolute (base, "..g", "http://a/b/c/..g");
		checkAbsolute (base, "./../g", "http://a/b/g");
		// checkAbsolute (base, "./g/.", "http://a/b/c/g/");  // the implementation does not normalize within the relative URI reference
		// checkAbsolute (base, "g/./h", "http://a/b/c/g/h"); // ditto
		// checkAbsolute (base, "g/../h", "http://a/b/c/h"); // ditto
		// checkAbsolute (base, "g;x=1/./y", "http://a/b/c/g;x=1/y"); // ditto
		// checkAbsolute (base, "g;x=1/../y", "http://a/b/c/y"); // ditto
		checkAbsolute (base, "g?y/./x", "http://a/b/c/g?y/./x");
		checkAbsolute (base, "g?y/../x", "http://a/b/c/g?y/../x");
		checkAbsolute (base, "g#s/./x", "http://a/b/c/g#s/./x");
		checkAbsolute (base, "g#s/../x", "http://a/b/c/g#s/../x");


		fprintf (stderr, "Test Result: %d/%d ok\n", m_numTests - m_numFailedTests, m_numTests);
	}
} myUrlTests;

#endif

