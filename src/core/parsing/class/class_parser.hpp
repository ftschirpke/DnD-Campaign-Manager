#ifndef CHARACTER_CLASS_PARSER_HPP_
#define CHARACTER_CLASS_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/effects_provider/class_feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/class/class_data.hpp>

namespace dnd {

class ClassParser : public FileParser {
public:
    explicit ClassParser(const std::filesystem::path& filepath) noexcept;
    virtual Errors parse() override;
    virtual Errors validate(const Content& content) const override;
    virtual void save_result(Content& content) override;
private:
    ClassFeatureParser class_feature_parser;
    ClassData data;
};

} // namespace dnd

#endif // CHARACTER_CLASS_PARSER_HPP_