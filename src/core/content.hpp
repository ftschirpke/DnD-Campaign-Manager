#ifndef CONTENT_HPP_
#define CONTENT_HPP_

#include <dnd_config.hpp>

#include <string>

#include <core/data_result.hpp>
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
#include <x/content_pieces.hpp>

namespace dnd {

using EffectsProviderVariant = VarOfCRef<Feature, ClassFeature, Choosable>;

using ContentPieceVariant = VarOfCRef<
    Character, Class, Subclass, Species, Subspecies, Item, Spell, Choosable, Feature, ClassFeature, SubclassFeature>;

/**
 * @brief A class that holds all the content for a certain session or campaign
 */
class Content {
public:
    bool empty() const;

    const Groups& get_groups() const;

    std::optional<Id> find(Type type, const std::string& key) const;
#define X(C, U, j, a, p, P) std::optional<Id> find_##j(const std::string& key) const;
    X_CONTENT_PIECES
#undef X

    ContentPieceVariant get(Id id) const;
    const ContentPiece* get_ptr(Id id) const;

#define X(C, U, j, a, p, P) const C& get_##j(Id id) const;
    X_CONTENT_PIECES
#undef X
#define X(C, U, j, a, p, P) const C& get_##j(size_t index) const;
    X_CONTENT_PIECES
#undef X

#define X(C, U, j, a, p, P) const std::vector<C>& get_all_##p() const;
    X_OWNED_CONTENT_PIECES
#undef X
#define X(C, U, j, a, p, P) const std::vector<CRef<C>>& get_all_##p() const;
    X_REFERENCE_CONTENT_PIECES
#undef X
#define X(C, U, j, a, p, P) const StorageContentLibrary<C>& get_##j##_library() const;
    X_OWNED_CONTENT_PIECES
#undef X
#define X(C, U, j, a, p, P) const ReferencingContentLibrary<C>& get_##j##_library() const;
    X_REFERENCE_CONTENT_PIECES
#undef X

    std::optional<EffectsProviderVariant> get_effects_provider(const std::string& key) const;

    void set_subgroup(const std::string& group_name, const std::string& subgroup_name);
    void set_subgroups(const std::string& group_name, std::set<std::string>&& subgroups);
    void add_group_member(const std::string& group_name, const std::string& value);
    void add_group_members(const std::string& group_name, std::set<std::string>&& values);

#define X(C, U, j, a, p, P) OptCRef<C> add_##j(C&& a);
    X_OWNED_CONTENT_PIECES
#undef X
#define X(C, U, j, a, p, P) OptCRef<C> add_##j##_result(CreateResult<C>&& a);
    X_OWNED_CONTENT_PIECES
#undef X
private:
    Groups groups;

#define X(C, U, j, a, p, P) StorageContentLibrary<C> j##_library;
    X_OWNED_CONTENT_PIECES
#undef X
#define X(C, U, j, a, p, P) ReferencingContentLibrary<C> j##_library;
    X_REFERENCE_CONTENT_PIECES
#undef X
};

} // namespace dnd

#endif // CONTENT_HPP_
