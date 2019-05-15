//
//  PlayTimer.hpp
//  todomath
//
//  Created by Sungwoo Kang on 3/2/16.
//
//

#ifndef PlayTimer_hpp
#define PlayTimer_hpp



class PlayTimer
{
public:

    
    void start();
    double pause();
    void resume();
    double stop();
    double getCurrentTimeOfDouble();
    
private:
    
    double _interval;
    double _lastLap;
    
    
    
};

#endif /* PlayTimer_hpp */
