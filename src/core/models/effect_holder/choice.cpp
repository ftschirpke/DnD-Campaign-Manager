#include "dnd_config.hpp"

#include "choice.hpp"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

bool dnd::SelectionChoice::isValidDecision(const std::string& decision_str) const {
    return std::find(selection.cbegin(), selection.cend(), decision_str) != selection.cend();
}

bool dnd::ChoosableGroupChoice::isValidDecision(const std::string& decision_str) const {
    for (auto subgroup : group_values) {
        if (subgroup->find(decision_str) != subgroup->cend()) {
            return true;
        }
    }
    return false;
}

bool dnd::StringGroupChoice::isValidDecision(const std::string& decision_str) const {
    for (auto subgroup : group_values) {
        if (std::find(subgroup->cbegin(), subgroup->cend(), decision_str) != subgroup->cend()) {
            return true;
        }
    }
    return false;
}
