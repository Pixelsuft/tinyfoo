#include <lbs.hpp>
#if ENABLE_BASS
#include <audio_base.hpp>
#include <new.hpp>
#include <lbs.hpp>
#include <log.hpp>
#include <break.hpp>
#include <playlist.hpp>
#include <stl.hpp>
#include <conf.hpp>
#include <SDL3/SDL.h>
#define BASS_LIKES_WCHAR (defined(_WIN32_WCE) || (defined(WINAPI_FAMILY) && WINAPI_FAMILY != WINAPI_FAMILY_DESKTOP_APP))

#if 1
#if IS_WIN
#include <wtypes.h>
#ifdef WINAPI_FAMILY
#include <winapifamily.h>
#endif
typedef unsigned __int64 QWORD;
#else
#define WINAPI
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;
typedef int BOOL;
#endif
// Actually a hack
#define BASSDEF(f) (WINAPI *f)

#define BASS_OK				0
#define BASS_ERROR_MEM		1
#define BASS_ERROR_FILEOPEN	2
#define BASS_ERROR_DRIVER	3
#define BASS_ERROR_BUFLOST	4
#define BASS_ERROR_HANDLE	5
#define BASS_ERROR_FORMAT	6
#define BASS_ERROR_POSITION	7
#define BASS_ERROR_INIT		8
#define BASS_ERROR_START	9
#define BASS_ERROR_SSL		10
#define BASS_ERROR_REINIT	11
#define BASS_ERROR_ALREADY	14
#define BASS_ERROR_NOTAUDIO	17
#define BASS_ERROR_NOCHAN	18
#define BASS_ERROR_ILLTYPE	19
#define BASS_ERROR_ILLPARAM	20
#define BASS_ERROR_NO3D		21
#define BASS_ERROR_NOEAX	22
#define BASS_ERROR_DEVICE	23
#define BASS_ERROR_NOPLAY	24
#define BASS_ERROR_FREQ		25
#define BASS_ERROR_NOTFILE	27
#define BASS_ERROR_NOHW		29
#define BASS_ERROR_EMPTY	31
#define BASS_ERROR_NONET	32
#define BASS_ERROR_CREATE	33
#define BASS_ERROR_NOFX		34
#define BASS_ERROR_NOTAVAIL	37
#define BASS_ERROR_DECODE	38
#define BASS_ERROR_DX		39
#define BASS_ERROR_TIMEOUT	40
#define BASS_ERROR_FILEFORM	41
#define BASS_ERROR_SPEAKER	42
#define BASS_ERROR_VERSION	43
#define BASS_ERROR_CODEC	44
#define BASS_ERROR_ENDED	45
#define BASS_ERROR_BUSY		46
#define BASS_ERROR_UNSTREAMABLE	47
#define BASS_ERROR_PROTOCOL	48
#define BASS_ERROR_DENIED	49
#define BASS_ERROR_UNKNOWN	-1

#define BASS_CONFIG_BUFFER			0
#define BASS_CONFIG_UPDATEPERIOD	1
#define BASS_CONFIG_GVOL_SAMPLE		4
#define BASS_CONFIG_GVOL_STREAM		5
#define BASS_CONFIG_GVOL_MUSIC		6
#define BASS_CONFIG_CURVE_VOL		7
#define BASS_CONFIG_CURVE_PAN		8

#define BASS_DEVICE_ENABLED		1

#define BASS_DEVICE_MONO		2
#define BASS_DEVICE_16BITS		8
#define BASS_DEVICE_REINIT		128
#define BASS_DEVICE_SPEAKERS	0x800
#define BASS_DEVICE_NOSPEAKER	0x1000
#define BASS_DEVICE_DMIX		0x2000
#define BASS_DEVICE_FREQ		0x4000
#define BASS_DEVICE_STEREO		0x8000
#define BASS_DEVICE_HOG			0x10000
#define BASS_DEVICE_AUDIOTRACK	0x20000
#define BASS_DEVICE_DSOUND		0x40000
#define BASS_DEVICE_SOFTWARE	0x80000

#define BASS_CTYPE_SAMPLE		1
#define BASS_CTYPE_RECORD		2
#define BASS_CTYPE_STREAM		0x10000
#define BASS_CTYPE_STREAM_VORBIS	0x10002
#define BASS_CTYPE_STREAM_OGG	0x10002
#define BASS_CTYPE_STREAM_MP1	0x10003
#define BASS_CTYPE_STREAM_MP2	0x10004
#define BASS_CTYPE_STREAM_MP3	0x10005
#define BASS_CTYPE_STREAM_AIFF	0x10006
#define BASS_CTYPE_STREAM_CA	0x10007
#define BASS_CTYPE_STREAM_MF	0x10008
#define BASS_CTYPE_STREAM_AM	0x10009
#define BASS_CTYPE_STREAM_SAMPLE	0x1000a
#define BASS_CTYPE_STREAM_DUMMY		0x18000
#define BASS_CTYPE_STREAM_DEVICE	0x18001
#define BASS_CTYPE_STREAM_WAV	0x40000
#define BASS_CTYPE_STREAM_WAV_PCM	0x50001
#define BASS_CTYPE_STREAM_WAV_FLOAT	0x50003
#define BASS_CTYPE_MUSIC_MOD	0x20000
#define BASS_CTYPE_MUSIC_MTM	0x20001
#define BASS_CTYPE_MUSIC_S3M	0x20002
#define BASS_CTYPE_MUSIC_XM		0x20003
#define BASS_CTYPE_MUSIC_IT		0x20004
#define BASS_CTYPE_MUSIC_MO3	0x00100

#define BASS_SAMPLE_FLOAT		256

#define BASS_CONFIG_GVOL_STREAM		5

#define BASS_ACTIVE_STOPPED			0
#define BASS_ACTIVE_PLAYING			1
#define BASS_ACTIVE_STALLED			2
#define BASS_ACTIVE_PAUSED			3
#define BASS_ACTIVE_PAUSED_DEVICE	4

#define BASS_ATTRIB_FREQ			1
#define BASS_ATTRIB_VOL				2
#define BASS_ATTRIB_PAN				3
#define BASS_ATTRIB_EAXMIX			4
#define BASS_ATTRIB_NOBUFFER		5
#define BASS_ATTRIB_VBR				6
#define BASS_ATTRIB_CPU				7
#define BASS_ATTRIB_SRC				8
#define BASS_ATTRIB_NET_RESUME		9
#define BASS_ATTRIB_SCANINFO		10
#define BASS_ATTRIB_NORAMP			11
#define BASS_ATTRIB_BITRATE			12
#define BASS_ATTRIB_BUFFER			13
#define BASS_ATTRIB_GRANULE			14
#define BASS_ATTRIB_USER			15
#define BASS_ATTRIB_TAIL			16
#define BASS_ATTRIB_PUSH_LIMIT		17
#define BASS_ATTRIB_DOWNLOADPROC	18
#define BASS_ATTRIB_VOLDSP			19
#define BASS_ATTRIB_VOLDSP_PRIORITY	20
#define BASS_ATTRIB_MUSIC_AMPLIFY	0x100
#define BASS_ATTRIB_MUSIC_PANSEP	0x101
#define BASS_ATTRIB_MUSIC_PSCALER	0x102
#define BASS_ATTRIB_MUSIC_BPM		0x103
#define BASS_ATTRIB_MUSIC_SPEED		0x104
#define BASS_ATTRIB_MUSIC_VOL_GLOBAL 0x105
#define BASS_ATTRIB_MUSIC_ACTIVE	0x106
#define BASS_ATTRIB_MUSIC_VOL_CHAN	0x200
#define BASS_ATTRIB_MUSIC_VOL_INST	0x300

#define BASS_SLIDE_LOG				0x1000000

#define BASS_POS_BYTE			0
#define BASS_POS_MUSIC_ORDER	1
#define BASS_POS_OGG			3
#define BASS_POS_END			0x10
#define BASS_POS_LOOP			0x11
#define BASS_POS_FLUSH			0x1000000
#define BASS_POS_RESET			0x2000000
#define BASS_POS_RELATIVE		0x4000000
#define BASS_POS_INEXACT		0x8000000
#define BASS_POS_DECODE			0x10000000
#define BASS_POS_DECODETO		0x20000000
#define BASS_POS_SCAN			0x40000000

typedef DWORD HMUSIC;
typedef DWORD HSAMPLE;
typedef DWORD HCHANNEL;
typedef DWORD HSTREAM;
typedef DWORD HRECORD;
typedef DWORD HSYNC;
typedef DWORD HDSP;
typedef DWORD HFX;
typedef DWORD HPLUGIN;

typedef struct {
#if BASS_LIKES_WCHAR
	const wchar_t* name;
	const wchar_t* driver;
#else
	const char* name;
	const char* driver;
#endif
	DWORD flags;
} BASS_DEVICEINFO;

typedef struct {
	DWORD flags;	
	DWORD hwsize;	
	DWORD hwfree;	
	DWORD freesam;	
	DWORD free3d;	
	DWORD minrate;	
	DWORD maxrate;	
	BOOL eax;		
	DWORD minbuf;	
	DWORD dsver;	
	DWORD latency;	
	DWORD initflags; 
	DWORD speakers; 
	DWORD freq;		
} BASS_INFO;

typedef struct {
	DWORD freq;
	DWORD chans;
	DWORD flags;
	DWORD ctype;
	DWORD origres;
	HPLUGIN plugin;
	HSAMPLE sample;
	const char* filename;
} BASS_CHANNELINFO;

#define BASS_LOAD_FUNC(func_name) do { \
    *(void**)&bass.func_name = (void*)SDL_LoadFunction(bass.handle, #func_name); \
    if (!bass.func_name) { \
        TF_ERROR(<< "Failed to load " << lib_name << " function \"" << #func_name << "\" (" << SDL_GetError() << ")"); \
        SDL_UnloadObject(bass.handle); \
        return; \
    } \
} while (0)
#endif
#define mus_h (*((DWORD*)(&mus->h1)))
#define cur_h (*((DWORD*)(&cur_mus->h1)))

static inline const char* BASS_ErrorString(int errcode) {
    switch (errcode) {
        case BASS_OK: return "All is OK";
        case BASS_ERROR_MEM: return "Memory error";
        case BASS_ERROR_FILEOPEN: return "Can't open the file";
        case BASS_ERROR_DRIVER: return "Can't find a free/valid driver";
        case BASS_ERROR_BUFLOST: return "The sample buffer was lost";
        case BASS_ERROR_HANDLE: return "Invalid handle";
        case BASS_ERROR_FORMAT: return "Unsupported sample format";
        case BASS_ERROR_POSITION: return "Invalid position";
        case BASS_ERROR_INIT: return "BASS_Init has not been successfully called";
        case BASS_ERROR_START: return "BASS_Start has not been successfully called";
        case BASS_ERROR_SSL: return "SSL/HTTPS support isn't available";
        case BASS_ERROR_REINIT: return "Device needs to be reinitialized";
        case BASS_ERROR_ALREADY: return "Already initialized/paused/whatever";
        case BASS_ERROR_NOTAUDIO: return "File does not contain audio";
        case BASS_ERROR_NOCHAN: return "Can't get a free channel";
        case BASS_ERROR_ILLTYPE: return "An illegal type was specified";
        case BASS_ERROR_ILLPARAM: return "An illegal parameter was specified";
        case BASS_ERROR_NO3D: return "No 3D support";
        case BASS_ERROR_NOEAX: return "No EAX support";
        case BASS_ERROR_DEVICE: return "Illegal device number";
        case BASS_ERROR_NOPLAY: return "Not playing";
        case BASS_ERROR_FREQ: return "Illegal sample rate";
        case BASS_ERROR_NOTFILE: return "The stream is not a file stream";
        case BASS_ERROR_NOHW: return "No hardware voices available";
        case BASS_ERROR_EMPTY: return "The file has no sample data";
        case BASS_ERROR_NONET: return "No internet connection could be opened";
        case BASS_ERROR_CREATE: return "Couldn't create the file";
        case BASS_ERROR_NOFX: return "Effects are not available";
        case BASS_ERROR_NOTAVAIL: return "Requested data/action is not available";
        case BASS_ERROR_DECODE: return "The channel is/isn't a \"decoding channel\"";
        case BASS_ERROR_DX: return "A sufficient DirectX version is not installed";
        case BASS_ERROR_TIMEOUT: return "Connection timedout";
        case BASS_ERROR_FILEFORM: return "Unsupported file format";
        case BASS_ERROR_SPEAKER: return "Unavailable speaker";
        case BASS_ERROR_VERSION: return "Invalid BASS version (used by add-ons)";
        case BASS_ERROR_CODEC: return "Codec is not available/supported";
        case BASS_ERROR_ENDED: return "The channel/file has ended";
        case BASS_ERROR_BUSY: return "The device is busy";
        case BASS_ERROR_UNSTREAMABLE: return "Unstreamable file";
        case BASS_ERROR_PROTOCOL: return "Unsupported protocol";
        case BASS_ERROR_DENIED: return "Access denied";
        case BASS_ERROR_UNKNOWN: return "Some other mystery problem";
    }
    return "Some other mystery problem";
}
#define BASS_GetError() BASS_ErrorString(bass.BASS_ErrorGetCode())

namespace app {
    extern void* win_handle;
}

namespace audio {
    struct BASSApi {
        SDL_SharedObject* handle;
        BOOL BASSDEF(BASS_Init)(int, DWORD, DWORD, void*, const void*);
        BOOL BASSDEF(BASS_Free)();
        int BASSDEF(BASS_ErrorGetCode)(void);
        BOOL BASSDEF(BASS_SetConfig)(DWORD, DWORD);
        BOOL BASSDEF(BASS_SetConfigPtr)(DWORD, const void*);
        DWORD BASSDEF(BASS_GetVersion)(void);
        BOOL BASSDEF(BASS_GetInfo)(BASS_INFO*);
        DWORD BASSDEF(BASS_GetCPU)(void);
        DWORD BASSDEF(BASS_GetDevice)(void);
        BOOL BASSDEF(BASS_GetDeviceInfo)(DWORD, BASS_DEVICEINFO*);
        HSTREAM BASSDEF(BASS_StreamCreateFile)(BOOL, const void*, QWORD, QWORD, DWORD);
        BOOL BASSDEF(BASS_StreamFree)(HSTREAM);
        HMUSIC BASSDEF(BASS_MusicLoad)(BOOL, const void*, QWORD, DWORD, DWORD, DWORD);
        BOOL BASSDEF(BASS_MusicFree)(HMUSIC);
        DWORD BASSDEF(BASS_ChannelIsActive)(DWORD);
        BOOL BASSDEF(BASS_ChannelGetInfo)(DWORD, BASS_CHANNELINFO*);
        BOOL BASSDEF(BASS_ChannelPlay)(DWORD, BOOL);
        BOOL BASSDEF(BASS_ChannelStart)(DWORD);
        BOOL BASSDEF(BASS_ChannelStop)(DWORD);
        BOOL BASSDEF(BASS_ChannelPause)(DWORD);
        BOOL BASSDEF(BASS_ChannelUpdate)(DWORD, DWORD);
        BOOL BASSDEF(BASS_ChannelSlideAttribute)(DWORD, DWORD, float, DWORD);
        BOOL BASSDEF(BASS_ChannelIsSliding)(DWORD, DWORD);
        QWORD BASSDEF(BASS_ChannelGetLength)(DWORD, DWORD);
        BOOL BASSDEF(BASS_ChannelSetPosition)(DWORD, QWORD, DWORD);
        QWORD BASSDEF(BASS_ChannelGetPosition)(DWORD, DWORD);
        BOOL BASSDEF(BASS_ChannelSetAttribute)(DWORD, DWORD, float);
        double BASSDEF(BASS_ChannelBytes2Seconds)(DWORD, QWORD);
        QWORD BASSDEF(BASS_ChannelSeconds2Bytes)(DWORD, double);
    };

    class AudioBASS : public AudioBase {
        protected:
        BASSApi bass;
        public:
        float pause_pos;
        bool paused;
        bool stopped;
        bool fading; // Actually for stopping only

        AudioBASS() : AudioBase() {
            lib_name = "BASS";
            pause_pos = 0.f;
            paused = stopped = fading = false;
            const char* lib_path = IS_WIN ? "bass.dll" : "libbass.so";
            bass.handle = SDL_LoadObject(lib_path);
            if (!bass.handle) {
                TF_WARN(<< "Failed to load " << lib_name << " library (" << SDL_GetError() << ")");
                return;
            }
            BASS_LOAD_FUNC(BASS_Init);
            BASS_LOAD_FUNC(BASS_Free);
            BASS_LOAD_FUNC(BASS_ErrorGetCode);
            BASS_LOAD_FUNC(BASS_SetConfig);
            BASS_LOAD_FUNC(BASS_SetConfigPtr);
            BASS_LOAD_FUNC(BASS_GetVersion);
            BASS_LOAD_FUNC(BASS_GetInfo);
            BASS_LOAD_FUNC(BASS_GetCPU);
            BASS_LOAD_FUNC(BASS_GetDevice);
            BASS_LOAD_FUNC(BASS_GetDeviceInfo);
            BASS_LOAD_FUNC(BASS_StreamCreateFile);
            BASS_LOAD_FUNC(BASS_StreamFree);
            BASS_LOAD_FUNC(BASS_MusicLoad);
            BASS_LOAD_FUNC(BASS_MusicFree);
            BASS_LOAD_FUNC(BASS_ChannelIsActive);
            BASS_LOAD_FUNC(BASS_ChannelGetInfo);
            BASS_LOAD_FUNC(BASS_ChannelPlay);
            BASS_LOAD_FUNC(BASS_ChannelStart);
            BASS_LOAD_FUNC(BASS_ChannelStop);
            BASS_LOAD_FUNC(BASS_ChannelPause);
            BASS_LOAD_FUNC(BASS_ChannelUpdate);
            BASS_LOAD_FUNC(BASS_ChannelSlideAttribute);
            BASS_LOAD_FUNC(BASS_ChannelIsSliding);
            BASS_LOAD_FUNC(BASS_ChannelGetLength);
            BASS_LOAD_FUNC(BASS_ChannelSetPosition);
            BASS_LOAD_FUNC(BASS_ChannelGetPosition);
            BASS_LOAD_FUNC(BASS_ChannelSetAttribute);
            BASS_LOAD_FUNC(BASS_ChannelBytes2Seconds);
            BASS_LOAD_FUNC(BASS_ChannelSeconds2Bytes);
            TF_INFO(<< "BASS inited successfully");
            inited = true;
        }

        bool dev_open() {
            BASS_DEVICEINFO d_info;
            void* hwnd = nullptr;
#if IS_WIN
            SDL_PropertiesID props = SDL_GetWindowProperties((SDL_Window*)app::win_handle);
            if (props) {
                hwnd = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
                SDL_DestroyProperties(props);
            }
            else
                TF_WARN(<< "Failed to get window props (" << SDL_GetError() << ")");
#endif
            DWORD init_flags = 0;
            int need_freq = 0;
            if (conf::get().contains("bass") && conf::get().at("bass").is_table()) {
                toml::value tab = conf::get().at("bass");
                need_dev = toml::find_or<tf::str>(tab, "device", need_dev);
                if (toml::find_or<bool>(tab, "force_16bits", false))
                    init_flags |= BASS_DEVICE_16BITS;
                if (toml::find_or<bool>(tab, "force_stereo", false))
                    init_flags |= BASS_DEVICE_STEREO;
                if (toml::find_or<bool>(tab, "force_dmix", false))
                    init_flags |= BASS_DEVICE_DMIX;
                if (toml::find_or<bool>(tab, "force_audiotrack", false))
                    init_flags |= BASS_DEVICE_AUDIOTRACK;
                if (toml::find_or<bool>(tab, "force_directsound", false))
                    init_flags |= BASS_DEVICE_DSOUND;
                if (toml::find_or<bool>(tab, "force_software", false))
                    init_flags |= BASS_DEVICE_SOFTWARE;
                need_freq = toml::find_or<int>(tab, "frequency", 0);
            }
            if (need_freq > 0)
                init_flags |= BASS_DEVICE_FREQ;
            int dev_id = -1;
            for (int i = 0; bass.BASS_GetDeviceInfo(i, &d_info); i++) {
                if (!(d_info.flags & BASS_DEVICE_ENABLED))
                    continue;
                // TF_INFO(<< "Found audio device: " << d_info.name);
#if BASS_LIKES_WCHAR
                // TODO
#else
                if (!SDL_strcmp(need_dev.c_str(), d_info.name)) {
                    dev_id = i;
                    break;
                }
#endif
            }
            if (!bass.BASS_Init(dev_id, (need_freq > 0) ? need_freq : 48000, init_flags, hwnd, nullptr)) {
                TF_INFO(<< "Failed to create BASS device (" << BASS_GetError() << ")");
                return false;
            }
            BASS_INFO info;
            if (!bass.BASS_GetInfo(&info)) {
                TF_WARN(<< "Failed to get BASS info (" << BASS_GetError() << ")");
                info.freq = 0;
            }
            if (!bass.BASS_GetDeviceInfo(bass.BASS_GetDevice(), &d_info)) {
                TF_WARN(<< "Failed to get audio device info (" << BASS_GetError() << ")");
#if BASS_LIKES_WCHAR
                d_info.name = L"";
#else
                d_info.name = "";
#endif
            }
            // bass.BASS_SetConfig(BASS_CONFIG_CURVE_VOL, 1);
            TF_INFO(<< "Audio device \"" << d_info.name << "\" opened (" << info.freq << "Hz freq)");
            dev_opened = true;
            return true;
        }

        void dev_close() {
            if (!bass.BASS_Free())
                TF_ERROR(<< "Failed to close audio device (" << BASS_GetError() << ")");
            dev_opened = false;
        }

        void dev_fill_arr(tf::vec<tf::str>& arr) {
            BASS_DEVICEINFO d_info;
            arr.push_back("Default");
            for (int i = 2; bass.BASS_GetDeviceInfo(i, &d_info); i++) {
                if (!(d_info.flags & BASS_DEVICE_ENABLED))
                    continue;
#if BASS_LIKES_WCHAR
                // TODO
#else
                arr.push_back(d_info.name);
#endif
            }
        }

        bool mus_open_fp(Music* mus, const char* fp) {
            if (mus->h1)
                return true;
            HSTREAM h = bass.BASS_StreamCreateFile(0, (const void*)fp, 0, 0, BASS_SAMPLE_FLOAT);
            if (!h && bass.BASS_ErrorGetCode() == BASS_ERROR_FILEFORM) {
                // Is last arg right?
                h = bass.BASS_MusicLoad(0, (const void*)fp, 0, 0, 0, 1);
                if (h)
                    mus->h2 = (void*)((size_t)6969);
            }
            if (!h) {
                TF_ERROR(<< "Failed to open music \"" << fp << "\" (" << BASS_GetError() << ")");
                return false;
            }
            mus_h = h;
            return true;
        }

        void mus_close(Music* mus) {
            if (!mus_h)
                return;
            if (!((((size_t)mus->h2) == 6969) ? bass.BASS_MusicFree(mus_h) : bass.BASS_StreamFree(mus_h)))
                TF_ERROR(<< "Failed to free music (" << BASS_GetError() << ")");
            mus->h1 = nullptr;
        }

        void force_play_cache() {
            if (cache.size() == 0)
                return;
            bool from_rep = false;
            if (cache[0] == cur_mus) {
                cache.erase(cache.begin());
                if (stopped || bass.BASS_ChannelIsActive(cur_h) == BASS_ACTIVE_STOPPED) {
                    cur_mus = nullptr;
                    force_play_cache();
                    pl::fill_cache();
                    return;
                }
                if (paused || bass.BASS_ChannelIsActive(cur_h) != BASS_ACTIVE_STOPPED) {
                    paused = false;
                    fading = false;
                    update_volume();
                    if (!bass.BASS_ChannelPlay(cur_h, 1))
                        TF_WARN(<< "Failed to play music from start (" << BASS_GetError() << ")");
                    pl::fill_cache();
                    return;
                }
                from_rep = true;
            }
            stopped = false;
            if (cur_mus && bass.BASS_ChannelIsActive(cur_h) != BASS_ACTIVE_STOPPED) {
                fading = true;
                stopped = false;
                paused = false;
                if (!bass.BASS_ChannelSlideAttribute(cur_h, BASS_ATTRIB_VOL | BASS_SLIDE_LOG, 0.f, (DWORD)(fade_next_time * 1000.f)))
                    TF_WARN(<< "Failed to set music next fade out (" << BASS_GetError() << ")");                
                return;
            }
            Music* prev = nullptr;
            if (!from_rep) {
                prev = cur_mus;
                cur_mus = cache[0];
                cache.erase(cache.begin());
            }
            cur_mus->cached = false;
            pl::mus_open_file(cur_mus);
            paused = false;
            if (bass.BASS_ChannelPlay(cur_h, TRUE)) {
                stopped = false;
                paused = false;
                fading = false;
                if (!bass.BASS_ChannelSetAttribute(cur_h, BASS_ATTRIB_VOL, volume))
                    TF_WARN(<< "Failed to set music volume (" << BASS_GetError() << ")");
            }
            else {
                TF_WARN(<< "Failed to play music (" << BASS_GetError() << ")");
                stopped = true;
                paused = false;
                fading = false;                
            }
            if (prev && prev != cur_mus && std::find(cache.begin(), cache.end(), prev) == cache.end())
                mus_close(prev);
            pl::fill_cache();
        }
    
        bool mus_fill_info(Music* mus) {
            QWORD dur = bass.BASS_ChannelGetLength(mus_h, BASS_POS_BYTE);
            if (dur < 0) {
                TF_WARN(<< "Failed to get music duration (" << BASS_GetError() << ")");
                dur = 0;
            }
            mus->dur = (float)bass.BASS_ChannelBytes2Seconds(mus_h, dur);
            if (mus->dur < 0.f || mus->dur > 86400000.f)
                mus->dur = 0.f;
            BASS_CHANNELINFO info;
            if (!bass.BASS_ChannelGetInfo(mus_h, &info)) {
                TF_ERROR(<< "Failed to get music info (" << BASS_GetError() << ")");
                return false;
            }
            if (info.ctype == BASS_CTYPE_STREAM_MP1)
                mus->type = Type::MP1;
            else if (info.ctype == BASS_CTYPE_STREAM_MP2)
                mus->type = Type::MP2;
            else if (info.ctype == BASS_CTYPE_STREAM_MP3)
                mus->type = Type::MP3;
            else if (info.ctype == BASS_CTYPE_STREAM_OGG)
                mus->type = Type::OGG;
            else if (info.ctype == BASS_CTYPE_STREAM_VORBIS)
                mus->type = Type::VORBIS;
            else if (info.ctype == BASS_CTYPE_STREAM_AIFF)
                mus->type = Type::AIFF;
            else if (info.ctype == BASS_CTYPE_STREAM_CA)
                mus->type = Type::AAC;
            else if (info.ctype == BASS_CTYPE_STREAM_MF)
                mus->type = Type::WMA;
            else if (info.ctype == BASS_CTYPE_STREAM_AM)
                mus->type = Type::NONE; // ?
            else if (info.ctype == BASS_CTYPE_STREAM_WAV || info.ctype == BASS_CTYPE_STREAM_WAV_PCM || info.ctype == BASS_CTYPE_STREAM_WAV_FLOAT)
                mus->type = Type::WAV;
            else if (info.ctype == BASS_CTYPE_MUSIC_MOD)
                mus->type = Type::MOD;
            else if (info.ctype == BASS_CTYPE_MUSIC_MTM)
                mus->type = Type::MTM;
            else if (info.ctype == BASS_CTYPE_MUSIC_XM)
                mus->type = Type::XM;
            else if (info.ctype == BASS_CTYPE_MUSIC_IT)
                mus->type = Type::IT;
            else if ((info.ctype & BASS_CTYPE_MUSIC_MO3) == BASS_CTYPE_MUSIC_MO3) // ?
                mus->type = Type::MO3;
            else
                mus->type = Type::NONE;
            return true;
        }

        void update() {
            if (cur_mus) {
                if (paused) {
                    if (!bass.BASS_ChannelIsSliding(cur_h, BASS_ATTRIB_VOL) && bass.BASS_ChannelIsActive(cur_h) != BASS_ACTIVE_PAUSED) {
                        if (bass.BASS_ChannelIsActive(cur_h) != BASS_ACTIVE_STOPPED && !bass.BASS_ChannelPause(cur_h))
                            TF_WARN(<< "Failed to pause music (" << BASS_GetError() << ")");
                    }
                }
                else if (fading && !bass.BASS_ChannelIsSliding(cur_h, BASS_ATTRIB_VOL)) {
                    if (!bass.BASS_ChannelStop(cur_h))
                        TF_WARN(<< "Failed to stop music (" << BASS_GetError() << ")");
                    fading = false;
                }
                if (!paused && bass.BASS_ChannelIsActive(cur_h) == BASS_ACTIVE_STOPPED) {
                    fading = false;
                    if (!stopped)
                        force_play_cache();
                    pre_open();
                }
            }
        }

        bool cur_stopped() {
            return !cur_mus || (bass.BASS_ChannelIsActive(cur_h) == BASS_ACTIVE_STOPPED);
        }

        void cur_stop() {
            if (!cur_mus)
                return;
            paused = false;
            stopped = true;
            fading = true;
            if (!bass.BASS_ChannelSlideAttribute(cur_h, BASS_ATTRIB_VOL | BASS_SLIDE_LOG, 0.f, (DWORD)(fade_stop_time * 1000.f)))
                TF_WARN(<< "Failed to set music stop fade out (" << BASS_GetError() << ")");
            pl::fill_cache();
        }

        float cur_get_pos() {
            if (cur_paused())
                return pause_pos;
            if (!cur_mus || stopped || bass.BASS_ChannelIsActive(cur_h) == BASS_ACTIVE_STOPPED)
                return 0.f;
            QWORD b_pos = bass.BASS_ChannelGetPosition(cur_h, BASS_POS_BYTE);
            if (b_pos < 0) {
                TF_WARN(<< "Failed to get music position (" << BASS_GetError() << ")");
                return 0.f;
            }
            // Actually shouldn't fail
            return (float)bass.BASS_ChannelBytes2Seconds(cur_h, b_pos);
        }

        void cur_set_pos(float pos) {
            if (!cur_mus || stopped || bass.BASS_ChannelIsActive(cur_h) == BASS_ACTIVE_STOPPED)
                return;
            pos = tf::clamp(pos, 0.f, cur_mus->dur);
            if (pos == cur_mus->dur) {
                if (!bass.BASS_ChannelStop(cur_h))
                    TF_WARN(<< "Failed to fast stop music (" << BASS_GetError() << ")");
                force_play_cache();
                return;
            }
            if (paused) {
                pause_pos = pos;
                return;
            }
            if (!bass.BASS_ChannelSetPosition(cur_h, bass.BASS_ChannelSeconds2Bytes(cur_h, (double)pos), BASS_POS_BYTE))
                TF_WARN(<< "Failed to set music position (" << BASS_GetError() << ")");
        }

        void cur_pause() {
            if (!cur_mus)
                return;
            pause_pos = cur_get_pos();
            paused = true;
            if (!bass.BASS_ChannelSlideAttribute(cur_h, BASS_ATTRIB_VOL | BASS_SLIDE_LOG, 0.f, (DWORD)(fade_pause_time * 1000.f)))
                TF_WARN(<< "Failed to set music pause fade out (" << BASS_GetError() << ")");
        }

        void cur_resume() {
            if (!cur_mus)
                return;
            paused = false;
            if (!bass.BASS_ChannelSetPosition(cur_h, bass.BASS_ChannelSeconds2Bytes(cur_h, (double)pause_pos), BASS_POS_BYTE))
                TF_WARN(<< "Failed to set music resume position (" << BASS_GetError() << ")");
            if (!bass.BASS_ChannelStart(cur_h))
                TF_WARN(<< "Failed to resume music (" << BASS_GetError() << ")");
            if (!bass.BASS_ChannelSlideAttribute(cur_h, BASS_ATTRIB_VOL | BASS_SLIDE_LOG, volume, (DWORD)(fade_resume_time * 1000.f)))
                TF_WARN(<< "Failed to set music resume fade in (" << BASS_GetError() << ")");
        }

        bool cur_paused() {
            return cur_mus && paused;
        }

        void update_volume() {
            volume = tf::clamp(volume, 0.f, 1.f);
            if (cur_mus && !bass.BASS_ChannelSetAttribute(cur_h, BASS_ATTRIB_VOL, volume))
                TF_WARN(<< "Failed to update music volume (" << BASS_GetError() << ")");
        }

        ~AudioBASS() {
            if (!inited)
                return;
            if (dev_opened)
                dev_close();
            inited = false;
            SDL_UnloadObject(bass.handle);
        }
    };
}

audio::AudioBase* audio::create_bass() {
    return (audio::AudioBase*)tf::nw<audio::AudioBASS>();
}
#endif
