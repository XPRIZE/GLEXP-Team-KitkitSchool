//
//  SoundToTextStage.h -- Listen to the sound, choose the right text.
//  TodoSchool on Nov 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "Stage.h"
#include "../ChildNodes/SoundButton.h"


BEGIN_NS_DIGITALQUIZ

class SoundToTextStage : public Stage {
    typedef Stage Super;
    
    SoundButton* TheSoundButton;

public:
    CREATE_FUNC(SoundToTextStage);
    SoundToTextStage();
    bool init() override;
    
private:
    void clearInternals() override;
    void refreshChildNodes() override;
    
    void onEnter() override;
    void handleAnswer(const string& Answer);
};

END_NS_DIGITALQUIZ
