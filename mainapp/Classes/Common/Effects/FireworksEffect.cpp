//
//  FireworksEffect.cpp
//  enumaXprize
//
//  Created by Gunho Lee on 9/7/17.
//
//

#include "FireworksEffect.hpp"
#include "ccMacros.h"

void FireworksEffect::fire()
{
    
    const auto margin = Size(200, 200);
    

    auto winSize = Director::getInstance()->getWinSize();
    
    {
        auto pos = Vec2(random(margin.width, winSize.width/2),
                        random(winSize.height/2, winSize.height-margin.height));
        
        fire(pos);
    }
    
    {
        auto pos = Vec2(random(winSize.width/2, winSize.width-margin.width),
                        random(winSize.height/2, winSize.height-margin.height));
        
        fire(pos);
    }
    

}

void FireworksEffect::fire(Vec2 pos)
{
    auto effect = FireworksEffect::create();
    
    effect->setPosition(pos);
    Director::getInstance()->getRunningScene()->addChild(effect);
    effect->start();
}

void FireworksEffect::miss()
{
    auto winSize = Director::getInstance()->getWinSize();
    const auto moveSize = Size(50, 0);
    
    
    auto cl = LayerColor::create(Color4B(255, 0, 0, 64), winSize.width+2*moveSize.width, winSize.height+2*moveSize.height);
    cl->setPosition(-Vec2(moveSize));
    

    
    Director::getInstance()->getRunningScene()->addChild(cl);
    auto shake = Sequence::create(EaseOut::create(MoveBy::create(0.06, moveSize), 2.0),
                                  EaseInOut::create(MoveBy::create(0.12, -2*Vec2(moveSize)), 2.0),
                                  EaseOut::create(MoveBy::create(0.06, moveSize), 2.0),
                                  CallFunc::create([cl](){
        cl->removeFromParent();
    }),
                                  nullptr);
    Director::getInstance()->getRunningScene()->runAction(shake);
}

bool FireworksEffect::init()
{
    
    if (!Node::init()) {
        return false;
    }
    
    
    _drawNode = DrawNode::create();
    addChild(_drawNode);
    
    _time = 0;
    
    
    _numParticles = 20;
    _size = 6;
    _maxtail = 70;
    
    
    _initVel = 2500;
    _accel = -5000;
    
    
    _startColor = Color4F::WHITE;
    _endColor = Color4F(142/255.0, 63/255.0, 152/255.0, 1.0);
    
    
    return true;
    
}



void FireworksEffect::update(float delta)
{
    _time += delta;
    
    _drawNode->clear();
    
    
    _velocity += _accel * delta;
    if (_velocity < 0) {
        _velocity = 0;
        stop();
        removeFromParent();
        return;
    }
    
    
    _loc += _velocity*delta;
    auto vRate = MIN(1.0, 2.0*_velocity / _initVel);
    auto color = Color4F( vRate*_startColor.r + (1.0-vRate)*_endColor.r,
                         vRate*_startColor.g + (1.0-vRate)*_endColor.g,
                         vRate*_startColor.b + (1.0-vRate)*_endColor.b,
                         vRate*_startColor.a + (1.0-vRate)*_endColor.a);
    
    
    for (int i=0; i<_numParticles; i++) {
        auto angle = i * 360.0 / _numParticles;
        auto radian = angle / 180.0 * M_PI;
        
        auto dir = Vec2(cos(radian), sin(radian));
        auto head = _loc * dir;
        auto taillen = MIN(_maxtail, _velocity*0.1);
        auto tail = MAX(0, _loc - taillen) * dir;
        
        auto diff1 = _size * Vec2(cos(radian + M_PI_2), sin(radian + M_PI_2));
        auto diff2 = _size * Vec2(cos(radian - M_PI_2), sin(radian - M_PI_2));
        
        Vec2 poli[4] = { head + diff1, tail + diff1, tail + diff2, head + diff2 };
        
        _drawNode->drawSolidPoly(poli, 4, color);
        _drawNode->drawDot(head, _size, color);
        _drawNode->drawDot(tail, _size, color);
        
        
        //_drawNode->drawDot(Vec2(x, y), _size, Color4F::RED);
    }
    
    
}

void FireworksEffect::preset(int pre)
{
    
    _numParticles = 4*(random(3, 5));
    _size = 6;
    _maxtail = 70;
    
    _initVel = 2500;
    _accel = -5000;
    
    _initVel = random(1800, 2800);
    _accel = -_initVel * random(1.9, 2.1);
    
    
    if (pre==0) {
        _startColor = Color4F::WHITE;
        _endColor = Color4F(142/255.0, 63/255.0, 152/255.0, 1.0);
    } else if (pre==1) {
        _startColor = Color4F(255/255.0, 187/255.0, 31/255.0, 1.0);
        _endColor = Color4F(241/255.0, 86/255.0, 68/255.0, 1.0);
    } else if (pre==2) {
        _startColor = Color4F(0/255.0, 162/255.0, 202/255.0, 1.0);
        _endColor = Color4F(61/255.0, 99/255.0, 175/255.0, 1.0);
    } else if (pre==3) {
        _startColor = Color4F(214/255.0, 224/255.0, 59/255.0, 1.0);;
        _endColor = Color4F(43/255.0, 179/255.0, 75/255.0, 1.0);
    }
    
    
    
}

void FireworksEffect::stop()
{
    _drawNode->clear();
    unscheduleUpdate();
    
}



void FireworksEffect::start()
{
    preset(random(0, 3));
    scheduleUpdate();
    
    _loc = 0;
    _velocity = _initVel;
    
    
}
