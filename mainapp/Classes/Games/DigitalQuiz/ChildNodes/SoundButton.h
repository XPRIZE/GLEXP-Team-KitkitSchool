//
//  SoundButton.h -- A button with speaker mark on it
//  TodoSchool on Nov 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#pragma once

#include "../Utils/DigitalQuizNS.h"
#include <Common/Basic/SoundEffect.h>


BEGIN_NS_DIGITALQUIZ

class SoundButton: public Node {
    Button* ButtonNode;

public:
    typedef Node Super;
    enum class SizeKind {
        Small,
        Large,
    };

    SizeKind TheSizeKind;
    SoundEffect SoundToPlay;

    Chain<bool> TouchEnabled;
    function<void()> OnClicked;

public:
    CREATE_FUNC(SoundButton);
    SoundButton();
    virtual ~SoundButton();

    bool init(/* SizeKind::Large */) override;
    bool init(SizeKind SK);
    
    void play();
    void stop();
    
private:
    void clearInternals();
    void refreshChildNodes();
};

END_NS_DIGITALQUIZ
