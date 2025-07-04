#pragma once
#include <lbs.hpp>
#include <sstream>

#if IS_RELEASE
#include <alloc.hpp>

#define _TF_LOG_TEMPLATE(category, ...) do { \
    std::basic_stringstream<char, std::char_traits<char>, mem::FAlloc<char>> _temp_string_stream; \
    _temp_string_stream __VA_ARGS__; \
    logger::log_by_category(_temp_string_stream.str().c_str(), nullptr, nullptr, 0, category); \
} while (0)
#else
#define _TF_LOG_TEMPLATE(category, ...) do { \
    std::stringstream _temp_string_stream; \
    _temp_string_stream __VA_ARGS__; \
    logger::log_by_category(_temp_string_stream.str().c_str(), logger::format_source_filename(__FILE__), __func__, __LINE__, category); \
} while (0)
#endif
// TODO: MIN_LOG_LEVEL
#define TF_INFO(...) _TF_LOG_TEMPLATE(0, __VA_ARGS__)
#define TF_WARN(...) _TF_LOG_TEMPLATE(1, __VA_ARGS__)
#define TF_ERROR(...) _TF_LOG_TEMPLATE(2, __VA_ARGS__)
#define TF_FATAL(...) _TF_LOG_TEMPLATE(3, __VA_ARGS__)

namespace logger {
    constexpr size_t comptime_strlen_heck(const char* str) {
        size_t result = 0;
        while (*(str++) != '\0')
            ++result;
        return result;
    }

    constexpr const char* format_source_filename(const char* fn) {
        char* end = (char*)fn + comptime_strlen_heck(fn) - 1;
        while (end > fn + 8 && *end != '\\' && *end != '/') {
            end--;
        }
        return (end > fn + 8) ? (end - 3) : fn;
    }

    void log_by_category(const char* data, const char* file, const char* func, int line, int category);
}
