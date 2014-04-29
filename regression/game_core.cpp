#include "../game_core.hpp"
#include <gtest.h>

namespace game_core_regression
{
    using namespace game_core;

    class game_core_basictest : public ::testing::Test
    {
    public:
        game_core::game_core game;
        game_core_basictest() : game(4)
        {
            game.set_number(2,0,4);
            game.set_number(3,0,32);
            game.set_number(1,1,4);
            game.set_number(3,1,4);
            game.set_number(0,2,8);
            game.set_number(1,2,8);
            game.set_number(2,2,4);
            game.set_number(3,2,16);
            game.set_number(0,3,2);
            game.set_number(1,3,2);
            game.set_number(2,3,2);
            game.set_number(3,3,2);
        }
    };

    TEST_F(game_core_basictest,MoveRight)
    {
        game.action(simple_matrix::rotation_angle::rotate_0);
        //Check if the action was performed properly
        ASSERT_EQ(4,game.get_number(2,0));
        ASSERT_EQ(32,game.get_number(3,0));
        ASSERT_EQ(8,game.get_number(3,1));
        ASSERT_EQ(16,game.get_number(1,2));
        ASSERT_EQ(4,game.get_number(2,2));
        ASSERT_EQ(16,game.get_number(3,2));
        ASSERT_EQ(4,game.get_number(2,3));
        ASSERT_EQ(4,game.get_number(3,3));

        ASSERT_EQ(32,game.get_score().points);
        ASSERT_EQ(32,game.get_score().last_hit);
        ASSERT_EQ(32,game.get_score().best_hit);
    }

    TEST_F(game_core_basictest,MoveDownAndThenLeft)
    {
        //Move down
        game.action(simple_matrix::rotation_angle::rotate_270);
        ASSERT_EQ(32,game.get_number(3,0));
        ASSERT_EQ(4,game.get_number(1,1));
        ASSERT_EQ(4,game.get_number(3,1));
        ASSERT_EQ(8,game.get_number(0,2));
        ASSERT_EQ(8,game.get_number(1,2));
        ASSERT_EQ(8,game.get_number(2,2));
        ASSERT_EQ(16,game.get_number(3,2));
        ASSERT_EQ(2,game.get_number(0,3));
        ASSERT_EQ(2,game.get_number(1,3));
        ASSERT_EQ(2,game.get_number(2,3));
        ASSERT_EQ(2,game.get_number(3,3));
        ASSERT_EQ(8,game.get_score().points);
        ASSERT_EQ(8,game.get_score().last_hit);
        ASSERT_EQ(8,game.get_score().best_hit);

        //Move left
        game.action(simple_matrix::rotation_angle::rotate_180);
        ASSERT_EQ(32,game.get_number(0,0));
        ASSERT_EQ(8,game.get_number(0,1));
        ASSERT_EQ(16,game.get_number(0,2));
        ASSERT_EQ(8,game.get_number(1,2));
        ASSERT_EQ(16,game.get_number(2,2));
        ASSERT_EQ(4,game.get_number(0,3));
        ASSERT_EQ(4,game.get_number(1,3));
        ASSERT_EQ(40,game.get_score().points);
        ASSERT_EQ(32,game.get_score().last_hit);
        ASSERT_EQ(32,game.get_score().best_hit);
    }

    TEST_F(game_core_basictest,MoveLeftAndUp)
    {
        //Left
        game.action(simple_matrix::rotation_angle::rotate_180);
        ASSERT_EQ(4,game.get_number(0,0));
        ASSERT_EQ(32,game.get_number(1,0));
        ASSERT_EQ(8,game.get_number(0,1));
        ASSERT_EQ(16,game.get_number(0,2));
        ASSERT_EQ(4,game.get_number(1,2));
        ASSERT_EQ(16,game.get_number(2,2));
        ASSERT_EQ(4,game.get_number(0,3));
        ASSERT_EQ(4,game.get_number(1,3));
        ASSERT_EQ(32,game.get_score().points);
        ASSERT_EQ(32,game.get_score().last_hit);
        ASSERT_EQ(32,game.get_score().best_hit);

        //Up
        game.action(simple_matrix::rotation_angle::rotate_90);
        ASSERT_EQ(4,game.get_number(0,0));
        ASSERT_EQ(32,game.get_number(1,0));
        ASSERT_EQ(16,game.get_number(2,0));
        ASSERT_EQ(8,game.get_number(0,1));
        ASSERT_EQ(8,game.get_number(1,1));
        ASSERT_EQ(16,game.get_number(0,2));
        ASSERT_EQ(4,game.get_number(0,3));
        ASSERT_EQ(40,game.get_score().points);
        ASSERT_EQ(8,game.get_score().last_hit);
        ASSERT_EQ(32,game.get_score().best_hit);
    }

}
