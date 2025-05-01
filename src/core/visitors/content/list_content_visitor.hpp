#ifndef LIST_CONTENT_VISITOR_HPP_
#define LIST_CONTENT_VISITOR_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/models/character/character.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

class ListContentVisitor : public ContentVisitor {
public:
    void reserve(size_t size);
    const std::vector<std::string>& get_list() const;
    std::vector<std::string> get_list();
    void clear_list();
    virtual void operator()(const Character& character) override;
    virtual void operator()(const Class& cls) override;
    virtual void operator()(const Subclass& subclass) override;
    virtual void operator()(const Species& species) override;
    virtual void operator()(const Subspecies& subspecies) override;
    virtual void operator()(const Item& item) override;
    virtual void operator()(const Spell& spell) override;
    virtual void operator()(const Feature& feature) override;
    virtual void operator()(const Choosable& choosable) override;
private:
    std::vector<std::string> string_list;
};


} // namespace dnd

#endif // LIST_CONTENT_VISITOR_HPP_
