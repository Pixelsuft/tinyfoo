#pragma once

namespace img {
    enum class format {
        UNKNOWN = -1,
        BMP = 0,
        PNG = 1,
        JPEG = 2,
        GIF = 3
	};

    bool init();
    void destroy();
    void* surf_from_io(void* ctx, bool free_src);
}
