//
//  BookChoiceScene.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 11/3/16.
//
//

#include "BookChoiceScene.hpp"

#include "Managers/LanguageManager.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include <functional>

#include "CCAppController.hpp"
#include "Utils/TodoUtil.h"
#include "Common/Controls/PopupBase.hpp"


Scene* BookChoiceScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    scene->setContentSize(visibleSize);
    
    // 'layer' is an autorelease object
    auto layer = BookChoiceScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

Scene* BookChoiceScene::createSceneForBookWithQuiz()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    scene->setContentSize(visibleSize);
    
    // 'layer' is an autorelease object
    auto layer = BookChoiceScene::create();
    layer->forBookWithQuizScene = true;
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool BookChoiceScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void BookChoiceScene::onEnter()
{
    Layer::onEnter();
    
    removeAllChildren();
    
    Size winSize = Director::getInstance()->getWinSize();
    
    auto chooser = PopupBase::create(this, winSize);
    
    auto innerView = Node::create();
    
    float y = -80;
    
    auto list = FileUtils::getInstance()->listFiles("Books/BookData");
    
    stable_sort(list.begin(), list.end());
    
    for (auto folder : list) {
        string bookPath = folder + "bookinfo.csv";
        if (FileUtils::getInstance()->isFileExist(bookPath)) {
            auto button = ui::Button::create();
            button->setTitleFontSize(50);
            button->setTitleColor(Color3B::WHITE);
            auto book = TodoUtil::split(folder, '/').back();
            button->setTitleText(book);
            button->setPosition(Vec2(winSize.width/2, y-=80));
            innerView->addChild(button);
            
            button->addClickEventListener([this, book, button, chooser](Ref*) {
                chooser->dismiss(true);
                if (forBookWithQuizScene)
                {
                    CCAppController::sharedAppController()->startBookWithQuizScene(book);
                }
                else
                {
                    CCAppController::sharedAppController()->startBookScene(book, true);
                }
            });
            
        }
        
    }
    
    
    auto height = -y + 100;
    auto scroll = ui::ScrollView::create();
    scroll->setContentSize(winSize);
    scroll->setInnerContainerSize(Size(winSize.width, height));
    innerView->setPosition(Vec2(0, height));
    scroll->addChild(innerView);
    chooser->addChild(scroll);
    
    
    chooser->show(this, true);
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
}
