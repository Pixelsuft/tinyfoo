#pragma once

namespace tf {
	template<class T>
	using vec = std::vector<T, mem::FAlloc<T>>;
}
