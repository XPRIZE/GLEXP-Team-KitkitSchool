//
//  SoundToNumberStage.h -- Listen to the sound, choose the right number
//  TodoSchool on Nov 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

// NB(xenosoz, 2016): It's very similar with SoundToTextStage.
//   But there's a chance we should use a calculator-shaped keypad.
//   So I didn't merge it to SoundToTextStage.

// NB(xenosoz, 2016): Number choices are not sorted as well.


#pragma once

#include "Stage.h"
#include "../ChildNodes/SoundButton.h"


BEGIN_NS_DIGITALQUIZ

class SoundToNumberStage : public Stage {
    typedef Stage Super;
    
    SoundButton* TheSoundButton;

public:
    CREATE_FUNC(SoundToNumberStage);
    SoundToNumberStage();
    bool init() override;
    
private:
    void clearInternals() override;
    void refreshChildNodes() override;
    
    void onEnter() override;
    void handleAnswer(const string& Answer);
};

END_NS_DIGITALQUIZ
