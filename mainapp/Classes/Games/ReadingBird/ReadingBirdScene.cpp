//
//  ReadingBirdScene.cpp
//
//  Created by YongSoo Hong on 02/09/18
//

#include <Common/Controls/TodoSchoolBackButton.hpp>
#include <Common/Controls/CompletePopup.hpp>
#include "Managers/GameSoundManager.h"
#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/StrictLogManager.h"
#include "ReadingBirdScene.hpp"
#include "CCAppController.hpp"
#include <Utils/TodoUtil.h>

using namespace cocos2d::ui;
using namespace std;
using namespace ReadingBird;

const std::string JNI_CLASS_NAME = "org/cocos2dx/cpp/AppActivity";
const std::string ASSET_PREFIX = "ReadingBird";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const std::string ROOT_RESOURCE_PATH = "Games/" + ASSET_PREFIX;
#else
const std::string ROOT_RESOURCE_PATH = ASSET_PREFIX;
#endif

const int ANIMATION_FPS = 30;
const int ANIMATION_FPS2 = 24;
int TRIGGER_VOLUME = DEFAULT_TRIGGER_VOLUME;
const int SILENT_VOLUME = 40;

const int CURRENT_SCENE_ID = 100;
const int CRITERIA_SPEECH_SCORE = 30;

const float RECORD_TIMEOUT_SEC_SHORT = 3;
const float RECORD_TIMEOUT_SEC_LONG = 8;
float RECORD_TIMEOUT_SEC;

const float RECORD_VOLUME_TIMEOUT_SEC_SHORT = 0.3f;
const float RECORD_VOLUME_TIMEOUT_SEC_LONG = 0.8f;
float RECORD_VOLUME_TIMEOUT_SEC = 0.3f;

const int CHARACTER1_OFFSET_POS_Y = (-80);
string SDCARD_ROOT_ANDROID;

bool isDebug = false;
const int LABEL_MAIN_STATE = 1001;
const int LABEL_CHARACTER1_STATE = 1002;
const int LABEL_CHARACTER2_STATE = 1003;
const int LABEL_VOLUME = 1004;
const int LABEL_SCORE = 1005;

string strMainState[] = {
    "none",
    "entrance",
    "wait",
    "record",
    "stop_record",
    "listen_record",
    "listen_original",
    "complete"
};

string strCharacter1State[] = {
    "none",
    "appear",
    "idle",
    "suggest",
    "induceIdle",
    "induceHappy",
    "look",
    "stopLook",
    "listen",
    "stopListen",
    "speakHappy",
    "speakUnhappy",
    "dancingA"
};

string strCharacter2State[] = {
    "none",
    "urgeCharacter",
    "urgeSentence",
    "dancingB"
};

Scene* ReadingBirdScene::createScene(int levelID, bool isSdcardData)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ReadingBirdScene::create();
    layer->setLevel(levelID);
    layer->setSdcardData(isSdcardData);
    layer->setTag(CURRENT_SCENE_ID);
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool ReadingBirdScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    TRIGGER_VOLUME = UserDefault::getInstance()->getIntegerForKey("TRIGGER_VOL", DEFAULT_TRIGGER_VOLUME);
    CCLOG("TRIGGER_VOLUME : %d", TRIGGER_VOLUME);
    isDebug = UserManager::getInstance()->isDebugMode();

    auto winSize = getContentSize();
    auto bg = Sprite::create(ROOT_RESOURCE_PATH + "/bg.png");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setContentSize(winSize);
    bg->setPosition(winSize/2);
    addChild(bg);
    
    // Back Button
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    mMainState = MAIN_STATE::none;
    mCharacter1State = CHARACTER1_STATE::none;
    mCharacter2State = CHARACTER2_STATE::none;
    
    mCharacter1 = Sprite::create();
    addChild(mCharacter1);
    
    mCharacter2 = Sprite::create();
    addChild(mCharacter2);
    
    mTextQuestion = Text::create("", "fonts/Andika-R.ttf", 300);
    mTextQuestion->setColor(Color3B(65, 64, 62));
    mTextQuestion->setOpacity(0xf2);
    mTextQuestion->setPosition(Vec2(winSize.width / 2 - 13, winSize.height / 2 - 251));
    addChild(mTextQuestion);
    
    if (isDebug == true)
    {
        auto mainState = Label::createWithTTF("main : " + strMainState[(int)mMainState], "fonts/TodoSchoolV2.ttf", 70);
        mainState->setTextColor(Color4B::WHITE);
        mainState->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        mainState->setPosition(Vec2(25, winSize.height - 300));
        mainState->setTag(LABEL_MAIN_STATE);
        addChild(mainState);

        auto character1State = Label::createWithTTF("c1 : " + strCharacter1State[(int)mCharacter1State], "fonts/TodoSchoolV2.ttf", 70);
        character1State->setTextColor(Color4B::WHITE);
        character1State->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        character1State->setPosition(Vec2(25, winSize.height - 400));
        character1State->setTag(LABEL_CHARACTER1_STATE);
        addChild(character1State);

        auto character2State = Label::createWithTTF("c2 : " + strCharacter2State[(int)mCharacter2State], "fonts/TodoSchoolV2.ttf", 70);
        character2State->setTextColor(Color4B::WHITE);
        character2State->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        character2State->setPosition(Vec2(25, winSize.height - 500));
        character2State->setTag(LABEL_CHARACTER2_STATE);
        addChild(character2State);

        auto volume = Label::createWithTTF("", "fonts/TodoSchoolV2.ttf", 70);
        volume->setTextColor(Color4B::WHITE);
        volume->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        volume->setPosition(Vec2(25, winSize.height - 600));
        volume->setTag(LABEL_VOLUME);

        addChild(volume);

        auto score = Label::createWithTTF("", "fonts/TodoSchoolV2.ttf", 70);
        score->setTextColor(Color4B::WHITE);
        score->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        score->setPosition(Vec2(25, winSize.height - 700));
        score->setTag(LABEL_SCORE);

        addChild(score);
    }
    
    // Progress Bar
    mProgressBar = ProgressIndicator::create();
    mProgressBar->setPosition(Vec2(winSize.width/2, winSize.height - mProgressBar->getContentSize().height));
    addChild(mProgressBar);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_appear.plist",ROOT_RESOURCE_PATH + "/bird_appear.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_idle.plist",ROOT_RESOURCE_PATH + "/bird_idle.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_suggest_a.plist",ROOT_RESOURCE_PATH + "/bird_suggest_a.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_induceIdle_A.plist",ROOT_RESOURCE_PATH + "/bird_induceIdle_A.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_induceHappy_A.plist",ROOT_RESOURCE_PATH + "/bird_induceHappy_A.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_look_A.plist",ROOT_RESOURCE_PATH + "/bird_look_A.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_stopLook_A.plist",ROOT_RESOURCE_PATH + "/bird_stopLook_A.png");
    //    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_listen_A.plist",ROOT_RESOURCE_PATH + "/bird_listen_A.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_listen.plist",ROOT_RESOURCE_PATH + "/bird_listen.png");
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_stopListen_A.plist",ROOT_RESOURCE_PATH + "/bird_stopListen_A.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_speakHappy_A.plist",ROOT_RESOURCE_PATH + "/bird_speakHappy_A.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_speakUnhappy_A.plist",ROOT_RESOURCE_PATH + "/bird_speakUnhappy_A.png");
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_suggest_b_1.plist",ROOT_RESOURCE_PATH + "/bird_suggest_b_1.png");
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_suggest_b_2.plist",ROOT_RESOURCE_PATH + "/bird_suggest_b_2.png");
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/urgeCharacter.plist",ROOT_RESOURCE_PATH + "/urgeCharacter.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/dancingA.plist",ROOT_RESOURCE_PATH + "/dancingA.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/dancing.plist",ROOT_RESOURCE_PATH + "/dancing.png");
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_induceIdle_B.plist",ROOT_RESOURCE_PATH + "/bird_induceIdle_B.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_induceHappy_B.plist",ROOT_RESOURCE_PATH + "/bird_induceHappy_B.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_look_B.plist",ROOT_RESOURCE_PATH + "/bird_look_B.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_stopLook_B.plist",ROOT_RESOURCE_PATH + "/bird_stopLook_B.png");
    //    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_listen_B.plist",ROOT_RESOURCE_PATH + "/bird_listen_B.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_stopListen_B.plist",ROOT_RESOURCE_PATH + "/bird_stopListen_B.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_speakHappy_B_1.plist",ROOT_RESOURCE_PATH + "/bird_speakHappy_B_1.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_speakHappy_B_2.plist",ROOT_RESOURCE_PATH + "/bird_speakHappy_B_2.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ROOT_RESOURCE_PATH + "/bird_speakUnhappy_B.plist",ROOT_RESOURCE_PATH + "/bird_speakUnhappy_B.png");
    
    Director::getInstance()->getTextureCache()->addImage("Common/Controls/CompletePopup/game_effect_glow.png");
    Director::getInstance()->getTextureCache()->addImage("Common/Controls/CompletePopup/game_effect_rotatingleft.png");
    Director::getInstance()->getTextureCache()->addImage("Common/Controls/CompletePopup/game_effect_rotatingright.png");
    
    GameSoundManager::getInstance()->preloadEffect("Main/Common/Sounds/Effect/SFX_Button.m4a");
    return true;
}

void ReadingBirdScene::onEnter()
{
    Layer::onEnter();
    CCLOG("onEnter : %d", (int)mbSdcardData);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "onSetupSpeechRecognition");
#endif
    readLevelData();
    readSpeechData();

    mCurrentProblemID = 0;
    mProgressBar->setMax((int)mData.size());
    mProgressBar->setCurrent(mCurrentProblemID, false);
    
    changeMainState(MAIN_STATE::entrance);
}

void ReadingBirdScene::onExit()
{
    CCLOG("onExit");
    
    unschedule(schedule_selector(ReadingBirdScene::onStopRecord));
    unschedule(schedule_selector(ReadingBirdScene::onStopRecordVolume));
    
    changeCharacter1State(CHARACTER1_STATE::none);
    changeCharacter2State(CHARACTER2_STATE::none);
    
    Layer::onExit();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "onCleanUpSpeechRecognition");
#endif
}

void ReadingBirdScene::setLevel(int levelID)
{
    mLevelID = levelID;
    CCLOG("mLevelID : %d", mLevelID);
}

void ReadingBirdScene::readLevelData()
{
    string p;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (mbSdcardData == true)
    {
        p = SDCARD_ROOT_ANDROID + "/Games/" + ASSET_PREFIX + "/ReadingBird_Levels.tsv";
    }
    else
    {
        p = LanguageManager::getInstance()->findLocalizedResource("Games/" + ASSET_PREFIX + "/ReadingBird_Levels.tsv");
    }
#else
    p = LanguageManager::getInstance()->findLocalizedResource("Games/" + ASSET_PREFIX + "/ReadingBird_Levels.tsv");
#endif

    string s = FileUtils::getInstance()->getStringFromFile(p);
    auto data = TodoUtil::readTSV(s);

    // data[6][2] : Worksheet or WorksheetAll
    bool bWorkSheetAll = data[6][2] == "WorksheetAll" ? true : false;
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
            data.mSuggest = row[4];
            data.mSound = row[5];
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

void ReadingBirdScene::readSpeechData()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    string p;
    if (mbSdcardData == true)
    {
        p = LanguageManager::getInstance()->findLocalizedResource(SDCARD_ROOT_ANDROID + "/" + ROOT_RESOURCE_PATH + "/speech.txt");
    }
    else
    {
        p = LanguageManager::getInstance()->findLocalizedResource(ROOT_RESOURCE_PATH + "/speech.txt");
    }
    
#else
    string p = LanguageManager::getInstance()->findLocalizedResource(ROOT_RESOURCE_PATH + "/speech.txt");
    
#endif
    
    string s = FileUtils::getInstance()->getStringFromFile(p);
    
    auto splitNewLine = TodoUtil::split(s, '\n');
    for (int i = 0; i < splitNewLine.size(); ++i) {
        auto splitComma = TodoUtil::split(splitNewLine.at(i), ',');
        mSpeech.insert(pair<string, string>(splitComma.at(0), splitComma.at(1)));
    }
}

Vector<SpriteFrame*> ReadingBirdScene::getAnimationInFile(const char *format, int startNumber, int endNumber)
{
    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> animFrames;
    char str[100];
    for(int i = startNumber; i <= endNumber; i++)
    {
        sprintf(str, format, i);
        auto sprite = spritecache->getSpriteFrameByName(str);
        if (sprite != nullptr)
        {
            animFrames.pushBack(sprite);
        }
    }
    return animFrames;
}

void ReadingBirdScene::setCharacter1Sprite(SpriteFrame* frame)
{
    auto winSize = getContentSize();
    mCharacter1->setSpriteFrame(frame);
    mCharacter1->setPosition(winSize.width / 2, winSize.height / 2 + CHARACTER1_OFFSET_POS_Y);
    mCharacter1->setScaleX(4.0);
    mCharacter1->setScaleY(4.0);
}

void ReadingBirdScene::setCharacter2Sprite(SpriteFrame* frame)
{
    auto winSize = getContentSize();
    mCharacter2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    mCharacter2->setSpriteFrame(frame);
    mCharacter2->setPosition(winSize.width - 1, 0);
    mCharacter2->setScaleX(4.0);
    mCharacter2->setScaleY(4.0);
}

void ReadingBirdScene::setTextQuestion(string question)
{
    auto winSize = getContentSize();
    
    TodoUtil::replaceAll(question, "_", " ");
    mTextQuestion->setString(question);
    
    const int maxFontSize = mbShortCurrentText == true ? 300 : 130;
    const int maxWidth = mbShortCurrentText == true ? 562 : 1410;
    
    for (int i = maxFontSize; i >= 1; --i)
    {
        mTextQuestion->setFontSize(i);
        if (mTextQuestion->getContentSize().width <= maxWidth)
        {
            CCLOG("size : %d, width : %f", i, mTextQuestion->getContentSize().width);
            break;
        }
    }
    
    if (mbShortCurrentText == true)
    {
        mTextQuestion->setPosition(Vec2(winSize.width / 2 - 13, winSize.height / 2 - 251 + CHARACTER1_OFFSET_POS_Y));
    }
    else
    {
        mTextQuestion->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 476 + CHARACTER1_OFFSET_POS_Y));
    }
}

void ReadingBirdScene::changeMainState(MAIN_STATE state)
{
    mMainState = state;
    
    CCLOG("mMainState = %s", strMainState[(int)mMainState].c_str());
    
    if (state == MAIN_STATE::entrance)
    {
        mIsPronunciation = false;
        mIsBeforePronunciation = false;
        
        changeCharacter1State(CHARACTER1_STATE::appear, [&](){
            this->changeCharacter1State(CHARACTER1_STATE::idle, [&]() {
                this->changeMainState(MAIN_STATE::wait);
            });
        });
    }
    else if (state == MAIN_STATE::wait)
    {
        if (isDebug)
        {
            Label* label = (Label *)getChildByTag(LABEL_VOLUME);
            label->setString("");
        }

        mProgressBar->setCurrent(++mCurrentProblemID, false);
        setScore(-1);
        mIsPronunciation = false;
        mTriggerVolume = 0;
        
        mCurrentText = mData[mCurrentProblemID - 1].mSuggest;
        mbShortCurrentText = mCurrentText.length() <= 6;
        CCLOG("result - %d,text - %s, %d", mbShortCurrentText, mCurrentText.c_str(), (mCurrentText.find(" ") != std::string::npos));
        if (mbShortCurrentText == false && (mCurrentText.find(" ") != std::string::npos))
        {
            RECORD_TIMEOUT_SEC = RECORD_TIMEOUT_SEC_LONG;
            RECORD_VOLUME_TIMEOUT_SEC = RECORD_VOLUME_TIMEOUT_SEC_LONG;
        }
        else
        {
            RECORD_TIMEOUT_SEC = RECORD_TIMEOUT_SEC_SHORT;
            RECORD_VOLUME_TIMEOUT_SEC = RECORD_VOLUME_TIMEOUT_SEC_SHORT;
        }
        
        setTextQuestion("");
        CCLOG("RECORD_TIMEOUT_SEC : %f", RECORD_TIMEOUT_SEC);
        CCLOG("RECORD_VOLUME_TIMEOUT_SEC : %f", RECORD_VOLUME_TIMEOUT_SEC);
        
        GameSoundManager::getInstance()->playEffectSound(ROOT_RESOURCE_PATH + "/effect/drop_card.m4a");
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            string key = mData[mCurrentProblemID - 1].mSound;
            TodoUtil::replace(key, ".m4a", "");
            TodoUtil::replace(key, ".wav", "");
            string phone = mSpeech[key];
            JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "onStartListening", TRIGGER_VOLUME, SILENT_VOLUME, phone);
            JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "onPauseListeningAndRecognition");
#endif

        changeCharacter1State(CHARACTER1_STATE::suggest, [&](){
//            setTextQuestion(mCurrentText);
            
            GameSoundManager::getInstance()->playEffectSound("Main/Common/Sounds/Effect/SFX_Button.m4a");
            
            CHARACTER1_STATE next = ((mIsBeforePronunciation == true) ? CHARACTER1_STATE::induceHappy : CHARACTER1_STATE::induceIdle);
            
            this->changeCharacter1State(next, [&]() {
                this->changeCharacter2State(CHARACTER2_STATE::urgeCharacter);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                this->changeCharacter1State(CHARACTER1_STATE::look, [&](){
                    auto languageManager = LanguageManager::getInstance();
                    auto locale = languageManager->convertLocaleTypeToCode(languageManager->getCurrentLocaleType());

                    JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "onPauseListeningAndRecognition");
                    JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "playAudio", "Localized/" + locale + "/" + ROOT_RESOURCE_PATH + "/effect/readaloud.m4a");
                });
#else
                this->changeCharacter1State(CHARACTER1_STATE::look, [&](){
                    GameSoundManager::getInstance()->playEffectSound(ROOT_RESOURCE_PATH + "/effect/readaloud.m4a");
                    processStopLook();
                });
#endif
            });
        });
        changeCharacter2State(CHARACTER2_STATE::none);
    }
    else if (state == MAIN_STATE::record)
    {
        mIsPronunciation = true;
        changeCharacter1State(CHARACTER1_STATE::listen);
        changeCharacter2State(CHARACTER2_STATE::none);
        scheduleOnce(schedule_selector(ReadingBirdScene::onStopRecord), RECORD_TIMEOUT_SEC);
    }
    else if (state == MAIN_STATE::stop_record)
    {
        if (isScheduled(schedule_selector(ReadingBirdScene::onStopRecord)) == true)
        {
            unschedule(schedule_selector(ReadingBirdScene::onStopRecord));
        }
        
        if (isScheduled(schedule_selector(ReadingBirdScene::onStopRecordVolume)) == true)
        {
            unschedule(schedule_selector(ReadingBirdScene::onStopRecordVolume));
        }
        
        changeCharacter1State(CHARACTER1_STATE::stopListen, [=]() {
            mProgressBar->runAction(CallFunc::create([=] {
                changeCharacter1State(CHARACTER1_STATE::speakHappy);
            }));
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "onStopListeningAndRecognition");
#endif
        });
        changeCharacter2State(CHARACTER2_STATE::none);
    }
    else if (state == MAIN_STATE::listen_record)
    {
        if (mScore >= CRITERIA_SPEECH_SCORE)
        {
            changeCharacter1State(CHARACTER1_STATE::speakHappy);
        }
        else
        {
            changeCharacter1State(CHARACTER1_STATE::speakUnhappy);
        }
        
        changeCharacter2State(CHARACTER2_STATE::none);
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "playPCMAudio");
#endif
        
    }
    else if (state == MAIN_STATE::listen_original)
    {
        mIsBeforePronunciation = mIsPronunciation;
        mProgressBar->setCurrent(mCurrentProblemID, true);
        
        changeCharacter1State(CHARACTER1_STATE::look, [&](){
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if (mbSdcardData == true)
            {
                JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "playAudio", SDCARD_ROOT_ANDROID + "/" + ROOT_RESOURCE_PATH + "/sound/" + mData[mCurrentProblemID - 1].mSound);
            }
            else
            {
                auto languageManager = LanguageManager::getInstance();
                auto locale = languageManager->convertLocaleTypeToCode(languageManager->getCurrentLocaleType());
                
                JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "playAudio", "Localized/" + locale + "/" + ROOT_RESOURCE_PATH + "/sound/" + mData[mCurrentProblemID - 1].mSound);
            }
#else
            GameSoundManager::getInstance()->playEffectSound(ROOT_RESOURCE_PATH + "/sound/" + mData[mCurrentProblemID - 1].mSound);
            
            mCharacter1->runAction(Sequence::create(DelayTime::create(3.0), CallFunc::create([=] {
                changeNextProblem();
            }), nullptr));
#endif
            
        });
        changeCharacter2State(CHARACTER2_STATE::urgeSentence);
    }
    else if (state == MAIN_STATE::complete)
    {
        setTextQuestion("");
        GameSoundManager::getInstance()->playEffectSound(ROOT_RESOURCE_PATH + "/effect/dancing.m4a");
        changeCharacter1State(CHARACTER1_STATE::dancingA, [&]() {
            CompletePopup::create()->show(1.3f, [](){
                CCAppController::sharedAppController()->handleGameComplete(1);
            });});
        changeCharacter2State(CHARACTER2_STATE::dancingB);
    }
    
    if (isDebug)
    {
        Label* label = (Label *)getChildByTag(LABEL_MAIN_STATE);
        label->setString("main : " + strMainState[(int)mMainState]);
    }
}

void ReadingBirdScene::changeCharacter1State(CHARACTER1_STATE state, std::function<void()> callback)
{
    auto winSize = getContentSize();
    Animation* animation;
    Vector<SpriteFrame*> frames;
    CHARACTER1_STATE beforeState = mCharacter1State;
    
    mCharacter1->stopAllActions();
    
    if (state == CHARACTER1_STATE::appear)
    {
        frames = getAnimationInFile("appear%04d.png", 9, 53);
        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
        setCharacter1Sprite(frames.front());
        mCharacter1->setPosition(winSize.width / 2 + (1791 - 983) / 2, winSize.height / 2 + (1636 - 1593) / 2 + CHARACTER1_OFFSET_POS_Y);
        
        mCharacter1->runAction(Sequence::create(Animate::create(animation)
                                                ,CallFunc::create([=] {
            if (callback != nullptr)
            {
                callback();
            }
        }), nullptr));
        
        mCharacter1->runAction(RepeatForever::create(Animate::create(animation)));
    }
    else if (state == CHARACTER1_STATE::idle)
    {
        frames = getAnimationInFile("idle%04d.png", 1, 6);
        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
        setCharacter1Sprite(frames.front());
        mCharacter1->runAction(Sequence::create(Animate::create(animation), CallFunc::create([=] {
            if (callback != nullptr)
            {
                callback();
            }
        }), nullptr));
    }
    else if (state == CHARACTER1_STATE::suggest)
    {
        if (mbShortCurrentText == true)
        {
            frames = getAnimationInFile("suggest_A%04d.png", 1, 27);
            setCharacter1Sprite(frames.front());
            mCharacter1->setPosition(winSize.width / 2, winSize.height / 2 + (1817 - 1593) / 2 + CHARACTER1_OFFSET_POS_Y);
        }
        else
        {
            frames = getAnimationInFile("suggest_B%04d.png", 1, 33);
            setCharacter1Sprite(frames.front());
            mCharacter1->setPosition(winSize.width / 2 + (2159 - 983) / 2 - 383, winSize.height / 2 + (1603 - 1593) / 2 + CHARACTER1_OFFSET_POS_Y);
        }
        
        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
        mCharacter1->runAction(Sequence::create(Animate::create(animation), CallFunc::create([=] {
            if (callback != nullptr)
            {
                callback();
            }
        }), nullptr));
    }
    else if (state == CHARACTER1_STATE::induceIdle || state == CHARACTER1_STATE::induceHappy)
    {
        if (state == CHARACTER1_STATE::induceIdle)
        {
            if (mbShortCurrentText == true)
            {
                frames = getAnimationInFile("induceldle_A%04d.png", 1, 18);
            }
            else
            {
                frames = getAnimationInFile("induceldle_B%04d.png", 1, 19);
            }
        }
        else
        {
            if (mbShortCurrentText == true)
            {
                frames = getAnimationInFile("induceHappy_A%04d.png", 1, 18);
            }
            else
            {
                frames = getAnimationInFile("induceHappy_B%04d.png", 1, 19);
            }
        }

        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
        setCharacter1Sprite(frames.front());
        mCharacter1->runAction(Sequence::create(Animate::create(animation), CallFunc::create([=] {
            setTextQuestion(mCurrentText);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "onResumeListeningAndRecognition");
#endif
            mCharacter1->runAction(Sequence::create(DelayTime::create(3.0), CallFunc::create([=] {
                if (callback != nullptr)
                {
                    callback();
                }
            }), nullptr));
        }), nullptr));
    }
    else if (state == CHARACTER1_STATE::look)
    {
        if (beforeState == CHARACTER1_STATE::induceIdle || beforeState == CHARACTER1_STATE::induceHappy)
        {
            if (mbShortCurrentText == true)
            {
                frames = getAnimationInFile("look_A%04d.png", 1, 15);
            }
            else
            {
                frames = getAnimationInFile("look_B%04d.png", 1, 15);
            }
            
        }
        else
        {
            if (mbShortCurrentText == true)
            {
                frames = getAnimationInFile("look_A%04d.png", 10, 15);
            }
            else
            {
                frames = getAnimationInFile("look_B%04d.png", 5, 15);
            }
            
        }
        
        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
        setCharacter1Sprite(frames.front());
        mCharacter1->runAction(Sequence::create(Animate::create(animation), CallFunc::create([=] {
            if (callback != nullptr)
            {
                callback();
            }
        }), nullptr));
    }
    else if (state == CHARACTER1_STATE::stopLook)
    {
        if (mbShortCurrentText == true)
        {
            frames = getAnimationInFile("stopLook_A%04d.png", 1, 15);
        }
        else
        {
            frames = getAnimationInFile("stopLook_B%04d.png", 1, 15);
        }
        
        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
        setCharacter1Sprite(frames.front());
        mCharacter1->runAction(Sequence::create(Animate::create(animation), CallFunc::create([=] {
            if (callback != nullptr)
            {
                callback();
            }
        }), nullptr));
    }
    else if (state == CHARACTER1_STATE::listen)
    {
        if (mbShortCurrentText == true)
        {
            frames = getAnimationInFile("listen_A%04d.png", 1, 20);
        }
        else
        {
            frames = getAnimationInFile("listen_B%04d.png", 1, 20);
        }
        
        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
        setCharacter1Sprite(frames.front());
        mCharacter1->runAction(Animate::create(animation));
    }
    else if (state == CHARACTER1_STATE::stopListen)
    {
        if (mbShortCurrentText == true)
        {
            frames = getAnimationInFile("stopListen_A%04d.png", 1, 17);
        }
        else
        {
            frames = getAnimationInFile("stopListen_B%04d.png", 1, 17);
        }
        
        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
        setCharacter1Sprite(frames.front());
        mCharacter1->runAction(Sequence::create(Animate::create(animation), CallFunc::create([=] {
            if (callback != nullptr)
            {
                callback();
            }
        }), nullptr));
    }
    else if (state == CHARACTER1_STATE::speakHappy)
    {
        if (mbShortCurrentText == true)
        {
            frames = getAnimationInFile("induceldle_A%04d.png", 1, 1);
        }
        else
        {
            frames = getAnimationInFile("induceldle_B%04d.png", 1, 1);
        }
        
        
        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
        setCharacter1Sprite(frames.front());
        mCharacter1SpeakType = SPEAK_TYPE::silent;
        
        mCharacter1->runAction(RepeatForever::create(Animate::create(animation)));
    }
    else if (state == CHARACTER1_STATE::speakUnhappy)
    {
        if (mbShortCurrentText == true)
        {
            frames = getAnimationInFile("speakUnhappy1_A%04d.png", 1, 1);
        }
        else
        {
            frames = getAnimationInFile("speakUnhappy1_B%04d.png", 1, 1);
        }
        
        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
        setCharacter1Sprite(frames.front());
        mCharacter1SpeakType = SPEAK_TYPE::silent;
        
        mCharacter1->runAction(RepeatForever::create(Animate::create(animation)));
    }
    else if (state == CHARACTER1_STATE::dancingA)
    {
        frames = getAnimationInFile("dancingA%04d.png", 1, 16);
        
        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS2);
        setCharacter1Sprite(frames.front());
        mCharacter1SpeakType = SPEAK_TYPE::silent;
        
        mCharacter1->runAction(RepeatForever::create(Animate::create(animation)));
        mCharacter1->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=] {
            if (callback != nullptr)
            {
                callback();
            }
        }), nullptr));
    }
    
    mCharacter1State = state;
    
    if (isDebug)
    {
        Label* label = (Label *)getChildByTag(LABEL_CHARACTER1_STATE);
        label->setString("c1 : " + strCharacter1State[(int)mCharacter1State]);
    }
    
    CCLOG("mCharacter1State : %s", strCharacter1State[(int)mCharacter1State].c_str());
    
}

void ReadingBirdScene::changeCharacter2State(CHARACTER2_STATE state, std::function<void()> callback)
{
    auto winSize = getContentSize();
    Animation* animation;
    Vector<SpriteFrame*> frames;
    CHARACTER2_STATE beforeState = mCharacter2State;
    
    mCharacter2->stopAllActions();
    
    if (state == CHARACTER2_STATE::none && (beforeState == CHARACTER2_STATE::urgeCharacter || beforeState == CHARACTER2_STATE::urgeSentence))
    {
        mCharacter2State = CHARACTER2_STATE::none;
        frames = getAnimationInFile("urgeCharacter%04d.png", 97, 114);
        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
        setCharacter2Sprite(frames.front());
        mCharacter2->runAction(Sequence::create(Animate::create(animation), CallFunc::create([=] {
            mCharacter2->setVisible(false);
        }), nullptr));
        return;
    }
    
    mCharacter2->setVisible(state != CHARACTER2_STATE::none);
    
    if (state == CHARACTER2_STATE::urgeCharacter || state == CHARACTER2_STATE::urgeSentence)
    {
        mCharacter2SpeakType = SPEAK_TYPE::silent;
        frames = getAnimationInFile("urgeCharacter%04d.png", 1, 29);
        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
        setCharacter2Sprite(frames.front());
        mCharacter2->runAction(Sequence::create(Animate::create(animation), CallFunc::create([=] {
            if (callback != nullptr)
            {
                callback();
            }
        }), nullptr));
        
    }
    else if (state == CHARACTER2_STATE::dancingB)
    {
        frames = getAnimationInFile("dancing%04d.png", 1, 57);
        animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS2);
        setCharacter2Sprite(frames.front());
        mCharacter2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        mCharacter2->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 250 + CHARACTER1_OFFSET_POS_Y));
        mCharacter2->runAction(RepeatForever::create(Animate::create(animation)));
    }
    
    mCharacter2State = state;
    
    if (isDebug)
    {
        Label* label = (Label *)getChildByTag(LABEL_CHARACTER2_STATE);
        label->setString("c2 : " + strCharacter2State[(int)mCharacter2State]);
    }

    CCLOG("mCharacter2State : %s", strCharacter2State[(int)mCharacter2State].c_str());
}

void ReadingBirdScene::processStopLook()
{
    this->changeCharacter1State(CHARACTER1_STATE::stopLook, [&](){

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "onResumeListeningAndRecognition");
#endif

        this->changeCharacter2State(CHARACTER2_STATE::none);
        CHARACTER1_STATE next2 = ((mIsBeforePronunciation == true) ? CHARACTER1_STATE::induceHappy : CHARACTER1_STATE::induceIdle);
        this->changeCharacter1State(next2, [=]() {
            
            this->changeMainState(MAIN_STATE::listen_original);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "onStopListeningAndRecognition");
#endif
            
        });
    });
}

void ReadingBirdScene::onStopRecord(float f)
{
    CCLOG("onStopRecord");
    if (mMainState == MAIN_STATE::record)
    {
        changeMainState(MAIN_STATE::stop_record);
    }
}

void ReadingBirdScene::onStopRecordVolume(float f)
{
    CCLOG("onStopRecordVolume");
    if (mMainState == MAIN_STATE::record)
    {
        changeMainState(MAIN_STATE::stop_record);
    }
}

string ReadingBirdScene::makeWorkPath() const
{
    stringstream ss;
    ss << "ReadingBird";
    ss << "/" << "level-" << mLevelID;
    ss << "/" << "work-" << mCurrentProblemID;
    
    return ss.str();
}

void ReadingBirdScene::checkVolume(int volume)
{
    if (volume > SILENT_VOLUME)
    {
        unschedule(schedule_selector(ReadingBirdScene::onStopRecordVolume));
    }
    else
    {
        if (isScheduled(schedule_selector(ReadingBirdScene::onStopRecordVolume)) == false)
        {
            scheduleOnce(schedule_selector(ReadingBirdScene::onStopRecordVolume), RECORD_VOLUME_TIMEOUT_SEC);
        }
    }
}

void ReadingBirdScene::setScore(int score)
{
    mScore = score;
    if (mScore >= CRITERIA_SPEECH_SCORE)
    {
        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/Result_RightAnswer.m4a");
    }

    if (score >= 0) {
        StrictLogManager::shared()->game_Peek_Answer("ReadingBird", makeWorkPath(), TodoUtil::itos(score), TodoUtil::itos(CRITERIA_SPEECH_SCORE));
    }

    
    if (isDebug)
    {
        Label* label = (Label *)getChildByTag(LABEL_SCORE);
        if (score == -1)
        {
            label->setString("");
        }
        else
        {
            label->setString(StringUtils::format("score : %d", score));
        }
    }
}

void ReadingBirdScene::setSdcardData(bool bSdcardData)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    mbSdcardData = bSdcardData;
    CCLOG("setSdcardData : %d", (int)mbSdcardData);
    if (mbSdcardData == true)
    {
        SDCARD_ROOT_ANDROID = JniHelper::callStaticStringMethod(JNI_CLASS_NAME, "getExternalStorageDirectory");
    }
#else
    mbSdcardData = false;
#endif
}

void ReadingBirdScene::changeNextProblem()
{
    changeCharacter2State(CHARACTER2_STATE::none);
    this->mCharacter1->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=] {
        if (mData[mData.size() - 1].mProblem == mCurrentProblemID)
        {
            this->changeMainState(MAIN_STATE::complete);
        }
        else
        {
            this->changeMainState(MAIN_STATE::wait);
        }
        
    }), nullptr));
}

void ReadingBirdScene::changeTalkingAnimation(int volume, bool recursive)
{
    Vector<SpriteFrame*> frames;
    Animation* animation;
    
    if (mMainState == MAIN_STATE::listen_record)
    {
        auto winSize = getContentSize();
        if (mCharacter1 != nullptr && (mCharacter1State == CHARACTER1_STATE::speakHappy || mCharacter1State == CHARACTER1_STATE::speakUnhappy))
        {
            if (volume <= SILENT_VOLUME)
            {
                mIsSilent = true;
                if (mCharacter1SpeakType != SPEAK_TYPE::silent && recursive == true)
                {
                    if (mbShortCurrentText == true)
                    {
                        if (mCharacter1State == CHARACTER1_STATE::speakHappy)
                        {
                            frames = getAnimationInFile("induceldle_A%04d.png", 1, 1);
                        }
                        else
                        {
                            frames = getAnimationInFile("speakUnhappy1_A%04d.png", 1, 1);
                        }
                    }
                    else
                    {
                        if (mCharacter1State == CHARACTER1_STATE::speakHappy)
                        {
                            frames = getAnimationInFile("induceldle_B%04d.png", 1, 1);
                        }
                        else
                        {
                            frames = getAnimationInFile("speakUnhappy1_B%04d.png", 1, 1);
                        }
                    }
                    
                    animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
                    mCharacter1->stopAllActions();
                    setCharacter1Sprite(frames.front());
                    mCharacter1->runAction(RepeatForever::create(Animate::create(animation)));
                    mCharacter1SpeakType = SPEAK_TYPE::silent;
                }
            }
            else
            {
                mIsSilent = false;
                if (mCharacter1SpeakType == SPEAK_TYPE::silent || recursive == true)
                {
                    mCharacter1SpeakType = (SPEAK_TYPE)cocos2d::RandomHelper::random_int(1, 3);
                    
                    if (mCharacter1State == CHARACTER1_STATE::speakHappy)
                    {
                        if (mbShortCurrentText == true)
                        {
                            if (mCharacter1SpeakType == SPEAK_TYPE::type_1)
                            {
                                frames = getAnimationInFile("speakHappy1_A%04d.png", 1, 10);
                            }
                            else if (mCharacter1SpeakType == SPEAK_TYPE::type_2)
                            {
                                frames = getAnimationInFile("speakHappy2_A%04d.png", 1, 11);
                            }
                            else if (mCharacter1SpeakType == SPEAK_TYPE::type_3)
                            {
                                frames = getAnimationInFile("speakHappy3_A%04d.png", 1, 13);
                            }
                        }
                        else
                        {
                            if (mCharacter1SpeakType == SPEAK_TYPE::type_1)
                            {
                                frames = getAnimationInFile("speakHappy1_B%04d.png", 1, 10);
                            }
                            else if (mCharacter1SpeakType == SPEAK_TYPE::type_2)
                            {
                                frames = getAnimationInFile("speakHappy2_B%04d.png", 1, 11);
                            }
                            else if (mCharacter1SpeakType == SPEAK_TYPE::type_3)
                            {
                                frames = getAnimationInFile("speakHappy3_B%04d.png", 1, 13);
                            }
                        }
                    }
                    else
                    {
                        if (mbShortCurrentText == true)
                        {
                            if (mCharacter1SpeakType == SPEAK_TYPE::type_1)
                            {
                                frames = getAnimationInFile("speakUnhappy1_A%04d.png", 1, 9);
                            }
                            else if (mCharacter1SpeakType == SPEAK_TYPE::type_2)
                            {
                                frames = getAnimationInFile("speakUnhappy2_A%04d.png", 1, 6);
                            }
                            else if (mCharacter1SpeakType == SPEAK_TYPE::type_3)
                            {
                                frames = getAnimationInFile("speakUnhappy3_A%04d.png", 1, 7);
                            }
                        }
                        else
                        {
                            if (mCharacter1SpeakType == SPEAK_TYPE::type_1)
                            {
                                frames = getAnimationInFile("speakUnhappy1_B%04d.png", 1, 8);
                            }
                            else if (mCharacter1SpeakType == SPEAK_TYPE::type_2)
                            {
                                frames = getAnimationInFile("speakUnhappy2_B%04d.png", 1, 6);
                            }
                            else if (mCharacter1SpeakType == SPEAK_TYPE::type_3)
                            {
                                frames = getAnimationInFile("speakUnhappy3_B%04d.png", 1, 6);
                            }
                        }
                    }
                    
                    animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
                    
                    mCharacter1->stopAllActions();
                    setCharacter1Sprite(frames.front());
                    
                    mCharacter1->runAction(Sequence::create(Animate::create(animation), CallFunc::create([=] {
                        if (mIsSilent == false)
                        {
                            changeTalkingAnimation(TRIGGER_VOLUME, true);
                        }
                        else
                        {
                            changeTalkingAnimation(SILENT_VOLUME, true);
                        }
                    }), nullptr));
                    
                }
            }
        }
    }
    else if (mMainState == MAIN_STATE::wait || mMainState == MAIN_STATE::listen_original)
    {
        if (mCharacter2 != nullptr && (mCharacter2State == CHARACTER2_STATE::urgeCharacter || mCharacter2State == CHARACTER2_STATE::urgeSentence))
        {
            if (volume <= SILENT_VOLUME)
            {
                mIsSilent = true;
                if (mCharacter2SpeakType != SPEAK_TYPE::silent && recursive == true)
                {
                    frames = getAnimationInFile("urgeCharacter%04d.png", 30, 30);
                    animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
                    mCharacter2->stopAllActions();
                    setCharacter2Sprite(frames.front());
                    mCharacter2->runAction(RepeatForever::create(Animate::create(animation)));
                    mCharacter2SpeakType = SPEAK_TYPE::silent;
                }
            }
            else
            {
                mIsSilent = false;
                if (mCharacter2SpeakType == SPEAK_TYPE::silent || recursive == true)
                {
                    frames = getAnimationInFile("urgeCharacter%04d.png", 31, 96);
                    mCharacter2SpeakType = SPEAK_TYPE::type_1;
                    animation = Animation::createWithSpriteFrames(frames, 1.0f/ANIMATION_FPS);
                    
                    mCharacter2->stopAllActions();
                    setCharacter2Sprite(frames.front());
                    
                    mCharacter2->runAction(Sequence::create(Animate::create(animation), CallFunc::create([=] {
                        if (mIsSilent == false)
                        {
                            changeTalkingAnimation(TRIGGER_VOLUME, true);
                        }
                        else
                        {
                            changeTalkingAnimation(SILENT_VOLUME, true);
                        }
                    }), nullptr));
                    
                }
            }
        }
    }
}

void ReadingBirdScene::processOnRecordVolume(int volume)
{
    if (mMainState != MAIN_STATE::record && mMainState != MAIN_STATE::stop_record)
    {
        if (isDebug)
        {
            Label* label = (Label *)getChildByTag(LABEL_VOLUME);
            label->setString(StringUtils::format("trigger volume : %d", volume));
        }

        mTriggerVolume = volume;
        changeMainState(MAIN_STATE::record);
    }
    
    if (mMainState == MAIN_STATE::record)
    {
        if (mTriggerVolume != 0) {
            double val = 1.0 - ((mTriggerVolume - volume) / (double)mTriggerVolume);
            double amplification = val * val;
            volume = (int)(amplification * mTriggerVolume);
        }
        
        CCLOG("scale volume : %d", volume);
        checkVolume(volume);
    }
}

void ReadingBirdScene::processOnRecordScore(int score)
{
    setScore(score);
    
    if (mMainState == MAIN_STATE::wait && mCharacter1State > CHARACTER1_STATE::suggest)
    {
        changeMainState(MAIN_STATE::listen_original);
    }
    else if (mMainState == MAIN_STATE::record)
    {
        changeMainState(MAIN_STATE::listen_original);
    }
    else if (mMainState == MAIN_STATE::stop_record)
    {
        changeMainState(MAIN_STATE::listen_record);
    }
}

void ReadingBirdScene::processOnStopPlayAudio()
{
    if (mMainState == MAIN_STATE::wait && mCharacter2State == CHARACTER2_STATE::urgeCharacter)
    {
        processStopLook();
    }
    else if (mMainState == MAIN_STATE::listen_record)
    {
        changeMainState(MAIN_STATE::listen_original);
    }
    else if (mMainState == MAIN_STATE::listen_original)
    {
        changeNextProblem();
    }
}

void ReadingBirdScene::processOnPlayVolume(int volume)
{
    changeTalkingAnimation(volume);
}

vector<int> ReadingBirdScene::levelIDsFor(bool isSdCard, string sdcardPath)
{
    vector<int> result;
    string p;
    if (isSdCard == true)
    {
        p = sdcardPath + "/Games/" + ASSET_PREFIX + "/ReadingBird_Levels.tsv";
    }
    else
    {
        p = LanguageManager::getInstance()->findLocalizedResource("Games/" + ASSET_PREFIX + "/ReadingBird_Levels.tsv");
    }
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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

ReadingBirdScene* getCurrentScene()
{
    ReadingBirdScene* result = nullptr;
    
    auto director = Director::getInstance();
    Scene* scene = nullptr;
    if (director != nullptr)
    {
        scene = director->getRunningScene();
        if (scene != nullptr)
        {
            result = (ReadingBirdScene*)(scene->getChildByTag(CURRENT_SCENE_ID));
            if (result == nullptr)
            {
                Vector<Node *> children = scene->getChildren();
                for (Node * child : children)
                {
                    result = (ReadingBirdScene*)(child->getChildByTag(CURRENT_SCENE_ID));
                    if (result != nullptr)
                    {
                        break;
                    }
                }
            }
        }
    }

    return result;
}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_ReadingBird_SpeechRecognition_onRecordVolume(JNIEnv* env, jobject obj, jint volume)
    {
        CCLOG("JNI : onRecordVolume Start : %d", volume);
        ReadingBirdScene* scene = getCurrentScene();
        if (scene != nullptr)
        {
            auto scheduler = cocos2d::Director::getInstance()->getScheduler();
            scheduler->performFunctionInCocosThread(CC_CALLBACK_0(ReadingBirdScene::processOnRecordVolume, scene, volume));
        }
        CCLOG("JNI : onRecordVolume End");
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_ReadingBird_SpeechRecognition_onRecordScore(JNIEnv* env, jobject obj, jint score)
    {
        CCLOG("JNI : onRecordScore Start : %d", score);
        ReadingBirdScene* scene = getCurrentScene();
        if (scene != nullptr)
        {
            auto scheduler = cocos2d::Director::getInstance()->getScheduler();
            scheduler->performFunctionInCocosThread(CC_CALLBACK_0(ReadingBirdScene::processOnRecordScore, scene, score));
        }
        CCLOG("JNI : onRecordScore End");
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_ReadingBird_PlayAudio_onStop(JNIEnv* env, jobject obj)
    {
        CCLOG("JNI : onStop");
        ReadingBirdScene* scene = getCurrentScene();
        if (scene != nullptr)
        {
            auto scheduler = cocos2d::Director::getInstance()->getScheduler();
            scheduler->performFunctionInCocosThread(CC_CALLBACK_0(ReadingBirdScene::processOnStopPlayAudio, scene));
        }
        CCLOG("JNI : onStop End");
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_ReadingBird_PlayAudio_onPlayVolume(JNIEnv* env, jobject obj, jint volume)
    {
        CCLOG("JNI : onPlayVolume Start : %d", volume);
        ReadingBirdScene* scene = getCurrentScene();
        if (scene != nullptr)
        {
            auto scheduler = cocos2d::Director::getInstance()->getScheduler();
            scheduler->performFunctionInCocosThread(CC_CALLBACK_0(ReadingBirdScene::processOnPlayVolume, scene, volume));
        }
        CCLOG("JNI : onPlayVolume End");
    }
}
#endif

