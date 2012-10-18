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

#ifndef BIMAP_H_
#define BIMAP_H_

#include <map>

//Bi-Directional map

template<typename TA, typename TB>
class BiMap {

public:

	typedef std::map<TA, TB> LeftMap;
	typedef std::map<TA, TB> RightMap;

	void insert(std::pair<TA, TB>& in) {
		map0.insert(in);
		map1.insert(std::make_pair(in.second, in.first));
	}

	void insert(std::pair<TB, TA>& in) {
		map1.insert(in);
		map0.insert(std::make_pair(in.second, in.first));
	}

	//////////////// Getting it in is easy, the tricky part is when you want to remove it...

	void erase(std::map<TA, TB>::iterator it) {
		TB b = it->second;
		map0.erase(it);
		map1.erase(b);
	}

	void erase(std::map<TA, TB>::iterator it1, std::map<TA, TB>::iterator it2) {
		std::map<TB, TA>::iterator B_it1 = map1.find(it1->second);
		std::map<TB, TA>::iterator B_it2 = map1.find(it2->second);
		map0.erase(it1, it2);
		if(B_it1 < B_it2)
			map1.erase(B_it1, B_it2);
		else map1.erase(B_it2, B_it1);
	}

	void erase(std::map<TA, TB>::const_iterator it) {
		TB b = it->second;
		map0.erase(it);
		map1.erase(b);
	}

	void erase(std::map<TA, TB>::const_iterator it1, std::map<TA, TB>::const_iterator it2) {
		std::map<TB, TA>::const_iterator B_it1 = map1.find(it1->second);
		std::map<TB, TA>::const_iterator B_it2 = map1.find(it2->second);
		map0.erase(it1, it2);
		if(B_it1 < B_it2)
			map1.erase(B_it1, B_it2);
		else map1.erase(B_it2, B_it1);
	}

	void erase(TA a) {
		std::map<TB, TA>::iterator B_it1 = map1.find(a);
		map0.erase(a);
		map1.erase(B_it1);
	}

	////////////////////////////////

	void erase(std::map<TB, TA>::iterator it) {
		TA a = it->second;
		map1.erase(it);
		map0.erase(a);
	}

	void erase(std::map<TB, TA>::iterator it1, std::map<TB, TA>::iterator it2) {
		std::map<TA, TB>::iterator A_it1 = map0.find(it1->second);
		std::map<TA, TB>::iterator A_it2 = map0.find(it2->second);
		map1.erase(it1, it2);
		if(A_it1 < A_it2)
			map1.erase(A_it1, A_it2);
		else map1.erase(A_it2, A_it1);
	}

	void erase(std::map<TB, TA>::const_iterator it) {
		TA a = it->second;
		map1.erase(it);
		map0.erase(a);
	}

	void erase(std::map<TB, TA>::const_iterator it1, std::map<TB, TA>::const_iterator it2) {
		std::map<TA, TB>::const_iterator A_it1 = map0.find(it1->second);
		std::map<TA, TB>::const_iterator A_it2 = map0.find(it2->second);
		map1.erase(it1, it2);
		if(A_it1 < A_it2)
			map1.erase(A_it1, A_it2);
		else map1.erase(A_it2, A_it1);
	}

	void erase(TB b) {
		std::map<TB, TA>::iterator A_it1 = map1.find(a);
		map0.erase(b);
		map1.erase(A_it1);
	}

	///////////////

	void clear() {
		map0.clear();
		map1.clear();
	}

	const LeftMap & getLeft() const { return map0; }
	const RightMap & getRight() const { return map1; }

private:

	std::map<TA, TB> map0;
	std::map<TB, TA> map1;

};

#endif /* BIMAP_H_ */
