#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <dnd_config.hpp>

#include <cassert>
#include <filesystem>
#include <set>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>

namespace dnd {

class Parser {
public:
    virtual ~Parser() noexcept = default;

    const std::filesystem::path& get_filepath() const noexcept;
protected:
    explicit Parser(const std::filesystem::path& filepath) noexcept;
    /**
     * @brief Parses an optional attribute from a json and adding it into the output variable if it exists
     * @tparam T the type the attribute value is supposed to be
     * @param json the json the attribute is supposed to be in
     * @param attribute_name the name of the attribute in the json
     * @param out the output to write the attribute value to
     * @param filepath the path to the file which is being parsed
     * @return the errors that occured while parsing
     */
    template <typename T>
    static Errors parse_optional_attribute(
        const nlohmann::json& json, const char* attribute_name, T& out, const std::filesystem::path& filepath
    );
    /**
     * @brief Parses a required attribute from a json and adding it into the output variable
     * @tparam T the type the attribute value is supposed to be
     * @param json the json the attribute is supposed to be in
     * @param attribute_name the name of the attribute in the json
     * @param out the output to write the attribute value to
     * @param filepath the path to the file which is being parsed
     * @return the errors that occured while parsing
     */
    template <typename T>
    static Errors parse_required_attribute(
        const nlohmann::json& json, const char* attribute_name, T& out, const std::filesystem::path& filepath
    );
private:
    std::filesystem::path filepath;
};

template <typename T>
const char* type_name() {
    return typeid(T).name();
}

template <typename T>
Errors Parser::parse_optional_attribute(
    const nlohmann::json& json, const char* attribute_name, T& out, const std::filesystem::path& filepath
) {
    assert(json.is_object());
    Errors errors;
    if (!json.contains(attribute_name)) {
        return errors;
    }
    try {
        out = json[attribute_name].get<T>();
    } catch (const nlohmann::json::type_error& e) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_ATTRIBUTE_TYPE, filepath,
            fmt::format("The attribute '{}' is of the wrong type, it should be a {}", attribute_name, type_name<T>())
        );
    }
    return errors;
}

template <typename T>
Errors Parser::parse_required_attribute(
    const nlohmann::json& json, const char* attribute_name, T& out, const std::filesystem::path& filepath
) {
    assert(json.is_object());
    Errors errors;
    if (!json.contains(attribute_name)) {
        errors.add_parsing_error(
            ParsingErrorCode::MISSING_ATTRIBUTE, filepath, fmt::format("The attribute '{}' is missing", attribute_name)
        );
        return errors;
    }
    try {
        out = json[attribute_name].get<T>();
    } catch (const nlohmann::json::type_error& e) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_ATTRIBUTE_TYPE, filepath,
            fmt::format("The attribute '{}' is of the wrong type, it should be {}", attribute_name, type_name<T>())
        );
    }
    return errors;
}

template <>
const char* type_name<std::vector<std::string>>() {
    return "array of strings";
}

template <>
const char* type_name<std::set<std::string>>() {
    return "set of strings";
}

template <>
const char* type_name<std::string>() {
    return "string";
}

template <>
const char* type_name<int>() {
    return "integer";
}

} // namespace dnd

#endif // PARSER_HPP_
