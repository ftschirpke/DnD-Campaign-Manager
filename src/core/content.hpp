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

/**
 * @brief A class that holds all the content for a certain session or campaign
 */
class Content {
public:
    bool empty() const;
    /**
     * @brief Returns a string describing how many items this controller holds
     * @return a string describing the current status of the controller
     */
    std::string status() const;

    const Groups& get_groups() const;

#define X(C, U, j, a, p, P) const StorageContentLibrary<C>& get_##p() const;
    X_OWNED_CONTENT_PIECES
#undef X
#define X(C, U, j, a, p, P) const ReferencingContentLibrary<C>& get_##p() const;
    X_REFERENCE_CONTENT_PIECES
#undef X

    std::optional<EffectsProviderVariant> get_effects_provider(const std::string& name) const;

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
