#include "game_graphic.hpp"
#include <fstream>
#include <chrono>
#include <sstream>

namespace graphic_ui
{

    graphic_ui::graphic_ui(game_core::game_core& game_core_ref,
                           int x_size,int y_size,int size) :core(game_core_ref),
                                                            x_s(x_size/size), y_s(y_size/size),
                                                            map_size(size),
                                                            grid_container(size)
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
                grid->text.setCharacterSize(40);
                grid->text.setColor(sf::Color::Black);
                grid->text.setPosition(sf::Vector2f(x*(x_s)+4,y*(y_s)+(y_s-grid->text.getCharacterSize())/2.3));

                grid_container.get(x,y)=grid;
            }
        }

        //Set the proper mapping
        keymap[sf::Keyboard::Up]=simple_matrix::rotation_angle::rotate_90;
        keymap[sf::Keyboard::Down]=simple_matrix::rotation_angle::rotate_270;
        keymap[sf::Keyboard::Left]=simple_matrix::rotation_angle::rotate_180;
        keymap[sf::Keyboard::Right]=simple_matrix::rotation_angle::rotate_0;

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
        if (!game_over_texture.loadFromFile("texture/over.png"))
        {
            throw std::runtime_error("HOLY SHIT! Unable to load the 'game over' texture!");
        }

        game_over_sprite.setPosition(x_size-200,y_size);
        game_over_sprite.setTexture(game_over_texture);
    }

    void graphic_ui::draw(sf::RenderWindow& rnd)
    {
        for(int y=0;y<map_size;y++)
        {
            for(int x=0;x<map_size;x++)
            {
                std::shared_ptr<single_grid> grid=grid_container.get(x,y);
                rnd.draw(grid->border);
                int n=core.get_number(x,y);
                if(n>0)
                {
                    std::stringstream ss;
                    ss<<n;
                    grid->text.setString(ss.str().c_str());
                    rnd.draw(grid->text);
                }
            }
        }

        if(!core.can_continue())
        {
            rnd.draw(game_over_sprite);
        }

        //Draw the info bar
        rnd.draw(info_bar_text);
    }

    void graphic_ui::set_info(const std::string& msg="")
    {
        if(msg.size())
        {
            info_bar_text.setString(msg.c_str());
        }
        else
        {
            const game_core::points_info& score=core.get_score();
            std::stringstream ss;
            ss<<"Points "<<score.points;
            if(score.last_hit>0)
            {
                ss<<" (+"<<score.last_hit;
                if(score.best_hit>0)
                {
                    ss<<",best: +"<<score.best_hit;
                }
                ss<<")";
            }
            ss<<" Level: "<<core.get_level(score.points);
            info_bar_text.setString(ss.str().c_str());
        }
    }

    void graphic_ui::button(const sf::Event& ev)
    {
        int key=ev.key.code;
        if(keymap.find(key)!=keymap.end())
        {
            core.action(keymap[key]);
        }
    }

    void graphic_ui::update_num_color()
    {
        sf::Color color;
        for(int y=0;y<map_size;y++)
        {
            for(int x=0;x<map_size;x++)
            {
                int n=core.get_number(x,y);
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

    void graphic_ui::trigger_event(const sf::Event& event)
    {
        switch(event.type)
        {
        case sf::Event::KeyPressed:
            if(core.can_continue())
            {
                button(event);
                update_num_color();
                set_info();
            }
            break;
        default:
            //Ignored
            ;
        }
    }

    long graphic_ui::save_data(std::ofstream& out_stream)
    {
   /*     std::function<void(const char*,std::size_t)> save([&](const char* data,std::size_t size)
        {
            out_stream.write(data,size);
        });
        save_and_load_impl(save);
        set_info("Game saved");*/
    }

    long graphic_ui::load_data(std::ifstream& in_stream)
    {
 /*       std::function<void(char*,std::size_t)> load([&](char* data,std::size_t size)
        {
            in_stream.read(data,size);
        });
        hof.clear();
        save_and_load_impl(load);
        update_num_color();
        set_info("Game loaded");*/
    }

    long graphic_ui::save_and_load_impl(std::function<void(char*,std::size_t)> operation)
    {
  /*      operation((char*)(&points),sizeof(int));
        operation((char*)(&last_hit),sizeof(int));
        operation((char*)(&best_hit),sizeof(int));
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
        }*/
    }
}

