//
//  LightBall.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 9/10/17.
//
//

#include "LightBall.hpp"
#include <string>


void LightBall::setup(int type)
{
    std::string folder = "Common/Effects/";
    
    _type = type;
    
    if (type==1) {
        _ball = Sprite::create(folder+"daily_window_beam_glitter_1.png");
    } else {
        _ball = Sprite::create(folder+"daily_window_beam_glitter_2.png");
    }
    addChild(_ball);
    
    setCascadeOpacityEnabled(true);
    
}

