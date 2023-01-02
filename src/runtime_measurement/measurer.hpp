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
    virtual void output(const std::string& name, long long start, long long end, size_t thread_id) const;
};

inline std::mutex Measurer::write_profile_mutex;

inline Measurer::Measurer() : session(nullptr) {}

inline void Measurer::beginSession(const std::string& name, const std::string& filepath = "results.json") {
    session = new MeasuringSession{name, filepath, {{"traceEvents", nlohmann::json::array()}}};
}

inline void Measurer::endSession() {
    output_stream.open(session->filepath);
    // convert thread ids to consecutive integers
    std::vector<size_t> thread_ids;
    int next_id = 0;
    for (auto& measurement : session->json.at("traceEvents")) {
        auto it = find(thread_ids.begin(), thread_ids.end(), measurement.at("tid"));
        if (it != thread_ids.end()) {
            int idx = it - thread_ids.begin();
            measurement.at("tid") = idx;
        } else {
            thread_ids.emplace_back(measurement.at("tid"));
            measurement.at("tid") = next_id++;
        }
    }
    output_stream << session->json;
    output_stream.flush();
    output_stream.close();
    delete session;
    session = nullptr;
}

inline void Measurer::writeProfile(const TimerResult& result) {
    if (session == nullptr) {
        return;
    }
    std::string name = result.name;
    std::replace(name.begin(), name.end(), '"', '\'');

    nlohmann::json result_json = {
        {"cat", "function"}, {"dur", result.end - result.start}, {"name", name},       {"ph", "X"},
        {"pid", 0},          {"tid", result.thread_id},          {"ts", result.start},
    };

    std::lock_guard<std::mutex> lock(write_profile_mutex);
    session->json.at("traceEvents").push_back(result_json);
}

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

inline void Timer::stop() {
    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();

    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(start_time).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time).time_since_epoch().count();

    size_t thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());

    output(name, start, end, thread_id);

    stopped = true;
}

inline void Timer::output(const std::string& name, long long start, long long end, size_t thread_id) const {
    Measurer::get().writeProfile({name, start, end, thread_id});
}

} // namespace dnd

#endif // MEASURER_HPP_
