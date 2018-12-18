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
#include "ui/CocosGUI.h"
#include "Managers/GameSoundManager.h"
#include "Managers/LanguageManager.hpp"
//#include "Managers/UserManager.hpp"
//#include "Managers/StrictLogManager.h"
#include "Utils/TodoUtil.h"
#include "Common/Basic/ScopeGuard.h"
#include "Common/Basic/NodeScopeGuard.h"

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
    const char* solveEffect = "Common/Sounds/Effect/UI_Star_Collected.m4a";
    const char* missEffect = "Common/Sounds/Effect/Help.m4a";
    const char* pageTurnEffect = "Common/Sounds/Effect/Card_Move_Right.m4a";
    const char* defaultFont = FONT_ANDIKA_BOLD;
    

    const Size gameSize = Size(2560, 1800);
    Size winSize;
    
}

using namespace ComprehensionSceneSpace;


bool ComprehensionScene::_isSolveAll = false;

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

Scene* ComprehensionScene::createScene(string bookFolder, int set, bool checkCompleteCondition)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ComprehensionScene::create();
    layer->setBookData(bookFolder, set);
    layer->setCheckCompleteCondition(checkCompleteCondition);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

int ComprehensionScene::getNumSet(string bookFolder)
{
    auto filedata = FileUtils::getInstance()->getStringFromFile(bookFolder+"/compquiz.txt");
    
    std::istringstream iss(filedata);
    std::string line;

    bool hasContents = false;
    int numSet = 0;
    
    while(iss.good())
    {
        TodoUtil::safegetline(iss, line);
        if(line.length() == 0 && !iss.good())
        {
            break;
        }
        
        if (line.length()>0 && line.front()=='-') {
            if (hasContents) numSet++;
            hasContents = false;
        }
        
        if (!line.empty()) hasContents = true;
        
    }
    if (hasContents) numSet++;
    
    
    
    return numSet;
    
}

bool ComprehensionScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _isSolveAll = false;
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

void ComprehensionScene::showBookChooser()
{
    auto chooser = PopupBase::create(this, gameSize);
    auto innerView = Node::create();
    
    
    // NB(xenosoz, 2018): Prevent iOS crash.
    auto list = [&] {
        if (!FileUtils::getInstance()->fullPathForFilename("Books/BookData/").empty()) {
            // NB(xenosoz, 2018): Good on non-iOS.
            return FileUtils::getInstance()->listFiles("Books/BookData/");
        }

        if (!FileUtils::getInstance()->fullPathForFilename("Books/BookData").empty()) {
            // NB(xenosoz, 2018): Good on iOS.
            return FileUtils::getInstance()->listFiles("Books/BookData");
        }
        return vector<string>();
    }();

    float y = -80;

    for (auto folder : list) {
        string quizPath = folder + "compquiz.txt";
        if (FileUtils::getInstance()->isFileExist(quizPath)) {
            auto button = Button::create();
            button->setTitleFontSize(50);
            button->setTitleColor(Color3B::WHITE);
            auto book = TodoUtil::split(folder, '/').back();
            button->setTitleText(book);
            button->setPosition(Vec2(gameSize.width/2, y-=80));
            innerView->addChild(button);
            
            button->addClickEventListener([this, folder, button, chooser](Ref*) {
                _bookFolder = folder;
                this->showSetChooser();
                
                chooser->dismiss(true);
            });
            
        }
    
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

void ComprehensionScene::showSetChooser()
{
    auto chooser = PopupBase::create(this, gameSize);
    
    auto innerView = Node::create();
    
    
    
    
    float y = -80;
    
    int num = getNumSet(_bookFolder);
    
    for (int i=1; i<=num; i++) {
        auto button = Button::create();
        button->setTitleFontSize(50);
        button->setTitleColor(Color3B::WHITE);
        button->setTitleText(TodoUtil::itos(i));
        button->setPosition(Vec2(gameSize.width/2, y-=80));
        innerView->addChild(button);
        
        button->addClickEventListener([this, i, button, chooser](Ref*) {
            _problemSetIndex = i;
            
            if (true) {
                auto skip = Button::create();
                skip->setTitleFontSize(100);
                auto folderName = TodoUtil::split(_bookFolder, '/').back();
                skip->setTitleText(format("[%s - %d] Skip", folderName.c_str(), _problemSetIndex));
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
    _currentProblem = 0;

    if (_bookFolder=="") {
        showBookChooser();
        return;
    }
    
    if (_problemSetIndex==0) {
        showSetChooser();
        return;
    }
    
    selectProblem();
    showProblem();
    
    
    _progressBar->setMax(problemSet.size());
    _progressBar->setCurrent(_currentProblem + 1);
    
}

void ComprehensionScene::onSolve()
{
  //  StrictLogManager::shared()->comprehension_Solve(_bookFolder, _problemSetIndex, _currentProblem);
    
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

void ComprehensionScene::setCheckCompleteCondition(bool checkCompleteCondition)
{
    _checkCompleteCondition = checkCompleteCondition;
}

void ComprehensionScene::setBookData(string bookFolder, int set)
{
    _bookFolder = bookFolder;
    _problemSetIndex = set;
    
    //_bookNo = bookNo;
    //_languageTag = languageTag;
}

std::string ComprehensionScene::getBookName()
{
    return TodoUtil::split(_bookFolder, '/').back();
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

void ComprehensionScene::selectProblem()
{
    
    auto filedata = FileUtils::getInstance()->getStringFromFile(_bookFolder+"/compquiz.txt");
    
    std::istringstream iss(filedata);
    std::string line;
    
    bool setStarted = false;
    bool problemStarted = false;
    int currentSet = 1;
    
    vector<string> v;
    
    while(iss.good())
    {
        TodoUtil::safegetline(iss, line);
        
        
        if(line.length() == 0 && !iss.good())
        {
            break;
        }
        
        if (currentSet>_problemSetIndex) break;
        
        if (line.length()>0 && line.front()=='-') {
            currentSet++;
            continue;
        }
        
        if (currentSet == _problemSetIndex) {
            line = TodoUtil::trim(line);
            if (!setStarted && line.empty()) {
                continue;
            } else {
                setStarted = true;
            }
            
            
            if (!line.empty()) {
                v.push_back(line);
                problemStarted = true;
            } else {
                if (problemStarted) {
            
                    ComprehensionProblem p;
                    p.first = v[0];
                    p.second = v;
                    problemSet.push_back(p);
                    v.clear();
                    problemStarted = false;
                }
            }
        }
    }
    
    if (v.size()>0 && currentSet == _problemSetIndex) {
        ComprehensionProblem p;
        p.first = v[0];
        p.second = v;
        problemSet.push_back(p);
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
            auto Guard = NodeScopeGuard(this);
            _isSolveAll = true;
            
            if (_checkCompleteCondition)
            {
                CCAppController::sharedAppController()->handleGameComplete(1);
            }
            else
            {
                CCAppController::sharedAppController()->handleGameQuit();
            }
            
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
    else if (comprehensionTestName.find("tracing") != string::npos)
    {
        _gameNode->addChild(CompTrace().createLayer(this));
    }
}

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
    
    auto titleLabel = TodoUtil::createLabel(titleText, 60.f, Size::ZERO, FONT_ANDIKA_BOLD, Color4B(77, 77, 77, 255));
    titleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    titleLabel->setPosition(leftPadding + addLeftPadding, questionHighlight->getContentSize().height / 2);
    questionHighlight->addChild(titleLabel);
    
    float maxLabelWidth = questionHighlight->getContentSize().width - leftPadding - rightPadding;
    float titleLabelWidth = titleLabel->getContentSize().width;
    if (titleLabelWidth > maxLabelWidth) { titleLabel->setScale(maxLabelWidth / titleLabelWidth); }
}
