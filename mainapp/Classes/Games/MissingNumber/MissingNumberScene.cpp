
//
//  MissingNumberScene.cpp
//  KitkitSchool
//
//  Created by YongSoo Hong on 03/28/18
//

#include <Common/Controls/TodoSchoolBackButton.hpp>
#include <Common/Controls/CompletePopup.hpp>
#include "MissingNumberScene.hpp"
#include <Utils/TodoUtil.h>
#include "Managers/LanguageManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"
#include "CCAppController.hpp"

using namespace cocos2d::ui;
using namespace std;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const std::string ROOT_RESOURCE_PATH = "Games/MissingNumber";
#else
const std::string ROOT_RESOURCE_PATH = "MissingNumber";
#endif

namespace MissingNumber
{
    Scene* MissingNumberScene::createScene(int levelID)
    {
        // 'scene' is an autorelease object
        auto scene = Scene::create();
        
        // 'layer' is an autorelease object
        auto layer = MissingNumberScene::create();
        layer->setLevel(levelID);
        // add layer as a child to scene
        scene->addChild(layer);
        
        // return the scene
        return scene;
    }
    
    bool MissingNumberScene::init()
    {
        if (!Layer::init())
        {
            return false;
        }
        
        auto winSize = getContentSize();
        
        // Background
        {
            auto bg = Sprite::create(ROOT_RESOURCE_PATH + "/back_image.png");
            bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            bg->setContentSize(winSize);
            bg->setPosition(winSize/2);
            addChild(bg);
            
            auto bgEffect0 = Sprite::create(ROOT_RESOURCE_PATH + "/lamp_light_02.png");
            bgEffect0->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            
            auto bgEffect1 = Sprite::create(ROOT_RESOURCE_PATH + "/lamp_light_01.png");
            bgEffect1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            
            auto bgEffect2 = Sprite::create(ROOT_RESOURCE_PATH + "/egg-lamp.png");
            bgEffect2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            
            mBgLight = Node::create();
            mBgLight->setContentSize(bgEffect1->getContentSize());
            
            Size size = mBgLight->getContentSize();
            bgEffect0->setPosition(Vec2(size.width/2, size.height));
            bgEffect1->setPosition(Vec2(size.width/2, size.height));
            bgEffect2->setPosition(Vec2(size.width/2, size.height));
            
            mBgLight->addChild(bgEffect0);
            mBgLight->addChild(bgEffect1);
            mBgLight->addChild(bgEffect2);
            mBgLight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            mBgLight->setPosition(Vec2(winSize.width/2, winSize.height));
            mBgLight->setVisible(false);
            addChild(mBgLight);
            
            mPaticleSnow = ParticleSnow::create();
            mPaticleSnow->initWithTotalParticles(10);
            mPaticleSnow->setLife(20);
            mPaticleSnow->setScale(4.f);
            mPaticleSnow->setEmissionRate(1.0f);
            mPaticleSnow->setTexture(Director::getInstance()->getTextureCache()->addImage(ROOT_RESOURCE_PATH + "/lamp_light_03.png"));
            mPaticleSnow->setPosVar(Vec2(bgEffect2->getContentSize().width / 2, 0));
            mPaticleSnow->setSpeed(8.0f);
            mPaticleSnow->stopSystem();
            addChild(mPaticleSnow);
        }
        
        // Back Button
        auto backButton = TodoSchoolBackButton::create();
        backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        backButton->setPosition(Vec2(25, winSize.height-25));
        addChild(backButton);
        
        // Progress Bar
        mProgressBar = ProgressIndicator::create();
        mProgressBar->setPosition(Vec2(winSize.width/2, winSize.height - mProgressBar->getContentSize().height));
        addChild(mProgressBar);        
        return true;
    }
    
    void MissingNumberScene::onEnter()
    {
        Layer::onEnter();
        
        readLevelData();
        setProblem(0);
        startJumpAnimationProblem(1.0f);
    }
    
    void MissingNumberScene::onExit()
    {
        Layer::onExit();
    }
    
    void MissingNumberScene::setLevel(int levelID)
    {
        mLevelID = levelID;
    }
    
    void MissingNumberScene::readLevelData()
    {
        string p = LanguageManager::getInstance()->findLocalizedResource(ROOT_RESOURCE_PATH + "/MissingNumber_Levels.tsv");
        
        string s = FileUtils::getInstance()->getStringFromFile(p);
        auto data = TodoUtil::readTSV(s);
        
        // data[0][2] : #Worksheet or #WorksheetAll
        bool bWorkSheetAll = data[0][2] == "#WorksheetAll" ? true : false;
        int maxWorkSheet = 0;
        for (vector<string> row : data)
        {
            if (row.size() < 1)
                continue;
            
            if (TodoUtil::trim(row[0]).size() <= 0)
                continue;
            
            if (row[0][0] == '#')
                continue;
            
            if (TodoUtil::stoi(row[1]) == mLevelID)
            {
                Data data;
                data.mLevel = TodoUtil::stoi(row[1]);
                data.mWorksheet = TodoUtil::stoi(row[2]);
                data.mProblem = TodoUtil::stoi(row[3]);
                data.mQuestion = row[4];
                data.mSuggest = row[5];
                data.mAnswer = row[6];
                mData.push_back(data);
                
                if (data.mWorksheet > maxWorkSheet)
                {
                    maxWorkSheet = data.mWorksheet;
                }
                
            }
        }
        
        if (bWorkSheetAll == false)
        {
            int curWorksheet = RandomHelper::random_int(1, maxWorkSheet);
            
            for (int i = mData.size() - 1; i >= 0; --i)
            {
                if (mData.at(i).mWorksheet != curWorksheet)
                {
                    mData.erase(mData.begin() + i);
                }
            }
        }
    }
    
    void MissingNumberScene::setProblem(int problem)
    {
        mCurrentProblemID = problem;
        mRetryCount = 0;
        mProgressBar->setMax(mData.size());
        mProgressBar->setCurrent(mCurrentProblemID + 1, false);
        
        setEggs();
        
        reorderChild(mEggRoot, 0);
        reorderChild(mBgLight, 0);

        setNumberCard();
        
        reorderChild(mPaticleSnow, 0);
        reorderChild(mProgressBar, 0);
    }
    
    void MissingNumberScene::startJumpAnimationProblem(float startOffset) {
        if (TodoUtil::stoi(TodoUtil::trim(mData.at(mCurrentProblemID).mAnswer)) > 10)
        {
            return;
        }
        
        for (int i = 0; i < mEggs.size(); ++i)
        {
            if (mEggs.at(i)->isQuestion())
            {
                continue;
            }
            
            if (TodoUtil::stoi(mEggs.at(i)->getNumberString()) > 10)
            {
                return;
            }
        }
        
        const float duration = 0.7f;
        for (int i = 0; i < mEggs.size(); ++i)
        {
            mEggs.at(i)->startJumpAnimation(2, i, startOffset + i * duration, nullptr);
        }
    }
    
    void MissingNumberScene::setEggs()
    {
        if (mEggRoot != nullptr)
        {
            mEggRoot->removeAllChildren();
        }
        
        mEggs.clear();
        if (mEggRoot == nullptr)
        {
            mEggRoot = Node::create();
        }
        
        float totalWidth = 0.f;
        float totalHeight = 0.f;
        
        auto split = TodoUtil::split(mData.at(mCurrentProblemID).mQuestion, ',');
        int count = split.size();
        
        vector<int> resourceNumber;
        for (int i = 0; i < 5; ++i)
        {
            resourceNumber.push_back(i);
        }
        
        random_shuffle(resourceNumber.begin(), resourceNumber.end());
        
        for (int i = 0; i < count; ++i)
        {
            auto egg = Egg::create();
            egg->initWithNumberString(resourceNumber.at(i), split.at(i), mData.at(mCurrentProblemID).mAnswer, (i == count - 1));
            
            totalHeight = egg->getContentSize().height;
            
            egg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            egg->setPosition(totalWidth + egg->getContentSize().width / 2, totalHeight / 2);
            mEggRoot->addChild(egg);
            
            totalWidth += egg->getContentSize().width;
            mEggs.push_back(egg);
        }

        mEggRoot->setContentSize(Size(totalWidth, totalHeight));
        mEggRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        
        if (mEggRoot->getParent() == nullptr)
        {
            mEggRoot->setPosition(getContentSize().width / 2, 820.f);
            addChild(mEggRoot);
            setLightPosition();
        }
    }
    
    void MissingNumberScene::setNumberCard()
    {
        if (mNumberCardRoot != nullptr)
        {
            mNumberCardRoot->removeAllChildren();
        }
        
        CCLOG("mSuggest : %s", mData.at(mCurrentProblemID).mSuggest.c_str());
        bool bSmall = mData.at(mCurrentProblemID).mSuggest == "NA";
        if (mData.at(mCurrentProblemID).mSuggest.length() == 0) {
            bSmall = true;
        }
        
        mNumberCards.clear();
        if (mNumberCardRoot == nullptr)
        {
            mNumberCardRoot = Node::create();
        }
        
        mDummyNumberCard = NumberCard::create();
        mDummyNumberCard->setOpacity(255 * 0.7f);
        mDummyNumberCard->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        mDummyNumberCard->initWithNumberString("", bSmall);
        mDummyNumberCard->setVisible(false);
        mNumberCardRoot->addChild(mDummyNumberCard);
        
        float totalWidth = 0.f;
        float totalHeight = 0.f;
        
        if (bSmall == true)
        {
            for (int i = 0; i < 10; i++)
            {
                auto card = NumberCard::create();
                card->initWithNumberString(TodoUtil::itos(i), bSmall, true);
                
                totalHeight = card->getContentSize().height;
                
                card->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                card->setPosition(totalWidth + card->getContentSize().width / 2, totalHeight / 2);
                mNumberCardRoot->addChild(card);
                
                totalWidth += card->getContentSize().width;
                mNumberCards.push_back(card);
                
                bindingCardEvent(card);
            }
        }
        else
        {
            auto split = TodoUtil::split(mData.at(mCurrentProblemID).mSuggest, ',');
//            random_shuffle(split.begin(), split.end());
            
            for (int i = 0; i < split.size(); ++i)
            {
                auto card = NumberCard::create();
                card->initWithNumberString(split.at(i), bSmall, true);
                
                totalHeight = card->getContentSize().height;
                
                card->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                card->setPosition(totalWidth + card->getContentSize().width / 2, totalHeight / 2);
                mNumberCardRoot->addChild(card);
                
                totalWidth += card->getContentSize().width;
                if (i != split.size() - 1)
                {
                    totalWidth += 100.0f;
                }
                
                mNumberCards.push_back(card);
                
                bindingCardEvent(card);
            }
        }
        
        mNumberCardRoot->setContentSize(Size(totalWidth, totalHeight));
        mNumberCardRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        if (mNumberCardRoot->getParent() == nullptr)
        {
            mNumberCardRoot->setPosition(getContentSize().width / 2, 50.f);
            addChild(mNumberCardRoot);
        }
        
        reorderChild(mNumberCardRoot, 0);
        
        NumberCard::isActive = true;
    }
    
    void MissingNumberScene::bindingCardEvent(NumberCard *card)
    {
        card->onTouchBegan = [this, card]() {
            GameSoundManager::getInstance()->playEffectSound("Main/Common/Sounds/Effect/Matrix_ClickBlock.m4a");
            
            mDummyNumberCard->setNumberString(card->getNumberString());
            mDummyNumberCard->setPosition(card->getPosition());
            mDummyNumberCard->setVisible(true);
            
            card->retain();
            card->removeFromParent();
            card->setScale(0.85f);
            mNumberCardRoot->addChild(card);
            card->release();
        };
        
        card->onTouchMoved = [this, card]() {
            for (const auto egg : mEggs)
            {
                if (egg->isQuestion() == true)
                {
                    int questionIndex = egg->checkCollisionWithQuestion(card);
                    egg->selectedQuestion(0, questionIndex == 0 ? true : false);
                    egg->selectedQuestion(1, questionIndex == 1 ? true : false);
                    break;
                }
            }
        };
        
        card->onTouchEnded = [this, card]() {
            {
                Egg* targetEgg = nullptr;
                Egg* candidateEgg = nullptr;
                for (const auto egg : mEggs)
                {
                    if (egg->isQuestion() == true)
                    {
                        int questionIndex = egg->checkCollisionWithQuestion(card);
                        
                        if (questionIndex >= 0) {
                            candidateEgg = egg;
                        }
                        if (card->getNumberString() == egg->getNumberString(questionIndex))
                        {
                            targetEgg = egg;
                            egg->showNumber(questionIndex);
                        }
                        else
                        {
                            if (questionIndex != -1 && egg->hasQuestion2() == true)
                            {
                                if (egg->hasCorrectQuestion() == false)
                                {
                                    ++mRetryCount;
                                }
                            }
                        }
                        
                        egg->selectedQuestion(0, false);
                        egg->selectedQuestion(1, false);
                        break;
                    }
                }
                
                if (targetEgg)
                {
                    GameSoundManager::getInstance()->playEffectSound("Main/Common/Sounds/Effect/Matrix_RightAnswer.m4a");
                    writePlayLog(card, nullptr);
                    
                    card->setScale(0.5f);
                    card->runAction(Sequence::create(MoveTo::create(0.01f, mDummyNumberCard->getPosition()),
                                                     EaseBackOut::create(ScaleTo::create(0.2f, 1.f)),
                                                     CallFunc::create([this](){
                        NumberCard::isActive = true;
                        mDummyNumberCard->setVisible(false);
                        
                        if (this->isSolved())
                        {
                            this->onSolved();
                            return;
                        }
                        
                    }), nullptr));
                }
                else
                {
                    if (card->getPosition().distance(mDummyNumberCard->getPosition()) > 100.f)
                    {
                        GameSoundManager::getInstance()->playEffectSound("Main/Common/Sounds/Effect/Matrix_WrongMove.m4a");
                        writePlayLog(card, candidateEgg);
                    }
                    
                    card->runAction(Sequence::create(EaseExponentialOut::create(MoveTo::create(0.18f, mDummyNumberCard->getPosition())), CallFunc::create([this, card](){
                        card->setScale(1.0f);
                        NumberCard::isActive = true;
                        mDummyNumberCard->setVisible(false);
                    }), nullptr));
                    
                    if (mRetryCount == 5)
                    {
                        for (const auto egg : mEggs)
                        {
                            if (egg->isQuestion() == true)
                            {
                                egg->showNumber(1);
                                mRetryCount = 0;
                                break;
                            }
                        }
                    }
                }
            }
        };
    }
    
    bool MissingNumberScene::isSolved()
    {
        for (const auto egg : mEggs)
        {
            if (egg->isSolved() == true)
            {
                return true;
            }
        }
        return false;
    }
    
    void MissingNumberScene::onSolved()
    {
        setPreventTouches(false);
        for (const auto egg : mEggs)
        {
            egg->startParticle();
        }
        for (const auto egg : mEggs)
        {
            egg->showNumberString();
        }

        mProgressBar->setCurrent(mCurrentProblemID + 1, true);
        runAction(Sequence::create(CallFunc::create([this](){
            GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/Mango_Close.m4a");
            
        }), EaseBackOut::create(ScaleTo::create(0.15f, 1.f)), CallFunc::create([this](){
            for (const auto egg : mEggs)
            {
                egg->stopParticle();
            }
            
            auto callback = [=]() {
                if (mCurrentProblemID == mData.size() - 1)
                {
                    this->setPreventTouches(true);
                    CompletePopup::create()->show(1.0f, [=](){
                        CCAppController::sharedAppController()->handleGameComplete(1);
                    });
                }
                else
                {
                    runAction(Sequence::create(DelayTime::create(1.0f),
                                               CallFunc::create([this](){
                        const float ANIMATION_DURATION = 0.3f;
                        
                        {
                            auto action_0 = MoveBy::create(ANIMATION_DURATION, Point(getContentSize().width, 0));
                            mBgLight->setVisible(false);
                            mPaticleSnow->setVisible(false);
                            mPaticleSnow->stopSystem();
                            mEggRoot->runAction(Sequence::create(action_0,
                                                                 CallFunc::create([this](){
                                Vec2 position = mEggRoot->getPosition();
                                mEggRoot->setPosition(-getContentSize().width / 2, position.y);
                            }),
                                                                 action_0,
                                                                 nullptr));
                        }
                        
                        {
                            auto action_0 = MoveBy::create(ANIMATION_DURATION, Point(0, -900));
                            auto action_1 = MoveBy::create(ANIMATION_DURATION, Point(0, 900));
                            mNumberCardRoot->runAction(Sequence::create(action_0,
                                                                        CallFunc::create([this](){
                                setProblem(++mCurrentProblemID);
                            }),
                                                                        action_1,
                                                                        CallFunc::create([this](){
                                setPreventTouches(true);
                                setLightPosition();
                                startJumpAnimationProblem(0.5f);
                            }),
                                                                        nullptr));
                        }
                        
                    }), nullptr));
                }
            };
            
            const float ANIMATION_START_OFFSET = 1.0f;
            int audioType = RandomHelper::random_int(0, 1);
            for (int i = 0; i < mEggs.size(); ++i)
            {
                if (i == mEggs.size() - 1)
                {
                    mEggs.at(i)->startJumpAnimation(audioType, i, ANIMATION_START_OFFSET + i * 0.2f, [=](){
                        mEggs.at(i)->startExplosionAnimation(callback);
                    });
                }
                else
                {
                    mEggs.at(i)->startJumpAnimation(audioType, i, ANIMATION_START_OFFSET + i * 0.2f, nullptr);
                }
            }

        } ), nullptr));
    }
    
    vector<int> MissingNumberScene::levelIDsFor()
    {
        vector<int> result;
        string p = LanguageManager::getInstance()->findLocalizedResource(ROOT_RESOURCE_PATH + "/MissingNumber_Levels.tsv");
        
        string s = FileUtils::getInstance()->getStringFromFile(p);
        auto data = TodoUtil::readTSV(s);
        for (vector<string> row : data)
        {
            if (row.size() < 1)
                continue;
            
            if (TodoUtil::trim(row[0]).size() <= 0)
                continue;
            
            if (row[0][0] == '#')
                continue;
            
            if(std::find(result.begin(), result.end(), TodoUtil::stoi(row[1])) == result.end()) {
                result.push_back(TodoUtil::stoi(row[1]));
            }
        }
        
        return result;
    }
    
    void MissingNumberScene::setPreventTouches(bool bEnable)
    {
        if (mNumberCardRoot != nullptr)
        {
            mNumberCardRoot->getEventDispatcher()->setEnabled(bEnable);
        }
    }
    
    void MissingNumberScene::setLightPosition()
    {
        for (Egg* egg : mEggs)
        {
            if (egg->isQuestion())
            {
                Vec2 pos = egg->convertToWorldSpace(egg->getContentSize()/2);
                mBgLight->setPosition(pos.x, getContentSize().height);
                mBgLight->setVisible(true);
                mPaticleSnow->setVisible(true);
                mPaticleSnow->setPosition(pos.x, getContentSize().height);
                break;
            }
        }
        
        mPaticleSnow->resetSystem();
    }
    
    string MissingNumberScene::makeWorkPath() const {
        stringstream ss;
        ss << "MissingNumber";
        ss << "/" << "level-" << mLevelID;
        ss << "/" << "work-" << mCurrentProblemID;
        return ss.str();
    }

    void MissingNumberScene::writePlayLog(NumberCard *hitCard, Egg *hitEgg) {
        stringstream userAnswer;
        stringstream correctAnswer;
        
        userAnswer << "[";
        correctAnswer << "[";

        bool first = true;
        for (Egg* egg : mEggs) {
            if (!first) {
                userAnswer << ", ";
                correctAnswer << ", ";
            }
            first = false;
            
            if (!egg) {
                userAnswer << "None";
                correctAnswer << "None";
                continue;
            }
            
            // NB(xenosoz, 2018): We need to respect and revive wrong input for logging here.
            if (egg == hitEgg && egg->isQuestion()) {
                int index = egg->checkCollisionWithQuestion(hitCard);
                
                userAnswer << (index == 0 ? hitCard->getNumberString() : egg->getNumberShowing(0));
                userAnswer << (index == 1 ? hitCard->getNumberString() : egg->getNumberShowing(1));
            }
            else if (egg->isQuestion()) {
                userAnswer << egg->getNumberShowing(0);
                userAnswer << egg->getNumberShowing(1);
            }
            else {
                userAnswer << egg->getNumberString();
            }
            
            correctAnswer << (egg->isQuestion() ? egg->getNumberString(0)+egg->getNumberString(1) : egg->getNumberString());
        }
        
        userAnswer << "]";
        correctAnswer << "]";
        
        StrictLogManager::shared()->game_Peek_Answer("MissingNumber", makeWorkPath(), userAnswer.str(), correctAnswer.str());
    }
}
