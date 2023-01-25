#include "dnd_config.hpp"

#include "choice.hpp"

#include <algorithm>
#include <string>
#include <vector>

bool dnd::SelectionChoice::isValidDecision(const std::string& decision_str) const {
    return std::find(selection.cbegin(), selection.cend(), decision_str) != selection.cend();
}

bool dnd::GroupChoice::isValidDecision(const std::string& decision_str) const {
    for (const auto& group_name : group_names) {
        // TODO: find an implementation that does not require each GroupChoice to have access to the Groups
    }
    return false;
}
