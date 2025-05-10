#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <dnd_config.hpp>

#include <cassert>
#include <filesystem>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <tl/expected.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/text/text.hpp>

namespace dnd {

class Parser {
public:
    virtual ~Parser() = default;

    const std::filesystem::path& get_filepath() const;
protected:
    explicit Parser(const std::filesystem::path& filepath);
private:
    const std::filesystem::path& filepath;
};

std::optional<Error> parse_paragraph(std::string&& str, Paragraph& paragraph, const std::filesystem::path& filepath);

tl::expected<Table, Error> parse_table(const nlohmann::json& json, const std::filesystem::path& filepath);

std::optional<Error> parse_list(const nlohmann::json& list_items, Text& out, const std::filesystem::path& filepath);

std::optional<Error> write_formatted_text_into(
    const nlohmann::json& json, Text& out, const std::filesystem::path& filepath
);

std::optional<Error> write_formatted_text_into(
    const nlohmann::json& json, Text& out, const std::filesystem::path& filepath, bool skip_first
);


enum class JsonType {
    STRING,
    OBJECT,
    ARRAY,
    ANY,
};

std::optional<Error> check_required_attribute(
    const nlohmann::json& json, const char* attribute_name, const std::filesystem::path& filepath, JsonType typ
);

std::optional<Error> check_required_index(
    const nlohmann::json& json, size_t index, const std::filesystem::path& filepath, JsonType typ
);

std::string or_of_strings(const std::vector<std::string>& strs);

std::string and_of_strings(const std::vector<std::string>& strs);

template <typename T>
const char* type_name();

template <typename T>
std::optional<Error> parse_optional_attribute_into(
    const nlohmann::json& json, const char* attribute_name, T& out, const std::filesystem::path& filepath
) {
    assert(json.is_object());
    Errors errors;
    if (!json.contains(attribute_name)) {
        return std::nullopt;
    }
    try {
        out = json[attribute_name].get<T>();
    } catch (const nlohmann::json::type_error& e) {
        DND_UNUSED(e);
        return ParsingError(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
            fmt::format("The attribute '{}' is of the wrong type, it should be {}", attribute_name, type_name<T>())
        );
    }
    return std::nullopt;
}

template <typename T>
std::optional<Error> parse_required_attribute_into(
    const nlohmann::json& json, const char* attribute_name, T& out, const std::filesystem::path& filepath
) {
    assert(json.is_object());
    std::optional<Error> check_error = check_required_attribute(json, attribute_name, filepath, JsonType::ANY);
    if (check_error) {
        return check_error;
    }
    try {
        out = json[attribute_name].get<T>();
    } catch (const nlohmann::json::type_error& e) {
        DND_UNUSED(e);
        return ParsingError(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
            fmt::format("The attribute '{}' is of the wrong type, it should be {}", attribute_name, type_name<T>())
        );
    }
    return std::nullopt;
}

template <typename T>
std::optional<Error> parse_attribute_into(
    const nlohmann::json& json, const char* attribute_name, T& out, const std::filesystem::path& filepath,
    bool is_required
) {
    if (is_required) {
        return parse_required_attribute_into(json, attribute_name, out, filepath);
    } else {
        return parse_optional_attribute_into(json, attribute_name, out, filepath);
    }
}


template <typename T>
std::optional<Error> parse_optional_index_into(
    const nlohmann::json& json, size_t index, T& out, const std::filesystem::path& filepath
) {
    assert(json.is_array());
    Errors errors;
    if (json.size() <= index) {
        return std::nullopt;
    }
    try {
        out = json[index].get<T>();
    } catch (const nlohmann::json::type_error& e) {
        DND_UNUSED(e);
        return ParsingError(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
            fmt::format("The value at index {} is of the wrong type, it should be {}", index, type_name<T>())
        );
    }
    return std::nullopt;
}

template <typename T>
std::optional<Error> parse_required_index_into(
    const nlohmann::json& json, size_t index, T& out, const std::filesystem::path& filepath
) {
    assert(json.is_array());
    std::optional<Error> check_error = check_required_index(json, index, filepath, JsonType::ANY);
    if (check_error.has_value()) {
        return check_error;
    }
    try {
        out = json[index].get<T>();
    } catch (const nlohmann::json::type_error& e) {
        DND_UNUSED(e);
        return ParsingError(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
            fmt::format("The value at index {} is of the wrong type, it should be {}", index, type_name<T>())
        );
    }
    return std::nullopt;
}

template <typename T>
std::optional<Error> parse_index_into(
    const nlohmann::json& json, size_t index, T& out, const std::filesystem::path& filepath, bool is_required
) {
    if (is_required) {
        return parse_required_index_into(json, index, out, filepath);
    } else {
        return parse_optional_index_into(json, index, out, filepath);
    }
}


template <typename T>
inline const char* type_name() {
    return typeid(T).name();
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
inline const char* type_name<std::array<int, 9>>() {
    return "length-9 array of integers";
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
