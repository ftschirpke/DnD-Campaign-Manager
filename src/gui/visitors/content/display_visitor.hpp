#ifndef DISPLAY_VISITOR_HPP_
#define DISPLAY_VISITOR_HPP_

#include <dnd_config.hpp>

#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

class Content;
struct Text;
struct GuiFonts;

class DisplayVisitor : public ContentVisitor {
public:
    DisplayVisitor(const Content& content, const GuiFonts& fonts);
#define X(C, U, j, a, p, P) virtual void visit(const C& a) override;
    X_CONTENT_PIECES
#undef X
private:
    const Content& content;
    const GuiFonts& fonts;
};

void display_formatted_text(const Text& formatted_text, const GuiFonts& fonts);

} // namespace dnd

#endif // DISPLAY_VISITOR_HPP_
