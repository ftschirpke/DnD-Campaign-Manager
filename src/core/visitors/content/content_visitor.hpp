#ifndef CONTENT_VISITOR_HPP_
#define CONTENT_VISITOR_HPP_

#include <dnd_config.hpp>

namespace dnd {

class Character;
class Class;
class Subclass;
class Species;
class Subspecies;
class Item;
class Spell;
class Feature;
class Choosable;

class ContentVisitor {
public:
    virtual ~ContentVisitor() = default;
    virtual void operator()(const Character& character) = 0;
    virtual void operator()(const Class& cls) = 0;
    virtual void operator()(const Subclass& subclass) = 0;
    virtual void operator()(const Species& species) = 0;
    virtual void operator()(const Subspecies& subspecies) = 0;
    virtual void operator()(const Item& item) = 0;
    virtual void operator()(const Spell& spell) = 0;
    virtual void operator()(const Feature& feature) = 0;
    virtual void operator()(const Choosable& choosable) = 0;
};

} // namespace dnd

#endif // CONTENT_VISITOR_HPP_
