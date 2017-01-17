//
//  PlayTimer.cpp
//  todomath
//
//  Created by Sungwoo Kang on 3/2/16.
//
//

#include "PlayTimer.hpp"
#include <time.h>

using namespace std;

void PlayTimer::start()
{
    _interval = 0;
    resume();
}

double PlayTimer::getCurrentTimeOfDouble() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return static_cast<double>(time.tv_sec + static_cast<double>(time.tv_usec) / 1000000.f);
}


double PlayTimer::pause()
{
    double lapse = getCurrentTimeOfDouble() - _lastLap;
    _interval += lapse;
    return _interval;
}

void PlayTimer::resume()
{
    _lastLap = getCurrentTimeOfDouble();
}

double PlayTimer::stop()
{
    pause();
    return _interval;
}
