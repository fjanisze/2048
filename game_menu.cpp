#include "2048.hpp"

namespace menu_2048
{
    button_info game_menu::buttons_position[amount_of_buttons]={
        {(menu_width-button_width)/2,10,                  button_height,button_width,""},
        {(menu_width-button_width)/2,button_height*1 + 20,button_height,button_width,""},
        {(menu_width-button_width)/2,button_height*2 + 30,button_height,button_width,""},
        {(menu_width-button_width)/2,button_height*3 + 40,button_height,button_width,""},
        {(menu_width-button_width)/2,button_height*4 + 50,button_height,button_width,""}
    };

    game_menu::game_menu(int window_x_size,int window_y_size) : next_button_index(0)
    {
        if(window_x_size<=0||
           window_y_size<=0)
            throw std::runtime_error("HOLY SHIT! The provided window size is not correct!");
        //Center the menu
        x_menu_pos=(window_x_size/2)-(menu_width/2);
        y_menu_pos=(window_y_size/2)-(menu_height/2);
        //Load the textures and se the position
        load_texture();
        //Load the font the button text
        if(!font.loadFromFile("LHANDW.TTF"))
        {
            throw std::runtime_error("HOLY SHIT! Cannot load the font!");
        }
        button_default_text.setFont(font);
        button_default_text.setCharacterSize(24);
        button_default_text.setColor(sf::Color::Black);
    }

    int game_menu::add_button(const std::string& name, std::function<void()>& call_back)
    {
        if(callback_map.find(name)==callback_map.end()&&
           callback_map.size()>=amount_of_buttons)
        {
            throw std::runtime_error("HOLY SHIT! Too much buttons!");
        }
        callback_map[name]=call_back;
        buttons_position[next_button_index].button_name=name;
        ++next_button_index;

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
            sprite_menu_bg.setTextureRect(sf::IntRect(0,0,menu_width,menu_height));
            sprite_menu_bg.setPosition(sf::Vector2f(x_menu_pos,y_menu_pos));
            sprite_button_bg.setTexture(tx_button_bg);
        }
        else throw std::runtime_error("HOLY SHIT! Not able to load the menu texture");
    }

    bool game_menu::is_in_the_tracking_area(int x,int y)
    {
        if((x>=x_menu_pos)&&(x<=(x_menu_pos+menu_width))&&
           (y>=y_menu_pos)&&(y<=(y_menu_pos+menu_height)))
            return true;
        return false;
    }
}

































