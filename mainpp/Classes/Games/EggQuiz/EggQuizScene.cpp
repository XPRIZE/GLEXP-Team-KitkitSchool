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
#include "ui/cocosGUI.h"

#include "Managers/GameSoundManager.h"
#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/CurriculumManager.hpp"


#include "Utils/TodoUtil.h"
#include <Games/NumberTrace/Common/Basic/ScopeGuard.h>
#include <Games/NumberTrace/Common/Basic/NodeScopeGuard.h>
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Controls/PopupBase.hpp"

#include "Common/Basic/SoundEffect.h"

#include "3rdParty/CCNativeAlert.h"

#include "CCAppController.hpp"

using namespace cocos2d::ui;
using namespace std;


namespace EggQuizSceneSpace {
    const char* solveEffect = "Counting/UI_Star_Collected.m4a";
    const char* missEffect = "Counting/Help.m4a";
    const char* pageTurnEffect = "DoubleDigit/Card_Move_Right.m4a";
    const char* touchEffect = "Counting/paneltouch.m4a";
    
    const char* defaultFont = "fonts/Aileron-Regular.otf";
    
    const string curlyFont = "fonts/TodoMainCurly.ttf";

    const Size gameSize = Size(2560, 1800);
    Size winSize;
    
    const string folder = "EggQuiz/";
    const string imageFolder = folder + "Image/";
    const string audioFolder = folder + "Audio/";
    
    
    
    //const Rect QuestionTopRect = Rect(0, 800, 2560, 800);
    //const Rect AnswerBottomRect = Rect(0, 0, 2560, 800);
    
    //const Rect QuestionLeftRect = Rect(0, 0, 1280, 1600);
    //const Rect AnswerRightRect = Rect(1280, 0, 1280, 1600);
    
    
    //const Size AnswerSizeWord = Size(500, 300);
    //const Size AnswerSizeSentence = Size(2000, 200);
    //const Size AnswerSizeRight = Size(1000, 400);
    
    
    
    
};

using namespace EggQuizSceneSpace;



Scene* EggQuizScene::createScene(int level)
{
    char cat = 'L';
    bool pretest;
    
    if (level>10) {
        pretest = true;
        level -= 11;
        if (level>5) {
            cat = 'M';
            level -= 5;
        }
    } else {
        pretest = false;
        if (level>5) {
            cat = 'M';
            level -= 5;
        }
    }
    
    return createScene(cat, level, pretest);
}

Scene* EggQuizScene::createScene(char category, int level, bool isPreTest)
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
    
    auto bg = Sprite::create("EggQuiz/Common/pretest_layout_bg.jpg");
    auto bgSize = bg->getContentSize();
    float bgScale = MAX(winSize.width/bgSize.width, winSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(winSize/2);
    addChild(bg);
    
    /*
    auto paperBottom = Sprite::create("EggQuiz/Common/pretest_layout_page.png");
    paperBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    paperBottom->setPosition(winSize.width / 2, 0.f);
    addChild(paperBottom);
    */
    
    _pageNode = Node::create();
    _pageNode->setContentSize(winSize);
    addChild(_pageNode);
    _currentPageGridZ = 0;

    
    {
        auto backBtn = ui::Button::create();
        backBtn->loadTextures("EggQuiz/Common/pretest_exit_normal.png", "EggQuiz/Common/pretest_exit_active.png");
        
        auto keyListener = EventListenerKeyboard::create();
        keyListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event *event) {
            if (!_touchEnabled) return;
            
            if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
                this->confirmQuit();
            }
        };
        backBtn->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, backBtn);
        backBtn->addClickEventListener([this](Ref*){
            if (!_touchEnabled) return;
            SoundEffect::menuBackEffect().play();
            
            this->confirmQuit();
        });
        
        backBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        backBtn->setPosition(Vec2(25, winSize.height-25));
        addChild(backBtn);
    }
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
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

void EggQuizScene::setLevel(char category, int level, bool isPreTest)
{
    _quizCategory = category;
    _currentLevel = level;
    _levelData = LevelData::defaultData();
    _isPreTest = isPreTest;
    
    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    
    auto numSet = _levelData.numSets(lang, category, level);
    _problemSet = _levelData.getProblemSet(lang, category, level, random(1, numSet));
    
    _answerResult.clear();
    for (int i=0; i<_problemSet.size(); i++) {
        _answerResult.push_back(false);
    }
    
    if (UserManager::getInstance()->isDebugMode()) {
        
        auto text = StringUtils::format("[%c-%d] as %s of [%c-%d]", category, level, isPreTest ? "PreTest" : "PostTest", category, isPreTest ? level+1 : level);
        auto l = TodoUtil::createLabel(text, 50, Size::ZERO, defaultFont, Color4B::BLACK);
        l->setPosition(winSize.width/2, winSize.height-100);
        addChild(l);
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
    
    
    _progressBar->setMax(_problemSet.size(), true);
    _progressBar->setCurrent(_currentProblemIndex + 1);
    
    showProblem();
    

    
    //    _
    //    float appearTime = 0;
    //    putObjects(appearTime);
}

void EggQuizScene::onSolve()
{
    

    GameSoundManager::getInstance()->stopAllEffects();
    
    setTouchEnabled(false);
    
    if (_currentProblemIndex == _problemSet.size()) return;
    
    _currentProblemIndex++;
    
    string progressStatus = "";
    for (int i=0; i<_problemSet.size(); i++) {
        string c = _answerResult[i] ? "o" : "x";
        if (i>=_currentProblemIndex) c = "-";
        progressStatus += c;
    }
    _progressBar->setStatus(progressStatus);
    
    
    
    if (_currentProblemIndex == _problemSet.size())
    {
        onComplete();
        
        return;
    }
    
    
    _currentProblemObject = _problemSet[_currentProblemIndex];
    

    
                   
    auto previousPageGrid = _pageGrid;
    showProblem();
    
    /*
    previousPageGrid->retain();
    previousPageGrid->removeFromParent();
    _pageNode->addChild(previousPageGrid);
    previousPageGrid->release();
    */
    
    GameSoundManager::getInstance()->playEffectSound(pageTurnEffect);
    
    float delay = 1.5f;
    
    this->runAction(Sequence::create(DelayTime::create(0.1+delay/2), CallFunc::create([this](){
        string progressStatus = "";
        for (int i=0; i<_problemSet.size(); i++) {
            string c = _answerResult[i] ? "o" : "x";;
            if (i==_currentProblemIndex) c = "c";
            if (i>_currentProblemIndex) c = "-";
            
            
            progressStatus += c;
        }
        _progressBar->setStatus(progressStatus);
        this->setTouchEnabled(true);
        
    }), NULL));
    
    auto turnSeq = Sequence::create(DelayTime::create(0.1), PageTurn3D::create(delay, Size(100,100)), FadeOut::create(0.1), CallFunc::create([this, previousPageGrid](){

        
        previousPageGrid->removeFromParent();
    }), nullptr);
    
    previousPageGrid->runAction(turnSeq);
    
    
    
}

void EggQuizScene::onCorrect()
{
    _answerResult[_currentProblemIndex] = true;
    onSolve();
    
}

void EggQuizScene::onIncorrect()
{
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

    
    
    _pageGrid = createPageGrid();
    _pageNode->addChild(_pageGrid, _currentPageGridZ--);
    
    _gameNode = Node::create();
    _gameNode->setContentSize(gameSize);
    float scale = MIN(winSize.width/gameSize.width, winSize.height/gameSize.height);
    _gameNode->setScale(scale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _gameNode->setPosition(Vec2(winSize.width/2, 0));
    
    
    auto paperTop = Sprite::create("EggQuiz/Common/pretest_layout_page.png");
    paperTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    paperTop->setPosition(gameSize.width / 2, 0.f);
    _gameNode->addChild(paperTop);
    
    
    
    _pageGrid->addChild(_gameNode);

    

    if (_quizCategory=='L') {
        _gameNode->addChild(createLiteracyQuestionView());
        _gameNode->addChild(createLiteracyAnswerView());
    } else {
        _gameNode->addChild(createMathView());

        
    }
    
    
}




Node* EggQuizScene::createLiteracyQuestionView()
{


    
    auto P = _currentProblemObject;
      
    auto qt = P.questionType;
    auto at = P.answerType;
    
    

    Node *node = Node::create();
    
    if (qt==SOUNDONLY) {
        auto soundPath = audioFolder + _currentProblemObject.questionAudio;
        auto soundBtn = EggQuizButton::create();
        soundBtn->setSound(soundPath);
        
        node->setContentSize(soundBtn->getContentSize());
        node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        soundBtn->setAnchorPoint(Vec2::ZERO);
        node->addChild(soundBtn);
        

        node->setPosition(gameSize.width/2, 1000);
        
    } else if (qt == WORD) {
        
        auto viewSize = Size(1340, 508);
        
        if (_currentProblemObject.questionAudio.length()>1) {
            viewSize = Size(1890, 508);
            
            auto soundPath = audioFolder + _currentProblemObject.questionAudio;
            auto soundBtn = EggQuizButton::create();
            soundBtn->setSound(soundPath);
            soundBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            soundBtn->setPosition(Vec2(0, viewSize.height/2));
            node->addChild(soundBtn);
        }
        
        node->setContentSize(viewSize);
        node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        node->setPosition(gameSize.width/2, 1060);
        
        auto boxSize = Size(1340, 508);
        auto box = Scale9Sprite::create("EggQuiz/Common/pretest_questionbox.png");
        box->setPreferredSize(boxSize);
        box->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        box->setPosition(viewSize.width, 0);
        node->addChild(box);
        
                                        
        //auto l = TodoUtil::createLabel(_currentProblemObject.questionStrings[0], 80, Size::ZERO, defaultFont, Color4B(70, 70, 70, 255));
        auto l = createSingleLine(_currentProblemObject.questionStrings[0], 80, boxSize);
        l->setPosition(boxSize/2);
        box->addChild(l);
        
    } else if (qt==SENTENCE) {
        
        auto viewSize = Size(1890, 508);
        
        if (_currentProblemObject.questionAudio.length()>1) {
            viewSize = Size(1890, 408);
            
            auto soundPath = audioFolder + _currentProblemObject.questionAudio;
            auto soundBtn = EggQuizButton::create();
            soundBtn->setSound(soundPath);
            soundBtn->setScale(0.5);
            soundBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            soundBtn->setPosition(Vec2(0, viewSize.height+150));
            node->addChild(soundBtn);
        }
        node->setContentSize(viewSize);
        node->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        node->setPosition(gameSize.width/2, 1060 - 254);
        
        auto boxSize = viewSize;
        auto box = Scale9Sprite::create("EggQuiz/Common/pretest_questionbox.png");
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
            auto soundBtn = EggQuizButton::create();
            soundBtn->setSound(soundPath);
            soundBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            soundBtn->setScale(0.5);
            soundBtn->setPosition(Vec2(viewSize.width/2, viewSize.height+150));
            node->addChild(soundBtn);
        }
        
        
        node->setContentSize(viewSize);
        node->setPosition(262, 185);
        
        auto boxSize = viewSize;
        auto box = Scale9Sprite::create("EggQuiz/Common/pretest_questionbox.png");
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


        
        auto box = Scale9Sprite::create("EggQuiz/Common/pretest_questionbox.png");
        node->addChild(box);
        
        if (_currentProblemObject.questionAudio.length()>1) {
            viewSize = Size(1890, 508);
            
            auto soundPath = audioFolder + _currentProblemObject.questionAudio;
            auto soundBtn = EggQuizButton::create();
            soundBtn->setSound(soundPath);
            soundBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            soundBtn->setScale(0.5);
            soundBtn->setPosition(Vec2(viewSize.height/2, viewSize.height/2));
            node->addChild(soundBtn);
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
        
        auto frame = Sprite::create("EggQuiz/Common/pretest_picture_withoutbg.png");
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
            auto soundBtn = EggQuizButton::create();
            soundBtn->setSound(soundPath);
            soundBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            soundBtn->setScale(0.5);
            soundBtn->setPosition(Vec2(0, viewSize.height+150));
            node->addChild(soundBtn);
        }
        
        
        Vec2 imageCenter = viewSize/2;
        
        auto box = Scale9Sprite::create("EggQuiz/Common/pretest_questionbox.png");
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
            auto frame = Sprite::create("EggQuiz/Common/pretest_picture_withoutbg.png");
            frame->setPosition(imageCenter + Vec2(622*(i-1), 0));
            auto frameSize = Size(472, 332);
            box->addChild(frame);
            auto frameCenter = frame->getContentSize()/2;
            
            
            auto filename = _currentProblemObject.questionStrings[i];
            if (filename=="-" || filename=="?") {
                filename = "EggQuiz/Common/pretest_english_filltheblank.png";
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
                auto arrow = Sprite::create("EggQuiz/Common/pretest_english_arrow.png");
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
        
        bool correctAnswer = P.answerStrings[ai]==P.correctAnswerString;
        if (correctAnswer) foundCorrectAnswer = true;
        
        auto cp = _currentProblemIndex;
        
        a->addClickEventListener([buttons, correctAnswer, cp, this](Ref*) {
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
                this->onCorrect();
            } else {
                this->onIncorrect();
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
    

Node* EggQuizScene::createMathView()
{
    Node *node = Node::create();
    node->setContentSize(gameSize);
    
    
    auto problem = _currentProblemIndex+1;

    int answer;
    vector<int> choices;
    
    
    auto makeSeq = [](int answer, int min, int max) {
        
        int s = random(min, max-3);
        int e = s+3;
        if (answer<s) { s = answer; e = s+3; };
        if (e<answer) { e = answer; s = e-3; };
        
        vector<int> ret = {s, s+1, s+2, s+3 };
        return ret;
    };
    
    auto makeRand = [](int answer, int min, int max, bool plusone = false) {
        
        vector<int> v;
        for (int i=min; i<=max; i++) {
            if (i==answer) continue;
            if (plusone && i==answer+1) continue;
            v.push_back(i);
        }
        random_shuffle(v.begin(), v.end());
        
        
        if (plusone) {
            vector<int> ret(v.begin(), v.begin()+2);
            ret.push_back(answer);
            ret.push_back(answer+1);
            random_shuffle(ret.begin(), ret.end());
            return ret;
        } else {
            vector<int> ret(v.begin(), v.begin()+3);
            ret.push_back(answer);
            random_shuffle(ret.begin(), ret.end());
            return ret;
        }
        
    };
    
    auto makeOneDiff = [] (int answer) {
        vector<int> v = {answer+10, answer-10, answer+1, answer-1};
        random_shuffle(v.begin(), v.end());
        
        vector<int> ret(v.begin(), v.begin()+3);
        ret.push_back(answer);
        random_shuffle(ret.begin(), ret.end());
        
        return ret;
    };
    
    auto makeCommonRandom = [] (int answer, bool twoway = false) {
        vector<int> v;
        
        // same tens
        for (int i=0; i<=9; i++) {
            if (answer%10==i) continue;
            v.push_back((answer/10)*10 + i);
        }
        
        // same ones
        if (twoway) {
            auto one = answer%10;
            
            for (int i=0; i<=9; i++) {
                if (answer/10==i) continue;
                auto value = one + i*10;
                if (find(v.begin(), v.end(), value)==v.end() && value>0) {
                    v.push_back(value);
                }
            }
        }
        
        random_shuffle(v.begin(), v.end());
        vector<int> ret(v.begin(), v.begin()+3);
        ret.push_back(answer);
        random_shuffle(ret.begin(), ret.end());
        return ret;
    };
    
    
    
    {
        int step = 1;
        bool soundonly = false;
        bool missing = false;
        bool missingLast = true;
        bool tenframe = false;
        bool expression = false;
        bool expressionSmall = false;
        bool expressionVertical = false;
        bool textQuestion = false;
        
        
        int A, B;
        string op;
        string left, right;
        string text;
        
        
        
        if (_currentLevel==1) {
            switch(problem) {
                case 1: answer = random(1, 4); step = 1; choices = makeSeq(answer, 1, 5); break;
                case 2: answer = random(1, 5); soundonly = true; choices = makeSeq(answer, 1, 9); break;
                case 3: answer = random(1, 5); step = 5; choices = makeSeq(answer, 1, 5); break;
                case 4: answer = random(6, 10); soundonly = true; choices = makeSeq(answer, 1, 9); break;
                case 5: answer = random(4, 7); step = 1; choices = makeSeq(answer, 3, 9); break;
            }
            
        } else if (_currentLevel==2) {
            switch(problem) {
                case 1: answer = random(5, 12); step = 5; choices = makeSeq(answer, 3, 15); break;
                case 2: answer = random(13, 20); step = 5; choices = makeSeq(answer, 10, 20); break;
                case 3: answer = random(4, 9); missing = true; choices = makeRand(answer, 4, 10); break;
                case 4: answer = random(10, 20); soundonly = true; choices = makeSeq(answer, 10, 20); break;
                case 5: A = random(2, 5); B = random(2, 5); answer = A+B; tenframe = true; choices = makeSeq(answer, 1, 10); break;
            }
        } else if (_currentLevel==3) {
            switch(problem) {
                case 1:
                case 2:
                    answer = random(11, 49); step = 10; choices = makeOneDiff(answer); break;
                case 3:
                case 4:
                    A = random(5, 10); B = random(5, 10); answer = A+B; tenframe = true;
                    choices = makeSeq(answer, 10, 20);
                    break;
                case 5:
                case 6:
                    answer = random(11, 20); missing = true; choices = makeRand(answer, 11, 20); break;
                case 7:
                case 8:
                    answer = random(2, 9);
                    A = random(1, answer-1);
                    B = answer-A;
                    op = "+";
                    expression = true;
                    choices = makeSeq(answer, 2, 9);
                    break;
                case 9:
                case 10:
                    A = random(2, 9);
                    B = random(1, A-1);
                    answer = A-B;
                    op = "-";
                    expression = true;
                    choices = makeSeq(answer, 1, 9);
                    break;
                    
            }
            
        }  else if (_currentLevel==4) {
            switch(problem) {
                    
                case 1:
                case 2:
                    switch (random(0, 2)) {
                        case 0: step = 2; answer = random(10, 49)*2; break;
                        case 1: step = 5; answer = random(4, 19)*5; break;
                        case 2: step = 10; answer = random(4, 9)*10; break;
                    }
                    missing = true;
                    choices = makeRand(answer, 10, 99, true);
                    break;
                case 3:
                case 4:
                    missing = true;
                    missingLast = false;
                    answer = random(20, 90);
                    choices = makeCommonRandom(answer);
                    
                    
                    break;
                    
                case 5:
                case 6:
                    
                    expressionSmall = true;
                    
                    answer = random(1, 3);
                    B = random(answer+1, 9);
                    A = B-answer;
                    
                    left = StringUtils::format("%d + ", A);
                    right = StringUtils::format(" = %d", B);
                    
                    break;
                case 7:
                case 8:
                    
                    expressionVertical = true;
                    
                    answer = random(2, 10);
                    A = random(1, answer-1);
                    B = answer - A;
                    op = "+";
                    
                    break;
                case 9:
                case 10:
                    
                    expressionVertical = true;
                    
                    A = random(2, 10);
                    B = random(1, A-1);
                    
                    answer = A-B;
                    op = "-";
                    
                    break;
            }
            
        } else if (_currentLevel==5) {
            switch(problem) {
                    
                case 1:
                    textQuestion = true;
                    text = _currentProblemObject.questionQuestion;
                    answer = random(1, 99);
                    choices = makeRand(answer, 1, 99, false);
                    for (auto c : choices) {
                        if (c>answer) answer = c;
                    }
                    break;
                case 2:
                    textQuestion = true;
                    text = _currentProblemObject.questionQuestion;
                    answer = random(1, 99);
                    choices = makeRand(answer, 1, 99, false);
                    for (auto c : choices) {
                        if (c<answer) answer = c;
                    }
                    break;
                case 3:
                case 4:
                    soundonly = true;
                    answer = random(10, 100);
                    choices = makeCommonRandom(answer, true);
                    break;
                    
                case 5:
                case 6:
                {
                    expressionSmall = true;
                    
                    
                    answer = random(11, 19);
                    
                    auto one = random(1, 9);
                    auto theother = 10-one;
                    auto third = answer-10;
                    
                    vector<int> v = {one, theother, third};
                    random_shuffle(v.begin(), v.end());
                    left = StringUtils::format("%d + %d + %d = ", v[0], v[1], v[2]);
                    right = "";
                }
                    break;
                case 7:
                    
                    expressionVertical = true;
                    
                    answer = random(11, 20);
                    A = random(10, answer-1);
                    B = answer - A;
                    op = "+";
                    
                    break;
                case 8:
                {
                    expressionVertical = true;
                    
                
                    auto firstone = random(0, 8);
                    auto secondone = random(0, 9-firstone);
                    
                    auto firstten = random(1, 8);
                    auto secondten = random(1, 9-firstten);
                    
                    A = firstone+firstten*10;
                    B = secondone+secondten*10;
                    answer = A+B;
                    op = "+";
                }
                    break;
                case 9:
                    expressionVertical = true;
                    
                    A = random(10, 20);
                    B = random(1, 9);
                    answer = A-B;
                    op = "-";
                    break;
                    
                case 10:
                    
                    expressionVertical = true;
                    
                    A = 20;
                    B = random(5, 15);
                    answer = A-B;
                    op = "-";
                    
                    break;
            }
        }
        
        
        _answer = answer;
        
        Node *questionView;
        if (soundonly) {
            std::string fileName = GameSoundManager::getInstance()->getAdultVoiceName(answer);
            std::string prefix = "Common/AdultVoice/" + LanguageManager::getInstance()->getCurrentLanguageCode()+"/";
            
            auto soundPath = prefix + fileName;
            
            auto soundBtn = EggQuizButton::create();
            soundBtn->setSound(soundPath);
            soundBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            
            questionView = soundBtn;
            questionView->setPosition(gameSize.width/2, 1000);
        } else if (missing) {
            questionView = createMathMissingView(answer, step, missingLast);
            questionView->setPosition(gameSize.width/2, 1060);
        } else if (tenframe) {
            questionView = createTenframeView(A, B);
            questionView->setPosition(gameSize.width/2, 1060);
        } else if (expression) {
            questionView = createExpressView(A, B, op);
            questionView->setPosition(gameSize.width/2, 1060);
        } else if (expressionSmall) {
            questionView = createSmallExpressView(left, right);
            questionView->setPosition(gameSize.width/2-questionView->getContentSize().width/2, 814);
        } else if (expressionVertical) {
            questionView = createVerticalExpressView(A, B, op);
            questionView->setPosition(gameSize.width/2-questionView->getContentSize().width/2, 814);
        } else if (textQuestion) {
            questionView = Node::create();
            questionView->setContentSize(Size(1900, 300));
            auto l = TodoUtil::createLabel(text, 75, Size::ZERO, defaultFont, Color4B(70, 70, 70, 255));
            l->setPosition(questionView->getContentSize()/2);
            questionView->addChild(l);
            questionView->setPosition(gameSize.width/2-questionView->getContentSize().width/2, 1115);
        } else {
            questionView = createMathCountingView(answer, step);
            questionView->setPosition(gameSize.width/2, 1060);
        }
        
        node->addChild(questionView);
        
    }
    
    
    auto aType = _currentProblemObject.answerType;
    
    auto answerView = createMathAnswerView(aType, answer, choices);
    if (aType==NUMBERPAD) {
        answerView->setPosition(gameSize.width/2 + 80 + answerView->getContentSize().width/2, 814);
    } else if (aType==CHOOSE_4_NUMBER) {
        answerView->setPosition(gameSize.width/2, 380);
    }
    node->addChild(answerView);
    
    
    return node;
}

Node* EggQuizScene::createMathCountingView(int num, int step)
{
    Node *node = Node::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto viewSize = Size(1890, 760);
    node->setContentSize(viewSize);
                         
    auto bg = Scale9Sprite::create("EggQuiz/Common/pretest_questionbox.png");
    bg->setPreferredSize(viewSize);
    bg->setPosition(viewSize/2);
    node->addChild(bg);
    
    
    if (step==1) {
        vector<Sprite*> stones;
        for (int i=0; i<num; i++) {
            auto stone = Sprite::create("EggQuiz/Common/pretest_math_acorn.png");
            Size margin = stone->getContentSize();
            stone->setRotation(random(-60.0, 60.0));
            
            Vec2 bestPos;
            float bestMinDist = -1;
            
            
            for (int j=0; j<10; j++) {
                Vec2 pos = Vec2(random(margin.width, viewSize.width-margin.width), random(margin.height, viewSize.height-margin.height));
                
                float minDist = -1;
                for (auto s : stones) {
                    auto dist = s->getPosition().distance(pos);
                    if (minDist<0 || dist<minDist) minDist = dist;
                }
                
                if (bestMinDist<0 || bestMinDist < minDist) {
                    bestMinDist = minDist;
                    bestPos = pos;
                }
                
            }
            
            stones.push_back(stone);
            stone->setPosition(bestPos);
            node->addChild(stone);
            
        }
    } else if (step==5) {
        int numSection = (num+4) / 5;
        auto tallySize = Size(486, 453);
        auto space = 120;
        
        auto tView = Node::create();
        tView->setContentSize(viewSize);
        tView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        tView->setPosition(viewSize/2);
        if (numSection>=4) {
            auto scale = viewSize.width / (tallySize.width*numSection + space*(numSection+1));
            tView->setScale(scale);
        }
        node->addChild(tView);

        auto occ = space*(numSection-1)+tallySize.width*numSection;
        auto left = (viewSize.width-occ)/2;

        for (int i=0; i<numSection; i++) {
            string tallyName = "pretest_math_tally_5.png";
            if (i==numSection-1) {
                tallyName = "pretest_math_tally_"+TodoUtil::itos(num-step*i)+".png";
            }
            auto tally = Sprite::create("EggQuiz/Common/"+tallyName);
            
            tally->setPosition(left + i*(space+tallySize.width)+tallySize.width/2, viewSize.height/2);
            
            tView->addChild(tally);
            
        }

    } else if (step==10) {
        int numSection = (num+9) / 10;
        auto sectionSize = Size(1518, 132);
        auto space = 10;
        
        auto tView = Node::create();
        tView->setContentSize(viewSize);
        tView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        tView->setPosition(viewSize/2);
        float sectionHeight = sectionSize.height*numSection + space*(numSection+1);
        if (sectionHeight>viewSize.height) {
            auto scale = viewSize.width / sectionHeight;
            tView->setScale(scale);
        }
        node->addChild(tView);
        
        auto occ = space*(numSection-1)+sectionSize.height*numSection;
        auto top = viewSize.height - (viewSize.height-occ)/2;
        auto left = sectionSize.width - ( 112*10 + 34*9 );
        
        
        for (int i=0; i<numSection; i++) {
            int numInRow = 10;
            if (i==numSection-1) numInRow = num % 10;
            
            auto section = Sprite::create("EggQuiz/Common/pretest_math_10set_outline.png");
            section->setPosition(viewSize.width/2, top - i*(sectionSize.height+space) - sectionSize.height/2);
            tView->addChild(section);
            
            for (int j=0; j<numInRow; j++) {
                auto dot = Sprite::create("EggQuiz/Common/pretest_math_dot_yellow.png");
                dot->setPosition(left + (112+34)*j, sectionSize.height/2);
                section->addChild(dot);
            }

        }
    }
    
    
    return node;
    
}

Node* EggQuizScene::createMathMissingView(int num, int step, bool forLast)
{
    Node *node = Node::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    const Size btnSize = Size(434, 566);
    const float space = 64;
    Size viewSize = Size(btnSize.width*4+space*3, btnSize.height);
    
    node->setContentSize(viewSize);
    
    
    int loc = 3;
    if (!forLast) {
        loc = random(0, 2);
    }
    

    for (int i=0; i<4; i++) {
        auto btn = Node::create();
        btn->setContentSize(btnSize);
        btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        btn->setPosition(Vec2((btnSize.width+space)*i+btnSize.width/2, viewSize.height/2));
        node->addChild(btn);
        
        auto sp = Scale9Sprite::create("EggQuiz/Common/pretest_questionbox.png");
        sp->setPreferredSize(btnSize);
        sp->setPosition(btnSize/2);
        btn->addChild(sp);
    
        
        if (i==loc) {
            auto q = Sprite::create("EggQuiz/Common/pretest_math_emptyanswer_dotted_large.png");
            q->setPosition(btnSize/2);
            btn->addChild(q);
        } else {
            
            int diff = i - loc;
            
            auto v = TodoUtil::itos(num + diff*step);
            
            auto l = TodoUtil::createLabel(v, 150, Size::ZERO, defaultFont, Color4B(70, 70, 70, 255));
            l->setPosition(btnSize/2);
            btn->addChild(l);
        }
    }

    
    return node;
}

Node* EggQuizScene::createTenframeView(int a, int b)
{
    Node *node = Node::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    auto viewSize = Size(1890, 760);
    node->setContentSize(viewSize);
    
    
    auto createFrame = [](int num){
        auto frame = Sprite::create("EggQuiz/Common/pretest_math_tenframe.png");
        auto frameSize = frame->getContentSize();
        float step = 156;
        
        for (int i=0; i<num; i++) {
            auto dot = Sprite::create("EggQuiz/Common/pretest_math_dot_yellow.png");
            int x = i % 5;
            int y = 1-(i / 5);
            Vec2 pos = Vec2(frameSize.width/2-step*2 + step*x+2, frameSize.height/2-step/2 + step*y);
            dot->setPosition(pos);
            frame->addChild(dot);
        }
        
        return frame;
    };
    
    auto f1 = createFrame(a);
    f1->setPosition(Vec2(viewSize.width*0.25, viewSize.height/2));
    node->addChild(f1);
    
    auto f2 = createFrame(b);
    f2->setPosition(Vec2(viewSize.width*0.75, viewSize.height/2));
    node->addChild(f2);
    
    
    return node;
    
}


Node* EggQuizScene::createExpressView(int a, int b, string op)
{
    Node *node = Node::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    auto viewSize = Size(1890, 760);
    node->setContentSize(viewSize);
    
    
    auto addLabel = [](Node *v, string s) {
        
        const auto space = 80;
        
        Node *l;
        if (s=="?") {
            l = Sprite::create("EggQuiz/Common/pretest_math_emptyanswer_dotted_large.png");
        } else {
            l = TodoUtil::createLabel(s, 150, Size::ZERO, defaultFont, Color4B(70, 70, 70, 255));
        }
        
        Size lSize = l->getContentSize();
        auto vSize = v->getContentSize();
        
        
        l->setPosition(vSize.width + space + lSize.width/2, vSize.height/2);
        v->addChild(l);
        
        v->setContentSize(Size(vSize.width+space+lSize.width, vSize.height));
        
    };
    
    auto exp = Node::create();
    
    exp->setContentSize(Size(0, 300));
    
    addLabel(exp, TodoUtil::itos(a));
    addLabel(exp, op);
    addLabel(exp, TodoUtil::itos(b));
    addLabel(exp, "=");
    addLabel(exp, "?");
    
    exp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    exp->setPosition(Vec2(viewSize.width/2, 260));
    node->addChild(exp);
    
    auto addBall = [](Node *v, string ballPath) {
        
        const auto space = 80;
        
        Node *l = Sprite::create(ballPath);
        auto vSize = v->getContentSize();
        
        Size lSize(112, 112);
        
        if (l) {
            lSize = l->getContentSize();
            l->setPosition(vSize.width + space + lSize.width/2, vSize.height/2);
            v->addChild(l);
        }
        
        v->setContentSize(Size(vSize.width+space+lSize.width, vSize.height));
        
    };
    
    auto ball = Node::create();
    ball->setContentSize(Size(0, 112));
    
    if (op=="+") {
        for (int i=0; i<a; i++) addBall(ball, "EggQuiz/Common/pretest_math_dot_yellow.png");
        addBall(ball, "");
        for (int i=0; i<b; i++) addBall(ball, "EggQuiz/Common/pretest_math_dot_blue.png");
        
    } else {
        for (int i=0; i<a-b; i++) addBall(ball, "EggQuiz/Common/pretest_math_dot_yellow.png");
        for (int i=0; i<b; i++) addBall(ball, "EggQuiz/Common/pretest_math_dot_yellow_empty.png");
    }
    
    ball->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    ball->setPosition(Vec2(viewSize.width/2, 540));
    node->addChild(ball);
    
    return node;
}

Node* EggQuizScene::createSmallExpressView(string left, string right)
{
    Node *node = Node::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto viewSize = Size(1040, 455);
    node->setContentSize(viewSize);
    
    auto bg = Scale9Sprite::create("EggQuiz/Common/pretest_questionbox.png");
    bg->setPreferredSize(viewSize);
    bg->setPosition(viewSize/2);
    node->addChild(bg);
    
    
    auto addLabel = [](Node *v, string s) {
        
        if (s=="") return;
        
        const auto space = 40;
        
        Node *l;
        if (s=="?") {
            l = Sprite::create("EggQuiz/Common/pretest_math_emptyanswer_dotted_small.png");
        } else {
            l = TodoUtil::createLabel(s, 120, Size::ZERO, defaultFont, Color4B(70, 70, 70, 255));
        }
        
        Size lSize = l->getContentSize();
        auto vSize = v->getContentSize();
        
        
        l->setPosition(vSize.width + space + lSize.width/2, vSize.height/2);
        v->addChild(l);
        
        v->setContentSize(Size(vSize.width+space+lSize.width, vSize.height));
        
    };
    
    auto exp = Node::create();
    
    exp->setContentSize(Size(0, 200));
    
    addLabel(exp, left);
    addLabel(exp, "?");
    addLabel(exp, right);
    
    exp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    exp->setPosition(Vec2(viewSize.width/2, viewSize.height/2));
    node->addChild(exp);

    
    return node;
    
}

Node* EggQuizScene::createVerticalExpressView(int a, int b, string op)
{
    Node *node = Node::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto viewSize = Size(1040, 1225);
    node->setContentSize(viewSize);
    
    auto bg = Scale9Sprite::create("EggQuiz/Common/pretest_questionbox.png");
    bg->setPreferredSize(viewSize);
    bg->setPosition(viewSize/2);
    node->addChild(bg);
    
    const auto fontSize = 190;
    
    auto la = TodoUtil::createLabel(TodoUtil::itos(a), fontSize, Size(370, 0), defaultFont, Color4B(70, 70, 70, 255), TextHAlignment::RIGHT);
    auto lb = TodoUtil::createLabel(TodoUtil::itos(b), fontSize, Size(370, 0), defaultFont, Color4B(70, 70, 70, 255), TextHAlignment::RIGHT);
    Node *lop;
    if (op=="-") {
        lop = Sprite::create("EggQuiz/Common/pretest_math_symbol_minus.png");
    } else {
        lop = TodoUtil::createLabel(op, fontSize, Size(0, 0), defaultFont, Color4B(70, 70, 70, 255), TextHAlignment::RIGHT);
    }
    
    auto lwidth = lb->getContentSize().width + lop->getContentSize().width;
    
    auto strokeWidth = MAX(630, lwidth+40);
    auto stroke = LayerColor::create(Color4B(70, 70, 70, 255), strokeWidth, 12);
    auto strokePos = Vec2(viewSize.width/2-strokeWidth/2, 550);
    
    stroke->setPosition(strokePos);
    node->addChild(stroke);
    
    
    lop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lop->setPosition(strokePos + Vec2(20, 50+lb->getContentSize().height/2));
    node->addChild(lop);
    
    lb->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    lb->setPosition(Vec2(strokePos.x + strokeWidth - 20, strokePos.y + 50));
    node->addChild(lb);
    
    la->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    la->setPosition(Vec2(strokePos.x + strokeWidth - 20, strokePos.y + 310));
    node->addChild(la);
    
    auto sp = Sprite::create("EggQuiz/Common/pretest_math_emptyanswer_dotted_large.png");
    sp->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    sp->setPosition(Vec2(strokePos.x + strokeWidth - 10, strokePos.y - 80));
    node->addChild(sp);
    
    auto le = TodoUtil::createLabel("=", fontSize, Size(0, 0), defaultFont, Color4B(70, 70, 70, 255), TextHAlignment::RIGHT);
    le->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    
    le->setPosition(Vec2(lop->getPosition().x, sp->getPosition().y - sp->getContentSize().height/2));
    node->addChild(le);
                    
    return node;
    
}

Node* EggQuizScene::createMathAnswerView(AnswerType type, int answer, vector<int>& choices)
{
    Node *node = Node::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    if (type==NUMBERPAD) {

        
        auto answerPad = Node::create();
        node->addChild(answerPad);
        
        
        
        
        auto answerPadBg = Sprite::create("EggQuiz/Common/pretest_math_calculator_bg.png");
        auto answerPadSize = answerPadBg->getContentSize();
        answerPad->setContentSize(answerPadSize);
        node->setContentSize(answerPadSize);
        
        answerPadBg->setPosition(answerPadSize/2);
        answerPad->addChild(answerPadBg);
        
        
        auto panelSize = Size(240, 178);
        
        const int rowCount = 4;
        const int colCount = 3;
        
        auto panelAreaSize = Size(panelSize.width*colCount + 10.f*(colCount-1),
                                  panelSize.height*rowCount + 10.f*(rowCount-1));
        float panelAreaMargin = (answerPadSize.width-panelAreaSize.width)/2.f;
        float answerAreaHeight = answerPadSize.height - 2*panelAreaMargin - panelAreaSize.height;
        auto answerLabelPos = Point(answerPadSize.width/2, 1040);
        
        
        
        
        const auto normalColor = Color4B(70, 70, 70, 255);
        const auto highlightColor = Color4B::WHITE;
        
        _answerString = "";
        
        Label* answerLabel = TodoUtil::createLabel(_answerString, 220, Size(750, 180), defaultFont, Color4B::WHITE, TextHAlignment::RIGHT);
        answerLabel->setPosition(answerLabelPos);
        answerPad->addChild(answerLabel);
        
        
        
        auto panelPos = [panelSize, rowCount, colCount, panelAreaMargin](int row, int col)
        {
            float x = col * (panelSize.width + 10.f) + panelSize.width/2 + panelAreaMargin;
            float y = (10.f + panelSize.height) * (rowCount - row-1) + panelSize.height/2 + panelAreaMargin;
            
            return Point(x, y);
        };
        
        for (int i = -2; i <= 9; i++) {
            Point btnPos;
            int fontSize = 120;
            
            ui::Button *btn;
            Label *btnLabel;
            
            
            if (i >= 0) {
                btn = ui::Button::create("EggQuiz/Common/pretest_math_calculator_button_normal.png",
                                         "EggQuiz/Common/pretest_math_calculator_button_active.png");
    
                btnLabel = TodoUtil::createLabel(TodoUtil::itos(i), fontSize, Size::ZERO, defaultFont, normalColor);
                if (i==0) {
                    btnPos = panelPos(rowCount - 1, 1);
                } else {
                    btnPos = panelPos((9-i)/colCount, colCount - (9-i)%colCount - 1);
                }
            } else if (i==-1) {
                btn = ui::Button::create("EggQuiz/Common/pretest_math_calculator_button_normal.png",
                                         "EggQuiz/Common/pretest_math_calculator_button_active.png");
                
                btnLabel = Label::create();
                
                auto backsp = Sprite::create("EggQuiz/Common/pretest_math_calculator_back.png");
                backsp->setPosition(panelSize/2 + Size(0, 8));
                btn->addChild(backsp);
                
                
                
                
                btnPos = panelPos(rowCount - 1, 0);
            } else if (i==-2) {
                btn = ui::Button::create("EggQuiz/Common/pretest_math_calculator_button_enter_normal.png",
                                         "EggQuiz/Common/pretest_math_calculator_button_enter_active.png");
                btnLabel = Label::create();
                
                btnPos = panelPos(rowCount - 1, 2);
            }
            
            btnLabel->setPosition(btn->getContentSize()/2 + Size(0, 8));
            btn->addChild(btnLabel);
            
            
            btn->setZoomScale(0);
            btn->setPosition(btnPos);
            btn->setTag(i);
            btn->addTouchEventListener([btn, btnLabel, normalColor, highlightColor, this](Ref* sender,Widget::TouchEventType event) {
                if (!_touchEnabled) return;
                btnLabel->setTextColor(btn->isHighlighted() ? highlightColor : normalColor);
                
            });
            
            btn->addClickEventListener([btn, this, answerLabel](Ref* sender) {
                if (!_touchEnabled) return;
                
                GameSoundManager::getInstance()->playEffectSound(touchEffect);
                auto ans = btn->getTag();
                if (ans==-2) {
                    //enter
                    if (_answerString.length()==0) return;
                    
                    if (TodoUtil::stoi(_answerString) == _answer) {
                        this->onCorrect();
                    } else {
                        this->onIncorrect();
                    }
                    
                    
                } else if (ans==-1) {
                    //back
                    auto l = _answerString.length();
                    _answerString = _answerString.substr(0, l-1);
                    answerLabel->setString(_answerString);
                    
                } else {
                    if (_answerString.size() <= 4) {
                        _answerString += TodoUtil::itos(ans);
                        answerLabel->setString(_answerString);
                    }
                }
            });
            
            answerPad->addChild(btn);
        }
        
        
    } else if (_currentProblemObject.answerType==CHOOSE_4_NUMBER) {
        
        
        
        const Size btnSize = Size(438, 458);
        const float space = 60;
        Size viewSize = Size(btnSize.width*4+space*3, btnSize.height);
        node->setContentSize(viewSize);
        
        
        
        for (int i=0; i<choices.size(); i++) {
            
            auto a = EggQuizButton::create();
            a->setNumber(choices[i]);
            a->setPosition(Vec2((btnSize.width+space)*i+btnSize.width/2, viewSize.height/2));
            node->addChild(a);
            
            bool correctAnswer = (choices[i]==answer);
            auto cp = _currentProblemIndex;
            
            a->addClickEventListener([correctAnswer, cp, this](Ref*) {
                if (!isTouchEnabled()) return;
                if (cp!=_currentProblemIndex) return;
                
                if (correctAnswer) {
                    this->onCorrect();
                } else {
                    this->onIncorrect();
                }
            });
            
            
           
            
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
                underlineLength = word.length();
                word = "";
                
            } else {
                underlineLength = last;
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

void EggQuizScene::confirmQuit()
{
    TodoSchoolBackButton::popGameScene();
    
    return;
    
    auto popup = EggQuizPopup::create(this, winSize);
    popup->setConfirmQuit(_quizCategory, _currentLevel);

    popup->show(this, true);

}

void EggQuizScene::passTest()
{
    auto popup = EggQuizPopup::create(this, winSize);
    
    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/pretest_success.m4a");
    
    if (_isPreTest) {
        popup->setPreCompleted(_quizCategory, _currentLevel);
    } else {
        popup->setPostCompleted(_quizCategory, _currentLevel);
    }
    
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
    
    if (_isPreTest) {
        popup->setPreFailed(_quizCategory, _currentLevel);
    } else {
        popup->setPostFailed(_quizCategory, _currentLevel);
    }
    
    
    
    popup->show(this, true, CallFunc::create([this](){
        
        CCAppController::sharedAppController()->handleGameQuit();
        if (onCompleteCallback) {
            onCompleteCallback(false);
        }
    }));
}
