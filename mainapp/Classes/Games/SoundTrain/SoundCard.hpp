//
//  SoundCard.hpp
//  enumaXprize
//
//  Created by JungJaehun on 05/09/2017.
//
//

#pragma once
#include "SoundTrainHeader.h"

BEGIN_NS_SOUNDTRAIN;

class SoundTrain;

class SoundCard : public Node {
public:
    CREATE_FUNC(SoundCard);
    virtual bool init() override;
    static int _index;
    
    void setType(string letter);
    
    void loadOnTrain(SoundTrain *train);
    void unload(Node *ground);
    void changePickUpState(bool picked);
    
    function<void()> onTouchBegan;
    function<void()> onTouchEnded;
    
    Sprite *_face, *_body, *_shadow;
    string _letter;
    SoundTrain *_targetTrain;
    Vec2 _originalPos;
    int _assignedIndex;
    map<int, int> _touching;

    bool _loading, _enableTouch;
    
};

END_NS_SOUNDTRAIN;
