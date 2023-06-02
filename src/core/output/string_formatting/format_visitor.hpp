#ifndef FORMAT_VISITOR_HPP_
#define FORMAT_VISITOR_HPP_

#include "dnd_config.hpp"

namespace dnd {

class BulletedList;
class Paragraph;
class Table;

class FormatVisitor {
public:
    virtual void visit(BulletedList* bulleted_list) = 0;
    virtual void visit(Paragraph* paragraph) = 0;
    virtual void visit(Table* table) = 0;
};

} // namespace dnd

#endif // FORMAT_VISITOR_HPP_
