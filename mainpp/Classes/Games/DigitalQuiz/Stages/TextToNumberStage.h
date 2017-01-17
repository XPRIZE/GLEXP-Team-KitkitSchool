//
//  TextToNumberStage.h -- See the text, choose the right number
//  TodoSchool on Nov 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "Stage.h"


BEGIN_NS_DIGITALQUIZ

class TextToNumberStage : public Stage {
    typedef Stage Super;
    
public:
    CREATE_FUNC(TextToNumberStage);
    TextToNumberStage();
    bool init() override;
    
private:
    void clearInternals() override;
    void refreshChildNodes() override;
    
    void onEnter() override;
    void handleAnswer(const string& Answer);
};

END_NS_DIGITALQUIZ
