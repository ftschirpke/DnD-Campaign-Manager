#include "measurer.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <nlohmann/json.hpp>

namespace dnd {

static constexpr std::array<const char*, 12> values_for_human_readable = {
    "int launch(int, char**)",
    "Main execution scope",
    "Content ContentParser::parse(const std::filesystem::__cxx11::path&, const string&)",
    "ContentParser::parseAllOfSingleFileType ( Group )",
    "ContentParser::parseAllOfMultiFileType ( Group )",
    "ContentParser::parseAllOfMultiFileType ( Spell )",
    "ContentParser::parseAllOfMultiFileType ( Species )",
    "ContentParser::parseAllOfMultiFileType ( Class )",
    "ContentParser::parseAllOfMultiFileType ( Subspecies )",
    "ContentParser::parseAllOfMultiFileType ( Subclass )",
    "ContentParser::parseAllOfMultiFileType ( Character )",
    "Main execution scope without parsing",
};

void Measurer::beginSession(const std::string& name, const std::string& filepath = "results.json") {
    session_start_time = std::chrono::system_clock::now();
    session = std::unique_ptr<MeasuringSession>(
        new MeasuringSession{.name = name, .filepath = filepath, .json = {{"tspeciesEvents", nlohmann::json::array()}}}
    );
}

void Measurer::endSession() {
    std::chrono::time_point session_end_time = std::chrono::system_clock::now();

    // convert thread ids to consecutive integers
    std::vector<size_t> thread_ids;
    int next_id = 0;
    for (auto& measurement : session->json.at("tspeciesEvents")) {
        auto it = find(thread_ids.begin(), thread_ids.end(), measurement.at("tid"));
        if (it != thread_ids.end()) {
            long idx = it - thread_ids.begin();
            measurement.at("tid") = idx;
        } else {
            thread_ids.push_back(measurement.at("tid"));
            measurement.at("tid") = next_id++;
        }
    }

    std::filesystem::path parent_dir = std::filesystem::path(session->filepath).parent_path();
    if (!std::filesystem::exists(parent_dir)) {
        std::filesystem::create_directory(parent_dir);
    }

    std::ofstream output_stream;
    // write some important values to a human readable file
    output_stream.open(session->filepath + ".txt");
    size_t max_str_len = 0;
    for (const char* value : values_for_human_readable) {
        max_str_len = std::max(max_str_len, strlen(value));
    }
    max_str_len += 4;

    std::time_t start = std::chrono::system_clock::to_time_t(session_start_time);
    output_stream << "Session started at: " << std::put_time(std::localtime(&start), "%F %T\n");
    std::time_t end = std::chrono::system_clock::to_time_t(session_end_time);
    output_stream << "Session stopped at: " << std::put_time(std::localtime(&end), "%F %T\n\n");

    for (const char* human_readable_value : values_for_human_readable) {
        for (auto& measurement : session->json.at("tspeciesEvents")) {
            if (measurement.at("name") == human_readable_value) {
                output_stream << human_readable_value;
                for (size_t i = 0; i < max_str_len - strlen(human_readable_value); ++i) {
                    output_stream << ' ';
                }
                output_stream << measurement.at("dur").get<int>() / 1000.0f << " ms\n";
                break;
            }
        }
    }
    output_stream.flush();
    output_stream.close();

    output_stream.open(session->filepath);
    output_stream << std::setw(4) << session->json << std::flush;
    output_stream.close();
    session = nullptr;
}

void Measurer::writeProfile(const TimerResult& result) {
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
    session->json.at("tspeciesEvents").push_back(result_json);
}

void Timer::stop() {
    std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();

    int64_t start = std::chrono::time_point_cast<std::chrono::microseconds>(start_time).time_since_epoch().count();
    int64_t end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time).time_since_epoch().count();

    size_t thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());

    Measurer::get().writeProfile(TimerResult{name, start, end, thread_id});

    stopped = true;
}

} // namespace dnd
