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
#define DECL_VISIT(C, U, j, a, p, P) virtual void visit(const C& a) override final;
    X_CONTENT_PIECES(DECL_VISIT)
#undef DECL_VISIT
private:
    const Content& content;
    const GuiFonts& fonts;
};

void display_formatted_text(const Text& formatted_text, const GuiFonts& fonts);

} // namespace dnd

#endif // DISPLAY_VISITOR_HPP_
