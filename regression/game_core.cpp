#include "../game_core.hpp"
#include <gtest.h>

namespace game_core_regression
{
    using namespace game_core;

    class game_core_dirtywrapper : public game_core::game_core
    {
    public:
        game_core_dirtywrapper(int size):
            game_core(size)
        {
            //Set all to 0 and create a basic
            num_container_t* num = &num_container;
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
        int get_map_size(){ return map_size; }

        num_container_t*  get_nums(){ return &num_container;}
    };

    class game_core_basictest : public ::testing::Test
    {
    public:
        game_core_dirtywrapper game;
        game_core_basictest() : game(4)
        {}
    };

    TEST_F(game_core_basictest,BasicConstructionCheck)
    {
        ASSERT_EQ( 4 ,game.get_map_size());
    }

    TEST_F(game_core_basictest,MoveRight)
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

        ASSERT_EQ(32,game.get_score().points);
    }

    TEST_F(game_core_basictest,MoveDownAndThenLeft)
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
        ASSERT_EQ(8,game.get_score().points);

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

    TEST_F(game_core_basictest,MoveLeftAndUp)
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
        ASSERT_EQ(32,game.get_score().points);

        //Up
        game.action(simple_matrix::rotation_angle::rotate_90);
        ASSERT_EQ(4,num->get(0,0));
        ASSERT_EQ(32,num->get(1,0));
        ASSERT_EQ(16,num->get(2,0));
        ASSERT_EQ(8,num->get(0,1));
        ASSERT_EQ(8,num->get(1,1));
        ASSERT_EQ(16,num->get(0,2));
        ASSERT_EQ(4,num->get(0,3));
        ASSERT_EQ(40,game.get_score().points);
    }

}
