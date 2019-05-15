//
//  HiddenBackButton.h -- Hidden back button
//  TodoSchool on Nov 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#pragma once

#include "../Utils/DigitalQuizNS.h"
#include <Common/Basic/SoundEffect.h>


BEGIN_NS_DIGITALQUIZ

class HiddenBackButton: public Node {
    typedef Node Super;

public:
    function<void()> OnClick;
    
public:
    CREATE_FUNC(HiddenBackButton);
    HiddenBackButton();
    bool init() override;
    
private:
    void clearInternals();
    void refreshChildNodes();
};

END_NS_DIGITALQUIZ
