//
//  TrailerNode.h
//  TodoSchool on Jul 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"


BEGIN_NS_LETTERTRACE

class TrailerNode: public Node {
    typedef Node Super;

public:
    Label* NextLabel;
    Button* NextButton;

    Chain<string> NextButtonText;
    
    function<void(TrailerNode*)> OnNextButtonClicked;
    
public:
    CREATE_FUNC(TrailerNode);
    TrailerNode();
    bool init() override;
    void clear();
    
private:
    void clearInternals();
    void updateNextLabel();
    void updateNextButton();

    void handleNextButtonTextValueUpdate(string&);
};

END_NS_LETTERTRACE