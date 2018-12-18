//
//  FishQuizPopup.cpp
//  KitkitSchool
//
//  Created by YongSoo Hong on 9/3/18.
//
//

#include <Menu/CoopScene.hpp>
#include "FishQuizPopup.hpp"
#include "ui/CocosGUI.h"


#include "Managers/UserManager.hpp"
#include "Managers/LanguageManager.hpp"
#include "Managers/GameSoundManager.h"

#include "Utils/TodoUtil.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "../../Menu/CoopScene.hpp"
#include "../../Menu/Fish.hpp"
#include "EggQuizScene.hpp"

#include <time.h>

namespace FishQuizPopupSpace {
    const std::string JNI_CLASS_NAME = "org/cocos2dx/cpp/AppActivity";
    const string curlyFont = "fonts/TodoMainCurly.ttf";
    
    class SwapNode : public ActionInterval {
    public:
        static SwapNode* create(float duration, int blinks, Node* node0, Node* node1);
        virtual void update(float time) override;
        virtual void startWithTarget(Node *target) override;
        virtual void stop() override;
        
    private:
        int _times;
        Node* _node0;
        Node* _node1;
        int _effectID;
        bool initWithDuration(float duration, int blinks, Node* node0, Node* node1);
    };
}

using namespace FishQuizPopupSpace;
using namespace cocos2d::ui;

const string partsPath = "eggquiz/parts/";
const string soundsPath = "eggquiz/sounds/";

FishQuizPopup* FishQuizPopup::create(Node* parent)
{
    FishQuizPopup *popup = new (std::nothrow) FishQuizPopup();
    if (popup && popup->init(parent))
    {
        popup->autorelease();
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}

FishQuizPopup* FishQuizPopup::create(Node* parent, Size size)
{
    FishQuizPopup *popup = new (std::nothrow) FishQuizPopup();
    if (popup && popup->init(parent, size))
    {
        popup->autorelease();
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}

void FishQuizPopup::dismiss(bool animate) {
    PopupBase::dismiss(animate);
    for (auto fish : _fishData) {
        delete fish;
    }
    _fishData.clear();
}

Node* FishQuizPopup::createGlow()
{
    Node *node = Node::create();
    
    {
        auto sp = Sprite::create(partsPath+"popup_window_glow_toleft.png");
        node->addChild(sp);
        
        sp->runAction(RepeatForever::create(RotateBy::create(0.5, 30)));
    }
    
    {
        auto sp = Sprite::create(partsPath+"popup_window_glow_toright.png");
        node->addChild(sp);

        sp->runAction(RepeatForever::create(RotateBy::create(0.5, -30)));
    }
    
    {
        auto sp = Sprite::create(partsPath+"popup_window_glow.png");
        node->addChild(sp);
    }

    return node;
}



Button* FishQuizPopup::createButton(string prefix, string text) {
    auto btn = Button::create(prefix+"_normal.png", prefix+"_active.png");
    
    
    if (text.length()>0) {
        auto localTxt = LanguageManager::getInstance()->getLocalizedString(text);
        auto l = TodoUtil::createLabel(localTxt, 60, Size::ZERO, curlyFont, Color4B(255, 252, 236, 255));
        auto lPos = btn->getContentSize()/2 + Size(0, 10);
        l->setPosition(lPos);
        btn->addChild(l);
        
        btn->addTouchEventListener([btn, lPos, l](Ref*,Widget::TouchEventType) {
            if (btn->isHighlighted()) l->setPosition(lPos + Size(0, -10));
            else l->setPosition(lPos);
        });
    }
    
    return btn;
}

void FishQuizPopup::setFishCompleted(char category, int progressLevelIndex, int progressIndex, int score)
{
    CCLOG("FishQuizPopup - category : %c, progressLevelIndex : %d, progressLevelIndex : %d, score : %d", category, progressLevelIndex, progressIndex, score);
    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    int maxClearLevel = 0;
    for (int i = CoopScene::LEVEL_COUNT_REGULAR_EGG - 1; i >= 0; --i) {
        auto levelID = CurriculumManager::getInstance()->makeLevelID(lang, category, i);
        if (UserManager::getInstance()->isLevelCleared(levelID)) {
            maxClearLevel = i;
            break;
        }
    }

    CCLOG("FishQuizPopup - maxClearLevel : %d", maxClearLevel);
    auto levelID = CurriculumManager::getInstance()->makeLevelID(lang, category, CoopScene::LEVEL_FISH_PRESENT);
    UserManager::getInstance()->setFishPresentCurrentProgressIndex(levelID, progressLevelIndex, progressIndex + 1);

    CCLOG("FishQuizPopup levelID : %s", levelID.c_str());
    UserManager::getInstance()->setFishPresentEnable(levelID, false);

    if (score > 60) {
        UserManager::getInstance()->setFishPresentCurrentProgressLevel(levelID, (progressLevelIndex + 1) % (maxClearLevel + 1));
        showSuccess(category, progressLevelIndex, progressIndex, score);
        
    } else {
        if (EggQuiz::EggQuizScene::getTryCountFishTest() != 0) {
            UserManager::getInstance()->setFishPresentCurrentProgressLevel(levelID, (progressLevelIndex + 1) % (maxClearLevel + 1));
        }
        showFail(category, progressLevelIndex, progressIndex, score);
        
    }
}

void FishQuizPopup::showSuccess(char category, int progressLevelIndex, int progressIndex, int score) {
    auto winSize = getContentSize();
    
    auto playWowSound = [this]() {
        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/pretest_success.m4a");
        
        this->runAction(Sequence::create(DelayTime::create(1.0),
                                         CallFunc::create([](){
            SoundEffect::wowEffect().play();
            GameSoundManager::getInstance()->playBGM(soundsPath + "congratseeyouseaworld.m4a");
        }),
                                         nullptr
                                         ));
    };
    
    if (score < 100) {
        playWowSound();
    }
    
    auto bg = Sprite::create(partsPath+"pretest_image_fishbowl_result_01.png");
    auto bgCenter = winSize / 2 + Size(0, 70);
    auto fishCenter = bgCenter + Size(0, bg->getContentSize().height / 2 - 162 - 412 / 2);
    
    auto glow = createGlow();
    glow->setPosition(fishCenter);
    this->addChild(glow);
    
    if (score == 100) {
        glow->setVisible(false);
    }
    
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(bgCenter);
    this->addChild(bg);

    auto playParticle = [this, winSize, fishCenter]() {
        for (int i=0; i<60; i++) {
            auto file = partsPath+StringUtils::format("popup_effect_confetti_%d.png", random(1, 8));
            auto sp = Sprite::create(file);
            auto posNode = Node::create();
            
            posNode->addChild(sp);
            
            posNode->setPosition(fishCenter);
            
            auto rotateAction = RotateBy::create(0.3, Vec3(random(-30.f, 30.f), random(-30.f, 30.f), random(-30.f, 30.f)));
            sp->runAction(RepeatForever::create(rotateAction));
            
            auto pos = Vec2(random<float>(winSize.width*0.15, winSize.width*0.85), random(winSize.height*0.4, winSize.height*0.9));
            auto speed = random(150.f, 250.f);
            float delay = random(0.0, 0.2);
            posNode->runAction(Sequence::create(DelayTime::create(delay), EaseOut::create(MoveTo::create(random(0.3, 0.5), pos), 2.0), MoveTo::create(pos.y / speed, Vec2(pos.x, -100)), nullptr));
            
            this->addChild(posNode);
        }
    };
    
    auto localTxt = LanguageManager::getInstance()->getLocalizedString("Congratulations!\nSee you at your sea world!");
    auto splits = TodoUtil::split(localTxt, '\n');
    auto l = TodoUtil::createLabel(localTxt, splits.size() > 2 ? 45 : 50, Size::ZERO, curlyFont, Color4B(255, 252, 236, 225));
    l->setAlignment(TextHAlignment::CENTER);
    l->setPosition(bgCenter + Size(0, -320));
    this->addChild(l);
    
    auto btn = createButton(partsPath+"popup_window_check", "");
    
    btn->addClickEventListener([this](Ref*){
        this->dismiss(true);
    });
    
    btn->setPosition(Vec2(winSize.width/2, winSize.height-1375));
    this->addChild(btn);
    
    if (score == 100) {
        l->setVisible(false);
        btn->setEnabled(false);
    }

    {
        loadFishDataFromDB();
        int skin = 1;
        
        auto fishID = Fish::getFishID(category, progressLevelIndex + 1, false);
        skin = generateFishSkin(fishID);
        CCLOG("monts skin : %d", skin);

        auto fish = Fish::create(category, progressLevelIndex + 1, false, skin);
        fish->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        fish->setContentSize(Size(520, 412));
        fish->setPosition(fishCenter);
        this->addChild(fish);

        if (score == 100) {
            auto fish2 = Fish::create(category, progressLevelIndex + 1, true);
            fish2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            fish2->setContentSize(Size(520, 412));
            fish2->setPosition(fishCenter);
            this->addChild(fish2);
            
            fish->setVisible(true);
            fish2->setVisible(false);
            
            int swapCount = random(30, 33);
            runAction(Sequence::create(DelayTime::create(1.0f), EaseIn::create(SwapNode::create(8.0f, swapCount, fish, fish2), 0.15),
                                       CallFunc::create([this, fish, fish2, glow, playWowSound, playParticle, l, btn](){
                glow->setVisible(true);
                playWowSound();
                playParticle();
                l->setVisible(true);
                btn->setEnabled(true);
                this->addFishToDB(fish->isVisible() ? fish : fish2);
                
            }), nullptr));
        } else {
            addFishToDB(fish);
        }
    }
    
    if (score < 100) {
        playParticle();
    }
}

void FishQuizPopup::showFail(char category, int progressLevelIndex, int progressIndex, int score) {
    bool bRetry = EggQuiz::EggQuizScene::getTryCountFishTest() == 0;
    
    auto winSize = getContentSize();
    if (bRetry) {
        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/sfx_grow.m4a");
        GameSoundManager::getInstance()->playBGM(soundsPath + "dontgiveupletstryagain.m4a");
    } else {
        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/pretest_fail1.m4a");
        GameSoundManager::getInstance()->playBGM(soundsPath + "tryagaintoaddmeseaworld.m4a");

    }
    
    auto bg = Sprite::create(partsPath+"popup_window_bg.png");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    bg->setPosition(Vec2(winSize.width/2, winSize.height));
    this->addChild(bg);
    this->showFromTop = true;
    
    int level = progressLevelIndex;
    _cur = CurriculumManager::getInstance()->findCurriculum(category, level);
    
    string panelFilename = category=='L' ? "daily_window_title_panel_english_.png" : "daily_window_title_panel_math.png";
    if (level==0) panelFilename = "daily_window_title_panel_prek.png";
    
    auto panel = Sprite::create("MainScene/DaySelect/"+panelFilename);
    panel->setPosition(winSize.width/2, winSize.height-320);
    this->addChild(panel);
    
    auto panelLabel = TodoUtil::createLabel(_cur->levelTitle, 70, Size::ZERO, curlyFont, Color4B(255, 252, 236, 255));
    panelLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    panelLabel->setPosition(panel->getContentSize()/2);
    panel->addChild(panelLabel);
    
    {
        auto fishBowl = Sprite::create(partsPath + (bRetry ? "pretest_image_fishbowl_result_02_colour.png" : "pretest_image_fishbowl_result_02.png"));
        fishBowl->setPosition(bg->getContentSize() / 2 + Size(0, 0));
        bg->addChild(fishBowl);
        
        auto fish = Fish::create(category, progressLevelIndex + 1, false, 1, !bRetry);
        fish->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        fish->setContentSize(Size(420, 346));
        fish->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        fish->setPosition(Vec2(fishBowl->getContentSize().width / 2, fishBowl->getContentSize().height - 162));
        fishBowl->addChild(fish);
    }
    
    {
        auto s1 = LanguageManager::getInstance()->getLocalizedString((bRetry ? "Don't give up! Let's try it again!": "Try again to add me to your sea world!"));
        auto l = TodoUtil::createLabel(s1, 50, Size::ZERO, curlyFont,
                                       Color4B(115, 61, 19, 225), TextHAlignment::CENTER);
        l->setPosition(Vec2(winSize.width / 2, winSize.height - 1120));
        this->addChild(l);
    }
    
    {
        auto btn = createButton(partsPath+(bRetry ? "popup_window_check" : "pretest_button_back"), "");
        
        btn->addClickEventListener([this](Ref *) {
            this->dismiss(true);
            
        });
        
        btn->setPosition(Vec2(winSize.width / 2, winSize.height - 1320));
        this->addChild(btn);
    }
                                                                     
    EggQuiz::EggQuizScene::increaseTryCountFishTest();
}

void FishQuizPopup::loadFishDataFromDB() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto strFishes = JniHelper::callStaticStringMethod(JNI_CLASS_NAME, "getFishes");
#else
    string strFishes = "[]";
    string path = FileUtils::getInstance()->getWritablePath() + "KitkitSchool";
    if (FileUtils::getInstance()->isFileExist(path+"/fish_sample.txt")) {
        strFishes = FileUtils::getInstance()->getStringFromFile(path+"/fish_sample.txt");
    }
#endif

    CCLOG("getFishes : \n%s", strFishes.c_str());
    Json::Value values = Json::Value();
    Json::Reader jsonReader;
    jsonReader.parse(strFishes, values);
    
    CCLOG("values size : %d", values.size());
    for (int i = 0; i < values.size(); ++i) {
        FISH_DATA* fishData = new FISH_DATA();
        fishData->_id = values[i].get("_id", 0).asInt();
        fishData->_userName = values[i].get("_userName", "").asString().c_str();
        fishData->_fishID = values[i].get("_fishID", "").asString().c_str();
        fishData->_skinNo = values[i].get("_skinNo", "").asString().c_str();
        fishData->_name = values[i].get("_name", "").asString().c_str();
        fishData->_createTime = values[i].get("_createTime", "").asString().c_str();
        
        struct tm tm;
        strptime(fishData->_createTime.c_str(), "%Y%m%d%H%M%S", &tm);
        fishData->_time = mktime(&tm);
        CCLOG("monts : fish id = %d, create = %s, time = %ld", fishData->_id, fishData->_createTime.c_str(), fishData->_time);
        
        fishData->_position = values[i].get("_position", "").asString().c_str();
        _fishData.push_back(fishData);
        
        CCLOG("%s", fishData->toString().c_str());
    }
}

void FishQuizPopup::addFishToDB(Fish* fish) {
    auto position = generateFishPosition(fish->getID());
    CCLOG("monts : FishQuizPopup fishID = %s, skin = %d, position = %s", fish->getID().c_str(), fish->getSkin(), position.c_str());
    auto dbFish = findDeleteFish(fish->getID().c_str(), position);
    if (dbFish != nullptr) {
        CCLOG("monts : FishQuizPopup delete id : %d, fishID = %s, createTime : %s, position = %s", dbFish->_id, dbFish->_fishID.c_str(), dbFish->_createTime.c_str(), dbFish->_position.c_str());
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (dbFish != nullptr) {
        JniHelper::callStaticBooleanMethod(JNI_CLASS_NAME, "deleteFish", dbFish->_id);
    }
    
    JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "addFish", fish->getID().c_str(), fish->getSkin(), "", position);
#else
#endif
    
}

FISH_DATA* FishQuizPopup::findDeleteFish(string fishID, string position) {
    // Fish
    if (TodoUtil::startsWith(fishID, "f")) {
        const int MAX_FISH_COUNT = 20;
        int fishCount = 0;
        long oldTime = LONG_MAX;
        
        FishData * oldData = nullptr;
        
        for (auto fishData : _fishData) {
            if (TodoUtil::startsWith(fishData->_fishID, "f")) {
                ++fishCount;
                
                if (oldTime > fishData->_time) {
                    oldTime = fishData->_time;
                    oldData = fishData;
                }
            }
        }
        
        CCLOG("monts : fish count = %d", fishCount);
        if (fishCount >= MAX_FISH_COUNT) {
            CCLOG("monts : delete id = %d, fishID = %s, createTime = %s", oldData->_id, oldData->_fishID.c_str(), oldData->_createTime.c_str());
            return oldData;
        }
    }
    // Object
    else {
        for (auto fishData : _fishData) {
            if (position == fishData->_position) {
                CCLOG("monts : delete id = %d, fishID = %s, position = %s", fishData->_id, fishData->_fishID.c_str(), fishData->_position.c_str());
                return fishData;
            }
        }
    }
    
    return nullptr;
}

int FishQuizPopup::generateFishSkin(string fishID) {
    bool isSkin1 = false;
    bool isSkin2 = false;
    
    for (auto fishData : _fishData) {
        if (fishID == fishData->_fishID) {
            int skin = TodoUtil::stoi(fishData->_skinNo);
            if (skin == 1) {
                isSkin1 = true;
            } else if (skin == 2) {
                isSkin2 = true;
            }
        }
    }
    
    CCLOG("monts : isSkin1 = %d, isSkin2 = %d", isSkin1, isSkin2);
    if (isSkin1 == isSkin2) {
        return random(1, 2);
    }
    
    return isSkin1 ? 2 : 1;
}

string FishQuizPopup::generateFishPosition(string fishID) {
    if (TodoUtil::startsWith(fishID, "d") == false) {
        return "";
    }
    
    auto positions = Fish::getFishPosition(fishID);
    vector<string>::iterator it;
    bool bErase = false;
    for(it = positions.begin(); it != positions.end(); ) {
        bErase = false;
        for (auto fishData : _fishData) {
            if ((*it) == fishData->_position) {
                it = positions.erase(it);
                bErase = true;
                break;
            }
        }
        
        if(bErase == false) {
            ++it;
        }
    }
    
    if (positions.size() == 0) {
        positions = Fish::getFishPosition(fishID);
        
        vector<FishData*> fishes;
        for (auto position : positions) {
            for (auto fishData : _fishData) {
                if (position == fishData->_position) {
                    fishes.push_back(fishData);
                    break;
                }
            }
        }
        
        long oldTime = LONG_MAX;
        FishData * oldData = nullptr;
    
        for (auto fishData : fishes) {
            if (oldTime > fishData->_time) {
                oldTime = fishData->_time;
                oldData = fishData;
            }
        }
        
        return oldData->_position;
    }
    
    return positions.at(random(0, (int)(positions.size()) - 1));
}

SwapNode* SwapNode::create(float duration, int blinks, Node* node0, Node* node1)
{
    SwapNode *swapNode = new (std::nothrow) SwapNode();
    if (swapNode && swapNode->initWithDuration(duration, blinks, node0, node1)) {
        swapNode->autorelease();
        return swapNode;
    }
    
    delete swapNode;
    return nullptr;
}

bool SwapNode::initWithDuration(float duration, int blinks, Node* node0, Node* node1) {
    if (ActionInterval::initWithDuration(duration)) {
        _times = blinks;
        _node0 = node0;
        _node1 = node1;
        return true;
    }
    
    return false;
}

void SwapNode::update(float time) {
    if (_target && ! isDone()) {
        float slice = 1.0f / _times;
        float m = fmodf(time, slice);
        bool temp = _node0->isVisible();
        _node0->setVisible(m > slice / 2 ? true : false);
        _node1->setVisible(!_node0->isVisible());
        
        if (temp != _node0->isVisible()) {
            if (_effectID>=0) GameSoundManager::getInstance()->stopEffectSound(_effectID);
            _effectID = GameSoundManager::getInstance()->playEffectSound(soundsPath+"c3.m4a");
        }

    }
}

void SwapNode::stop() {
    if (nullptr != _target)
    ActionInterval::stop();
}

void SwapNode::startWithTarget(Node *target) {
    ActionInterval::startWithTarget(target);
}

