
#include <SFML/Graphics.hpp>
#include "game_menu.hpp"
#include "game_graphic.hpp"
#include "game_core.hpp"


namespace game_runner
{
    static const std::string data_filename{"2048data.dat"};

    enum current_game_mode
    {
        MENU_MODE,
        PLAY_MODE,
        HOF_MODE
    };

    class game_runner
    {
        game_core::game_core core;
        graphic_ui::graphic_ui graphic;
        menu_2048::game_menu menu;
        current_game_mode current_mode;
        sf::RenderWindow app;

        sf::Font font;
        sf::Text hof_text;

        void new_game_button();
        void save_game_button();
        void load_game_button();
        void hof_game_button();
        void quit_game_button();
        bool escape_button_pressed(const sf::Event& event);
        void save_data();
        void load_data();
    public:
        game_runner();
        void loop();
    };
}
