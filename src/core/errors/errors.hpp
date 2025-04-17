#ifndef ERRORS_HPP_
#define ERRORS_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <core/errors/parsing_error.hpp>
#include <core/errors/runtime_error.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

using Error = std::variant<ParsingError, ValidationError, RuntimeError>;

class Errors {
public:
    Errors() = default;
    Errors(Error&& error);

    Errors(const Errors&) = default;
    Errors& operator=(const Errors&) = default;
    Errors(Errors&&) noexcept = default;
    Errors& operator=(Errors&&) noexcept = default;

    bool ok() const;
    const std::vector<Error>& get_errors() const;

    void add_parsing_error(ParsingError::Code error_code, const std::filesystem::path& filepath, std::string&& message);
    void add_parsing_error(ParsingError&& error);
    void add_validation_error(ValidationError::Code error_code, std::string&& message);
    void add_validation_error(ValidationError&& error);
    void add_runtime_error(RuntimeError::Code error_code, std::string&& message);
    void add_runtime_error(RuntimeError&& error);

    void add_error(Error&& error);
    Errors& operator+=(Error&& error);
    Errors& operator+=(std::optional<Error>&& error);

    void merge(Errors&& other);
    Errors& operator+=(Errors&& other);
private:
    std::vector<Error> errors;
};

template <typename T>
struct WithErrors {
    T value;
    Errors errors;

    bool ok() const;
    void move_into(T& value_target, Errors& errors_target);
};

template <typename T>
inline bool WithErrors<T>::ok() const {
    return errors.ok();
}

template <typename T>
inline void WithErrors<T>::move_into(T& value_target, Errors& errors_target) {
    value_target = std::move(value);
    errors_target += std::move(errors);
}

} // namespace dnd

#endif // ERRORS_HPP_
