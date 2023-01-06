#ifndef MEASURER_HPP_
#define MEASURER_HPP_

#include <algorithm>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>

#include <nlohmann/json.hpp>

// inspired by TheCherno

namespace dnd {

struct TimerResult {
    std::string name;
    long long start, end;
    size_t thread_id;
};

struct MeasuringSession {
    std::string name, filepath;
    nlohmann::json json;
};

// dirty way of saving threading information into json file that you can view in https://ui.perfetto.dev/
class Measurer {
private:
    MeasuringSession* session;
    std::chrono::high_resolution_clock::time_point session_start_time;
    std::ofstream output_stream;
    static std::mutex write_profile_mutex;
public:
    Measurer();
    void beginSession(const std::string& name, const std::string& filepath);
    void endSession();
    void writeProfile(const TimerResult& result);
    static Measurer& get();
};

// a dirty instrumenation scope timer
class Timer {
private:
    const std::string name;
    bool stopped;
    std::chrono::high_resolution_clock::time_point start_time;
public:
    Timer(const char* name);
    virtual ~Timer();
    void stop();
};

inline std::mutex Measurer::write_profile_mutex;

inline Measurer::Measurer() : session(nullptr) {}

inline Measurer& Measurer::get() {
    static Measurer instance;
    return instance;
}

inline Timer::Timer(const char* name) : name(name), stopped(false) {
    start_time = std::chrono::high_resolution_clock::now();
}

inline Timer::~Timer() {
    if (!stopped) {
        stop();
    }
}

} // namespace dnd

#endif // MEASURER_HPP_
