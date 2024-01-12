#ifndef CLASS_PARSER_HPP_
#define CLASS_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/class/class.hpp>
#include <core/parsing/effects_provider/class_feature_parser.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

class ClassParser : public FileParser {
public:
    explicit ClassParser(const std::filesystem::path& filepath);
    virtual Errors parse() override;
    virtual void save_result(Content& content) override;
private:
    ClassFeatureParser class_feature_parser;
    Class::Data data;
};

} // namespace dnd

#endif // CLASS_PARSER_HPP_
