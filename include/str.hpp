#pragma once
#include <alloc.hpp>
#include <string>

namespace tf {
    using str = std::basic_string<char, std::char_traits<char>, mem::FAlloc<char>>;
}
