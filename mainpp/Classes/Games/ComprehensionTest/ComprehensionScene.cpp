//
//  ComprehensionScene.cpp
//
//  Created by Gunho Lee on 12/10/16.
//
//


#include "ComprehensionScene.hpp"

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include "ui/cocosGUI.h"
#include "Managers/GameSoundManager.h"
#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"
#include "Utils/TodoUtil.h"
#include <Games/NumberTrace/Common/Basic/ScopeGuard.h>
#include <Games/NumberTrace/Common/Basic/NodeScopeGuard.h>

#include "3rdParty/CCNativeAlert.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Controls/PopupBase.hpp"

#include "CompTrace/CompTrace.h"
#include "FillTheBlanks/FillTheBlanksScene.hpp"
#include "Matching/MatchingScene.hpp"
#include "Reordering/ReorderingScene.hpp"
#include "MultipleChoices/MultipleChoicesScene.hpp"

#include "CCAppController.hpp"

using namespace cocos2d::ui;
using cocos2d::StringUtils::format;
using namespace std;

using namespace ComprehensionTest;

namespace ComprehensionSceneSpace {
    const char* solveEffect = "Counting/UI_Star_Collected.m4a";
    const char* missEffect = "Counting/Help.m4a";
    const char* pageTurnEffect = "DoubleDigit/Card_Move_Right.m4a";
    const char* defaultFont = "fonts/TodoSchoolV2.ttf";
    

    const Size gameSize = Size(2560, 1800);
    Size winSize;
    
}

using namespace ComprehensionSceneSpace;


Scene* ComprehensionScene::createScene(string bookFolder, int set)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ComprehensionScene::create();
    layer->setBookData(bookFolder, set);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool ComprehensionScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _touchEnabled = false;
    
    GameSoundManager::getInstance()->preloadEffect(solveEffect);
    GameSoundManager::getInstance()->preloadEffect(missEffect);
    GameSoundManager::getInstance()->preloadEffect(pageTurnEffect);
    
    winSize = getContentSize();
    
    auto bg = Sprite::create("ComprehensionTest/Common/_comprehenson_background.png");
    auto bgSize = bg->getContentSize();
    float bgScale = MAX(winSize.width/bgSize.width, winSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(winSize/2);
    addChild(bg);
    
    _paperBottom = Sprite::create("ComprehensionTest/Common/comprehensive_papers_bottom.png");
    _paperBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _paperBottom->setPosition(winSize.width / 2, 0.f);
    addChild(_paperBottom);
    
    _pageNode = Node::create();
    _pageNode->setContentSize(winSize);
    addChild(_pageNode);
    _currentPageGridZ = 0;

    auto blockerView = Node::create();
    blockerView->setContentSize(winSize);
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
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);

    return true;
}

void ComprehensionScene::onEnter()
{
    Layer::onEnter();


   // _progressBar->setMax((int)sheet.size());

}

void ComprehensionScene::onEnterTransitionDidFinish()
{
    onStart();
}

/*
void ComprehensionScene::onStart()
{
    generateProblem();

    _currentProblem = 0;
    

    _progressBar->setMax(5);
    _progressBar->setCurrent(_currentProblem+1);
    
    showProblem();
//    _
//    float appearTime = 0;
//    putObjects(appearTime);
}
*/


void ComprehensionScene::showChooser()
{
    

    
    
    
    auto chooser = PopupBase::create(this, gameSize);
    
    auto innerView = Node::create();
    
    
    
    
    float y = -80;
    
    for (auto it : problemSetMap) {
        auto key = it.first;
        auto value = it.second;
        
        
        auto button = Button::create();
        button->setTitleFontSize(50);
        button->setTitleColor(Color3B::WHITE);
        
        string folder = get<0>(key);
        int set = get<1>(key);
        
        string title = folder + " - " + TodoUtil::itos(set);
        
        button->setTitleText(title);
        button->setPosition(Vec2(gameSize.width/2, y-=80));
        innerView->addChild(button);
        
        button->addClickEventListener([this, folder, set, button, chooser](Ref*) {
            _bookFolder = folder;
            _problemSetIndex = set;
            
            if (UserManager::getInstance()->isDebugMode()) {
                auto skip = Button::create();
                skip->setTitleFontSize(100);
                skip->setTitleText(format("[%s - %d] Skip", _bookFolder.c_str(), _problemSetIndex));
                skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                skip->setPosition(Vec2(winSize.width-25, winSize.height-25));
                addChild(skip);
                skip->addTouchEventListener([this](Ref*,Widget::TouchEventType e) {
                    if (e == Widget::TouchEventType::ENDED) {
                        onSolve();
                    }
                });
            }
            
            selectProblem();
            showProblem();
            _progressBar->setMax(problemSet.size());
            _progressBar->setCurrent(_currentProblem + 1);
            
            chooser->dismiss(true);
            
        });

    }
    
    
    auto height = -y + 100;
    auto scroll = ui::ScrollView::create();
    scroll->setContentSize(gameSize);
    scroll->setInnerContainerSize(Size(gameSize.width, height));
    innerView->setPosition(Vec2(0, height));
    scroll->addChild(innerView);
    chooser->addChild(scroll);
    
    
    chooser->show(this, true);
    
    
}

void ComprehensionScene::onStart()
{
    createProblemMap();
    
    _currentProblem = 0;

    if (_bookFolder=="") {
        
    
        showChooser();
        return;
        
    }
    
    selectProblem();
    showProblem();
    
    
    _progressBar->setMax(problemSet.size());
    _progressBar->setCurrent(_currentProblem + 1);
    
    //    _
    //    float appearTime = 0;
    //    putObjects(appearTime);
}

void ComprehensionScene::onSolve()
{
    _blocker->setEnabled(true);
    _progressBar->setCurrent(_currentProblem+1, true);
    
    _currentProblem++;

    
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
        _progressBar->setCurrent(_currentProblem+1);
    }), NULL));
    
    auto turnSeq = Sequence::create(DelayTime::create(0.1), PageTurn3D::create(delay, Size(100,100)), FadeOut::create(0.1), CallFunc::create([this, previousPageGrid](){
        previousPageGrid->removeFromParent();
        _blocker->setEnabled(false);
    }), nullptr);
    previousPageGrid->runAction(turnSeq);
}

void ComprehensionScene::setBookData(string bookFolder, int set)
{
    _bookFolder = bookFolder;
    _problemSetIndex = set;
    
    //_bookNo = bookNo;
    //_languageTag = languageTag;
}

int ComprehensionScene::getCurrentProblem()
{
    return _currentProblem;
}

NodeGrid* ComprehensionScene::createPageGrid()
{
    
    auto grid = NodeGrid::create();
    grid->setContentSize(winSize);
    
    auto paperTop = Sprite::create("ComprehensionTest/Common/comprehensive_papers_top.png");
    paperTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    paperTop->setPosition(winSize.width / 2, 0.f);
    grid->addChild(paperTop);
    
    return grid;
}


void checkData(vector<string>& v, int line) {
    string lineStr = TodoUtil::itos(line);
    
    if (v.size()<1) {
        NativeAlert::show("Error in Comp Data (line "+lineStr+")", "no subgame definition", "OK");
        return;
    }
 
    
    int requiredColumns = 0;
    

    if (v[0] == "matching")
    {
        requiredColumns = 5;
    }
    else if (v[0] == "fill the blanks")
    {
        requiredColumns = 4;
    }
    else if (v[0] == "reordering")
    {
       requiredColumns = 2;
    }
    else if (v[0] == "multiple choices")
    {
       requiredColumns = 4;
    }
    else if (v[0] == "tracing")
    {
        requiredColumns = 2;
    } else {
        NativeAlert::show("Error in Comp Data (line "+lineStr+")", "no subgame definition", "OK");
        return;
    }
    
    if (v.size()<requiredColumns) {
        NativeAlert::show("Error in Comp Data (line "+lineStr+")", v[0]+"needs "+TodoUtil::itos(requiredColumns)+" columns, not "+TodoUtil::itos(v.size()), "OK");
        return;
    }

}


void ComprehensionScene::createProblemMap()
{
    problemSet.clear();
    problemSetMap.clear();
    
/*
 
#multiple choices	question text	question image	answers (image or text)	right answers (image or text)
#fill the blanks	question text	question image	text to fill	wrong words
#matching	question text	left type	right type	left items (image or text)	right items (image or text)
#tracing	question text	question image	tracing word
#reordering	question text	images in order

 */
    
    std::string dataPath = "ComprehensionTest/ComprehensionTest - Comprehension Test.tsv";
    std::string rawData = cocos2d::FileUtils::getInstance()->getStringFromFile(dataPath);
    auto data = TodoUtil::readTSV(rawData);
    
    int line = 0;
    
    for (auto row : data)
    {
        line++;
        
        if (row.size() < 1)
            continue;
        
        if (TodoUtil::trim(row[0]).size() <= 0)
            continue;
        
        if (row[0][0] == '#')
            continue;
        
        auto ck = ComprehensionProblemKey(row[0], TodoUtil::stoi(row[3]));
        
        vector<string> newRow;
        newRow.clear();
        // 앞의 4개의 컬럼은 사용되지 않는 것으로 ComprehensionTest의 게임들이 데이터를이 index값을 사용하여 아래와 같이 추출함
        for (int i = 4; i < row.size(); i++)
        {
            newRow.push_back(row[i]);
        }

        checkData(newRow, line);
        
        auto cv = ComprehensionProblem(row[4], newRow);
        problemSetMap[ck].push_back(cv);
    }
}

void ComprehensionScene::selectProblem()
{
    if (_problemSetIndex>0) {
        auto key = ComprehensionProblemKey(_bookFolder, _problemSetIndex);
        problemSet = problemSetMap[key];
        if (problemSet.size() > 0) return;
    }
    
    
    int maxSetNum = 3;
    // 셋의 종류가 최대 3개인 것도 있고 2개인 것도 있는데, 최소 1개는 있다는 가정하에 있을 때까지 찾음
    while(true)
    {
        auto key = ComprehensionProblemKey(_bookFolder, random(1, maxSetNum));
        problemSet = problemSetMap[key];
        if (problemSet.size() > 0)
            break;
        
        maxSetNum--;
        
        CC_ASSERT(maxSetNum > 0);
    }
}

void ComprehensionScene::showProblem()
{
    // All clear
    if (_currentProblem == problemSet.size())
    {
        _paperBottom->setVisible(false);
        
        auto CP = CompletePopup::create();
        CP->show(1.f, [this] {
            auto Guard = todoschool::NodeScopeGuard(this);
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
        return;
    }
    else
    {
        _pageGrid = createPageGrid();
        _pageNode->addChild(_pageGrid, _currentPageGridZ--);
        
        _gameNode = Node::create();
        _gameNode->setContentSize(winSize);
        _pageGrid->addChild(_gameNode);
    }
    
    std::string comprehensionTestName = problemSet[_currentProblem].first;
    
    if (comprehensionTestName == "matching")
    {
        _gameNode->addChild(Matching::MatchingScene::createLayer(this));
    }
    else if (comprehensionTestName == "fill the blanks")
    {
        _gameNode->addChild(FillTheBlanks::FillTheBlanksScene::createLayer(this));
    }
    else if (comprehensionTestName == "reordering")
    {
        _gameNode->addChild(Reordering::ReorderingScene::createLayer(this));
    }
    else if (comprehensionTestName == "multiple choices")
    {
        _gameNode->addChild(MultipleChoices::MultipleChoicesScene::createLayer(this));
    }
    else if (comprehensionTestName == "tracing")
    {
        _gameNode->addChild(CompTrace().createLayer(this));
    }
}

/*
void ComprehensionScene::showProblem()
{
    _pageGrid = createPageGrid();
    _pageNode->addChild(_pageGrid);
    
    
    _gameNode = Node::create();
    _gameNode->setContentSize(winSize);
    _pageGrid->addChild(_gameNode);
    
    if (_currentProblem==0) {
        auto layer = Reordering::ReorderingScene::createLayer(this);
        _gameNode->addChild(layer);
        
    } else if (_currentProblem==1) {
        auto layer = FillTheBlanks::FillTheBlanksScene::createLayer(this);
        _gameNode->addChild(layer);
    
    } else if (_currentProblem==2) {
        CompTrace compTrace;
        compTrace.setText("nanasi");
        auto layer = compTrace.createLayer(this);
        layer->onEnterTransitionDidFinish();
        _gameNode->addChild(layer);
            
    } else if (_currentProblem==3) {
        auto layer = Matching::MatchingScene::createLayer(this);
        _gameNode->addChild(layer);
        
    } else {
        auto CP = CompletePopup::create();
        CP->show(1.f, [this] {
            auto Guard = todoschool::NodeScopeGuard(this);
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
    }
}
*/

Button* ComprehensionScene::drawSoundButton(Node* targetNode)
{
    auto soundButton = Button::create("ComprehensionTest/Common/comprehension_speaker_normal.png", "ComprehensionTest/Common/comprehension_speaker_active.png");
    soundButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    soundButton->setPosition(Vec2(175.f, targetNode->getContentSize().height - 185.f));
    targetNode->addChild(soundButton);
    return soundButton;
}

void ComprehensionScene::drawQuestionTitle(string titleText, Node* parentNode, float addLeftPadding)
{
//    if (TodoUtil::trim(titleText).size() <= 0)
//        return;
    
    float leftPadding = 200.f;
    float rightPadding = 200.f;
    
    auto questionHighlight = Sprite::create("ComprehensionTest/Common/comprehention_question_highlight.png");
    questionHighlight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    questionHighlight->setPosition(200.f, parentNode->getContentSize().height - 300.f);
    parentNode->addChild(questionHighlight);
    
    auto titleLabel = TodoUtil::createLabel(titleText, 65.f, Size::ZERO, "fonts/Aileron-Regular.otf", Color4B(56, 56, 56, 255));
    titleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    titleLabel->setPosition(leftPadding + addLeftPadding, questionHighlight->getContentSize().height / 2);
    questionHighlight->addChild(titleLabel);
    
    float maxLabelWidth = questionHighlight->getContentSize().width - leftPadding - rightPadding;
    float titleLabelWidth = titleLabel->getContentSize().width;
    if (titleLabelWidth > maxLabelWidth) { titleLabel->setScale(maxLabelWidth / titleLabelWidth); }
}
