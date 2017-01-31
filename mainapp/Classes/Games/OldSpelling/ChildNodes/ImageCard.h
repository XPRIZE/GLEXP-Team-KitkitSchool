//
//  ImageCard.h -- Work image card
//  TodoSchool on Aug 24, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Models/Work.h"
#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING;

class ImageCard: public Node {
    typedef Node Super;
    
public:
    Chain<Work> TheWork;

public:
    CREATE_FUNC(ImageCard);
    bool init() override;

private:
    void clearInternals();
    void refreshChildNodes();
};

END_NS_SPELLING;