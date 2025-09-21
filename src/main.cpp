#ifdef LBS_GO_JOIN_UR_CODE
// Hacky
#define IMGUI_DEFINE_MATH_OPERATORS
#include <lbs.hpp>
#include <app.hpp>
#include <vec.hpp>
#include <new.hpp>
#include <control.hpp>
#include <log.hpp>
#endif
#include <SDL3/SDL.h>
#include <lbs.hpp>
#include <app.hpp>
#if IS_DLL_BUILD
#include <vec.hpp>
#include <new.hpp>
#include <control.hpp>
#include <log.hpp>

#if IS_WIN
#define TF_EXPORT extern "C" __declspec(dllexport)
#else
#define TF_EXPORT extern "C"
#endif

struct TF_Cmd {
	int idx;
	float val;
};

namespace ui {
	extern void (*global_status_cb)(const char*);
}

tf::vec<TF_Cmd>* tf_dll_commands;
SDL_Mutex* tf_dll_mut;
int tf_dll_inited;

void tf_dll_update() {
	SDL_LockMutex(tf_dll_mut);
	while (tf_dll_commands->size() > 0) {
		TF_Cmd cmd = tf_dll_commands->at(tf_dll_commands->size() - 1);
		tf_dll_commands->pop_back();
		switch (cmd.idx) {
			case 0: {
				ctrl::stop();
				break;
			}
			case 1: {
				ctrl::play();
				break;
			}
			case 2: {
				ctrl::pause();
				break;
			}
			case 3: {
				ctrl::next();
				break;
			}
			case 4: {
				ctrl::set_vol(cmd.val);
				break;
			}
			case 5: {
				ctrl::ch_vol(cmd.val);
				break;
			}
			case 6: {
				ctrl::set_pos(cmd.val);
				break;
			}
			case 7: {
				ctrl::ch_pos(cmd.val);
				break;
			}
			case 8: {
				app::stop(cmd.val >= 1.f);
				break;
			}
			default: {
				TF_WARN(<< "Unknown cmd: " << cmd.idx);
				break;
			}
		}
	}
	SDL_UnlockMutex(tf_dll_mut);
}

static int tf_prog_thread(void* ptr) {
	(void)ptr;
	tf_dll_mut = SDL_CreateMutex();
	if (!tf_dll_mut) {
		TF_FATAL(<< "Failed to create tinyfoo mutex (" << SDL_GetError() << ")");
		tf_dll_inited = -1;
		return 1;
	}
	if (!app::init()) {
		tf_dll_inited = -1;
		return 1;
	}
	tf_dll_commands = tf::nw<tf::vec<TF_Cmd>>();
	tf_dll_inited = 1;
	app::run();
	SDL_LockMutex(tf_dll_mut);
	tf_dll_inited = 0;
	tf_dll_commands->clear();
	tf::dl(tf_dll_commands);
	SDL_UnlockMutex(tf_dll_mut);
	SDL_DestroyMutex(tf_dll_mut);
	tf_dll_commands = nullptr;
	tf_dll_mut = nullptr;
	app::destroy();
    return 0;
}

TF_EXPORT void tf_time_delay(int ms) {
	SDL_Delay((Uint32)ms);
}

TF_EXPORT int tf_get_init_state() {
	return tf_dll_inited;
}

TF_EXPORT void tf_set_status_callback(void (*status_cb)(const char*)) {
	// TODO: make this safe
	SDL_LockMutex(tf_dll_mut);
	ui::global_status_cb = status_cb;
	SDL_UnlockMutex(tf_dll_mut);
}

TF_EXPORT int tf_thread_cmd(TF_Cmd cmd) {
	if (tf_dll_inited != 1)
		return 0;
	SDL_LockMutex(tf_dll_mut);
	if (tf_dll_commands->size() <= 5) {
		tf_dll_commands->push_back(cmd);
		SDL_UnlockMutex(tf_dll_mut);
		return 1;
	}
	SDL_UnlockMutex(tf_dll_mut);
	return 0;
}

TF_EXPORT int tf_threaded_main(int blocking) {
	tf_dll_inited = 0;
	tf_dll_commands = nullptr;
	tf_dll_mut = nullptr;
	SDL_Thread* thread = SDL_CreateThread(tf_prog_thread, "tinyfoo", nullptr);
	if (!thread) {
		TF_FATAL(<< "Failed to create tinyfoo thread (" << SDL_GetError() << ")");
		return 0;
	}
	if (blocking > 0)
		SDL_WaitThread(thread, nullptr);
	else
		SDL_DetachThread(thread);
	return 1;
}
#else
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	if (!app::init())
		return 1;
	app::run();
	app::destroy();
	return 0;
}
#endif
