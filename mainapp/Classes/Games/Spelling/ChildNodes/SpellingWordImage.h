//
//  WordImage.h -- The image part of the problem
//  TodoSchool on Sep 16, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Models/SpellingProblem.h"
#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING

class WordImage: public Node {
    typedef Node Super;
    SoundEffect SoundForWord;
    bool IsSoundPlaying;

public:
    Chain<Problem> TheProblem;
    
public:
    CREATE_FUNC(WordImage);
    bool init() override;

private:
    void clearInternals();
    void refreshComponents();
    void refreshChildNodes();
    
    void playWordSound();
};



END_NS_SPELLING
