//
//  LargeImageButton.h -- A large button with an image on it
//  TodoSchool on Nov 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#pragma once

#include "../Utils/MainNS.h"


BEGIN_NS_DIGITALQUIZ

class LargeImageButton: public Node {
    typedef Node Super;

    Button* ButtonNode;
    Sprite* ImageNode;
    
public:
    Chain<string> ImageName;
    Chain<bool> TouchEnabled;

    function<void(const string&)> OnAnswer;
    
public:
    CREATE_FUNC(LargeImageButton);
    LargeImageButton();
    bool init() override;
    
private:
    void clearInternals();
    void refreshChildNodes();
    
    void handleButtonDown();
    void handleButtonUp();
};

END_NS_DIGITALQUIZ
