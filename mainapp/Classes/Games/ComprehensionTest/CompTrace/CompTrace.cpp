//
//  CompTrace.cpp -- Comprehenshion tracing game with bonus video
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "CompTrace.h"
#include "Core/CompTraceScene.h"
#include <Managers/LanguageManager.hpp>


namespace {
}  // unnamed namespace


CompTrace::CompTrace() {

}



void CompTrace::setOnSuccess(std::function<void()> F) {
    this->OnSuccess = F;
}

void CompTrace::setOnFail(std::function<void()> F) {
    this->OnFail = F;
}

cocos2d::Scene* CompTrace::createScene() {
    
    auto It = createLayer(nullptr);

    auto scene = Scene::create();
    scene->addChild(It);
    return scene;
}

cocos2d::Layer* CompTrace::createLayer(ComprehensionScene* parent)
{
    using namespace todoschool::comptrace;
    
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    
    auto It = CompTraceScene::create();
    
    Problem p;
    

    {
        std::vector<std::string> rawData;
        
        for (int i = 0; i < parent->problemSet.size(); i++)
        {
            if (i == parent->getCurrentProblem())
            {
                rawData = parent->problemSet[i].second;
                break;
            }
        }
        
        p.TeaserFileName = rawData[2];
        p.Text = rawData[3];
        
    }
    
    
    It->TheTraceWork.update(p);
    It->OnSuccess = OnSuccess;
    It->OnFail = OnFail;
    It->_comprehensionScene = parent;
    return It;
    
}
