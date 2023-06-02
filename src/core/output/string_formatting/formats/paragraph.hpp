#ifndef PARAGRAPH_HPP_
#define PARAGRAPH_HPP_

#include "dnd_config.hpp"

#include <string_view>

#include "core/output/string_formatting/formats/format.hpp"

namespace dnd {

class Paragraph : public Format {
public:
    /**
     * @brief Initialize a new Paragraph object
     * @param text the text of the paragraph
     * @param empty_line_after whether to add an empty line after the paragraph
     */
    explicit Paragraph(std::string_view text, bool empty_line_after = true) noexcept;
    /**
     * @brief Accept a format visitor
     * @param visitor a pointer to the format visitor
     */
    virtual void accept(FormatVisitor* visitor) const;
    std::string_view get_text() const noexcept;
private:
    std::string_view text;
};

inline Paragraph::Paragraph(std::string_view text) noexcept : text(text) {}

inline void Paragraph::accept(FormatVisitor* visitor) const { visitor->visit(this); }

inline std::string_view Paragraph::get_text() const noexcept { return text; }

} // namespace dnd

#endif // PARAGRAPH_HPP_
