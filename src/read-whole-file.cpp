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
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
#include <vector>
#include <sstream>
#include <sys/stat.h>

static const size_t R = 7;

std::string read_whole_file_sstream(const char *fname) {
    std::ifstream file(fname, std::ios::binary);
    std::ostringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

std::string read_whole_file_iterator(const char *fname) {
    std::ifstream file(fname, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

std::string read_whole_file_seek(const char *fname) {
    std::ifstream file(fname, std::ios::binary);
    file.seekg(0, std::istream::end);
    std::size_t size(static_cast<size_t>(file.tellg()));

    file.seekg(0, std::istream::beg);

    std::string result(size, 0);
    file.read(&result[0], size);

    return result;
}

std::string read_whole_file_stat(const char *fname) {
    std::ifstream file(fname, std::ios::binary);
    struct stat st;
    stat(fname, &st);

    std::string result(static_cast<size_t>(st.st_size), 0);
    file.read(&result[0], static_cast<size_t>(st.st_size));

    return result;
}

std::string read_whole_file_stat_C(const char *fname) {
    FILE *file = fopen(fname, "rb");
    struct stat st;
    stat(fname, &st);

    std::string result(static_cast<size_t>(st.st_size), 0);
    fread(&result[0], 1, static_cast<size_t>(st.st_size), file);
	fclose(file);

    return result;
}

int main() {
//*
    {
        size_t tlen = 0;
        std::vector<double> timings;
        timings.reserve(R);
        for (size_t r=0 ; r<R ; ++r) {
            ticks start = getticks();
            std::string a = read_whole_file_sstream("random100");
            tlen += a.length();
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "Stringstream: ";
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
            std::string a = read_whole_file_iterator("random100");
            tlen += a.length();
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "Iterator: ";
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
            std::string a = read_whole_file_seek("random100");
            tlen += a.length();
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "Seek: ";
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
            std::string a = read_whole_file_stat("random100");
            tlen += a.length();
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "Stat: ";
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
            std::string a = read_whole_file_stat_C("random100");
            tlen += a.length();
            ticks end = getticks();
            double timed = elapsed(end, start);
            timings.push_back(timed);
        }

        std::cout << "Stat C: ";
        PrintStats(timings);
        std::cout << std::endl;
        std::cout << tlen << std::endl;
    }
//*/
}
