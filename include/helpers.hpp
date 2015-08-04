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

#pragma once
#ifndef c6d28b7452ec699b_HELPERS_HPP
#define c6d28b7452ec699b_HELPERS_HPP

#ifdef _MSC_VER
	// warning C4258: definition from the for loop is ignored; the definition from the enclosing scope is used
	#pragma warning (disable: 4258)
	#pragma conform(forScope, on)
	// warning C4127: conditional expression is constant
	#pragma warning (disable: 4127)
	// warning C4428: universal-character-name encountered in source
	#pragma warning (disable: 4428)
	// warning C4512: assignment operator could not be generated
	#pragma warning (disable: 4512)
	// warning C4480: nonstandard extension used: specifying underlying type for enum
	// 'cause that is actually standard in C++11
	#pragma warning (disable: 4480)
	// warning C4456: declaration hides previous local declaration
	#pragma warning (disable: 4456)
	// warning C4458: declaration hides class member
	#pragma warning (disable: 4458)
	// warning C4459: declaration hides global
	#pragma warning (disable: 4459)
#endif

#include <stdint.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <limits>
#include <vector>
#include <iomanip>

inline double PrintStats(std::vector<double> timings) {
    double fastest = std::numeric_limits<double>::max();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[";
    for (size_t i = 0 ; i<timings.size()-1 ; ++i) {
        fastest = std::min(fastest, timings[i]);
        std::cout << timings[i] << ",";
    }
    std::cout << timings.back();
    std::cout << "]";

    double sum = 0.0;
    for (size_t i = 0 ; i<timings.size() ; ++i) {
        sum += timings[i];
    }
    double avg = sum / double(timings.size()-1);

    sum = 0.0;
    for (size_t i = 0 ; i<timings.size() ; ++i) {
        timings[i] = pow(timings[i]-avg, 2);
        sum += timings[i];
    }
    double var = sum/(timings.size()-2);
    double sdv = sqrt(var);

    std::cout << " with fastest " << fastest << ", average " << avg << ", stddev " << sdv;
    return fastest;
}


inline size_t checkvalue(uint32_t t) {
    return t;
}
inline size_t checkvalue(const std::string& t) {
    return t.length();
}

#endif
