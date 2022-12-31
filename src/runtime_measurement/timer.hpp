#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

// a dirty scope timer
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
    std::cout << name << " : " << (end - start) << "ms (thread " << thread_id << ")\n";
}


#endif // TIMER_HPP_
