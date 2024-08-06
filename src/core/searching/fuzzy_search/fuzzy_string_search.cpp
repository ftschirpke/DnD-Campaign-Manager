#include <dnd_config.hpp>

#include "fuzzy_string_search.hpp"

#include <array>
#include <cassert>
#include <cctype>
#include <string>
#include <vector>

#include <iostream>

#include <core/utils/char_manipulation.hpp>
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
int64_t fuzzy_match_string_old(const std::string& search_query, const std::string& string_to_match) {
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

enum class CharType {
    LOWER_CHAR,
    UPPER_CHAR,
    DIGIT,
    WHITESPACE,
    DELIMITER,
    NON_WORD,
};

constexpr std::array<char, 5> delimiter_chars = {'-', '(', ')', ':', ','};

constexpr int16_t SCORE_MATCH = 16;
constexpr int16_t BONUS_FIRST = 1;
constexpr int16_t BONUS_BOUNDARY = 8;
constexpr int16_t BONUS_BOUNDARY_WHITESPACE = 10;
constexpr int16_t BONUS_BOUNDARY_DELIMITER = 9;
constexpr int16_t BONUS_NON_WORD = 8;
constexpr int16_t BONUS_CAMEL_CASE = 7;
constexpr int16_t BONUS_CONSECUTIVE = 8;
constexpr int16_t SCORE_GAP_START = -3;
constexpr int16_t SCORE_GAP_EXTENSION = -1;

static CharType char_type(char c) {
    if (std::isalpha(c)) {
        if (std::isupper(c)) {
            return CharType::UPPER_CHAR;
        } else {
            return CharType::LOWER_CHAR;
        }
    } else if (std::isdigit(c)) {
        return CharType::DIGIT;
    } else if (c == '\'') {
        return CharType::LOWER_CHAR; // treat apostrophe as normal character (as it is often used in D&D words)
    } else if (std::isspace(c)) {
        return CharType::WHITESPACE;
    } else if (std::find(delimiter_chars.cbegin(), delimiter_chars.cend(), c) != delimiter_chars.cend()) {
        return CharType::DELIMITER;
    } else {
        return CharType::NON_WORD;
    }
}

int16_t bonus_for_types(CharType previous_type, CharType type) {
    if (type != CharType::NON_WORD) {
        switch (previous_type) {
            case CharType::WHITESPACE:
                return BONUS_BOUNDARY_WHITESPACE;
            case CharType::DELIMITER:
                return BONUS_BOUNDARY_DELIMITER;
            case CharType::NON_WORD:
                return BONUS_NON_WORD;
            default:
                break;
        }
    }

    if ((previous_type == CharType::LOWER_CHAR && type == CharType::UPPER_CHAR)
        || (previous_type != CharType::DIGIT && type == CharType::DIGIT)) {
        return BONUS_CAMEL_CASE;
    }

    switch (type) {
        case CharType::NON_WORD:
            [[fallthrough]];
        case CharType::DELIMITER:
            return BONUS_NON_WORD;
        case CharType::WHITESPACE:
            return BONUS_BOUNDARY_WHITESPACE;
        default:
            return 0;
    }
}

// fuzzy search implementation heavily inspired by fzf's algorithm
// see https://github.com/junegunn/fzf/blob/db01e7dab65423cd1d14e15f5b15dfaabe760283/src/algo/algo.go#L432
int64_t fuzzy_match_string(const std::string& search_query, const std::string& string_to_match) {
    if (search_query.empty() || string_to_match.empty()) {
        return 0;
    }
    size_t query_len = search_query.size();
    size_t string_len = string_to_match.size();
    if (query_len > string_len) {
        return 0;
    }

    size_t min_idx = 0;
    for (char c : string_to_match) {
        if (char_to_lowercase(c) == char_to_lowercase(search_query[0])) {
            break;
        }
        min_idx++;
    }
    if (min_idx == string_len) {
        return 0;
    }

    size_t max_idx = string_len;
    for (size_t i = string_len - 1; i > 0; --i) {
        char c = string_to_match[i];
        if (char_to_lowercase(c) == char_to_lowercase(search_query[query_len - 1])) {
            break;
        }
        max_idx = i;
    }
    if (min_idx >= max_idx) {
        return 0;
    }

    size_t range_len = max_idx - min_idx;

    std::vector<int16_t> initial_scores(range_len);
    std::vector<int16_t> initial_occupation(range_len);
    std::vector<int16_t> bonus_points(range_len);
    std::vector<size_t> first_occurences(query_len);
    std::string search_range = string_to_match.substr(min_idx, range_len);
    string_lowercase_inplace(search_range);

    int16_t max_score = 0;
    size_t max_score_idx = 0;

    char first_query_char = char_to_lowercase(search_query[0]);
    char query_char = char_to_lowercase(search_query[0]);
    int16_t previous_inital_bonus = 0;
    CharType previous_type = CharType::WHITESPACE;
    bool in_gap = false;

    size_t query_idx = 0;
    size_t last_idx = 0;

    // calculate bonus values and initial scores
    for (size_t i = 0; i < range_len; ++i) {
        char c = search_range[i];
        CharType type = char_type(c);
        if (type == CharType::UPPER_CHAR) {
            c = char_to_lowercase(c);
            search_range[i] = c;
        }
        int16_t bonus = bonus_for_types(previous_type, type);
        bonus_points[i] = bonus;
        previous_type = type;

        if (c == query_char) {
            if (query_idx < query_len) {
                first_occurences[query_idx] = i;
                query_idx++;
                query_char = search_query[std::min(query_idx, query_len - 1)];
                query_char = char_to_lowercase(query_char);
            }
            last_idx = i;
        }

        if (c == first_query_char) {
            int16_t score = SCORE_MATCH + bonus * 2;
            initial_scores[i] = score;
            initial_occupation[i] = 1;
            if (range_len == 1 && score > max_score) {
                max_score = score;
                max_score_idx = i;
                if (bonus >= BONUS_BOUNDARY) {
                    break;
                }
            }
            in_gap = false;
        } else {
            int16_t gap_penalty = in_gap ? SCORE_GAP_EXTENSION : SCORE_GAP_START;
            initial_scores[i] = std::max(previous_inital_bonus + gap_penalty, 0);
            initial_occupation[i] = 0;
            in_gap = true;
        }
    }

    if (query_idx != query_len) { // did not match whole query
        return 0;
    }

    if (query_len == 1) {
        return static_cast<int64_t>(min_idx + max_score_idx);
    }

    size_t very_first_occurence = first_occurences[0];
    size_t match_width = last_idx - very_first_occurence + 1;
    std::vector<int16_t> scores(match_width * query_len);
    std::vector<int16_t> occupation(match_width * query_len);

    size_t idx = 0;
    for (size_t i = very_first_occurence; i <= last_idx; ++i) {
        scores[idx] = initial_scores[i];
        occupation[idx] = initial_occupation[i];
    }

    // calculate scores
    size_t occurence_count = first_occurences.size() - 1;
    for (size_t i = 0; i < occurence_count; ++i) {
        query_idx = i + 1;
        query_char = char_to_lowercase(search_query[query_idx]);
        size_t occurence = first_occurences[query_idx];
        size_t row = query_idx * match_width;
        in_gap = false;

        char* search_range_subrange = search_range.data() + occurence;
        int16_t* bonus_points_subrange = bonus_points.data() + occurence;
        int16_t* occupation_subrange = occupation.data() + row + occurence - very_first_occurence;
        int16_t* occupation_diagonal = occupation_subrange - 1 - match_width;
        int16_t* scores_subrange = scores.data() + row + occurence - very_first_occurence;
        int16_t* scores_diagonal = scores_subrange - 1 - match_width;
        int16_t* scores_left = scores_subrange - 1;
        scores_left[0] = 0;

        for (size_t j = 0; j <= last_idx - occurence; ++j) {
            char c = search_range_subrange[j];
            size_t col = j + occurence;

            int16_t score1 = 0;
            int16_t score2 = 0;
            int16_t consecutive = 0;

            int16_t gap_penalty = in_gap ? SCORE_GAP_EXTENSION : SCORE_GAP_START;
            score2 = scores_left[j] + gap_penalty;

            if (query_char == c) {
                score1 = scores_diagonal[j] + SCORE_MATCH;
                int16_t bonus = bonus_points_subrange[j];
                consecutive = occupation_diagonal[j] + 1;
                if (consecutive > 1) {
                    int16_t occupation_bonus = bonus_points[col - static_cast<size_t>(consecutive) + 1];
                    if (bonus >= BONUS_BOUNDARY && bonus > occupation_bonus) {
                        consecutive = 1;
                    } else {
                        bonus = std::max(bonus, std::max(BONUS_CONSECUTIVE, occupation_bonus));
                    }
                }
                if (score1 + bonus < score2) {
                    score1 += bonus_points_subrange[j];
                    consecutive = 0;
                } else {
                    score1 += bonus;
                }
            }
            occupation_subrange[j] = consecutive;

            in_gap = score1 < score2;
            int16_t score = std::max(static_cast<int16_t>(0), std::max(score1, score2));
            if (query_idx == query_len - 1 && score > max_score) {
                max_score = score;
                max_score_idx = col;
            }
            scores_subrange[j] = score;
        }
    }

    for (size_t i = 0; i < query_len; ++i) {
        if (char_to_lowercase(search_query[i]) == char_to_lowercase(string_to_match[i])) {
            max_score += BONUS_FIRST;
        } else {
            break;
        }
    }

    return max_score;
}

} // namespace dnd
