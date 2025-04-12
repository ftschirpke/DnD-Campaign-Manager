#ifndef LOG_HPP_
#define LOG_HPP_

#include <dnd_config.hpp>

namespace dnd {

#if DND_DEBUG_MODE

#include <fmt/format.h>

class Logger {
public:
    Logger(const char* log_type, const char* file, const char* func, int line);
    template <typename... T>
    void operator()(fmt::format_string<T...> fmt, T&&... args);
private:
    void log_helper(fmt::string_view fmt, fmt::format_args args);

    const char* log_type;
    const char* file;
    const char* func;
    int line;
};

inline Logger::Logger(const char* log_type, const char* file, const char* func, int line)
    : log_type(log_type), file(file), func(func), line(line) {}

template <typename... T>
inline void Logger::operator()(fmt::format_string<T...> fmt, T&&... args) {
    log_helper(fmt, fmt::make_format_args(args...));
}

inline void Logger::log_helper(fmt::string_view fmt, fmt::format_args args) {
    fmt::print("[{}] ({}:{}:{}): {}\n", log_type, file, line, func, fmt::vformat(fmt, args));
}

#define _DND_LOG(log_type) Logger(log_type, __FILE__, __func__, __LINE__)

#define LOGINFO _DND_LOG("INFO")
#define LOGDEBUG _DND_LOG("DEBUG")
#define LOGWARN _DND_LOG("WARN")

#else

#define LOG(logging_type, format_message, ...)                                                                         \
    do {                                                                                                               \
    } while (false)

#endif

} // namespace dnd

#endif // LOG_HPP_
