#ifndef SPELL_PARSING_HPP_
#define SPELL_PARSING_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/spell/spell.hpp>

namespace dnd {

WithErrors<Spell::Data> parse_spell(const nlohmann::ordered_json& obj, const std::filesystem::path& filepath);

}

#endif // SPELL_PARSING_HPP_
