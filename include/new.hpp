#pragma once
#include <config.hpp>
#include <mem.hpp>
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || (__cplusplus >= 201703L)
#include <new>
#endif
#include <memory>
#include <utility>

namespace tf {
	template <typename T, typename... Args>
	static inline T* nw(Args&&... args) {
		void* ptr_res = mem::alloc(sizeof(T));
		if (ptr_res == nullptr) {
            // TODO: unreachable (release mode)
			return nullptr;
		}
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) || (__cplusplus >= 202002L)
		return std::construct_at((T*)ptr_res, std::forward<Args>(args)...);
#else
		return new(ptr_res) T(std::forward<Args>(args)...);
#endif
	}

	template <typename T>
	static inline void dl(T* ptr) {
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || (__cplusplus >= 201703L)
		std::destroy_at(ptr);
#else
		ptr->~T();
#endif
		mem::free(ptr);
	}

	template <typename T, typename... Args>
	static inline T* bump_nw(Args&&... args) {
		void* ptr_res = mem::bump_alloc(sizeof(T));
		if (ptr_res == nullptr) {
            // TODO: unreachable
			return nullptr;
		}
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) || (__cplusplus >= 202002L)
		return std::construct_at((T*)ptr_res, std::forward<Args>(args)...);
#else
		return new(ptr_res) T(std::forward<Args>(args)...);
#endif
	}

	template <typename T>
	static inline void bump_dl(T* ptr) {
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || (__cplusplus >= 201703L)
		std::destroy_at(ptr);
#else
		ptr->~T();
#endif
	}
}
