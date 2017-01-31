//
//  TodoSchoolBackButton.hpp
//  enumaXprize
//
//  Created by 강승우 on 6/27/16.
//
//

#ifndef TodoSchoolBackButton_hpp
#define TodoSchoolBackButton_hpp

#include "cocos2d.h"
#include "ui/uiButton.h"

USING_NS_CC;

class TodoSchoolBackButton : public ui::Button {
public:
    CREATE_FUNC(TodoSchoolBackButton);
    bool init() override;
    
    void onEnter() override;
    
    static void popGameScene();
    
protected:
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);
    void onTouched(Ref* pSender, ui::Widget::TouchEventType eEventType);
    
};

#endif /* TodoSchoolBackButton_hpp */
