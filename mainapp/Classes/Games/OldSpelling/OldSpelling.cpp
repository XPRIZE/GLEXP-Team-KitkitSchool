//
//  OldSpelling.cpp -- Spell-filling game with balls
//  TodoSchool on Aug 10, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "OldSpelling.h"
#include "Core/SpellingScene.h"


cocos2d::Scene* OldSpelling::createScene() {
    using namespace todoschool::oldspelling;
    
    auto It = SpellingScene::createScene();
    return It;
}
