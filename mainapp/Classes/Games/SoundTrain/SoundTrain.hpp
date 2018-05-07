//
//  SoundTrain.hpp
//  KitkitSchool
//
//  Created by JungJaehun on 04/09/2017.
//
//

#pragma once
#include "SoundTrainHeader.h"


BEGIN_NS_SOUNDTRAIN;

class SoundCard;

class SoundTrain : public Node {
public:
    CREATE_FUNC(SoundTrain);
    virtual bool init() override;
    
    void setType(string letter);
    void toggleConnector();
    
    SoundCard *_cardInTrain;
    Sprite *_trainBody, *_connector;
    Vec2 _slotPos;
    string _correctAnswer;
    bool _connectorStatus;
};

END_NS_SOUNDTRAIN;
