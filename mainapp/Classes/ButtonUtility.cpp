//
//  ButtonUtility.cpp
//  Matching
//
//  Created by sugar on 5/25/16.
//
//

#include "ButtonUtility.hpp"
#include "cocos2d.h"

void ButtonUtility::scaledButtonListener(Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            ((Button*)sender)->runAction(EaseElasticInOut::create(ScaleTo::create(0.1f, 1.2f)));
            break;
        }
        case ui::Widget::TouchEventType::ENDED:
        {
            ((Button*)sender)->runAction(EaseElasticInOut::create(ScaleTo::create(0.1f, 1.0f)));
            break;
        }
        case ui::Widget::TouchEventType::CANCELED:
        {
            ((Button*)sender)->runAction(EaseElasticInOut::create(ScaleTo::create(0.1f, 1.0f)));
            break;
        }
        default:
            break;
    }
}
