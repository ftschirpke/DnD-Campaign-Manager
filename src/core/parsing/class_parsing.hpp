#ifndef CLASS_PARSING_HPP_
#define CLASS_PARSING_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/class/class.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

WithErrors<Class::Data> parse_class(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath, const FoundryFileParser& foundry_parser
);
Errors parse_class_feature(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath,
    std::map<std::string, Class::Data>& parsed_classes, const FoundryFileParser& foundry_parser
);
WithErrors<Subclass::Data> parse_subclass(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath, const FoundryFileParser& foundry_parser
);
Errors parse_subclass_feature(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath,
    std::map<std::string, Subclass::Data>& parsed_subclasses, const FoundryFileParser& foundry_parser
);

} // namespace dnd

#endif // CLASS_PARSING_HPP_
