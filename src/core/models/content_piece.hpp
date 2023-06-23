#ifndef CONTENT_PIECE_HPP_
#define CONTENT_PIECE_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

#include <core/models/source_info.hpp>

namespace dnd {

class ContentVisitor;

class ContentPiece {
public:
    ContentPiece(
        const std::string& name, const std::string& description, const std::filesystem::path& source_path
    ) noexcept;
    ContentPiece(std::string&& name, std::string&& description, std::filesystem::path&& source_path) noexcept;
    virtual ~ContentPiece() = default;
    /**
     * @brief Accepts a visitor
     * @param visitor pointer to the visitor
     */
    virtual void accept(ContentVisitor* visitor) const = 0;
    /**
     * @brief Returns the name of the content piece
     * @return name of the content piece
     */
    const std::string& get_name() const;
    /**
     * @brief Returns the description of the content piece
     * @return description of the content piece
     */
    const std::string& get_description() const;
    /**
     * @brief Returns the information about the source of the content piece
     * @return the information about the source of the content piece
     */
    const SourceInfo& get_source_info() const;
protected:
    std::string name;
    std::string description;
    SourceInfo source_info;
};

template <typename T>
concept ContentPieceType = std::is_base_of_v<ContentPiece, T>;

} // namespace dnd

#endif // CONTENT_PIECE_HPP_
