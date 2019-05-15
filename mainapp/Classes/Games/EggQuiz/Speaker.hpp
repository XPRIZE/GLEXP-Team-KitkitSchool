//
//  Speaker.hpp
//  KitkitTest
//
//  Created by JungJaehun on 18/07/2018.
//

#pragma once
#include "EggQuizHeader.hpp"
#include "ProblemBank.hpp"

BEGIN_NS_EGGQUIZ;

class DigitalTestScene;
class Speaker : public Node {
public:
    static Speaker* create(SpeakerSize speakerSize);
    void setFileName(string fileName);
    void setPressed(float delay = 0);
    void setPressDisabled();
    function<void()> _onCompleteCallback;
    
private:
    Scale9Sprite *_body, *_over;
    string _fileName;
    bool _pressDisabled = false;
    bool initWithSize(SpeakerSize speakerSize);
    
};

END_NS_EGGQUIZ;
