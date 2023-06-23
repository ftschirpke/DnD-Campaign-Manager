#ifndef VALIDATION_DATA_MOCK_HPP_
#define VALIDATION_DATA_MOCK_HPP_

#include <dnd_config.hpp>

#include <memory>

#include <core/validation/validation_data.hpp>

namespace dnd {

void set_valid_mock_values(ValidationData& data, const char* data_name = nullptr);

class ValidationDataMock : public ValidationData {
public:
    ValidationDataMock() = default;
    int operator<=>(const ValidationDataMock&) const noexcept = default;
    /**
     * @brief Packs the data into a ValidationData unique pointer
     * @return the packed data
     */
    virtual std::unique_ptr<ValidationData> pack() const override;
};

} // namespace dnd

#endif // VALIDATION_DATA_MOCK_HPP_
