#ifndef CONTENT_VISITOR_HPP_
#define CONTENT_VISITOR_HPP_

#include <dnd_config.hpp>

#include <core/content.hpp>
#include <x/content_pieces.hpp>

namespace dnd {

#define DECL_CLASS(C, U, j, a, p, P) class C;
X_CONTENT_PIECES(DECL_CLASS)
#undef DECL_CLASS

class ContentVisitor {
public:
    virtual ~ContentVisitor() = default;
    void visit_variant(const ContentPieceVariant& variant);
#define DECL_VISIT(C, U, j, a, p, P) virtual void visit(const C& a) = 0;
    X_CONTENT_PIECES(DECL_VISIT)
#undef DECL_VISIT
};

} // namespace dnd

#endif // CONTENT_VISITOR_HPP_
