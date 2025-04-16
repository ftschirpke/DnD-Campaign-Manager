#ifndef CLASS_PARSING_HPP_
#define CLASS_PARSING_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/class/class.hpp>
#include <core/models/subclass/subclass.hpp>

namespace dnd {

WithErrors<Class::Data> parse_class(const nlohmann::ordered_json& obj, const std::filesystem::path& filepath);
Errors parse_class_feature(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath,
    std::map<std::string, Class::Data>& parsed_classes
);
WithErrors<Subclass::Data> parse_subclass(const nlohmann::ordered_json& obj, const std::filesystem::path& filepath);
Errors parse_subclass_feature(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath,
    std::map<std::string, Subclass::Data>& parsed_subclasses
);

} // namespace dnd

#endif // CLASS_PARSING_HPP_
