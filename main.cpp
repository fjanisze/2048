#include "2048.hpp"

static const bool run_regression = false; //Set to true to build the regression code

namespace graphic_2048
{

    graphic_2048::graphic_2048(int x_size,int y_size,int size) : x_s(x_size/size), y_s(y_size/size), map_size(size),
                                                                     grid_container(size), num_container(size),
                                                                     random_coord_index(0),points(0),game_over(false)
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
            throw std::runtime_error("HOLY SHIT!!!");
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

    void graphic_2048::set_info()
    {

        std::stringstream ss;
        auto get_level=[this](){
            switch(points/512)
            {
            case 0:
                return "Pussy!";
            case 1:
                return "Half man!";
            case 2:
                return "Bitch!";
            case 3:
                return "Nigga way!";
            case 4:
                return "Real pimp!";
            default:
                return "Fucking master!";
            };
        };
        ss<<"Points "<<points<<", Level: "<<get_level();
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
        action(keymap[key]);
    }

    void graphic_2048::add_new_number(int amount)
    {
        //Generate a block
        int x,y;
        do{
            get_random_coord(x,y);
            int n=num_container.get(x,y);
            if(num_container.get(x,y)==0)
            {
                int a,b;
                get_random_coord(a,b);
                num_container.get(x,y)=((a+b)%2?2:4);
                --amount;
            }
        }while(amount>0);
    }

    bool graphic_2048::action(simple_matrix::rotation_angle angle)
    {
        num_container.rotate(angle);
        bool moved=false;
        //Now do the sum
        for(int y=0;y<map_size;y++)
        {
            int c=-1,cur_x=map_size-1;
            for(int x=map_size-1;x>=0;x--)
            {
                int t=num_container.get(x,y);
                if(t!=0)
                {
                    if(cur_x==x) continue;
                    int t2=num_container.get(cur_x,y);
                    if(t2==t)
                    {
                        points+=(t2*2);
                        num_container.get(cur_x,y)*=2;
                        num_container.get(x,y)=0;
                        --cur_x;
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
}

int main(int argc,char** argv)
{
    if(run_regression)
    {
        ::testing::InitGoogleTest(&argc,argv);
        return RUN_ALL_TESTS();
    }
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(800, 630), "2048");
    graphic_2048::graphic_2048 game(800,600,4);
    game.add_new_number(2);
    game.update_num_color();

	// Start the game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                app.close();
                break;
            case sf::Event::MouseButtonPressed:
                game.click(event);
                break;
            case sf::Event::KeyPressed:
                if(game.can_continue())
                {
                    game.button(event);
                }
                break;
            };
        }

        // Clear screen
        app.clear(sf::Color::Black);

        game.draw(app);
        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
