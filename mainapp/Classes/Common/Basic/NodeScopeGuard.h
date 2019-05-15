//
//  NodeScopeGuard.h -- Temporarily increase the reference count in the scope
//  TodoSchool on Sep 16, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "ScopeGuard.h"
#include <cocos/2d/CCNode.h>


class NodeScopeGuard {
    // NB(xenosoz, 2016): Weak reference to the node
    cocos2d::Node* TheNode;
    ScopeGuard TheScopeGuard;

public:
    NodeScopeGuard(cocos2d::Node* N);
};

