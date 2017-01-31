//
//  FunStage.h -- A refresher for children
//  TodoSchool on Nov 3, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "Stage.h"
#include <Common/Basic/SoundEffect.h>


BEGIN_NS_DIGITALQUIZ

class FunStage : public Stage {
    typedef Stage Super;
    
    SoundEffect SoundToPlay;
    
public:
    CREATE_FUNC(FunStage);
    FunStage();
    bool init() override;
    
private:
    void clearInternals() override;
    void refreshChildNodes() override;
    
    void onEnter() override;
    void handleAnswer(const string& Answer);
};

END_NS_DIGITALQUIZ
