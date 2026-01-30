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

    Opt<Id> find(Type type, const std::string& key) const;
#define DECLARE_FIND(C, U, j, a, p, P) Opt<Id> find_##j(const std::string& key) const;
    X_CONTENT_PIECES(DECLARE_FIND)
#undef DECLARE_FIND

    ContentPieceVariant get(Id id) const;
    const ContentPiece* get_ptr(Id id) const;

#define GET_ID(C, U, j, a, p, P) const C& get_##j(Id id) const;
    X_CONTENT_PIECES(GET_ID)
#undef GET_ID
#define GET_IDX(C, U, j, a, p, P) const C& get_##j(size_t index) const;
    X_CONTENT_PIECES(GET_IDX)
#undef GET_IDX

#define GET_ALL(C, U, j, a, p, P) const std::vector<C>& get_all_##p() const;
    X_OWNED_CONTENT_PIECES(GET_ALL)
#undef GET_ALL
#define GET_ALL_REF(C, U, j, a, p, P) const std::vector<CRef<C>>& get_all_##p() const;
    X_REFERENCE_CONTENT_PIECES(GET_ALL_REF)
#undef GET_ALL_REF
#define GET_LIB(C, U, j, a, p, P) const StorageContentLibrary<C>& get_##j##_library() const;
    X_OWNED_CONTENT_PIECES(GET_LIB)
#undef GET_LIB
#define GET_LIB_REF(C, U, j, a, p, P) const ReferencingContentLibrary<C>& get_##j##_library() const;
    X_REFERENCE_CONTENT_PIECES(GET_LIB_REF)
#undef GET_LIB_REF

    std::optional<EffectsProviderVariant> get_effects_provider(const std::string& key) const;

    void set_subgroup(const std::string& group_name, const std::string& subgroup_name);
    void set_subgroups(const std::string& group_name, std::set<std::string>&& subgroups);
    void add_group_member(const std::string& group_name, const std::string& value);
    void add_group_members(const std::string& group_name, std::set<std::string>&& values);

#define ADD(C, U, j, a, p, P) Opt<CRef<C>> add_##j(C&& a);
    X_OWNED_CONTENT_PIECES(ADD)
#undef ADD
#define ADD_REF(C, U, j, a, p, P) Opt<CRef<C>> add_##j##_result(CreateResult<C>&& a);
    X_OWNED_CONTENT_PIECES(ADD_REF)
#undef ADD_REF
private:
    Groups groups;

#define DECL_LIB(C, U, j, a, p, P) StorageContentLibrary<C> j##_library;
    X_OWNED_CONTENT_PIECES(DECL_LIB)
#undef DECL_LIB
#define DECL_REF_LIB(C, U, j, a, p, P) ReferencingContentLibrary<C> j##_library;
    X_REFERENCE_CONTENT_PIECES(DECL_REF_LIB)
#undef DECL_REF_LIB
};

} // namespace dnd

#endif // CONTENT_HPP_
