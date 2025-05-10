#ifndef LOG_HPP_
#define LOG_HPP_

#include <dnd_config.hpp>

#if DND_DEBUG_MODE

#include <fmt/format.h>

#define _DND_LOG(log_type, msg)                                                                                        \
    do {                                                                                                               \
        fmt::print("[" log_type "] ({}:{}:{}): {}\n", __FILE__, __func__, __LINE__, msg);                              \
    } while (false)

#else

#define _DND_LOG(...)                                                                                                  \
    do {                                                                                                               \
    } while (false)

#endif

#define LOGINFO(...) _DND_LOG("INFO", fmt::format(__VA_ARGS__))
#define LOGDEBUG(...) _DND_LOG("DEBUG", fmt::format(__VA_ARGS__))
#define LOGWARN(...) _DND_LOG("WARN", fmt::format(__VA_ARGS__))

#endif // LOG_HPP_
