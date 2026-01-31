#ifndef EFFECTS_PARSING_HPP_
#define EFFECTS_PARSING_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects/effects.hpp>

namespace dnd {

WithErrors<Effects::Data> parse_effects(const nlohmann::json& obj, const std::filesystem::path& filepath);

} // namespace dnd

#endif // EFFECTS_PARSING_HPP_
