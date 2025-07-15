#pragma once
#include <lbs.hpp>

#if (defined(_MSVC_LANG) ? _MSVC_LANG : __cplusplus) >= 201703L
#define UNUSED_END
#if IS_MSVC
#define UNUSED_BEGIN [[maybe_unused]]
#else
#define UNUSED_BEGIN [[gnu::unused]]
#endif
#elif IS_MSVC
#define UNUSED_BEGIN
#define UNUSED_END
#else
#define UNUSED_BEGIN
#define UNUSED_END __attribute__((unused))
#endif
