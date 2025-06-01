#pragma once
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || (__cplusplus >= 201703L)
#include <string_view>
#else
#include <bpstd/string_view.hpp>
#endif

namespace tf {
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || (__cplusplus >= 201703L)
    using str_view = std::string_view;
#else
    using str_view = bpstd::string_view;
#endif
}
