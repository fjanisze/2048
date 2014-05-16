#include "game_core.hpp"

namespace game_core
{

    game_core::game_core(int board_size):map_size(board_size),game_over(false),
                                    num_container(board_size)
    {
        reset_board();
        generate_random_numbers();
    }

    void game_core::generate_random_numbers()
    {
         //Random number generator, use the marsenne twister engine
        std::random_device rd;
        std::mt19937 en(rd()); //marsenne twister engine
        std::uniform_int_distribution<> generator{0,map_size-1};
        for(int i=0;i<10000;i++)
        {
            random_coords.push_back(std::make_pair<int,int>(generator(en),generator(en)));
        }
        random_coord_index=0;
    }

    void game_core::get_random_coord(int& x,int& y)
    {
        if(random_coord_index>=random_coords.size())
            random_coord_index=0;
        x=random_coords[random_coord_index].first;
        y=random_coords[random_coord_index].second;
        ++random_coord_index;
    }

    bool game_core::can_continue()
    {
        return !game_over;
    }

    std::string game_core::get_level(int pt)
    {
        static std::string levels[]=
        {
            "Worthless!",
            "Rat excrement!",
            "Rat!",
            "Little dick!",
            "Half man!",
            "Still an half man!",
            "Man with a small dick",
            "Faceless dick!",
            "NSN developer!",
            "Human being!",
            "Justin bieber fan!",
            "Pussy",
            "Big Pussy!",
            "Nice Pussy!",
            "Very good pussy!",
            "drunken dwarf!",
            "drunken dwarf with a pussy!",
            "Failed porn actor!",
            "Pornstar actress without boobs"
        };
        int index = pt/(256*3);
        index = index>19?19:index;
        return levels[index];
    }


    void game_core::add_new_number(int amount)
    {
        if(run_regression)
        {
            //do not generate numbers in regression mode
            return;
        }
        //Generate a block
        int x,y;
        do{
            get_random_coord(x,y);
            if(num_container.get(x,y)==0)
            {
                int a,b;
                get_random_coord(a,b);
                num_container.get(x,y)=((a+b)%2?2:4);
                --amount;
            }
        }while(amount>0);
    }

    int game_core::get_number(int x,int y)
    {
        if((x<0||x>=map_size)||(y<0||y>=map_size)) throw std::runtime_error("HOLY SHIT!! out of range in get_number");
        return num_container.get(x,y);
    }

    void game_core::set_number(int x,int y,int n)
    {
        if((x<0||x>=map_size)||(y<0||y>=map_size)) throw std::runtime_error("HOLY SHIT!! out of range in set_number");
        num_container.get(x,y)=n;
    }

    bool game_core::perform_the_move(simple_matrix::rotation_angle angle)
    {
        num_container.rotate(angle);
        bool moved=false;
        //Now do the sum
        game_score.last_hit=0;
        bool skip_insertion=false;
        for(int y=0;y<map_size;y++)
        {
            int cur_x=map_size-1;
            for(int x=map_size-1;x>=0;x--)
            {
                int t=num_container.get(x,y);
                if(t!=0)
                {
                    if(cur_x==x) continue;
                    int t2=num_container.get(cur_x,y);
                    if(t2==t)
                    {
                        score_point(cur_x,y);
                        num_container.get(x,y)=0;
                        movement_info.emplace_back(grid_mov_info{x,y,cur_x,y});
                        skip_insertion=true;
                        --cur_x;
                        moved=true;
                    }
                    else
                    {
                        if(t2!=0) --cur_x;
                        num_container.get(cur_x,y)=t;
                    }
                    if(cur_x!=x)
                    {
                        num_container.get(x,y)=0;
                        moved=true;
                        if(!skip_insertion)
                        {
                            movement_info.emplace_back(grid_mov_info{x,y,cur_x,y});
                        }
                    }
                    skip_insertion=false;
                }
            }
        }

        //Rotate back
        switch(angle)
        {
        case simple_matrix::rotation_angle::rotate_90:
            num_container.rotate(simple_matrix::rotation_angle::rotate_270);
            {
                for(auto& elem:movement_info)
                {
                    //Transponse
                    std::swap(elem.x_from,elem.y_from);
                    std::swap(elem.x_to,elem.y_to);
                    //Reverse col
                    elem.y_from=map_size-elem.y_from-1;
                    elem.y_to=map_size-elem.y_to-1;
                }
            }
            break;
        case simple_matrix::rotation_angle::rotate_180:
            num_container.rotate(simple_matrix::rotation_angle::rotate_180);
            {
                for(auto& elem:movement_info)
                {
                    //Reverse row
                    elem.x_from=map_size-elem.x_from-1;
                    elem.x_to=map_size-elem.x_to-1;
                    //Reverse col
                    elem.y_from=map_size-elem.y_from-1;
                    elem.y_to=map_size-elem.y_to-1;
                }
            }
            break;
        case simple_matrix::rotation_angle::rotate_270:
            num_container.rotate(simple_matrix::rotation_angle::rotate_90);
            {
                for(auto& elem:movement_info)
                {
                    //Transponse
                    std::swap(elem.x_from,elem.y_from);
                    std::swap(elem.x_to,elem.y_to);
                    //Reverse row
                    elem.x_from=map_size-elem.x_from-1;
                    elem.x_to=map_size-elem.x_to-1;
                }
            }
            break;
        default:
            //Ignored
            ;
        };
        return moved;
    }

    void game_core::add_new_hof_entry()
    {
        //Save a new entry in the hall of fame
        std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
        time_t tt;
        tt=std::chrono::system_clock::to_time_t (today);
        const char* cur_time=ctime(&tt);

        hof_entry new_hof;
        strncpy(new_hof.date,cur_time,strlen(cur_time)-1);
        new_hof.points=game_score.points;
        new_hof.best_hit=game_score.best_hit;

        hof.push_back(new_hof);
    }


    bool game_core::action(simple_matrix::rotation_angle angle)
    {
        bool moved = perform_the_move(angle);

        if(moved)
        {
            add_new_number();
        }

        //Game over?
        game_over = !check_is_possible_to_continue();

        if(game_over)
        {
            add_new_hof_entry();
        }
        return moved;
    }

    int game_core::score_point(int x, int y)
    {
        int score = num_container.get(x,y)*2;
        game_score.last_hit+=score;
        game_score.best_hit=std::max(game_score.best_hit,game_score.last_hit);
        game_score.points+=score;
        num_container.get(x,y)*=2;
        return game_score.points;
    }

    points_info game_core::get_score()
    {
        return game_score;
    }

    bool game_core::check_is_possible_to_continue()
    {
        for(int y=0;y<map_size;y++)
        {
            for(int x=0;x<map_size;x++)
            {
                if(num_container.get(x,y)==0)
                    return true;
                if((x<(map_size-1))&&num_container.get(x,y)==num_container.get(x+1,y))
                    return true;
                if((y<(map_size-1))&&num_container.get(x,y)==num_container.get(x,y+1))
                    return true;
            }
        }
        return false;
    }


    void game_core::reset_board()
    {
        for(int y=0;y<map_size;y++)
        {
            for(int x=0;x<map_size;x++)
            {
                num_container.get(x,y)=0;
            }
        }
        game_score.reset();
        game_over=false;
    }

    std::vector<grid_mov_info> game_core::get_movement_info()
    {
        std::vector<grid_mov_info> movement_vec_copy(std::move(movement_info));
        movement_info.clear();
        return std::move(movement_vec_copy);
    }

    const std::vector<hof_entry>& game_core::get_hof()
    {
        std::sort(hof.rbegin(),hof.rend());
        return hof;
    }

    void game_core::save_load_common(std::function<void(char*,std::size_t)> operation)
    {
        operation((char*)(&game_over),sizeof(bool));
        operation((char*)(&game_score.points),sizeof(int));
        operation((char*)(&game_score.last_hit),sizeof(int));
        operation((char*)(&game_score.best_hit),sizeof(int));
        for(int y=0;y<map_size;y++)
        {
            for(int x=0;x<map_size;x++)
            {
                int& num = num_container.get(x,y);
                operation((char*)(&num),sizeof(int));
            }
        }
        //Hall of fame
        size_t size = hof.size();
        operation((char*)(&size),sizeof(size_t));
        if(hof.empty()&&size>0)
        {
            //Seems this is a load operation, and size was set to >0 even if hof is 0
            hof.resize(size);
        }
        for(int i=0;i<size;i++)
        {
            hof_entry& entry=hof[i];
            operation((char*)(&entry),sizeof(hof_entry));
        }
    }

    game_core& game_core::operator<<(std::ifstream& in)
    {
        std::function<void(char*,std::size_t)> load([&](char* data,std::size_t size)
        {
            in.read(data,size);
        });
        save_load_common(load);
        return *this;
    }

    game_core& game_core::operator>>(std::ofstream& out)
    {
        std::function<void(const char*,std::size_t)> save([&](const char* data,std::size_t size)
        {
            out.write(data,size);
        });
        save_load_common(save);
        return *this;
    }
}
