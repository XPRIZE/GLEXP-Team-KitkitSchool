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

void ProgressIndicator::setMax(int max)
{
    removeAllChildren();
    _balls.clear();
 
    
    for (int i=0; i<max; i++) {
        auto b = Sprite::create("Common/Controls/wm_progress_level.png");
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
            b->setTexture("Common/Controls/wm_progress_completed.png");
        } else if (i+1==current) {
            if (complete) {
                b->setTexture("Common/Controls/wm_progress_completed.png");
            } else {
                b->setTexture("Common/Controls/wm_progress_level_current.png");
            }
        } else {
            b->setTexture("Common/Controls/wm_progress_level.png");
        }
    }
    
    
    
}

