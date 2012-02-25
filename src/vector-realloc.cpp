#ifdef _MSC_VER
    #define _SECURE_SCL 0
    #define _CRT_SECURE_NO_DEPRECATE 1
    #define WIN32_LEAN_AND_MEAN
    #define VC_EXTRALEAN
    #define NOMINMAX
#endif

#include <cstdlib>
#include <iostream>
#include <vector>

int main() {
    int *c = (int*)malloc(sizeof(*c));
    int *old_c = c;
    size_t cnt_c = 0;

    std::vector<int> v(1);
    int *old_v = &v[0];
    size_t cnt_v = 0;

    for (size_t i=0 ; i<10000000 ; ++i) {
        c[i] = i;
        c = (int*)realloc(c, sizeof(*c)*(i+2));
        if (old_c != c) {
            ++cnt_c;
        }
        old_c = c;

        v[i] = i;
        v.resize(v.size()+1);
        if (old_v != &v[0]) {
            ++cnt_v;
        }
        old_v = &v[0];
    }
    
    std::cout << "realloc() reallocations: " << cnt_c << std::endl;
    std::cout << "std::vector reallocations: " << cnt_v << std::endl;
}
