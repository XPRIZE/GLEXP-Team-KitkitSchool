//
//  DoubleDigitScene.cpp
//  todomath
//
//  Created by Sunmi Seol on 12/14/15.
//
//

#include "DoubleDigitScene.h"
#include "Utils/TodoUtil.h"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include <string>
#include "ui/UIWidget.h"
#include "Common/Controls/CompletePopup.hpp"
#include "CCAppController.hpp"

using namespace std;
USING_NS_CC;
using namespace cocos2d::ui;



namespace DoubleDigitSceneSpace {
    const char* solveEffect = "Common/Sounds/Effect/UI_Star_Collected.m4a";
    const char* missEffect = "Common/Sounds/Effect/Help.m4a";
    const Size defaultGameSize = Size(2048, 1440);
    const char* touchEffect = "Common/Sounds/Effect/paneltouch.m4a";
    const char* defaultFont = "fonts/TodoSchoolV2.ttf";

}

using namespace DoubleDigitSceneSpace;



DoubleDigitScene::DoubleDigitScene()
{
}

DoubleDigitScene::~DoubleDigitScene()
{
}

void DoubleDigitScene::prepareNewGameWithLevel(Json::Value problemParameter)
{
    
    currentLevel = problemParameter["level"].asInt();
    problems = problemParameter["problems"];
    
    
    numSolved = 0;
    
    _progressBar->setMax((int)problems.size());
}

bool DoubleDigitScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
    // set the position of background

    
    auto dirSize = Director::getInstance()->getWinSize();
    
    this->setContentSize(dirSize);
    

    
    auto bg = Sprite::create("DoubleDigit/multidigit_background.png");

    Size SpriteSize = bg->getContentSize();
    float bgScale = max(dirSize.width / SpriteSize.width,
                      dirSize.height / SpriteSize.height);
    
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(dirSize/2.f);
    
    bg->setScale(bgScale);
    addChild(bg);
    
    auto gameSize = Size(2560, 1800);
    gameScale = MIN(dirSize.width/gameSize.width, dirSize.height/gameSize.height);

    


    initAnswerPad();
    initNotePad();
    

    
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, dirSize.height-25));
    addChild(backButton);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(dirSize.width/2, dirSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    return true;
}



void DoubleDigitScene::initNotePad()
{
    auto winSize = getContentSize();
    auto noteSize = Size(1228, 1612);
 
    notePad = Node::create();
    notePad->setContentSize(noteSize);
    notePad->setScale(gameScale);
    notePad->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    notePad->setPosition(winSize.width*0.75, winSize.height*0.5);
    addChild(notePad);
    

    
    notePadBG = Sprite::create("DoubleDigit/multidigit_notepad.png");
    notePadBG->setPosition(noteSize/2);
    notePad->addChild(notePadBG);
    
    notePadPage = Node::create();
    notePadPage->setContentSize(noteSize);
    notePad->addChild(notePadPage);
    
    notePadBinding = Sprite::create("DoubleDigit/multidigit_binding.png");
    notePadBinding->setPosition(noteSize/2);
    notePad->addChild(notePadBinding);
    
    
    // add buttons and touch event to each
    auto clearButton = cocos2d::ui::Button::create("DoubleDigit/multidigit_trashcan_normal.png", "DoubleDigit/multidigit_trashcan_touch.png");
    clearButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    clearButton->setPosition(Vec2(noteSize.width-96, noteSize.height-32));
    notePadBinding->addChild(clearButton);
    clearButton->addTouchEventListener([this](Ref* sender,Widget::TouchEventType event) {
        if (event==Widget::TouchEventType::ENDED) {
            notePageJotPlane->clear();
        }
    });

    
    createNotePage();
    
}

void DoubleDigitScene::createNotePage()
{
    
    auto noteSize = notePad->getContentSize();
    
    notePageGrid = NodeGrid::create();
    notePageGrid->setContentSize(noteSize);
    notePadPage->addChild(notePageGrid);
    

    
    notePageProblemView = Node::create();
    notePageProblemView->setContentSize(noteSize);
    notePageGrid->addChild(notePageProblemView);
    
    auto notePageSprite = Sprite::create("DoubleDigit/multidigit_toppage.png");
    notePageSprite->setPosition(noteSize/2);
    notePageProblemView->addChild(notePageSprite);
    
    // add jot plane
    notePageJotPlane = JotPlane::create();
    notePageGrid->addChild(notePageJotPlane);
    
    

}

void DoubleDigitScene::initAnswerPad()
{
    auto winSize = getContentSize();
    
    answerPad = Node::create();
    answerPad->setScale(gameScale);
    answerPad->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    answerPad->setPosition(winSize.width*0.25, winSize.height*0.5);
    addChild(answerPad);
    
    
    

    auto answerPadBg = Sprite::create("Counting/counting_image_numberpad_bg.png");
    auto answerPadSize = answerPadBg->getContentSize();
    answerPad->setContentSize(answerPadSize);
    answerPadBg->setPosition(answerPadSize/2);
    
    answerPad->addChild(answerPadBg);
    
    
    //auto panelSize = Size(306, 184);
    auto panelSize = Size(286, 174);

    const int rowCount = 4;
    const int colCount = 3;
    
    auto panelAreaSize = Size(panelSize.width*colCount + 10.f*(colCount-1),
                              panelSize.height*rowCount + 10.f*(rowCount-1));
    float panelAreaMargin = (answerPadSize.width-panelAreaSize.width)/2.f;
    float answerAreaHeight = answerPadSize.height - 2*panelAreaMargin - panelAreaSize.height;
    answerLabelPos = Point(answerPadSize.width/2, answerPadSize.height - answerAreaHeight/1.25);
    

    

    const auto normalColor = Color3B(110, 85, 67);
    const auto highlightColor = Color3B(110, 85, 67);
    
    answerString = "";

    answerLabel = nullptr;
    
    
    
    auto panelPos = [panelSize, rowCount, colCount, panelAreaMargin](int row, int col)
    {
        float x = col * (panelSize.width + 10.f) + panelSize.width/2 + panelAreaMargin;
        float y = (10.f + panelSize.height) * (rowCount - row-1) + panelSize.height/2 + panelAreaMargin;
        
        return Point(x, y);
    };
    
    for (int i = -2; i <= 9; i++) {
        
        auto btn = ui::Button::create("DoubleDigit/button_inactive.png", "DoubleDigit/button_active.png");
        btn->setZoomScale(0);
 
        Point btnPos = panelPos((9-i)/colCount, colCount - (9-i)%colCount - 1);
 
        
        if (i >= 0) {
            
            if (i==0) btnPos = panelPos(rowCount - 1, 1);
        
            btn->setTitleFontSize(160);
            btn->setTitleColor(normalColor);
            btn->setTitleText(TodoUtil::itos(i));
            
        } else if (i==-1) { // clear
            btnPos = panelPos(rowCount - 1, 0);
            auto sp = Sprite::create("DoubleDigit/multidigit_icon_clear.png");
            sp->setPosition(btn->getContentSize()/2);
            btn->addChild(sp);

        } else if (i==-2) { // enter
            btnPos = panelPos(rowCount - 1, 2);
            auto sp = Sprite::create("DoubleDigit/multidigit_icon_enter.png");
            sp->setPosition(btn->getContentSize()/2);
            btn->addChild(sp);

        }
        

        
 
        btn->setPosition(btnPos);
        btn->setTag(i);
        btn->addTouchEventListener([btn, normalColor, highlightColor, this](Ref* sender,Widget::TouchEventType event) {
            
            if (!_touchEnabled) return;
            
            switch (event) {
                case Widget::TouchEventType::BEGAN:
                    btn->setTitleColor(highlightColor);
                    break;
                case Widget::TouchEventType::CANCELED:
                case Widget::TouchEventType::ENDED:
                    btn->setTitleColor(normalColor);
                    break;
                case Widget::TouchEventType::MOVED:
                    
                    btn->setTitleColor(btn->isHighlighted() ? highlightColor : normalColor);
                    break;
                default:
                    break;
            }
        });
        
        btn->addClickEventListener([btn, this](Ref* sender) {
            if (!_touchEnabled) return;
            
            GameSoundManager::getInstance()->playEffectSound(touchEffect);
            auto ans = btn->getTag();
            if (ans==-2) {
                //enter
                handleAnswerEntered(answerString);
            } else if (ans==-1) {
                //clear
                //answerString = "";
                if (answerString.length()>0) {
                    answerString = answerString.substr(0, answerString.length()-1);
                }
                refreshAnswerLabel();
                
            } else {
                if (answerString.size() <= 4) {
                    answerString += TodoUtil::itos(ans);
                    refreshAnswerLabel();
                }
            }
        });
        
        //if (i == 0) { _zeroButton = btn; }
        answerPad->addChild(btn);
    }
    

}

void DoubleDigitScene::refreshAnswerLabel()
{
    if (answerLabel) answerLabel->removeFromParent();

    answerLabel = TodoUtil::createLabel(answerString, 300, Size(1000, 400), "fonts/Aileron-Regular.otf", Color4B(242, 245, 240, 255), TextHAlignment::CENTER, TextVAlignment::CENTER);
    answerLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    answerLabel->setPosition(answerLabelPos);
    answerPad->addChild(answerLabel);
}



void DoubleDigitScene::makeFakeView()
{
    
    
    notePageGridFading = notePageGrid;
    
    createNotePage();
    

    
    showProblem(notePageProblemView, numSolved);
    
    notePageGridFading->getParent()->reorderChild(notePageGridFading, notePageGridFading->getLocalZOrder());
    
//    notePageGridFading->retain();
//    notePageGridFading->removeFromParent();
//    notePadPage->addChild(notePageGridFading);
//    notePageGridFading->release();
    
    
    

}

void DoubleDigitScene::startNewStage()
{


    _progressBar->setCurrent(numSolved+1);
    //GameController::getInstance()->onReady();
    showProblem(notePageProblemView, numSolved);
}

void DoubleDigitScene::showProblem(Node *view, int indexProblem)
{
    
    setTouchEnabled(true);
    


    const float fontSize = 186;
    //const Size labelSize = Size(400, 200);
    const Color4B labelColor = Color4B(45, 110, 166, 255);
    const Size charSize = Size(fontSize*0.7, fontSize);
    


    
    float viewWidth = view->getContentSize().width;
    float viewHeight = view->getContentSize().height;
    

    float rightX = viewWidth/2;
    string lhs = problems[indexProblem]["lhs"].asString();
    string rhs = problems[indexProblem]["rhs"].asString();
    if (lhs.length()>=4 || rhs.length()>=4) rightX+=fontSize;
    
    
    float minX;
    {
        float x = rightX;
        float y = viewHeight - 400;
        auto s = lhs;
        for (int i=s.length()-1; i>=0; i--) {
            auto l = TodoUtil::createLabel(string(1, s[i]), fontSize, charSize, defaultFont, labelColor, cocos2d::TextHAlignment::CENTER);
            l->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            l->setPosition(x, y);
            x-=charSize.width;
            view->addChild(l);
        }
        
        minX = x;
    }
    
    {
        float x = rightX;
        float y = viewHeight - 600;
        auto s = rhs;
        for (int i=s.length()-1; i>=0; i--) {
            auto l = TodoUtil::createLabel(string(1, s[i]), fontSize, charSize, defaultFont, labelColor, cocos2d::TextHAlignment::CENTER);
            l->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            l->setPosition(x, y);
            x-=charSize.width;
            view->addChild(l);
        }
        
        minX = MIN(minX, x);
        
        
    }
    
    auto opPosX = minX ;
    
    auto opLabel = TodoUtil::createLabel(problems[indexProblem]["op"].asString(), fontSize, charSize,defaultFont, labelColor, cocos2d::TextHAlignment::CENTER);
    opLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    opLabel->setPosition(opPosX, viewHeight - 600  );
    view->addChild(opLabel);
    
    
    auto lineWidth = (rightX - opPosX) + opLabel->getContentSize().width + 40;
    auto lineX = opPosX-opLabel->getContentSize().width-20;
    auto underLine = LayerColor::create(labelColor, lineWidth, 20.f);
    underLine->setPosition(lineX,  viewHeight - 700 );
    view->addChild(underLine);
    

}


void DoubleDigitScene::showAnswer(Node *view, int indexProblem)
{
    

    
    
    
    const float fontSize = 186;
    //const Size labelSize = Size(400, 200);
    const Color4B labelColor = Color4B(45, 110, 166, 255);
    const Size charSize = Size(fontSize*0.7, fontSize);
    
    
    float viewWidth = view->getContentSize().width;
    float viewHeight = view->getContentSize().height;
    
    float rightX = viewWidth/2;
    string lhs = problems[indexProblem]["lhs"].asString();
    string rhs = problems[indexProblem]["rhs"].asString();
    if (lhs.length()>=4 || rhs.length()>=4) rightX+=fontSize;
    
    {
        float x = rightX;
        float y = viewHeight - 840;
        auto s = problems[indexProblem]["answer"].asString();
        for (int i=s.length()-1; i>=0; i--) {
            auto l = TodoUtil::createLabel(string(1, s[i]), fontSize, charSize, defaultFont, labelColor, cocos2d::TextHAlignment::CENTER);
            l->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            l->setPosition(x, y);
            x-=charSize.width;
            view->addChild(l);
        }
        
   
    }
    
    
}


void DoubleDigitScene::handleAnswerEntered(std::string &answer)
{
    // NB(xenosoz, 2018): Log for future analysis
    auto workPath = [this] {
        stringstream ss;
        ss << "/" << "DoubleDigit";
        ss << "/" << "level-" << currentLevel << "-0";
        ss << "/" << "work-" << numSolved;
        return ss.str();
    }();
    
    StrictLogManager::shared()->game_Peek_Answer("DoubleDigit", workPath,
                                                 answer, problems[numSolved]["answer"].asString());

    
    if (problems[numSolved]["answer"].asString().compare(answer) == 0) {
        GameSoundManager::getInstance()->playEffectSound(solveEffect);
        this->scheduleOnce(schedule_selector(DoubleDigitScene::onSolved),0.2);
        
    } else {
        GameSoundManager::getInstance()->playEffectSound(missEffect);
        answerString = "";
        refreshAnswerLabel();
    }
    
}

void DoubleDigitScene::onSolved(float)
{
    setTouchEnabled(false);
    notePageJotPlane->setTouchEnabled(false);
    _progressBar->setCurrent(numSolved+1, true);
    
    showAnswer(notePageProblemView, numSolved);

    numSolved++;
    //GameController::getInstance()->onSolve();
    this->scheduleOnce(schedule_selector(DoubleDigitScene::showComplete),0.8);
}

void DoubleDigitScene::showComplete(float)
{
    
    
    if (numSolved == problems.size()) {
        
        CompletePopup::create()->show(1.0, [](){
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
        
    } else {
        turningPageAnimation();
    }
    
}

void DoubleDigitScene::turningPageAnimation()
{
    // add sound
    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/Card_Move_Right.m4a");
    
    float delay = 1.5f;
    
    makeFakeView(); // add the fake view
    
    answerString = "";
    refreshAnswerLabel();
    
    
    
    this->runAction(Sequence::create(DelayTime::create(0.1+delay/2), CallFunc::create([this](){
        _progressBar->setCurrent(numSolved+1);
    }), NULL));
    
    auto turnSeq = Sequence::create(DelayTime::create(0.1), PageTurn3D::create(delay, Size(100,100)), FadeOut::create(0.1), CallFunc::create([this](){
        this->gotoNextPage();
    }), nullptr);
    
    notePageGridFading->runAction(turnSeq);
    

}


void DoubleDigitScene::gotoNextPage ()
{

    notePageGridFading->removeFromParent();
    notePageGridFading = nullptr;
    
    setTouchEnabled(true);

    
    
}



void DoubleDigitScene::handleStart() {
    
    
    //GameController::getInstance()->onBeginWithTotalProblems(problems.size());
    startNewStage();
    
}

void DoubleDigitScene::handlePause() {
}

void DoubleDigitScene::handleResume() {
}

void DoubleDigitScene::handleQuit() {
    
    
    this->stopAllActions();
    this->unscheduleAllCallbacks();
    
}



void DoubleDigitScene::onExit(){
    Layer::onExit();
    
    GameSoundManager::getInstance()->unloadAllEffect();
    
    this->removeAllChildren();
    this->unscheduleAllCallbacks();
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    
//    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}
