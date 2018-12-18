//
//  CountField.h on Sep 2, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MovingInsectsNS.h"
#include <Games/NumberTrace/Common/CountField/ChildNodes/CountObject.h>


BEGIN_NS_MOVINGINSECTS

using todoschool::countfield::CountObject;


class CountField: public Node {
    typedef Node Super;

    function<CountObject*()> ObjectCreatorA;
    function<CountObject*()> ObjectCreatorB;
    function<Sprite*()> ShadowCreator;

    // NB(xenosoz, 2016): Weak references
    vector<CountObject*> GroupA;
    vector<CountObject*> GroupB;

public:
    CREATE_FUNC(CountField);
    bool init() override;

    void appendGroups(size_t CountA, size_t CountB, float Duration);
    void appendGroupA(size_t CountA, size_t CountB, float Duration);
    void appendGroupB(size_t CountA, size_t CountB, float Duration);

    void removeGroups(float Duration);
    void removeGroupA(float Duration);
    void removeGroupB(float Duration);
    
    void shakeGroups(float Duration);
    void shakeGroupA(float Duration);
    void shakeGroupB(float Duration);
    
private:
    void clearInternals();
    void refreshChildNodes();
};

END_NS_MOVINGINSECTS
