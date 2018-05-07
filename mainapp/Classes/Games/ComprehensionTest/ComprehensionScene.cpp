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
    //createProblemMap();
    
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



void ComprehensionScene::convertToText()
{
    auto path = FileUtils::getInstance()->getWritablePath() + "Books/";
    
    string previousBook = "";
    
    string dataString = "";
    string bookPath = "";
    
    vector<string> files;
    
    
    
    for (auto it : problemSetMap) {
        ComprehensionProblemKey key = it.first;
        string bookName = get<0>(key);
        int quizSet = get<1>(key);
    
        
        if (bookName!=previousBook) {

            if (dataString!="") {
                FileUtils::getInstance()->writeStringToFile(dataString, bookPath+"/compquiz.txt");
                dataString = "";
                
                string filestring = "";
                sort(files.begin(), files.end());
                vector<string> picked;
                for (auto f : files) if (find(picked.begin(), picked.end(), f)==picked.end()) picked.push_back(f);
//                for (auto f : picked) filestring += f + "\n";
//                if (!filestring.empty()) {
//                    FileUtils::getInstance()->writeStringToFile(filestring, bookPath+"/files.txt");
//                }
                
                for (auto f : picked) {
                    string fromFile = "";
                    if (FileUtils::getInstance()->isFileExist("ComprehensionTest/Image/"+f)) fromFile =  "ComprehensionTest/Image/"+f;
                    if (FileUtils::getInstance()->isFileExist("ComprehensionTest/FillTheBlanks/"+f)) fromFile =  "ComprehensionTest/FillTheBlanks/"+f;
                    if (!fromFile.empty()) {
                        auto data = FileUtils::getInstance()->getDataFromFile(fromFile);
                        auto toPath = bookPath + "/quiz";
                        if (!FileUtils::getInstance()->isDirectoryExist(toPath)) {
                            FileUtils::getInstance()->createDirectory(toPath);
                        }

                        bool ret = FileUtils::getInstance()->writeDataToFile(data, toPath + "/" + f);
                        if (!ret) {
                            break;
                        }
                        
                        
                    }
                    
                }
                
                files.clear();
                
            } else {
                if (previousBook!="") dataString += "\n";
            }
            previousBook = bookName;
            
            bookPath= path + bookName;
            if (!FileUtils::getInstance()->isDirectoryExist(bookPath)) {
                FileUtils::getInstance()->createDirectory(bookPath);
            }
        } else {
            dataString += "--------\n";
        }
       
        for (ComprehensionProblem prob  : it.second) {
            std::string comprehensionTestName = prob.first;
            vector<string> v = prob.second;
            
            
            dataString += comprehensionTestName + "\n";
            if (v[0] == "matching")
            {
                //dataString += v[2] + "\n";
                dataString += v[4] + "\n";
                //dataString += v[3] + "\n";
                dataString += v[5] + "\n";
                
                {
                    auto fs = TodoUtil::splitCSV(v[4]);
                    for (auto f : fs) files.push_back(f);
                }
                {
                    auto fs = TodoUtil::splitCSV(v[5]);
                    for (auto f : fs) files.push_back(f);
                }
                
                    
                
                
            }
            else if (v[0] == "fill the blanks")
            {
                dataString += v[2] + "\n"; files.push_back(v[2]);
                dataString += v[3] + "\n";
                if (v.size()>4) {
                    dataString += v[4] + "\n";
                } else {
                    dataString += "\n";
                    
                }
                if (v.size()>6) {
                    dataString += v[6] + "\n"; files.push_back(v[6]);
                }
            }
            else if (v[0] == "reordering")
            {
                dataString += v[2] + "\n";
                {
                    auto fs = TodoUtil::splitCSV(v[2]);
                    for (auto f : fs) files.push_back(f);
                }
            }
            else if (v[0] == "multiple choices")
            {
                if (v[1].length()==0) {
                    dataString+=v[2] + "\n";
                    files.push_back(v[2]);
                }
                else dataString+=v[1] + "\n";
                dataString+=v[3] + "\n";
                dataString+=v[4] + "\n";
                
                {
                    auto fs = TodoUtil::splitCSV(v[3]);
                    for (auto f : fs) files.push_back(f);
                }
                {
                    auto fs = TodoUtil::splitCSV(v[4]);
                    for (auto f : fs) files.push_back(f);
                }
                
            }
            else if (v[0] == "tracing")
            {
                dataString += v[2] + "\n"; files.push_back(v[2]);
                dataString += v[3] + "\n";
            } else {
                
            }

            dataString += "\n";
            
        }
    }
    if (dataString!="") {
        FileUtils::getInstance()->writeStringToFile(dataString, bookPath+"/comptest.txt");
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
    
    std::string dataPath = "ComprehensionTest/ComprehensionTest.tsv";
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
    
    //convertToText();
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
    //else if (comprehensionTestName == "tracing")
    else if (comprehensionTestName.find("tracing") != string::npos)
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
            auto Guard = NodeScopeGuard(this);
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
