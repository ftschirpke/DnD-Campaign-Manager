#ifndef ADVANCED_SEARCH_WINDOW_HPP_
#define ADVANCED_SEARCH_WINDOW_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/session.hpp>
#include <gui/visitors/filters/filter_setting_visitor.hpp>

namespace dnd {

class AdvancedSearchWindow {
public:
    AdvancedSearchWindow(Session& session);
    void render();
private:
    Session& session;
    FilterSettingVisitor filter_setting_visitor;

    std::vector<std::string> result_list;
};

} // namespace dnd

#endif // ADVANCED_SEARCH_WINDOW_HPP_
