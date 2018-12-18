//
//  LRComprehensionScene.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 07/08/2018.
//

#include "LRComprehensionScene.hpp"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "LRLevelData.hpp"
#include "LRAllInOneTypeQuestion/LRAllInOneTypeQuestionScene.hpp"
#include "LRDividedTypeQuestion/LRDividedTypeQuestionScene.hpp"
#include "LRDividedTypeReordering/LRDividedTypeReorderingScene.hpp"
#include "LRDividedTypeFillTheBlanks/LRDividedTypeFillTheBlanksScene.hpp"
#include "Utils/TodoUtil.h"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Controls/PopupBase.hpp"
#include "CCAppController.hpp"
#include "Common/Basic/ScopeGuard.h"
#include "Common/Basic/NodeScopeGuard.h"
#include "Managers/GameSoundManager.h"
#include "Managers/UserManager.hpp"
#include "LRTitlePageView.hpp"

#define LR_GUIDEVIEW_SIZE   Size(2272.f, 1308.f);

namespace
{
    const string kPrefixPath = "lrcomprehension/common/";
    const string solveEffect = kPrefixPath + "ui_star_collected.m4a";
    const string missEffect = kPrefixPath + "help.m4a";
    const string pageTurnEffect = kPrefixPath + "card_move_right.m4a";
    const string defaultFont = FONT_ANDIKA_BOLD;
    const float kGuideViewTweenDuration = 0.3f;
    const float kGuideViewDelayGameClear = 0.3f;
    const Size gameSize = Size(2560, 1800);
    const Size kLRGuideViewSize = Size(2272.f, 1308.f);
}

LRComprehensionScene::LRComprehensionScene()
:_guideHighlight(nullptr)
{
    
}

Scene* LRComprehensionScene::createScene(int levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LRComprehensionScene::create();
    layer->setLevel(levelID);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool LRComprehensionScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _winSize = Director::getInstance()->getWinSize();
    
    _boardPositionActive = Vec2(this->getContentSize().width/2 - 68.f, 0.f);
    _boardPositionInactive = Vec2(this->getContentSize().width + kLRGuideViewSize.width/2 - 162.f, 0.f);
    _boardPositionHidden = Vec2(this->getContentSize().width + kLRGuideViewSize.width/2, 0.f);
    
    auto bg = Sprite::create(kPrefixPath + "comprehenson_background.png");
    auto bgSize = bg->getContentSize();
    float bgScale = MAX(_winSize.width/bgSize.width, _winSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(_winSize/2);
    addChild(bg);
    
    _paperBottom = Sprite::create(kPrefixPath + "comprehensive_papers_bottom.png");
    _paperBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _paperBottom->setPosition(_winSize.width / 2, 0.f);
    addChild(_paperBottom);
    
    _pageNode = Node::create();
    _pageNode->setContentSize(_winSize);
    addChild(_pageNode);
    _currentPageGridZ = 0;

    auto blockerView = Node::create();
    blockerView->setContentSize(_winSize);
    _blocker = EventListenerTouchOneByOne::create();
    _blocker->setSwallowTouches(true);
    _blocker->onTouchBegan = [](Touch* T, Event* E) {
        return true;
    };
    blockerView->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_blocker, blockerView);
    addChild(blockerView);
    _blocker->setEnabled(false);
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, _winSize.height-25));
    addChild(backButton);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(_winSize.width/2, _winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    //initGuideView();
    
    return true;
}

void LRComprehensionScene::onEnter()
{
    Layer::onEnter();
}

void LRComprehensionScene::onEnterTransitionDidFinish()
{
    onStart();
}

void LRComprehensionScene::onExit()
{
    Layer::onExit();
    LRSoundButton::soundButtons.clear();
    GameSoundManager::getInstance()->stopBGM();
    GameSoundManager::getInstance()->stopAllEffects();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void LRComprehensionScene::onStart()
{
    _currentProblem = 0;
    
    selectProblem();
    showProblem();
    
    int exceptionTypeCount = 0;
    for (auto p : problemSet)
    {
        if (p->questionType == LRComprehensionType::TitlePage || p->questionType == LRComprehensionType::ReadingOnly || p->questionType == LRComprehensionType::ListeningOnly)
        {
            ++exceptionTypeCount;
        }
    }
    
    _progressBar->setMax((int)problemSet.size() - exceptionTypeCount);
    
    if (problemSet[_currentProblem]->questionType != LRComprehensionType::TitlePage)
    {
        _progressBar->setCurrent(_progressBarIndex);
    }
    
    if (UserManager::getInstance()->isDebugMode()) {
        auto skip = ui::Button::create();
        skip->setTitleFontSize(100);
        skip->setTitleText("Skip");
        skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        skip->setPosition(Vec2(_winSize.width-25, _winSize.height-25));
        addChild(skip);
        skip->addTouchEventListener([this](Ref*,ui::Widget::TouchEventType e) {
            if (e == ui::Widget::TouchEventType::ENDED) {
                onSolve();
            }
        });
    }
}

vector<int> LRComprehensionScene::getLevels()
{
    map<int, int> levelMap;
    vector<int> result;
    
    auto data = LRLevelData::parse();
    for (const auto p : data->problems)
    {
        levelMap[p->level] = p->level;
    }
    
    for (const auto& keyValuePair : levelMap)
    {
        result.push_back(keyValuePair.first);
    }
    
    return result;
}

void LRComprehensionScene::selectProblem()
{
    problemSet = LRLevelData::parse()->getRandomProblemsByLevel(_currentLevel);
}

void LRComprehensionScene::showProblem()
{
    // All clear
    if (_currentProblem == problemSet.size())
    {
        _paperBottom->setVisible(false);
        
        if (guideView != nullptr)
        {
            this->runAction(Sequence::create(DelayTime::create(kGuideViewDelayGameClear), CallFunc::create([this](){
                guideView->runAction(EaseQuarticActionOut::create(MoveTo::create(kGuideViewTweenDuration, _boardPositionHidden)));
            }), nullptr));
        }
        
        auto CP = CompletePopup::create();
        CP->show(1.f, [this] {
            auto Guard = NodeScopeGuard(this);
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
        return;
    }
    else
    {
        _pageGrid = createPageGrid();
        _pageNode->addChild(_pageGrid, _currentPageGridZ--);
        
        _gameNode = Node::create();
        _gameNode->setContentSize(_winSize);
        _pageGrid->addChild(_gameNode);
    }
    
    auto questionType = problemSet[_currentProblem]->questionType;
    
    if (questionType == LRComprehensionType::TitlePage)
    {
        if (guideView != nullptr)
        {
            guideView->runAction(EaseQuarticActionOut::create(MoveTo::create(kGuideViewTweenDuration, _boardPositionHidden)));
        }
        
        auto text = problemSet[_currentProblem]->script;
        _gameNode->addChild(LRTitlePageView::create(text, this));
        return;
    }
    
    if (questionType == LRComprehensionType::ListeningOnly)
    {
        auto text = problemSet[_currentProblem]->script;
        drawGuideView(false, text);
        drawGuideTitle(LanguageManager::getInstance()->isEnglish() ? "Listen to the story." : "Sikiliza hadithi.");
        _currentProblem++;
    }
    else if (questionType == LRComprehensionType::ReadingOnly)
    {
        auto soundPath = problemSet[_currentProblem]->script;
        drawGuideView(true, soundPath);
        drawGuideTitle(LanguageManager::getInstance()->isEnglish() ? "Read the story." : "Soma hadithi.");
        _currentProblem++;
    }
    
    switch (problemSet[_currentProblem]->questionType) {
        case LRComprehensionType::LCAllInOne:
            _gameNode->addChild(LRAllInOneTypeQuestion::LRAllInOneTypeQuestionScene::createLayer(LRAllInOneTypeQuestion::QuestionType::Listening, this));
            break;
        case LRComprehensionType::RCAllInOne:
            _gameNode->addChild(LRAllInOneTypeQuestion::LRAllInOneTypeQuestionScene::createLayer(LRAllInOneTypeQuestion::QuestionType::Reading, this));
            break;
        case LRComprehensionType::MultipleChoices:
            _gameNode->addChild(LRDividedTypeQuestion::LRDividedTypeQuestionScene::createLayer(LRDividedTypeQuestion::QuestionType::TextAndText, this));
            break;
        case LRComprehensionType::FillTheBlank:
            _gameNode->addChild(LRDividedTypeFillTheBlanks::LRDividedTypeFillTheBlanksScene::createLayer(this));
            break;
        case LRComprehensionType::Ordering:
            _gameNode->addChild(LRDividedTypeReordering::LRDividedTypeReorderingScene::createLayer(LRDividedTypeReordering::GameType::Text, this));
            break;
        default:
            break;
    }
}

void LRComprehensionScene::drawGuideView(bool isText, string textOrSoundPath)
{
    if (guideView != nullptr)
    {
        guideView->removeAllChildren();
        guideView->removeFromParent();
    }
    
    _blocker->setEnabled(true);
    
    if (isText)
    {
        guideView  = LRGuideView::createWithText(textOrSoundPath);
        guideView->setVisible(false);
        
        guideView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        guideView->setPosition(Vec2(this->getContentSize().width/2 - 68, 0.f));
        this->addChild(guideView);

        
        guideView->onClickLeftButton = [this]() {
            guideView->runAction(EaseQuarticActionOut::create(MoveTo::create(kGuideViewTweenDuration, _boardPositionActive)));
            CCLOG("onClickLeftButton");
            _blocker->setEnabled(true);
        };
        
        guideView->onClickRightButton = [this]() {
            guideView->runAction(EaseQuarticActionOut::create(MoveTo::create(kGuideViewTweenDuration, _boardPositionInactive)));
            CCLOG("onClickRightButton");
            _blocker->setEnabled(false);
            if (_guideHighlight != nullptr)
            {
                _guideHighlight->removeFromParent();
                _guideHighlight = nullptr;
                
                _questionHighlight->setVisible(true);
            }
        };
    }
    else
    {
        guideView  = LRGuideView::createWithSound(textOrSoundPath);
        guideView->setVisible(false);
        
        guideView->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        guideView->setPosition(Vec2(this->getContentSize().width/2 - 68, 0.f));
        this->addChild(guideView);
        
        guideView->onClickLeftButton = [this]() {
            guideView->runAction(EaseQuarticActionOut::create(MoveTo::create(kGuideViewTweenDuration, _boardPositionActive)));
            CCLOG("onClickLeftButton");
            _blocker->setEnabled(true);
        };
        
        guideView->onClickRightButton = [this]() {
            guideView->runAction(EaseQuarticActionOut::create(MoveTo::create(kGuideViewTweenDuration, _boardPositionInactive)));
            CCLOG("onClickRightButton");
            _blocker->setEnabled(false);
            if (_guideHighlight != nullptr)
            {
                _guideHighlight->removeFromParent();
                _guideHighlight = nullptr;
                
                _questionHighlight->setVisible(true);
            }
        };
    }
    
    float d = _currentProblem == 0 ? 0.1f : 0.7f;
    this->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([this, d](){
        guideView->setPosition(_boardPositionHidden);
        guideView->setVisible(true);
        guideView->runAction(EaseQuarticActionOut::create(MoveTo::create(0.3f, _boardPositionActive)));
    }), nullptr));
}

void LRComprehensionScene::setLevel(int level)
{
    _currentLevel = level;
}

void LRComprehensionScene::drawQuestionTitleWithSoundButton(string titleText, string soundPath, Node* parentNode)
{
    drawQuestionTitle(titleText, parentNode, true, soundPath, 40.f);
}

void LRComprehensionScene::drawQuestionTitle(string titleText, Node* parentNode)
{
    drawQuestionTitle(titleText, parentNode, false, "", -100.f);
}

void LRComprehensionScene::drawQuestionTitle(string titleText, Node* parentNode, bool useSoundButton, string soundPath, float addLeftPadding)
{
    float leftPadding = 200.f;
    float rightPadding = 200.f;
    
    _questionHighlight = Sprite::create(kPrefixPath + "comprehention_question_highlight.png");
    _questionHighlight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _questionHighlight->setPosition(200.f, parentNode->getContentSize().height - 300.f);
    parentNode->addChild(_questionHighlight);
    
    auto titleLabel = TodoUtil::createLabel(titleText, 60.f, Size::ZERO, FONT_ANDIKA_BOLD, Color4B(77, 77, 77, 255));
    titleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    titleLabel->setPosition(leftPadding + addLeftPadding, _questionHighlight->getContentSize().height / 2);
    _questionHighlight->addChild(titleLabel);
    
    float maxLabelWidth = _questionHighlight->getContentSize().width - (leftPadding + addLeftPadding) - rightPadding;
    float titleLabelWidth = titleLabel->getContentSize().width;
    if (titleLabelWidth > maxLabelWidth) { titleLabel->setScale(maxLabelWidth / titleLabelWidth); }

    if (useSoundButton)
    {
        auto soundButton = LRSoundButton::createSmall(soundPath);
        soundButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        soundButton->setPosition(Vec2(88.f, _questionHighlight->getContentSize().height/2));
        _questionHighlight->addChild(soundButton);
    }
    
    if (_guideHighlight != nullptr) _questionHighlight->setVisible(false);
}

void LRComprehensionScene::drawGuideTitle(string titleText)
{
    float leftPadding = 200.f;
    float rightPadding = 200.f;
    
    auto guideHighlightWrapper = Node::create();
    guideHighlightWrapper->setContentSize(fixedDeviceSize);
    guideHighlightWrapper->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    guideHighlightWrapper->setPosition(getContentSize().width / 2, 0.f);
    addChild(guideHighlightWrapper);
    
    _guideHighlight = Sprite::create(kPrefixPath + "comprehention_question_highlight.png");
    _guideHighlight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _guideHighlight->setPosition(200.f, this->getContentSize().height - 300.f);
    guideHighlightWrapper->addChild(_guideHighlight);
    
    auto titleLabel = TodoUtil::createLabel(titleText, 60.f, Size::ZERO, FONT_ANDIKA_BOLD, Color4B(77, 77, 77, 255));
    titleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    titleLabel->setPosition(leftPadding - 100.f, _guideHighlight->getContentSize().height / 2);
    _guideHighlight->addChild(titleLabel);
    
    float maxLabelWidth = _guideHighlight->getContentSize().width - (leftPadding - 100.f) - rightPadding;
    float titleLabelWidth = titleLabel->getContentSize().width;
    if (titleLabelWidth > maxLabelWidth) { titleLabel->setScale(maxLabelWidth / titleLabelWidth); }
}

int LRComprehensionScene::getCurrentProblem()
{
    return _currentProblem;
}

void LRComprehensionScene::onSolveWithDelay(float delayTime, bool playSound)
{
    this->runAction(Sequence::create(DelayTime::create(delayTime), CallFunc::create([this, playSound](){
        onSolve(playSound);
    }) ,nullptr));
}

void LRComprehensionScene::onSolve(bool playSound)
{
    this->stopAllActions();
    _blocker->setEnabled(true);
    if (playSound)
    {
        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/success.m4a");
    }
    
    this->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([this](){
        GameSoundManager::getInstance()->stopBGM();
        GameSoundManager::getInstance()->stopAllEffects();
        LRSoundButton::soundButtons.clear();

        _progressBar->setCurrent(_progressBarIndex, true);
        
        _currentProblem++;
            
        auto previousPageGrid = _pageGrid;
        
        showProblem();
        
        GameSoundManager::getInstance()->playEffectSound(pageTurnEffect);
        
        const float delay = 1.5f;
        
        if (_currentProblem < problemSet.size())
        {
            this->runAction(Sequence::create(DelayTime::create(0.1f + delay / 2), CallFunc::create([this](){
                if (problemSet[_currentProblem]->questionType != LRComprehensionType::TitlePage)
                {
                    _progressBarIndex++;
                    _progressBar->setCurrent(_progressBarIndex);
                }
            }), nullptr));
        }
        
        auto turnSeq = Sequence::create(DelayTime::create(0.1f), PageTurn3D::create(delay, Size(100, 100)), FadeOut::create(0.1f), CallFunc::create([this, previousPageGrid](){
            previousPageGrid->removeFromParent();
            _blocker->setEnabled(false);
        }), nullptr);
        
        previousPageGrid->runAction(turnSeq);
        
    }), nullptr));
}

NodeGrid* LRComprehensionScene::createPageGrid()
{
    auto grid = NodeGrid::create();
    grid->setContentSize(_winSize);
    
    auto paperTop = Sprite::create(kPrefixPath + "comprehensive_papers_top.png");
    paperTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    paperTop->setPosition(_winSize.width / 2, 0.f);
    grid->addChild(paperTop);
    
    return grid;
}
