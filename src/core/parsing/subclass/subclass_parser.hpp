#ifndef CHARACTER_SUBCLASS_PARSER_HPP_
#define CHARACTER_SUBCLASS_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/effects_provider/class_feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/subclass/subclass_data.hpp>

namespace dnd {

class SubclassParser : public FileParser {
public:
    explicit SubclassParser(const std::filesystem::path& filepath) noexcept;
    virtual Errors parse() override;
    virtual Errors validate(const Content& content) const override;
    virtual void save_result(Content& content) override;
private:
    ClassFeatureParser class_feature_parser;
    SubclassData data;
};

} // namespace dnd

#endif // CHARACTER_SUBCLASS_PARSER_HPP_
