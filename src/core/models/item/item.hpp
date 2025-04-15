#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <string>

#include <core/models/content_piece.hpp>
#include <core/models/source_info.hpp>
#include <core/utils/data_result.hpp>

namespace dnd {

class ContentVisitor;

class Item : public ContentPiece {
public:
    struct Data;

    static CreateResult<Item> create(Data&& item_data);

    const std::string& get_name() const override;
    const std::string& get_description() const override;
    const SourceInfo& get_source_info() const override;
    const std::string& get_cosmetic_description() const;
    bool requires_attunement() const;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Item(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::string&& source_name,
        std::string&& cosmetic_description, bool requires_attunement
    );

    std::string name;
    std::string description;
    SourceInfo source_info;
    std::string cosmetic_description;
    bool attunement;
};

struct Item::Data : public ValidationData {
    std::strong_ordering operator<=>(const Data&) const = default;

    std::string cosmetic_description;
    bool requires_attunement;
};

} // namespace dnd

#endif // ITEM_HPP_
