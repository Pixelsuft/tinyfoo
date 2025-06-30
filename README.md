# tinyfoo
Simple music player written in C++ with SDL3 and ImGui <br />
![logo](assets/icon.png)
## Suppored audio backends
Backends are loaded dynamically in runtime. Only dynamic (shared) lib files are required to make them working. <br />
 - [SDL2_mixer](https://wiki.libsdl.org/SDL3_mixer/FrontPage)/[SDL-Mixer-X](https://github.com/WohlSoft/SDL-Mixer-X) via [SDL2-compat](https://github.com/libsdl-org/sdl2-compat) (because there is no stable SDL3_mixer release yet)
 - [FMOD](https://www.fmod.com/)
 - [BASS](https://www.un4seen.com/)
## External dependencies
 - [SDL3](https://github.com/libsdl-org/SDL)
 - [nlohmann json](https://github.com/nlohmann/json)
 - [toml11](https://github.com/ToruNiina/toml11)
 - [ImGui](https://github.com/ocornut/imgui)
All of them (except SDL3 for non-msvc and non-mingw builds) are fetched automatically using LBS script
## Building (Visual Studio)
```sh
python lbs.py msvc init --msvc
python lbs.py msvc fetch
python lbs.py msvc conf
python lbs.py msvc gen_res
```
Then open VS project and build in Debug/Release mode
## Building (CMake)
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
Note: when using in debug mode, assets folder must be in cwd
