#ifndef VALIDATION_DATA_MOCK_HPP_
#define VALIDATION_DATA_MOCK_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>

#include <core/validation/validation_data.hpp>

namespace dndtest {

void set_valid_mock_values(dnd::ValidationData& data, const char* data_name = nullptr);

class ValidationDataMock : public dnd::ValidationData {
public:
    ValidationDataMock() = default;
    std::strong_ordering operator<=>(const ValidationDataMock&) const noexcept = default;
    /**
     * @brief Packs the data into a ValidationData unique pointer
     * @return the packed data
     */
    virtual std::unique_ptr<dnd::ValidationData> pack() const override;
};

} // namespace dndtest

#endif // VALIDATION_DATA_MOCK_HPP_
