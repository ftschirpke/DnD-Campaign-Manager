#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <string>

#include <core/data_result.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/source_info.hpp>
#include <core/text/text.hpp>

namespace dnd {

class ContentVisitor;

class Item : public ContentPiece {
public:
    struct Data;

    static CreateResult<Item> create(Data&& item_data);

    const std::string& get_name() const override;
    const Text& get_description() const override;
    const SourceInfo& get_source_info() const override;
    const std::string& get_cosmetic_description() const;
    bool requires_attunement() const;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Item(
        std::string&& name, Text&& description, std::filesystem::path&& source_path, std::string&& source_name,
        std::string&& cosmetic_description, bool requires_attunement
    );

    std::string name;
    Text description;
    SourceInfo source_info;
    Text cosmetic_description;
    bool attunement;
};

struct Item::Data : public ValidationData {
    std::strong_ordering operator<=>(const Data&) const = default;

    Text cosmetic_description;
    bool requires_attunement;
};

} // namespace dnd

#endif // ITEM_HPP_
