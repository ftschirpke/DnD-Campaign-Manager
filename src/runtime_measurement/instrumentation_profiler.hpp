#ifndef INSTRUMENTATION_PROFILER_HPP_
#define INSTRUMENTATION_PROFILER_HPP_

#include <fstream>
#include <string>

#include <nlohmann/json.hpp>

#include "runtime_measurement/timer.hpp"

struct ProfileResult {
    std::string name;
    long long start, end;
    size_t thread_id;
};

struct InstrumentationSession {
    std::string name;
};

// dirty way of saving threading information into json file that you can view in https://ui.perfetto.dev/
class Instrumentor {
private:
    InstrumentationSession* session;
    std::ofstream output_stream;
    int profile_count;
public:
    Instrumentor() : session(nullptr), profile_count(0) {}

    void beginSession(const std::string& name, const std::string& filepath = "results.json") {
        output_stream.open(filepath);
        writeHeader();
        session = new InstrumentationSession{name};
    }

    void endSession() {
        writeFooter();
        output_stream.close();
        delete session;
        session = nullptr;
        profile_count = 0;
    }

    void writeProfile(const ProfileResult& result) {
        if (profile_count++ > 0)
            output_stream << ",";

        std::string name = result.name;
        std::replace(name.begin(), name.end(), '"', '\'');

        nlohmann::json result_json = {
            {"cat", "function"}, {"dur", result.end - result.start}, {"name", name},       {"ph", "X"},
            {"pid", 0},          {"tid", result.thread_id},          {"ts", result.start},
        };

        output_stream << result_json;

        output_stream.flush();
    }

    void writeHeader() {
        output_stream << "{\"otherData\": {},\"traceEvents\":[";
        output_stream.flush();
    }

    void writeFooter() {
        output_stream << "]}";
        output_stream.flush();
    }

    static Instrumentor& get() {
        static Instrumentor instance;
        return instance;
    }
};

// a dirty instrumenation scope timer
class InstrumentationTimer {
private:
    const std::string name;
    bool stopped;
    std::chrono::high_resolution_clock::time_point start_time;
public:
    InstrumentationTimer(const char* name);
    virtual ~InstrumentationTimer();
    void stop();
    virtual void output(const std::string& name, long long start, long long end, size_t thread_id) const;
};

inline InstrumentationTimer::InstrumentationTimer(const char* name) : name(name), stopped(false) {
    start_time = std::chrono::high_resolution_clock::now();
}

inline InstrumentationTimer::~InstrumentationTimer() {
    if (!stopped) {
        stop();
    }
}

inline void InstrumentationTimer::stop() {
    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();

    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(start_time).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time).time_since_epoch().count();

    size_t thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());

    output(name, start, end, thread_id);

    stopped = true;
}

inline void InstrumentationTimer::output(const std::string& name, long long start, long long end, size_t thread_id)
    const {
    Instrumentor::get().writeProfile({name, start, end, thread_id});
}

#endif // INSTRUMENTATION_PROFILER_HPP_
