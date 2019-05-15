//
//  TodoSchoolBackButton.hpp
//  KitkitSchool
//
//  Created by 강승우 on 6/27/16.
//
//

#ifndef TodoSchoolBackButton_hpp
#define TodoSchoolBackButton_hpp

#include "cocos2d.h"
#include "ui/UIButton.h"
#include <functional>

USING_NS_CC;

class TodoSchoolBackButton : public ui::Button {
public:
    CREATE_FUNC(TodoSchoolBackButton);
    bool init() override;
    
    void onEnter() override;
    
    static void popGameScene();

    std::function<void(void)> onBack;
    
    
protected:
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);
    void onTouched(Ref* pSender, ui::Widget::TouchEventType eEventType);
    
private:
    static bool _didFinish;
    
};

#endif /* TodoSchoolBackButton_hpp */
