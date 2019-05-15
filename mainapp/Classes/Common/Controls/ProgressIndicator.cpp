//
//  ProgressIndicator.cpp
//
//  Created by Gunho Lee on 6/28/16.
//
//

#include "ProgressIndicator.hpp"
#include <cocos/ui/CocosGUI.h>


using namespace cocos2d::ui;


const Size dotSize = Size(46, 48);
const float dotMargin = 10;

bool ProgressIndicator::init()
{
    
    if ( !Widget::init() )
    {
        return false;
    }
    
    _balls.clear();
    setContentSize(Size(0, dotSize.height));

    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    return true;
}

void ProgressIndicator::setMax(int max, bool forEgg)
{
    removeAllChildren();
    _balls.clear();
 
    if (forEgg) {
        _completedBall = "Common/Controls/pretest_progress_level_pass.png";
        _futureBall = "Common/Controls/pretest_progress_level_notyet.png";
        _currentBall = "Common/Controls/pretest_progress_level_current.png";
        _missedBall = "Common/Controls/pretest_progress_level_fail.png";
    } else {
        _completedBall = "Common/Controls/wm_progress_completed.png";
        _futureBall = "Common/Controls/wm_progress_level.png";
        _currentBall = "Common/Controls/wm_progress_level_current.png";
        _missedBall = _completedBall;
    }
    
    for (int i=0; i<max; i++) {
        auto b = Sprite::create(_futureBall);
        b->setPosition(i*(dotMargin+dotSize.width)+0.5*dotSize.width, 0.5*dotSize.height);
        addChild(b);
        _balls.push_back(b);
    }
    
    setContentSize(Size(max*dotSize.width + (max-1)*dotMargin, dotSize.height));
    
}



void ProgressIndicator::setCurrent(int current, bool complete)
{
    
    for (int i=0; i<_balls.size(); i++) {
        Sprite *b = _balls.at(i);
        if (i+1<current) {
            b->setTexture(_completedBall);
        } else if (i+1==current) {
            if (complete) {
                b->setTexture(_completedBall);
            } else {
                b->setTexture(_currentBall);
            }
        } else {
            b->setTexture(_futureBall);
        }
    }
    
    
    
}

void ProgressIndicator::setStatus(std::string status)
{
    for (int i=0; i<_balls.size(); i++) {
        Sprite *b = _balls.at(i);
        auto s = status[i];
        
        if (s=='c') {
            b->setTexture(_currentBall);
        } else if (s=='o') {
            b->setTexture(_completedBall);
        } else if (s=='x') {
            b->setTexture(_missedBall);
        } else {
            b->setTexture(_futureBall);
        }
    }
}
