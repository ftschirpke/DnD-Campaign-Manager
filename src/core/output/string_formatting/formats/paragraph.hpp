#ifndef PARAGRAPH_HPP_
#define PARAGRAPH_HPP_

#include <dnd_config.hpp>

#include <string_view>

#include <core/output/string_formatting/format_visitor.hpp>
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
    virtual void accept(const FormatVisitor& visitor) const override;
    std::string_view get_text() const noexcept;
    bool get_empty_line_after() const noexcept;
private:
    std::string_view text;
    bool empty_line_after;
};

} // namespace dnd

#endif // PARAGRAPH_HPP_
