#ifndef VALIDATION_DATA_MOCK_HPP_
#define VALIDATION_DATA_MOCK_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>

#include <core/validation/validation_data.hpp>

namespace dnd::test {

void set_valid_mock_values(dnd::ValidationData& data, const char* data_name = nullptr);

class ValidationDataMock : public dnd::ValidationData {
public:
    std::strong_ordering operator<=>(const ValidationDataMock&) const noexcept = default;
};

} // namespace dnd::test

#endif // VALIDATION_DATA_MOCK_HPP_
