#ifndef SUBCLASS_PARSER_HPP_
#define SUBCLASS_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/parsing/effects_provider/class_feature_parser.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

class SubclassParser : public FileParser {
public:
    explicit SubclassParser(const std::filesystem::path& filepath);
    virtual Errors parse() override;
    virtual void save_result(Content& content) override;
private:
    ClassFeatureParser class_feature_parser;
    Subclass::Data data;
};

} // namespace dnd

#endif // SUBCLASS_PARSER_HPP_
