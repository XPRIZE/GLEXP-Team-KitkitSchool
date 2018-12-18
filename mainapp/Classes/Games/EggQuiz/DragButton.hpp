//
//  DragButton.hpp
//  KitkitTest
//
//  Created by JungJaehun on 18/07/2018.
//

#pragma once
#include "EggQuizHeader.hpp"
#include "Slot.hpp"

BEGIN_NS_EGGQUIZ;

class Slot;
class DragButton : public Node {
public:
    static DragButton* create(string label, Size size, float fontSize, vector<Slot*> slots, function<void()> onTouchMoved, function<void()> onTouchEnded);
    Slot* _targetSlot;
    Label *_label;
    bool _holding, _willBeDisappear = false;
    
    void setLabel(string label);
    void setOriginalPos(Vec2 pos);
    void fadeIn();
    void fadeOut();
    void setActive();
    void disableDuplicate();
    
private:
    bool initWithSize(string label, Size size, float fontSize, vector<Slot*> slots, function<void()> onTouchMoved, function<void()> onTouchEnded);
    bool _duplicate = true;
    Vec2 _originPos;
    Scale9Sprite *_active, *_body, *_shadow;
};

END_NS_EGGQUIZ;
