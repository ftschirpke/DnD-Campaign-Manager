#include <dnd_config.hpp>

#include "validation_exceptions.hpp"

#include <stdexcept>
#include <string>

dnd::invalid_data::invalid_data(const std::string& message) : std::invalid_argument(message) {}

dnd::invalid_data::invalid_data(const char* message) : std::invalid_argument(message) {}
