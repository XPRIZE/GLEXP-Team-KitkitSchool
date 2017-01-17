//
//  ButtonUtility.hpp
//  Matching
//
//  Created by sugar on 5/25/16.
//
//

#ifndef ButtonUtility_hpp
#define ButtonUtility_hpp

#include <stdio.h>
#include "cocostudio/CocoStudio.h"
#include "ui/cocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class ButtonUtility
{
public:
    static void scaledButtonListener(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
};

#endif /* ButtonUtility_hpp */
