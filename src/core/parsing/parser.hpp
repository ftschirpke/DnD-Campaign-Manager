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
    virtual ~Parser() = default;

    const std::filesystem::path& get_filepath() const;
protected:
    explicit Parser(const std::filesystem::path& filepath);
    bool contains_required_attribute(const nlohmann::json& json, const char* attribute_name, Errors& errors) const;
    bool contains_required_index(const nlohmann::json& json, size_t index, Errors& errors) const;
    template <typename T>
    Errors parse_optional_attribute_into(const nlohmann::json& json, const char* attribute_name, T& out) const;
    template <typename T>
    Errors parse_required_attribute_into(const nlohmann::json& json, const char* attribute_name, T& out) const;
    template <typename T>
    Errors parse_optional_index_into(const nlohmann::json& json, size_t index, T& out) const;
    template <typename T>
    Errors parse_required_index_into(const nlohmann::json& json, size_t index, T& out) const;
private:
    const std::filesystem::path& filepath;
};

template <typename T>
inline const char* type_name() {
    return typeid(T).name();
}

template <typename T>
Errors Parser::parse_optional_attribute_into(const nlohmann::json& json, const char* attribute_name, T& out) const {
    assert(json.is_object());
    Errors errors;
    if (!json.contains(attribute_name)) {
        return errors;
    }
    try {
        out = json[attribute_name].get<T>();
    } catch (const nlohmann::json::type_error& e) {
        DND_UNUSED(e);
        errors.add_parsing_error(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
            fmt::format("The attribute '{}' is of the wrong type, it should be {}", attribute_name, type_name<T>())
        );
    }
    return errors;
}

template <typename T>
Errors Parser::parse_required_attribute_into(const nlohmann::json& json, const char* attribute_name, T& out) const {
    assert(json.is_object());
    Errors errors;
    if (!contains_required_attribute(json, attribute_name, errors)) {
        return errors;
    }
    try {
        out = json[attribute_name].get<T>();
    } catch (const nlohmann::json::type_error& e) {
        DND_UNUSED(e);
        errors.add_parsing_error(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
            fmt::format("The attribute '{}' is of the wrong type, it should be {}", attribute_name, type_name<T>())
        );
    }
    return errors;
}

template <typename T>
Errors Parser::parse_optional_index_into(const nlohmann::json& json, size_t index, T& out) const {
    assert(json.is_array());
    Errors errors;
    if (json.size() <= index) {
        return errors;
    }
    try {
        out = json[index].get<T>();
    } catch (const nlohmann::json::type_error& e) {
        DND_UNUSED(e);
        errors.add_parsing_error(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
            fmt::format("The value at index {} is of the wrong type, it should be {}", index, type_name<T>())
        );
    }
    return errors;
}

template <typename T>
Errors Parser::parse_required_index_into(const nlohmann::json& json, size_t index, T& out) const {
    assert(json.is_array());
    Errors errors;
    if (!contains_required_index(json, index, errors)) {
        return errors;
    }
    try {
        out = json[index].get<T>();
    } catch (const nlohmann::json::type_error& e) {
        DND_UNUSED(e);
        errors.add_parsing_error(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
            fmt::format("The value at index {} is of the wrong type, it should be {}", index, type_name<T>())
        );
    }
    return errors;
}

template <>
inline const char* type_name<std::map<std::string, std::string>>() {
    return "map of strings to strings";
}

template <>
inline const char* type_name<std::array<int, 20>>() {
    return "length-20 array of integers";
}

template <>
inline const char* type_name<std::array<int, 6>>() {
    return "length-6 array of integers";
}

template <>
inline const char* type_name<std::vector<std::string>>() {
    return "array of strings";
}

template <>
inline const char* type_name<std::vector<int>>() {
    return "array of integers";
}

template <>
inline const char* type_name<std::set<std::string>>() {
    return "set of strings";
}

template <>
inline const char* type_name<std::string>() {
    return "string";
}

template <>
inline const char* type_name<int>() {
    return "integer";
}

template <>
inline const char* type_name<bool>() {
    return "boolean";
}

} // namespace dnd

#endif // PARSER_HPP_
