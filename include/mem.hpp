#pragma once

namespace mem {
    void* alloc(int size);
    void free(void* ptr);
    void* bump_alloc(int size);
}
