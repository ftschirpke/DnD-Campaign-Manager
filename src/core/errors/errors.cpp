#include <dnd_config.hpp>

#include "errors.hpp"

#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include <core/errors/parsing_error.hpp>
#include <core/errors/runtime_error.hpp>
#include <core/errors/validation_error.hpp>

namespace dnd {

Errors::Errors(Error&& error) : errors({std::move(error)}) {}

bool Errors::ok() const { return errors.empty(); }

void Errors::add_parsing_error(
    ParsingError::Code error_code, const std::filesystem::path& filepath, std::string&& message
) {
    errors.push_back(ParsingError(error_code, std::move(filepath), std::move(message)));
}

void Errors::add_parsing_error(ParsingError&& error) { errors.push_back(std::move(error)); }

void Errors::add_validation_error(ValidationError::Code error_code, std::string&& message) {
    errors.push_back(ValidationError(error_code, std::move(message)));
}

void Errors::add_validation_error(ValidationError&& error) { errors.push_back(std::move(error)); }

void Errors::add_runtime_error(RuntimeError::Code error_code, std::string&& message) {
    errors.push_back(RuntimeError(error_code, std::move(message)));
}

void Errors::add_runtime_error(RuntimeError&& error) { errors.push_back(std::move(error)); }

const std::vector<Error>& Errors::get_errors() const { return errors; }

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
