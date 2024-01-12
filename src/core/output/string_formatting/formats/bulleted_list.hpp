#ifndef BULLETED_LIST_HPP_
#define BULLETED_LIST_HPP_

#include <dnd_config.hpp>

#include <string_view>
#include <vector>

#include <core/output/string_formatting/formats/format.hpp>

namespace dnd {

class BulletedList : public Format {
public:
    explicit BulletedList();
    void add_item(std::string_view item);
    std::vector<std::string_view> get_items() const;
    virtual void accept(const FormatVisitor& visitor) const override;
private:
    std::vector<std::string_view> items;
};

} // namespace dnd

#endif // BULLETED_LIST_HPP_
