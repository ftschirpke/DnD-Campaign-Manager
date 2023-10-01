#include <dnd_config.hpp>

#include "cli_app.hpp"

#include <iostream>
#include <stdexcept>

#include <cli/output/command_line_output.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/session.hpp>
#include <core/utils/char_manipulation.hpp>
#include <core/visitors/content/list_content_visitor.hpp>

constexpr const char* separator = "--------------------------------------------------------------------------------";

dnd::CliApp::CliApp() : session("last_cli_session.ini"), output() {}

void dnd::CliApp::initialize(const std::filesystem::path& content_directory, const std::string& campaign_name) {
    Errors errors = session.set_content_directory(content_directory);
    if (!errors.ok()) {
        for (const ValidationError& e : errors.get_validation_errors()) {
            output.error(e.get_error_message());
        }
        throw std::invalid_argument("Invalid content directory.");
    }
    errors = session.set_campaign_name(campaign_name);
    if (!errors.ok()) {
        for (const ValidationError& e : errors.get_validation_errors()) {
            output.error(e.get_error_message());
        }
        throw std::invalid_argument("Invalid campaign name.");
    }
    assert(session.get_status() == SessionStatus::PARSING);
    output.text("Parsing...");
    while (session.get_status() == SessionStatus::PARSING) {
        session.update();
    }
    if (session.get_status() == SessionStatus::UNKNOWN_ERROR) {
        throw std::runtime_error("Unknown error occurred while parsing.");
    }
    assert(session.get_status() == SessionStatus::READY);
    output.text(session.get_content().status());
    output.text("Ready.");
}

void dnd::CliApp::start() {
    std::string command_input;
    while (true) {
        output.text(separator);
        output.text("What do you wanna do?");
        output.text("[S]earch content by name\n[L]ist all content of a type\n[V]iew open content pieces\ne[X]it");
        output.prompt_input("", command_input);
        while (command_input.size() == 0) {
            output.prompt_input("", command_input);
        }
        char command = dnd::char_to_uppercase(command_input[0]);
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
                output.error("Not implemented yet."); // TODO
                break;
            default:
                output.error("Unknown command.");
                break;
        }
    }
}

constexpr std::array<bool, 9> search_options = {true, true, true, true, true, true, true, true, true};

void dnd::CliApp::search_content_by_name() {
    std::string search_query = "?";
    while (!search_query.empty()) {
        output.prompt_input("Search (leave empty to exit):", search_query);
        session.set_trie_search(search_query, search_options);
        std::vector<std::string> results = session.get_trie_search_result_strings();
        if (results.empty()) {
            output.text("No results.");
        } else {
            for (size_t i = 0; i < results.size(); ++i) {
                output.formatted_text("{:>4} -- {}", i, results[i]);
            }
            std::string index_str;
            output.prompt_input(
                "Enter the index of the content piece you want to open (leave empty for new search):", index_str
            );
            if (index_str.empty()) {
                continue;
            }
            size_t index = std::stoul(index_str);
            if (index >= results.size()) {
                output.error("Invalid index.");
                continue;
            }
            session.open_trie_search_result(index);
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

void dnd::CliApp::list_all_content_of_a_type() {
    std::string type_str = "?";
    while (!type_str.empty()) {
        output.text("[0] Characters");
        output.text("[1] Classes");
        output.text("[2] Subclasses");
        output.text("[3] Races");
        output.text("[4] Subraces");
        output.text("[5] Spells");
        output.text("[6] Items");
        output.text("[7] Features");
        output.text("[8] Choosables");
        output.prompt_input("Type (leave empty to exit):", type_str);
        int type = std::stoi(type_str);
        if (type < 0 || type > 8) {
            output.error("Invalid type.");
            continue;
        }
        ListContentVisitor visitor;
        switch (type) {
            case 0:
                for (const Character& character : session.get_content().get_characters().get_all()) {
                    character.accept(visitor);
                }
                break;
            case 1:
                for (const CharacterClass& character_class : session.get_content().get_character_classes().get_all()) {
                    character_class.accept(visitor);
                }
                break;
            case 2:
                for (const CharacterSubclass& character_subclass :
                     session.get_content().get_character_subclasses().get_all()) {
                    character_subclass.accept(visitor);
                }
                break;
            case 3:
                for (const CharacterRace& character_race : session.get_content().get_character_races().get_all()) {
                    character_race.accept(visitor);
                }
                break;
            case 4:
                for (const CharacterSubrace& character_subrace :
                     session.get_content().get_character_subraces().get_all()) {
                    character_subrace.accept(visitor);
                }
                break;
            case 5:
                for (const Spell& spell : session.get_content().get_spells().get_all()) {
                    spell.accept(visitor);
                }
                break;
            case 6:
                for (const Item& item : session.get_content().get_items().get_all()) {
                    item.accept(visitor);
                }
                break;
            case 7:
                for (const Feature* feature : session.get_content().get_features().get_all()) {
                    feature->accept(visitor);
                }
                break;
            case 8:
                for (const Choosable& choosable : session.get_content().get_choosables().get_all()) {
                    choosable.accept(visitor);
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
        for (const std::string& line : list) {
            output.formatted_text("{:>4} -- {}", line);
        }
        std::string index_str;
        output.prompt_input(
            "Enter the index of the content piece you want to open (leave empty for new search):", index_str
        );
        if (index_str.empty()) {
            return;
        }
        size_t index = std::stoul(index_str);
        if (index >= list.size()) {
            output.error("Invalid index.");
            continue;
        }
        switch (type) {
            case 0:
                display_content_piece(&session.get_content().get_characters().get(index));
                break;
            case 1:
                display_content_piece(&session.get_content().get_character_classes().get(index));
                break;
            case 2:
                display_content_piece(&session.get_content().get_character_subclasses().get(index));
                break;
            case 3:
                display_content_piece(&session.get_content().get_character_races().get(index));
                break;
            case 4:
                display_content_piece(&session.get_content().get_character_subraces().get(index));
                break;
            case 5:
                display_content_piece(&session.get_content().get_spells().get(index));
                break;
            case 6:
                display_content_piece(&session.get_content().get_items().get(index));
                break;
            case 7:
                display_content_piece(&session.get_content().get_features().get(index));
                break;
            case 8:
                display_content_piece(&session.get_content().get_choosables().get(index));
                break;
            default:
                output.error("Invalid type.");
                continue;
        }
    }
}

void dnd::CliApp::view_open_content_pieces() {
    std::string index_str = "?";
    while (!index_str.empty()) {
        std::deque<const ContentPiece*>& open_content_pieces = session.get_open_content_pieces();
        if (open_content_pieces.empty()) {
            output.text("No open content pieces.");
            return;
        }
        for (size_t i = 0; i < open_content_pieces.size(); ++i) {
            output.formatted_text("{:>4} -- {}", i, open_content_pieces[i]->get_name());
        }
        output.prompt_input("Enter the index of the content piece you want to open (leave empty to exit):", index_str);
        if (index_str.empty()) {
            return;
        }
        size_t index = std::stoul(index_str);
        if (index >= open_content_pieces.size()) {
            output.error("Invalid index.");
            continue;
        }
        display_content_piece(open_content_pieces[index]);
    }
}

void dnd::CliApp::display_content_piece(const ContentPiece* content_piece) {
    output.text(content_piece->get_name());
    output.text("TODO");
}
