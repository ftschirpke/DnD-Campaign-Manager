#ifndef GUI_APP_HPP_
#define GUI_APP_HPP_

#include <dnd_config.hpp>

#include <core/session.hpp>
#include <gui/gui_fonts.hpp>
#include <gui/windows/advanced_search_window.hpp>
#include <gui/windows/content_configuration_window.hpp>
#include <gui/windows/content_window.hpp>
#include <gui/windows/error_messages_window.hpp>
#include <gui/windows/fuzzy_search_window.hpp>
#include <gui/windows/pdf_create_window.hpp>

namespace dnd {

class GuiApp {
public:
    GuiApp(const GuiFonts& fonts);
    void initialize();
    void render();
private:
    void render_overview_window();
    void render_parsing_error_popup();

    bool show_demo_window;
    bool show_advanced_search_window;
    bool show_pdf_create_window;

    Session session;

    ContentConfigurationWindow content_configuration_window;
    ContentWindow content_window;
    ErrorMessagesWindow error_messages_window;
    FuzzySearchWindow fuzzy_search_window;
    AdvancedSearchWindow advanced_search_window;

    PdfCreateWindow pdf_create_window;
};

} // namespace dnd

#endif // GUI_APP_HPP_
