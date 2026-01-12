#ifndef CONTENT_VISITOR_HPP_
#define CONTENT_VISITOR_HPP_

#include <dnd_config.hpp>

#include <core/content.hpp>
#include <x/content_pieces.hpp>

namespace dnd {

#define X(C, U, j, a, p, P) class C;
X_CONTENT_PIECES
#undef X

class ContentVisitor {
public:
    virtual ~ContentVisitor() = default;
    void visit_variant(const ContentPieceVariant& variant);
#define X(C, U, j, a, p, P) virtual void visit(const C& a) = 0;
    X_CONTENT_PIECES
#undef X
};

} // namespace dnd

#endif // CONTENT_VISITOR_HPP_
