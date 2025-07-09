#ifdef LBS_GO_JOIN_UR_CODE
// Hacky
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <lbs.hpp>
#if 0
#include <log.hpp>

#else
#include <app.hpp>

int main(int argc, char* argv[]) {
	if (!app::init())
		return 1;
	app::run();
	app::destroy();
	return 0;
}
#endif
