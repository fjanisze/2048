#include <SFML/Graphics.hpp>
#include "simple_matrix.hpp"
#include "game_core.hpp"
#include <map>
#include <vector>
#include <string>

namespace graphic_ui
{

    struct single_grid
    {
        sf::VertexArray border;
        sf::Text text;
    };

    template<typename T>
    using sq_matrix = simple_matrix::simple_square_matrix<T>;

    class graphic_ui
    {
    public:
        typedef sq_matrix<std::shared_ptr<single_grid>> grid_container_t;
    protected: //:( for testing purpose

        game_core::game_core& core;

        int x_s,
            y_s,
            map_size;

        sf::Texture game_over_texture;
        sf::Sprite game_over_sprite;

        grid_container_t grid_container;

        std::map<int,simple_matrix::rotation_angle> keymap;
        std::map<int,sf::Color> num_colors;

        sf::Font font;
        sf::Color bg_default{230,230,200};
        sf::Color num_bg_default{255,100,100};
        sf::Text  info_bar_text;

        void set_info(const std::string& msg);
        void button(const sf::Event& ev);
        long save_and_load_impl(std::function<void(char*,std::size_t)> operation);
    public:
        graphic_ui(game_core::game_core& game_core_ref,int x_size,int y_size,int size);
        void draw(sf::RenderWindow& rnd);
        void update_num_color();
        void trigger_event(const sf::Event& event);
        long save_data(std::ofstream& out_stream);
        long load_data(std::ifstream& in_stream);
    };
}
