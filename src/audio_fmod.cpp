#include <lbs.hpp>
#if ENABLE_FMOD
#include <audio_base.hpp>
#include <new.hpp>
#include <log.hpp>
#include <break.hpp>
#include <playlist.hpp>
#include <algorithm>
#include <SDL3/SDL.h>
#if 1
#define F_CALL SDLCALL

typedef int                        FMOD_BOOL;
typedef struct FMOD_SYSTEM         FMOD_SYSTEM;
typedef struct FMOD_SOUND          FMOD_SOUND;
typedef struct FMOD_CHANNELCONTROL FMOD_CHANNELCONTROL;
typedef struct FMOD_CHANNEL        FMOD_CHANNEL;
typedef struct FMOD_CHANNELGROUP   FMOD_CHANNELGROUP;
typedef struct FMOD_SOUNDGROUP     FMOD_SOUNDGROUP;
typedef struct FMOD_REVERB3D       FMOD_REVERB3D;
typedef struct FMOD_DSP            FMOD_DSP;
typedef struct FMOD_DSPCONNECTION  FMOD_DSPCONNECTION;
typedef struct FMOD_POLYGON        FMOD_POLYGON;
typedef struct FMOD_GEOMETRY       FMOD_GEOMETRY;
typedef struct FMOD_SYNCPOINT      FMOD_SYNCPOINT;
typedef struct FMOD_ASYNCREADINFO  FMOD_ASYNCREADINFO;

typedef unsigned int FMOD_DEBUG_FLAGS;
#define FMOD_DEBUG_LEVEL_NONE                       0x00000000
#define FMOD_DEBUG_LEVEL_ERROR                      0x00000001
#define FMOD_DEBUG_LEVEL_WARNING                    0x00000002
#define FMOD_DEBUG_LEVEL_LOG                        0x00000004
#define FMOD_DEBUG_TYPE_MEMORY                      0x00000100
#define FMOD_DEBUG_TYPE_FILE                        0x00000200
#define FMOD_DEBUG_TYPE_CODEC                       0x00000400
#define FMOD_DEBUG_TYPE_TRACE                       0x00000800
#define FMOD_DEBUG_DISPLAY_TIMESTAMPS               0x00010000
#define FMOD_DEBUG_DISPLAY_LINENUMBERS              0x00020000
#define FMOD_DEBUG_DISPLAY_THREAD                   0x00040000

typedef unsigned int FMOD_TIMEUNIT;
#define FMOD_TIMEUNIT_MS                            0x00000001
#define FMOD_TIMEUNIT_PCM                           0x00000002
#define FMOD_TIMEUNIT_PCMBYTES                      0x00000004
#define FMOD_TIMEUNIT_RAWBYTES                      0x00000008
#define FMOD_TIMEUNIT_PCMFRACTION                   0x00000010
#define FMOD_TIMEUNIT_MODORDER                      0x00000100
#define FMOD_TIMEUNIT_MODROW                        0x00000200
#define FMOD_TIMEUNIT_MODPATTERN                    0x00000400

typedef unsigned int FMOD_SYSTEM_CALLBACK_TYPE;
#define FMOD_SYSTEM_CALLBACK_DEVICELISTCHANGED      0x00000001
#define FMOD_SYSTEM_CALLBACK_DEVICELOST             0x00000002
#define FMOD_SYSTEM_CALLBACK_MEMORYALLOCATIONFAILED 0x00000004
#define FMOD_SYSTEM_CALLBACK_THREADCREATED          0x00000008
#define FMOD_SYSTEM_CALLBACK_BADDSPCONNECTION       0x00000010
#define FMOD_SYSTEM_CALLBACK_PREMIX                 0x00000020
#define FMOD_SYSTEM_CALLBACK_POSTMIX                0x00000040
#define FMOD_SYSTEM_CALLBACK_ERROR                  0x00000080
#define FMOD_SYSTEM_CALLBACK_MIDMIX                 0x00000100
#define FMOD_SYSTEM_CALLBACK_THREADDESTROYED        0x00000200
#define FMOD_SYSTEM_CALLBACK_PREUPDATE              0x00000400
#define FMOD_SYSTEM_CALLBACK_POSTUPDATE             0x00000800
#define FMOD_SYSTEM_CALLBACK_RECORDLISTCHANGED      0x00001000
#define FMOD_SYSTEM_CALLBACK_BUFFEREDNOMIX          0x00002000
#define FMOD_SYSTEM_CALLBACK_DEVICEREINITIALIZE     0x00004000
#define FMOD_SYSTEM_CALLBACK_OUTPUTUNDERRUN         0x00008000
#define FMOD_SYSTEM_CALLBACK_RECORDPOSITIONCHANGED  0x00010000
#define FMOD_SYSTEM_CALLBACK_ALL                    0xFFFFFFFF

typedef unsigned int FMOD_INITFLAGS;
#define FMOD_INIT_NORMAL                            0x00000000
#define FMOD_INIT_STREAM_FROM_UPDATE                0x00000001
#define FMOD_INIT_MIX_FROM_UPDATE                   0x00000002
#define FMOD_INIT_3D_RIGHTHANDED                    0x00000004
#define FMOD_INIT_CLIP_OUTPUT                       0x00000008
#define FMOD_INIT_CHANNEL_LOWPASS                   0x00000100
#define FMOD_INIT_CHANNEL_DISTANCEFILTER            0x00000200
#define FMOD_INIT_PROFILE_ENABLE                    0x00010000
#define FMOD_INIT_VOL0_BECOMES_VIRTUAL              0x00020000
#define FMOD_INIT_GEOMETRY_USECLOSEST               0x00040000
#define FMOD_INIT_PREFER_DOLBY_DOWNMIX              0x00080000
#define FMOD_INIT_THREAD_UNSAFE                     0x00100000
#define FMOD_INIT_PROFILE_METER_ALL                 0x00200000
#define FMOD_INIT_MEMORY_TRACKING                   0x00400000

typedef unsigned int FMOD_MODE;
#define FMOD_DEFAULT                                0x00000000
#define FMOD_LOOP_OFF                               0x00000001
#define FMOD_LOOP_NORMAL                            0x00000002
#define FMOD_LOOP_BIDI                              0x00000004
#define FMOD_2D                                     0x00000008
#define FMOD_3D                                     0x00000010
#define FMOD_CREATESTREAM                           0x00000080
#define FMOD_CREATESAMPLE                           0x00000100
#define FMOD_CREATECOMPRESSEDSAMPLE                 0x00000200
#define FMOD_OPENUSER                               0x00000400
#define FMOD_OPENMEMORY                             0x00000800
#define FMOD_OPENMEMORY_POINT                       0x10000000
#define FMOD_OPENRAW                                0x00001000
#define FMOD_OPENONLY                               0x00002000
#define FMOD_ACCURATETIME                           0x00004000
#define FMOD_MPEGSEARCH                             0x00008000
#define FMOD_NONBLOCKING                            0x00010000
#define FMOD_UNIQUE                                 0x00020000
#define FMOD_3D_HEADRELATIVE                        0x00040000
#define FMOD_3D_WORLDRELATIVE                       0x00080000
#define FMOD_3D_INVERSEROLLOFF                      0x00100000
#define FMOD_3D_LINEARROLLOFF                       0x00200000
#define FMOD_3D_LINEARSQUAREROLLOFF                 0x00400000
#define FMOD_3D_INVERSETAPEREDROLLOFF               0x00800000
#define FMOD_3D_CUSTOMROLLOFF                       0x04000000
#define FMOD_3D_IGNOREGEOMETRY                      0x40000000
#define FMOD_IGNORETAGS                             0x02000000
#define FMOD_LOWMEM                                 0x08000000
#define FMOD_VIRTUAL_PLAYFROMSTART                  0x80000000

typedef unsigned int FMOD_MEMORY_TYPE;
#define FMOD_MEMORY_NORMAL                          0x00000000
#define FMOD_MEMORY_STREAM_FILE                     0x00000001
#define FMOD_MEMORY_STREAM_DECODE                   0x00000002
#define FMOD_MEMORY_SAMPLEDATA                      0x00000004
#define FMOD_MEMORY_DSP_BUFFER                      0x00000008
#define FMOD_MEMORY_PLUGIN                          0x00000010
#define FMOD_MEMORY_PERSISTENT                      0x00200000
#define FMOD_MEMORY_ALL                             0xFFFFFFFF

typedef enum FMOD_SOUND_FORMAT {
    FMOD_SOUND_FORMAT_NONE,
    FMOD_SOUND_FORMAT_PCM8,
    FMOD_SOUND_FORMAT_PCM16,
    FMOD_SOUND_FORMAT_PCM24,
    FMOD_SOUND_FORMAT_PCM32,
    FMOD_SOUND_FORMAT_PCMFLOAT,
    FMOD_SOUND_FORMAT_BITSTREAM,
    FMOD_SOUND_FORMAT_MAX,
    FMOD_SOUND_FORMAT_FORCEINT = 65536
} FMOD_SOUND_FORMAT;

typedef enum FMOD_CHANNELCONTROL_TYPE {
    FMOD_CHANNELCONTROL_CHANNEL,
    FMOD_CHANNELCONTROL_CHANNELGROUP,
    FMOD_CHANNELCONTROL_MAX,
    FMOD_CHANNELCONTROL_FORCEINT = 65536
} FMOD_CHANNELCONTROL_TYPE;

typedef enum FMOD_CHANNELCONTROL_CALLBACK_TYPE {
    FMOD_CHANNELCONTROL_CALLBACK_END,
    FMOD_CHANNELCONTROL_CALLBACK_VIRTUALVOICE,
    FMOD_CHANNELCONTROL_CALLBACK_SYNCPOINT,
    FMOD_CHANNELCONTROL_CALLBACK_OCCLUSION,
    FMOD_CHANNELCONTROL_CALLBACK_MAX,
    FMOD_CHANNELCONTROL_CALLBACK_FORCEINT = 65536
} FMOD_CHANNELCONTROL_CALLBACK_TYPE;

typedef enum FMOD_OUTPUTTYPE {
    FMOD_OUTPUTTYPE_AUTODETECT,
    FMOD_OUTPUTTYPE_UNKNOWN,
    FMOD_OUTPUTTYPE_NOSOUND,
    FMOD_OUTPUTTYPE_WAVWRITER,
    FMOD_OUTPUTTYPE_NOSOUND_NRT,
    FMOD_OUTPUTTYPE_WAVWRITER_NRT,
    FMOD_OUTPUTTYPE_WASAPI,
    FMOD_OUTPUTTYPE_ASIO,
    FMOD_OUTPUTTYPE_PULSEAUDIO,
    FMOD_OUTPUTTYPE_ALSA,
    FMOD_OUTPUTTYPE_COREAUDIO,
    FMOD_OUTPUTTYPE_AUDIOTRACK,
    FMOD_OUTPUTTYPE_OPENSL,
    FMOD_OUTPUTTYPE_AUDIOOUT,
    FMOD_OUTPUTTYPE_AUDIO3D,
    FMOD_OUTPUTTYPE_WEBAUDIO,
    FMOD_OUTPUTTYPE_NNAUDIO,
    FMOD_OUTPUTTYPE_WINSONIC,
    FMOD_OUTPUTTYPE_AAUDIO,
    FMOD_OUTPUTTYPE_AUDIOWORKLET,
    FMOD_OUTPUTTYPE_PHASE,
    FMOD_OUTPUTTYPE_OHAUDIO,
    FMOD_OUTPUTTYPE_MAX,
    FMOD_OUTPUTTYPE_FORCEINT = 65536
} FMOD_OUTPUTTYPE;

typedef enum FMOD_SOUND_TYPE {
    FMOD_SOUND_TYPE_UNKNOWN,
    FMOD_SOUND_TYPE_AIFF,
    FMOD_SOUND_TYPE_ASF,
    FMOD_SOUND_TYPE_DLS,
    FMOD_SOUND_TYPE_FLAC,
    FMOD_SOUND_TYPE_FSB,
    FMOD_SOUND_TYPE_IT,
    FMOD_SOUND_TYPE_MIDI,
    FMOD_SOUND_TYPE_MOD,
    FMOD_SOUND_TYPE_MPEG,
    FMOD_SOUND_TYPE_OGGVORBIS,
    FMOD_SOUND_TYPE_PLAYLIST,
    FMOD_SOUND_TYPE_RAW,
    FMOD_SOUND_TYPE_S3M,
    FMOD_SOUND_TYPE_USER,
    FMOD_SOUND_TYPE_WAV,
    FMOD_SOUND_TYPE_XM,
    FMOD_SOUND_TYPE_XMA,
    FMOD_SOUND_TYPE_AUDIOQUEUE,
    FMOD_SOUND_TYPE_AT9,
    FMOD_SOUND_TYPE_VORBIS,
    FMOD_SOUND_TYPE_MEDIA_FOUNDATION,
    FMOD_SOUND_TYPE_MEDIACODEC,
    FMOD_SOUND_TYPE_FADPCM,
    FMOD_SOUND_TYPE_OPUS,

    FMOD_SOUND_TYPE_MAX,
    FMOD_SOUND_TYPE_FORCEINT = 65536
} FMOD_SOUND_TYPE;

typedef enum FMOD_CHANNELORDER {
    FMOD_CHANNELORDER_DEFAULT,
    FMOD_CHANNELORDER_WAVEFORMAT,
    FMOD_CHANNELORDER_PROTOOLS,
    FMOD_CHANNELORDER_ALLMONO,
    FMOD_CHANNELORDER_ALLSTEREO,
    FMOD_CHANNELORDER_ALSA,
    FMOD_CHANNELORDER_MAX,
    FMOD_CHANNELORDER_FORCEINT = 65536
} FMOD_CHANNELORDER;

typedef enum FMOD_RESULT {
    FMOD_OK,
    FMOD_ERR_BADCOMMAND,
    FMOD_ERR_CHANNEL_ALLOC,
    FMOD_ERR_CHANNEL_STOLEN,
    FMOD_ERR_DMA,
    FMOD_ERR_DSP_CONNECTION,
    FMOD_ERR_DSP_DONTPROCESS,
    FMOD_ERR_DSP_FORMAT,
    FMOD_ERR_DSP_INUSE,
    FMOD_ERR_DSP_NOTFOUND,
    FMOD_ERR_DSP_RESERVED,
    FMOD_ERR_DSP_SILENCE,
    FMOD_ERR_DSP_TYPE,
    FMOD_ERR_FILE_BAD,
    FMOD_ERR_FILE_COULDNOTSEEK,
    FMOD_ERR_FILE_DISKEJECTED,
    FMOD_ERR_FILE_EOF,
    FMOD_ERR_FILE_ENDOFDATA,
    FMOD_ERR_FILE_NOTFOUND,
    FMOD_ERR_FORMAT,
    FMOD_ERR_HEADER_MISMATCH,
    FMOD_ERR_HTTP,
    FMOD_ERR_HTTP_ACCESS,
    FMOD_ERR_HTTP_PROXY_AUTH,
    FMOD_ERR_HTTP_SERVER_ERROR,
    FMOD_ERR_HTTP_TIMEOUT,
    FMOD_ERR_INITIALIZATION,
    FMOD_ERR_INITIALIZED,
    FMOD_ERR_INTERNAL,
    FMOD_ERR_INVALID_FLOAT,
    FMOD_ERR_INVALID_HANDLE,
    FMOD_ERR_INVALID_PARAM,
    FMOD_ERR_INVALID_POSITION,
    FMOD_ERR_INVALID_SPEAKER,
    FMOD_ERR_INVALID_SYNCPOINT,
    FMOD_ERR_INVALID_THREAD,
    FMOD_ERR_INVALID_VECTOR,
    FMOD_ERR_MAXAUDIBLE,
    FMOD_ERR_MEMORY,
    FMOD_ERR_MEMORY_CANTPOINT,
    FMOD_ERR_NEEDS3D,
    FMOD_ERR_NEEDSHARDWARE,
    FMOD_ERR_NET_CONNECT,
    FMOD_ERR_NET_SOCKET_ERROR,
    FMOD_ERR_NET_URL,
    FMOD_ERR_NET_WOULD_BLOCK,
    FMOD_ERR_NOTREADY,
    FMOD_ERR_OUTPUT_ALLOCATED,
    FMOD_ERR_OUTPUT_CREATEBUFFER,
    FMOD_ERR_OUTPUT_DRIVERCALL,
    FMOD_ERR_OUTPUT_FORMAT,
    FMOD_ERR_OUTPUT_INIT,
    FMOD_ERR_OUTPUT_NODRIVERS,
    FMOD_ERR_PLUGIN,
    FMOD_ERR_PLUGIN_MISSING,
    FMOD_ERR_PLUGIN_RESOURCE,
    FMOD_ERR_PLUGIN_VERSION,
    FMOD_ERR_RECORD,
    FMOD_ERR_REVERB_CHANNELGROUP,
    FMOD_ERR_REVERB_INSTANCE,
    FMOD_ERR_SUBSOUNDS,
    FMOD_ERR_SUBSOUND_ALLOCATED,
    FMOD_ERR_SUBSOUND_CANTMOVE,
    FMOD_ERR_TAGNOTFOUND,
    FMOD_ERR_TOOMANYCHANNELS,
    FMOD_ERR_TRUNCATED,
    FMOD_ERR_UNIMPLEMENTED,
    FMOD_ERR_UNINITIALIZED,
    FMOD_ERR_UNSUPPORTED,
    FMOD_ERR_VERSION,
    FMOD_ERR_EVENT_ALREADY_LOADED,
    FMOD_ERR_EVENT_LIVEUPDATE_BUSY,
    FMOD_ERR_EVENT_LIVEUPDATE_MISMATCH,
    FMOD_ERR_EVENT_LIVEUPDATE_TIMEOUT,
    FMOD_ERR_EVENT_NOTFOUND,
    FMOD_ERR_STUDIO_UNINITIALIZED,
    FMOD_ERR_STUDIO_NOT_LOADED,
    FMOD_ERR_INVALID_STRING,
    FMOD_ERR_ALREADY_LOCKED,
    FMOD_ERR_NOT_LOCKED,
    FMOD_ERR_RECORD_DISCONNECTED,
    FMOD_ERR_TOOMANYSAMPLES,

    FMOD_RESULT_FORCEINT = 65536
} FMOD_RESULT;

extern "C" {
    typedef FMOD_RESULT(F_CALL* FMOD_DEBUG_CALLBACK)           (FMOD_DEBUG_FLAGS, const char*, int, const char*, const char*);
    typedef FMOD_RESULT(F_CALL* FMOD_SYSTEM_CALLBACK)          (FMOD_SYSTEM*, FMOD_SYSTEM_CALLBACK_TYPE, void*, void*, void*);
    typedef FMOD_RESULT(F_CALL* FMOD_CHANNELCONTROL_CALLBACK)  (FMOD_CHANNELCONTROL*, FMOD_CHANNELCONTROL_TYPE, FMOD_CHANNELCONTROL_CALLBACK_TYPE, void*, void*);
    typedef FMOD_RESULT(F_CALL* FMOD_SOUND_NONBLOCK_CALLBACK)  (FMOD_SOUND*, FMOD_RESULT);
    typedef FMOD_RESULT(F_CALL* FMOD_SOUND_PCMREAD_CALLBACK)   (FMOD_SOUND*, void*, unsigned int);
    typedef FMOD_RESULT(F_CALL* FMOD_SOUND_PCMSETPOS_CALLBACK) (FMOD_SOUND*, int, unsigned int, FMOD_TIMEUNIT);
    typedef FMOD_RESULT(F_CALL* FMOD_FILE_OPEN_CALLBACK)       (const char*, unsigned int*, void**, void*);
    typedef FMOD_RESULT(F_CALL* FMOD_FILE_CLOSE_CALLBACK)      (void*, void*);
    typedef FMOD_RESULT(F_CALL* FMOD_FILE_READ_CALLBACK)       (void*, void*, unsigned int, unsigned int*, void*);
    typedef FMOD_RESULT(F_CALL* FMOD_FILE_SEEK_CALLBACK)       (void*, unsigned int, void*);
    typedef FMOD_RESULT(F_CALL* FMOD_FILE_ASYNCREAD_CALLBACK)  (FMOD_ASYNCREADINFO*, void*);
    typedef FMOD_RESULT(F_CALL* FMOD_FILE_ASYNCCANCEL_CALLBACK)(FMOD_ASYNCREADINFO*, void*);
    typedef void        (F_CALL* FMOD_FILE_ASYNCDONE_FUNC)      (FMOD_ASYNCREADINFO*, FMOD_RESULT);
    typedef void* (F_CALL* FMOD_MEMORY_ALLOC_CALLBACK)    (unsigned int, FMOD_MEMORY_TYPE, const char*);
    typedef void* (F_CALL* FMOD_MEMORY_REALLOC_CALLBACK)  (void*, unsigned int, FMOD_MEMORY_TYPE, const char*);
    typedef void        (F_CALL* FMOD_MEMORY_FREE_CALLBACK)     (void*, FMOD_MEMORY_TYPE, const char*);
    typedef float       (F_CALL* FMOD_3D_ROLLOFF_CALLBACK)      (FMOD_CHANNELCONTROL*, float);
}

typedef struct FMOD_GUID {
    unsigned int   Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} FMOD_GUID;

typedef struct FMOD_CREATESOUNDEXINFO {
    int                            cbsize;
    unsigned int                   length;
    unsigned int                   fileoffset;
    int                            numchannels;
    int                            defaultfrequency;
    FMOD_SOUND_FORMAT              format;
    unsigned int                   decodebuffersize;
    int                            initialsubsound;
    int                            numsubsounds;
    int* inclusionlist;
    int                            inclusionlistnum;
    FMOD_SOUND_PCMREAD_CALLBACK    pcmreadcallback;
    FMOD_SOUND_PCMSETPOS_CALLBACK  pcmsetposcallback;
    FMOD_SOUND_NONBLOCK_CALLBACK   nonblockcallback;
    const char* dlsname;
    const char* encryptionkey;
    int                            maxpolyphony;
    void* userdata;
    FMOD_SOUND_TYPE                suggestedsoundtype;
    FMOD_FILE_OPEN_CALLBACK        fileuseropen;
    FMOD_FILE_CLOSE_CALLBACK       fileuserclose;
    FMOD_FILE_READ_CALLBACK        fileuserread;
    FMOD_FILE_SEEK_CALLBACK        fileuserseek;
    FMOD_FILE_ASYNCREAD_CALLBACK   fileuserasyncread;
    FMOD_FILE_ASYNCCANCEL_CALLBACK fileuserasynccancel;
    void* fileuserdata;
    int                            filebuffersize;
    FMOD_CHANNELORDER              channelorder;
    FMOD_SOUNDGROUP* initialsoundgroup;
    unsigned int                   initialseekposition;
    FMOD_TIMEUNIT                  initialseekpostype;
    int                            ignoresetfilesystem;
    unsigned int                   audioqueuepolicy;
    unsigned int                   minmidigranularity;
    int                            nonblockthreadid;
    FMOD_GUID* fsbguid;
} FMOD_CREATESOUNDEXINFO;

static const char* FMOD_ErrorString(FMOD_RESULT errcode) {
    switch (errcode) {
    case FMOD_OK:                            return "No errors.";
    case FMOD_ERR_BADCOMMAND:                return "Tried to call a function on a data type that does not allow this type of functionality (ie calling Sound::lock on a streaming sound).";
    case FMOD_ERR_CHANNEL_ALLOC:             return "Error trying to allocate a channel.";
    case FMOD_ERR_CHANNEL_STOLEN:            return "The specified channel has been reused to play another sound.";
    case FMOD_ERR_DMA:                       return "DMA Failure.  See debug output for more information.";
    case FMOD_ERR_DSP_CONNECTION:            return "DSP connection error.  Connection possibly caused a cyclic dependency or connected dsps with incompatible buffer counts.";
    case FMOD_ERR_DSP_DONTPROCESS:           return "DSP return code from a DSP process query callback.  Tells mixer not to call the process callback and therefore not consume CPU.  Use this to optimize the DSP graph.";
    case FMOD_ERR_DSP_FORMAT:                return "DSP Format error.  A DSP unit may have attempted to connect to this network with the wrong format, or a matrix may have been set with the wrong size if the target unit has a specified channel map.";
    case FMOD_ERR_DSP_INUSE:                 return "DSP is already in the mixer's DSP network. It must be removed before being reinserted or released.";
    case FMOD_ERR_DSP_NOTFOUND:              return "DSP connection error.  Couldn't find the DSP unit specified.";
    case FMOD_ERR_DSP_RESERVED:              return "DSP operation error.  Cannot perform operation on this DSP as it is reserved by the system.";
    case FMOD_ERR_DSP_SILENCE:               return "DSP return code from a DSP process query callback.  Tells mixer silence would be produced from read, so go idle and not consume CPU.  Use this to optimize the DSP graph.";
    case FMOD_ERR_DSP_TYPE:                  return "DSP operation cannot be performed on a DSP of this type.";
    case FMOD_ERR_FILE_BAD:                  return "Error loading file.";
    case FMOD_ERR_FILE_COULDNOTSEEK:         return "Couldn't perform seek operation.  This is a limitation of the medium (ie netstreams) or the file format.";
    case FMOD_ERR_FILE_DISKEJECTED:          return "Media was ejected while reading.";
    case FMOD_ERR_FILE_EOF:                  return "End of file unexpectedly reached while trying to read essential data (truncated?).";
    case FMOD_ERR_FILE_ENDOFDATA:            return "End of current chunk reached while trying to read data.";
    case FMOD_ERR_FILE_NOTFOUND:             return "File not found.";
    case FMOD_ERR_FORMAT:                    return "Unsupported file or audio format.";
    case FMOD_ERR_HEADER_MISMATCH:           return "There is a version mismatch between the FMOD header and either the FMOD Studio library or the FMOD Low Level library.";
    case FMOD_ERR_HTTP:                      return "A HTTP error occurred. This is a catch-all for HTTP errors not listed elsewhere.";
    case FMOD_ERR_HTTP_ACCESS:               return "The specified resource requires authentication or is forbidden.";
    case FMOD_ERR_HTTP_PROXY_AUTH:           return "Proxy authentication is required to access the specified resource.";
    case FMOD_ERR_HTTP_SERVER_ERROR:         return "A HTTP server error occurred.";
    case FMOD_ERR_HTTP_TIMEOUT:              return "The HTTP request timed out.";
    case FMOD_ERR_INITIALIZATION:            return "FMOD was not initialized correctly to support this function.";
    case FMOD_ERR_INITIALIZED:               return "Cannot call this command after System::init.";
    case FMOD_ERR_INTERNAL:                  return "An error occurred that wasn't supposed to.  Contact support.";
    case FMOD_ERR_INVALID_FLOAT:             return "Value passed in was a NaN, Inf or denormalized float.";
    case FMOD_ERR_INVALID_HANDLE:            return "An invalid object handle was used.";
    case FMOD_ERR_INVALID_PARAM:             return "An invalid parameter was passed to this function.";
    case FMOD_ERR_INVALID_POSITION:          return "An invalid seek position was passed to this function.";
    case FMOD_ERR_INVALID_SPEAKER:           return "An invalid speaker was passed to this function based on the current speaker mode.";
    case FMOD_ERR_INVALID_SYNCPOINT:         return "The syncpoint did not come from this sound handle.";
    case FMOD_ERR_INVALID_THREAD:            return "Tried to call a function on a thread that is not supported.";
    case FMOD_ERR_INVALID_VECTOR:            return "The vectors passed in are not unit length, or perpendicular.";
    case FMOD_ERR_MAXAUDIBLE:                return "Reached maximum audible playback count for this sound's soundgroup.";
    case FMOD_ERR_MEMORY:                    return "Not enough memory or resources.";
    case FMOD_ERR_MEMORY_CANTPOINT:          return "Can't use FMOD_OPENMEMORY_POINT on non PCM source data, or non mp3/xma/adpcm data if FMOD_CREATECOMPRESSEDSAMPLE was used.";
    case FMOD_ERR_NEEDS3D:                   return "Tried to call a command on a 2d sound when the command was meant for 3d sound.";
    case FMOD_ERR_NEEDSHARDWARE:             return "Tried to use a feature that requires hardware support.";
    case FMOD_ERR_NET_CONNECT:               return "Couldn't connect to the specified host.";
    case FMOD_ERR_NET_SOCKET_ERROR:          return "A socket error occurred.  This is a catch-all for socket-related errors not listed elsewhere.";
    case FMOD_ERR_NET_URL:                   return "The specified URL couldn't be resolved.";
    case FMOD_ERR_NET_WOULD_BLOCK:           return "Operation on a non-blocking socket could not complete immediately.";
    case FMOD_ERR_NOTREADY:                  return "Operation could not be performed because specified sound/DSP connection is not ready.";
    case FMOD_ERR_OUTPUT_ALLOCATED:          return "Error initializing output device, but more specifically, the output device is already in use and cannot be reused.";
    case FMOD_ERR_OUTPUT_CREATEBUFFER:       return "Error creating hardware sound buffer.";
    case FMOD_ERR_OUTPUT_DRIVERCALL:         return "A call to a standard soundcard driver failed, which could possibly mean a bug in the driver or resources were missing or exhausted.";
    case FMOD_ERR_OUTPUT_FORMAT:             return "Soundcard does not support the specified format.";
    case FMOD_ERR_OUTPUT_INIT:               return "Error initializing output device.";
    case FMOD_ERR_OUTPUT_NODRIVERS:          return "The output device has no drivers installed.  If pre-init, FMOD_OUTPUT_NOSOUND is selected as the output mode.  If post-init, the function just fails.";
    case FMOD_ERR_PLUGIN:                    return "An unspecified error has been returned from a plugin.";
    case FMOD_ERR_PLUGIN_MISSING:            return "A requested output, dsp unit type or codec was not available.";
    case FMOD_ERR_PLUGIN_RESOURCE:           return "A resource that the plugin requires cannot be allocated or found. (ie the DLS file for MIDI playback)";
    case FMOD_ERR_PLUGIN_VERSION:            return "A plugin was built with an unsupported SDK version.";
    case FMOD_ERR_RECORD:                    return "An error occurred trying to initialize the recording device.";
    case FMOD_ERR_REVERB_CHANNELGROUP:       return "Reverb properties cannot be set on this channel because a parent channelgroup owns the reverb connection.";
    case FMOD_ERR_REVERB_INSTANCE:           return "Specified instance in FMOD_REVERB_PROPERTIES couldn't be set. Most likely because it is an invalid instance number or the reverb doesn't exist.";
    case FMOD_ERR_SUBSOUNDS:                 return "The error occurred because the sound referenced contains subsounds when it shouldn't have, or it doesn't contain subsounds when it should have.  The operation may also not be able to be performed on a parent sound.";
    case FMOD_ERR_SUBSOUND_ALLOCATED:        return "This subsound is already being used by another sound, you cannot have more than one parent to a sound.  Null out the other parent's entry first.";
    case FMOD_ERR_SUBSOUND_CANTMOVE:         return "Shared subsounds cannot be replaced or moved from their parent stream, such as when the parent stream is an FSB file.";
    case FMOD_ERR_TAGNOTFOUND:               return "The specified tag could not be found or there are no tags.";
    case FMOD_ERR_TOOMANYCHANNELS:           return "The sound created exceeds the allowable input channel count.  This can be increased using the 'maxinputchannels' parameter in System::setSoftwareFormat.";
    case FMOD_ERR_TRUNCATED:                 return "The retrieved string is too long to fit in the supplied buffer and has been truncated.";
    case FMOD_ERR_UNIMPLEMENTED:             return "Something in FMOD hasn't been implemented when it should be! contact support!";
    case FMOD_ERR_UNINITIALIZED:             return "This command failed because System::init or System::setDriver was not called.";
    case FMOD_ERR_UNSUPPORTED:               return "A command issued was not supported by this object.  Possibly a plugin without certain callbacks specified.";
    case FMOD_ERR_VERSION:                   return "The version number of this file format is not supported.";
    case FMOD_ERR_EVENT_ALREADY_LOADED:      return "The specified bank has already been loaded.";
    case FMOD_ERR_EVENT_LIVEUPDATE_BUSY:     return "The live update connection failed due to the game already being connected.";
    case FMOD_ERR_EVENT_LIVEUPDATE_MISMATCH: return "The live update connection failed due to the game data being out of sync with the tool.";
    case FMOD_ERR_EVENT_LIVEUPDATE_TIMEOUT:  return "The live update connection timed out.";
    case FMOD_ERR_EVENT_NOTFOUND:            return "The requested event, parameter, bus or vca could not be found.";
    case FMOD_ERR_STUDIO_UNINITIALIZED:      return "The Studio::System object is not yet initialized.";
    case FMOD_ERR_STUDIO_NOT_LOADED:         return "The specified resource is not loaded, so it can't be unloaded.";
    case FMOD_ERR_INVALID_STRING:            return "An invalid string was passed to this function.";
    case FMOD_ERR_ALREADY_LOCKED:            return "The specified resource is already locked.";
    case FMOD_ERR_NOT_LOCKED:                return "The specified resource is not locked, so it can't be unlocked.";
    case FMOD_ERR_RECORD_DISCONNECTED:       return "The specified recording driver has been disconnected.";
    case FMOD_ERR_TOOMANYSAMPLES:            return "The length provided exceeds the allowable limit.";
    default:                                return "Unknown error.";
    };
}

#define FMOD_LOAD_FUNC(func_name) do { \
    *(void**)&fmod.func_name = (void*)SDL_LoadFunction(fmod.handle, #func_name); \
    if (!fmod.func_name) { \
        TF_ERROR(<< "Failed to load " << lib_name << " function \"" << #func_name << "\" (" << SDL_GetError() << ")"); \
        SDL_UnloadObject(fmod.handle); \
        return; \
    } \
} while (0)
#endif
#define FMOD_HAS_ERROR(res) ((res) != FMOD_OK)
#define mus_h ((FMOD_SOUND*)mus->h1)
#define cur_h ((FMOD_SOUND*)cur_mus->h1)

FMOD_RESULT F_CALL fmod_channel_callback(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2);

void* SDLCALL FMOD_malloc(unsigned int size, FMOD_MEMORY_TYPE type, const char* sourcestr) {
    (void)type;
    (void)sourcestr;
	void* result = SDL_malloc((size_t)size);
	return result;
}

void* SDLCALL FMOD_realloc(void* ptr, unsigned int size, FMOD_MEMORY_TYPE type, const char* sourcestr) {
    (void)type;
    (void)sourcestr;
	void* result = SDL_realloc(ptr, (size_t)size);
	return result;
}

void SDLCALL FMOD_free(void* ptr, FMOD_MEMORY_TYPE type, const char* sourcestr) {
    (void)type;
    (void)sourcestr;
	SDL_free(ptr);
}

namespace audio {
    static inline const char* output_type_to_str(FMOD_OUTPUTTYPE type) {
        if (type == FMOD_OUTPUTTYPE_UNKNOWN)
            return "unknown";
        else if (type == FMOD_OUTPUTTYPE_NOSOUND)
            return "nosound";
        else if (type == FMOD_OUTPUTTYPE_WAVWRITER)
            return "wavwriter";
        else if (type == FMOD_OUTPUTTYPE_NOSOUND_NRT)
            return "nosound_nrt";
        else if (type == FMOD_OUTPUTTYPE_WAVWRITER_NRT)
            return "wavwriter_nrt";
        else if (type == FMOD_OUTPUTTYPE_WASAPI)
            return "wasapi";
        else if (type == FMOD_OUTPUTTYPE_ASIO)
            return "asio";
        else if (type == FMOD_OUTPUTTYPE_PULSEAUDIO)
            return "pulseaudio";
        else if (type == FMOD_OUTPUTTYPE_ALSA)
            return "alsa";
        else if (type == FMOD_OUTPUTTYPE_COREAUDIO)
            return "coreaudio";
        else if (type == FMOD_OUTPUTTYPE_AUDIOTRACK)
            return "audiotrack";
        else if (type == FMOD_OUTPUTTYPE_OPENSL)
            return "opensl";
        else if (type == FMOD_OUTPUTTYPE_AUDIOOUT)
            return "audioout";
        else if (type == FMOD_OUTPUTTYPE_AUDIO3D)
            return "audio3d";
        else if (type == FMOD_OUTPUTTYPE_WEBAUDIO)
            return "webaudio";
        else if (type == FMOD_OUTPUTTYPE_NNAUDIO)
            return "nnaudio";
        else if (type == FMOD_OUTPUTTYPE_WINSONIC)
            return "winsonic";
        else if (type == FMOD_OUTPUTTYPE_AAUDIO)
            return "aaudio";
        else if (type == FMOD_OUTPUTTYPE_AUDIOWORKLET)
            return "audioworklet";
        else if (type == FMOD_OUTPUTTYPE_PHASE)
            return "phase";
        else if (type == FMOD_OUTPUTTYPE_OHAUDIO)
            return "ohaudio";
        return "nosound";
    }

    struct FMODApi {
        SDL_SharedObject* handle;
        FMOD_RESULT (F_CALL *FMOD_System_Create)(FMOD_SYSTEM**, unsigned int);
        FMOD_RESULT (F_CALL *FMOD_System_Release)(FMOD_SYSTEM*);
        FMOD_RESULT (F_CALL *FMOD_System_Init)(FMOD_SYSTEM*, int, FMOD_INITFLAGS, void*);
        FMOD_RESULT (F_CALL *FMOD_System_Close)(FMOD_SYSTEM*);
        FMOD_RESULT (F_CALL *FMOD_System_Update)(FMOD_SYSTEM*);
        FMOD_RESULT (F_CALL *FMOD_System_GetVersion)(FMOD_SYSTEM*, unsigned int*);
        FMOD_RESULT (F_CALL *FMOD_System_CreateStream)(FMOD_SYSTEM*, const char*, FMOD_MODE, FMOD_CREATESOUNDEXINFO*, FMOD_SOUND**);
        FMOD_RESULT (F_CALL *FMOD_System_PlaySound)(FMOD_SYSTEM*, FMOD_SOUND*, FMOD_CHANNELGROUP*, FMOD_BOOL, FMOD_CHANNEL**);
        FMOD_RESULT (F_CALL *FMOD_System_GetChannel)(FMOD_SYSTEM*, int, FMOD_CHANNEL**);
        FMOD_RESULT (F_CALL *FMOD_System_GetOutput)(FMOD_SYSTEM*, FMOD_OUTPUTTYPE*);
        FMOD_RESULT (F_CALL *FMOD_Sound_Release)(FMOD_SOUND*);
        FMOD_RESULT (F_CALL *FMOD_Sound_GetLength)(FMOD_SOUND*, unsigned int*, FMOD_TIMEUNIT);
        FMOD_RESULT (F_CALL *FMOD_Sound_GetFormat)(FMOD_SOUND*, FMOD_SOUND_TYPE*, FMOD_SOUND_FORMAT*, int*, int*);
        FMOD_RESULT (F_CALL *FMOD_Sound_SetMode)(FMOD_SOUND*, FMOD_MODE);
        FMOD_RESULT (F_CALL *FMOD_Sound_GetMode)(FMOD_SOUND*, FMOD_MODE*);
        FMOD_RESULT (F_CALL *FMOD_Sound_SetLoopCount)(FMOD_SOUND*, int);
        FMOD_RESULT (F_CALL *FMOD_Sound_GetLoopCount)(FMOD_SOUND*, int*);
        FMOD_RESULT (F_CALL *FMOD_Channel_Stop)(FMOD_CHANNEL*);
        FMOD_RESULT (F_CALL *FMOD_Channel_SetPaused)(FMOD_CHANNEL*, FMOD_BOOL);
        FMOD_RESULT (F_CALL *FMOD_Channel_GetPaused)(FMOD_CHANNEL*, FMOD_BOOL*);
        FMOD_RESULT (F_CALL *FMOD_Channel_SetVolume)(FMOD_CHANNEL*, float);
        FMOD_RESULT (F_CALL *FMOD_Channel_GetVolume)(FMOD_CHANNEL*, float*);
        FMOD_RESULT (F_CALL *FMOD_Channel_SetPitch)(FMOD_CHANNEL*, float);
        FMOD_RESULT (F_CALL *FMOD_Channel_GetPitch)(FMOD_CHANNEL*, float*);
        FMOD_RESULT (F_CALL *FMOD_Channel_SetMode)(FMOD_CHANNEL*, FMOD_MODE);
        FMOD_RESULT (F_CALL *FMOD_Channel_GetMode)(FMOD_CHANNEL*, FMOD_MODE*);
        FMOD_RESULT (F_CALL *FMOD_Channel_SetCallback)(FMOD_CHANNEL*, FMOD_CHANNELCONTROL_CALLBACK);
        FMOD_RESULT (F_CALL *FMOD_Channel_SetUserData)(FMOD_CHANNEL*, void*);
        FMOD_RESULT (F_CALL *FMOD_Channel_GetUserData)(FMOD_CHANNEL*, void**);
        FMOD_RESULT (F_CALL *FMOD_Channel_GetCurrentSound)(FMOD_CHANNEL*, FMOD_SOUND**);
        FMOD_RESULT (F_CALL *FMOD_Channel_SetPosition)(FMOD_CHANNEL*, unsigned int, FMOD_TIMEUNIT);
        FMOD_RESULT (F_CALL *FMOD_Channel_GetPosition)(FMOD_CHANNEL*, unsigned int*, FMOD_TIMEUNIT);
        FMOD_RESULT (F_CALL *FMOD_Channel_SetDelay)(FMOD_CHANNEL*, unsigned long long, unsigned long long, FMOD_BOOL);
        FMOD_RESULT (F_CALL *FMOD_Channel_GetDSPClock)(FMOD_CHANNEL*, unsigned long long*, unsigned long long*);
        FMOD_RESULT (F_CALL *FMOD_Channel_AddFadePoint)(FMOD_CHANNEL*, unsigned long long, float);
        FMOD_RESULT (F_CALL *FMOD_Channel_RemoveFadePoints)(FMOD_CHANNEL*, unsigned long long, unsigned long long);
        FMOD_RESULT (F_CALL *FMOD_Memory_Initialize)(void*, int, FMOD_MEMORY_ALLOC_CALLBACK, FMOD_MEMORY_REALLOC_CALLBACK, FMOD_MEMORY_FREE_CALLBACK, FMOD_MEMORY_TYPE);
    };

    class AudioFMOD : public AudioBase {
        protected:
        public:
        FMOD_CHANNEL* ch;
	    FMOD_SYSTEM* sys;
        FMODApi fmod;
        float sps;
        float pause_pos;
        unsigned int fmod_ver;
        bool was_finished;
        bool stopped;
        bool fading;
        AudioFMOD() : AudioBase() {
            lib_name = "FMOD";
            ch = nullptr;
            sps = 44100.f;
            stopped = was_finished = fading = false;
            // max_volume = 2.f;
            pause_pos = 0.f;
            const char* lib_path = IS_WIN ? "fmod.dll" : "libfmod.so";
            fmod.handle = SDL_LoadObject(lib_path);
            if (!fmod.handle) {
                TF_WARN(<< "Failed to load FMOD library (" << SDL_GetError() << ")");
                return;
            }
            FMOD_LOAD_FUNC(FMOD_System_Create);
            FMOD_LOAD_FUNC(FMOD_System_Release);
            FMOD_LOAD_FUNC(FMOD_System_Init);
            FMOD_LOAD_FUNC(FMOD_System_Close);
            FMOD_LOAD_FUNC(FMOD_System_Update);
            FMOD_LOAD_FUNC(FMOD_System_GetVersion);
            FMOD_LOAD_FUNC(FMOD_System_CreateStream);
            FMOD_LOAD_FUNC(FMOD_System_PlaySound);
            FMOD_LOAD_FUNC(FMOD_System_GetChannel);
            FMOD_LOAD_FUNC(FMOD_System_GetOutput);
            FMOD_LOAD_FUNC(FMOD_Sound_Release);
            FMOD_LOAD_FUNC(FMOD_Sound_GetLength);
            FMOD_LOAD_FUNC(FMOD_Sound_GetFormat);
            FMOD_LOAD_FUNC(FMOD_Sound_SetMode);
            FMOD_LOAD_FUNC(FMOD_Sound_GetMode);
            FMOD_LOAD_FUNC(FMOD_Sound_SetLoopCount);
            FMOD_LOAD_FUNC(FMOD_Sound_GetLoopCount);
            FMOD_LOAD_FUNC(FMOD_Channel_Stop);
            FMOD_LOAD_FUNC(FMOD_Channel_SetPaused);
            FMOD_LOAD_FUNC(FMOD_Channel_GetPaused);
            FMOD_LOAD_FUNC(FMOD_Channel_SetVolume);
            FMOD_LOAD_FUNC(FMOD_Channel_GetVolume);
            FMOD_LOAD_FUNC(FMOD_Channel_SetPitch);
            FMOD_LOAD_FUNC(FMOD_Channel_GetPitch);
            FMOD_LOAD_FUNC(FMOD_Channel_SetMode);
            FMOD_LOAD_FUNC(FMOD_Channel_GetMode);
            FMOD_LOAD_FUNC(FMOD_Channel_SetCallback);
            FMOD_LOAD_FUNC(FMOD_Channel_SetUserData);
            FMOD_LOAD_FUNC(FMOD_Channel_GetUserData);
            FMOD_LOAD_FUNC(FMOD_Channel_GetCurrentSound);
            FMOD_LOAD_FUNC(FMOD_Channel_SetPosition);
            FMOD_LOAD_FUNC(FMOD_Channel_GetPosition);
            FMOD_LOAD_FUNC(FMOD_Channel_SetDelay);
            FMOD_LOAD_FUNC(FMOD_Channel_GetDSPClock);
            FMOD_LOAD_FUNC(FMOD_Channel_AddFadePoint);
            FMOD_LOAD_FUNC(FMOD_Channel_RemoveFadePoints);
            FMOD_LOAD_FUNC(FMOD_Memory_Initialize);
            FMOD_RESULT err;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_Memory_Initialize(
                NULL, 0, FMOD_malloc, FMOD_realloc, FMOD_free, FMOD_MEMORY_ALL
            ))) {
                TF_ERROR(<< "Failed set FMOD memory callbacks (" << FMOD_ErrorString(err) << ")");
                SDL_UnloadObject(fmod.handle);
                return;
            }
            // TODO: brute force version/read from config
            if (FMOD_HAS_ERROR(err = fmod.FMOD_System_Create(&sys, 0x00020308))) {
                TF_ERROR(<< "Failed to create FMOD system (" << FMOD_ErrorString(err) << ")");
                SDL_UnloadObject(fmod.handle);
                return;
            }
            FMOD_OUTPUTTYPE out_type;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_System_GetOutput(sys, &out_type))) {
                TF_WARN(<< "Failed to get FMOD output type (" << FMOD_ErrorString(err) << ")");
                out_type = FMOD_OUTPUTTYPE_NOSOUND;
            }
            TF_INFO(<< "FMOD inited successfully with " << output_type_to_str(out_type) << " driver");
            inited = true;
        }

        bool dev_open() {
            FMOD_RESULT err;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_System_Init(sys, 2, FMOD_INIT_NORMAL, NULL))) {
                TF_ERROR(<< "Failed to create FMOD system (" << FMOD_ErrorString(err) << ")");
                return true;
            }
            if (FMOD_HAS_ERROR(err = fmod.FMOD_System_GetVersion(sys, &fmod_ver))) {
                TF_WARN(<< "Failed to get FMOD version (" << FMOD_ErrorString(err) << ")");
                fmod_ver = 0;
            }
            dev_opened = true;
            return true;
        }

        void dev_close() {
            FMOD_RESULT err;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_System_Close(sys)))
                TF_ERROR(<< "Failed to close FMOD system (" << FMOD_ErrorString(err) << ")");
        }

        void force_play_cache() {
            FMOD_RESULT err;
            if (cache.size() == 0)
                return;
            bool from_rep = false;
            if (cache[0] == cur_mus) {
                cache.erase(cache.begin());
                if (stopped || was_finished) {
                    // Hack
                    cur_mus = nullptr;
                    force_play_cache();
                    return;
                }
                if (ch) {
                    if (FMOD_HAS_ERROR(err = fmod.FMOD_Channel_SetPosition(ch, 0, FMOD_TIMEUNIT_MS)))
                        TF_WARN(<< "Failed to seek music to start (" << FMOD_ErrorString(err) << ")");
                    return;
                }
                from_rep = true;
            }
            stopped = false;
            if (ch) {
                // TODO: fade out
                fmod.FMOD_Channel_Stop(ch);
                return;
            }
            Music* prev = nullptr;
            if (!from_rep) {
                prev = cur_mus;
                cur_mus = cache[0];
                cache.erase(cache.begin());
            }
            pl::mus_open_file(cur_mus);
            if (FMOD_HAS_ERROR(err = fmod.FMOD_System_PlaySound(sys, cur_h, nullptr, 1, &ch))) {
                TF_ERROR(<< "Failed to play music (" << FMOD_ErrorString(err) << ")");
                ch = nullptr;
                was_finished = true;
            }
            else {
                was_finished = false;
                unsigned int samp_buf;
                if (FMOD_HAS_ERROR(err = fmod.FMOD_Sound_GetLength(cur_h, &samp_buf, FMOD_TIMEUNIT_PCM))) {
                    TF_WARN(<< "Failed to get music pcm length (" << FMOD_ErrorString(err) << ")");
                    sps = 44100.f;
                }
                else
                    sps = (float)samp_buf / cur_mus->dur;
                if (FMOD_HAS_ERROR(err = fmod.FMOD_Channel_SetCallback(ch, fmod_channel_callback)))
                    TF_WARN(<< "Failed to set music callback (" << FMOD_ErrorString(err) << ")");
                if (FMOD_HAS_ERROR(err = fmod.FMOD_Channel_SetVolume(ch, volume)))
                    TF_WARN(<< "Failed to set music volume (" << FMOD_ErrorString(err) << ")");
                if (FMOD_HAS_ERROR(err = fmod.FMOD_Channel_SetPaused(ch, 0)))
                    TF_WARN(<< "WTF failed to unpause music for playing (" << FMOD_ErrorString(err) << ")");
            }
            if (prev && prev != cur_mus && std::find(cache.begin(), cache.end(), prev) == cache.end())
                mus_close(prev);
            pl::fill_cache();
        }

        void cur_pause() {
            if (!ch || cur_paused())
                return;
            FMOD_RESULT err;
            unsigned long long buf;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_Channel_GetDSPClock(ch, nullptr, &buf))) {
                TF_WARN(<< "Failed to get music DSP clock (" << FMOD_ErrorString(err) << ")");
                return;
            }
            if (FMOD_HAS_ERROR(err = fmod.FMOD_Channel_AddFadePoint(ch, buf, 1.f)))
                TF_WARN(<< "Failed to add music fade start point (" << FMOD_ErrorString(err) << ")");
            if (FMOD_HAS_ERROR(err = fmod.FMOD_Channel_AddFadePoint(ch, buf + (unsigned long long)(fade_pause_time * sps), 0.f)))
                TF_WARN(<< "Failed to add music fade end point (" << FMOD_ErrorString(err) << ")");
            fading = true;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_Channel_SetDelay(ch, 0, buf + (unsigned long long)(fade_pause_time * sps), 0)))
                TF_WARN(<< "Failed to set music delay (" << FMOD_ErrorString(err) << ")");
        }

        void remove_fades() {
            if (!ch || !fading)
                return;
            FMOD_RESULT err;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_Channel_RemoveFadePoints(ch, 0, (unsigned long long)((cur_mus->dur + 1.f) * sps))))
                TF_WARN(<< "Failed to remove music fade points (" << FMOD_ErrorString(err) << ")");
            fading = false;
        }

        void cur_resume() {
            if (!ch)
                return;
            remove_fades();
        }

        bool cur_paused() {
            if (!ch)
                return false;
            FMOD_RESULT err;
            FMOD_BOOL ret;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_Channel_GetPaused(ch, &ret))) {
                TF_WARN(<< "Failed to get music pause state (" << FMOD_ErrorString(err) << ")");
                return false;
            }
            return ret ? true : false;
        }

        void update_volume() {
            FMOD_RESULT err;
            if (ch && FMOD_HAS_ERROR(err = fmod.FMOD_Channel_SetVolume(ch, volume)))
                TF_WARN(<< "Failed to set music volume (" << FMOD_ErrorString(err) << ")");
        }

        void cur_stop() {
            if (!ch)
                return;
            bool was_paused = cur_paused();
            stopped = true;
            // TODO: fade out
            fmod.FMOD_Channel_Stop(ch);
            pl::fill_cache();
        }

        float cur_get_pos() {
            if (cur_paused())
                return pause_pos;
            if (!cur_mus || stopped || !ch)
                return 0.f;
            FMOD_RESULT err;
            unsigned int buf;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_Channel_GetPosition(ch, &buf, FMOD_TIMEUNIT_MS))) {
                TF_WARN(<< "Failed to get music pos (" << FMOD_ErrorString(err) << ")");
                return 0.f;
            }
            return (float)buf / 1000.f;
        }

        void cur_set_pos(float pos) {
            if (!cur_mus || !ch)
                return;
            if (pos < 0.f)
                pos = 0.f;
            else if (pos > cur_mus->dur)
                pos = cur_mus->dur;
            // TODO: if paused
            if (0) {
                pause_pos = pos;
                return;
            }
            FMOD_RESULT err;
            remove_fades();
            if (FMOD_HAS_ERROR(err = fmod.FMOD_Channel_SetPosition(ch, (unsigned int)(pos * 1000.f), FMOD_TIMEUNIT_MS)))
                TF_WARN(<< "Failed to set music pos (" << FMOD_ErrorString(err) << ")");
        }

        void update() {
            FMOD_RESULT err;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_System_Update(sys)))
                TF_WARN(<< "Failed to update FMOD system (" << FMOD_ErrorString(err) << ")");
            // fmod_channel_callback is called here, am i right?
            if (was_finished) {
                was_finished = false;
                if (!stopped)
                    force_play_cache();
                pl::fill_cache();
                int cnt = std::min((int)cache.size(), cache_opened_cnt);
                for (int i = 0; i < cnt; i++)
                    pl::mus_open_file(cache[i]);
            }
        }

        bool mus_fill_info(Music* mus) {
            bool ret = true;
            FMOD_RESULT err;
            unsigned int buf;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_Sound_GetLength(mus_h, &buf, FMOD_TIMEUNIT_MS))) {
                TF_ERROR(<< "Failed to get music length (" << FMOD_ErrorString(err) << ")");
                ret = false;
            }
            else
                mus->dur = (float)buf / 1000.f;
            FMOD_SOUND_TYPE tp;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_Sound_GetFormat(mus_h, &tp, nullptr, nullptr, nullptr))) {
                TF_ERROR(<< "Failed to get music format (" << FMOD_ErrorString(err) << ")");
                ret = false;
            }
            else {
                if (tp == FMOD_SOUND_TYPE_UNKNOWN)
                    mus->type = Type::NONE;
                else if (tp == FMOD_SOUND_TYPE_AIFF)
                    mus->type = Type::AIFF;
                else if (tp == FMOD_SOUND_TYPE_ASF)
                    mus->type = Type::ASF;
                else if (tp == FMOD_SOUND_TYPE_DLS)
                    mus->type = Type::DLS;
                else if (tp == FMOD_SOUND_TYPE_FLAC)
                    mus->type = Type::FLAC;
                else if (tp == FMOD_SOUND_TYPE_FSB || tp == FMOD_SOUND_TYPE_XMA || tp == FMOD_SOUND_TYPE_AT9)
                    mus->type = Type::FSB;
                else if (tp == FMOD_SOUND_TYPE_IT)
                    mus->type = Type::IT;
                else if (tp == FMOD_SOUND_TYPE_MIDI)
                    mus->type = Type::MID;
                else if (tp == FMOD_SOUND_TYPE_MOD)
                    mus->type = Type::MOD;
                else if (tp == FMOD_SOUND_TYPE_MPEG)
                    mus->type = Type::MP3;
                else if (tp == FMOD_SOUND_TYPE_OGGVORBIS)
                    mus->type = Type::OGG;
                else if (tp == FMOD_SOUND_TYPE_RAW)
                    mus->type = Type::RAW;
                else if (tp == FMOD_SOUND_TYPE_S3M)
                    mus->type = Type::S3M;
                else if (tp == FMOD_SOUND_TYPE_WAV)
                    mus->type = Type::WAV;
                else if (tp == FMOD_SOUND_TYPE_XM)
                    mus->type = Type::XM;
                else if (tp == FMOD_SOUND_TYPE_AUDIOQUEUE)
                    mus->type = Type::AAC;
                else if (tp == FMOD_SOUND_TYPE_VORBIS)
                    mus->type = Type::VORBIS;
                else if (tp == FMOD_SOUND_TYPE_MEDIA_FOUNDATION)
                    mus->type = Type::WMA;
                else if (tp == FMOD_SOUND_TYPE_MEDIACODEC)
                    mus->type = Type::M4A;
                else if (tp == FMOD_SOUND_TYPE_FADPCM)
                    mus->type = Type::FADPCM;
                else if (tp == FMOD_SOUND_TYPE_OPUS)
                    mus->type = Type::OPUS;
                else
                    mus->type = Type::NONE;
            }
            return ret;
        }

        bool mus_open_fp(Music* mus, const char* fp) {
            if (mus->h1)
                return true;
            FMOD_RESULT err;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_System_CreateStream(
                sys, fp, FMOD_LOOP_OFF | FMOD_2D | FMOD_CREATESTREAM,
                nullptr, (FMOD_SOUND**)&mus->h1
            ))) {
                TF_ERROR(<< "Failed to open music (" << FMOD_ErrorString(err) << ")");
                mus->h1 = nullptr;
                return false;
            }
            return true;
        }

        void mus_close(Music* mus) {
            if (!mus->h1)
                return;
            FMOD_RESULT err;
            if (err = fmod.FMOD_Sound_Release(mus_h))
                TF_ERROR(<< "Failed to close music (" << FMOD_ErrorString(err) << ")");
            mus->h1 = nullptr;
        }

        ~AudioFMOD() {
            if (!inited)
                return;
            if (dev_opened)
                dev_close();
            FMOD_RESULT err;
            if (FMOD_HAS_ERROR(err = fmod.FMOD_System_Release(sys)))
                TF_ERROR(<< "Failed to release FMOD system (" << FMOD_ErrorString(err) << ")");
            SDL_UnloadObject(fmod.handle);
            inited = false;
        }
    };
}

FMOD_RESULT F_CALL fmod_channel_callback(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2) {
	if ((controltype == FMOD_CHANNELCONTROL_CHANNEL) && (callbacktype == FMOD_CHANNELCONTROL_CALLBACK_END)) {
		((audio::AudioFMOD*)audio::au)->ch = nullptr;
		((audio::AudioFMOD*)audio::au)->was_finished = true;
		((audio::AudioFMOD*)audio::au)->fading = false;
	}
	return FMOD_OK;
}

audio::AudioBase* audio::create_fmod() {
    return (audio::AudioBase*)tf::nw<audio::AudioFMOD>();
}
#endif
