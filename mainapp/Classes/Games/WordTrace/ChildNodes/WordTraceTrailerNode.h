//
//  TrailerNode.h
//  TodoSchool on Jul 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Models/WordTraceProblem.h"
#include "../Utils/WordTraceNS.h"
#include <Common/ADT/Chain.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <functional>


BEGIN_NS_WORDTRACE

class TrailerNode: public cocos2d::Node {
    
public:
    CREATE_FUNC(TrailerNode);
    
    Label* NextLabel;
    Button* NextButtonA;
    Button* NextButtonB;
    Label* NextButtonLabelA;
    Label* NextButtonLabelB;
    
    Chain<Problem> NextTraceWorkA;
    Chain<Problem> NextTraceWorkB;
    
    // NB(xenosoz, 2016): Pass the selected problem.
    std::function<void(TrailerNode&, Problem)> OnNextButtonClicked;
    bool mbClicked;
public:
    bool init() override;
    void clear();
    
private:
    void clearInternals();
    void refreshNextLabel();
    void refreshNextButton();
};

END_NS_WORDTRACE

