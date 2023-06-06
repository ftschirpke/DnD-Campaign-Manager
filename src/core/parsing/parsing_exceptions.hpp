#ifndef PARSING_EXCEPTIONS_HPP_
#define PARSING_EXCEPTIONS_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <stdexcept>
#include <string>

#include <core/parsing/parsing_types.hpp>

namespace dnd {

/**
 * @brief Removes the ugly parts of nlohmann::json exceptions.
 * @param original_what the original exception message
 * @return a stripped version of the same message
 */
std::string strip_json_exception_what(const std::string& original_what);

/**
 * @brief An exception that is thrown when something went wrong while parsing DnD content
 */
class parsing_error : public std::invalid_argument {
public:
    /**
     * @brief Constructs a general parsing exception
     * @param path the file that was being parsed as the error occured
     * @param error_msg an error message
     */
    parsing_error(const std::filesystem::path& path, const std::string& error_msg);
    /**
     * @brief Constructs a parsing error for a certain parsing type
     * @param parsing_type the type of content that was being parsed as the error occured
     * @param path the file that was being parsed as the error occured
     * @param error_msg an error message
     */
    parsing_error(ParsingType parsing_type, const std::filesystem::path& path, const std::string& error_msg);
    /**
     * @brief Relativises the file path of the parsing_error to the given path (helps making the error message cleaner)
     * @param root_path the updated path will be relative to this root path
     */
    void relativiseFileName(const std::filesystem::path& root_path);
    /**
     * @brief Returns a C-style character string describing the general cause of the error
     * @return a C-style character string describing the general cause of the error
     */
    const char* what() const noexcept override;
private:
    // path to the file the error occured
    std::filesystem::path path;
    // start of the error message (where did it happen)
    std::string msg_start;
    // body of the error message (what happened)
    const std::string error_msg;
    // the constructed error message that will be thrown by what()
    std::string w;
    /**
     * @brief reconstructs the error message (e.g. after relativising the file path)
     */
    void updateWhat();
};

/**
 * @brief An exception that is thrown when a json file is not formatted the way it's supposed to
 */
class json_format_error : public parsing_error {
public:
    /**
     * @brief Constructs a json-format exception
     * @param parsing_type the type of content that was being parsed as the error occured
     * @param path the file that was being parsed as the error occured
     * @param desired_format the desired format for the file
     */
    json_format_error(ParsingType parsing_type, const std::filesystem::path& path, const std::string& desired_format);
};

/**
 * @brief An exception that is thrown when an attribute is not formatted the way it's supposed to
 */
class attribute_format_error : public parsing_error {
public:
    /**
     * @brief Constructs an attribute-format exception
     * @param parsing_type the type of content that was being parsed as the error occured
     * @param path the file that was being parsed as the error occured
     * @param attribute the name of the attribute that has the wrong format
     * @param desired_format the desired format for the attribute
     */
    attribute_format_error(
        ParsingType parsing_type, const std::filesystem::path& path, const std::string& attribute,
        const std::string& desired_format
    );
};

/**
 * @brief An exception that is thrown when a required attribute is missing i.e. the key is missing in the JSON file
 */
class attribute_missing : public parsing_error {
public:
    /**
     * @brief Constructs an attribute-missing exception
     * @param parsing_type the type of content that was being parsed as the error occured
     * @param path the file that was being parsed as the error occured
     * @param missing_attribute_msg an error message describing what exactly is missing
     */
    attribute_missing(
        ParsingType parsing_type, const std::filesystem::path& path, const std::string& missing_attribute_msg
    );
};

/**
 * @brief An exception that is thrown when an attribute is of a wrong type
 */
class attribute_type_error : public parsing_error {
public:
    /**
     * @brief Constructs an attribute-type exception
     * @param parsing_type the type of content that was being parsed as the error occured
     * @param path the file that was being parsed as the error occured
     * @param type_error_msg an error message describing the type the attribute should have
     */
    attribute_type_error(
        ParsingType parsing_type, const std::filesystem::path& path, const std::string& type_error_msg
    );
};

/**
 * @brief An exception that is thrown when an attribute has a logically wrong or forbidden value
 */
class invalid_attribute : public parsing_error {
public:
    /**
     * @brief Constructs and invalid-attribute exception
     * @param parsing_type the type of content that was being parsed as the error occured
     * @param path the file that was being parsed as the error occured
     * @param attribute the name of the attribute that's invalid
     * @param error_msg an error message describing in which way the attribute is invalid
     */
    invalid_attribute(
        ParsingType parsing_type, const std::filesystem::path& path, const std::string& attribute,
        const std::string& error_msg
    );
};

inline std::string strip_json_exception_what(const std::string& original_what) {
    std::string what = original_what;
    if (size_t i = what.find("]"); i != std::string::npos) {
        what.erase(0, i + 2);
    }
    return what;
}

inline parsing_error::parsing_error(const std::filesystem::path& path, const std::string& error_msg)
    : std::invalid_argument(""), path(path), msg_start("File"), error_msg(error_msg) {
    updateWhat();
}

inline parsing_error::parsing_error(
    ParsingType parsing_type, const std::filesystem::path& path, const std::string& error_msg
)
    : std::invalid_argument(""), path(path), msg_start(std::string(parsing_type_name(parsing_type)) + " in file"),
      error_msg(error_msg) {
    updateWhat();
}

inline void parsing_error::updateWhat() { w = msg_start + " \"" + path.string() + "\" " + error_msg; }


inline void parsing_error::relativiseFileName(const std::filesystem::path& root_path) {
    path = path.lexically_relative(root_path);
    updateWhat();
}

inline const char* parsing_error::what() const noexcept { return w.c_str(); }


inline json_format_error::json_format_error(
    dnd::ParsingType parsing_type, const std::filesystem::path& path, const std::string& desired_format
)
    : parsing_error(parsing_type, path, "has wrong format: should be " + desired_format) {}


inline attribute_format_error::attribute_format_error(
    ParsingType parsing_type, const std::filesystem::path& path, const std::string& attribute,
    const std::string& desired_format
)
    : parsing_error(
        parsing_type, path, "has attribute of wrong format: \"" + attribute + "\" should be " + desired_format
    ) {}


inline attribute_missing::attribute_missing(
    dnd::ParsingType parsing_type, const std::filesystem::path& path, const std::string& missing_attribute_msg
)
    : parsing_error(parsing_type, path, "is missing an attribute: " + missing_attribute_msg) {}


inline attribute_type_error::attribute_type_error(
    ParsingType parsing_type, const std::filesystem::path& path, const std::string& type_error_msg
)
    : parsing_error(parsing_type, path, "has attribute of wrong type: " + type_error_msg) {}


inline invalid_attribute::invalid_attribute(
    ParsingType parsing_type, const std::filesystem::path& path, const std::string& attribute,
    const std::string& error_msg
)
    : parsing_error(parsing_type, path, "has invalid attribute \"" + attribute + "\": " + error_msg) {}


} // namespace dnd

#endif // PARSING_EXCEPTIONS_HPP_
