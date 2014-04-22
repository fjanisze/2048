#include "2048.hpp"

namespace menu_2048
{
    static const int button_height=80;

    int game_menu::menu_height{ button_height*amount_of_buttons+100 };
    int game_menu::menu_width{200};

    button_info game_menu::buttons_position[amount_of_buttons]={
        {10,10,                  button_height,180},
        {10,button_height*1 + 20,button_height,180},
        {10,button_height*2 + 30,button_height,180},
        {10,button_height*3 + 40,button_height,180},
        {10,button_height*4 + 50,button_height,180}
    };

    game_menu::game_menu(int window_x_size,int window_y_size)
    {
        //Center the menu
        x_menu_pos=(window_x_size/2)-(menu_width/2);
        y_menu_pos=(window_y_size/2)-(menu_height/2);

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
}
