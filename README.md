# tinyfoo
Simple music player written in C++ with SDL3 and ImGui <br />
![Logo](assets/icon.png)
## Suppored audio backends
Backends are loaded dynamically in runtime. Only dynamic (shared) lib files are required to make them working. <br />
 - [SDL2_mixer](https://wiki.libsdl.org/SDL2_mixer/FrontPage)/[SDL-Mixer-X](https://wohlsoft.github.io/SDL-Mixer-X/) via [sdl2-compat](https://github.com/libsdl-org/sdl2-compat) (because there is no stable [SDL3_mixer](https://github.com/libsdl-org/SDL_mixer) release yet)
 - [FMOD](https://www.fmod.com/)
 - [BASS](https://www.un4seen.com/)
## External dependencies
All of them (except SDL3 for non-msvc and non-mingw builds) are fetched automatically using LBS script
 - [SDL3](https://github.com/libsdl-org/SDL)
 - [nlohmann json](https://github.com/nlohmann/json)
 - [toml11](https://github.com/ToruNiina/toml11)
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
Note: when using in debug mode, assets folder must be in cwd
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
python lbs.py release init --mingw
python lbs.py release fetch
python lbs.py release conf --release
python lbs.py release gen_res
python lbs.py release join_code
cd release
# You can add -std=gnu++20 (-std=gnu++2a) for C++20
g++ tinyfoo.cpp -o tinyfoo.exe -Ofast -fno-rtti -Iimgui -Ilbs -ISDL/x86_64-w64-mingw32/include -LSDL/x86_64-w64-mingw32/lib -lSDL3
```
## TODO
 - Add support toml++ (it's C++17 but can work without exceptions)
 - Other audio libs
 - Fix TODOs in code and code cleanup