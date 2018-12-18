//
//  Slot.hpp
//  KitkitTest
//
//  Created by JungJaehun on 18/07/2018.
//

#pragma once
#include "EggQuizHeader.hpp"
#include "DragButton.hpp"

BEGIN_NS_EGGQUIZ;

class DragButton;
class Slot : public Node {
public:
    static Slot* create(SlotSize slotSize);
    DragButton* _targetButton;
    void setAnswer(string answer);
    void enableOver();
    void disableOver();
    void registerButton(DragButton* button);
    void releaseButton();
    
    string getAnswer();
private:
    bool initWithSize(SlotSize slotSize);
    string _answer;
    Scale9Sprite *_body, *_over;
};

END_NS_EGGQUIZ;
