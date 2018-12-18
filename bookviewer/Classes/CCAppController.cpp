//
//  CCAppController.cpp
//  booktest
//
//  Created by Gunho Lee on 11/14/16.
//
//

#include "CCAppController.hpp"
#include "cocos2d.h"

using namespace std;
USING_NS_CC;


static CCAppController *instance;

CCAppController *CCAppController::sharedAppController() {
    if (instance == nullptr) {
        instance = new CCAppController;
    }
    return instance;
}


void CCAppController::handleGameQuit(bool bImmediately)
{
    (Director::getInstance())->popScene();
}

void CCAppController::handleGameComplete(int result)
{
    (Director::getInstance())->popScene();
}
