#pragma once
#include <lbs.hpp>

#if 0
// TODO: Modern C++17
#elif IS_MSVC
#define UNUSED_END
#else
#define UNUSED_END __attribute__((unused))
#endif
