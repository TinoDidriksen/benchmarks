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

#include <boost/unordered_set.hpp>
#include <boost/container/flat_set.hpp>
#include <sorted_vector.hpp>
#include <interval_vector.hpp>
#include <sorted_deque.hpp>
#include <tdc_trie.hpp>
#include <btree_set.h>
#include <safe_btree_set.h>
#ifdef _MSC_VER
    #include <sti/sset.h>
#endif
#include <set>
#include <unordered_set>

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

const size_t N = 1000000;
const size_t R = 7;

template<typename Cont, typename VT>
void runTest(const std::string& name, const VT& values) {
    std::cout << "Testing " << name << " ..." << std::endl;

    std::vector< std::vector<double> > timings(4);

    for (size_t r=0 ; r<R ; ++r) {
        Cont Set;
        ticks start, end;
        double timed = 0.0;
        size_t res = 0;

        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            Set.insert(values[i]);
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[0].push_back(timed);

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            typename Cont::const_iterator it = Set.find(values[i]);
            if (it != Set.end()) {
                res += checkvalue(*it);
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[1].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        size_t n = 0;
        start = getticks();
        for (typename Cont::const_iterator it = Set.begin(); it != Set.end() ; ++it) {
            res += checkvalue(*it);
            ++n;
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[2].push_back(timed);
        std::cerr << res << std::endl;
        std::cerr << n << std::endl;

        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            Set.erase(values[i]);
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[3].push_back(timed);
    }

    std::cout << name << " insertion: ";
    timings[0][0] = PrintStats(timings[0]);
    std::cout << std::endl;

    std::cout << name << " lookup: ";
    timings[1][0] = PrintStats(timings[1]);
    std::cout << std::endl;

    std::cout << name << " iterate: ";
    timings[2][0] = PrintStats(timings[2]);
    std::cout << std::endl;

    std::cout << name << " erase: ";
    timings[3][0] = PrintStats(timings[3]);
    std::cout << std::endl;

    std::cout << name << "\t" << timings[0][0] << "\t" << timings[1][0] << "\t" << timings[2][0] << "\t" << timings[3][0] << std::endl;

    std::cout << std::endl;
}

int main() {
    srand(902200987);
    std::vector<uint32_t> numbers;
    std::vector<std::string> strings;
    std::ostringstream ss;
    std::string tmp;
    for (size_t i=0 ; i<N ; ++i) {
        tmp.clear();
        ss.clear();
        ss.str("");
        int rnd = rand() & 0x00003FFF;
        ss << rnd;
        strings.push_back(ss.str());
        numbers.push_back(rnd);
    }

    std::cout << "<uint32_t>\tInsert\tLookup\tIterate\tErase" << std::endl;
    runTest< std::set<uint32_t> >("std::set", numbers);
    runTest< std::unordered_set<uint32_t> >("std::unordered_set", numbers);
    runTest< boost::unordered_set<uint32_t> >("boost::unordered_set", numbers);
    //runTest< boost::container::flat_set<uint32_t> >("boost::container::flat_set", numbers); // Broken Boost 1.55.0 vs. VS12?
    runTest< CG3::interval_vector<uint32_t> >("CG3::interval_vector", numbers);
    runTest< CG3::sorted_vector<uint32_t> >("CG3::sorted_vector", numbers);
    //runTest< CG3::sorted_deque<uint32_t> >("CG3::sorted_deque", numbers);
    runTest< btree::btree_set<uint32_t> >("btree::btree_set", numbers);
    //runTest< btree::safe_btree_set<uint32_t> >("btree::safe_btree_set", numbers);
#ifdef _MSC_VER
    runTest< sti::sset<uint32_t> >("sti::sset", numbers);
#endif

    std::cout << "<std::string>\tInsert\tLookup\tIterate\tErase" << std::endl;
    runTest< std::set<std::string> >("std::set", strings);
    runTest< std::unordered_set<std::string> >("std::unordered_set", strings);
    runTest< boost::unordered_set<std::string> >("boost::unordered_set", strings);
	//runTest< boost::container::flat_set<std::string> >("boost::container::flat_set", strings); // Broken Boost 1.55.0 vs. VS12?
    //runTest< CG3::interval_vector<std::string> >("CG3::interval_vector", strings); // only makes sense for integers
    runTest< CG3::sorted_vector<std::string> >("CG3::sorted_vector", strings);
    //runTest< CG3::sorted_deque<std::string> >("CG3::sorted_deque", strings);
    runTest< tdc::trie<std::string> >("tdc::trie", strings);
    runTest< btree::btree_set<std::string> >("btree::btree_set", strings);
    //runTest< btree::safe_btree_set<std::string> >("btree::safe_btree_set", strings);
#ifdef _MSC_VER
    runTest< sti::sset<std::string> >("sti::sset", strings);
#endif
}
