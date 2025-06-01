#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <app.hpp>

int main(int argc, char* argv[]) {
	if (!app::init())
		return 1;
	app::run();
	app::destroy();
	return 0;
}
