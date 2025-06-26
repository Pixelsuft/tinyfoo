#pragma once
#include <algorithm>

namespace tf {
    template<class T>
    constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
#if (defined(_MSVC_LANG) ? _MSVC_LANG : __cplusplus) >= 201703L
        return std::clamp(v, lo, hi);
#else
        return std::min(std::max(v, lo), hi);
#endif
    }
}
