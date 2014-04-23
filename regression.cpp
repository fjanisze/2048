#include "regression.hpp"
#include "2048.hpp"
#include "simple_matrix.hpp"
#include <gmock/gmock.h>

namespace regression_2048
{
    class graphic_2048_dirtywrapper : public graphic_2048::graphic_2048
    {
    public:
        graphic_2048_dirtywrapper(int x_size,int y_size,int size):
            graphic_2048(x_size,y_size,size)
        {
            //Set all to 0 and create a basic
            num_container_t* num = get_nums();
            num->get(2,0)=4;
            num->get(3,0)=32;
            num->get(1,1)=4;
            num->get(3,1)=4;
            num->get(0,2)=8;
            num->get(1,2)=8;
            num->get(2,2)=4;
            num->get(3,2)=16;
            num->get(0,3)=2;
            num->get(1,3)=2;
            num->get(2,3)=2;
            num->get(3,3)=2;

        }
        int get_x_s(){ return x_s; }
        int get_y_s(){ return y_s; }
        int get_map_size(){ return map_size; }

        grid_container_t* get_grid(){ return &grid_container;}
        num_container_t*  get_nums(){ return &num_container;}
    };

    class graphic_2048_basictest : public ::testing::Test
    {
    public:
        graphic_2048_dirtywrapper game;
        graphic_2048_basictest() : game(800,600,4)
        {}
    };

    TEST_F(graphic_2048_basictest,BasicConstructionCheck)
    {
        ASSERT_EQ( 800 / 4,game.get_x_s());
        ASSERT_EQ( 600 / 4 ,game.get_y_s());
        ASSERT_EQ( 4 ,game.get_map_size());
    }

    TEST_F(graphic_2048_basictest,MoveRight)
    {
        game.action(simple_matrix::rotation_angle::rotate_0);
        //Check if the action was performed properly
        auto* num = game.get_nums();
        ASSERT_EQ(4,num->get(2,0));
        ASSERT_EQ(32,num->get(3,0));
        ASSERT_EQ(8,num->get(3,1));
        ASSERT_EQ(16,num->get(1,2));
        ASSERT_EQ(4,num->get(2,2));
        ASSERT_EQ(16,num->get(3,2));
        ASSERT_EQ(4,num->get(2,3));
        ASSERT_EQ(4,num->get(3,3));

        ASSERT_EQ(32,game.get_score());
    }

    TEST_F(graphic_2048_basictest,MoveDownAndThenLeft)
    {
        //Move down
        game.action(simple_matrix::rotation_angle::rotate_270);
        auto* num = game.get_nums();
        ASSERT_EQ(32,num->get(3,0));
        ASSERT_EQ(4,num->get(1,1));
        ASSERT_EQ(4,num->get(3,1));
        ASSERT_EQ(8,num->get(0,2));
        ASSERT_EQ(8,num->get(1,2));
        ASSERT_EQ(8,num->get(2,2));
        ASSERT_EQ(16,num->get(3,2));
        ASSERT_EQ(2,num->get(0,3));
        ASSERT_EQ(2,num->get(1,3));
        ASSERT_EQ(2,num->get(2,3));
        ASSERT_EQ(2,num->get(3,3));
        ASSERT_EQ(8,game.get_score());

        //Move left
        game.action(simple_matrix::rotation_angle::rotate_180);
        ASSERT_EQ(32,num->get(0,0));
        ASSERT_EQ(8,num->get(0,1));
        ASSERT_EQ(16,num->get(0,2));
        ASSERT_EQ(8,num->get(1,2));
        ASSERT_EQ(16,num->get(2,2));
        ASSERT_EQ(4,num->get(0,3));
        ASSERT_EQ(4,num->get(1,3));

    }

    TEST_F(graphic_2048_basictest,MoveLeftAndUp)
    {
        //Left
        game.action(simple_matrix::rotation_angle::rotate_180);
        auto* num = game.get_nums();
        ASSERT_EQ(4,num->get(0,0));
        ASSERT_EQ(32,num->get(1,0));
        ASSERT_EQ(8,num->get(0,1));
        ASSERT_EQ(16,num->get(0,2));
        ASSERT_EQ(4,num->get(1,2));
        ASSERT_EQ(16,num->get(2,2));
        ASSERT_EQ(4,num->get(0,3));
        ASSERT_EQ(4,num->get(1,3));
        ASSERT_EQ(32,game.get_score());

        //Up
        game.action(simple_matrix::rotation_angle::rotate_90);
        ASSERT_EQ(4,num->get(0,0));
        ASSERT_EQ(32,num->get(1,0));
        ASSERT_EQ(16,num->get(2,0));
        ASSERT_EQ(8,num->get(0,1));
        ASSERT_EQ(8,num->get(1,1));
        ASSERT_EQ(16,num->get(0,2));
        ASSERT_EQ(4,num->get(0,3));
        ASSERT_EQ(40,game.get_score());
    }


    //For the matrix

    class StupidMatrixSize3 : public ::testing::Test
    {
    public:
        simple_matrix::simple_square_matrix<int> matrix;
        StupidMatrixSize3() : matrix(3)
        {
             //Init
            for(int y=0;y<3;y++)
            {
                for(int x=0;x<3;x++)
                {
                    matrix.get(x,y)=x+y*3+1;
                }
            }
        }
    };

    TEST(StupidMatrix,CreationAndDestructionSmallWithINT)
    {
        auto test=[](){
             simple_matrix::simple_square_matrix<int> matrix(100);
        };
        EXPECT_NO_THROW(test());
    }

    TEST(StupidMatrix,SetAndGetSmallWithINT)
    {
        simple_matrix::simple_square_matrix<int> matrix(100);
        matrix.get(5,5)=1;
        matrix.get(50,50)=2;
        matrix.get(90,10)=3;
        matrix.get(10,90)=4;
        matrix.get(90,90)=5;
        ASSERT_EQ(matrix.get(5,5),1);
        ASSERT_EQ(matrix.get(50,50),2);
        ASSERT_EQ(matrix.get(90,10),3);
        ASSERT_EQ(matrix.get(10,90),4);
        ASSERT_EQ(matrix.get(90,90),5);
    }

    TEST(StupidMatrix,GetWithOutOfRangeSmallWithINT)
    {
        simple_matrix::simple_square_matrix<int> matrix(100);
        EXPECT_ANY_THROW(matrix.get(0,100));
        EXPECT_ANY_THROW(matrix.get(100,0));
        EXPECT_ANY_THROW(matrix.get(100,100));
        EXPECT_ANY_THROW(matrix.get(-1,0));
        EXPECT_ANY_THROW(matrix.get(0,-1));
        EXPECT_NO_THROW(matrix.get(0,0));
        EXPECT_NO_THROW(matrix.get(0,99));
        EXPECT_NO_THROW(matrix.get(99,0));
        EXPECT_NO_THROW(matrix.get(99,99));
    }

    TEST(StupidMatrix,RotationEdge2Of90WithINT)
    {
        simple_matrix::simple_square_matrix<int> matrix(2);
        //Init
        for(int y=0;y<2;y++)
        {
            for(int x=0;x<2;x++)
            {
                matrix.get(x,y)=x+y*2+1;
            }
        }
        //Rotate and check
        matrix.rotate();
        ASSERT_EQ(matrix.get(0,0),3);
        ASSERT_EQ(matrix.get(0,1),4);
        ASSERT_EQ(matrix.get(1,0),1);
        ASSERT_EQ(matrix.get(1,1),2);
    }

    TEST_F(StupidMatrixSize3,RotationEdge3Of180WithINT)
    {
        //Rotate and check
        matrix.rotate(simple_matrix::rotation_angle::rotate_180);
        ASSERT_EQ(matrix.get(0,0),9);
        ASSERT_EQ(matrix.get(2,2),1);
        ASSERT_EQ(matrix.get(1,1),5);
        ASSERT_EQ(matrix.get(0,1),6);
        ASSERT_EQ(matrix.get(1,2),2);
        ASSERT_EQ(matrix.get(2,0),7);
    }

    TEST_F(StupidMatrixSize3,RotationEdge3Of270WithINT)
    {
        //Rotate and check
        matrix.rotate(simple_matrix::rotation_angle::rotate_270);
        ASSERT_EQ(matrix.get(0,0),3);
        ASSERT_EQ(matrix.get(2,2),7);
        ASSERT_EQ(matrix.get(1,1),5);
        ASSERT_EQ(matrix.get(0,1),2);
        ASSERT_EQ(matrix.get(1,2),4);
        ASSERT_EQ(matrix.get(2,0),9);
    }

    TEST_F(StupidMatrixSize3,CheckMatrixTransponseWithINT)
    {
        matrix.transponse_matrix();
        ASSERT_EQ(matrix.get(0,0),1);
        ASSERT_EQ(matrix.get(2,2),9);
        ASSERT_EQ(matrix.get(1,1),5);
        ASSERT_EQ(matrix.get(0,1),2);
        ASSERT_EQ(matrix.get(1,2),6);
        ASSERT_EQ(matrix.get(2,0),7);
        ASSERT_EQ(matrix.get(2,1),8);
    }

    TEST_F(StupidMatrixSize3,CheckMatrixReverseWithINT)
    {
        matrix.reverse_row();
        ASSERT_EQ(matrix.get(0,0),3);
        ASSERT_EQ(matrix.get(2,2),7);
        ASSERT_EQ(matrix.get(1,1),5);
        matrix.reverse_col();
        ASSERT_EQ(matrix.get(0,0),9);
        ASSERT_EQ(matrix.get(2,2),1);
        ASSERT_EQ(matrix.get(1,1),5);
    }

    //Menu functionality
    class game_menu_testsuit_basic : public ::testing::Test
    {
    public:
        menu_2048::game_menu menu;
        game_menu_testsuit_basic():menu(800,630)
        {
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
        EXPECT_CALL(renderWnd,draw(_)).Times(6);

         ASSERT_NO_THROW(menu.draw_menu(renderWnd));
    }

}











