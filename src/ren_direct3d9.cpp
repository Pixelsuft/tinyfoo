#include <lbs.hpp>
#if ENABLE_DIRECT3D9
#include <ren.hpp>
#include <new.hpp>
#include <log.hpp>
#include <lbs.hpp>
#include <stl.hpp>
#include <conf.hpp>
#include <image.hpp>
#include <SDL3/SDL.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <d3d9.h>
#if ENABLE_IMGUI
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_dx9.h>
#endif

using ren::RendererBase;

namespace ren {
    class RendererDirect3D9 : public RendererBase {
        SDL_FPoint scale;
        IDirect3D9* (*Direct3DCreate9_ptr)(UINT);
        SDL_Window* win;
        HWND hwnd;
        HMODULE dll;
        LPDIRECT3D9 pD3D;
        LPDIRECT3DDEVICE9 pd3dDevice;
        D3DPRESENT_PARAMETERS d3dpp;
        bool dev_lost;
        public:
        RendererDirect3D9(void* _win) {
            win = (SDL_Window*)_win;
            hwnd = nullptr;
            dev_lost = false;
            inited = false;
            scale.x = scale.y = 1.f;
            SDL_PropertiesID props = SDL_GetWindowProperties(win);
            if (props) {
                hwnd = (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
                SDL_DestroyProperties(props);
            }
            if (!hwnd) {
                TF_ERROR(<< "Failed to get window HWND");
                return;
            }
            pD3D = nullptr;
            pd3dDevice = nullptr;
            dll = LoadLibraryExW(L"D3d9.dll", nullptr, 0);
            if (!dll) {
                TF_ERROR(<< "Failed to load Direct3D9 dll");
                return;
            }
            *(void**)&Direct3DCreate9_ptr = (void*)GetProcAddress(dll, "Direct3DCreate9");
            if (!Direct3DCreate9_ptr) {
                TF_ERROR(<< "Failed to find Direct3DCreate9 function in Direct3D9 dll");
                FreeLibrary(dll);
                return;
            }
            pD3D = Direct3DCreate9_ptr(D3D_SDK_VERSION);
            if (!pD3D) {
                TF_ERROR(<< "Failed to create Direct3D9 device");
                FreeLibrary(dll);
                return;
            }
            auto vsync_b = conf::read_bool("renderer", "vsync", true);
            ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
            d3dpp.Windowed = TRUE;
            d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
            d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
            d3dpp.EnableAutoDepthStencil = TRUE;
            d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
            d3dpp.PresentationInterval = vsync_b ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
            if (pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice) < 0) {
                TF_ERROR(<< "Failed to create Direct3D9 device");
                pD3D->Release();
                FreeLibrary(dll);
                return;
            }
#if ENABLE_IMGUI
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            ImGui_ImplSDL3_InitForD3D(win);
            ImGui_ImplDX9_Init(pd3dDevice);
#endif
            TF_INFO(<< "Direct3D9 native renderer created");
            inited = true;
        }

        ~RendererDirect3D9() {
            if (!inited)
                return;
#if ENABLE_IMGUI
            ImGui_ImplDX9_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();
#endif
            pd3dDevice->Release();
            pD3D->Release();
            FreeLibrary(dll);
        }

        void begin_frame() {
            if (dev_lost) {
                HRESULT hr = pd3dDevice->TestCooperativeLevel();
                if (hr == D3DERR_DEVICELOST) {
                    // WTF
                    Sleep(20);
                    return;
                }
                if (hr == D3DERR_DEVICENOTRESET)
                    reset_device();
                dev_lost = false;
            }
            ImGuiIO& io = ImGui::GetIO();
            ImVec4 bg_col = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
            pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
            pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
            pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
            D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(bg_col.x * bg_col.w * 255.0f), (int)(bg_col.y * bg_col.w * 255.0f), (int)(bg_col.z * bg_col.w * 255.0f), (int)(bg_col.w * 255.0f));
            pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
#if ENABLE_IMGUI
            ImGui_ImplDX9_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
#endif
        }

        void end_frame() {
            if (dev_lost)
                return;
#if ENABLE_IMGUI
            ImGui::EndFrame();
#endif
            if (pd3dDevice->BeginScene() >= 0) {
#if ENABLE_IMGUI
                ImGui::Render();
                ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
#endif
                pd3dDevice->EndScene();
            }
            HRESULT result = pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
            if (result == D3DERR_DEVICELOST)
                dev_lost = true;
        }

        Point get_size() {
            int w_buf, h_buf;
            if (!SDL_GetWindowSizeInPixels(win, &w_buf, &h_buf)) {
                TF_ERROR(<< "Failed to get window size in pixels (" << SDL_GetError() << ")");
                w_buf = 640;
                h_buf = 480;
            }
            Point res;
            res.x = (float)w_buf;
            res.y = (float)h_buf;
            if (SDL_GetWindowSize(win, &w_buf, &h_buf)) {
                scale.x = res.x / (float)w_buf;
                scale.y = res.y / (float)h_buf;
            }
            else
                scale.x = scale.y = 1.f;
            // Assuming this function is called rarely
            d3dpp.BackBufferWidth = (UINT)w_buf;
            d3dpp.BackBufferHeight = (UINT)h_buf;
            reset_device();
            return res;
        }

        Point point_win_to_ren(const Point& pos) {
            Point ret = { pos.x * scale.x, pos.y * scale.y };
            return ret;
        }

        void* tex_from_io(void* ctx, bool free_src) {
            SDL_Surface* surf = (SDL_Surface*)img::surf_from_io(ctx, free_src);
            if (!surf)
                return create_fallback_texture();
            // Assuming original surface format is cool
            IDirect3DTexture9* pTex = nullptr;
            HRESULT hr = pd3dDevice->CreateTexture(
                surf->w,
                surf->h,
                1,
                0,
                D3DFMT_A8R8G8B8,
                D3DPOOL_MANAGED,
                &pTex,
                nullptr
            );
            if (!SUCCEEDED(hr)) {
                TF_ERROR(<< "Failed to create Direct3D9 texture");
                return nullptr;
            }
            D3DLOCKED_RECT lockedRect;
            hr = pTex->LockRect(0, &lockedRect, NULL, 0);
            if(SUCCEEDED(hr)) {
                char* src = (char*)surf->pixels;
                char* dst = (char*)lockedRect.pBits;
                size_t numRows = surf->h;
                size_t rowSize = surf->pitch;
                while(numRows--) {
                    SDL_memcpy(dst, src, rowSize);
                    src += surf->pitch;
                    dst += lockedRect.Pitch;
                }
                hr = pTex->UnlockRect(0);
            }
            else
                TF_WARN(<< "Failed to lock Direct3D9 texture");
            return (void*)(intptr_t)pTex;
        }

        void tex_destroy(void* tex) {
            if (!tex)
                return;
            IDirect3DTexture9* pTex = (IDirect3DTexture9*)(intptr_t)tex;
            pTex->Release();
        }

        void* create_fallback_texture() {
            TF_WARN(<< "Returning fallback texture");
            return nullptr;
        }

        void reset_device() {
#if ENABLE_IMGUI
            ImGui_ImplDX9_InvalidateDeviceObjects();
#endif
            HRESULT hr = pd3dDevice->Reset(&d3dpp);
            if (hr == D3DERR_INVALIDCALL) {
                TF_ERROR(<< "Failed to reset Direct3D9 device");
                dev_lost = true; // ???
            }
#if ENABLE_IMGUI
            ImGui_ImplDX9_CreateDeviceObjects();
#endif
        }
    };
}

RendererBase* ren::create_renderer_direct3d9(void* win) {
    return tf::bump_nw<RendererDirect3D9>(win);
}
#endif
