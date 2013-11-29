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

#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <limits>
#include <vector>
#include <iomanip>

const size_t N = 1000000;
const size_t R = 7;
std::vector<uint32_t> numbers;

enum TYPES {
    T_ONE_BASE,
    T_ONE_LEVEL1,
    T_ONE_LEVEL2,
    T_ONE_LEVEL3,
    T_TWO_BASE,
    T_TWO_LEVEL1,
};

struct OneBase {
    TYPES type;

    OneBase() : type(T_ONE_BASE) {}
    virtual ~OneBase() {}

    virtual TYPES getType() {
        return T_ONE_BASE;
    }
};

struct OneLevel1 : OneBase {
    OneLevel1() {
        type = T_ONE_LEVEL1;
    }
    virtual TYPES getType() {
        return T_ONE_LEVEL1;
    }
};

struct OneLevel2 : OneLevel1 {
    OneLevel2() {
        type = T_ONE_LEVEL2;
    }
    virtual TYPES getType() {
        return T_ONE_LEVEL2;
    }
};

struct OneLevel3 : OneLevel2 {
    OneLevel3() {
        type = T_ONE_LEVEL3;
    }
    TYPES getType() {
        return T_ONE_LEVEL3;
    }
};

struct TwoBase {
    TYPES type;

    TwoBase() : type(T_TWO_BASE) {}
    virtual ~TwoBase() {}

    virtual TYPES getType() {
        return T_TWO_BASE;
    }
};

struct TwoLevel1 : TwoBase {
    TwoLevel1() {
        type = T_TWO_LEVEL1;
    }
    virtual TYPES getType() {
        return T_TWO_LEVEL1;
    }
};

int main() {
    std::vector< std::vector<double> > timings(17);

    for (size_t r=0 ; r<R ; ++r) {
        OneBase a;
        OneLevel1 b;
        OneLevel2 c;
        OneLevel3 d;
        OneBase* ones[] = {&a, &b, &c, &d};

        ticks start, end;
        double timed = 0.0;
        size_t res = 0;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel3 *a = reinterpret_cast<OneLevel3*>(ones[3]);
            res += a->getType();
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[0].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            if (ones[2]->getType() == T_ONE_LEVEL2) {
                OneLevel2 *a = reinterpret_cast<OneLevel2*>(ones[2]);
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[1].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneBase *a = dynamic_cast<OneBase*>(ones[0]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[2].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel1 *a = dynamic_cast<OneLevel1*>(ones[1]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[3].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel2 *a = dynamic_cast<OneLevel2*>(ones[2]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[4].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel3 *a = dynamic_cast<OneLevel3*>(ones[3]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[5].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneBase *a = dynamic_cast<OneBase*>(ones[1]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[6].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneBase *a = dynamic_cast<OneBase*>(ones[2]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[7].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneBase *a = dynamic_cast<OneBase*>(ones[3]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[8].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel1 *a = dynamic_cast<OneLevel1*>(ones[2]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[9].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel1 *a = dynamic_cast<OneLevel1*>(ones[3]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[10].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel2 *a = dynamic_cast<OneLevel2*>(ones[3]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[11].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            TwoBase *a = dynamic_cast<TwoBase*>(ones[0]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[12].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            TwoBase *a = dynamic_cast<TwoBase*>(ones[1]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[13].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            TwoBase *a = dynamic_cast<TwoBase*>(ones[2]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[14].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            TwoBase *a = dynamic_cast<TwoBase*>(ones[3]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[15].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            if (ones[3]->type == T_ONE_LEVEL3) {
                OneLevel3 *a = reinterpret_cast<OneLevel3*>(ones[3]);
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[16].push_back(timed);
        std::cerr << res << std::endl;

        std::cerr << std::endl;
    }

    std::cout << "reinterpret_cast known-type: ";
    PrintStats(timings[0]);
    std::cout << std::endl;

    std::cout << "virtual function + reinterpret_cast: ";
    PrintStats(timings[1]);
    std::cout << std::endl;

    std::cout << "member variable + reinterpret_cast: ";
    PrintStats(timings[16]);
    std::cout << std::endl;

    std::cout << "dynamic_cast same-type-base success: ";
    PrintStats(timings[2]);
    std::cout << std::endl;

    std::cout << "dynamic_cast same-type-level1 success: ";
    PrintStats(timings[3]);
    std::cout << std::endl;

    std::cout << "dynamic_cast same-type-level2 success: ";
    PrintStats(timings[4]);
    std::cout << std::endl;

    std::cout << "dynamic_cast same-type-level3 success: ";
    PrintStats(timings[5]);
    std::cout << std::endl;

    std::cout << "dynamic_cast level1-to-base success: ";
    PrintStats(timings[6]);
    std::cout << std::endl;

    std::cout << "dynamic_cast level2-to-base success: ";
    PrintStats(timings[7]);
    std::cout << std::endl;

    std::cout << "dynamic_cast level3-to-base success: ";
    PrintStats(timings[8]);
    std::cout << std::endl;

    std::cout << "dynamic_cast level2-to-level1 success: ";
    PrintStats(timings[9]);
    std::cout << std::endl;

    std::cout << "dynamic_cast level3-to-level1 success: ";
    PrintStats(timings[10]);
    std::cout << std::endl;

    std::cout << "dynamic_cast level3-to-level2 success: ";
    PrintStats(timings[11]);
    std::cout << std::endl;

    std::cout << "dynamic_cast onebase-to-twobase fail: ";
    PrintStats(timings[12]);
    std::cout << std::endl;

    std::cout << "dynamic_cast onelevel1-to-twobase fail: ";
    PrintStats(timings[13]);
    std::cout << std::endl;

    std::cout << "dynamic_cast onelevel2-to-twobase fail: ";
    PrintStats(timings[14]);
    std::cout << std::endl;

    std::cout << "dynamic_cast onelevel3-to-twobase fail: ";
    PrintStats(timings[15]);
    std::cout << std::endl;

    std::cout << std::endl;
}
