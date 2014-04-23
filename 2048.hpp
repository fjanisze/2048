#include <SFML/Graphics.hpp>
#include "simple_matrix.hpp"
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

    struct button_info
    {
        int x_position,
            y_position;
        int height,
            width;
    };

    class game_menu
    {
        int x_size,
            y_size;
        int x_menu_pos,
            y_menu_pos;

        static int menu_width,
                   menu_height;
        static button_info buttons_position[amount_of_buttons];
        sf::Texture tx_menu_bg,
                    tx_button_bg;
        sf::Sprite sprite_menu_bg,
                   sprite_button_bg;

        std::map<std::string,std::function<void()>> callback_map;
        void load_texture();
    public:
        game_menu(int window_x_size,int window_y_size);
        int add_button(const std::string& name, std::function<void()>& call_back);
        void trigger_button(const std::string& name);
        template<typename RenderClassT>
        void draw_menu(RenderClassT& rnd);
    };

    template<typename RenderClassT>
    void game_menu::draw_menu(RenderClassT& rnd)
    {
        //First the background the the buttons
        rnd.draw(sprite_menu_bg);
        //Show the buttons
        for(auto elem:buttons_position)
        {
            sprite_button_bg.setPosition(sf::Vector2f(elem.x_position,elem.y_position));
            rnd.draw(sprite_button_bg);
        }
    }
}

namespace graphic_2048
{

    struct single_grid
    {
        sf::VertexArray border;
        sf::Text text;
    };

    template<typename T>
    using sq_matrix = simple_matrix::simple_square_matrix<T>;


    class graphic_2048
    {
    public:
        typedef sq_matrix<std::shared_ptr<single_grid>> grid_container_t;
        typedef sq_matrix<int> num_container_t;
    protected: //:( for testing purpose
        int x_s,
            y_s,
            map_size;
        int points,
            last_hit;
        bool game_over;
        sf::Texture game_over_texture;
        sf::Sprite game_over_sprite;

        grid_container_t grid_container;
        num_container_t  num_container;

        std::map<int,simple_matrix::rotation_angle> keymap;
        std::map<int,sf::Color> num_colors;
        std::vector<std::pair<int,int>> random_coords;

        sf::Font font;
        sf::Color bg_default{230,230,200};
        sf::Color num_bg_default{255,100,100};
        sf::Text  info_bar_text;

        int random_coord_index;
        void get_random_coord(int& x,int& y);
        void set_info();
        bool check_is_possible_to_continue();
        int score_point(int x, int y);
    public:
        graphic_2048(int x_size,int y_size,int size);
        void draw(sf::RenderWindow& rnd);
        void click(const sf::Event& ev);
        void button(const sf::Event& ev);
        void action(simple_matrix::rotation_angle angle);
        void add_new_number(int amount=1);
        void update_num_color();
        bool can_continue();
        int get_score();
    };
}
