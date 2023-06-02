#ifndef STRING_FORMATTER_HPP_
#define STRING_FORMATTER_HPP_

#include "dnd_config.hpp"

#include <memory>
#include <string>
#include <vector>

#include "core/output/string_formatting/formats/bulleted_list.hpp"
#include "core/output/string_formatting/formats/format.hpp"
#include "core/output/string_formatting/formats/paragraph.hpp"
#include "core/output/string_formatting/formats/table.hpp"

namespace dnd {

class StringFormatter {
public:
    /**
     * @brief Initialize a new string formatter
     * @param ignore_double_newline whether to ignore double newlines and just treat them as one newline character
     */
    explicit StringFormatter(bool ignore_double_newline = false) noexcept;
    /**
     * @brief Parse the given text into a vector of formats
     * @param text the text to find the formats in
     * @return a vector of formats
     */
    std::vector<std::unique_ptr<Format>> parse_formats(const std::string& text) const;
private:
    std::unique_ptr<Paragraph> parse_paragraph(
        std::string::const_iterator& it, const std::string::const_iterator& end_it
    ) const;
    std::unique_ptr<BulletedList> parse_bulleted_list(
        std::string::const_iterator& it, const std::string::const_iterator& end_it
    ) const;
    std::unique_ptr<Table> parse_table(std::string::const_iterator& it, const std::string::const_iterator& end_it)
        const;

    bool ignore_double_newline;
};

inline StringFormatter::StringFormatter(bool ignore_double_newline) noexcept
    : ignore_double_newline(ignore_double_newline) {}

} // namespace dnd

#endif // STRING_FORMATTER_HPP_
