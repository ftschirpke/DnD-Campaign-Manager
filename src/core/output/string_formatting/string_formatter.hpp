#ifndef STRING_FORMATTER_HPP_
#define STRING_FORMATTER_HPP_

#include "dnd_config.hpp"

#include <memory>
#include <string>
#include <vector>

#include "core/output/string_formatting/formats/format.hpp"

namespace dnd {

class StringFormatter {
public:
    /**
     * @brief Initialize a new String Formatter object
     */
    explicit StringFormatter() noexcept;
    /**
     * @brief Parse the given text into a vector of formats
     * @param text the text to find the formats in
     * @return a vector of formats
     */
    std::vector<std::unique_ptr<Format>> parse_formats(std::string text) const;
};

StringFormatter::StringFormatter(bool tables, bool bulleted_lists) noexcept
    : tables(tables), bulleted_lists(bulleted_lists) {}

} // namespace dnd

#endif // STRING_FORMATTER_HPP_
