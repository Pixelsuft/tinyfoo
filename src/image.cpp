#include <image.hpp>
#include <lbs.hpp>
#include <new.hpp>
#include <log.hpp>
#include <ren.hpp>
#include <SDL3/SDL.h>
#if ENABLE_GDIPLUS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef UNICODE
#define UNICODE
#endif
#if !defined(_MSC_VER) && !defined(__uuidof)
#define __uuidof(x) (IID_ ## x)
#endif

#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>

class SDLWindowsStream : public IStream {
    SDL_IOStream* ctx;
    long refCount;

public:
    SDLWindowsStream(SDL_IOStream* sdl_ctx) : refCount(1) {
        this->ctx = sdl_ctx;
    }

    virtual ~SDLWindowsStream() {

    }

    virtual ULONG STDMETHODCALLTYPE AddRef(void) {
        return (ULONG)InterlockedIncrement(&refCount);
    }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject) {
        if (iid == __uuidof(IUnknown) || iid == __uuidof(ISequentialStream)
            || iid == __uuidof(IStream)) {
            *ppvObject = static_cast<IStream*>(this);
            AddRef();
            return S_OK;
        }
        else {
            return E_NOINTERFACE;
        }
    }

    virtual ULONG STDMETHODCALLTYPE Release(void) {
        ULONG ret = InterlockedDecrement(&refCount);
        if (ret == 0) {
            // delete this;
            return 0;
        }
        return ret;
    }

    virtual HRESULT STDMETHODCALLTYPE Read(void* pv, ULONG cb, ULONG* pcbRead) {
        size_t read = SDL_ReadIO(ctx, pv, (size_t)cb);
        if (pcbRead) {
            *pcbRead = (ULONG)read;
        }
        return read == cb ? S_OK : E_FAIL;
    }

    virtual HRESULT STDMETHODCALLTYPE Write(const void* pv, ULONG cb, ULONG* pcbWritten) {
        (void)pv;
        (void)cb;
        (void)pcbWritten;
        return E_NOTIMPL;
    }

    virtual HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition) {
        SDL_IOWhence whence;
        if (dwOrigin == STREAM_SEEK_SET)
            whence = SDL_IO_SEEK_SET;
        else if (dwOrigin == STREAM_SEEK_CUR)
            whence = SDL_IO_SEEK_CUR;
        else
            whence = SDL_IO_SEEK_END;
        Sint64 ret = SDL_SeekIO(ctx, (Sint64)dlibMove.QuadPart, whence);
        if (ret < 0)
            return E_FAIL;
        if (plibNewPosition) {
            plibNewPosition->QuadPart = (ULONGLONG)ret;
        }
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE Stat(STATSTG* pstatstg, DWORD grfStatFlag) {
        (void)grfStatFlag;
        SDL_memset(pstatstg, 0, sizeof(*pstatstg));
        pstatstg->type = STGTY_STREAM;
        pstatstg->cbSize.QuadPart = (ULONGLONG)SDL_GetIOSize(ctx);
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE Clone(IStream** ppstm) {
        (void)ppstm;
        return E_NOTIMPL;
    }

    virtual HRESULT STDMETHODCALLTYPE Commit(DWORD grfCommitFlags) {
        (void)grfCommitFlags;
        return E_NOTIMPL;
    }

    virtual HRESULT STDMETHODCALLTYPE CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten) {
        (void)pstm;
        (void)cb;
        (void)pcbRead;
        (void)pcbWritten;
        return E_NOTIMPL;
    }

    virtual HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) {
        (void)libOffset;
        (void)cb;
        (void)dwLockType;
        return E_NOTIMPL;
    }

    virtual HRESULT STDMETHODCALLTYPE Revert() {
        return E_NOTIMPL;
    }

    virtual HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER libNewSize) {
        (void)libNewSize;
        return E_NOTIMPL;
    }

    virtual HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) {
        (void)libOffset;
        (void)cb;
        (void)dwLockType;
        return E_NOTIMPL;
    }
};

static inline const char* gdi32_status_string(const Gdiplus::Status status) {
    switch (status) {
    case Gdiplus::Ok: return "Ok";
    case Gdiplus::GenericError: return "GenericError";
    case Gdiplus::InvalidParameter: return "InvalidParameter";
    case Gdiplus::OutOfMemory: return "OutOfMemory";
    case Gdiplus::ObjectBusy: return "ObjectBusy";
    case Gdiplus::InsufficientBuffer: return "InsufficientBuffer";
    case Gdiplus::NotImplemented: return "NotImplemented";
    case Gdiplus::Win32Error: return "Win32Error";
    case Gdiplus::Aborted: return "Aborted";
    case Gdiplus::FileNotFound: return "FileNotFound";
    case Gdiplus::ValueOverflow: return "ValueOverflow";
    case Gdiplus::AccessDenied: return "AccessDenied";
    case Gdiplus::UnknownImageFormat: return "UnknownImageFormat";
    case Gdiplus::FontFamilyNotFound: return "FontFamilyNotFound";
    case Gdiplus::FontStyleNotFound: return "FontStyleNotFound";
    case Gdiplus::NotTrueTypeFont: return "NotTrueTypeFont";
    case Gdiplus::UnsupportedGdiplusVersion: return "UnsupportedGdiplusVersion";
    case Gdiplus::GdiplusNotInitialized: return "GdiplusNotInitialized";
    case Gdiplus::PropertyNotFound: return "PropertyNotFound";
    case Gdiplus::PropertyNotSupported: return "PropertyNotSupported";
    default: return "UnknownStatus";
    }
}
#endif
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

#if ENABLE_GDIPLUS
namespace img {
    struct ImgData {
        Gdiplus::GdiplusStartupInput start_input;
        ULONG_PTR token;
    };

    ImgData* data;
}
#endif

bool img::init() {
#if ENABLE_GDIPLUS
    data = tf::bump_nw<ImgData>();
    data->token = 0;
    Gdiplus::Status ret = Gdiplus::GdiplusStartup(&data->token, &data->start_input, NULL);
    if (ret != Gdiplus::Ok) {
        TF_ERROR(<< "Failed to init GDI+ (" << gdi32_status_string(ret) << ")");
        tf::bump_dl(data);
        data = nullptr;
        return false;
    }
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

static inline void* create_fallback_surf() {
    // TODO: actually don't blur when creating a texture
    SDL_Surface* result = SDL_CreateSurface(2, 2, SDL_PIXELFORMAT_XRGB8888);
    if (!result) {
        TF_ERROR(<< "WTF failed to create fallback surface (" << SDL_GetError() << ")");
        return nullptr;
    }
	if (SDL_MUSTLOCK(result))
		SDL_LockSurface(result);
	((Uint32*)result->pixels)[0] = ((Uint32*)result->pixels)[3] = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? 0xF93EFB : 0xFB3EF9;
	((Uint32*)result->pixels)[1] = ((Uint32*)result->pixels)[2] = 0;
	if (SDL_MUSTLOCK(result))
		SDL_UnlockSurface(result);
	return result;
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
        return create_fallback_surf();
    }
    if (data_size < 10) {
        TF_ERROR(<< "Too small data size for an image");
        loader_close_io(ctx, free_src);
        return create_fallback_surf();
    }
    img::format fmt = get_image_format(ctx);
    if (fmt == img::format::UNKNOWN) {
        TF_ERROR(<< "Unknown image format");
        loader_close_io(ctx, free_src);
        return create_fallback_surf();
    }
    if (fmt == img::format::BMP) {
        SDL_Surface* ret = SDL_LoadBMP_IO(ctx, free_src);
        if (!ret)
            TF_ERROR(<< "Failed to load BMP (" << SDL_GetError() << ")");
        return ret;
    }
#if ENABLE_GDIPLUS
    SDLWindowsStream* s = data ? tf::nw<SDLWindowsStream>(ctx) : nullptr;
    if (!s) {
        TF_ERROR(<< "Failed to create SDLWindowsStream");
        loader_close_io(ctx, free_src);
        return create_fallback_surf();
    }
    SDL_Surface* ret = nullptr;
    Gdiplus::Bitmap* gdi_bmp = Gdiplus::Bitmap::FromStream(s, false);
    if (gdi_bmp) {
        const uint32_t w = gdi_bmp->GetWidth();
        const uint32_t h = gdi_bmp->GetHeight();
        ret = SDL_CreateSurface((int)w, (int)h, SDL_PIXELFORMAT_BGRA32);
        if (ret) {
            Gdiplus::BitmapData* gdi_lock = tf::nw<Gdiplus::BitmapData>();
            Gdiplus::Rect rect(0, 0, w, h);
            Gdiplus::Status lock_ret = gdi_bmp->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, gdi_lock);
            if (lock_ret == Gdiplus::Ok) {
                if (SDL_MUSTLOCK(ret))
                    SDL_LockSurface(ret);
                unsigned char* in = (unsigned char*)gdi_lock->Scan0;
                unsigned char* out = (unsigned char*)ret->pixels;
                if (gdi_lock->Stride == ret->pitch)
                    SDL_memcpy(out, in, h * (uint32_t)gdi_lock->Stride);
                else {
                    uint32_t rows = h;
                    while (rows--) {
                        SDL_memcpy(out, in, (((size_t)w) << 2));
                        in += gdi_lock->Stride;
                        out += ret->pitch;
                    }
                }
                if (SDL_MUSTLOCK(ret))
                    SDL_UnlockSurface(ret);
                gdi_bmp->UnlockBits(gdi_lock);
            }
            else
                TF_ERROR(<< "Gdiplus::Bitmap->LockBits failed (" << gdi32_status_string(lock_ret) << ")");
            tf::dl(gdi_lock);
        }
        else
            TF_ERROR(<< "Failed to create SDL surface (" << SDL_GetError() << ")");
        Gdiplus::GdiplusBase::operator delete(gdi_bmp);
    }
    s->Release();
    tf::dl(s);
    loader_close_io(ctx, free_src);
    return ret ? ret : create_fallback_surf();
#endif
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
        return ret ? ret : create_fallback_surf();
    }
#endif
    loader_close_io(ctx, free_src);
    TF_ERROR(<< "Failed to find loader for an image");
    return create_fallback_surf();
}

void img::destroy() {
#if ENABLE_GDIPLUS
    Gdiplus::GdiplusShutdown(data->token);
    tf::bump_dl(data);
#endif
}
