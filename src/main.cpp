#include "game_runner.hpp"

int main(int argc,char** argv)
{
    if(run_regression)
    {
        ::testing::InitGoogleMock(&argc,argv);
        return RUN_ALL_TESTS();
    }
    game_runner::game_runner runner;
    runner.loop();

    return EXIT_SUCCESS;
}
