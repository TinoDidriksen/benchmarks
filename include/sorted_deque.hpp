/*
* Copyright (C) 2007-2011, GrammarSoft ApS
* Developed by Tino Didriksen <tino@didriksen.cc>
* Design by Eckhard Bick <eckhard.bick@mail.dk>, Tino Didriksen <tino@didriksen.cc>
*
* This file is part of VISL CG-3
*
* VISL CG-3 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* VISL CG-3 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with VISL CG-3.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#ifndef c6d28b7452ec699b_SORTED_DEQUE_HPP
#define c6d28b7452ec699b_SORTED_DEQUE_HPP
#include <iostream>
#include <deque>
#include <algorithm>
#include <functional>
#include <stdint.h> // C99 or C++0x or C++ TR1 will have this header. ToDo: Change to <cstdint> when C++0x broader support gets under way.

namespace CG3 {

template<typename T, typename Comp = std::less<T> >
class sorted_deque {
private:
	typedef typename std::deque<T> Cont;
	typedef typename Cont::iterator iterator;
	Cont elements;
	Comp comp;

public:
	typedef typename Cont::const_iterator const_iterator;
	typedef typename Cont::size_type size_type;
	typedef T value_type;
	typedef T key_type;

	#ifdef CG_TRACE_OBJECTS
	sorted_deque() {
		std::cerr << "OBJECT: " << __PRETTY_FUNCTION__ << std::endl;
	}

	~sorted_deque() {
		std::cerr << "OBJECT: " << __PRETTY_FUNCTION__ << ": " << elements.size() << std::endl;
	}
	#endif

	bool insert(T t) {
		if (elements.empty()) {
			elements.push_back(t);
			return true;
		}
		/*
		else if (comp(elements.back(), t)) {
			elements.push_back(t);
			return true;
		}
		else if (comp(t, elements.front())) {
			elements.insert(elements.begin(), t);
			return true;
		}
		//*/
		iterator it = std::lower_bound(elements.begin(), elements.end(), t, comp);
		if (it == elements.end() || comp(*it, t) || comp(t, *it)) {
			elements.insert(it, t);
			return true;
		}
		return false;
	}

	bool push_back(T t) {
		return insert(t);
	}

	bool erase(T t) {
		if (elements.empty()) {
			return false;
		}
		else if (comp(elements.back(), t)) {
			return false;
		}
		else if (comp(t, elements.front())) {
			return false;
		}
		iterator it = lower_bound(t);
		if (it != elements.end() && !comp(*it, t) && !comp(t, *it)) {
			elements.erase(it);
			return true;
		}
		return false;
	}

	const_iterator erase(const_iterator it) {
		size_type o = std::distance<const_iterator>(elements.begin(), it);
		return elements.erase(elements.begin() + o);
	}

	const_iterator find(T t) const {
		if (elements.empty()) {
			return elements.end();
		}
		else if (comp(elements.back(), t)) {
			return elements.end();
		}
		else if (comp(t, elements.front())) {
			return elements.end();
		}
		const_iterator it = lower_bound(t);
		if (it != elements.end() && (comp(*it, t) || comp(t, *it))) {
			return elements.end();
		}
		return it;
	}

	const_iterator begin() const {
		return elements.begin();
	}

	const_iterator end() const {
		return elements.end();
	}

	T front() const {
		return elements.front();
	}

	T back() const {
		return elements.back();
	}

	iterator lower_bound(T t) {
		return std::lower_bound(elements.begin(), elements.end(), t, comp);
	}

	const_iterator lower_bound(T t) const {
		return std::lower_bound(elements.begin(), elements.end(), t, comp);
	}

	const_iterator upper_bound(T t) const {
		return std::upper_bound(elements.begin(), elements.end(), t, comp);
	}

	size_type size() const {
		return elements.size();
	}

	bool empty() const {
		return elements.empty();
	}

	void clear() {
		elements.clear();
	}
};

typedef sorted_deque<uint32_t> uint32SortedDeque;

}

#endif
