#ifndef FORMAT_VISITOR_HPP_
#define FORMAT_VISITOR_HPP_

#include <dnd_config.hpp>

namespace dnd {

class BulletedList;
class Paragraph;
class Table;

class FormatVisitor {
public:
    virtual ~FormatVisitor() = default;
    virtual void visit(const BulletedList& bulleted_list) const = 0;
    virtual void visit(const Paragraph& paragraph) const = 0;
    virtual void visit(const Table& table) const = 0;
};

} // namespace dnd

#endif // FORMAT_VISITOR_HPP_
