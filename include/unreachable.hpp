#pragma once

#if (defined(_MSVC_LANG) ? _MSVC_LANG : __cplusplus) >= 202302L
#include <utility>

#define TF_UNREACHABLE() std::unreachable()
#else
#if defined(_MSC_VER)
#define TF_UNREACHABLE() __assume(0)
#elif defined(__GNUC__)
#define TF_UNREACHABLE() __builtin_unreachable()
#else
#define TF_UNREACHABLE() do { int* _its_fucking_null_ptr = nullptr; *_its_fucking_null_ptr = 69; } while (0)
#endif
#endif
