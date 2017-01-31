//
//  TestPopup.hpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 11/1/16.
//
//

#ifndef TestPopup_hpp
#define TestPopup_hpp

#include "Common/Controls/PopupBase.hpp"
#include <cocos/ui/CocosGUI.h>

class MainScene2;
class TestPopup : public PopupBase
{
public:
    static TestPopup* create(Node* parentView, Size size);
    bool init(Node* parentView, Size size);
    void onNextPressed(Ref *sender, ui::Widget::TouchEventType event);
    
    MainScene2* _delegate;

private:
    int _classIdx;
    int _courseIdx;
    int _studentNumber;
    
    cocos2d::Label* classLabel;
    cocos2d::ui::TextField* numberField;
    cocos2d::Label* courseLabel;
    
//    void backPressed(Ref *sender, Control::EventType event);

};

#endif /* TestPopup_hpp */
