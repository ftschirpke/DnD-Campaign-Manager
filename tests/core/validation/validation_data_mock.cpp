#include <dnd_config.hpp>

#include "validation_data_mock.hpp"

#include <memory>

#include <core/validation/validation_data.hpp>

std::unique_ptr<dnd::ValidationData> dnd::ValidationDataMock::pack() const {
    return std::make_unique<ValidationDataMock>(*this);
}
