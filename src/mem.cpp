#include <mem.hpp>
#include <lbs.hpp>
#include <log.hpp>
#include <SDL3/SDL_stdinc.h>

namespace mem {
	Uint8* bump_ptr;

#if !IS_RELEASE
	static int bump_alloc_size = 0;
#endif
}

void* mem::alloc(int size) {
	return SDL_malloc((size_t)size);
}

void mem::free(void* ptr) {
	SDL_free(ptr);
}

void* mem::bump_alloc(int size) {
#if !IS_RELEASE
	mem::bump_alloc_size += size;
#if 1
	TF_INFO(<< "Bump alloc size: " << mem::bump_alloc_size);
#endif
#endif
	void* result = (void*)bump_ptr;
	bump_ptr += (size_t)size;
	return result;
}
