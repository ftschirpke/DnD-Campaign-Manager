#include <dnd_config.hpp>

#include "fuzzy_string_search.hpp"

#include <cassert>
#include <string>
#include <vector>

#include <iostream>

#include <core/utils/string_manipulation.hpp>

struct LocalMaximum {
    size_t index;
    int64_t value;
};

namespace dnd {

static constexpr int64_t similarity_value = 3;
static constexpr int64_t mismatch_penalty = 10;
static constexpr int64_t gap_penalty = 0;

// implementation of fuzzy string matching using the Smith-Waterman algorithm
// see https://en.wikipedia.org/wiki/Smith%E2%80%93Waterman_algorithm for details
int64_t fuzzy_match_string(const std::string& search_query, const std::string& string_to_match) {
    std::string query = string_lowercase_copy(search_query);
    std::string target = string_lowercase_copy(string_to_match);

    if (query.empty()) {
        return 0;
    }
    size_t query_length = query.size();
    size_t string_length = target.size();
    if (query_length > string_length) {
        return 0;
    }

    int64_t max_score = 0;
    std::vector<int64_t> scoring_matrix((query_length + 1) * (string_length + 1), 0);
    std::vector<LocalMaximum> column_maxima(string_length + 1, LocalMaximum{.index = 0, .value = 0});
    std::vector<LocalMaximum> row_maxima(query_length + 1, LocalMaximum{.index = 0, .value = 0});

    // start iterating in the row with index 1
    size_t index = string_length + 1;
    for (size_t row = 1; row <= query_length; row++) {
        // in each row, start in the column with index 1
        assert(index / (string_length + 1) == row);
        assert(index % (string_length + 1) == 0);
        index++;
        assert(index % (string_length + 1) == 1);

        for (size_t col = 1; col <= string_length; col++) {
            assert(row * (string_length + 1) + col == index);

            int64_t prev_row_prev_col = scoring_matrix[index - string_length - 1];
            int64_t similarity = (query[row - 1] == target[col - 1]) ? similarity_value : -similarity_value;
            int64_t similarity_score = prev_row_prev_col + similarity;
            if (similarity_score > scoring_matrix[index]) {
                scoring_matrix[index] = similarity_score;
            }

            LocalMaximum& column_maximum = column_maxima[col];
            int64_t mismatch_gap_score = column_maximum.value
                                         - mismatch_penalty * static_cast<int64_t>(row - column_maximum.index);
            if (mismatch_gap_score > scoring_matrix[index]) {
                scoring_matrix[index] = mismatch_gap_score;
            }

            LocalMaximum& row_maximum = row_maxima[row];
            int64_t omission_gap_score = row_maximum.value
                                         - gap_penalty * static_cast<int64_t>(col - row_maximum.index);
            if (omission_gap_score > scoring_matrix[index]) {
                scoring_matrix[index] = omission_gap_score;
            }

            bool is_new_column_maximum = scoring_matrix[index] > mismatch_gap_score;
            bool is_new_row_maximum = scoring_matrix[index] > omission_gap_score;
            if (is_new_column_maximum) {
                column_maximum.index = row;
                column_maximum.value = scoring_matrix[index];
            }
            if (is_new_row_maximum) {
                row_maximum.index = col;
                row_maximum.value = scoring_matrix[index];
            }

            index++;
        }
    }

    index = 0;
    for (size_t row = 0; row <= query_length; row++) {
        for (size_t col = 0; col <= string_length; col++) {
            std::cout << scoring_matrix[index] << ", ";
            index++;
        }
        std::cout << '\n';
    }

    max_score = scoring_matrix[scoring_matrix.size() - 1];
    max_score -= static_cast<int64_t>(query_length);

    std::cout << query << " in " << target << " = " << max_score << '\n';

    return max_score;
}

} // namespace dnd
