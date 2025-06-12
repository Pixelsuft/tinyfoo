#pragma once
#include <alloc.hpp>
#include <vector>

namespace tf {
    template<typename T>
    using vec = std::vector<T, mem::FAlloc<T>>;
}
