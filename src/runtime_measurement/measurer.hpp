#ifndef MEASURER_HPP_
#define MEASURER_HPP_

#include "dnd_config.hpp"

#include <chrono>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>

#include <nlohmann/json.hpp>

// inspired by TheCherno

namespace dnd {

/**
 * @brief A struct representing a result of one timer
 */
struct TimerResult {
    // the name of the timer
    std::string name;
    // the start time id
    int64_t start;
    // the end time id
    int64_t end;
    // the thread id
    size_t thread_id;
};

/**
 * @brief A struct representing the information for one measuring session
 */
struct MeasuringSession {
    // the name of the session
    std::string name;
    // the file path where to store the results
    std::string filepath;
    // the json where the results are stored before they are written into the file
    nlohmann::json json;
};

/**
 * @brief A dirty way of saving threading information into json files that you can view with https://ui.perfetto.dev/
 */
class Measurer {
public:
    /**
     * @brief Constructs a Measurer
     */
    Measurer() noexcept;
    /**
     * @brief Starts a measuring session with a certain name
     * @param name a name for the measuring session
     * @param filepath the file to store the results in
     */
    void beginSession(const std::string& name, const std::string& filepath);
    /**
     * @brief Ends a measuring session
     */
    void endSession();
    /**
     * @brief Save the profile of one timing result
     * @param result the result of one timer
     */
    void writeProfile(const TimerResult& result);
    static Measurer& get();
private:
    // a mutex to control writing access to the results json
    static std::mutex write_profile_mutex;
    // the current measuring session, nullptr if there is none
    std::unique_ptr<MeasuringSession> session;
    // the start of the current session
    std::chrono::system_clock::time_point session_start_time;
};

/**
 * @brief A quick and dirty instrumenation scope timer
 */
class Timer {
public:
    /**
     * @brief Constructs or rather starts a new timer with the given name
     * @param name a name for the timer
     */
    Timer(const char* name);
    /**
     * @brief Destructs the object and stops the timer if not already stopped
     */
    virtual ~Timer();
    /**
     * @brief Stops the timer
     */
    void stop();
private:
    // the name of the timer
    const std::string name;
    // "true" if timer was stopped, "false" otherwise
    bool stopped;
    // the time the timer was started at
    std::chrono::system_clock::time_point start_time;
};

inline std::mutex Measurer::write_profile_mutex;

inline Measurer::Measurer() noexcept : session(nullptr) {}

inline Measurer& Measurer::get() {
    static Measurer instance;
    return instance;
}

inline Timer::Timer(const char* name) : name(name), stopped(false) { start_time = std::chrono::system_clock::now(); }

inline Timer::~Timer() {
    if (!stopped) {
        stop();
    }
}

} // namespace dnd

#endif // MEASURER_HPP_
