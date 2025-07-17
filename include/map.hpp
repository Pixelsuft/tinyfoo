#pragma once
#include <alloc.hpp>
#include <map>

namespace tf {
    template<typename K, typename T>
    using map = std::map<K, T, std::less<K>, mem::FAlloc<std::pair<const K, T>>>;
}
