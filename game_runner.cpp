#include "game_runner.hpp"

namespace game_runner
{
    game_runner::game_runner() : core(4),
                                 graphic(core,800,600,4),
                                 menu(800,600)
    {
        current_mode=current_game_mode::MENU_MODE;//By default
        app.create(sf::VideoMode(800, 630), "2048");
        app.setFramerateLimit(40);

        core.add_new_number(2);
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
        std::function<void()> trigger_quit_game([this](){quit_game_button();});
        //Add the menu buttons
        menu.add_button("New game",trigger_new_game);
        menu.add_button("Load game",trigger_load_game);
        menu.add_button("Save game",trigger_save_game);
        menu.add_button("Hall of fame",trigger_hof_game);
        menu.add_button("Quit",trigger_quit_game);
    }

    void game_runner::new_game_button()
    {
        current_mode=current_game_mode::PLAY_MODE;
        core.reset_board();
        core.add_new_number(2);
    }

    void game_runner::load_game_button()
    {
        std::ifstream in_file{data_filename,std::ios_base::binary};
        if(in_file)
        {
            //graphic.load_data(in_file);
            in_file.close();
        }
    }

    void game_runner::save_game_button()
    {
        std::ofstream out_file{data_filename,std::ios_base::binary};
        if(out_file)
        {
            //graphic.save_data(out_file);
            out_file.close();
        }
    }

    void game_runner::hof_game_button()
    {
        current_mode=current_game_mode::HOF_MODE;
    }

    void game_runner::quit_game_button()
    {
        exit(0);
    }

    void game_runner::loop()
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
                for(auto elem:core.get_hof())
                {
                    std::stringstream ss;
                    ss<<elem.date<<", PT: "<<elem.points<<" (+"<<elem.best_hit<<"), LV: "<<core.get_level(elem.points);

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

    bool game_runner::escape_button_pressed(const sf::Event& event)
    {
        if((event.type==sf::Event::KeyPressed)&&
           (event.key.code==sf::Keyboard::Escape))
           return true;
        return false;
    }

}
