#include <SFML/Graphics.hpp>
#include "simple_matrix.hpp"
#include <gtest.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include <memory>
#include <random>
#include <utility>
#include <cmath>

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

        grid_container_t grid_container;
        num_container_t  num_container;

        std::map<int,simple_matrix::rotation_angle> keymap;
        std::map<int,sf::Color> num_colors;
        std::vector<std::pair<int,int>> random_coords;

        sf::Font font;
        sf::Color bg_default{230,230,200};
        sf::Color num_bg_default{255,100,100};

        int random_coord_index;
        void get_random_coord(int& x,int& y);
        void update_num_color();
    public:
        graphic_2048(int x_size,int y_size,int size);
        void draw(sf::RenderWindow& rnd);
        void click(const sf::Event& ev);
        void button(const sf::Event& ev);
        bool action(simple_matrix::rotation_angle angle);
        void add_new_number(int amount=1);
    };
}
