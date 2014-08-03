#include "regression.hpp"
#include "../src/game_graphic.hpp"
#include "../src/simple_matrix.hpp"
#include "../src/game_menu.hpp"
#include <gmock/gmock.h>

namespace regression_2048
{
    //Menu functionality
    class game_menu_testsuit_basic : public ::testing::Test
    {
    public:
        int x_menu_pos;
        int y_menu_pos;
        menu_2048::game_menu menu;
        game_menu_testsuit_basic():menu(800,630)
        {
            x_menu_pos=(800/2)-(menu_2048::menu_width/2);
            y_menu_pos=(630/2)-(menu_2048::menu_height/2);
        }
    };

    TEST_F(game_menu_testsuit_basic, AddButtonAndCallBackWithOneFailure)
    {
        int counter=0;
        std::function<void()> callback([&](){
                //Do nothing
                ++counter;
        });
        ASSERT_EQ(1, menu.add_button("test",callback));
        ASSERT_NO_THROW(menu.trigger_button("test"));
        ASSERT_THROW(menu.trigger_button("failind"),std::runtime_error);
        ASSERT_EQ(1,counter);
    }

    TEST_F(game_menu_testsuit_basic, AddMoreThanTheAllowedAmountOfButtons)
    {
        std::function<void()> callback([](){
                //Do nothing
        });
        ASSERT_EQ(1, menu.add_button("test",callback));
        ASSERT_EQ(2, menu.add_button("test2",callback));
        ASSERT_EQ(3, menu.add_button("test3",callback));
        ASSERT_EQ(4, menu.add_button("test4",callback));
        ASSERT_EQ(5, menu.add_button("test5",callback));
        ASSERT_THROW(menu.add_button("test6",callback),std::runtime_error);
    }

    TEST_F(game_menu_testsuit_basic, AddButtonsAndClick)
    {
        using namespace menu_2048;

        int counter1{0},
            counter2{0},
            counter3{0};
        std::function<void()> button1([&](){
                ++counter1;
        });
        std::function<void()> button2([&](){
                ++counter2;
        });
        std::function<void()> button3([&](){
                ++counter3;
        });
        //Add the buttons
        ASSERT_EQ(1, menu.add_button("test",button1));
        ASSERT_EQ(2, menu.add_button("test2",button2));
        ASSERT_EQ(3, menu.add_button("test3",button3));
        //Emulate a button click
        ASSERT_TRUE(menu.trigger_button((menu_width-button_width)/2+x_menu_pos,
                                        10+y_menu_pos));
        ASSERT_TRUE(menu.trigger_button((menu_width-button_width)/2+x_menu_pos,
                                        button_height*1+20+y_menu_pos));
        ASSERT_TRUE(menu.trigger_button((menu_width-button_width)/2+x_menu_pos,
                                        button_height*2+30+y_menu_pos));

        //Trigger some other valid buttons
        ASSERT_TRUE(menu.trigger_button((menu_width-button_width)/2+x_menu_pos+button_width,
                                        10+y_menu_pos));
        ASSERT_TRUE(menu.trigger_button((menu_width-button_width)/2+x_menu_pos,
                                        button_height*1+20+y_menu_pos+button_height));
        ASSERT_TRUE(menu.trigger_button((menu_width-button_width)/2+x_menu_pos+5,
                                        button_height*2+30+y_menu_pos+5));
        //Trigger not valid positions
        ASSERT_FALSE(menu.trigger_button((menu_width-button_width)/2+x_menu_pos-1,
                                        10+y_menu_pos));
        ASSERT_FALSE(menu.trigger_button((menu_width-button_width)/2+x_menu_pos,
                                        button_height*1+20+y_menu_pos-1));
        ASSERT_FALSE(menu.trigger_button((menu_width-button_width)/2+x_menu_pos-1,
                                        button_height*2+30+y_menu_pos-1));
        //Check the counters for the callback functions
        ASSERT_EQ(2,counter1);
        ASSERT_EQ(2,counter2);
        ASSERT_EQ(2,counter3);
    }


    TEST_F(game_menu_testsuit_basic, TrackingAreaSmallTest)
    {
        int x_menu_pos=800/2-(menu_2048::menu_width/2),
            y_menu_pos=630/2-(menu_2048::menu_height/2);

        ASSERT_FALSE(menu.is_in_the_tracking_area(0,0));
        ASSERT_TRUE(menu.is_in_the_tracking_area(x_menu_pos,y_menu_pos));
        ASSERT_TRUE(menu.is_in_the_tracking_area(x_menu_pos+menu_2048::menu_width,
                                                 y_menu_pos+menu_2048::menu_height));
        ASSERT_FALSE(menu.is_in_the_tracking_area(x_menu_pos+menu_2048::menu_width+1,
                                                 y_menu_pos+menu_2048::menu_height));
    }

    using ::testing::_;

    //Mock for the RenderWindow class
    class RenderWindowMock
    {
    public:
        MOCK_CONST_METHOD1(draw,void(sf::Drawable& rnd));
    };

    class game_menu_testsuit_drawing : public ::testing::Test
    {
    public:
        menu_2048::game_menu menu;
        game_menu_testsuit_drawing():menu(800,630)
        {
        }
        void SetUp()
        {
            std::function<void()> callback([](){
                //Do nothing
            });
            ASSERT_EQ(1, menu.add_button("test",callback));
            ASSERT_EQ(2, menu.add_button("test2",callback));
            ASSERT_EQ(3, menu.add_button("test3",callback));
            ASSERT_EQ(4, menu.add_button("test4",callback));
            ASSERT_EQ(5, menu.add_button("test5",callback));
        }
    };


    TEST_F(game_menu_testsuit_drawing, AddButtonsAndDraw)
    {
        RenderWindowMock renderWnd;
        EXPECT_CALL(renderWnd,draw(_)).Times(11);

        ASSERT_NO_THROW(menu.draw_menu(renderWnd));
    }

}











