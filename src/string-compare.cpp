/*
* Copyright (C) 2015, Tino Didriksen <mail@tinodidriksen.com>
*
* This file is part of Benchmarks
*
* Benchmarks is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Benchmarks is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Benchmarks.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cycle.h>
#include <helpers.hpp>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
#include <vector>
#include <iomanip>
#include <stdint.h>

static const size_t N = 1000000;
static const size_t R = 7;

inline bool naive_char(const char *a, const char *b) {
	while (*a || *b) {
		if (*a != *b) {
			return false;
		}
		++a;
		++b;
	}
	return true;
}

inline bool naive_string(const std::string& a, const std::string& b) {
	if (a.length() == b.length()) {
		return naive_char(a.c_str(), b.c_str());
	}
	return false;
}

int main() {
	setlocale(LC_ALL, "C");

	std::vector<const char*> chars;
    std::vector<std::string> strings;
    chars.reserve(N);
    strings.reserve(N);
    for (size_t i=0 ; i<N ; ++i) {
        char *s = new char[12];
        sprintf(s, "%u", uint32_t(i));
        chars.push_back(s);
        strings.push_back(s);
        strings.back().c_str();
    }

//*
    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            for (size_t i=1 ; i<N ; ++i) {
                if (naive_char(chars[i], chars[i-1])) {
                    ++tlen;
                }
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "naive(char*,char*): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }
//*/

//*
    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            for (size_t i=1 ; i<N ; ++i) {
                if (naive_string(strings[i], strings[i-1])) {
                    ++tlen;
                }
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "naive(string,string): ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }
//*/

//*
    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            for (size_t i=1 ; i<N ; ++i) {
                if (strings[i] == strings[i-1]) {
                    ++tlen;
                }
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "string == string: ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }
//*/

//*
    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            for (size_t i=1 ; i<N ; ++i) {
                if (strings[i] == chars[i-1]) {
                    ++tlen;
                }
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "string == char*: ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }
//*/

//*
    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            for (size_t i=1 ; i<N ; ++i) {
                if (strcmp(chars[i], chars[i-1]) == 0) {
                    ++tlen;
                }
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "strcmp(char*,char*) == 0: ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }
//*/

//*
    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            for (size_t i=1 ; i<N ; ++i) {
                if (strcmp(chars[i], strings[i-1].c_str()) == 0) {
                    ++tlen;
                }
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "strcmp(char*,string.c_str()) == 0: ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }
//*/

//*
    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            for (size_t i=1 ; i<N ; ++i) {
                if (strcmp(strings[i].c_str(), strings[i-1].c_str()) == 0) {
                    ++tlen;
                }
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "strcmp(string.c_str(),string.c_str()) == 0: ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }
//*/

//*
    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            for (size_t i=1 ; i<N ; ++i) {
                if (strings[i].compare(strings[i-1]) == 0) {
                    ++tlen;
                }
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "string.compare(string) == 0: ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }
//*/

//*
    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            for (size_t i=1 ; i<N ; ++i) {
                if (strings[i].compare(chars[i-1]) == 0) {
                    ++tlen;
                }
            }
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "string.compare(char*) == 0: ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }
//*/

	for (size_t i=0 ; i<N ; ++i) {
    	delete[] chars[i];
    }
}
