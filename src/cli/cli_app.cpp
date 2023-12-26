#include <dnd_config.hpp>

#include "cli_app.hpp"

#include <stdexcept>

#include <cli/output/command_line_output.hpp>
#include <cli/visitors/content/display_visitor.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/species/species.hpp>
#include <core/session.hpp>
#include <core/utils/char_manipulation.hpp>
#include <core/visitors/content/list_content_visitor.hpp>

namespace dnd {

constexpr const char* separator = "================================================================================";

CliApp::CliApp() : session("last_cli_session.ini"), output() {}

void CliApp::initialize(
    const std::filesystem::path& content_directory, const std::string& campaign_name, bool testrun
) {
    Errors errors = session.set_content_directory(content_directory);
    if (!errors.ok()) {
        output.formatted_error("Invalid content directory: {}", content_directory.string());
        for (const ValidationError& e : errors.get_validation_errors()) {
            output.error(e.get_error_message());
        }
        if (testrun) {
            throw std::invalid_argument("Invalid content directory.");
        }
        std::string content_dir_str;
        std::filesystem::path content_dir;
        while (!errors.ok()) {
            output.prompt_input("\nEnter a valid content directory:", content_dir_str);
            content_dir = std::filesystem::path(content_dir_str);
            errors = session.set_content_directory(content_dir);
            if (!errors.ok()) {
                output.formatted_error("Invalid content directory: {}", content_dir.string());
                for (const ValidationError& e : errors.get_validation_errors()) {
                    output.error(e.get_error_message());
                }
            }
        }
    }

    errors = session.set_campaign_name(campaign_name);
    if (!errors.ok()) {
        output.formatted_error("Invalid campaign name: {}", campaign_name);
        for (const ValidationError& e : errors.get_validation_errors()) {
            output.error(e.get_error_message());
        }
        if (testrun) {
            throw std::invalid_argument("Invalid campaign name.");
        }
        std::string campaign_name_str;
        while (!errors.ok()) {
            output.prompt_input("\nEnter a valid campaign name:", campaign_name_str);
            errors = session.set_campaign_name(campaign_name_str);
            if (!errors.ok()) {
                output.formatted_error("Invalid campaign name: {}", campaign_name_str);
                for (const ValidationError& e : errors.get_validation_errors()) {
                    output.error(e.get_error_message());
                }
            }
        }
    }

    output.formatted_text("Content directory: {}", session.get_content_directory().string());
    output.formatted_text("Campaign name:     {}", session.get_campaign_name());
    assert(session.get_status() == SessionStatus::PARSING);
    output.text("Parsing...");
    while (!session.parsing_result_available()) {
    }
    if (session.get_status() == SessionStatus::UNKNOWN_ERROR) {
        output.error("Unknown error occurred while parsing:");
        for (const std::string& error_message : session.get_unknown_error_messages()) {
            output.error(error_message);
        }
        throw std::runtime_error("Unknown error occurred while parsing.");
    }
    assert(session.get_status() == SessionStatus::READY);
    output.text(session.get_content().status());
    output.text("Ready.");
}

void CliApp::run() {
    std::string command_input;
    while (true) {
        output.text(separator);
        output.text("What do you wanna do?");
        output.prompt_input(
            "[S]earch content by name\n[L]ist all content of a type\n[V]iew open content pieces\n(Press Enter to exit)",
            command_input
        );
        if (command_input.empty()) {
            return;
        }
        char command = char_to_uppercase(command_input[0]);
        switch (command) {
            case 'X':
                return;
            case 'S':
                search_content_by_name();
                break;
            case 'L':
                list_all_content_of_a_type();
                break;
            case 'V':
                view_open_content_pieces();
                break;
            default:
                output.error("Unknown command.");
                break;
        }
    }
}

constexpr std::array<bool, 9> search_options = {true, true, true, true, true, true, true, true, true};

void CliApp::search_content_by_name() {
    std::string search_query = "?";
    while (true) {
        output.text(separator);
        output.prompt_input("Search (Press Enter to exit):", search_query);
        if (search_query.empty()) {
            return;
        }
        session.set_fuzzy_search(search_query, search_options);
        std::vector<std::string> results = session.get_fuzzy_search_result_strings();
        if (results.empty()) {
            output.formatted_text("{}", session.get_fuzzy_search_result_count());
            output.text("No results.");
        } else {
            for (size_t i = 0; i < results.size(); ++i) {
                output.formatted_text("{:>4} -- {}", i + 1, results[i]);
            }
            std::string index_str;
            output.prompt_input(
                "Enter the number for the content piece you want to open (Press Enter for new search):", index_str
            );
            if (index_str.empty()) {
                continue;
            }
            size_t index = std::stoul(index_str) - 1;
            if (index >= results.size()) {
                output.error("Invalid number.");
                continue;
            }
            session.open_fuzzy_search_result(index);
            std::deque<const ContentPiece*>& open_content_pieces = session.get_open_content_pieces();
            const ContentPiece* new_content_piece = open_content_pieces.back();
            display_content_piece(new_content_piece);
            std::string save;
            output.prompt_input(
                "Press 'S' to save this content piece for quick access later. Press any other key to continue.", save
            );
            if (save != "S") {
                open_content_pieces.pop_back();
            }
        }
    }
}

void CliApp::list_all_content_of_a_type() {
    std::string type_str = "?";
    while (true) {
        output.text(separator);
        output.text("[0] Characters");
        output.text("[1] Classes");
        output.text("[2] Subclasses");
        output.text("[3] Species");
        output.text("[4] Subspecies");
        output.text("[5] Spells");
        output.text("[6] Items");
        output.text("[7] Features");
        output.text("[8] Choosables");
        output.prompt_input("Type (Press Enter to exit):", type_str);
        if (type_str.empty()) {
            return;
        }
        int type = std::stoi(type_str);
        if (type < 0 || type > 8) {
            output.error("Invalid type.");
            continue;
        }
        ListContentVisitor visitor;
        switch (type) {
            case 0:
                for (const auto& [_, character] : session.get_content().get_characters().get_all()) {
                    character.accept_visitor(visitor);
                }
                break;
            case 1:
                for (const auto& [_, cls] : session.get_content().get_classes().get_all()) {
                    cls.accept_visitor(visitor);
                }
                break;
            case 2:
                for (const auto& [_, subclass] : session.get_content().get_subclasses().get_all()) {
                    subclass.accept_visitor(visitor);
                }
                break;
            case 3:
                for (const auto& [_, species] : session.get_content().get_species().get_all()) {
                    species.accept_visitor(visitor);
                }
                break;
            case 4:
                for (const auto& [_, subspecies] : session.get_content().get_subspecies().get_all()) {
                    subspecies.accept_visitor(visitor);
                }
                break;
            case 5:
                for (const auto& [_, spell] : session.get_content().get_spells().get_all()) {
                    spell.accept_visitor(visitor);
                }
                break;
            case 6:
                for (const auto& [_, item] : session.get_content().get_items().get_all()) {
                    item.accept_visitor(visitor);
                }
                break;
            case 7:
                for (const auto& [_, feature] : session.get_content().get_features().get_all()) {
                    feature.get().accept_visitor(visitor);
                }
                break;
            case 8:
                for (const auto& [_, choosable] : session.get_content().get_choosables().get_all()) {
                    choosable.accept_visitor(visitor);
                }
                break;
            default:
                output.error("Invalid type.");
                continue;
        }
        const std::vector<std::string> list = visitor.get_list();
        if (list.empty()) {
            output.text("No results.");
            continue;
        }
        for (size_t i = 0; i < list.size(); ++i) {
            output.formatted_text("{:>4} -- {}", i + 1, list[i]);
        }
        std::string index_str;
        output.prompt_input(
            "Enter the number for the content piece you want to open (Press Enter for new search):", index_str
        );
        if (index_str.empty()) {
            return;
        }
        size_t index = std::stoul(index_str) - 1;
        if (index >= list.size()) {
            output.error("Invalid index.");
            continue;
        }
        switch (type) {
            case 0:
                display_content_piece(&session.get_content().get_characters().get(index).value().get());
                break;
            case 1:
                display_content_piece(&session.get_content().get_classes().get(index).value().get());
                break;
            case 2:
                display_content_piece(&session.get_content().get_subclasses().get(index).value().get());
                break;
            case 3:
                display_content_piece(&session.get_content().get_species().get(index).value().get());
                break;
            case 4:
                display_content_piece(&session.get_content().get_subspecies().get(index).value().get());
                break;
            case 5:
                display_content_piece(&session.get_content().get_spells().get(index).value().get());
                break;
            case 6:
                display_content_piece(&session.get_content().get_items().get(index).value().get());
                break;
            case 7:
                display_content_piece(&session.get_content().get_features().get(index).value().get());
                break;
            case 8:
                display_content_piece(&session.get_content().get_choosables().get(index).value().get());
                break;
            default:
                output.error("Invalid type.");
                continue;
        }
    }
}

void CliApp::view_open_content_pieces() {
    std::string index_str = "?";
    while (true) {
        std::deque<const ContentPiece*>& open_content_pieces = session.get_open_content_pieces();
        if (open_content_pieces.empty()) {
            output.text("No open content pieces.");
            return;
        }
        for (size_t i = 0; i < open_content_pieces.size(); ++i) {
            output.formatted_text("{:>4} -- {}", i + 1, open_content_pieces[i]->get_name());
        }
        output.prompt_input(
            "Enter the number for the content piece you want to open (Press Enter to exit):", index_str
        );
        if (index_str.empty()) {
            return;
        }
        size_t index = std::stoul(index_str) - 1;
        if (index >= open_content_pieces.size()) {
            output.error("Invalid index.");
            continue;
        }
        display_content_piece(open_content_pieces[index]);
    }
}

void CliApp::display_content_piece(const ContentPiece* content_piece) {
    output.text(separator);
    content_piece->accept_visitor(display_visitor);
    output.text(separator);
}

} // namespace dnd
