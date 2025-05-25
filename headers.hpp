#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <deque>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
// #include <stack>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Sseu.hpp"

#define TCO (::std::cout << __FILE__ << ':' << __LINE__ << '\t')

namespace Sseu::BlockSource {
    template<::uint32_t N> struct BlockTexDataWrapper{
        ::std::vector<::std::vector<::uint8_t>> faces;
        ::uint8_t *data[N];
        BlockTexDataWrapper(::std::vector<::std::vector<::uint8_t>> faces_data): faces(faces_data) {
            for (::uint32_t face{0U}; face < N; ++face) {
                this->data[face] = this->faces[face].data();
            }
        }
    };
}

#endif
