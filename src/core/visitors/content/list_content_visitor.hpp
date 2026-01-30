#ifndef LIST_CONTENT_VISITOR_HPP_
#define LIST_CONTENT_VISITOR_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/content.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

class ListContentVisitor : public ContentVisitor {
public:
    explicit ListContentVisitor(const Content& content) noexcept;

    void reserve(size_t size);
    const std::vector<std::string>& get_list() const;
    std::vector<std::string> get_list();
    void clear_list();

#define DECL_VISIT(C, U, j, a, p, P) virtual void visit(const C& a) override;
    X_CONTENT_PIECES(DECL_VISIT)
#undef DECL_VISIT
private:
    const Content& content;
    std::vector<std::string> string_list;
};


} // namespace dnd

#endif // LIST_CONTENT_VISITOR_HPP_
