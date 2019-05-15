//
//  FireworksEffect.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 9/7/17.
//
//

#pragma once


#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;

class FireworksEffect : public Node
{
public:
    CREATE_FUNC(FireworksEffect)
    
    static void fire();
    static void fire(Vec2 pos);
    static void miss();
    
    
    virtual bool init() override;
    virtual void update(float delta) override;
    
    void preset(int pre);
    void stop();
    void start();
    
private:
    
    float _time;
    float _velocity;
    float _loc;
    
    
    
    int _numParticles;
    float _size;
    float _maxtail;
    
    float _initVel;
    float _accel;
    
    Color4F _startColor, _endColor;
    
    
    
    
    DrawNode *_drawNode;
    
    
    
};
