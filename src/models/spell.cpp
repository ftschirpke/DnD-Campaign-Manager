#include "dnd_config.hpp"

#include "spell.hpp"

#include <cctype>
#include <string>

std::string dnd::SpellType::str() const {
    std::string spell_type_string;
    int lv = levelAsNumber();
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
    std::string school_name = magicSchoolName(magic_school);
    auto toupper = [](unsigned char c) { return static_cast<unsigned char>(std::toupper(c)); };
    school_name[0] = toupper(school_name[0]);
    spell_type_string += school_name;

    if (is_ritual) {
        spell_type_string += " (Ritual)";
    }

    return spell_type_string;
}

std::string dnd::SpellComponents::shortStr() const {
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
    std::string components_string = shortStr();
    if (material) {
        components_string += " (";
        components_string += materials_needed;
        components_string += ')';
    }
    return components_string;
}
