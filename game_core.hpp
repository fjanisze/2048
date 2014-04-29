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
    };

    template<typename T>
    using sq_matrix = simple_matrix::simple_square_matrix<T>;

    typedef sq_matrix<int> num_container_t;

    class game_core
    {
    protected:
        bool game_over;
        points_info game_score;
        int map_size;

        std::vector<hof_entry> hof;
        std::vector<std::pair<int,int>> random_coords;
        int random_coord_index;

        num_container_t  num_container;

        void get_random_coord(int& x,int& y);
        bool perform_the_move(simple_matrix::rotation_angle angle);
        void add_new_hof_entry();
    public:
        game_core(int board_size);
        bool check_is_possible_to_continue();
        bool can_continue();
        void add_new_number(int amount=1);
        int get_number(int x,int y);
        void set_number(int x,int y,int n);
        const points_info& get_score();
        void action(simple_matrix::rotation_angle angle);
        int score_point(int x, int y);
        void reset_board();

        std::string get_level(int pt);
        std::vector<hof_entry>& get_hof();
    };
}

#endif //GAME_CORE_HPP
