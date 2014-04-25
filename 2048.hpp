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
            button_default_text.setPosition(position);
            button_default_text.setString(elem->button_name.c_str());
            rnd.draw(button_default_text);
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
        void click(const sf::Event& ev);
        void button(const sf::Event& ev);
    public:
        graphic_2048(int x_size,int y_size,int size);
        void draw(sf::RenderWindow& rnd);
        void action(simple_matrix::rotation_angle angle);
        void add_new_number(int amount=1);
        void update_num_color();
        bool can_continue();
        int get_score();
        void trigger_event(const sf::Event& event);
        long save_data(std::ofstream& out_stream);
        long load_data(std::ifstream& in_stream);
    };
}

namespace game_runner
{
    static const std::string data_filename{"2048data.dat"};

    enum current_game_mode
    {
        MENU_MODE,
        PLAY_MODE
    };

    class runner_2048
    {
        graphic_2048::graphic_2048 graphic;
        menu_2048::game_menu menu;
        current_game_mode current_mode;
        sf::RenderWindow app;
        void new_game_button();
        void save_game_button();
        void load_game_button();
        bool escape_button_pressed(const sf::Event& event);
    public:
        runner_2048();
        void loop();
    };
}
