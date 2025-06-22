#include <image.hpp>
#include <lbs.hpp>
#include <new.hpp>
#include <log.hpp>
#include <ren.hpp>
#include <SDL3/SDL.h>
/*
TODO:
- GdiPlus on Windows
- Alloc ImgData only if needed
*/
#if ENABLE_UPNG
#include <upng.hpp>

static inline const char* upng_error_string(upng_error status) {
    switch (status) {
    case UPNG_EOK: return "UPNG_EOK";
    case UPNG_ENOMEM: return "UPNG_ENOMEM";
    case UPNG_ENOTFOUND: return "UPNG_ENOTFOUND";
    case UPNG_ENOTPNG: return "UPNG_ENOTPNG";
    case UPNG_EMALFORMED: return "UPNG_EMALFORMED";
    case UPNG_EUNSUPPORTED: return "UPNG_EUNSUPPORTED";
    case UPNG_EUNINTERLACED: return "UPNG_EUNINTERLACED";
    case UPNG_EUNFORMAT: return "UPNG_EUNFORMAT";
    case UPNG_EPARAM: return "UPNG_EPARAM";
    default: return "UPNG_UNKNOWN";
    }
}
#endif

namespace img {
    struct ImgData {
        int dummy;
    };

    ImgData* data;
}

bool img::init() {
#if 0
    data = tf::bump_nw<ImgData>();
#endif
    return true;
}

img::format get_image_format(SDL_IOStream* ctx) {
	if (SDL_GetIOSize(ctx) < 8)
		return img::format::UNKNOWN;
	char buf[8];
	size_t num_read = SDL_ReadIO(ctx, buf, 8);
    SDL_SeekIO(ctx, 0, SDL_IO_SEEK_SET);
	if (num_read < 8)
		return img::format::UNKNOWN;
	if (SDL_memcmp(buf, "BM", 2) == 0)
		return img::format::BMP;
	if (SDL_memcmp(buf, "\x89PNG\r\n\x1a\n", 8) == 0)
		return img::format::PNG;
	if (SDL_memcmp(buf, "\xFF\xD8", 2) == 0)
		return img::format::JPEG;
	if (SDL_memcmp(buf, "GIF8", 4) == 0)
		return img::format::GIF;
	return img::format::UNKNOWN;
}

static inline SDL_Surface* create_fallback_surface() {
    TF_WARN(<< "Returning fallback surface");
    return nullptr;
}

static inline void loader_close_io(SDL_IOStream* ctx, bool free_src) {
    if (free_src && !SDL_CloseIO(ctx))
        TF_ERROR(<< "Failed to close image file (" << SDL_GetError() << ")");
}

void* img::surf_from_io(void* _ctx, bool free_src) {
    SDL_IOStream* ctx = (SDL_IOStream*)_ctx;
    Sint64 data_size = SDL_GetIOSize(ctx);
    if (data_size < 0) {
        TF_ERROR(<< "Failed to get image data size (" << SDL_GetError() << ")");
        loader_close_io(ctx, free_src);
        return create_fallback_surface();
    }
    if (data_size < 10) {
        TF_ERROR(<< "Too small data size for an image");
        loader_close_io(ctx, free_src);
        return create_fallback_surface();
    }
    img::format fmt = get_image_format(ctx);
    if (fmt == img::format::UNKNOWN) {
        TF_ERROR(<< "Unknown image format");
        loader_close_io(ctx, free_src);
        return create_fallback_surface();
    }
    if (fmt == img::format::BMP) {
        SDL_Surface* ret = SDL_LoadBMP_IO(ctx, free_src);
        if (!ret)
            TF_ERROR(<< "Failed to load BMP (" << SDL_GetError() << ")");
        return ret;
    }
#if ENABLE_UPNG
    if (fmt == img::format::PNG) {
        SDL_Surface* ret = nullptr;
        unsigned char* buf = (unsigned char*)mem::alloc((int)data_size);
        if (buf) {
            if (SDL_ReadIO(ctx, buf, (size_t)data_size) >= (size_t)data_size) {
                upng_t* handle = upng_new_from_bytes(buf, (unsigned long)data_size);
                if (handle) {
                    upng_error err = upng_decode(handle);
                    if (err == UPNG_EOK) {
                        ret = SDL_CreateSurface(
                            (int)upng_get_width(handle), (int)upng_get_height(handle),
                            SDL_PIXELFORMAT_RGBA32
                        );
                        if (ret) {
                            if (SDL_MUSTLOCK(ret))
                                SDL_LockSurface(ret);
                            if (upng_get_components(handle) == 4)
                                SDL_memcpy(ret->pixels, upng_get_buffer(handle), upng_get_size(handle));
                            else if (upng_get_components(handle) == 3) {
                                unsigned long sz = upng_get_size(handle);
                                const unsigned char* src = upng_get_buffer(handle);
                                unsigned char* dst = (unsigned char*)ret->pixels;
                                for (unsigned long i = 0; i < (sz / 3); i++) {
                                    dst[i * 4] = src[i * 3];
                                    dst[i * 4 + 1] = src[i * 3 + 1];
                                    dst[i * 4 + 2] = src[i * 3 + 2];
                                    dst[i * 4 + 3] = 255;
                                }
                            }
                            else
                                TF_ERROR(<< "Unsupported UPNG decode format");
                            if (SDL_MUSTLOCK(ret))
                                SDL_UnlockSurface(ret);
                        }
                        else
                            TF_ERROR(<< "Failed to create SDL surface " << SDL_GetError() << ")");
                    }
                    else
                        TF_ERROR(<< "Failed to decode upng image " << SDL_GetError() << ")");
                    upng_free(handle);
                }
                else
                    TF_ERROR(<< "Failed to create upng handle");
            }
            else
                TF_ERROR(<< "Failed to read into image buffer " << SDL_GetError() << ")");
            mem::free(buf);
        }
        else
            TF_ERROR(<< "Failed to allocate image buffer");
        loader_close_io(ctx, free_src);
        return ret ? ret : create_fallback_surface();
    }
#endif
    loader_close_io(ctx, free_src);
    TF_ERROR(<< "Failed to find loader for an image");
    return create_fallback_surface();
}

void img::destroy() {
#if 0
    tf::bump_dl(data);
#endif
}
