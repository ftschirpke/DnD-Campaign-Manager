#ifndef VALIDATION_EXCEPTIONS_HPP_
#define VALIDATION_EXCEPTIONS_HPP_

#include <dnd_config.hpp>

#include <stdexcept>

namespace dnd {

class invalid_data : public std::invalid_argument {
public:
    invalid_data(const std::string& message);
    invalid_data(const char* message);
};

} // namespace dnd

#endif // VALIDATION_EXCEPTIONS_HPP_
