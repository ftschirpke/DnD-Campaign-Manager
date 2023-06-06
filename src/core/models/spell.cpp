#include <dnd_config.hpp>

#include "spell.hpp"

#include <string>

#include <fmt/format.h>

#include <core/content_visitors/content_visitor.hpp>
#include <core/models/content_piece.hpp>
#include <core/utils/char_manipulation.hpp>

std::string dnd::SpellType::short_str() const {
    int lv = level_number();
    std::string school_name = magic_school_name(magic_school);
    if (lv == 0) {
        return fmt::format("{} cantrip", school_name);
    }
    std::string spell_level_string;
    if (lv == 1) {
        spell_level_string = "1st";
    } else if (lv == 2) {
        spell_level_string = "2nd";
    } else if (lv == 3) {
        spell_level_string = "3rd";
    } else {
        spell_level_string = std::to_string(lv) + "th";
    }
    return fmt::format("{} level {}", spell_level_string, school_name);
}

std::string dnd::SpellType::str() const {
    std::string spell_type_string;
    int lv = level_number();
    if (lv == 0) {
        spell_type_string += "Cantrip";
    } else {
        if (lv == 1) {
            spell_type_string += "1st";
        } else if (lv == 2) {
            spell_type_string += "2nd";
        } else if (lv == 3) {
            spell_type_string += "3rd";
        } else {
            spell_type_string += std::to_string(lv) + "th";
        }
        spell_type_string += "-level spell";
    }

    spell_type_string += " - School of ";
    std::string school_name = magic_school_name(magic_school);
    school_name[0] = char_to_uppercase(school_name[0]);
    spell_type_string += school_name;

    if (is_ritual) {
        spell_type_string += " (Ritual)";
    }

    return spell_type_string;
}

std::string dnd::SpellComponents::short_str() const {
    std::string components_string;
    int i = 0;
    if (verbal) {
        components_string += 'V';
        i++;
    }
    if (somatic) {
        if (i > 0) {
            components_string += ", ";
        }
        components_string += 'S';
        i++;
    }
    if (material) {
        if (i > 0) {
            components_string += ", ";
        }
        components_string += 'M';
    }
    return components_string;
}

std::string dnd::SpellComponents::str() const {
    std::string components_string = short_str();
    if (material) {
        components_string += " (";
        components_string += materials_needed;
        components_string += ')';
    }
    return components_string;
}
