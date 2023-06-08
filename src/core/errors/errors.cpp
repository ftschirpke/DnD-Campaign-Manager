#include <dnd_config.hpp>

#include "errors.hpp"

bool dnd::Errors::ok() const { return parsing_errors.empty() && validation_errors.empty(); }

void dnd::Errors::add_parsing_error(
    ParsingErrorCode error_code, std::filesystem::path&& filepath, std::string&& message
) {
    parsing_errors.emplace_back(error_code, std::move(filepath), std::move(message));
}

void dnd::Errors::add_parsing_error(ParsingError&& error) { parsing_errors.emplace_back(std::move(error)); }

void dnd::Errors::add_validation_error(
    ValidationErrorCode error_code, ValidationData* const validation_data, std::string&& message
) {
    validation_errors.emplace_back(error_code, validation_data, std::move(message));
}

void dnd::Errors::add_validation_error(ValidationError&& error) { validation_errors.emplace_back(std::move(error)); }

void dnd::Errors::merge(Errors&& other) {
    parsing_errors.insert(parsing_errors.end(), other.parsing_errors.begin(), other.parsing_errors.end());
    validation_errors.insert(validation_errors.end(), other.validation_errors.begin(), other.validation_errors.end());
}

const std::vector<ParsingError>& dnd::Errors::get_parsing_errors() const { return parsing_errors; }

const std::vector<ValidationError>& dnd::Errors::get_validation_errors() const { return validation_errors; }
