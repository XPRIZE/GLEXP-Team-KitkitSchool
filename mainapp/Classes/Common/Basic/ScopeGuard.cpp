//
//  ScopeGuard.h -- Function caller for the end of a scope
//  TodoSchool on Aug 19, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "ScopeGuard.h"


ScopeGuard::ScopeGuard(std::function<void()> EndFn)
    : EndFn(EndFn)
{
    // NB(xenosoz, 2016): Do nothing.
}

ScopeGuard::ScopeGuard(std::function<void()> BeginFn,
                       std::function<void()> EndFn)
    : EndFn(EndFn)
{
    BeginFn();
}
    
ScopeGuard::~ScopeGuard() { if (EndFn) { EndFn(); } }
    
void ScopeGuard::dismiss() { EndFn = nullptr; }


