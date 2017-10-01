//
//  Box.hpp
//  enumaXprize
//
//  Created by JungJaehun on 08/09/2017.
//
//

#pragma once
#include "../PatternTrainHeader.h"
#include "../PatternTrainProblemBank.hpp"

BEGIN_NS_PATTERNTRAIN;

class FreightCar;
class Slot;
class Box : public Node {
public:
    CREATE_FUNC(Box);
    virtual bool init() override;
    static int _index;
    
    void setType(string letter, string boxPath, PatternTrainProblemBank::shape newShape, bool setAnswer = false);
    void loadOnSlot(Slot *targetSlot);
    void unload(Node *ground);
    void setGlobalZOrderManual(float order);
    
    string _assignedLetter;
    int _assignedIndex;
    function<void()> onTouchBegan;
    function<void()> onTouchEnded;
    function<void()> onTouchMoved;
    int _originZIndex;
    Slot *_targetSlot;
    bool _loading;
    Vec2 _originalPos;
    bool _duplicated;
    bool _enableTouch;
    map<int, int> _touching;

    Sprite *_body, *_deco;

};

END_NS_PATTERNTRAIN;
