#include "dnd_config.hpp"

#include "cmd_launcher.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

#include <cxxopts.hpp>

#include "core/controllers/content_holder.hpp"
#include "core/models/effect_holder/choosable.hpp"
#include "core/models/effect_holder/feature.hpp"
#include "core/models/item.hpp"
#include "core/models/spell.hpp"
#include "core/output/command_line_output.hpp"
#include "core/output/output.hpp"
#include "core/parsing/controllers/content_parser.hpp"
#include "core/parsing/parsing_exceptions.hpp"

int dnd::launch(int argc, char** argv) {
    DND_MEASURE_FUNCTION();
    std::unique_ptr<Output> output = std::make_unique<CommandLineOutput>(CommandLineOutput());
    const std::filesystem::path cur_path = std::filesystem::current_path();

    cxxopts::Options options(DND_CAMPAIGN_MANAGER_NAME, DND_CAMPAIGN_MANAGER_DESCRIPTION);

    options.add_options()("c,campaign", "Name of campaign directory", cxxopts::value<std::string>())(
        "d,directory", "Content directory",
        cxxopts::value<std::string>()->default_value((cur_path / "content").string())
    )("X,X11app", "Start the app with its X11 graphical user interface"
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
        ContentHolder content = parser.parse(content_path, campaign_dir_name);
        output->text(content.printStatus());

        DND_MEASURE_SCOPE("Main execution scope without parsing");

        output->text("\n=== CHARACTER INITIALISATION ===\n");
        for (const auto& [name, _] : content.characters.get_all()) {
            output->formatted_text("# {}", name);
            content.characters.get(name).determineState();
            output->text("#\n\n");
        }

        content.finished_parsing();

        if (args.count("testrun")) {
            return 0;
        }

        bool exit = false;
        while (!exit) {
            exit = content_search(content, output.get());
        }

    } catch (const parsing_error& e) {
        output->formatted_error("Parsing Error: {}", e.what());
        return -1;
    } catch (const std::invalid_argument& e) {
        output->formatted_error("Invalid Argument: {}", e.what());
        return -1;
    } catch (const std::exception& e) {
        output->formatted_error("Error: {}", e.what());
        return -1;
    }

    return 0;
}

std::vector<const dnd::Spell*> search_spells(
    const std::string& search, dnd::ContentHolder& content, dnd::Output* output
) {
    size_t i = 0;
    std::unordered_set<const dnd::Spell*> matched_spells = content.spells.prefix_get(search);
    if (matched_spells.empty()) {
        return std::vector<const dnd::Spell*>();
    }
    std::vector<const dnd::Spell*> spells_in_order(matched_spells.begin(), matched_spells.end());
    std::sort(spells_in_order.begin(), spells_in_order.end(), [](const dnd::Spell* a, const dnd::Spell* b) {
        return a->name < b->name;
    });
    output->text("-- A -- Spells");
    for (const dnd::Spell* spell : spells_in_order) {
        output->formatted_text("- [{:>2d}] {:<30s} ({:s})", i, spell->name, spell->type.str());
        if (++i >= 100) {
            break;
        }
    }
    return spells_in_order;
}

std::vector<const dnd::Item*> search_items(
    const std::string& search, dnd::ContentHolder& content, dnd::Output* output
) {
    size_t i = 0;
    std::unordered_set<const dnd::Item*> matched_items = content.items.prefix_get(search);
    if (matched_items.empty()) {
        return std::vector<const dnd::Item*>();
    }
    std::vector<const dnd::Item*> items_in_order(matched_items.begin(), matched_items.end());
    std::sort(items_in_order.begin(), items_in_order.end(), [](const dnd::Item* a, const dnd::Item* b) {
        return a->name < b->name;
    });
    output->text("-- B --  Items");
    for (const dnd::Item* item : items_in_order) {
        output->formatted_text("- [{:>2d}] {:<30s}", i, item->name);
        if (++i >= 100) {
            break;
        }
    }
    return items_in_order;
}

std::vector<const dnd::Feature**> search_features(
    const std::string& search, dnd::ContentHolder& content, dnd::Output* output
) {
    size_t i = 0;
    std::unordered_set<const dnd::Feature**> matched_features = content.features.prefix_get(search);
    if (matched_features.empty()) {
        return std::vector<const dnd::Feature**>();
    }
    std::vector<const dnd::Feature**> features_in_order(matched_features.begin(), matched_features.end());
    std::sort(features_in_order.begin(), features_in_order.end(), [](const dnd::Feature** a, const dnd::Feature** b) {
        return (*a)->name < (*b)->name;
    });
    output->text("-- C -- Features");
    for (const dnd::Feature** feature : features_in_order) {
        output->formatted_text("- [{:>2d}] {:<30s}", i, (*feature)->name);
        if (++i >= 100) {
            break;
        }
    }
    return features_in_order;
}

std::map<std::string, std::vector<const dnd::Choosable**>> search_choosables(
    const std::string& search, dnd::ContentHolder& content, dnd::Output* output
) {
    size_t i = 0;
    std::map<std::string, std::vector<const dnd::Choosable**>> choosables_in_order;
    for (auto& [name, choosables_content] : content.choosables) {
        std::unordered_set<const dnd::Choosable**> matched_choosables = choosables_content.prefix_get(search);
        if (matched_choosables.empty()) {
            continue;
        }
        choosables_in_order[name] = std::vector<const dnd::Choosable**>(
            matched_choosables.begin(), matched_choosables.end()
        );
        std::sort(
            choosables_in_order[name].begin(), choosables_in_order[name].end(),
            [](const dnd::Choosable** a, const dnd::Choosable** b) { return (*a)->name < (*b)->name; }
        );
    }
    if (choosables_in_order.empty()) {
        return choosables_in_order;
    }
    char letter = 'D';
    for (auto& [category, ordered_choosables] : choosables_in_order) {
        if (!ordered_choosables.empty()) {
            output->formatted_text("-- {} -- {:s}", letter++, category);
        }
        for (const dnd::Choosable** choosable : ordered_choosables) {
            output->formatted_text("- [{:>2d}] {:<30s}", i, (*choosable)->name);
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
    auto tolower = [](unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); };
    std::transform(search.begin(), search.end(), search.begin(), tolower);

    const std::vector<const dnd::Spell*> spells_found = search_spells(search, content, output);
    const std::vector<const dnd::Item*> items_found = search_items(search, content, output);
    const std::vector<const dnd::Feature**> features_found = search_features(search, content, output);
    const std::map<std::string, std::vector<const dnd::Choosable**>> choosables_found = search_choosables(
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
        auto toupper = [](unsigned char c) { return static_cast<unsigned char>(std::toupper(c)); };
        std::transform(display.begin(), display.end(), display.begin(), toupper);

        char c = display[0];
        size_t idx;
        try {
            idx = static_cast<size_t>(std::stoi(display.substr(1)));
            if (c == 'A') {
                output->display(spells_found.at(idx));
            } else if (c == 'B') {
                output->display(items_found.at(idx));
            } else if (c == 'C') {
                output->display(*features_found.at(idx));
            } else {
                int i = c - 'D';
                int j = 0;
                for (const auto& [choosable_group, choosables] : choosables_found) {
                    if (i == j) {
                        output->display(*choosables.at(idx));
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
