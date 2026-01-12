#include <dnd_config.hpp>

#include "content.hpp"

#include <cassert>
#include <string>
#include <utility>

#include <fmt/format.h>

#include <core/groups.hpp>
#include <core/models/character/character.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/effects_provider/subclass_feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/referencing_content_library.hpp>
#include <core/storage_content_library.hpp>
#include <core/types.hpp>

namespace dnd {

bool Content::empty() const {
#define X(C, U, j, a, p, P) j##_library.empty() &&
    return X_CONTENT_PIECES true;
#undef X
}

const Groups& Content::get_groups() const { return groups; }

Opt<Id> Content::find(Type type, const std::string& key) const {
    std::optional<size_t> index;
    switch (type) {
#define X(C, U, j, a, p, P)                                                                                            \
    case Type::C: {                                                                                                    \
        index = j##_library.find(key);                                                                                 \
        break;                                                                                                         \
    }
        X_CONTENT_PIECES
#undef X
        default:
            std::unreachable();
    }
    if (!index.has_value()) {
        return std::nullopt;
    }
    return Id{.index = index.value(), .type = type};
}

#define X(C, U, j, a, p, P)                                                                                            \
    Opt<Id> Content::find_##j(const std::string& key) const { return find(Type::C, key); }
X_CONTENT_PIECES
#undef X

ContentPieceVariant Content::get(Id id) const {
    switch (id.type) {
#define X(C, U, j, a, p, P)                                                                                            \
    case Type::C: {                                                                                                    \
        auto val = j##_library.get(id.index);                                                                          \
        assert(val.has_value()); /* we assume the IDs we handed out to be valid */                                     \
        return val.value();                                                                                            \
    }
        X_CONTENT_PIECES
#undef X
    }
    std::unreachable();
}

const ContentPiece* Content::get_ptr(Id id) const {
    switch (id.type) {
#define X(C, U, j, a, p, P)                                                                                            \
    case Type::C: {                                                                                                    \
        return &get_##j(id);                                                                                           \
    }
        X_CONTENT_PIECES
#undef X
    }
    std::unreachable();
}

#define X(C, U, j, a, p, P)                                                                                            \
    const C& Content::get_##j(size_t index) const {                                                                    \
        auto val = j##_library.get(index);                                                                             \
        assert(val.has_value()); /* we assume the IDs we handed out to be valid */                                     \
        return val.value();                                                                                            \
    }
X_CONTENT_PIECES
#undef X

#define X(C, U, j, a, p, P)                                                                                            \
    const C& Content::get_##j(Id id) const {                                                                           \
        assert(id.type == Type::C);                                                                                    \
        return get_##j(id.index);                                                                                      \
    }
X_CONTENT_PIECES
#undef X

#define X(C, U, j, a, p, P)                                                                                            \
    const std::vector<C>& Content::get_all_##p() const { return get_##j##_library().get_all(); }
X_OWNED_CONTENT_PIECES
#undef X

#define X(C, U, j, a, p, P)                                                                                            \
    const std::vector<CRef<C>>& Content::get_all_##p() const { return get_##j##_library().get_all(); }
X_REFERENCE_CONTENT_PIECES
#undef X

#define X(C, U, j, a, p, P)                                                                                            \
    const StorageContentLibrary<C>& Content::get_##j##_library() const { return j##_library; }
X_OWNED_CONTENT_PIECES
#undef X

#define X(C, U, j, a, p, P)                                                                                            \
    const ReferencingContentLibrary<C>& Content::get_##j##_library() const { return j##_library; }
X_REFERENCE_CONTENT_PIECES
#undef X

std::optional<EffectsProviderVariant> Content::get_effects_provider(const std::string& key) const {
    Opt<CRef<Feature>> feature = feature_library.get(key);
    if (feature.has_value()) {
        return feature.value();
    }
    Opt<CRef<ClassFeature>> class_feature = class_feature_library.get(key);
    if (class_feature.has_value()) {
        return class_feature.value();
    }
    Opt<CRef<Choosable>> choosable = choosable_library.get(key);
    if (choosable.has_value()) {
        return choosable.value();
    }
    return std::nullopt;
}

void Content::set_subgroup(const std::string& group_name, const std::string& subgroup_name) {
    groups.set_subgroup(group_name, subgroup_name);
}

void Content::set_subgroups(const std::string& group_name, std::set<std::string>&& subgroup_names) {
    groups.set_subgroups(group_name, std::move(subgroup_names));
}

void Content::add_group_member(const std::string& group_name, const std::string& value) {
    groups.add(group_name, value);
}

void Content::add_group_members(const std::string& group_name, std::set<std::string>&& values) {
    groups.add(group_name, std::move(values));
}

Opt<CRef<Character>> Content::add_character(Character&& character) {
    std::optional<size_t> character_index = character_library.add(std::move(character));
    if (!character_index.has_value()) {
        return std::nullopt;
    }
    Opt<CRef<Character>> inserted_character = character_library.get(character_index.value());
    if (inserted_character.has_value()) {
        for (const Feature& feature : inserted_character.value().get().get_features()) {
            feature_library.add(feature);
        }
    }
    return inserted_character;
}

Opt<CRef<Class>> Content::add_class(Class&& cls) {
    std::optional<size_t> class_index = class_library.add(std::move(cls));
    if (!class_index.has_value()) {
        return std::nullopt;
    }
    Opt<CRef<Class>> inserted_class = class_library.get(class_index.value());
    if (inserted_class.has_value()) {
        for (const ClassFeature& feature : inserted_class.value().get().get_features()) {
            class_feature_library.add(feature);
        }
    }
    return inserted_class;
}

Opt<CRef<Subclass>> Content::add_subclass(Subclass&& subclass) {
    std::optional<size_t> subclass_index = subclass_library.add(std::move(subclass));
    if (!subclass_index.has_value()) {
        return std::nullopt;
    }
    Opt<CRef<Subclass>> inserted_subclass = subclass_library.get(subclass_index.value());
    if (inserted_subclass.has_value()) {
        for (const SubclassFeature& subclass_feature : inserted_subclass.value().get().get_features()) {
            subclass_feature_library.add(subclass_feature);
        }
    }
    return inserted_subclass;
}

Opt<CRef<Species>> Content::add_species(Species&& species) {
    std::optional<size_t> species_index = species_library.add(std::move(species));
    if (!species_index.has_value()) {
        return std::nullopt;
    }
    Opt<CRef<Species>> inserted_species = species_library.get(species_index.value());
    if (inserted_species.has_value()) {
        for (const Feature& feature : inserted_species.value().get().get_features()) {
            feature_library.add(feature);
        }
    }
    return inserted_species;
}

Opt<CRef<Subspecies>> Content::add_subspecies(Subspecies&& subspecies) {
    std::optional<size_t> subspecies_index = subspecies_library.add(std::move(subspecies));
    if (!subspecies_index.has_value()) {
        return std::nullopt;
    }
    Opt<CRef<Subspecies>> inserted_subspecies = subspecies_library.get(subspecies_index.value());
    if (inserted_subspecies.has_value()) {
        for (const Feature& feature : inserted_subspecies.value().get().get_features()) {
            feature_library.add(feature);
        }
    }
    return inserted_subspecies;
}

Opt<CRef<Item>> Content::add_item(Item&& item) {
    std::optional<size_t> item_index = item_library.add(std::move(item));
    if (!item_index.has_value()) {
        return std::nullopt;
    }
    return item_library.get(item_index.value());
}

Opt<CRef<Spell>> Content::add_spell(Spell&& spell) {
    std::optional<size_t> spell_index = spell_library.add(std::move(spell));
    if (!spell_index.has_value()) {
        return std::nullopt;
    }
    return spell_library.get(spell_index.value());
}

Opt<CRef<Choosable>> Content::add_choosable(Choosable&& choosable) {
    std::optional<size_t> choosable_index = choosable_library.add(std::move(choosable));
    if (!choosable_index.has_value()) {
        return std::nullopt;
    }
    Opt<CRef<Choosable>> inserted_choosable = choosable_library.get(choosable_index.value());
    if (inserted_choosable.has_value()) {
        const Choosable& choosable_val = inserted_choosable.value().get();
        const std::string key = choosable_val.get_name();
        const std::string type_name = choosable_val.get_type();
        groups.add(type_name, key);
    }
    return inserted_choosable;
}

#define X(C, U, j, a, p, P)                                                                                            \
    Opt<CRef<C>> Content::add_##j##_result(CreateResult<C>&& a) {                                                      \
        if (a.is_valid()) {                                                                                            \
            return add_##j(std::move(a.value()));                                                                      \
        } else {                                                                                                       \
            j##_library.add_draft(std::move(a.data_and_errors()));                                                     \
            return std::nullopt;                                                                                       \
        }                                                                                                              \
    }
X_OWNED_CONTENT_PIECES
#undef X

} // namespace dnd
