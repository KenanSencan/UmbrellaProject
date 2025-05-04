#include <chrono>
#include <iostream>

#include "TimerComponent.h"

int main()
{
    TimerComponent timer(5.0f, TimeUnit::Second);
    timer.Start();

    while (true)
    {
        timer.Update();

        if (timer.IsFinished())
        {
            std::cout << "Timer finished" << std::endl;
            break;
        }

        std::cout << "Time remaining: " << timer.GetRemainingTime() << " seconds (" << timer.GetRemainingTime(TimeUnit::Millisecond) << " ms) - " << timer.GetElapsedTime() << std::endl;
    }
    return 0;
}
