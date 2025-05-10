#include <dnd_config.hpp>

#include "validation_data_mock.hpp"

#include <filesystem>

#include <fmt/format.h>

#include <core/text/text.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd::test {

void set_valid_mock_values(ValidationData& data, const char* data_name) {
    if (data_name == nullptr) {
        data.name = "Name";
        data.description = Text::simple("Description");
    } else {
        data.name = std::string(data_name);
        data.description = Text::simple(fmt::format("{} Description", data_name));
    }
    data.source_path = std::filesystem::path(DND_MOCK_DIRECTORY) / "dummy_files" / "file1.json";
    data.source_name = "dummy";
}

} // namespace dnd::test
