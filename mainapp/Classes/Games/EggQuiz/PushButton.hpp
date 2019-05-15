//
//  PushButton.hpp
//  KitkitTest
//
//  Created by JungJaehun on 18/07/2018.
//

#pragma once
#include "EggQuizHeader.hpp"

BEGIN_NS_EGGQUIZ;


class PushButton : public Node {
public:
    static PushButton* create(Size buttonSize, float fontSize, string label, function<void(string answer)> onTouchBegan, string fontPath="");
    Label *_label;
    
    void setLabel(string label);
    void setActive();
    void setDeactive();
    void setImage();
    
private:
    bool initWithSize(Size buttonSize, float fontSize, string label, function<void(string answer)> onTouchBegan, string fontPath);
    Scale9Sprite *_active, *_body;
};

END_NS_EGGQUIZ;
