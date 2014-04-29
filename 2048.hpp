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

    class runner_2048
    {
        graphic_2048::graphic_2048 graphic;
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
    public:
        runner_2048();
        void loop();
    };
}
