# tinyfoo
Simple music player written in C++11 with SDL3 and ImGui <br />
![Logo](assets/icon.png)
## Suppored audio backends
Backends are loaded dynamically in runtime. Only dynamic (shared) lib files are required to make them working. <br />
 - [SDL2_mixer](https://wiki.libsdl.org/SDL2_mixer/FrontPage)/[SDL-Mixer-X](https://wohlsoft.github.io/SDL-Mixer-X/) via [sdl2-compat](https://github.com/libsdl-org/sdl2-compat) (because there is no stable [SDL3_mixer](https://github.com/libsdl-org/SDL_mixer) release yet)
 - [FMOD](https://www.fmod.com/)
 - [BASS](https://www.un4seen.com/)
 - [SoLoud](https://solhsa.com/soloud/) (setting music position is broken somewhy)
## External dependencies
All of them (except SDL3 for non-msvc and non-mingw builds) are fetched automatically using LBS script
 - [SDL3](https://github.com/libsdl-org/SDL)
 - [nlohmann json](https://github.com/nlohmann/json)
 - [toml11](https://github.com/ToruNiina/toml11) or [toml++](https://github.com/marzer/tomlplusplus)
 - [ImGui](https://github.com/ocornut/imgui)
## Building (Visual Studio)
```sh
python lbs.py msvc init --msvc
python lbs.py msvc fetch
python lbs.py msvc conf
python lbs.py msvc gen_res
```
Then open VS project and build in Debug/Release mode
## Building (CMake)
Note: when using in debug mode, assets folder must be in executable cwd
```sh
python lbs.py build init
python lbs.py build fetch
# Optionally add --release
python lbs.py build conf
python lbs.py build gen_res
cd build
cmake ..
make
```
## Example building using MinGW64 in release mode
```sh
# --mingw is not required for MSYS2
python lbs.py release init --mingw
python lbs.py release fetch
python lbs.py release conf --release
python lbs.py release gen_res
python lbs.py release join_code
cd release
# MinGW64
g++ tinyfoo.cpp -o tinyfoo.exe -std=gnu++2a -Ofast -fno-rtti -Iimgui -Ilbs -ISDL/x86_64-w64-mingw32/include -LSDL/x86_64-w64-mingw32/lib -lSDL3 -lgdiplus
# MSYS2
g++ tinyfoo.cpp -o tinyfoo.exe -std=gnu++23 -Ofast -fno-rtti -fno-exceptions -Iimgui -Ilbs -lSDL3 -lgdiplus
```
## TODO
 - Improve UI
 - Support configuring tab columns
 - Other playmodes (like smart_rng to choose track author and then title)
 - Add support for libpng (useful on linux?) (waiting for v2)
 - Other audio libs (miniaudio, ...)
 - Fix TODOs in code and code cleanup
