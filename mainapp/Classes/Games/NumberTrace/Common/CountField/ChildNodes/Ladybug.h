//
//  Ladybug.h
//  TodoSchool on Jun 30, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#pragma once

#include "CountObject.h"
#include "../Utils/CountFieldNS.h"


BEGIN_NS_COUNTFIELD

class Ladybug: public CountObject {
    typedef CountObject Super;
    int SkinID;
    
public:
    CREATE_FUNC(Ladybug);
    bool init(int SkinID);

    bool init() override;
    void clear() override;

public:
    static Size defaultSize();

private:
    void updateWalkSprite();
    void updateRestSprite();
};

END_NS_COUNTFIELD
