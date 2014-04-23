#include "2048.hpp"

namespace menu_2048
{
    static const int button_height=50;
    static const int button_width=180;

    int game_menu::menu_height{ button_height*amount_of_buttons+100 };
    int game_menu::menu_width{200};

    button_info game_menu::buttons_position[amount_of_buttons]={
        {10,10,                  button_height,button_width},
        {10,button_height*1 + 20,button_height,button_width},
        {10,button_height*2 + 30,button_height,button_width},
        {10,button_height*3 + 40,button_height,button_width},
        {10,button_height*4 + 50,button_height,button_width}
    };

    game_menu::game_menu(int window_x_size,int window_y_size)
    {
        if(window_x_size<=0||
           window_y_size<=0)
            throw std::runtime_error("HOLY SHIT! The provided window size is not correct!");
        //Center the menu
        x_menu_pos=(window_x_size/2)-(menu_width/2);
        y_menu_pos=(window_y_size/2)-(menu_height/2);
        //Load the textures and se the position
        load_texture();

    }

    int game_menu::add_button(const std::string& name, std::function<void()>& call_back)
    {
        if(callback_map.find(name)==callback_map.end()&&
           callback_map.size()>=amount_of_buttons)
        {
            throw std::runtime_error("HOLY SHIT! Too much buttons!");
        }
        callback_map[name]=call_back;
        return callback_map.size();
    }

    void game_menu::trigger_button(const std::string& name)
    {
        if(callback_map.find(name)==callback_map.end())
        {
            throw std::runtime_error("HOLY SHIT! I dont know what are you trying to trigger!");
        }
        callback_map[name]();
    }

    void game_menu::load_texture()
    {
        if(tx_menu_bg.loadFromFile("texture/menu_bg.png")&&
           tx_button_bg.loadFromFile("texture/button.png"))
        {
            sprite_menu_bg.setTexture(tx_menu_bg);
            sprite_menu_bg.setOrigin(sf::Vector2f(x_menu_pos,y_menu_pos));
            sprite_button_bg.setTexture(tx_button_bg);
        }
        else throw std::runtime_error("HOLY SHIT! Not able to load the menu texture");
    }
}

































