//
//  SoundToImageStage.h -- Listen to the sound, choose the right image
//  TodoSchool on Nov 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "Stage.h"
#include "../ChildNodes/SoundButton.h"


BEGIN_NS_DIGITALQUIZ

class SoundToImageStage : public Stage {
    typedef Stage Super;
    
    SoundButton* TheSoundButton;
    
public:
    CREATE_FUNC(SoundToImageStage);
    SoundToImageStage();
    bool init() override;
    
private:
    void clearInternals() override;
    void refreshChildNodes() override;
    
    void onEnter() override;
    void handleAnswer(const string& Answer);
};

END_NS_DIGITALQUIZ
