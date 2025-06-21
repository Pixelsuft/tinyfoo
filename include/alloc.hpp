#pragma once
#include <mem.hpp>
#include <cstddef>

namespace mem {
    template <class T>
    struct FAlloc {
        typedef T value_type;
        FAlloc() noexcept {}
        template<class U> FAlloc(const FAlloc<U>&) noexcept {}
        template<class U> bool operator==(const FAlloc<U>&) const noexcept {
            return true;
        }
        template<class U> bool operator!=(const FAlloc<U>&) const noexcept {
            return false;
        }
        T* allocate(const size_t n) const;
        void deallocate(T* const p, size_t) const noexcept;
    };
    
    template <class T>
    T* FAlloc<T>::allocate(const size_t n) const {
        if (n == 0) {
            return nullptr;
        }
        if (n > static_cast<size_t>(-1) / sizeof(T)) {
            // throw std::bad_array_new_length();
        }
        void* const pv = mem::alloc((int)(n * sizeof(T)));
        if (!pv) {
            // throw std::bad_alloc();
        }
        return static_cast<T*>(pv);
    }
    
    template<class T>
    void FAlloc<T>::deallocate(T* const p, size_t) const noexcept {
        mem::free(p);
    }
}
