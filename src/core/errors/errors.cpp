#include <dnd_config.hpp>

#include "errors.hpp"

#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include <core/errors/parsing_error.hpp>
#include <core/errors/validation_error.hpp>

bool dnd::Errors::ok() const { return parsing_errors.empty() && validation_errors.empty(); }

void dnd::Errors::add_parsing_error(
    ParsingErrorCode error_code, const std::filesystem::path& filepath, std::string&& message
) {
    parsing_errors.emplace_back(error_code, std::move(filepath), std::move(message));
}

void dnd::Errors::add_parsing_error(ParsingError&& error) { parsing_errors.emplace_back(std::move(error)); }

void dnd::Errors::add_validation_error(
    ValidationErrorCode error_code, const ValidationData* validation_data, std::string&& message
) {
    validation_errors.emplace_back(error_code, validation_data, std::move(message));
}

void dnd::Errors::add_validation_error(ValidationError&& error) { validation_errors.emplace_back(std::move(error)); }

const std::vector<dnd::ParsingError>& dnd::Errors::get_parsing_errors() const { return parsing_errors; }

const std::vector<dnd::ValidationError>& dnd::Errors::get_validation_errors() const { return validation_errors; }

void dnd::Errors::merge(Errors&& other) {
    parsing_errors.insert(
        parsing_errors.end(), std::make_move_iterator(other.parsing_errors.begin()),
        std::make_move_iterator(other.parsing_errors.end())
    );
    validation_errors.insert(
        validation_errors.end(), std::make_move_iterator(other.validation_errors.begin()),
        std::make_move_iterator(other.validation_errors.end())
    );
}

dnd::Errors& dnd::Errors::operator+=(Errors&& other) {
    merge(std::move(other));
    return *this;
}
