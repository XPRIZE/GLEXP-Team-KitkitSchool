//
//  AnimalPizzleScene.cpp
//
//  Created by Gunho Lee on 12/10/16.
//
//


#include "AnimalPuzzleScene.hpp"
#include <Common/Basic/CommentStream.h>
#include <Common/Basic/CustomFacet.h>
#include "Common/Basic/SoundEffect.h"

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include "ui/CocosGUI.h"

#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/GameSoundManager.h"


#include "Utils/TodoUtil.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Controls/TutorialVideoPlayer.hpp"


#include "CCAppController.hpp"

using namespace cocos2d::ui;
using namespace std;



namespace AnimalPuzzleSceneSpace {

    
    const char* defaultFont = "fonts/TodoSchoolV2.ttf";
    
    const string resourcePath = "AnimalPuzzle/";
    
    const Size gameSize = Size(2560, 1800);
    const Size boardSize = Size(2407, 1296);
    const Size frameSize = Size(2426, 1315);
    
    const float snapRadiusOnMoved = 20.f;
    const float snapRadiusOnEnded = 80.f;
    
    SoundEffect pickEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Wood_SlideOut.m4a"); }
    SoundEffect snapEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Wood_Correct.m4a"); }
    SoundEffect solveEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Counting_Win.m4a"); }

    
    const string fontName = "fonts/TodoSchoolV2.ttf";
    const bool gameDebug = false;
    
    
    
}

using namespace AnimalPuzzleSceneSpace;


Scene* AnimalPuzzleScene::createScene(int levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = AnimalPuzzleScene::create();
    layer->setLevel(levelID);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool AnimalPuzzleScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    

    
    pickEffect().preload();
    snapEffect().preload();
    solveEffect().preload();
    
    
    auto winSize = getContentSize();
    
    auto bg = Sprite::create(resourcePath+"_ap_woodbackground.jpg");
    auto bgSize = bg->getContentSize();
    float bgScale = MAX(winSize.width/bgSize.width, winSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(winSize/2);
    addChild(bg);
    
    
    _gameNode = Node::create();
    _gameNode->setContentSize(gameSize);
    float gameScale = MIN(winSize.width/gameSize.width, winSize.height/gameSize.height) * 0.9f;
    _gameNode->setScale(gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(Vec2(winSize.width/2, winSize.height*(1-0.5*0.9)));
    addChild(_gameNode);

    

    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    if (UserManager::getInstance()->isDebugMode()) {
        auto skip = Button::create();
        skip->setTitleFontSize(100);
        skip->setTitleText("Skip");
        skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        skip->setPosition(Vec2(winSize.width-25, winSize.height-25));
        addChild(skip);
        skip->addTouchEventListener([this](Ref*,Widget::TouchEventType e) {
            if (e == Widget::TouchEventType::ENDED) {
                for (auto p : _animalPieces) {
                    if (!p->_snapped) {
                        p->snapTarget();
                        break;
                    }
                }
            }
        });
        
    }
    
    return true;
}

void AnimalPuzzleScene::onEnter()
{
    Layer::onEnter();


   // _progressBar->setMax((int)sheet.size());

}

void AnimalPuzzleScene::onEnterTransitionDidFinish()
{
    /*
    if (_currentLevel==1) {
     
        auto v = TutorialVideoPlayer::create();
        addChild(v);
        v->playVideo("TutorialVideo/xprize_tutorial_alphabet.m4v");
        v->onCompleted = [this]() {
            onStart();
        };
        
        
        
    } else
     */
    {
        onStart();
    }
}

void AnimalPuzzleScene::onStart()
{
    



    
    loadData(_currentLevel);
    _currentPuzzleIndex = 0;
    
    if (_currentLevel>=5) {
        random_shuffle(_puzzles.begin(), _puzzles.end());
        auto p = _puzzles.front();
        _puzzles.clear();
        _puzzles.push_back(p);
    

    }
    
    _progressBar->setMax(_puzzles.size());
    
    
    showPuzzle(_currentPuzzleIndex);
    
    
    

    
}


void AnimalPuzzleScene::showPuzzle(int index)
{
    _progressBar->setCurrent(index+1, false);
    createPuzzle(index);
    
}

void AnimalPuzzleScene::onPuzzleComplete()
{
    pickEffect().play();
    solveEffect().play();
    
    auto e = _puzzles[_currentPuzzleIndex];
    
    
    _progressBar->setCurrent(_currentPuzzleIndex+1, true);
    _currentPuzzleIndex++;
    
    auto winSize = getContentSize();
    
    
    auto label = TodoUtil::createLabel(e.text, 160, Size::ZERO, fontName, Color4B(70, 70, 70, 255));
    auto labelPos = Vec2(winSize.width/2, _gameNode->getBoundingBox().origin.y);
    label->setPosition(labelPos);
    addChild(label);
    
    float voiceDelay = (e.text.length()>15) ? 2.0 : 1.0;
    
    if (_currentPuzzleIndex>=_puzzles.size()) {
        auto seq = Sequence::create(DelayTime::create(1.0),
                                    CallFunc::create([this, e](){ playSound(e.text); }),
                                    nullptr);
        this->runAction(seq);
        
        auto CP = CompletePopup::create();
        CP->show(voiceDelay+1.5f, [this] {
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
    } else {
        auto seq = Sequence::create(DelayTime::create(1.0),
                                    CallFunc::create([this, e](){ playSound(e.text); }),
                                    DelayTime::create(voiceDelay+1.0),
                                    CallFunc::create([this, label](){ label->removeFromParent(); showPuzzle(_currentPuzzleIndex); }),
                                    nullptr);
        this->runAction(seq);
        
    }
    
}

void AnimalPuzzleScene::setLevel(int level)
{
    _currentLevel = level;
}



void AnimalPuzzleScene::createPuzzle(int index)
{
    _gameNode->removeAllChildren();
    
    _animalPieces.clear();
    _numSnappedPieces = 0;
    
    auto info = _puzzles.at(index);
    auto folder = resourcePath + info.folderName;
    

    float startDelay = (info.text.length()>15) ? 2.0 : 1.2;
    

    
    auto gameBoard = Sprite::create(folder + info.backgroundFilename);
    auto boardPos = Vec2(gameSize.width/2, gameSize.height/2);
    
    auto boardOrigin = Vec2(boardPos.x-boardSize.width/2, boardPos.y-boardSize.height/2);
    
    gameBoard->setPosition(boardPos);
    _gameNode->addChild(gameBoard);
    
    
    auto boardFrame = Scale9Sprite::create(resourcePath + "ap_boardframe.png");
    boardFrame->setPreferredSize(frameSize);
    boardFrame->setPosition(boardPos);
    _gameNode->addChild(boardFrame);
    

    
    auto clip = ClippingNode::create();
    //auto clip = Node::create();
    Node *stencil;
    if (info.maskFilename=="x") {
        //auto draw = DrawNode::create();
        //draw->drawSolidRect(Vec2::ZERO, boardSize, Color4F::RED);
        stencil = LayerColor::create(Color4B(255, 0, 0, 255), boardSize.width, boardSize.height);
        //stencil = Sprite::create(folder + info.backgroundFilename);
        //stencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
        clip->setAlphaThreshold(1);
        if (_currentLevel<=16) {
            clip->setVisible(false);
        }
    } else {
        stencil = Sprite::create(folder + info.maskFilename);
        stencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
        clip->setAlphaThreshold(0.5);
        //auto l = LayerColor::create(Color4B(255, 0, 0, 255));
        //clip->addChild(l);
    }
    stencil->setPosition(boardPos-boardSize/2);
    clip->setStencil(stencil);

    //stencil->setOpacity(128);
    //clip->addChild(stencil);
    
    clip->setContentSize(gameSize);
    
    
    _depthNode = clip;
    _gameNode->addChild(_depthNode);
    
    _pieceNode = Node::create();
    _pieceNode->setContentSize(gameSize);
    _gameNode->addChild(_pieceNode);
    
    auto winSize = getContentSize();
    auto piecePlaceOrigin = _gameNode->convertToNodeSpace(Vec2(0, 0));
    auto boardOriginWorldPos = _gameNode->convertToWorldSpace(boardOrigin);
    auto piecePlaceEnd = _gameNode->convertToNodeSpace(Vec2(winSize.width, boardOriginWorldPos.y));
    
    
    for (int i=0; i<info.pieceFaceVector.size(); i++) {
        string face = folder + info.pieceFaceVector.at(i)+".png";
        string depth = folder + info.pieceDepthVector.at(i)+".png";
        string shadow = folder + info.pieceShadowVector.at(i)+".png";
        
        auto piece = AnimalPiece::create();
        piece->setTexture(face, depth, shadow);
        
        auto pSize = piece->getContentSize();
        
        piece->_targetPos = boardOrigin+info.piecePosVector.at(i)+pSize/2;
        
        auto x = random(piecePlaceOrigin.x+pSize.width/2, piecePlaceEnd.x-pSize.width/2);
        auto y = random(piecePlaceOrigin.y+pSize.height/4, piecePlaceEnd.y-pSize.height/4);
        auto t = random(startDelay, startDelay+0.5f);

        piece->setPosition(piece->_targetPos);
        piece->_body->setVisible(false);
        piece->_shadow->setVisible(false);
        piece->runAction(Sequence::create(DelayTime::create(t),
                                          CallFunc::create([piece](){
            piece->_body->setVisible(true);
            piece->_shadow->setVisible(true);
            pickEffect().play();
        }), EaseOut::create(MoveTo::create(0.5, Vec2(x, y)), 2.f),
                                          CallFunc::create([piece](){
            piece->_touchEnabled = true;
        }), nullptr));
        

        
        piece->onSnapped = [this, piece]() {
            _numSnappedPieces++;
            
            piece->_body->setVisible(true);
            
            auto bodyPos = piece->getPosition();
            piece->_body->retain();
            piece->_body->removeFromParent();
            piece->_body->setPosition(bodyPos);
            _depthNode->addChild(piece->_body);
            piece->_body->release();
            
            piece->retain();
            piece->removeFromParent();
            _pieceNode->addChild(piece);
            piece->release();
            
            if (_numSnappedPieces >= _animalPieces.size()) {
                onPuzzleComplete();
            } else {
                snapEffect().play();
            }
        };
        
        _gameNode->addChild(piece);
        _animalPieces.push_back(piece);
        
        
    }
    
    playSound(info.text);
    
}

void AnimalPuzzleScene::loadData(int level)
{
    string P = resourcePath + "AnimalPuzzle_Levels.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);
    
    todoschool::CommentStream IS(S);
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    
    auto Facet = [&] {
        auto It = new todoschool::CustomFacet(IS.getloc());
        
        // NB(xenosoz, 2016): U+0020 'SPACE' is not a whitespace anymore.
        It->clearSpaceFlagsFor(" ");
        return It;
    }();
    

    auto Loc = locale(IS.getloc(), Facet);
    IS.imbue(Loc);
    
    
    string LanguageTag;
    size_t levelID;
    size_t puzzleID;
    size_t pieceNum;
    
    _puzzles.clear();
    
    AnimalPuzzleLevelStruct e;


    const string engTag = "en-US";
    
    while (IS >> LanguageTag >> levelID >> puzzleID >> e.text )
    {
        
        if (LanguageTag!=engTag) {
            if (LanguageTag==Lang && levelID==level) {
                AnimalPuzzleLevelStruct info = _puzzles[puzzleID-1];
                info.text = e.text;
                _puzzles[puzzleID-1] = info;
            }
            continue;
                
            
        }
        
        if (!(IS >> e.folderName >> e.backgroundFilename >> pieceNum >> e.maskFilename)) return;
        
        
        e.pieceFaceVector.clear();
        e.pieceDepthVector.clear();
        e.pieceShadowVector.clear();
        e.piecePosVector.clear();
        
        for (int i=0; i<pieceNum; i++) {
        
            int pieceID;
            string face, depth, shadow;
            float x, y;
            
            if (!(IS >> pieceID >> face >> x >> y >> depth >> shadow)) {
                CCLOGERROR("Unexpected input stream error in %s", __PRETTY_FUNCTION__);
            }
            
            e.pieceFaceVector.push_back(face);
            if (depth=="x") depth = face+"depth";
            e.pieceDepthVector.push_back(depth);
            if (shadow=="x") shadow = face+"shadow";
            e.pieceShadowVector.push_back(shadow);
            
            e.piecePosVector.push_back(Point(x, y));
        }
        
        if (LanguageTag==engTag && levelID==level) {
            _puzzles.push_back(e);
        }
    }
    
    
    
    
}


void AnimalPuzzleScene::playSound(string name)
{
    string lower;
    
    
    
    for (int i=0; i<name.length(); i++) {
        char c = name[i];
        if (c>'A' && c<'Z') c = c+'a'-'A';
        lower += c;
    }
    
    string lang = LanguageManager::getInstance()->getCurrentLanguageCode();
    string path = "AnimalPuzzle/Sound/"+lang+"/"+lower+".m4a";
    
    GameSoundManager::getInstance()->playEffectSound(path);
    
}

////////////////////



bool AnimalPiece::init()
{

    if (!Node::init()) return false;

    _snapped = false;
    onSnapped = nullptr;
    _touchEnabled = false;
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        if (_snapped) return false;
        if (!_touchEnabled) return false;
        
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        if (this->getBoundingBox().containsPoint(pos)) {


            this->setPicked(true);
            pickEffect().play();
            
            this->retain();
            this->removeFromParent();
            P->addChild(this);
            this->release();
            
            return true;
        }
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* T, Event* E) {
        if (_snapped) return;
        
        auto P = getParent();
        auto pl = P->convertToNodeSpace(T->getPreviousLocation());
        auto cl = P->convertToNodeSpace(T->getLocation());
        auto delta = cl-pl;
        this->setPosition(this->getPosition()+delta);
        
        auto tp = _targetPos;
        auto cp = getPosition();
        
        if (gameDebug) {
            _ghost->setVisible(false);
        }
        
        if (_targetPos.distance(getPosition())<snapRadiusOnMoved) {
            this->snapTarget();
            
        }
    };
    
    listener->onTouchEnded = [this](Touch* T, Event* E) {
        if (_snapped) return;
        setPicked(false);
        
        if (_targetPos.distance(getPosition())<snapRadiusOnEnded) {
            this->snapTarget();
        } else {
            auto P = getParent();
            auto pos = P->convertToWorldSpace(this->getPosition());
            Vec2 loc = pos;
            
            auto winSize = Director::getInstance()->getWinSize();
            const auto margin = Size(50, 50);
            
            loc.x = MAX(margin.width, loc.x);
            loc.x = MIN(winSize.width-margin.width, loc.x);
            loc.y = MAX(margin.height, loc.y);
            loc.y = MIN(winSize.height-margin.height, loc.y);
            if (loc.distance(pos)>5) {
                runAction(EaseOut::create(MoveTo::create(0.12, loc), 2.0));
            }
        }
        
        
        
        
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
    

}


void AnimalPiece::setTexture(string face, string body, string shadow)
{
    _face = Sprite::create(face);

    Size size = _face->getContentSize();
    setContentSize(size);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _face->setPosition(size/2);

    
    _body = Sprite::create(body);
    _body->setPosition(size/2);
    
    _shadow = Sprite::create(shadow);
    _shadow->setPosition(size/2);
    
    addChild(_shadow);
    addChild(_body);
    addChild(_face);
    
    if (gameDebug) {
        _ghost = Sprite::create(face);
        _ghost->setOpacity(128);
        _ghost->setVisible(false);
        addChild(_ghost);

    }
    
    
}

void AnimalPiece::setPicked(bool picked)
{
    Size size = getContentSize();
    
    if (picked) {
        
        _face->setPosition(size.width/2, size.height/2+5);
        _body->setPosition(size.width/2, size.height/2+5);
        _shadow->setPosition(size.width/2+5, size.height/2-5);
        
        if (gameDebug) {
            _ghost->setVisible(true);
            _ghost->setPosition(_targetPos - getPosition() + size/2);
            
        }
    } else {
        _face->setPosition(size/2);
        _body->setPosition(size/2);
        _shadow->setPosition(size/2);
        
        if (gameDebug) {
            _ghost->setVisible(false);
        }
    }
}

void AnimalPiece::snapTarget()
{
    this->setPosition(_targetPos);
    setPicked(false);
    _shadow->setVisible(false);
    _snapped = true;
    _touchEnabled = false;
    
    if (onSnapped) {
        onSnapped();
    }

}
