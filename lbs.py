import os
import sys
import platform
import datetime
from liblbs import LbsApp


logo = b'ICAgX18gIF8gICAgICAgICAgICAgX19fXyAgICAgICAgICAKICAvIC9fKF8pX19fICBfXyAgX18vIF9fL19fXyAgX19fXyAKIC8gX18vIC8gX18gXC8gLyAvIC8gL18vIF9fIFwvIF9fIFwKLyAvXy8gLyAvIC8gLyAvXy8gLyBfXy8gL18vIC8gL18vIC8KXF9fL18vXy8gL18vXF9fLCAvXy8gIFxfX19fL1xfX19fLyAKICAgICAgICAgICAvX19fXy8gICAgICAgICAgICAgICAgICA='
app = LbsApp(sys.argv[1:], __file__)
if app.exit_code:
    sys.exit(app.exit_code)
app.print_logo(logo)

if not app.stage or app.stage == 'help':
    app.info(f'Usage: "{sys.executable}" "{__file__}" [dir_name] [init/fetch/conf/gen_res/join_code]')
    sys.exit(0)

if app.stage == 'init':
    if not app.default_stage_init():
        sys.exit(1)
    if app.conf['msvc'] and not app.b_name == 'msvc':
        app.warn('Please use "msvc" dir for Visual Studio builds')
    app.save_conf()
    app.info('Inited!')
    sys.exit(0)

if app.stage == 'fetch':
    if not app.load_conf():
        sys.exit(1)
    if app.conf['msvc']:
        app.download_sdl_lib('', '-VC.zip', 3, os.path.join(app.b_path, 'SDL'))
    elif app.conf['mingw']:
        app.download_sdl_lib('', '-mingw.zip', 3, os.path.join(app.b_path, 'SDL'))
    else:
        pass
    ig_branch = 'master'
    app.download_imgui_files(os.path.join(app.b_path, 'imgui'), (
        f'https://github.com/ocornut/imgui/raw/refs/heads/{ig_branch}/imgui.cpp',
        f'https://github.com/ocornut/imgui/raw/refs/heads/{ig_branch}/imgui.h',
        f'https://github.com/ocornut/imgui/raw/refs/heads/{ig_branch}/imgui_draw.cpp',
        f'https://github.com/ocornut/imgui/raw/refs/heads/{ig_branch}/imgui_internal.h',
        f'https://github.com/ocornut/imgui/raw/refs/heads/{ig_branch}/imgui_tables.cpp',
        f'https://github.com/ocornut/imgui/raw/refs/heads/{ig_branch}/imgui_widgets.cpp',
        f'https://github.com/ocornut/imgui/raw/refs/heads/{ig_branch}/imstb_rectpack.h',
        f'https://github.com/ocornut/imgui/raw/refs/heads/{ig_branch}/imstb_textedit.h',
        f'https://github.com/ocornut/imgui/raw/refs/heads/{ig_branch}/imstb_truetype.h',
        f'https://github.com/ocornut/imgui/raw/refs/heads/{ig_branch}/backends/imgui_impl_sdl3.cpp',
        f'https://github.com/ocornut/imgui/raw/refs/heads/{ig_branch}/backends/imgui_impl_sdl3.h',
        f'https://github.com/ocornut/imgui/raw/refs/heads/{ig_branch}/backends/imgui_impl_sdlrenderer3.cpp',
        f'https://github.com/ocornut/imgui/raw/refs/heads/{ig_branch}/backends/imgui_impl_sdlrenderer3.h'
    ))
    if not os.path.isdir(os.path.join(app.b_path, 'toml')):
        os.mkdir(os.path.join(app.b_path, 'toml'))
    app.download_toml11_lib(os.path.join(app.b_path, 'toml', 'toml11.hpp'))
    app.download_tomlpp_lib(os.path.join(app.b_path, 'toml', 'tomlpp.hpp'))
    if not os.path.isdir(os.path.join(app.b_path, 'nlohmann')):
        os.mkdir(os.path.join(app.b_path, 'nlohmann'))
    app.download_nlohmann_json_lib(os.path.join(app.b_path, 'nlohmann', 'json.hpp'), fwd=True)
    app.info('Fetched!')
    sys.exit(0)

if app.stage == 'conf':
    if not app.load_conf():
        sys.exit(1)
    app.conf['extra_libs'] = []
    conf_header = open(os.path.join(app.b_path, 'lbs', 'lbs.hpp'), 'w', encoding='utf-8')
    is_win = int(sys.platform == 'win32')
    is_release = int('--release' in app.args)
    conf_header.write(f'#pragma once\n\n')
    conf_header.write(f'#define LBS_PLATFORM "{sys.platform}"\n')
    conf_header.write(f'#define LBS_PYTHON "{sys.version}"\n')
    conf_header.write(f'#define LBS_MACHINE "{platform.machine()}"\n')
    conf_header.write(f'#define LBS_SYSTEM "{platform.platform()}"\n')
    conf_header.write(f'#define LBS_CPU "{platform.processor()}"\n')
    conf_header.write(f'#define LBS_CONF_TIME "{datetime.datetime.today().strftime(r"%Y-%m-%d %H:%M:%S")}"\n')
    conf_header.write(f'#define IS_MSVC {int(app.conf["msvc"])}\n')
    conf_header.write(f'#define IS_MINGW {int(app.conf["mingw"])}\n')
    conf_header.write(f'#define IS_DLL_BUILD 0\n')
    if app.conf['msvc']:
        conf_header.write(f'#ifdef _DEBUG\n')
        conf_header.write(f'#define IS_RELEASE 0\n')
        conf_header.write(f'#else\n')
        conf_header.write(f'#define IS_RELEASE 1\n')
        conf_header.write(f'#endif\n')
    else:
        conf_header.write(f'#define IS_RELEASE {is_release}\n')
    conf_header.write(f'#define IS_WIN {is_win}\n')
    conf_header.write(f'#define ENABLE_IMGUI 1\n')
    can_gdi = app.conf["msvc"]
    if app.conf['msvc']:
        conf_header.write(f'#if _MSVC_LANG >= 201703L\n')
    elif app.conf['mingw']:
        # it's broken here somewhy
        conf_header.write('#if 0\n')
    else:
        conf_header.write(f'#if __cplusplus >= 201703L\n')
    conf_header.write(f'#define ENABLE_TOMLPP 1\n')
    conf_header.write(f'#else\n')
    conf_header.write(f'#define ENABLE_TOMLPP 0\n')
    conf_header.write(f'#endif\n')
    conf_header.write(f'#define ENABLE_UPNG {int(not can_gdi)}\n')
    conf_header.write(f'#define ENABLE_GDIPLUS {int(can_gdi)}\n')
    conf_header.write(f'#define ENABLE_SDL2_MIXER 1\n')
    conf_header.write(f'#define ENABLE_FMOD 1\n')
    conf_header.write(f'#define ENABLE_BASS 1\n')
    conf_header.write(f'#define ENABLE_SOLOUD 1\n')
    conf_header.write(f'#define ENABLE_STDOUT_LOGGING 1\n')
    conf_header.write(f'#define BUMP_SIZE 4096\n')
    conf_header.write(f'#define MIN_LOG_LEVEL {1 if is_release else 0}\n')
    conf_header.write(f'#define LOG_CACHE_COUNT 1024\n')
    conf_header.write(f'#define MUSIC_SEARCH_LIMIT 2500\n')
    conf_header.write(f'#define DOUBLE_CLICK_TIME 250\n')
    conf_header.write(f'#define WIN_TITLE_PATCH 1\n')
    conf_header.write(f'#define DWM_STATUS_PATCH 0\n')
    conf_header.write(f'#define DWM_STATUS_BUF_SIZE 1024\n')
    conf_header.write(f'#define DWM_STATUS_FN_SIZE 512\n')
    conf_header.close()
    open(os.path.join(app.b_path, 'imgui', 'imconfig.h'), 'w', encoding='utf-8').write(
'''#pragma once
#include <lbs.hpp>

#define IMGUI_DISABLE_DEFAULT_ALLOCATORS
#if IS_RELEASE
#define IM_ASSERT(_EXPR)  ((void)(_EXPR))
#define IMGUI_DISABLE_DEMO_WINDOWS
#define IMGUI_DISABLE_DEBUG_TOOLS
#endif
'''
    )
    app.save_conf()
    app.info(f'Configured at "{os.path.join(app.b_path, "lbs", "lbs.hpp")}"')
    sys.exit(0)

if app.stage == 'gen_res':
    def write_res(name, data):
        f.write('const unsigned char ')
        f.write(name)
        f.write('[] = { ')
        f.write(', '.join(map(hex, list(data))))
        f.write(' };\n')

    f = open(os.path.join(app.b_path, 'lbs', 'res_data.hpp'), 'w', encoding='utf-8')
    f.write('#pragma once\n\n')
    write_res('asset_icon', open(os.path.join(app.cwd, 'assets', 'icon.png'), 'rb').read())
    write_res('asset_icon_stop', open(os.path.join(app.cwd, 'assets', 'icon_stop.png'), 'rb').read())
    write_res('asset_icon_play', open(os.path.join(app.cwd, 'assets', 'icon_play.png'), 'rb').read())
    write_res('asset_icon_pause', open(os.path.join(app.cwd, 'assets', 'icon_pause.png'), 'rb').read())
    write_res('asset_icon_back', open(os.path.join(app.cwd, 'assets', 'icon_back.png'), 'rb').read())
    write_res('asset_icon_fwd', open(os.path.join(app.cwd, 'assets', 'icon_fwd.png'), 'rb').read())
    write_res('asset_icon_rng', open(os.path.join(app.cwd, 'assets', 'icon_rng.png'), 'rb').read())
    write_res('asset_font1', open(os.path.join(app.cwd, 'assets', 'Roboto-Regular.ttf'), 'rb').read())
    f.close()
    app.info('Generated resources for release!')
    sys.exit(0)

if app.stage == 'join_code':
    f = open(os.path.join(app.b_path, 'tinyfoo.cpp'), 'w', encoding='utf-8')
    f.write(app.join_code(
        sorted(app.cool_list_dir(os.path.join(app.cwd, 'src'), '.cpp'), key=lambda x: ('' if x.endswith('main.cpp') else x)) +
        app.cool_list_dir(os.path.join(app.b_path, 'imgui'), '.cpp'),
        app.cool_list_dir(os.path.join(app.cwd, 'include'), '.hpp') +
        # app.cool_list_dir(os.path.join(app.b_path, 'imgui'), '.h') +
        app.cool_list_dir(os.path.join(app.b_path, 'nlohmann'), '.hpp') +
        app.cool_list_dir(os.path.join(app.b_path, 'toml'), '.hpp')
    ))
    f.close()
    app.info('Code joined!')
    sys.exit(0)
