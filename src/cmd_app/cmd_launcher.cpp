#include <dnd_config.hpp>

#include "cmd_launcher.hpp"

#include <algorithm>
#include <filesystem>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

#include <cxxopts.hpp>

#include <cmd_app/output/command_line_output.hpp>
#include <core/controllers/content_holder.hpp>
#include <core/models/feature/choosable_feature.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/output/output.hpp>
#include <core/parsing/content_parser.hpp>
#include <core/utils/string_manipulation.hpp>

int dnd::launch(int argc, char** argv) {
    DND_MEASURE_FUNCTION();
    std::unique_ptr<Output> output = std::make_unique<CommandLineOutput>(CommandLineOutput());
    const std::filesystem::path cur_path = std::filesystem::current_path();

    cxxopts::Options options(DND_CAMPAIGN_MANAGER_NAME, DND_CAMPAIGN_MANAGER_DESCRIPTION);

    options.add_options()("c,campaign", "Name of campaign directory", cxxopts::value<std::string>())(
        "d,directory", "Content directory",
        cxxopts::value<std::string>()->default_value((cur_path / "content").string())
    )("t,testrun", "App starts and does not wait for input")("v,version", "Print version")("h,help", "Print usage");

    cxxopts::ParseResult args;
    try {
        args = options.parse(argc, argv);
    } catch (const cxxopts::exceptions::parsing& e) {
        output->formatted_error("Error: {}", e.what());
        return 1;
    }

    if (args.count("help")) {
        output->error(options.help());
        return 0;
    }
    if (args.count("version")) {
        output->formatted_error(
            "{} Version {}.{}.{}", DND_CAMPAIGN_MANAGER_NAME, DND_CAMPAIGN_MANAGER_VERSION_MAJOR,
            DND_CAMPAIGN_MANAGER_VERSION_MINOR, DND_CAMPAIGN_MANAGER_VERSION_PATCH
        );
        return 0;
    }

    if (args.count("campaign") != 1) {
        output->error("Error: Please provide exactly one campaign directory.");
        return -1;
    }
    if (args.count("directory") > 1) {
        output->error("Error: Please provide only one directory.\n");
        return -1;
    }

    try {
        DND_MEASURE_SCOPE("Main execution scope");
        const std::filesystem::path content_path(args["directory"].as<std::string>());
        const std::string campaign_dir_name = args["campaign"].as<std::string>();
        output->formatted_text("Content directory:       {}", content_path.string());
        output->formatted_text("Campaign directory name: {}", campaign_dir_name);
        if (campaign_dir_name.empty()) {
            throw std::invalid_argument("Campaign directory name cannot be \"\".");
        }
        ContentParser parser;
        ParsingResult result = parser.parse(content_path, campaign_dir_name);
        ContentHolder content = std::move(result.content);
        output->text(content.status());

        DND_MEASURE_SCOPE("Main execution scope without parsing");

        if (args.count("testrun")) {
            return 0;
        }

        bool exit = false;
        while (!exit) {
            exit = content_search(content, output.get());
        }

    } catch (const std::exception& e) {
        output->formatted_error("Error: {}", e.what());
        return -1;
    }

    return 0;
}

static std::vector<const dnd::Spell*> search_spells(
    const std::string& search, dnd::ContentHolder& content, dnd::Output* output
) {
    size_t i = 0;
    std::unordered_set<const dnd::Spell*> matched_spells = content.get_spells().prefix_get(search);
    if (matched_spells.empty()) {
        return std::vector<const dnd::Spell*>();
    }
    std::vector<const dnd::Spell*> spells_in_order(matched_spells.begin(), matched_spells.end());
    std::sort(spells_in_order.begin(), spells_in_order.end(), [](const dnd::Spell* a, const dnd::Spell* b) {
        return a->get_name() < b->get_name();
    });
    output->text("-- A -- Spells");
    for (const dnd::Spell* spell : spells_in_order) {
        output->formatted_text("- [{:>2d}] {:<30s} ({:s})", i, spell->get_name(), spell->get_type().str());
        if (++i >= 100) {
            break;
        }
    }
    return spells_in_order;
}

static std::vector<const dnd::Item*> search_items(
    const std::string& search, dnd::ContentHolder& content, dnd::Output* output
) {
    size_t i = 0;
    std::unordered_set<const dnd::Item*> matched_items = content.get_items().prefix_get(search);
    if (matched_items.empty()) {
        return std::vector<const dnd::Item*>();
    }
    std::vector<const dnd::Item*> items_in_order(matched_items.begin(), matched_items.end());
    std::sort(items_in_order.begin(), items_in_order.end(), [](const dnd::Item* a, const dnd::Item* b) {
        return a->get_name() < b->get_name();
    });
    output->text("-- B --  Items");
    for (const dnd::Item* item : items_in_order) {
        output->formatted_text("- [{:>2d}] {:<30s}", i, item->get_name());
        if (++i >= 100) {
            break;
        }
    }
    return items_in_order;
}

static std::vector<const dnd::Feature*> search_features(
    const std::string& search, dnd::ContentHolder& content, dnd::Output* output
) {
    size_t i = 0;
    std::unordered_set<const dnd::Feature*> matched_features = content.get_features().prefix_get(search);
    if (matched_features.empty()) {
        return std::vector<const dnd::Feature*>();
    }
    std::vector<const dnd::Feature*> features_in_order(matched_features.begin(), matched_features.end());
    std::sort(features_in_order.begin(), features_in_order.end(), [](const dnd::Feature* a, const dnd::Feature* b) {
        return a->get_name() < b->get_name();
    });
    output->text("-- C -- Features");
    for (const dnd::Feature* feature : features_in_order) {
        output->formatted_text("- [{:>2d}] {:<30s}", i, feature->get_name());
        if (++i >= 100) {
            break;
        }
    }
    return features_in_order;
}

static std::map<std::string, std::vector<const dnd::ChoosableFeature*>> search_choosables(
    const std::string& search, dnd::ContentHolder& content, dnd::Output* output
) {
    size_t i = 0;
    std::map<std::string, std::vector<const dnd::ChoosableFeature*>> choosables_in_order;
    std::unordered_set<const dnd::ChoosableFeature*> matched_choosables = content.get_choosable_features().prefix_get(search);
    for (auto matched_choosable : matched_choosables) {
        choosables_in_order[matched_choosable->get_type()].push_back(matched_choosable);
    }
    if (choosables_in_order.empty()) {
        return choosables_in_order;
    }
    char letter = 'D';
    for (auto& [category, ordered_choosables] : choosables_in_order) {
        std::sort(
            ordered_choosables.begin(), ordered_choosables.end(),
            [](const dnd::ChoosableFeature* a, const dnd::ChoosableFeature* b) { return a->get_name() < b->get_name(); }
        );
        if (!ordered_choosables.empty()) {
            output->formatted_text("-- {} -- {:s}", letter++, category);
        }
        for (const dnd::ChoosableFeature* choosable : ordered_choosables) {
            output->formatted_text("- [{:>2d}] {:<30s}", i, choosable->get_name());
            if (++i >= 100) {
                break;
            }
        }
    }
    return choosables_in_order;
}

bool dnd::content_search(dnd::ContentHolder& content, dnd::Output* output) {
    std::string search;
    output->prompt_input("----------------------------------------\nSearch:", search);
    output->text("");
    if (search.size() > 0 && search[0] == 'X') {
        return true;
    }
    if (search.size() < 2) {
        return false;
    }
    string_to_lowercase(search);

    const std::vector<const dnd::Spell*> spells_found = search_spells(search, content, output);
    const std::vector<const dnd::Item*> items_found = search_items(search, content, output);
    const std::vector<const dnd::Feature*> features_found = search_features(search, content, output);
    const std::map<std::string, std::vector<const dnd::ChoosableFeature*>> choosables_found = search_choosables(
        search, content, output
    );
    std::string display;
    if (spells_found.size() + items_found.size() + features_found.size() + choosables_found.size() == 0) {
        output->text("No results found.");
        return false;
    }
    bool first = true;
    bool choosing_phase = true;
    while (choosing_phase) {
        if (first) {
            output->prompt_input("\nWhich one do you want to see?", display);
            first = false;
        } else {
            output->prompt_input("\nYour request is invalid. Retry.", display);
        }
        output->text("");
        // exit
        if (display[0] == 'X') {
            break;
        }
        string_to_uppercase(display);

        char c = display[0];
        size_t idx;
        try {
            idx = static_cast<size_t>(std::stoi(display.substr(1)));
            if (c == 'A') {
                output->display(spells_found.at(idx));
            } else if (c == 'B') {
                output->display(items_found.at(idx));
            } else if (c == 'C') {
                output->display(features_found.at(idx));
            } else {
                int i = c - 'D';
                int j = 0;
                for (const auto& [choosable_group, choosables] : choosables_found) {
                    if (i == j) {
                        output->display(choosables.at(idx));
                        break;
                    }
                    ++j;
                }
            }
            choosing_phase = false;
        } catch (const std::invalid_argument& e) {
            DND_UNUSED(e);
        } catch (const std::out_of_range& e) {
            DND_UNUSED(e);
        }
    }
    return false;
}
