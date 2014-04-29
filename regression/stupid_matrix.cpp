#include <gtest.h>
#include "../simple_matrix.hpp"

namespace regression_2048
{
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
}
