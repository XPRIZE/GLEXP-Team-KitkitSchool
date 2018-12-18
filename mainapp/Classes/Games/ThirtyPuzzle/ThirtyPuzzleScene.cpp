//
//  ThirtyPuzzleScene.cpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 07/02/2018.
//

#include "ThirtyPuzzleScene.hpp"
#include <Common/Basic/CommentStream.h>
#include <Common/Basic/CustomFacet.h>

#include "CCAppController.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/UserManager.hpp"
#include "Managers/StrictLogManager.h"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"

BEGIN_NS_THIRTYPUZZLE;

Scene* ThirtyPuzzleScene::createScene(int levelID)
{
    auto scene = Scene::create();
    auto layer = ThirtyPuzzleScene::create();
    layer->setLevel(levelID);
    scene->addChild(layer);
    return scene;
}

void ThirtyPuzzleScene::setLevel(int level) {
    LOGFN();
    _currentLevel = level;
}

void ThirtyPuzzleScene::removeObjects() {
    for (auto p : _pieces) p->removeFromParent();
    _pieces.clear();
    _gameNode->removeAllChildren();
}

vector<Piece*> ThirtyPuzzleScene::getOverlappedPieces(Piece &piece, float distance) {
    vector<Piece*> rtPieces = {};

    for (auto p : _pieces) {
        if (p->_snapped) continue;
        if (p->_number == piece._number) continue;

        //CCLOG("test x:%f",p->getPositionX());
        //CCLOG("test y:%f",p->getPositionY());

        //CCLOG("new x:%f",piece.getPositionX());
        //CCLOG("new y:%f",piece.getPositionY());

        if (p->getPosition().distance(piece.getPosition())<distance) {
            //CCLOG("crash!");
            rtPieces.push_back(p);
        } //else CCLOG("ok");

    }
    return rtPieces;
}

void ThirtyPuzzleScene::cleanUpOverlappedPieces(Piece &piece) {
    auto targetPieces = getOverlappedPieces(piece);

    for (auto tp : targetPieces) {
        tp->runAction(EaseIn::create(MoveBy::create(0.1, Vec2(0, -320)),3.0));
    }
}

bool ThirtyPuzzleScene::init() {

    if (!Layer::init()) return false;

    srand((unsigned)time(0));

    // _gameSize = Size(_winSize.height*4/3, _winSize.height);

    _progressBar = ProgressIndicator::create();

    _winSize = getContentSize();

    auto bg = Sprite::create("ThirtyPuzzle/boards_image_background.jpg");
    auto bgSize = bg->getContentSize();
    auto bgScale = MAX(_winSize.width / bgSize.width, _winSize.height / bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(_winSize/2);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(bg);

    _gameSize = Size(2560, 1800);
    _gameScale = _winSize.height / 1800;

    _gameNode = Node::create();
    _gameNode->setContentSize(_gameSize);
    _gameNode->setScale(_gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(Vec2(_winSize.width/2, _winSize.height/2));
    addChild(_gameNode);

    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, _winSize.height-25));
    addChild(backButton);

    _progressBar->setPosition(Vec2(_winSize.width/2, _winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);

    if (UserManager::getInstance()->isDebugMode()) {
        auto skip = Button::create();
        skip->setTitleFontSize(100);
        skip->setTitleText("Skip");
        skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        skip->setPosition(Vec2(_winSize.width-25, _winSize.height-25));
        addChild(skip);
        skip->addTouchEventListener([this](Ref*,Widget::TouchEventType e) {
            if (e == Widget::TouchEventType::ENDED) {
                checkAnswer(true);
            }
        });

    }

    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_Counting_Win.m4a", false, 1.0f, 0, 0);
    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_ChildrenCheerNew.m4a", false, 1.0f, 0, 0);
    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_Wood_Correct.m4a", false, 1.0f, 0, 0);
    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_Wood_SlideOut.m4a", false, 1.0f, 0, 0);
    return true;
}

void ThirtyPuzzleScene::goNextProblem() {

    this->runAction(Sequence::create(
        DelayTime::create(1.0),
        CallFunc::create([this](){
            setProblem();
        }),
        nullptr
    ));

}

void ThirtyPuzzleScene::setProblem() {
    LOGFN();
    _progressBar->setCurrent(_currentProblemIndex+1, false);

    removeObjects();

    auto problem = _problems[_currentProblemIndex];
    vector<int> questionNumbers;
    vector<int> cellNumbers;

    stringstream ss(problem.suggest);
    while(ss.good()) {
        string substr;
        getline(ss, substr, ',');
        questionNumbers.push_back(TodoUtil::stoi(substr));
    }
    
    stringstream ssc(problem.columns);
    while(ssc.good()) {
        string substr;
        getline(ssc, substr, ',');
        cellNumbers.push_back(TodoUtil::stoi(substr));
    }
    
    int cellCount = cellNumbers.size();

    Sprite *answerBoard;
    
    if (cellCount < 11) {
        answerBoard = Sprite::create("ThirtyPuzzle/boards_image_10board.png");
        answerBoard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        answerBoard->setPosition(_gameSize.width/2, _gameSize.height-310);
        _gameNode->addChild(answerBoard);
    } else if (cellCount < 21) {
        answerBoard = Sprite::create("ThirtyPuzzle/boards_image_20board.png");
        answerBoard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        answerBoard->setPosition(_gameSize.width/2, _gameSize.height-289);
        _gameNode->addChild(answerBoard);
    } else {
        answerBoard = Sprite::create("ThirtyPuzzle/boards_image_30board.png");
        answerBoard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        answerBoard->setPosition(_gameSize.width/2, _gameSize.height-267);
        _gameNode->addChild(answerBoard);
    }

    float startPosX = _gameSize.width/2-234*4.5;
    float startPosY = answerBoard->getPositionY()-188;
    
    float fontSize = 140;
    for (auto cellNumber : cellNumbers) if (cellNumber > 99) fontSize = 90;
    
    int i=1;
    for (auto cellNumber : cellNumbers) {
        
        Piece* piece;
        bool inLastLine = (cellCount-i) <10 ? true : false;
        if (find(begin(questionNumbers), end(questionNumbers), cellNumber) == questionNumbers.end()) {
            piece = Piece::create();
            piece->setAttribute(cellNumber, fontSize, true, inLastLine);
            
        } else {
            piece = Piece::create();
            piece->setAttribute(cellNumber, fontSize, false, inLastLine, random<int>(1, 10));
        }
        //264*(int)((i-1)/10);
        
        piece->setPosition(startPosX+234*((i-1)%10),startPosY-234*(int)((i-1)/10));
        piece->setAnswerPos(piece->getPosition());
        _gameNode->addChild(piece);
        piece->onSnapEnded = [this, piece](){
            checkAnswer();
            cleanUpOverlappedPieces(*piece);
        };
        piece->onTouchEnded = [this, piece](){
            writeActionLog(*piece);
        };
        
        _pieces.push_back(piece);
        releaseColoredPiece(*piece);
        i++;
    }

}

void ThirtyPuzzleScene::writeActionLog(Piece& piece) {
    LOGFN();

    // NB(xenosoz, 2018): Log for future analysis (#2/2)
    auto workPath = [this] {
        stringstream ss;
        ss << "/" << "ThirtyPuzzle";
        ss << "/" << "level-" << _currentLevel << "-0";
        ss << "/" << "work-" << _currentProblemIndex;
        return ss.str();
    }();

    StrictLogManager::shared()->game_Peek_Answer("ThirtyPuzzle", workPath,
                                                 TodoUtil::itos(piece._number),
                                                 (piece._snapped ? TodoUtil::itos(piece._number) : "None"));
}

void ThirtyPuzzleScene::checkAnswer(bool force) {
    LOGFN();
    bool finished = true;
    for (auto p : _pieces) {
        if (p->_snapped) continue;
        finished = false;
    }
    if (force) finished = true;
    if (finished) {
        _currentProblemIndex++;
        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_Counting_Win.m4a");

        if (_currentProblemIndex == _totalProblemCount) { // clear
            _progressBar->setCurrent(_currentProblemIndex+1, false);

            this->scheduleOnce([this](float){
                auto CP = CompletePopup::create();
                CP->show(0.3, [this] {
                    CCAppController::sharedAppController()->handleGameComplete(1);
                });
            }, 0.3, "onGameComplete");

        } else {
            goNextProblem();
        }
    }
}

void ThirtyPuzzleScene::releaseColoredPiece(Piece &piece) {
    if (piece._snapped) return;
    for (int i=0; i<100; i++) {
        float x = random<float>(400, _winSize.width-400);
        float y = random<float>(200, 450);

        piece.setPosition(Vec2(x,y));
        vector<Piece*> overlappedPieces = getOverlappedPieces(piece, 260-i);
        if (overlappedPieces.size() == 0) return;
    }
}

void ThirtyPuzzleScene::onEnter(){
    LOGFN();
    Layer::onEnter();
    _problems = ProblemBank::getInstance()->loadData(_currentLevel);

    _totalProblemCount = (int)_problems.size();
    _currentProblemIndex = 0;

    _progressBar->setMax(_totalProblemCount);

    setProblem();
}

void ThirtyPuzzleScene::onExit() {
    LOGFN();
    Layer::onExit();

    removeObjects();
    this->removeAllChildren();
    this->unscheduleAllCallbacks();
    GameSoundManager::getInstance()->stopAllEffects();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();

}

END_NS_THIRTYPUZZLE;
