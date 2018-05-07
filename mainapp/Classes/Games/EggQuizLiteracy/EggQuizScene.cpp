//
//  EggQuizScene.cpp
//
//  Created by Gunho Lee on 1/1/17.
//
//


#include "EggQuizScene.hpp"
#include "EggQuizButton.hpp"
#include "EggQuizPopup.hpp"

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include "ui/CocosGUI.h"

#include "Managers/GameSoundManager.h"
#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/CurriculumManager.hpp"
#include "Managers/StrictLogManager.h"


#include "Utils/TodoUtil.h"
#include "Common/Basic/ScopeGuard.h"
#include "Common/Basic/NodeScopeGuard.h"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Controls/PopupBase.hpp"

#include "Common/Basic/SoundEffect.h"

#include "3rdParty/CCNativeAlert.h"

#include "CCAppController.hpp"

using namespace cocos2d::ui;
using namespace std;


namespace EggQuizSceneSpace {
    const char* solveEffect = "Common/Sounds/Effect/UI_Star_Collected.m4a";
    const char* missEffect = "Common/Sounds/Effect/Help.m4a";
    const char* pageTurnEffect = "Common/Sounds/Effect/Card_Move_Right.m4a";
    const char* touchEffect = "Common/Sounds/Effect/paneltouch.m4a";
    
    const char* defaultFont = "fonts/Andika-R.ttf";
    
    const string curlyFont = "fonts/TodoMainCurly.ttf";
    
    const Size gameSize = Size(2560, 1800);
    Size winSize;
    
    const string folder = "EggQuizLiteracy/";
    const string imageFolder = folder + "Image/";
    const string audioFolder = folder + "Audio/";
    
    
    
    //const Rect QuestionTopRect = Rect(0, 800, 2560, 800);
    //const Rect AnswerBottomRect = Rect(0, 0, 2560, 800);
    
    //const Rect QuestionLeftRect = Rect(0, 0, 1280, 1600);
    //const Rect AnswerRightRect = Rect(1280, 0, 1280, 1600);
    
    
    //const Size AnswerSizeWord = Size(500, 300);
    //const Size AnswerSizeSentence = Size(2000, 200);
    //const Size AnswerSizeRight = Size(1000, 400);
    
    const int tagSoundButtonNormal = 1001;
    const int tagSoundButtonActive = 1002;
    const int tagSoundButtonNode = 1003;

    const int tagAutoSequence = 3001;
    const int tagStopSoundButtonOff = 3002;

};

using namespace EggQuizSceneSpace;



Scene* EggQuizScene::createScene(string level)
{
    char cat = 'L';
    std::transform(level.begin(), level.end(), level.begin(), ::tolower);
    return createScene(cat, level, false);
}

Scene* EggQuizScene::createScene(char category, string level, bool isPreTest)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = EggQuizScene::create();
    layer->setLevel(category, level, isPreTest);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool EggQuizScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _touchEnabled = false;
    onCompleteCallback = nullptr;
    
    
    GameSoundManager::getInstance()->preloadEffect(solveEffect);
    GameSoundManager::getInstance()->preloadEffect(missEffect);
    GameSoundManager::getInstance()->preloadEffect(pageTurnEffect);
    
    winSize = getContentSize();
    _winSize = winSize;
    
    auto bg = Sprite::create("EggQuizLiteracy/Common/pretest_layout_bg.jpg");
    auto bgSize = bg->getContentSize();
    float bgScale = MAX(_winSize.width/bgSize.width, _winSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(_winSize/2);
    addChild(bg);
    
    /*
     auto paperBottom = Sprite::create("EggQuizLiteracy/Common/pretest_layout_page.png");
     paperBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
     paperBottom->setPosition(winSize.width / 2, 0.f);
     addChild(paperBottom);
     */
    
    _pageNode = Node::create();
    _pageNode->setContentSize(_winSize);
    addChild(_pageNode);
    _currentPageGridZ = 0;
    
    _gameSize = Size(2560, 1800);
    //_gameSize = Size(3200, 1800);
    _gameScale = _winSize.height / 1800;
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, _winSize.height-25));
    addChild(backButton);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(_winSize.width/2, _winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    auto paperTop = Sprite::create("EggQuizLiteracy/Common/pretest_layout_page.png");
    paperTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    paperTop->setPosition(_winSize.width / 2, 0.f);
    addChild(paperTop);
    _paperTop = paperTop;
    
    auto gameNode = Node::create();
    gameNode->setContentSize(_gameSize);
    gameNode->setScale(_gameScale);
    gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    gameNode->setPosition(Vec2(_winSize.width/2, _winSize.height/2));

    addChild(gameNode);
    _gameNode = gameNode;

    
    if (UserManager::getInstance()->isDebugMode()) {
        auto pass = Button::create();
        pass->setTitleText("Pass");
        pass->setTitleFontName(defaultFont);
        pass->setTitleFontSize(50);
        pass->setTitleColor(Color3B::BLACK);
        pass->setPosition(Vec2(winSize.width-200, winSize.height-50));
        pass->addClickEventListener([this](Ref*) {
            for (int i=0; i<_problemSet.size(); i++) {
                _answerResult[i] = true;
            }
            this->onComplete();
        });
        addChild(pass);
        
        
        auto fail = Button::create();
        fail->setTitleText("Fail");
        fail->setTitleFontName(defaultFont);
        fail->setTitleFontSize(50);
        fail->setTitleColor(Color3B::BLACK);
        fail->setPosition(Vec2(winSize.width-500, winSize.height-50));
        fail->addClickEventListener([this](Ref*) {
            for (int i=0; i<_problemSet.size(); i++) {
                _answerResult[i] = false;
            }
            this->onComplete();
        });
        addChild(fail);
        
        
        auto next = Button::create();
        next->setTitleText("Next");
        next->setTitleFontName(defaultFont);
        next->setTitleFontSize(50);
        next->setTitleColor(Color3B::BLACK);
        next->setPosition(Vec2(winSize.width-800, winSize.height-50));
        next->addClickEventListener([this](Ref*) {
            this->onSolve();
        });
        addChild(next);
        
    }
    
    return true;
}

void EggQuizScene::setLevel(char category, string level, bool isPreTest)
{
    _quizCategory = category;
    _currentLevel = level;
    _levelData = LevelData::defaultData();
    _duration = LevelData::loadDurationsheet();
    _isPreTest = isPreTest;
    
    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    
    auto numSet = _levelData.numSets(lang, category, level);
    _problemSet = _levelData.getProblemSet(lang, category, level, random(1, numSet));
    
    _answerResult.clear();
    for (int i=0; i<_problemSet.size(); i++) {
        _answerResult.push_back(false);
    }
    
    if (UserManager::getInstance()->isDebugMode()) {
        
        /*
        auto text = StringUtils::format("[%c-%s] as %s of [%c-%d]", category, level.c_str(), isPreTest ? "PreTest" : "PostTest", category, isPreTest ? level+1 : level);
        auto l = TodoUtil::createLabel(text, 50, Size::ZERO, defaultFont, Color4B::BLACK);
        l->setPosition(winSize.width/2, winSize.height-100);
        addChild(l);
         */
    }
}



void EggQuizScene::onEnter()
{
    Layer::onEnter();
    
    
    
    
}

void EggQuizScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    onStart();
    
    if (false) {
        auto popupSize = Size(800, 800);
        auto popup = PopupBase::create(this, popupSize);
        
        auto lc = LayerColor::create(Color4B::WHITE, popupSize.width, popupSize.height);
        popup->addChild(lc);
        
        char buf[1024];
        sprintf(buf, "Make %d correct answers out of %d quiz questions", (int)(_problemSet.size()*0.8),(int)(_problemSet.size()) );
        
        
        auto desc = TodoUtil::createLabelMultiline(buf, 50, Size(500, 300), defaultFont, Color4B::BLACK);
        desc->setPosition(Vec2(popupSize.width/2, 600));
        popup->addChild(desc);
        
        auto modeDesc = TodoUtil::createLabel(_isPreTest ? "PreTest" : "PostTest", 50, Size::ZERO, defaultFont, Color4B::BLACK);
        modeDesc->setPosition(Vec2(popupSize.width/2, 400));
        popup->addChild(modeDesc);
        
        
        {
            auto startBtn = ui::Button::create();
            startBtn->loadTextures("CoopScene/LevelOpenPopup/main_button_normal.png", "CoopScene/LevelOpenPopup/main_button_active.png");
            startBtn->setZoomScale(0);
            auto startL = TodoUtil::createLabel(LanguageManager::getInstance()->getLocalizedString("Start"), 70, Size::ZERO, defaultFont, Color4B(255, 249, 235, 255));
            auto startLPos = Vec2(startBtn->getContentSize()/2 + Size(0, 10));
            
            startL->setPosition(startLPos);
            startBtn->addChild(startL);
            startBtn->addTouchEventListener([startBtn, startL, startLPos](Ref*, ui::Widget::TouchEventType){
                startL->setPosition(startBtn->isHighlighted() ? startLPos+Vec2(0, -12) : startLPos);
            });
            
            startBtn->addClickEventListener([this, popup](Ref*) {
                onStart();
                popup->dismiss(true);
            });
            startBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            startBtn->setPosition(Vec2(popupSize.width/2, 114));
            popup->addChild(startBtn);
        }
        
        
        popup->show(this, true);
        
    }
    
    //onStart();
}



void EggQuizScene::onStart()
{
    this->setTouchEnabled(true);
    
    if (_problemSet.size()==0) return;
    
    
    _currentProblemIndex = 0;
    _currentProblemObject = _problemSet[_currentProblemIndex];
    
    
    _progressBar->setMax((int)_problemSet.size(), true);
    _progressBar->setCurrent(_currentProblemIndex + 1);
    
    showProblem();
    
    
    
    //    _
    //    float appearTime = 0;
    //    putObjects(appearTime);
}

void EggQuizScene::onSolve() {
    GameSoundManager::getInstance()->stopAllEffects();
    GameSoundManager::getInstance()->stopBGM();

    stopActionByTag(tagAutoSequence);
    stopActionByTag(tagStopSoundButtonOff);

    setTouchEnabled(false);
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    
    _currentProblemIndex++;
    updateProgressBar(true);
    
    if (_currentProblemIndex == _problemSet.size()) {
        onComplete();
        return;
    }
    
    _gameNode->runAction(
        Sequence::create(
        DelayTime::create(0.5f),
        CallFunc::create([this](){ _gameNode->removeAllChildren();}),
        DelayTime::create(0.1f),
        CallFunc::create([this](){
            this->setTouchEnabled(true);
            showProblem();
        }),
        nullptr)
    );
    
}

void EggQuizScene::updateProgressBar(bool setCurrent) {
    string progressStatus = "";
    for (int i=0; i<_problemSet.size(); i++) {
        string c = _answerResult[i] ? "o" : "x";
        if (setCurrent && i==_currentProblemIndex) c = "c";
        else if (i>=_currentProblemIndex) c = "-";
        progressStatus += c;
    }
    CCLOG("%s", progressStatus.c_str());
    _progressBar->setStatus(progressStatus);
}

void EggQuizScene::onCorrect(const string& answer, int answerIndex)
{
    StrictLogManager::shared()->eggQuiz_CorrectAnswer(_currentProblemIndex, answer, answerIndex);
    
    _answerResult[_currentProblemIndex] = true;
    onSolve();
}

void EggQuizScene::onIncorrect(const string& correctAnswer, const string& myAnswer, int answerIndex)
{
    StrictLogManager::shared()->eggQuiz_WrongAnswer(_currentProblemIndex, correctAnswer, myAnswer, answerIndex);
    
    _answerResult[_currentProblemIndex] = false;
    onSolve();
}


void EggQuizScene::onComplete()
{
    
    
    int numCorrect = 0;
    for (auto s : _answerResult) {
        if (s) numCorrect++;
    }
    bool cleared = (numCorrect>=_problemSet.size()*0.8);
    
    
    if (cleared) {
        passTest();
        
    } else {
        failTest();
    }
    
}


NodeGrid* EggQuizScene::createPageGrid()
{
    
    auto grid = NodeGrid::create();
    grid->setContentSize(winSize);
    
    
    return grid;
}


void EggQuizScene::showProblem()
{
    _currentProblemObject = _problemSet[_currentProblemIndex];
    
    _gameNode->addChild(createLiteracyQuestionView());
    _gameNode->addChild(createLiteracyAnswerView());
    
}

Node* EggQuizScene::createSoundButton(string fileName, float duration, float size, bool autoStart) {
    Node *node = Node::create();
    auto fileNameVec = TodoUtil::split(fileName, '/');
    auto playFileName = fileNameVec.back();
    
    if (_duration.count(playFileName)) duration = _duration[playFileName];
    
    auto soundNormal = Sprite::create("EggQuizLiteracy/Common/pretest_speaker_normal.png");
    soundNormal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    node->addChild(soundNormal);
    soundNormal->setTag(tagSoundButtonNormal);
    
    auto soundActive = Sprite::create("EggQuizLiteracy/Common/pretest_speaker_active.png");
    soundActive->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    soundActive->setVisible(false);
    node->addChild(soundActive);
    soundActive->setTag(tagSoundButtonActive);
    
    node->setContentSize(soundNormal->getContentSize());
    soundNormal->setPosition(soundNormal->getContentSize()/2);
    soundActive->setPosition(soundActive->getContentSize()/2);
    
    node->setScale((float)size/node->getContentSize().width);
    
    auto onTouch = [this, node, duration, fileName](){
        GameSoundManager::getInstance()->playEffectSoundVoiceOnly(fileName);
        node->setTag(tagSoundButtonNode);
        node->getChildByTag(tagSoundButtonActive)->setVisible(true);
        node->getChildByTag(tagSoundButtonNormal)->setVisible(false);
        
        auto sequence = Sequence::create(
                                         DelayTime::create(duration),
                                         CallFunc::create([node](){
            node->setTag(0);
            node->getChildByTag(tagSoundButtonActive)->setVisible(false);
            node->getChildByTag(tagSoundButtonNormal)->setVisible(true);
        }),
                                         nullptr
                                         );
        sequence->setTag(tagStopSoundButtonOff);
        this->runAction(sequence);
    };
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, node, duration, fileName, onTouch](Touch* T, Event* E) {
        auto P = node->getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        
        if (node->getBoundingBox().containsPoint(pos)) {
            // if (node->getTag() == tagSoundButtonNode) return false;
            GameSoundManager::getInstance()->stopAllEffects();
            this->stopActionByTag(tagAutoSequence);
            this->stopActionByTag(tagStopSoundButtonOff);
            onTouch();
        }
        return false;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    if (autoStart) {

        auto autoSequence = Sequence::create(
                                             DelayTime::create(0.5f),
                                             CallFunc::create([onTouch](){
            onTouch();
        }),
        //DelayTime::create(duration+1.f),
        //CallFunc::create([onTouch](){ onTouch(); }),
                                             nullptr
                                             );
        autoSequence->setTag(tagAutoSequence);
        this->runAction(autoSequence);
    }
    
    return node;
}


Node* EggQuizScene::createLiteracyQuestionView()
{
    
    
    
    auto P = _currentProblemObject;
    
    auto qt = P.questionType;
    auto at = P.answerType;
    
    
    
    Node *node = Node::create();
    
    if (qt==SOUNDONLY) {
        auto soundPath = audioFolder + _currentProblemObject.questionAudio;
        auto soundButton = createSoundButton(soundPath, 1.f, 500, true);
        soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        node->addChild(soundButton);
        node->setPosition(gameSize.width/2, 1000);
        
    } else if (qt == QuestionType::WORD) {
        
        auto viewSize = Size(1340, 508);
        
        if (_currentProblemObject.questionAudio.length()>1) {
            viewSize = Size(1890, 508);
            
            auto soundPath = audioFolder + _currentProblemObject.questionAudio;
            auto soundButton = createSoundButton(soundPath, 1.f, 500, true);
            soundButton->setPosition(Vec2(0, viewSize.height/2));
            node->addChild(soundButton);
        }
        
        node->setContentSize(viewSize);
        node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        node->setPosition(gameSize.width/2, 1060);
        
        auto boxSize = Size(1340, 508);
        auto box = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
        box->setPreferredSize(boxSize);
        box->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        box->setPosition(viewSize.width, 0);
        node->addChild(box);
        
        
        //auto l = TodoUtil::createLabel(_currentProblemObject.questionStrings[0], 80, Size::ZERO, defaultFont, Color4B(70, 70, 70, 255));
        auto l = createSingleLine(_currentProblemObject.questionStrings[0], 112, boxSize);
        l->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        l->setPosition(boxSize.width/2, boxSize.height/2-20);
        box->addChild(l);
        
    } else if (qt==SENTENCE) {
        
        auto viewSize = Size(1890, 508);
        
        if (_currentProblemObject.questionAudio.length()>1) {
            viewSize = Size(1890, 408);
            
            auto soundPath = audioFolder + _currentProblemObject.questionAudio;
            auto soundButton = createSoundButton(soundPath, 1.f, 250, true);
            soundButton->setPosition(Vec2(0, viewSize.height+150));
            soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            node->addChild(soundButton);
        }
        node->setContentSize(viewSize);
        node->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        node->setPosition(gameSize.width/2, 1060 - 254);
        
        auto boxSize = viewSize;
        auto box = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
        box->setPreferredSize(boxSize);
        box->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        box->setPosition(viewSize.width, 0);
        node->addChild(box);
        
        auto labelSize = boxSize - Size(250, 100);
        
        //auto l = TodoUtil::createLabelMultilineToFit(_currentProblemObject.questionStrings[0], 80, labelSize, defaultFont, Color4B(70, 70, 70, 255));
        auto l = createMultiLine(_currentProblemObject.questionStrings[0], 80, labelSize);
        l->setPosition(boxSize/2);
        box->addChild(l);
        
        
        
    } else if (qt==PARAGRAPH) {
        
        auto viewSize = Size(1040, 1226);
        
        if (_currentProblemObject.questionAudio.length()>1) {
            viewSize = Size(1040, 926);
            
            auto soundPath = audioFolder + _currentProblemObject.questionAudio;
            auto soundButton = createSoundButton(soundPath, 1.f, 250, true);
            soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            soundButton->setPosition(Vec2(viewSize.width/2, viewSize.height+150));
            node->addChild(soundButton);
        }
        
        node->setContentSize(viewSize);
        node->setPosition(262, 185);
        
        auto boxSize = viewSize;
        auto box = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
        box->setPreferredSize(boxSize);
        box->setPosition(viewSize/2);
        node->addChild(box);
        
        auto labelSize = boxSize - Size(250, 250);
        
        //auto l = TodoUtil::createLabelMultilineToFit(_currentProblemObject.questionStrings[0], 60, labelSize, defaultFont, Color4B(70, 70, 70, 255));
        auto l = createMultiLine(_currentProblemObject.questionStrings[0], 60, labelSize);
        l->setPosition(boxSize/2);
        box->addChild(l);
        
        
    } else if (qt == IMAGE) {
        
        
        auto viewSize = Size(1340, 508);
        
        
        
        auto box = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
        node->addChild(box);
        
        if (_currentProblemObject.questionAudio.length()>1) {
            viewSize = Size(1890, 508);
            
            auto soundPath = audioFolder + _currentProblemObject.questionAudio;
            auto soundButton = createSoundButton(soundPath, 1.f, 250, true);
            soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            soundButton->setPosition(Vec2(viewSize.height/2, viewSize.height/2));
            node->addChild(soundButton);
        }
        
        auto imagePos = Vec2(viewSize/2);
        auto frameSize = Size(472, 332);
        
        if (P.questionQuestion.length()>1) {
            viewSize = Size(1890, 508);
            
            {
                
                auto textSize = Size(1000, 332);
                //auto l = TodoUtil::createLabelMultilineToFit(P.questionQuestion, 100, textSize, defaultFont, Color4B(70, 70, 70, 255));
                auto l = createMultiLine(P.questionQuestion, 100, textSize);
                
                l->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                auto lSize = l->getContentSize();
                auto totalWidth = frameSize.width + lSize.width + 100;
                
                l->setPosition(Vec2(viewSize.width/2 + totalWidth/2 - lSize.width/2, viewSize.height/2));
                box->addChild(l);
                
                imagePos = Vec2(viewSize.width/2 - totalWidth/2 + frameSize.width/2, viewSize.height/2);
            }
            
        }
        
        auto frame = Sprite::create("EggQuizLiteracy/Common/pretest_picture_withoutbg.png");
        frame->setPosition(imagePos);
        
        auto frameCenter = frame->getContentSize()/2;
        box->addChild(frame);
        
        auto sp = Sprite::create(imageFolder+_currentProblemObject.questionStrings[0]);
        if (sp) {
            auto spSize = sp->getContentSize();
            sp->setScale(MIN(frameSize.width / spSize.width, frameSize.height / spSize.height));
            
            sp->setPosition(frameCenter);
            frame->addChild(sp);
        } else {
            auto l = TodoUtil::createLabel(_currentProblemObject.questionStrings[0], 80, Size::ZERO, defaultFont, Color4B(70, 70, 70, 255));
            l->setPosition(frameCenter);
            frame->addChild(l);
        }
        
        
        node->setContentSize(viewSize);
        node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        node->setPosition(gameSize.width/2, 1060);
        
        auto boxSize = viewSize;
        box->setPreferredSize(boxSize);
        box->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        box->setPosition(viewSize.width, 0);
        
    } else if (qt == IMAGESEQ) {
        
        Size viewSize = Size(1890, 508);
        
        if (_currentProblemObject.questionAudio.length()>1) {
            viewSize = Size(1890, 508);
            
            auto soundPath = audioFolder + _currentProblemObject.questionAudio;
            auto soundButton = createSoundButton(soundPath, 1.f, 250, true);
            soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            soundButton->setPosition(Vec2(0, viewSize.height+150));
            node->addChild(soundButton);
        }
        
        Vec2 imageCenter = viewSize/2;
        
        auto box = ui::Scale9Sprite::create("EggQuizLiteracy/Common/pretest_questionbox.png");
        node->addChild(box);
        
        if (P.questionQuestion.length()>1) {
            
            viewSize = Size(1948, 700);
            imageCenter = Vec2(viewSize.width/2, 700-230);
            
            
            //auto l = TodoUtil::createLabelMultilineToFit(P.questionQuestion, 60, Size(1720, 170), defaultFont, Color4B(70, 70, 70, 255));
            auto l = createMultiLine(P.questionQuestion, 60, Size(1720, 170));
            l->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            l->setPosition(Vec2(130, 200));
            box->addChild(l);
        }
        
        for (int i=0; i<3; i++) {
            auto frame = Sprite::create("EggQuizLiteracy/Common/pretest_picture_withoutbg.png");
            frame->setPosition(imageCenter + Vec2(622*(i-1), 0));
            auto frameSize = Size(472, 332);
            box->addChild(frame);
            auto frameCenter = frame->getContentSize()/2;
            
            
            auto filename = _currentProblemObject.questionStrings[i];
            if (filename=="-" || filename=="?") {
                filename = "EggQuizLiteracy/Common/pretest_english_filltheblank.png";
            } else {
                filename = imageFolder+filename;
            }
            
            auto sp = Sprite::create(filename);
            if (sp) {
                auto spSize = sp->getContentSize();
                sp->setScale(MIN(frameSize.width / spSize.width, frameSize.height / spSize.height));
                sp->setPosition(frameCenter);
                frame->addChild(sp);
            } else {
                
                //auto l = TodoUtil::createLabelMultilineToFit(_currentProblemObject.questionStrings[i], 80, frameSize-Size(20, 20), defaultFont, Color4B(70, 70, 70, 255));
                auto l = createMultiLine(_currentProblemObject.questionStrings[i], 80, frameSize-Size(20, 20));
                l->setPosition(frameCenter);
                frame->addChild(l);
                
            }
            
            if (i>0) {
                auto arrow = Sprite::create("EggQuizLiteracy/Common/pretest_english_arrow.png");
                arrow->setPosition(imageCenter + Vec2(622*(i-1.5), 0));
                box->addChild(arrow);
            }
        }
        
        node->setContentSize(viewSize);
        node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        node->setPosition(gameSize.width/2, 750 + viewSize.height/2 + 30);
        
        auto boxSize = viewSize;
        box->setPreferredSize(boxSize);
        box->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        box->setPosition(viewSize.width, 0);
        
        
    }
    
    
    /*
     if (P.questionAudio.length()>1) {
     auto soundBtn = createSoundButton(qt != SOUNDONLY);
     
     if (qt==SOUNDONLY) {
     soundBtn->setPosition(viewRect.size/2);
     } else {
     soundBtn->setPosition(Vec2(100, viewRect.size.height-100));
     }
     node->addChild(soundBtn);
     }
     */
    
    
    
    
    
    return node;
    
    
}



Node* EggQuizScene::createLiteracyAnswerView()
{
    
    
    Node *node = Node::create();
    
    Rect viewRect = Rect((2560-1920)/2, 80, 1920, 680);
    
    
    auto P = _currentProblemObject;
    
    auto qt = P.questionType;
    auto at = P.answerType;
    bool hori = true;
    
    vector<EggQuizButton*> buttons;
    
    if (qt==PARAGRAPH) {
        viewRect = Rect(1310, 185+150, 1080, 926);
        
        if (P.questionQuestion.length()>1) {
            
            //auto l = TodoUtil::createLabel(P.questionQuestion, 60, Size::ZERO, defaultFont, Color4B::BLACK);
            auto l = createMultiLine(P.questionQuestion, 60, Size(viewRect.size.width, 150));
            
            l->setPosition(Vec2(viewRect.size.width/2, 1126));
            node->addChild(l);
            
            viewRect.origin.y -= 150;
            
        }
        hori = false;
    } else if (at==CHOOSE_3_SENTENCE) {
        hori = false;
        viewRect = Rect((2560-1950)/2, 100, 1950, 600);
    } else if (at==CHOOSE_3_PARAGRAPH) {
    }
    
    node->setContentSize(viewRect.size);
    node->setPosition(viewRect.origin);
    
    _currentAnswerOrder = { 0, 1, 2 };
    random_shuffle(_currentAnswerOrder.begin(), _currentAnswerOrder.end());
    
    bool foundCorrectAnswer = false;
    
    for (int i=0; i<3; i++) {
        auto ai = _currentAnswerOrder[i];
        
        auto a = EggQuizButton::create();
        buttons.push_back(a);
        
        
        node->addChild(a);
        
        auto correctAnswerString = P.correctAnswerString;
        auto answerString = P.answerStrings[ai];
        
        bool correctAnswer = P.answerStrings[ai]==P.correctAnswerString;
        if (correctAnswer) foundCorrectAnswer = true;
        
        auto cp = _currentProblemIndex;
        
        a->addClickEventListener([buttons, correctAnswer, cp, correctAnswerString, answerString, i, this](Ref*) {
            if (!isTouchEnabled()) {
                return;
            }
            if (cp!=_currentProblemIndex) {
                return;
            }
            
            for (auto b : buttons) {
                b->setTouchEnabled(false);
            }
            if (correctAnswer) {
                this->onCorrect(correctAnswerString, i);
            } else {
                this->onIncorrect(correctAnswerString, answerString, i);
            }
        });
        
        if (at == CHOOSE_3_WORD) {
            if (qt==PARAGRAPH) {
                a->setWord(_currentProblemObject.answerStrings[ai], Size(948, 260));
            } else {
                a->setWord(_currentProblemObject.answerStrings[ai]);
            }
        } else if (at == CHOOSE_3_SENTENCE) {
            if (qt==PARAGRAPH) {
                a->setSentence(_currentProblemObject.answerStrings[ai], Size(948, 260));
            } else {
                a->setSentence(_currentProblemObject.answerStrings[ai]);
            }
        } else if (at == CHOOSE_3_PARAGRAPH) {
            if (qt==PARAGRAPH) {
                a->setParagraph(_currentProblemObject.answerStrings[ai]);
            } else {
                hori = false;
                a->setParagraph(_currentProblemObject.answerStrings[ai], Size(1948, 210));
            }
            
            
        } else if (at == CHOOSE_3_IMAGE) {
            a->setImage(imageFolder+_currentProblemObject.answerStrings[ai]);
        }
        
        auto answerSize = a->getContentSize();
        
        if (hori) {
            auto dist = (viewRect.size.width - answerSize.width)/2;
            
            a->setPosition(Vec2(viewRect.size.width/2.0 + dist*(i-1), viewRect.size.height/2));
        } else {
            auto dist = (viewRect.size.height - answerSize.height)/2;
            a->setPosition(Vec2(viewRect.size.width/2, viewRect.size.height/2 + dist*(1-i)));
        }
        
        
    }
    
    if (!foundCorrectAnswer) {
        NativeAlert::show("Error in EggQuiz", "No correct answer found", "OK");
    }
    
    
    if (at!=CHOOSE_3_IMAGE) {
        float minScale = -1;
        for (auto b : buttons) {
            if (minScale < 0 || b->getLabelScale() < minScale) minScale = b->getLabelScale();
        }
        
        for (auto b : buttons) {
            b->setLabelScale(minScale);
        }
    }
    
    return node;
    
    
    
    
}

Node* createWord(string word, float fontSize) {
    
    auto underlineSize = fontSize*0.5;
    auto wordHeight = fontSize;
    
    Node *node = Node::create();
    Size wordSize = Size(0, wordHeight);
    
    while (word.length()>0) {
        if (word[0]=='_')
        {
            
            int underlineLength;
            
            size_t last = word.find_first_not_of("_");
            if (last==string::npos) {
                underlineLength = (int)word.length();
                word = "";
                
            } else {
                underlineLength = (int)last;
                word = word.substr(last, word.length()-last);
            }
            
            auto width = underlineLength*underlineSize;
            
            auto slot = DrawNode::create();
            slot->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            
            
            slot->drawLine(Vec2::ZERO, Vec2(width, 0), Color4F::BLACK);
            auto slotSize = Size(width, wordHeight);
            slot->setContentSize(slotSize);
            slot->setPosition(Vec2(wordSize.width, 0));
            
            wordSize.width += slotSize.width;
            
            node->addChild(slot);
        }
        
        
        if (word.length()>0) {
            
            string currentSyl;
            
            size_t nextUnder = word.find_first_of("_");
            if (nextUnder!=string::npos) {
                currentSyl = word.substr(0, nextUnder);
                word = word.substr(nextUnder, word.length()-nextUnder);
                
            } else {
                currentSyl = word;
                word = "";
            }
            
            auto label = TodoUtil::createLabel(currentSyl, fontSize, Size::ZERO, defaultFont, Color4B::BLACK);
            label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            auto labelSize = label->getContentSize();
            
            
            label->setPosition(Vec2(wordSize.width, 0));
            
            wordSize.width += labelSize.width;
            node->addChild(label);
        }
        
        
    }
    
    node->setContentSize(wordSize);
    
    return node;
    
}

Node* EggQuizScene::createMultiLine(string text, float fontSize, Size dim)
{
    
    vector<string> words = TodoUtil::split(text, ' ');
    
    bool complete = false;
    Node *page;
    
    int trial = 5;
    
    
    do {
        
        page = Node::create();
        page->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        Node *innerPage = Node::create();
        
        
        Point pos = Point::ZERO;
        Size pageSize = Size::ZERO;
        
        float pageTopMargin = 0;
        
        auto pageWidth = dim.width;
        auto wordHeight = fontSize;
        auto lineSpacing = fontSize*0.25;
        auto wordSpacing = fontSize*0.25;
        
        
        auto underlineSize = fontSize*0.5;
        
        
        for (auto word : words) {
            
            auto slot = createWord(word, fontSize);
            auto slotSize = slot->getContentSize();
            slot->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            if (pageWidth>0 && pos.x + slotSize.width > pageWidth) {
                pos.x = 0;
                pos.y -= wordHeight + lineSpacing;
            }
            
            slot->setPosition(pos);
            
            pageSize.width = MAX(pageSize.width, pos.x+slotSize.width);
            pageSize.height = MIN(pageSize.height, pos.y-slotSize.height);
            
            pageTopMargin = MAX(pageTopMargin, pos.y);
            
            pos.x += wordSpacing + slotSize.width;
            
            innerPage->addChild(slot);
            
            
            
        }
        
        
        pageSize.height *= -1.0;
        pageSize.height += pageTopMargin;
        
        page->setContentSize(pageSize);
        innerPage->setPosition(Vec2(0, pageSize.height-pageTopMargin));
        
        page->addChild(innerPage);
        
        
        complete = pageSize.height <= dim.height;
        if (!complete) {
            auto scale = sqrt(dim.height / pageSize.height)* 0.9;
            fontSize *= scale;
        }
        
        trial--;
        
        
        
    } while (!complete && trial>0);
    
    
    return page;
}


Node* EggQuizScene::createSingleLine(string text, float fontSize, Size dim)
{
    auto ret = createMultiLine(text, fontSize, Size(0, dim.height));
    auto retSize = ret->getContentSize();
    if (retSize.width > dim.width) {
        ret->setScale(dim.width / retSize.width);
    }
    
    return ret;
}

void EggQuizScene::passTest()
{
    auto popup = EggQuizPopup::create(this, winSize);
    
    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/pretest_success.m4a");
    auto levelVec = TodoUtil::split(_currentLevel, '_');

    if (levelVec.size()>1) {
        popup->setPostCompleted('L', TodoUtil::stoi(levelVec[1]));
    }

    this->runAction(Sequence::create(DelayTime::create(1.0),
                                     CallFunc::create([](){SoundEffect::wowEffect().play();}),
                                     nullptr
                                     ));
    
    popup->show(this, true, CallFunc::create([this](){
        
        CCAppController::sharedAppController()->handleGameComplete(1);
        
        if (onCompleteCallback) {
            onCompleteCallback(true);
        }
    }));
    
    
}

void EggQuizScene::failTest()
{
    auto popup = EggQuizPopup::create(this, winSize);
    
    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/pretest_fail1.m4a");
    
    auto levelVec = TodoUtil::split(_currentLevel, '_');
    if (levelVec.size()>1) {
        popup->setPostFailed('L', TodoUtil::stoi(levelVec[1]));
    }

    
    
    popup->show(this, true, CallFunc::create([this](){
        
        CCAppController::sharedAppController()->handleGameQuit();
        if (onCompleteCallback) {
            onCompleteCallback(false);
        }
    }));
}
