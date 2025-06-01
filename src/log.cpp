#include <config.hpp>
#include <log.hpp>
#include <unreachable.hpp>
#include <SDL3/SDL_log.h>

namespace logger {
	int log_level;
}

void logger::log_by_category(const char* data, const char* file, const char* func, int line, int category) {
	if (category < log_level)
		return;
	SDL_LogPriority pr;
	switch (category) {
	case 0: {
		pr = SDL_LOG_PRIORITY_INFO;
		break;
	}
	case 1: {
		pr = SDL_LOG_PRIORITY_WARN;
		break;
	}
	case 2: {
		pr = SDL_LOG_PRIORITY_ERROR;
		break;
	}
	case 3: {
		pr = SDL_LOG_PRIORITY_CRITICAL;
		break;
	}
	default: {
		TF_UNREACHABLE();
	}
	}
#if IS_RELEASE
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, pr, "[%s:%i] at %s: %s", file, line, func, data);
#else
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, pr, "[%s:%i] at %s: %s", file, line, func, data);
#endif
}
