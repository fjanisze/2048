#include <SFML/Graphics.hpp>
#include "simple_matrix.hpp"
#include "game_core.hpp"
#include <map>
#include <vector>
#include <string>
#include "animation/animation.hpp"

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
        sf::RenderWindow& render_window;

        int x_s,
            y_s,
            map_size;

        sf::Texture game_over_texture;
        sf::Sprite game_over_sprite;

        grid_container_t grid_container;

        std::map<int,simple_matrix::rotation_angle> keymap;
        std::map<int,sf::Color> num_colors;
        std::vector<game_core::grid_mov_info> movements_info;

        std::shared_ptr<animation_engine::animation_engine> anim_engine;

        sf::Font font;
        sf::Color bg_default{230,230,200};
        sf::Color num_bg_default{255,100,100};
        sf::Text  info_bar_text;

        void button(const sf::Event& ev);
    public:
        graphic_ui(game_core::game_core& game_core_ref,sf::RenderWindow& p_render_window,int x_size,int y_size,int size,int p_frame_rate);
        void draw();
        void update_num_color();
        void trigger_event(const sf::Event& event);
        void set_info(const std::string& msg);
        sf::Color get_bg_default()
        {   return bg_default;  }
    };
}
