#include "simple_matrix.hpp"
#include <map>
#include <vector>
#include <string>

namespace graphic_2048
{

    struct single_grid
    {
        sf::VertexArray border;
        sf::Text text;
    };

    struct hof_entry
    {
        char date[30];
        int points{0};
        int best_hit{0};
        bool operator<(const hof_entry& other) const
        {
            return points<other.points;
        }
        hof_entry()
        {
            memset(date,0,30);
        }
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
            last_hit,
            best_hit;
        std::vector<hof_entry> hof;
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
        void set_info(const std::string& msg);
        bool check_is_possible_to_continue();
        int score_point(int x, int y);
        void click(const sf::Event& ev);
        void button(const sf::Event& ev);
        long save_and_load_impl(std::function<void(char*,std::size_t)> operation);
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
        void reset_board();
        std::string get_level(int pt);
        std::vector<hof_entry>& get_hof();
    };
}
