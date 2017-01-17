//
//  RawContactBody.h -- Component for listening physics contact messages
//  TodoSchool on Aug 16, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

// NB(xenosoz, 2016): It seems working only with dynamic physics objects.


#pragma once

#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING;

class RawContactBody: public Component {
    typedef Component Super;
    EventListener* Listener;
    
public:
    FunctionVector<bool(PhysicsContact*)> OnContactDidBegin;
    FunctionVector<bool(PhysicsContact*, PhysicsContactPreSolve*)> OnContactWillSolve;
    FunctionVector<void(PhysicsContact*, const PhysicsContactPostSolve*)> OnContactDidSolve;
    FunctionVector<void(PhysicsContact*)> OnContactDidEnd;
    
public:
    CREATE_FUNC(RawContactBody);
    RawContactBody();
    bool init() override;
    
    static string defaultName();
    
private:
    void attachEventListener();
    void detachEventListener();
    
    void onEnter() override;
    void onExit() override;
    void onAdd() override;
    void onRemove() override;
};

END_NS_SPELLING;