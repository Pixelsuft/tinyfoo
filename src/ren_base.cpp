#include <ren.hpp>
#if IS_WIN
#include <log.hpp>
#include <conf.hpp>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif

using ren::RendererBase;

RendererBase::RendererBase() {
    inited = false;
#if IS_WIN
    DwmFlush_ptr = nullptr;
#endif
}

RendererBase::~RendererBase() {
#if IS_WIN
    DwmFlush_ptr = nullptr;
#endif
    inited = false;
}

bool RendererBase::init_fake_vsync() {
#if IS_WIN
    auto vfb = conf::read_bool("renderer", "vsync_fix", true);
    if (!vfb)
        return false;
    auto handle = GetModuleHandleW(L"dwmapi.dll");
    if (!handle)
        return false;
    *(void**)&DwmFlush_ptr = (void*)GetProcAddress(handle, "DwmFlush");
    if (!DwmFlush_ptr) {
        TF_WARN(<< "Failed to find DwmFlush in dwmapi, expect vsync problems");
        return false;
    }
    return true;
#else
    return false;
#endif
}

void RendererBase::do_fake_vsync() {
#if IS_WIN
    if (DwmFlush_ptr) {
        if ((HRESULT)DwmFlush_ptr() != S_OK) // WTF why does it fail
            Sleep(100);
    }
#endif
}
