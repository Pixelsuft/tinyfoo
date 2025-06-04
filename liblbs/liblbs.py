import os
import sys
import subprocess
import base64
import shutil
import json
import zipfile
import io
from urllib import request

'''
TODO:
 - Handle networking exceptions
 - Replace asserts with return False and warns
 - More validations (example: msvc year)
'''

class LbsApp:
    def __init__(self, args: list, main_file: str) -> None:
        self.exit_code = 0
        self.args = args
        self.cwd = os.path.dirname(main_file or __file__) or os.getcwd()
        self.b_name = args[0] if args else ''
        self.b_path = os.path.join(self.cwd, args[0]) if args else ''
        self.stage = args[1] if len(args) > 1 else ''
        self.conf = {}
    
    def default_stage_init(self) -> bool:
        self.info('Trying to do basic init...')
        if not self.validate_b_name():
            return False
        if os.path.isdir(self.b_path):
            # TODO: should I do that???
            shutil.rmtree(self.b_path)
        if not os.path.isdir(self.b_path):
            os.mkdir(self.b_path)
        if not os.path.isdir(os.path.join(self.b_path, 'lbs')):
            os.mkdir(os.path.join(self.b_path, 'lbs'))
        if '--mingw' in self.args and '--msvc' in self.args:
            self.error('Can\'t use MSVC and MINGW at the same time')
            return False
        self.conf['mingw'] = '--mingw' in self.args
        self.conf['msvc'] = '--msvc' in self.args
        self.conf['msvc_year'] = 2022
        if '--msvc-year' in self.args[:-1]:
            self.conf['msvc_year'] = int(self.args[self.args.index('--msvc-year') + 1])
        msvc_ver_map = {
            2022: 17,
            2019: 16,
            2017: 15,
            2015: 14,
            2013: 12,
            2012: 11,
            2010: 10
        }
        self.conf['msvc_ver'] = msvc_ver_map.get(self.conf['msvc_year'], msvc_ver_map[max(msvc_ver_map.keys())])
        return True
    
    def save_conf(self) -> bool:
        open(os.path.join(self.b_path, '.lbs'), 'w', encoding='utf-8').write(
            json.dumps(self.conf, indent=4) + '\n'
        )
        return True
    
    def load_conf(self) -> bool:
        if os.path.isfile(os.path.join(self.b_path, '.lbs')):
            self.conf = json.loads(open(os.path.join(self.b_path, '.lbs'), 'r', encoding='utf-8').read())
            return True
        self.warn('LBS config not found')
        return False

    def validate_b_name(self) -> bool:
        if not self.b_name:
            self.warn('LBS output dir is not specified')
            return False
        if all(x in '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ' for x in self.args[0]):
            return True
        self.error('Invalid LBS output dir')
        return False
    
    def info(self, *args, **kwargs) -> any:
        print('[INFO]', *args, **kwargs)
    
    def warn(self, *args, **kwargs) -> any:
        print('[WARN]', *args, **kwargs)
    
    def error(self, *args, **kwargs) -> any:
        print('[ERROR]', *args, **kwargs)
    
    def download_miniz_lib(self, dst: str, rel_url: str = 'https://api.github.com/repos/richgel999/miniz/releases/latest') -> bool:
        if os.path.isdir(dst):
            self.info('Miniz found')
            return True
        else:
            self.info('Fetching Miniz...')
            resp = self.request_json(rel_url)
            assert len(resp['assets']) == 1
            zip_data = self.request_bin(resp['assets'][0]['browser_download_url'])
            self.info(f'Unzipping Miniz...')
            zip_f = zipfile.ZipFile(io.BytesIO(zip_data))
            os.mkdir(dst)
            zip_f.extractall(dst)
            return True
    
    def download_spng_lib(self, dst: str, archive_url: str = 'https://github.com/Pixelsuft/libspng/archive/refs/heads/main.zip') -> bool:
        if os.path.isdir(dst):
            self.info('SPNG found')
            return True
        else:
            self.info('Fetching SPNG...')
            zip_data = self.request_bin(archive_url)
            self.info(f'Unzipping SPNG...')
            zip_f = zipfile.ZipFile(io.BytesIO(zip_data))
            zip_f.extractall(self.lib_path)
            os.rename(os.path.join(self.lib_path, 'libspng-' + archive_url.split('/')[-1].replace('.zip', '')), dst)
            return True

    def download_sdl_lib(self, lib_postfix: str, asset_postfix: str, sdl_ver: int, dst: str) -> bool:
        if os.path.isdir(dst):
            self.info(f'SDL{sdl_ver}{lib_postfix} found')
            return True
        self.info(f'Fetching SDL{sdl_ver}{lib_postfix}...')
        rel_info = self.fetch_sdl_rel_info(lib_postfix, sdl_ver)
        if not rel_info:
            return False
        for asset_info in rel_info['assets']:
            if asset_info['name'].endswith(asset_postfix):
                break
        else:
            self.error(f'SDL{sdl_ver}{lib_postfix} asset info was not found')
            return False
        assert asset_info['name'].endswith(asset_postfix)
        zip_data = self.request_bin(asset_info['browser_download_url'])
        self.info(f'Unzipping SDL{sdl_ver}{lib_postfix}...')
        zp = zipfile.ZipFile(io.BytesIO(zip_data))
        zp.extractall(self.cwd)
        os.rename(os.path.join(self.cwd, 'SDL' + str(sdl_ver) + lib_postfix + '-' + rel_info['name']), dst)
        return True

    def fetch_sdl_rel_info(self, lib_postfix: str, sdl_ver: int) -> dict:
        resp = self.request_json(f'https://api.github.com/repos/libsdl-org/SDL{lib_postfix}/releases')
        assert len(resp) >= 1
        for rel_info in resp:
            if rel_info['name'].startswith(str(sdl_ver) + '.'):
                break
        else:
            self.error(f'Failed to fetch SDL{sdl_ver}{lib_postfix} release info')
            return {}
        assert len(rel_info['assets']) >= 1
        return rel_info
    
    def download_nlohmann_json_lib(self, dst: str, url: str = 'https://github.com/nlohmann/json/releases/latest/download/json.hpp', fwd: bool = False) -> None:
        dst_fwd = dst.replace('json.hpp', 'json_fwd.hpp') if fwd else ''
        if os.path.isfile(dst) and (not fwd or os.path.isfile(dst_fwd)):
            self.info('Nlohmann JSON found')
            return True
        else:
            self.info('Fetching Nlohmann JSON...')
            data = self.request_bin(url)
            open(dst, 'wb').write(data)
            if fwd:
                data = self.request_bin(url.replace('json.hpp', 'json_fwd.hpp'))
                open(dst_fwd, 'wb').write(data)
            return True

    def download_toml11_lib(self, dst: str, url: str = 'https://github.com/ToruNiina/toml11/raw/refs/heads/main/single_include/toml.hpp') -> None:
        if os.path.isfile(dst):
            self.info('toml11 found')
            return True
        else:
            self.info('Fetching toml11...')
            data = self.request_bin(url)
            open(dst, 'wb').write(data)
            return True
    
    def download_bpstd_string_view_lib(self, dst: str, url: str = 'https://github.com/bitwizeshift/string_view-standalone/raw/refs/heads/master/single_include/bpstd/string_view.hpp') -> None:
        if os.path.isfile(dst):
            self.info('bpstd string_view found')
            return True
        else:
            self.info('Fetching bpstd string_view...')
            data = self.request_bin(url)
            open(dst, 'wb').write(data)
            return True
    
    def download_imgui_files(self, dst_dir: str, urls: any) -> None:
        made_dir = False
        if not os.path.isdir(dst_dir):
            self.info('Fetching ImGui...')
            os.mkdir(dst_dir)
            made_dir = True
        for url in urls:
            fn = url.strip('/').split('/')[-1]
            dst = os.path.join(dst_dir, fn)
            if os.path.isfile(dst):
                pass
            else:
                if not made_dir:
                    self.warn(f'ImGui "{fn}" not found, fetching...')
                data = self.request_bin(url)
                open(dst, 'wb').write(data)
        return True

    def print_logo(self, data: bin) -> None:
        print(base64.b64decode(data).decode())
        self.info('Lexa\'s build system (WIP)')

    @staticmethod
    def request_json(url: str) -> dict:
        resp = request.urlopen(url)
        return json.loads(resp.read().decode(resp.headers.get_content_charset(), errors='replace'))

    @staticmethod
    def request_bin(url: str) -> bytes:
        resp = request.urlopen(url)
        return resp.read()
