//
//  AnswerTextButton.h -- A large button with a text on it
//  TodoSchool on Nov 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#pragma once

#include "../Utils/MainNS.h"
#include <Games/NumberTrace/Common/Basic/BigLabel.h>


BEGIN_NS_DIGITALQUIZ

class AnswerTextButton: public Node {
public:
    enum class SizeKind {
        Small,
        Medium,
        Large,
    };
    
private:
    typedef Node Super;

    SizeKind TheSizeKind;
    Button* ButtonNode;
    todoschool::BigLabel* TextNode;
    
public:
    Chain<string> Text;
    Chain<bool> TouchEnabled;
    
    function<void(const string&)> OnAnswer;
    
public:
    CREATE_FUNC(AnswerTextButton);
    AnswerTextButton();
    bool init(/* SizeKind SK = Large */) override;
    bool init(SizeKind SK);
    
private:
    void clearInternals();
    void refreshChildNodes();
    
    void handleButtonDown();
    void handleButtonUp();
};

END_NS_DIGITALQUIZ
