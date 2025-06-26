#pragma once
#include <algorithm>

namespace tf {
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || (__cplusplus >= 201703L)
    using clamp = std::clamp;
#else
    template<class T>
    constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
        return std::min(std::max(v, lo), hi);
    }
#endif
}
