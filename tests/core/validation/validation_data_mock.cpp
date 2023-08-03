#include <dnd_config.hpp>

#include "validation_data_mock.hpp"

#include <filesystem>
#include <memory>

#include <fmt/format.h>

#include <core/validation/validation_data.hpp>

void dndtest::set_valid_mock_values(dnd::ValidationData& data, const char* data_name) {
    if (data_name == nullptr) {
        data.name = "Name";
        data.description = "Description";
    } else {
        data.name = std::string(data_name);
        data.description = fmt::format("{} Description", data_name);
    }
    data.source_path = std::filesystem::path(DND_MOCK_DIRECTORY) / "dummy_files" / "file1.json";
}

std::unique_ptr<dnd::ValidationData> dndtest::ValidationDataMock::pack() const {
    return std::make_unique<ValidationDataMock>(*this);
}
