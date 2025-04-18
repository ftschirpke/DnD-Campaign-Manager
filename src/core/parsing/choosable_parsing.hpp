#ifndef CHOOSABLE_PARSING_HPP_
#define CHOOSABLE_PARSING_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects_provider/choosable.hpp>

namespace dnd {

WithErrors<Choosable::Data> parse_feat(const nlohmann::json& obj, const std::filesystem::path& filepath);

} // namespace dnd

#endif // CHOOSABLE_PARSING_HPP_
