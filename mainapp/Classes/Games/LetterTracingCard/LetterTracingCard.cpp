//
//  LetterTracingCard.cpp
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "LetterTracingCard.h"
#include "Core/LetterTracingCardScene.h"
#include <Managers/LanguageManager.hpp>

LetterTracingCard::LetterTracingCard()
{

}

void LetterTracingCard::setOnSuccess(std::function<void()> F)
{
    this->OnSuccess = F;
}

void LetterTracingCard::setOnFail(std::function<void()> F)
{
    this->OnFail = F;
}

cocos2d::Scene* LetterTracingCard::createScene(int levelID)
{
    using namespace todoschool::lettertracingcard;
    auto It = LetterTracingCardScene::create();
    It->setCurrentLevel(levelID);
    It->OnSuccess = OnSuccess;
    It->OnFail = OnFail;
    It->onStart();
    
    auto scene = Scene::create();
    scene->addChild(It);
    return scene;
}
