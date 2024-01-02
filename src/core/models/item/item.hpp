#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <memory>
#include <string>

#include <core/models/content_piece.hpp>
#include <core/models/source_info.hpp>
#include <core/utils/data_result.hpp>

namespace dnd {

class ContentVisitor;

class Item : public ContentPiece {
public:
    class Data;

    static CreateResult<Item> create(Data&& item_data);

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const std::string& get_cosmetic_description() const noexcept;
    bool requires_attunement() const noexcept;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Item(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::string&& cosmetic_description, bool requires_attunement
    ) noexcept;

    std::string name;
    std::string description;
    SourceInfo source_info;
    std::string cosmetic_description;
    bool attunement;
};

class Item::Data : public ValidationData {
public:
    std::strong_ordering operator<=>(const Data&) const noexcept = default;

    std::string cosmetic_description;
    bool requires_attunement;
};

} // namespace dnd

#endif // ITEM_HPP_
