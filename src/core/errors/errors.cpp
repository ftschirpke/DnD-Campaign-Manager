#include <dnd_config.hpp>

#include "errors.hpp"

#include <iterator>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <core/errors/parsing_error.hpp>
#include <core/errors/runtime_error.hpp>
#include <core/errors/validation_error.hpp>

namespace dnd {

Errors::Errors(Error&& error) : errors({std::move(error)}) {}

bool Errors::ok() const { return errors.empty(); }

const std::vector<Error>& Errors::get_errors() const { return errors; }

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

void Errors::add_error(Error&& error) { errors.push_back(std::move(error)); }

Errors& Errors::operator+=(Error&& error) {
    add_error(std::move(error));
    return *this;
}

Errors& Errors::operator+=(std::optional<Error>&& error) {
    if (error.has_value()) {
        add_error(std::move(error.value()));
    }
    return *this;
}

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
