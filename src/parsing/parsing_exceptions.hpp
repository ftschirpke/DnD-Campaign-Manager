#ifndef PARSING_EXCEPTIONS_HPP_
#define PARSING_EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>

#include "parsing/parsing_types.hpp"

namespace dnd {

std::string stripJsonExceptionWhat(const std::string& original_what);

// thrown when something went wrong with parsing DnD content
class parsing_error : public std::invalid_argument {
private:
    std::filesystem::path path;
    const std::string msg_start, error_msg;
public:
    parsing_error(const std::filesystem::path& path, const std::string& error_msg);
    parsing_error(const std::filesystem::path& path, const std::string& error_msg, ParsingType parsing_type);
    void relativiseFileName(const std::filesystem::path& root_path);
    const char* what() const noexcept override;
};

// thrown when a json file is not formatted the way it's supposed to
class json_format_error : public parsing_error {
public:
    json_format_error(const std::filesystem::path& path, const std::string& desired_format);
    json_format_error(const std::filesystem::path& path, const std::string& desired_format, ParsingType parsing_type);
};

// thrown when a required attribute is missing i.e. the key is missing in the JSON file
class attribute_missing : public parsing_error {
public:
    attribute_missing(const std::filesystem::path& path, const std::string& missing_attribute_msg);
    attribute_missing(
        const std::filesystem::path& path, const std::string& missing_attribute_msg, ParsingType parsing_type
    );
};

// throw when an attribute is of a wrong type
class attribute_type_error : public parsing_error {
public:
    attribute_type_error(const std::filesystem::path& path, const std::string& type_error_msg);
    attribute_type_error(
        const std::filesystem::path& path, const std::string& type_error_msg, ParsingType parsing_type
    );
};

// thrown when an attribute has a logically wrong or forbidden value
class invalid_attribute : public parsing_error {
public:
    invalid_attribute(const std::filesystem::path& path, const std::string& attribute, const std::string& error_msg);
    invalid_attribute(
        const std::filesystem::path& path, const std::string& attribute, const std::string& error_msg,
        ParsingType parsing_type
    );
};

inline std::string stripJsonExceptionWhat(const std::string& original_what) {
    std::string what = original_what;
    if (int i = what.find("]"); i != std::string::npos) {
        what.erase(0, i + 2);
    }
    return what;
}

inline parsing_error::parsing_error(const std::filesystem::path& path, const std::string& error_msg)
    : std::invalid_argument(""), msg_start("File"), path(path), error_msg(error_msg) {}

inline parsing_error::parsing_error(
    const std::filesystem::path& path, const std::string& error_msg, ParsingType parsing_type
)
    : std::invalid_argument(""), msg_start(parsing_type_names.at(parsing_type) + " in file"), path(path),
      error_msg(error_msg) {}

inline void parsing_error::relativiseFileName(const std::filesystem::path& root_path) {
    path = path.lexically_relative(root_path);
}

inline const char* parsing_error::what() const noexcept {
    return (msg_start + " \"" + path.c_str() + "\" " + error_msg).c_str();
}

inline json_format_error::json_format_error(const std::filesystem::path& path, const std::string& desired_format)
    : parsing_error(path, "has wrong format: should be " + desired_format) {}


inline json_format_error::json_format_error(
    const std::filesystem::path& path, const std::string& desired_format, dnd::ParsingType parsing_type
)
    : parsing_error(path, "has wrong format: should be " + desired_format, parsing_type) {}


inline attribute_missing::attribute_missing(const std::filesystem::path& path, const std::string& missing_attribute_msg)
    : parsing_error(path, "is missing an attribute: " + missing_attribute_msg) {}


inline attribute_missing::attribute_missing(
    const std::filesystem::path& path, const std::string& missing_attribute_msg, ParsingType parsing_type
)
    : parsing_error(path, "is missing an attribute: " + missing_attribute_msg, parsing_type) {}


inline attribute_type_error::attribute_type_error(const std::filesystem::path& path, const std::string& type_error_msg)
    : parsing_error(path, "has attribute of wrong type: " + type_error_msg) {}


inline attribute_type_error::attribute_type_error(
    const std::filesystem::path& path, const std::string& type_error_msg, ParsingType parsing_type
)
    : parsing_error(path, "has attribute of wrong type: " + type_error_msg, parsing_type) {}


inline invalid_attribute::invalid_attribute(
    const std::filesystem::path& path, const std::string& attribute, const std::string& error_msg
)
    : parsing_error(path, "the attribute \"" + attribute + "\" " + error_msg) {}


inline invalid_attribute::invalid_attribute(
    const std::filesystem::path& path, const std::string& attribute, const std::string& error_msg,
    ParsingType parsing_type
)
    : parsing_error(path, "the attribute \"" + attribute + "\" " + error_msg, parsing_type) {}


} // namespace dnd

#endif // PARSING_EXCEPTIONS_HPP_
