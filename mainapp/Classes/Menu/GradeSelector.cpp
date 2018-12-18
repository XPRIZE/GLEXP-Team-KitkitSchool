//
//  GradeSelector.cpp -- Temporary grade selector for debugging
//  TodoSchool on Oct 20, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "GradeSelector.hpp"
#include <Common/Basic/SoundEffect.h>
#include <Common/Controls/TodoLoadingScene.hpp>
#include <Common/Controls/TodoSchoolBackButton.hpp>
#include "Common/Basic/DeviceSpec.h"
#include "Common/Basic/NodeScopeGuard.h"
#include <Utils/TodoUtil.h>
#include <cocos/ui/UIButton.h>
#include "Managers/UserManager.hpp"

#include "CCAppController.hpp"

using namespace cocos2d;
using namespace std;



namespace {
    const string defaultFont("fonts/TodoSchoolV2.ttf");
    const float defaultFontSize(100.f);
    

    Size windowSize() {
        return todoschool::DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
    }
    
    Size gameSize() {
        return todoschool::DeviceSpec::Google_Pixel_C().FullScreenResolution;
    }

}  // unnamed namespace


GradeSelector::GradeSelector() {
}

bool GradeSelector::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();

    return true;
}

void GradeSelector::setGameName(std::string gameName) {
    gameName_ = gameName;
}

void GradeSelector::setChoices(const std::vector<int> choices) {
    _choicesStr.clear();
    _choicesInt = choices;
    refreshChildNodes();
}
void GradeSelector::setChoices(const std::vector<std::string> choices) {
    _choicesInt.clear();
    _choicesStr = choices;
    refreshChildNodes();
}

cocos2d::Scene* GradeSelector::minimalSceneByWrapping() {
    removeAllChildren();
    auto scene = Scene::create();
    
    _winSize = Director::getInstance()->getWinSize();
    auto bg = Sprite::create("MainScene/bg_ground.jpg");
    auto bgSize = bg->getContentSize();
    auto bgScale = MAX(_winSize.width / bgSize.width, _winSize.height / bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(_winSize/2);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    scene->addChild(bg);
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, _winSize.height-25));
    scene->addChild(backButton);
    
    auto label = Label::createWithTTF("Choose a level to play (" + gameName_ + ")",
                                   defaultFont, defaultFontSize);
    
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    label->setPosition(_winSize.width/2, _winSize.height-150);
    scene->addChild(label);
    
    bool levelIDisInteger = false;
    
    if (_choicesInt.size()>0) {
        levelIDisInteger = true;
        for (auto it : _choicesInt) {
            _choicesStr.push_back(TodoUtil::itos(it));
        }
    }
    
    auto node = Node::create();
    scene->addChild(node);
    node->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    node->setPosition(_winSize.width*0.05,_winSize.height-400);


    {
        bool isWorksheetTest = UserManager::getInstance()->isWorksheetTestMode();

        ui::Button* worksheetTestButton = ui::Button::create();
        auto txt = isWorksheetTest ? "worksheet all: on" : "worksheet all: off";
        worksheetTestButton->setTitleText(txt);
        worksheetTestButton->setTitleFontSize(50);

        worksheetTestButton->setPosition(Vec2(_winSize.width-300, _winSize.height-300));
        worksheetTestButton->addTouchEventListener([worksheetTestButton, this](Ref*,ui::Widget::TouchEventType e) {
            if (e == ui::Widget::TouchEventType::ENDED) {
                bool isWorksheetTest = UserManager::getInstance()->isWorksheetTestMode();
                auto txt = !isWorksheetTest ? "worksheet all: on" : "worksheet all: off";
                worksheetTestButton->setTitleText(txt);
                UserManager::getInstance()->setWorksheetTestMode(!isWorksheetTest);
            }
        }  );
        scene->addChild(worksheetTestButton);
    }


    float posX = 0.f, posY = 0.f;
    float width = 0.f, height = 0.f;
    
    int levelCount = -1;
    int columnCount = 0;

    string gameName = gameName_;
    
    int specialTestCount=0;
    int fishQuizCount=0;
 
    for (auto it : _choicesStr) {

        bool newLine = false;

        auto itVec = TodoUtil::split(it, '_');
        
        if (itVec[0] == "specialtest") {
            if (specialTestCount%10==0) newLine = true;
            specialTestCount++;
        } else if (itVec[0] == "fishtest") {
            if (fishQuizCount%10==0) newLine = true;
            fishQuizCount++;
        } else if (itVec.size()>1) {
            auto currentLevel = TodoUtil::stoi(itVec[1]);
            CCLOG("currentLevel: %d", currentLevel);
            CCLOG("levelCount: %d", levelCount);
            if (levelCount != -1 && levelCount != currentLevel) newLine = true;
            levelCount = currentLevel;
        } else {
            columnCount++;
            if (columnCount !=1 && columnCount%10 == 1) newLine = true;
        }

        auto button = ui::Button::create();
        button->setTitleFontSize(200.f);
        button->setTitleFontName(defaultFont);
        button->setColor(Color3B::WHITE);
        button->setTitleText(" "+it+" ");
        button->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        Size buttonSize = button->getContentSize();
        button->setTitleAlignment(cocos2d::TextHAlignment::CENTER);
        
        if (newLine){
            CCLOG("new line!");
            posX = 0.f;
            posY -= buttonSize.height * 2;
            if (-posY > height) height = -posY;
        }
        
        button->setPosition(Vec2(posX, posY));
        button->addTouchEventListener([this, it, gameName, levelIDisInteger, scene](Ref*,ui::Widget::TouchEventType e) {
            if (e == ui::Widget::TouchEventType::ENDED) {
                if (gameName == "ReadingBird") {
                    cocos2d::ui::EditBox* editBox = (cocos2d::ui::EditBox*)(scene->getChildByTag(1000)->getChildByTag(100));
                    if (editBox != nullptr) {
                        int val = TodoUtil::stoi(string(editBox->getText()));
                        
                        if (val != INT_MIN) {
                            UserDefault::getInstance()->setIntegerForKey("TRIGGER_VOL", val);
                        }
                    }
                }
                
                if (levelIDisInteger) handleSelection(gameName, TodoUtil::stoi(it));
                else handleSelection(gameName, it);
            }
        });
        node->addChild(button);
        posX += buttonSize.width * 1.1;
        if (posX > width) width = posX;
    }
    
    if (gameName == "ReadingBird")
    {
        const int DEFAULT_TRIGGER_VOLUME = 110;
        
        Node* nodeVolume = Node::create();
        auto label = Label::createWithTTF("Trigger Volume: ",
                                          defaultFont, defaultFontSize);
        label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        label->setPosition(Vec2::ZERO);
        nodeVolume->addChild(label);
        
        auto editVolume = cocos2d::ui::EditBox::create(Size(200, label->getContentSize().height), cocos2d::ui::Scale9Sprite::create());
        editVolume->setTag(100);
        editVolume->setFont(defaultFont.c_str(), defaultFontSize);
        editVolume->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        editVolume->setPosition(Vec2(label->getContentSize().width, 0));
        editVolume->setMaxLength(3);
        editVolume->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);
        editVolume->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
        int triggerVolume = UserDefault::getInstance()->getIntegerForKey("TRIGGER_VOL", DEFAULT_TRIGGER_VOLUME);
        editVolume->setText(TodoUtil::itos(triggerVolume).c_str());
        nodeVolume->addChild(editVolume);

        nodeVolume->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        nodeVolume->setContentSize(Size(label->getContentSize().width + editVolume->getContentSize().width / 2, label->getContentSize().height));
        nodeVolume->setPosition(Vec2(_winSize.width / 2 - nodeVolume->getContentSize().width / 2, 200));
        nodeVolume->setTag(1000);
        scene->addChild(nodeVolume);
    }

    float scale = MIN(_winSize.width / width, (_winSize.height-600) / height) * 0.9;

    if (scale < 1) node->setScale(scale);
    
    return scene;
    
}

cocos2d::Scene* GradeSelector::createScene() {
    auto scene = Scene::create();
    auto layer = GradeSelector::create();
    scene->addChild(layer);
    return scene;
    // return it->minimalSceneByWrapping();
}

void GradeSelector::clearInternals() {
    
}

void GradeSelector::refreshChildNodes() {
    removeAllChildren();
    
}

void GradeSelector::handleSelection(std::string gameName, int choice) {
    CCLOG("gameName %s", gameName.c_str());
    CCAppController::sharedAppController()->startGame(gameName, choice);
}

void GradeSelector::handleSelection(std::string gameName, string choice) {
    CCLOG("gameName %s", gameName.c_str());
    CCAppController::sharedAppController()->startGame(gameName, 0, choice);
}

