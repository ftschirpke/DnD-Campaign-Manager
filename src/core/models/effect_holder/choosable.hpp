#ifndef CHOOSABLE_HPP_
#define CHOOSABLE_HPP_

#include "dnd_config.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "models/content_piece.hpp"
#include "models/effect_holder/effect_holder.hpp"
#include "models/effect_holder/prerequisite.hpp"

namespace dnd {

/**
 * @brief A class representing feature-like objects a character can choose to have such as feats or eldritch invocations
 */
class Choosable : public ContentPiece {
public:
    /**
     * @brief Constructs a choosable with its name and description
     * @param name the name of the choosable
     * @param description a human-readable description of what the choosable provides
     */
    Choosable(
        const std::string& name, const std::filesystem::path& source_file_path, const std::string& description
    ) noexcept;
    Choosable(Choosable&& other) noexcept = default;

    // a human-readable description of what the choosable provides
    const std::string description;
    // the prerequisites a character must fulfill to be able to choose this choosable
    std::vector<std::unique_ptr<Prerequisite>> prerequisites;
    // the main part of the choosable i.e. the most general part
    EffectHolder main_part;
    // other parts of the choosable that activate on later levels or in certain conditions for example
    std::vector<EffectHolder> parts;
};

inline Choosable::Choosable(
    const std::string& name, const std::filesystem::path& source_file_path, const std::string& description
) noexcept
    : ContentPiece(name, source_file_path), description(description) {}

} // namespace dnd

#endif // CHOOSABLE_HPP_
