#ifndef FUZZY_SEARCH_WINDOW_HPP_
#define FUZZY_SEARCH_WINDOW_HPP_

#include <dnd_config.hpp>

#include <string>

#include <core/session.hpp>

namespace dnd {

class FuzzySearchWindow {
public:
    FuzzySearchWindow(Session& session);
    void render();
private:
    Session& session;
    std::string search_query;
    FuzzySearchOptions search_options;
};

} // namespace dnd

#endif // FUZZY_SEARCH_WINDOW_HPP_
