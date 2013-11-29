/*
* Copyright (C) 2012, Tino Didriksen Consult
* Developed by Tino Didriksen <tino@didriksen.cc>
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
#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

static const int N = 100000;
static const size_t R = 7;

int naive(const char *p) {
	int x = 0;
	bool neg = false;
	if (*p == '-') {
		neg = true;
		++p;
	}
	while (*p >= '0' && *p <= '9') {
		x = (x * 10) + (*p - '0');
		++p;
	}
	if (neg) {
		x = -x;
	}
	return x;
}

int main() {
	std::vector<std::string> nums;
	nums.reserve(N);
	for (int i = 0 - (N / 2); i < N / 2; ++i) {
		std::string y = boost::lexical_cast<std::string>(i);
		nums.push_back(y);
	}

	{
		int tsum = 0.0;
		std::vector<double> timings;
		timings.reserve(R);
		for (size_t r = 0; r < R; ++r) {
			ticks start = getticks();
			for (size_t i = 0; i < nums.size(); ++i) {
				int x = naive(nums[i].c_str());
				tsum += x;
			}
			ticks end = getticks();
			double timed = elapsed(end, start);
			timings.push_back(timed);
		}

		std::cout << "naive: ";
		PrintStats(timings);
		std::cout << std::endl;
		std::cout << tsum << std::endl;
	}

	{
		int tsum = 0.0;
		std::vector<double> timings;
		timings.reserve(R);
		for (size_t r = 0; r < R; ++r) {
			ticks start = getticks();
			for (size_t i = 0; i < nums.size(); ++i) {
				int x = atoi(nums[i].c_str());
				tsum += x;
			}
			ticks end = getticks();
			double timed = elapsed(end, start);
			timings.push_back(timed);
		}

		std::cout << "atoi(): ";
		PrintStats(timings);
		std::cout << std::endl;
		std::cout << tsum << std::endl;
	}

	{
		int tsum = 0.0;
		std::vector<double> timings;
		timings.reserve(R);
		for (size_t r = 0; r < R; ++r) {
			ticks start = getticks();
			for (size_t i = 0; i < nums.size(); ++i) {
				int x = strtol(nums[i].c_str(), 0, 10);
				tsum += x;
			}
			ticks end = getticks();
			double timed = elapsed(end, start);
			timings.push_back(timed);
		}

		std::cout << "strtol(): ";
		PrintStats(timings);
		std::cout << std::endl;
		std::cout << tsum << std::endl;
	}

	{
		int tsum = 0.0;
		std::vector<double> timings;
		timings.reserve(R);
		for (size_t r = 0; r < R; ++r) {
			ticks start = getticks();
			for (size_t i = 0; i < nums.size(); ++i) {
				int x = 0;
				sscanf(nums[i].c_str(), "%d", &x);
				tsum += x;
			}
			ticks end = getticks();
			double timed = elapsed(end, start);
			timings.push_back(timed);
		}

		std::cout << "sscanf(): ";
		PrintStats(timings);
		std::cout << std::endl;
		std::cout << tsum << std::endl;
	}

	{
		int tsum = 0.0;
		std::vector<double> timings;
		timings.reserve(R);
		for (size_t r = 0; r < R; ++r) {
			ticks start = getticks();
			for (size_t i = 0; i < nums.size(); ++i) {
				int x = boost::lexical_cast<int>(nums[i]);
				tsum += x;
			}
			ticks end = getticks();
			double timed = elapsed(end, start);
			timings.push_back(timed);
		}

		std::cout << "lexical_cast: ";
		PrintStats(timings);
		std::cout << std::endl;
		std::cout << tsum << std::endl;
	}

	{
		using boost::spirit::qi::int_;
		using boost::spirit::qi::parse;
		int tsum = 0.0;
		std::vector<double> timings;
		timings.reserve(R);
		for (size_t r = 0; r < R; ++r) {
			ticks start = getticks();
			for (size_t i = 0; i < nums.size(); ++i) {
				int x = 0.0;
				char const *str = nums[i].c_str();
				parse(str, &str[nums[i].size()], int_, x);
				tsum += x;
			}
			ticks end = getticks();
			double timed = elapsed(end, start);
			timings.push_back(timed);
		}

		std::cout << "spirit qi: ";
		PrintStats(timings);
		std::cout << std::endl;
		std::cout << tsum << std::endl;
	}

	{
		int tsum = 0.0;
		std::vector<double> timings;
		timings.reserve(R);
		for (size_t r = 0; r < R; ++r) {
			ticks start = getticks();
			for (size_t i = 0; i < nums.size(); ++i) {
				std::istringstream ss(nums[i]);
				int x = 0;
				ss >> x;
				tsum += x;
			}
			ticks end = getticks();
			double timed = elapsed(end, start);
			timings.push_back(timed);
		}

		std::cout << "stringstream: ";
		PrintStats(timings);
		std::cout << std::endl;
		std::cout << tsum << std::endl;
	}

	{
		int tsum = 0.0;
		std::vector<double> timings;
		timings.reserve(R);
		for (size_t r = 0; r < R; ++r) {
			ticks start = getticks();
			std::istringstream ss;
			for (size_t i = 0; i < nums.size(); ++i) {
				ss.str(nums[i]);
				ss.clear();
				int x = 0;
				ss >> x;
				tsum += x;
			}
			ticks end = getticks();
			double timed = elapsed(end, start);
			timings.push_back(timed);
		}

		std::cout << "stringstream reused: ";
		PrintStats(timings);
		std::cout << std::endl;
		std::cout << tsum << std::endl;
	}
}
