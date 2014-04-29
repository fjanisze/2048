#include "simple_matrix.hpp"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <memory>
#include <random>
#include <utility>
#include <chrono>
#include <algorithm>

static const bool run_regression = false; //Set to true to build the regression code

namespace game_core
{
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

    typedef sq_matrix<int> num_container_t;

    class game_core
    {
        int points,
            last_hit,
            best_hit;
        bool game_over;
        int map_size;

        std::vector<hof_entry> hof;
        std::vector<std::pair<int,int>> random_coords;
        int random_coord_index;

        num_container_t  num_container;

        void get_random_coord(int& x,int& y);
    public:
        bool check_is_possible_to_continue();
        bool can_continue();
        void add_new_number(int amount=1);
        void action(simple_matrix::rotation_angle angle);
        int score_point(int x, int y);
        int get_score();
        void reset_board();
        std::string get_level(int pt);
        std::vector<hof_entry>& get_hof();
    };
}
