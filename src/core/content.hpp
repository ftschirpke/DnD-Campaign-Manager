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

    std::optional<EffectsProviderVariant> get_effects_provider(const std::string& name) const;

    void set_subgroup(const std::string& group_name, const std::string& subgroup_name);
    void set_subgroups(const std::string& group_name, std::set<std::string>&& subgroups);
    void add_group_member(const std::string& group_name, const std::string& value);
    void add_group_members(const std::string& group_name, std::set<std::string>&& values);
private:
    Groups groups;

#define X(Cap, UP, join, alone, plural)                                                                                \
public:                                                                                                                \
    const StorageContentLibrary<Cap>& get_##plural() const;                                                            \
    OptCRef<Cap> add_##alone(Cap&& alone);                                                                             \
    OptCRef<Cap> add_##alone##_result(CreateResult<Cap>&& alone);                                                      \
private:                                                                                                               \
    StorageContentLibrary<Cap> join##_library;
#include <x/owned_content_pieces.x>
#undef X

#define X(Cap, UP, join, alone, plural)                                                                                \
public:                                                                                                                \
    const ReferencingContentLibrary<Cap>& get_##plural() const;                                                        \
private:                                                                                                               \
    ReferencingContentLibrary<Cap> join##_library;
#include <x/reference_content_pieces.x>
#undef X
};

} // namespace dnd

#endif // CONTENT_HPP_
