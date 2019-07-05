//
//  WordMachineScene.cpp
//  KitkitSchool
//
//  Created by timewalker on 7/1/16.
//
//

#include "Managers/LanguageManager.hpp"
#include "WordMachineScene.hpp"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Managers/GameSoundManager.h"
#include "Managers/UserManager.hpp"
#include "Managers/StrictLogManager.h"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include <Common/Controls/CompletePopup.hpp>
#include <Common/Basic/SoundEffect.h>
#include <Managers/VoiceMoldManager.h>
#include "Common/Sounds/CommonSound.hpp"
#include "Utils/TodoUtil.h"


#include "CCAppController.hpp"

using namespace cocostudio;
using namespace cocos2d::ui;
using namespace todoschool::wordmachine;
using namespace todoschool;

static bool __wmDebug = false;
const Size panelSize = Size(692, 564);

namespace {
    SoundEffect soundForLetterName(const string& Letter) {
        return CommonSound().soundForLetterName(Letter);
    }
    
    SoundEffect soundForPhonic(const char c) {
        return CommonSound().soundForPhonic(string{c});
    }
    
}  // unnamed namespace


Scene* WordMachineScene::createScene(int levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = WordMachineScene::create(levelID);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

//
//void readLegitWordList(std::vector<std::string> &wordVector, std::vector<std::string> &imageVector, std::string filename)
//{
//    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
//    if (str.length()==0) return;
//    
//    auto data = TodoUtil::readCSV(str);
//    
//    for (int i=0; i<data.size(); i++) {
//        if (i==0) continue;
//        
//        auto row = data[i];
//        if (row.size()<3) continue;
//        
//        auto word = row[1];
//        auto image = row[2];
//        wordVector.push_back(word);
//        imageVector.push_back(image);
//        
//        if (__wmDebug) {
//            std::string resPath = "WordMachine/WordCards/";
//            
//            resPath += (LanguageManager::getInstance()->isEnglish()) ? "en/" : "sw/";
//            resPath += "legit/";;
//            
//            auto sp = Sprite::create(resPath+image);
//            CCASSERT(sp!=nullptr, "Image does not exist");
//            
//            CCASSERT(FileUtils::getInstance()->isFileExist(resPath+word+".m4a"), "Audio does not exist");
//            
//        }
//        
//    }
//}
//
//
//void readMadeupWordList(std::vector<std::string> &wordVector, std::string filename)
//{
//    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
//    if (str.length()==0) return;
//    
//    auto data = TodoUtil::readCSV(str);
//    
//    for (int i=0; i<data.size(); i++) {
//        if (i==0) continue;
//        
//        auto row = data[i];
//        if (row.size()<2) continue;
//        
//        auto word = row[1];
//
//        wordVector.push_back(word);
//        
//    }
//}

bool WordMachineScene::init(int levelID)
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto winSize = getContentSize();
    _gameSize = Size(2560, 1800);
    _levelID = levelID;
    
    // Background
    Sprite* background = Sprite::create("WordMachine/Images/wm_bg.png");
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setPosition(winSize / 2);
    addChild(background);
    
    // GameNode
    _gameNode = Node::create();
    _gameNode->setContentSize(_gameSize);
    float gameScale = MIN(winSize.width/_gameSize.width, winSize.height/_gameSize.height);
    _gameNode->setScale(gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(winSize/2);
    addChild(_gameNode);

    
    // init problem
    _targetIsLegit = true;
    
    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    
    
    if (UserManager::getInstance()->isDebugMode()){
        vector<string> _missings;
        _missings.clear();
        
        LevelData levelData = LevelData::defaultData();
        for (auto it : levelData.levels_) {
            for (auto worksheet : it.second) {
                for (int i=worksheet.beginProblemID(); i<worksheet.endProblemID(); i++) {
                    auto p = worksheet.problemByID(i);
                    std::string resPath = "WordMachine/WordCards/";
                    {
                        auto sound = resPath + p.sound;
                        
                        if (!FileUtils::getInstance()->isFileExist(sound)) {
                            if (find(_missings.begin(), _missings.end(), p.sound)==_missings.end()) {
                                _missings.push_back(p.sound);
                            }
                        }
                    }
                    {
                        auto image = resPath + p.goodImage;
                        
                        if (!FileUtils::getInstance()->isFileExist(image)) {
                            if (find(_missings.begin(), _missings.end(), p.goodImage)==_missings.end()) {
                                _missings.push_back(p.goodImage);
                            }
                        }
                    }
                    {
                        auto image = resPath + p.badImage;
                        
                        if (!FileUtils::getInstance()->isFileExist(image)) {
                            if (find(_missings.begin(), _missings.end(), p.badImage)==_missings.end()) {
                                _missings.push_back(p.badImage);
                            }
                        }
                    }
                    
                    
                }
            }
    
        }
        
        for (auto m : _missings) {
            CCLOG("%s\n", m.c_str());
        }
        
    }
    
    auto worksheet = LevelData::defaultData().randomSheetFor(lang, _levelID, &_worksheetID);
    _worksheet = worksheet;
    _currentProblemID = worksheet.beginProblemID();
    

    
    auto sampleProblem = _worksheet.problemByID(1);
    auto sampleWord = sampleProblem.word;
    
    int sampleLength = sampleWord.length();
    auto wheelConfig = "CCC";//"CVC";
    if (sampleLength==4) wheelConfig = "CCCC";//"CVCV";
    if (sampleLength==5) wheelConfig = "CCCCC";
    if (sampleLength==6) wheelConfig = "CCCCCC";
    initWheels(sampleLength, wheelConfig);
    
    
    
//    if (LanguageManager::getInstance()->isEnglish()) {
//        initWheels(3, "CVC");
//       
//        // NB(xenosoz, 2016): Those word/image vector were used pre-leveling era.
//        //readLegitWordList(_legitWords, _legitImages, "WordMachine/WordCards/en_legit.csv");
//        //readMadeupWordList(_madeupWords, "WordMachine/WordCards/en_madeup.csv");
//        
//    } else {
//        initWheels(4, "CVCV");
//        
//        // NB(xenosoz, 2016): Those word/image vector were used pre-leveling era.
//        //readLegitWordList(_legitWords, _legitImages, "WordMachine/WordCards/sw_legit.csv");
//        //readMadeupWordList(_madeupWords, "WordMachine/WordCards/sw_madeup.csv");
//    }
    
    // Start Button
    
    GameSoundManager::getInstance()->preloadEffect("WordMachine/button.m4a");
    GameSoundManager::getInstance()->preloadEffect("Common/Sounds/Effect/UI_Star_Collected.m4a");
    GameSoundManager::getInstance()->preloadEffect("WordMachine/boing1.m4a");
    
    
    _startButton = Button::create();
    _startButton->loadTextures("WordMachine/Images/wm_button_1.png", "WordMachine/Images/wm_button_2.png");
    _startButton->setPosition(Vec2(_gameSize.width / 2, _gameSize.height / 2 - 500.f));
    _startButton->addClickEventListener([this](Ref*) {
        _leftCard->removeAllChildren();
        _rightCard->removeAllChildren();
        startMachine();
        _startButton->setEnabled(false);
        GameSoundManager::getInstance()->playEffectSound("WordMachine/button.m4a");
        
    });
    _gameNode->addChild(_startButton);
    
    // Left Blank Frame
    {
        auto place = Node::create();
        place->setContentSize(panelSize);
        place->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        place->setPosition(_gameSize.width/2-750, _gameSize.height/2-500);
        _gameNode->addChild(place);
        
        auto placeBg = Sprite::create("WordMachine/Images/wm_word_frame_2.png");
        placeBg->setPosition(panelSize/2);
        place->addChild(placeBg);
        
        auto stencil = Sprite::create("WordMachine/Images/wm_wordslot_card.png");
        stencil->setPosition(Vec2(panelSize)/2);
        stencil->setScale(0.95);
        
        auto clip = ClippingNode::create(stencil);
        clip->setAlphaThreshold(0.5);
        place->addChild(clip);
        
        _leftCard = Node::create();
        _leftCard->setContentSize(panelSize);
        clip->addChild(_leftCard);
        
        auto frame = Sprite::create("WordMachine/Images/wm_word_frame.png");
        frame->setPosition(panelSize/2);
        place->addChild(frame);
    }
    
    
    // Right Blank Frame
    {
        auto place = Node::create();
        place->setContentSize(panelSize);
        place->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        place->setPosition(_gameSize.width/2+750, _gameSize.height/2-500);
        _gameNode->addChild(place);
        
        auto placeBg = Sprite::create("WordMachine/Images/wm_word_frame_2.png");
        placeBg->setPosition(panelSize/2);
        place->addChild(placeBg);
        
        auto stencil = Sprite::create("WordMachine/Images/wm_wordslot_card.png");
        stencil->setPosition(Vec2(panelSize)/2);
        stencil->setScale(0.95);
        
        auto clip = ClippingNode::create(stencil);
        clip->setAlphaThreshold(0.5);
        place->addChild(clip);
        
        _rightCard = Node::create();
        _rightCard->setContentSize(panelSize);
        clip->addChild(_rightCard);
        
        auto frame = Sprite::create("WordMachine/Images/wm_word_frame.png");
        frame->setPosition(panelSize/2);
        place->addChild(frame);
    }
    

    // Progress Bar
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    _progressBar->setMax(worksheet.size());
    _progressBar->setCurrent(_currentProblemID, false);
    
    // Back Button
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    

    return true;
}

void WordMachineScene::initWheels(int numWheels, std::string types)
{
    _validWheelCount = numWheels;
    
    
    float factor = 1.0;
    if (numWheels==5) factor = 0.9;
    if (numWheels==6) factor = 0.8;
    
    auto slotWidth = 545.0 * factor;
    
    
    
    auto slotX = _gameNode->getContentSize().width/2-slotWidth*(numWheels-1)/2.0;// _gameSize.width/2-815;
    auto slotY = _gameSize.height / 2 + 300.f;
    
    
    // Wheel Slot Background
//    auto wheelSlotBackground = Sprite::create("WordMachine/Images/wm_slotbg.png");
//    wheelSlotBackground->setPosition(_gameSize.width / 2, _gameSize.height / 2 + 300.f);
//    _gameNode->addChild(wheelSlotBackground);
    
    for (int i=0; i<numWheels; i++) {
        auto bg = Sprite::create("WordMachine/Images/wm_4letters_slotwheel_2.png");
        
        bg->setPosition(slotX, slotY);
        bg->setScaleX(factor);
        _gameNode->addChild(bg);
        
        WordType wtype = types[i]=='C' ? WordType::Consonant : WordType::Vowel;
        
        auto wheel = CreateWheel(wtype, (i>=numWheels-3));
        wheel->setScaleX(factor);
        wheel->setPosition(bg->getPosition());
        wheel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _wheels.push_back(wheel);
        _gameNode->addChild(wheel);
        
        slotX+=slotWidth;
        
    }
    
    slotX = _gameNode->getContentSize().width/2-slotWidth*(numWheels-1)/2.0;;

    for (int i=0; i<numWheels; i++) {
        Sprite *cover;
        if (i==0) {
            cover = Sprite::create("WordMachine/Images/wm_oneslot_frame_left.png");
            cover->setPosition(Vec2(slotX-3, slotY));
        } else if (i==numWheels-1) {
            cover = Sprite::create("WordMachine/Images/wm_oneslot_frame_right.png");
            cover->setPosition(Vec2(slotX+3, slotY));
        } else {
            cover = Sprite::create("WordMachine/Images/wm_oneslot_frame.png");
            cover->setPosition(Vec2(slotX, slotY));
        }
        cover->setScaleX(factor);
        _gameNode->addChild(cover);
        slotX += slotWidth;
    }

}

void WordMachineScene::onWheelsStop()
{
    std::string resPath = "WordMachine/WordCards/";
    auto sound = resPath + _currentProblem.sound;
    scheduleOnce([sound, this](float){  // Sound delay
        VoiceMoldManager::shared()->speak(_currentProblem.sound);
        showImageCard();
    }, 0.7, "WheelWord");
}

void WordMachineScene::handleCorrectAnswer()
{
    
    if (_currentProblemID + 1 >= _worksheet.endProblemID()) {
        handleSuccess();
        return;
    }

    _currentProblemID += 1;
    _startButton->setEnabled(true);
    _progressBar->setCurrent(_currentProblemID, false);
}

void WordMachineScene::handleMadeupWordDone()
{
    if (_currentProblemID + 1 >= _worksheet.endProblemID()) {
        handleSuccess();
        return;
    }

    _currentProblemID += 1;
    _startButton->setEnabled(true);
}

void WordMachineScene::handleSuccess()
{
    auto CP = CompletePopup::create();
    CP->show(1.f, [this] {
        CCAppController::sharedAppController()->handleGameComplete(1);
        //TodoSchoolBackButton::popGameScene();
    });
}

void WordMachineScene::showImageCard()
{
    if (_targetIsLegit) {
        std::string resPath = "WordMachine/WordCards/";
        
        auto targetImage = resPath + _currentProblem.goodImage;
        auto alterImage = resPath + _currentProblem.badImage;
        
        
        auto alter = Node::create();
        alter->setContentSize(panelSize);
        alter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        
        auto abg = Sprite::create("WordMachine/Images/wm_wordslot_card.png");
        abg->setPosition(panelSize/2);
        alter->addChild(abg);
        
        
        auto ab = Button::create(alterImage);
        ab->setPosition(panelSize/2);
        alter->addChild(ab);
        alter->setPosition(Vec2(panelSize/2) + Vec2(0, panelSize.height));
        
        auto abCallFunc = [this, alter, ab]() {
            ab->addClickEventListener([this, alter](Ref*) {
                GameSoundManager::getInstance()->playEffectSound("WordMachine/boing1.m4a");
                alter->runAction(Sequence::create(EaseOut::create(MoveBy::create(0.05, Vec2(-30, 0)), 2.0),
                                                  EaseInOut::create(MoveBy::create(0.1, Vec2(60, 0)), 2.0),
                                                  EaseIn::create(MoveBy::create(0.05, Vec2(-30, 0)), 2.0),
                                                  nullptr));
                
                // NB(xenosoz, 2018): Log for future analysis (#1/2)
                string workPath = [this] {
                    stringstream SS;
                    SS << "/" << "WordMachine";
                    SS << "/" << "level-" << _levelID << "-" << _worksheetID;
                    SS << "/" << "work-" << _currentProblemID;
                    return SS.str();
                }();
                StrictLogManager::shared()->game_Peek_Answer("WordMachine", workPath, _currentProblem.badImage, _currentProblem.goodImage);
            });
        };
        
        alter->runAction(Sequence::create(DelayTime::create(random<float>(0.0, 0.2)),
                                          EaseBounceOut::create(MoveTo::create(0.5, panelSize/2)),
                                          CallFunc::create(abCallFunc),
                                          nullptr));
        
        
        auto target = Node::create();
        target->setContentSize(panelSize);
        target->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        
        auto tbg = Sprite::create("WordMachine/Images/wm_wordslot_card.png");
        tbg->setPosition(panelSize/2);
        target->addChild(tbg);
        
        auto tb = Button::create(targetImage);
        tb->setPosition(panelSize/2);
        tb->setTag(0);
        target->addChild(tb);
         _speakMachineWord=" ";
        auto audioPath = resPath + _currentProblem.sound;
        _speakMachineWord=_currentProblem.sound;
        target->setPosition(Vec2(panelSize/2) + Vec2(0, panelSize.height));
        auto tbCallFunc = [this, target, alter, audioPath, tb, ab]() {
            tb->addClickEventListener([this, target, alter, audioPath, ab](Ref* btn) {
                Button *tb = (Button*)btn;
                
                if (tb->getTag()==1) return;
                tb->setTag(1);
                alter->runAction(EaseIn::create(MoveBy::create(0.3, Vec2(0, panelSize.height)), 2.0));
                
                target->setScale(1.1);
                GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/UI_Star_Collected.m4a");
                _progressBar->setCurrent(_currentProblemID, true);
                
                target->runAction(Sequence::create(DelayTime::create(0.5),
                                                   CallFunc::create([audioPath](){
                  VoiceMoldManager::shared()->speak(_speakMachineWord);  //Implementation of TTS for this module
                    
                }),
                                                   DelayTime::create(1.5),
                                                   EaseIn::create(MoveBy::create(0.3, Vec2(0, panelSize.height)), 2.0),
                                                   CallFunc::create([this](){
                    handleCorrectAnswer();
                }),
                                                   nullptr));
                
                // NB(xenosoz, 2018): Log for future analysis (#2/2)
                string workPath = [this] {
                    stringstream SS;
                    SS << "/" << "WordMachine";
                    SS << "/" << "level-" << _levelID << "-" << _worksheetID;
                    SS << "/" << "work-" << _currentProblemID;
                    return SS.str();
                }();
                StrictLogManager::shared()->game_Peek_Answer("WordMachine", workPath, _currentProblem.goodImage, _currentProblem.goodImage);
                //tb->addClickEventListener(nullptr);
                //ab->addClickEventListener(nullptr);
                tb->setTouchEnabled(false);
                ab->setTouchEnabled(false);
            });
        };
                          
        target->runAction(Sequence::create(DelayTime::create(random<float>(0.0, 0.2)),
                                           EaseBounceOut::create(MoveTo::create(0.5, panelSize/2)),
                                           CallFunc::create(tbCallFunc),
                                           nullptr));
        
        
        if (random<int>(0, 1)==0) {
            _leftCard->addChild(target);
            _rightCard->addChild(alter);
        } else {
            _leftCard->addChild(alter);
            _rightCard->addChild(target);
        }
        
        
    } else {
        handleMadeupWordDone();
    }
    
}


void WordMachineScene::startMachine()
{
    auto problem = _worksheet.problemByID(_currentProblemID);
    _currentProblem = problem;

    _targetIsLegit = problem.legitType();
    _targetWord = problem.word;
    _numSpinningWheels = _wheels.size();
    
    std::function<void()> trigger = [this] {
        _numSpinningWheels -= 1;
        onWheelsStop();
    };

    for (size_t i = _wheels.size(); i --> 0; /* empty */) {
        auto w = _wheels[i];
        char c = toupper(_targetWord[i]);
        
        float slowdownAfter = (i == 0 ? random(0.3, 0.5) : 0.2);
        float velocity = (i % 2 == 0 ? random(5500, 6250) : random(4000, 4750));
        if (_numSpinningWheels > 4) {
            // NB(xenosoz, 2016): Go faster when slot count is large.
            slowdownAfter = (i == 0 ? 0.3 : 0.1);
            velocity = (i % 2 == 0 ? random(6000, 7000) : random(4000, 5000));
        }
        w->start(velocity);

        //soundForLetterName(string{c}).preload();
        soundForPhonic(c).preload();

        
        
        auto nextWork = [this, trigger, w, c, slowdownAfter] {
            w->stop(c, slowdownAfter, CallFunc::create([this, trigger, c]{

                //soundForLetterName(string{c}).play();
                soundForPhonic(c).play();
                
                _numSpinningWheels -= 1;
                trigger();
            }));
        };
        trigger = nextWork;
    }
    trigger();

    
#if false
    for (int i=0; i<_wheels.size(); i++) {
        auto w = _wheels[i];
        char c = toupper(_targetWord[i]);

        float slowdownAfter = (delay += random(0.7, 0.9));
        w->start(i%2==0 ? random(3000, 3500) : random(2000, 2500));
        
        w->stop(c, slowdownAfter, CallFunc::create([this, c](){
            soundForLetterName(string{c}).play();
            
            _numSpinningWheels--;
            if (_numSpinningWheels<=0) {
                onWheelsStop();
            }
        }));
    }
#endif
}

LetterWheel* WordMachineScene::CreateWheel(WordType wordType, bool playTick)
{
    std::string consonantString = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string vowelString = "AEIOU";
    
    auto w = LetterWheel::create();
    w->setup(wordType == WordType::Consonant ? consonantString : vowelString, Size(400, 700), 2.0, 270, 0.6, playTick);
    
    return w;
}
