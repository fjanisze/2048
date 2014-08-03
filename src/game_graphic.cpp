#include "game_graphic.hpp"
#include <fstream>
#include <chrono>
#include <sstream>

namespace graphic_ui
{

    graphic_ui::graphic_ui(game_core::game_core& game_core_ref,sf::RenderWindow& p_render_window,
                           int x_size,int y_size,int size,
                           int p_frame_rate) :core(game_core_ref),
                                                            x_s(x_size/size), y_s(y_size/size),
                                                            map_size(size),
                                                            grid_container(size),
                                                            render_window(p_render_window)
    {
        if(!font.loadFromFile("../consola.ttf"))
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

                grid->text.setFont(font);
                grid->text.setCharacterSize(52);
                grid->text.setColor(sf::Color::Black);
                grid->text.setStyle(sf::Text::Bold);

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
        if (!game_over_texture.loadFromFile("../texture/over.png"))
        {
            throw std::runtime_error("HOLY SHIT! Unable to load the 'game over' texture!");
        }

        game_over_sprite.setPosition(x_size-200,y_size);
        game_over_sprite.setTexture(game_over_texture);

        //Create the animation engine instance
        anim_engine=std::shared_ptr<animation_engine::animation_engine>(new animation_engine::animation_engine(render_window,p_frame_rate));
    }

    void graphic_ui::draw()
    {
        //Any movement to draw?
        if(draw_movement)
        {
            anim_engine->draw();
            if(anim_engine->check_if_all_completed_or_stopped())
            {
                draw_movement=false;
                anim_engine->clean_up(true);
            }
        }
        else
        {
            for(int y=0;y<map_size;y++)
            {
                for(int x=0;x<map_size;x++)
                {
                    std::shared_ptr<single_grid> grid=grid_container.get(x,y);
                    render_window.draw(grid->border);
                    int n=core.get_number(x,y);
                    if(n>0)
                    {
                        std::stringstream ss;
                        ss<<n;
                        grid->text.setString(ss.str().c_str());
                        int x_offset=x_s/2-grid->text.getCharacterSize()*ss.str().size()/4,
                            y_offset=y_s/2-grid->text.getCharacterSize()/2;
                        grid->text.setPosition(x*x_s+x_offset,y*y_s+y_offset);
                        render_window.draw(grid->text);
                    }
                }
            }
       }

        if(core.is_game_over())
        {
            render_window.draw(game_over_sprite);
        }
        //Draw the info bar
        render_window.draw(info_bar_text);
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
            bool any_movement_to_draw=false;
            any_movement_to_draw=core.action(keymap[key]);
            if(any_movement_to_draw)
            {
                movements_info=std::move(core.get_movement_info());
                create_the_animation_objects();
            }
        }
    }

    void graphic_ui::create_the_animation_objects()
    {
        if(movements_info.size())
        {
            for(auto& elem:movements_info)
            {
                sf::Vector2f begin_pos(elem.x_from*x_s,elem.y_from*y_s),
                             end_pos(elem.x_to*x_s,elem.y_to*y_s);

                sf::Texture grid_movement_texture;
                grid_movement_texture.create(x_s,y_s);
                sf::Uint32 pixel=(0xff<<24)|//Assuming a little endian machine.. from RGBA to ABGR :|
                                 ((sf::Uint32)num_colors[elem.num].b<<16)|
                                 ((sf::Uint32)num_colors[elem.num].g<<8)|
                                 ((sf::Uint32)num_colors[elem.num].r);
                std::vector<sf::Uint32> pixel_map(x_s*y_s,pixel);
                grid_movement_texture.update((sf::Uint8*)&pixel_map.front());
                sf::Sprite obj_sprite(grid_movement_texture);

                animation_engine::anim_texture_ptr object=animation_engine::animated_texture::create(obj_sprite);
                object->set_begin_position(begin_pos);
                object->set_end_position(end_pos);
                object->prepare_to_render();
                object->set_animation_duration(0.07);

                anim_engine->register_object(object,animation_engine::animated_obj_completion_opt::ACTION_DONT_MOVE);
                //Add the text
                std::stringstream text_ss;
                text_ss<<elem.num;
                sf::Text text(text_ss.str().c_str(),font,52);
                text.setColor(sf::Color::Black);
                text.setStyle(sf::Text::Bold);
                animation_engine::anim_text_ptr object_text=animation_engine::animated_text::create(text);
                //Set the proper offset
                int x_offset=x_s/2-text.getCharacterSize()*text_ss.str().size()/4,
                    y_offset=y_s/2-text.getCharacterSize()/2;
                begin_pos.x+=x_offset;
                begin_pos.y+=y_offset;
                end_pos.x+=x_offset;
                end_pos.y+=y_offset;
                object_text->set_begin_position(begin_pos);
                object_text->set_end_position(end_pos);
                object_text->prepare_to_render();
                object_text->set_animation_duration(0.07);
                anim_engine->register_object(object_text,animation_engine::animated_obj_completion_opt::ACTION_DONT_MOVE);
            }
            movements_info.clear();
            draw_movement=true;
        }
        else draw_movement=false;
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
            if(!core.is_game_over())
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
}

