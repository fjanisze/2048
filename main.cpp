#include "2048.hpp"
#include <fstream>
#include <chrono>

static const bool run_regression = false; //Set to true to build the regression code

namespace graphic_2048
{

    graphic_2048::graphic_2048(int x_size,int y_size,int size) : x_s(x_size/size), y_s(y_size/size), map_size(size),
                                                                     grid_container(size), num_container(size),
                                                                     random_coord_index(0),points(0),game_over(false),
                                                                     last_hit(0)
    {
        if(!font.loadFromFile("consola.ttf"))
        {
            throw std::runtime_error("ERROR: Unable to load consola.ttf");
        }
        //Init the grid
        for(int y=0;y<map_size;y++)
        {
            for(int x=0;x<map_size;x++)
            {
                std::shared_ptr<single_grid> grid(new single_grid);
                grid->border = sf::VertexArray(sf::Quads,4);
                grid->border[0].position = sf::Vector2f( x*(x_s) , y*(y_s) );
                grid->border[1].position = sf::Vector2f( x*(x_s) + x_s , y*(y_s) );
                grid->border[2].position = sf::Vector2f( x*(x_s) + x_s , y*(y_s) + y_s );
                grid->border[3].position = sf::Vector2f( x*(x_s) , y*(y_s) + y_s );
                grid->border[0].color = bg_default;
                grid->border[1].color = bg_default;
                grid->border[2].color = bg_default;
                grid->border[3].color = bg_default;
                //grid->border[4].position = sf::Vector2f( x*(x_s) , y*(y_s) );

                grid->text.setFont(font);
                grid->text.setCharacterSize(45);
                grid->text.setColor(sf::Color::Black);
                grid->text.setPosition(sf::Vector2f(x*(x_s)+x_s/2.2,y*(y_s)+y_s/2.2));

                grid_container.get(x,y)=grid;
                num_container.get(x,y)=0;
            }
        }

        //Set the proper mapping
        keymap[sf::Keyboard::Up]=simple_matrix::rotation_angle::rotate_90;
        keymap[sf::Keyboard::Down]=simple_matrix::rotation_angle::rotate_270;
        keymap[sf::Keyboard::Left]=simple_matrix::rotation_angle::rotate_180;
        keymap[sf::Keyboard::Right]=simple_matrix::rotation_angle::rotate_0;

        //Random number generator, use the marsenne twister engine
        std::random_device rd;
        std::mt19937 en(rd()); //marsenne twister engine
        std::uniform_int_distribution<> generator{0,map_size-1};
        for(int i=0;i<10000;i++)
        {
            random_coords.push_back(std::make_pair<int,int>(generator(en),generator(en)));
        }

        //Generate the num colors
        int r=192,b=192,g=192,n=2;
        for(;n<=1<<16;n*=2)
        {
            num_colors[n]=sf::Color(r,b,g);
            if((g-=64)==0)
            {
                g=192;
                b-=64;
            }
            if(b==0)
            {
                b=192;
                r-=64;
            }
        }

        //Set properly the info bar string
        info_bar_text.setString("Make a move!");
        info_bar_text.setFont(font);
        info_bar_text.setCharacterSize(20);
        info_bar_text.setColor(sf::Color::Red);
        info_bar_text.setPosition(sf::Vector2f(10,y_size+2));

        //Game over sprite
        if (!game_over_texture.loadFromFile("over.png"))
        {
            throw std::runtime_error("HOLY SHIT! Unable to load the 'game over' texture!");
        }
        game_over_sprite.setPosition(x_size-200,y_size);
        game_over_sprite.setTexture(game_over_texture);
    }

    void graphic_2048::draw(sf::RenderWindow& rnd)
    {
        for(int y=0;y<map_size;y++)
        {
            for(int x=0;x<map_size;x++)
            {
                std::shared_ptr<single_grid> grid=grid_container.get(x,y);
                rnd.draw(grid->border);
                int n=num_container.get(x,y);
                if(n>0)
                {
                    std::stringstream ss;
                    ss<<n;
                    grid->text.setString(ss.str().c_str());
                    rnd.draw(grid->text);
                }
            }
        }

        if(game_over)
        {
            rnd.draw(game_over_sprite);
        }

        //Draw the info bar
        rnd.draw(info_bar_text);
    }

    std::string graphic_2048::get_level(int pt)
    {
        static std::string levels[]=
        {
            "Worthless!",
            "Rat excrement!",
            "Rat!",
            "Little dick!",
            "Half man!",
            "Still an half man!",
            "NSN developer!",
            "Human being!",
            "Pussy",
            "Big Pussy!",
            "Nice Pussy!",
            "Very good pussy!",
            "Ohh shit!",
            "Do it again!",
            "Chuck Norris!",
        };
        int index = pt/(256*3);
        index = index>14?14:index;
        return levels[index];
    }

    void graphic_2048::set_info()
    {
        std::stringstream ss;
        ss<<"Points "<<points;
        if(last_hit>0)
        {
            ss<<" (+"<<last_hit<<")";
        }
        ss<<" Level: "<<get_level(points);
        info_bar_text.setString(ss.str().c_str());

    }

    void graphic_2048::click(const sf::Event& ev)
    {
        if(game_over) return;
        int x = ev.mouseButton.x / x_s,
            y = ev.mouseButton.y / y_s;

        if(num_container.get(x,y)==0)
            num_container.get(x,y) = 2;
        else
        {
            points+=(num_container.get(x,y)*2);
            num_container.get(x,y) *= 2;
        }
    }

    void graphic_2048::button(const sf::Event& ev)
    {
        int key=ev.key.code;
        if(keymap.find(key)!=keymap.end())
        {
            action(keymap[key]);
        }
    }

    void graphic_2048::add_new_number(int amount)
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

    void graphic_2048::action(simple_matrix::rotation_angle angle)
    {
        num_container.rotate(angle);
        bool moved=false;
        //Now do the sum
        last_hit=0;
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
                    }
                }
            }
        }

        //Rotate back
        switch(angle)
        {
        case simple_matrix::rotation_angle::rotate_90:
            num_container.rotate(simple_matrix::rotation_angle::rotate_270);
            break;
        case simple_matrix::rotation_angle::rotate_180:
            num_container.rotate(simple_matrix::rotation_angle::rotate_180);
            break;
        case simple_matrix::rotation_angle::rotate_270:
            num_container.rotate(simple_matrix::rotation_angle::rotate_90);
            break;
        default:
            //Ignored
            ;
        };

        if(moved)
        {
            add_new_number();
        }

        //Fix the background color
        update_num_color();
        //Update the info bar
        set_info();
        //Game over?
        game_over = !check_is_possible_to_continue();
        if(game_over)
        {
            //Save a new entry in the hall of fame
            std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
            time_t tt;
            tt=std::chrono::system_clock::to_time_t (today);
            const char* cur_time=ctime(&tt);

            hof_entry new_hof;
            strncpy(new_hof.date,cur_time,strlen(cur_time)-1);
            new_hof.points=points;

            hof.push_back(new_hof);
        }
    }

    int graphic_2048::score_point(int x, int y)
    {
        last_hit+=num_container.get(x,y)*2;
        points+=last_hit;
        num_container.get(x,y)*=2;
        return points;
    }

    int graphic_2048::get_score()
    {
        return points;
    }

    bool graphic_2048::check_is_possible_to_continue()
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

    void graphic_2048::update_num_color()
    {
        sf::Color color;
        for(int y=0;y<map_size;y++)
        {
            for(int x=0;x<map_size;x++)
            {
                int n=num_container.get(x,y);
                color=bg_default;
                if(n>0)
                {
                    color=num_colors[n];
                }
                for(int i=0;i<map_size;i++)
                    grid_container.get(x,y)->border[i].color=color;
            }
        }
    }

    void graphic_2048::get_random_coord(int& x,int& y)
    {
        if(random_coord_index>=random_coords.size())
            random_coord_index=0;
        x=random_coords[random_coord_index].first;
        y=random_coords[random_coord_index].second;
        ++random_coord_index;
    }

    bool graphic_2048::can_continue()
    {
        return !game_over;
    }

    void graphic_2048::trigger_event(const sf::Event& event)
    {
        switch(event.type)
        {
        case sf::Event::MouseButtonPressed:
            click(event);
            break;
        case sf::Event::KeyPressed:
            if(can_continue())
            {
                button(event);
            }
            break;
        default:
            //Ignored
            ;
        }
    }

    void graphic_2048::reset_board()
    {
        for(int y=0;y<map_size;y++)
        {
            for(int x=0;x<map_size;x++)
            {
                num_container.get(x,y)=0;
            }
        }
        points=last_hit=0;
        game_over=false;
        add_new_number(2);
        update_num_color();
        set_info();
    }

    long graphic_2048::save_data(std::ofstream& out_stream)
    {
        std::function<void(const char*,std::size_t)> save([&](const char* data,std::size_t size)
        {
            out_stream.write(data,size);
        });
        save_and_load_impl(save);
    }

    long graphic_2048::load_data(std::ifstream& in_stream)
    {
        std::function<void(char*,std::size_t)> load([&](char* data,std::size_t size)
        {
            in_stream.read(data,size);
        });
        hof.clear();
        save_and_load_impl(load);
        update_num_color();
        set_info();
    }

    long graphic_2048::save_and_load_impl(std::function<void(char*,std::size_t)> operation)
    {
        operation((char*)(&points),sizeof(int));
        operation((char*)(&last_hit),sizeof(int));
        operation((char*)(&game_over),sizeof(bool));
        std::size_t pos_type_size=sizeof(decltype(grid_container.get(0,0)->border[0].position.x));
        for(int y=0;y<map_size;y++)
        {
            for(int x=0;x<map_size;x++)
            {
                std::shared_ptr<single_grid> grid=grid_container.get(x,y);
                sf::VertexArray* vertex=&grid->border;
                for(int i=0;i<4;i++)
                {
                    operation((char*)(&((*vertex)[i].position.x)),pos_type_size);
                    operation((char*)(&((*vertex)[i].position.y)),pos_type_size);
                }
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

    std::vector<hof_entry>& graphic_2048::get_hof()
    {
        std::sort(hof.rbegin(),hof.rend());
        return hof;
    }
}

namespace game_runner
{
    runner_2048::runner_2048() : graphic(800,600,4),
                                 menu(800,600)
    {
        current_mode=current_game_mode::MENU_MODE;//By default
        app.create(sf::VideoMode(800, 630), "2048");

        graphic.add_new_number(2);
        graphic.update_num_color();

        if(!font.loadFromFile("consola.ttf"))
        {
            throw std::runtime_error("ERROR: Unable to load consola.ttf");
        }

        hof_text.setFont(font);
        hof_text.setColor(sf::Color::Red);
        hof_text.setCharacterSize(20);

        std::function<void()> trigger_new_game([this](){new_game_button();});
        std::function<void()> trigger_save_game([this](){save_game_button();});
        std::function<void()> trigger_load_game([this](){load_game_button();});
        std::function<void()> trigger_hof_game([this](){hof_game_button();});
        //Add the menu buttons
        menu.add_button("New game",trigger_new_game);
        menu.add_button("Load game",trigger_load_game);
        menu.add_button("Save game",trigger_save_game);
        menu.add_button("Hall of fame",trigger_hof_game);
    }

    void runner_2048::new_game_button()
    {
        current_mode=current_game_mode::PLAY_MODE;
        graphic.reset_board();
    }

    void runner_2048::load_game_button()
    {
        std::ifstream in_file{data_filename,std::ios_base::binary};
        if(in_file)
        {
            graphic.load_data(in_file);
            in_file.close();
        }
    }

    void runner_2048::save_game_button()
    {
        std::ofstream out_file{data_filename,std::ios_base::binary};
        if(out_file)
        {
            graphic.save_data(out_file);
            out_file.close();
        }
    }

    void runner_2048::hof_game_button()
    {
        current_mode=current_game_mode::HOF_MODE;
    }

    void runner_2048::loop()
    {
        // Start the game loop
        while (app.isOpen())
        {
            // Process the events
            sf::Event event;

            while (app.pollEvent(event))
            {
                //Generic events
                switch(event.type)
                {
                case sf::Event::Closed:
                    app.close();
                    break;
                default:
                    //Ignored
                    ;
                };
                //Specific events
                if(current_mode==current_game_mode::PLAY_MODE)
                {
                    if(!escape_button_pressed(event))
                    {
                        graphic.trigger_event(event);
                    }
                    else
                    {
                        current_mode=current_game_mode::MENU_MODE;
                    }
                }
                else if(current_mode==current_game_mode::MENU_MODE)
                {
                    if(escape_button_pressed(event))
                    {
                        current_mode=current_game_mode::PLAY_MODE;
                    }
                    else
                    {
                        menu.trigger_event(event);
                    }
                }else
                {
                    if(escape_button_pressed(event))
                    {
                        current_mode=current_game_mode::MENU_MODE;
                    }
                }
            }

            // Clear screen
            app.clear(sf::Color::Black);
            if(current_mode==current_game_mode::PLAY_MODE)
            {
                graphic.draw(app);
            }
            else if(current_mode==current_game_mode::MENU_MODE)
            {
                graphic.draw(app);
                menu.draw_menu(app);
            }
            else
            {
                //Show the hall of fame.
                int x_pos = 30,y_pos = 30;
                for(auto elem:graphic.get_hof())
                {
                    std::stringstream ss;
                    ss<<elem.date<<", Points: "<<elem.points<<", Level: "<<graphic.get_level(elem.points);

                    hof_text.setString(ss.str().c_str());
                    hof_text.setPosition(x_pos,y_pos);

                    app.draw(hof_text);
                    y_pos+=hof_text.getCharacterSize()+10;
                }
            }
            // Update the window
            app.display();
        }
    }

    bool runner_2048::escape_button_pressed(const sf::Event& event)
    {
        if((event.type==sf::Event::KeyPressed)&&
           (event.key.code==sf::Keyboard::Escape))
           return true;
        return false;
    }

}

int main(int argc,char** argv)
{
    if(run_regression)
    {
        ::testing::InitGoogleMock(&argc,argv);
        return RUN_ALL_TESTS();
    }
    game_runner::runner_2048 runner;
    runner.loop();

    return EXIT_SUCCESS;
}
