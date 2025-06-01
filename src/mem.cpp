#include <mem.hpp>
#include <SDL3/SDL_stdinc.h>

namespace mem {
	Uint8* bump_ptr;
}

void* mem::alloc(int size) {
	return SDL_malloc((size_t)size);
}

void mem::free(void* ptr) {
	SDL_free(ptr);
}

void* mem::bump_alloc(int size) {
	void* result = (void*)bump_ptr;
	bump_ptr += (size_t)size;
	return result;
}
