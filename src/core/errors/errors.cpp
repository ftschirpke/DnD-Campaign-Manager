#include <dnd_config.hpp>

#include "errors.hpp"

#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include <core/errors/parsing_error.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

Errors::Errors(Error&& error) noexcept { errors.emplace_back(std::move(error)); }

bool Errors::ok() const { return errors.empty(); }

void Errors::add_parsing_error(
    ParsingError::Code error_code, const std::filesystem::path& filepath, std::string&& message
) {
    errors.emplace_back(ParsingError(error_code, std::move(filepath), std::move(message)));
}

void Errors::add_parsing_error(ParsingError&& error) { errors.emplace_back(std::move(error)); }

void Errors::add_validation_error(
    ValidationError::Code error_code, std::shared_ptr<const ValidationData> validation_data, std::string&& message
) {
    errors.emplace_back(ValidationError(error_code, validation_data, std::move(message)));
}

void Errors::add_validation_error(ValidationError&& error) { errors.emplace_back(std::move(error)); }

void Errors::add_runtime_error(RuntimeError::Code error_code, std::string&& message) {
    errors.emplace_back(RuntimeError(error_code, std::move(message)));
}

void Errors::add_runtime_error(RuntimeError&& error) { errors.emplace_back(std::move(error)); }

const std::vector<Error>& Errors::get_errors() const noexcept { return errors; }

void Errors::merge(Errors&& other) {
    errors.insert(
        errors.end(), std::make_move_iterator(other.errors.begin()), std::make_move_iterator(other.errors.end())
    );
}

Errors& Errors::operator+=(Errors&& other) {
    merge(std::move(other));
    return *this;
}

} // namespace dnd
