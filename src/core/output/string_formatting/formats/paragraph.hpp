#ifndef PARAGRAPH_HPP_
#define PARAGRAPH_HPP_

#include <dnd_config.hpp>

#include <string_view>

#include <core/output/string_formatting/formats/format.hpp>

namespace dnd {

class Paragraph : public Format {
public:
    /**
     * @brief Initialize a new Paragraph object
     * @param text the text of the paragraph
     * @param empty_line_after whether to add an empty line after the paragraph
     */
    explicit Paragraph(std::string_view text, bool empty_line_after = false) noexcept;
    /**
     * @brief Accept a format visitor
     * @param visitor a pointer to the format visitor
     */
    virtual void accept(FormatVisitor* visitor);
    std::string_view get_text() const noexcept;
    bool get_empty_line_after() const noexcept;
private:
    std::string_view text;
    bool empty_line_after;
};

inline Paragraph::Paragraph(std::string_view text, bool empty_line_after) noexcept
    : text(text), empty_line_after(empty_line_after) {}

inline void Paragraph::accept(FormatVisitor* visitor) { visitor->visit(this); }

inline std::string_view Paragraph::get_text() const noexcept { return text; }

inline bool Paragraph::get_empty_line_after() const noexcept { return empty_line_after; }

} // namespace dnd

#endif // PARAGRAPH_HPP_
