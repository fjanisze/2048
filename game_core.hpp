#ifndef GAME_CORE_HPP
#define GAME_CORE_HPP

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
        int points;
        int best_hit;
        bool operator<(const hof_entry& other) const
        {
            return points<other.points;
        }
        hof_entry():points(0),best_hit(0)
        {
            memset(date,0,30);
        }
    };

    struct points_info
    {
        int points,
            last_hit,
            best_hit;
        points_info():points(0),last_hit(0),best_hit(0)
        {}
        void reset()
        {
            points=0;
            last_hit=0;
            best_hit=0;
        }
        points_info(const points_info& other):points(other.points),
                last_hit(other.last_hit),best_hit(other.best_hit)
        {   }
    };

    template<typename T>
    using sq_matrix = simple_matrix::simple_square_matrix<T>;

    typedef sq_matrix<int> num_container_t;

    class game_core
    {
        bool game_over;
        points_info game_score;
        int map_size;

        std::vector<std::pair<int,int>> random_coords;
        int random_coord_index;
        void generate_random_numbers();

        num_container_t  num_container;
        std::vector<hof_entry> hof;

        void get_random_coord(int& x,int& y);
        bool perform_the_move(simple_matrix::rotation_angle angle);
        void add_new_hof_entry();
        void save_load_common(std::function<void(char*,std::size_t)> operation);
    public:
        game_core(int board_size);
        bool check_is_possible_to_continue();
        bool can_continue();
        void add_new_number(int amount=1);
        int get_number(int x,int y);
        void set_number(int x,int y,int n);
        points_info get_score();
        void action(simple_matrix::rotation_angle angle);
        int score_point(int x, int y);
        void reset_board();

        std::string get_level(int pt);
        const std::vector<hof_entry>& get_hof();
        game_core& operator<<(std::ifstream& in);
        game_core& operator>>(std::ofstream& out);
    };
}

#endif //GAME_CORE_HPP
