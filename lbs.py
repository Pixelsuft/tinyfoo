import os
import sys
import json
from liblbs import LbsApp


logo = b'ICAgX18gIF8gICAgICAgICAgICAgX19fXyAgICAgICAgICAKICAvIC9fKF8pX19fICBfXyAgX18vIF9fL19fXyAgX19fXyAKIC8gX18vIC8gX18gXC8gLyAvIC8gL18vIF9fIFwvIF9fIFwKLyAvXy8gLyAvIC8gLyAvXy8gLyBfXy8gL18vIC8gL18vIC8KXF9fL18vXy8gL18vXF9fLCAvXy8gIFxfX19fL1xfX19fLyAKICAgICAgICAgICAvX19fXy8gICAgICAgICAgICAgICAgICA='
app = LbsApp(sys.argv[1:], __file__)
if app.exit_code:
    sys.exit(app.exit_code)
app.print_logo(logo)

if not app.stage:
    app.warn('TODO: write to use help')
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
    app.download_imgui_files(os.path.join(app.b_path, 'imgui'), (
        # 'https://github.com/ocornut/imgui/raw/refs/heads/master/imconfig.h',
        'https://github.com/ocornut/imgui/raw/refs/heads/master/imgui.cpp',
        'https://github.com/ocornut/imgui/raw/refs/heads/master/imgui.h',
        'https://github.com/ocornut/imgui/raw/refs/heads/master/imgui_draw.cpp',
        'https://github.com/ocornut/imgui/raw/refs/heads/master/imgui_internal.h',
        'https://github.com/ocornut/imgui/raw/refs/heads/master/imgui_tables.cpp',
        'https://github.com/ocornut/imgui/raw/refs/heads/master/imgui_widgets.cpp',
        'https://github.com/ocornut/imgui/raw/refs/heads/master/imstb_rectpack.h',
        'https://github.com/ocornut/imgui/raw/refs/heads/master/imstb_textedit.h',
        'https://github.com/ocornut/imgui/raw/refs/heads/master/imstb_truetype.h',
        'https://github.com/ocornut/imgui/raw/refs/heads/master/backends/imgui_impl_sdl3.cpp',
        'https://github.com/ocornut/imgui/raw/refs/heads/master/backends/imgui_impl_sdl3.h',
        'https://github.com/ocornut/imgui/raw/refs/heads/master/backends/imgui_impl_sdlrenderer3.cpp',
        'https://github.com/ocornut/imgui/raw/refs/heads/master/backends/imgui_impl_sdlrenderer3.h'
    ))
    if not os.path.isdir(os.path.join(app.b_path, 'bpstd')):
        os.mkdir(os.path.join(app.b_path, 'bpstd'))
    if not os.path.isdir(os.path.join(app.b_path, 'bpstd', 'bpstd')):
        os.mkdir(os.path.join(app.b_path, 'bpstd', 'bpstd'))
    app.download_bpstd_string_view_lib(os.path.join(app.b_path, 'bpstd', 'bpstd', 'string_view.hpp'))
    if not os.path.isdir(os.path.join(app.b_path, 'toml11')):
        os.mkdir(os.path.join(app.b_path, 'toml11'))
    app.download_toml11_lib(os.path.join(app.b_path, 'toml11', 'toml.hpp'))
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
    # TODO: write configuration info like time, os info, etc
    # TODO: MIN_LOG_LEVEL, ENABLE_STDOUT_LOGGING
    is_win = int(sys.platform == 'win32')
    is_release = int('--release' in app.args)
    conf_header.write('#pragma once\n\n')
    conf_header.write(f'#define IS_MSVC {int(app.conf["msvc"])}\n')
    conf_header.write(f'#define IS_MINGW {int(app.conf["mingw"])}\n')
    conf_header.write(f'#define IS_RELEASE {is_release}\n')
    conf_header.write(f'#define IS_WIN {is_win}\n')
    conf_header.write(f'#define MIN_LOG_LEVEL {1 if is_release else 0}\n')
    conf_header.write(f'#define LOG_CACHE_COUNT 1024\n')
    conf_header.write(f'#define IS_IMGUI 1\n')
    conf_header.write('#define BUMP_SIZE 4096\n')
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
    app.info('Configured!')
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
    write_res('asset_font1', open(os.path.join(app.cwd, 'assets', 'Roboto-Regular.ttf'), 'rb').read())
    f.close()
    app.info('Generated resources for release!')
    sys.exit(0)
