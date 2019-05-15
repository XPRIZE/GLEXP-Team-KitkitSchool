//
//  NumberTraceExt.cpp -- Comprehenshion tracing game with bonus video
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "NumberTraceExt.h"
#include "Core/NumberTraceExtScene.h"
#include <Managers/LanguageManager.hpp>

namespace
{
    
}

NumberTraceExt::NumberTraceExt()
{

}

cocos2d::Scene* NumberTraceExt::createScene(int levelID)
{
    using namespace todoschool::numbertraceext;
    auto It = NumberTraceExtScene::create();
    Problem p;
    p.number = levelID * 10 - 9;
    It->setCurrentLevel(levelID);
    It->OnSuccess = OnSuccess;
    It->OnFail = OnFail;
    It->onStart();
    
    auto scene = Scene::create();
    scene->addChild(It);
    
    return scene;
}
