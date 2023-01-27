#include "dnd_config.hpp"

#include "measurer.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

const std::vector<std::string> values_for_human_readable = {
    "int dnd::launch(int, char**)",
    "Main execution scope",
    "dnd::Content dnd::ContentParser::parse(const std::filesystem::__cxx11::path&, const string&)",
    "dnd::ContentParser::parseAllOfSingleFileType ( Group )",
    "dnd::ContentParser::parseAllOfMultiFileType ( Group )",
    "dnd::ContentParser::parseAllOfMultiFileType ( Spell )",
    "dnd::ContentParser::parseAllOfMultiFileType ( Race )",
    "dnd::ContentParser::parseAllOfMultiFileType ( Class )",
    "dnd::ContentParser::parseAllOfMultiFileType ( Subrace )",
    "dnd::ContentParser::parseAllOfMultiFileType ( Subclass )",
    "dnd::ContentParser::parseAllOfMultiFileType ( Character )",
};

void dnd::Measurer::beginSession(const std::string& name, const std::string& filepath = "results.json") {
    session_start_time = std::chrono::high_resolution_clock::now();
    session = new MeasuringSession{name, filepath, {{"traceEvents", nlohmann::json::array()}}};
}

void dnd::Measurer::endSession() {
    auto session_end_time = std::chrono::high_resolution_clock::now();

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

    // write some important values to a human readable file
    output_stream.open(session->filepath + ".txt");
    size_t max_str_len = 0;
    for (const auto& value : values_for_human_readable) {
        max_str_len = std::max(max_str_len, value.size());
    }
    max_str_len += 4;

    auto start = std::chrono::system_clock::to_time_t(session_start_time);
    output_stream << "Session started at: " << std::put_time(std::localtime(&start), "%F %T\n");
    auto end = std::chrono::system_clock::to_time_t(session_end_time);
    output_stream << "Session stopped at: " << std::put_time(std::localtime(&end), "%F %T\n\n");

    for (const auto& human_readable_value : values_for_human_readable) {
        for (auto& measurement : session->json.at("traceEvents")) {
            if (measurement.at("name") == human_readable_value) {
                output_stream << human_readable_value;
                for (int i = 0; i < max_str_len - human_readable_value.size(); ++i) {
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
    delete session;
    session = nullptr;
}

void dnd::Measurer::writeProfile(const TimerResult& result) {
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

void dnd::Timer::stop() {
    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();

    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(start_time).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time).time_since_epoch().count();

    size_t thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());

    Measurer::get().writeProfile({name, start, end, thread_id});

    stopped = true;
}
