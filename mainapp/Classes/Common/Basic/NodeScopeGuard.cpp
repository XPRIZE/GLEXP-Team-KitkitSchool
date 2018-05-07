//
//  NodeScopeGuard.cpp -- Temporarily increase the reference count in the scope
//  TodoSchool on Sep 16, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "NodeScopeGuard.h"


NodeScopeGuard::NodeScopeGuard(cocos2d::Node* N)
    : TheNode(N)
    , TheScopeGuard([this] { if (TheNode) TheNode->retain(); },
                    [this] { if (TheNode) TheNode->release(); })
{
    // NB(xenosoz, 2016): Do nothing here.
}

