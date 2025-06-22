#pragma once
#include <alloc.hpp>
#include <set>

namespace tf {
    template<typename T>
    using set = std::set<T, std::less<T>, mem::FAlloc<T>>;
}
