//
//  Slot.hpp
//  KitkitSchool
//
//  Created by JungJaehun on 08/09/2017.
//
//

#pragma once
#include "../PatternTrainHeader.h"

BEGIN_NS_PATTERNTRAIN;

class Box;
class Slot : public Node {
public:
    CREATE_FUNC(Slot);
    virtual bool init() override;
    bool _hasBox, _replaceable;
    Box *_boxInSlot;
    Vec2 _pos;
    void setBlank(string letter);
    string _correctAnswer;
};

END_NS_PATTERNTRAIN;
