//
//  TargetTextNode.h on Jul 26, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/StarFallNS.h"


BEGIN_NS_STARFALL

class TargetTextNode: public Node {
    typedef Node Super;
    Label* TextLabel;

public:
    Chain<bool> GamePlaying;
    Chain<string> TitleText;
    Chain<string> ActiveText;
    Chain<Point> StartPosition;
    Chain<Point> StopPosition;
    Chain<float> StartPhase;
    Chain<float> StopPhase;
    Chain<float> SwingRatio;
    Chain<float> Duration;
    Chain<float> Clock;
    
    function<void(TargetTextNode&)> OnMatched;
    function<void(TargetTextNode&)> OnTimeOver;

public:
    CREATE_FUNC(TargetTextNode);
    TargetTextNode();
    bool init() override;

public:
    static Size defaultSize();

private:
    void onEnter() override;
    void onExit() override;
    void update(float DeltaSeconds) override;
    
    void clearInternals();
    void refreshChildNodes();

    void checkForTextMatch();
    void checkForTimeOver();
};

END_NS_STARFALL
