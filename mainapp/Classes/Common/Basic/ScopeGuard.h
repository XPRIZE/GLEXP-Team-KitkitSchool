//
//  ScopeGuard.h -- Function caller for the end of a scope
//  TodoSchool on Aug 19, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

// NB(xenosoz, 2016): It's a minimal implementation of ScopeGuard.


#pragma once

#include <functional>



class ScopeGuard {
    std::function<void()> EndFn;

public:
    // NB(xenosoz, 2016): Register EndFn to called when the scope ends.
    ScopeGuard(std::function<void()> EndFn);

    // NB(xenosoz, 2016): This constructor was added for a stylistic reason. BeginFn is just called immediately.
    ScopeGuard(std::function<void()> BeginFn,
               std::function<void()> EndFn);

    ~ScopeGuard();
    
    void dismiss();
};


