#include <gmock.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include <memory>
#include <random>
#include <utility>
#include <cmath>
#include <functional>

namespace menu_2048
{
    static const int amount_of_buttons{5};
    static const int button_height{50};
    static const int button_width{180};
    static const int menu_height{ button_height*amount_of_buttons+100 };
    static const int menu_width{200};

    struct button_info
    {
        int x_position,
            y_position;
        int height,
            width;
        std::string button_name;
    };

    class game_menu
    {
        int x_menu_pos,
            y_menu_pos;

        static button_info buttons_position[amount_of_buttons];
        int next_button_index;
        sf::Texture tx_menu_bg,
                    tx_button_bg;
        sf::Sprite sprite_menu_bg,
                   sprite_button_bg;
        sf::Text button_default_text;
        sf::Font font;

        std::map<std::string,std::function<void()>> callback_map;
        void load_texture();
    public:
        game_menu(int window_x_size,int window_y_size);
        int add_button(const std::string& name, std::function<void()>& call_back);
        void trigger_button(const std::string& name);
        bool trigger_button(int x,int y);
        template<typename RenderClassT>
        void draw_menu(RenderClassT& rnd);
        void trigger_event(const sf::Event& event);
    public: //Track the mouse move actions
        bool is_in_the_tracking_area(int x,int y);
        bool is_in_the_tracking_area(const sf::Event& event);
    };

    template<typename RenderClassT>
    void game_menu::draw_menu(RenderClassT& rnd)
    {
        //First the background the the buttons
        rnd.draw(sprite_menu_bg);
        //Show the buttons
        button_info* elem;
        for(int i=0;i<next_button_index;i++)
        {
            elem=&buttons_position[i];
            sf::Vector2f position{sf::Vector2f(x_menu_pos+elem->x_position,y_menu_pos+elem->y_position)};

            sprite_button_bg.setPosition(position);
            rnd.draw(sprite_button_bg);
            //Draw the button name
            position.x+=3;
            position.y+=10;
            button_default_text.setPosition(position);
            button_default_text.setString(elem->button_name.c_str());
            rnd.draw(button_default_text);
        }
    }
}
