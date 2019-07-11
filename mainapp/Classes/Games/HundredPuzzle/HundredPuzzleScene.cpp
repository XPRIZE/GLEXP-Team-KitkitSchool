//
//  HundredPizzleScene.cpp
//
//  Created by Gunho Lee on 12/10/16.
//
//


#include "HundredPuzzleScene.hpp"

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include "ui/CocosGUI.h"
#include "Common/Basic/SoundEffect.h"
#include "Managers/LanguageManager.hpp"
#include "Managers/StrictLogManager.h"
#include "Utils/TodoUtil.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"


#include "CCAppController.hpp"

using namespace cocos2d::ui;
using namespace std;



namespace HundredPuzzleSceneSpace {
  //  const char* solveEffect = "Counting/UI_Star_Collected.m4a";
  //  const char* missEffect = "Counting/Help.m4a";
  //  const char* touchEffect = "Counting/tokenappear.m4a";
  //  const char* dropEffect = "Counting/panelput.m4a";
    
    
    const char* defaultFont = "fonts/mukta-bold.ttf";
    const Size gameSize = Size(2560, 1800);
    //const Size boardSize = Size(1000, 1000);
    const Size unitSize = Size(138, 138);
    const Vec2 boardMargin = Size(82, 114+6);
    
    const float snapRadiusOnMoved = 20.f;
    const float snapRadiusOnEnded = 80.f;
    
    
    SoundEffect pickEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Wood_SlideOut.m4a"); }
    SoundEffect snapEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Wood_Correct.m4a"); }
    SoundEffect solveEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Counting_Win.m4a"); }
}

using namespace HundredPuzzleSceneSpace;


Scene* HundredPuzzleScene::createScene(int levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HundredPuzzleScene::create();
    layer->setLevel(levelID);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool HundredPuzzleScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _touchEnabled = false;
    
    pickEffect().preload();
    snapEffect().preload();
    solveEffect().preload();
    
    auto winSize = getContentSize();
    
    auto bg = Sprite::create("HundredPuzzle/hundredpuzzle_bg.jpg");
    auto bgSize = bg->getContentSize();
    float bgScale = MAX(winSize.width/bgSize.width, winSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(winSize/2);
    addChild(bg);
    
    


    
    _gameNode = Node::create();
    _gameNode->setContentSize(gameSize);
    float gameScale = MIN(winSize.width/gameSize.width, winSize.height/gameSize.height);
    _gameNode->setScale(gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(winSize/2);
    addChild(_gameNode);

    
    _gameBoard = Sprite::create("HundredPuzzle/hundredpuzzle_board.png");
    auto boardSize = _gameBoard->getContentSize();
//    _gameBoard->setContentSize(boardSize);
    _gameBoard->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _gameBoard->setPosition(34, 36);
    _gameNode->addChild(_gameBoard);
    
    _backNode = Node::create();
    _backNode->setContentSize(gameSize);
    _gameNode->addChild(_backNode);
    
//    auto layerColor = LayerColor::create(Color4B::WHITE, boardSize.width, boardSize.height);
//    _gameBoard->addChild(layerColor);
//    
//    auto draw = DrawNode::create();
//    _gameBoard->addChild(draw);
//    
//    for (int i=0; i<=10; i++) {
//        
//        draw->drawLine(Vec2(boardMargin.x, boardMargin.y+unitSize.height*i), Vec2(boardMargin.x+unitSize.width*10, boardMargin.y+unitSize.height*i), Color4F::BLUE);
//        draw->drawLine(Vec2(boardMargin.x+unitSize.width*i, boardMargin.y), Vec2(boardMargin.x+unitSize.width*i, boardMargin.y+unitSize.height*10), Color4F::BLUE);
//    }
    
    
        
    random_shuffle(_colors.begin(), _colors.end());
    _currentColorIndex = 0;

    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);
    
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(winSize.width/2, winSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    return true;
}

void HundredPuzzleScene::onEnter()
{
    Layer::onEnter();


   // _progressBar->setMax((int)sheet.size());

}

void HundredPuzzleScene::onEnterTransitionDidFinish()
{
    onStart();
}

void HundredPuzzleScene::onStart()
{
    


    setTouchEnabled(true);
    

    createPieces();
    
    if (_currentLevel!=9) {
        fillBoard();
    }
    
}

void HundredPuzzleScene::setLevel(int level)
{
    _currentLevel = level;
}

void HundredPuzzleScene::setTouchEnabled(bool enabled)
{
    _touchEnabled = enabled;
    
}

void HundredPuzzleScene::createPieces()
{
    _pieces.clear();
    
    for (int i=0; i<100; i++) {
        _picked[i] = false;
    }
    
    auto createOnePiece = [&](vector<int>&ps) {
        auto piece = HundredPiece::create();
        piece->setPieces(ps);
        for (auto n : ps) {
            _picked[n] = true;
        }
        
        // NB(xenosoz, 2018): Log for future analysis (#2/2)
        auto workPath = [&] {
            auto pieceId = _pieces.size();
            
            stringstream ss;
            ss << "/" << "HundredPuzzle";
            ss << "/" << "level-" << _currentLevel << "-0";
            ss << "/" << "work-" << pieceId;
            return ss.str();
        }();
        
        auto signature = [&] {
            stringstream ss;
            ss << "[";
            for (auto n : ps) { ss << n << ", "; }
            ss << "]";
            return ss.str();
        }();
        
        piece->onTouched = [this, piece, workPath, signature]() {
            StrictLogManager::shared()->game_Peek_Answer("HundredPuzzle", workPath, signature, "None");
        };
        
        piece->onSnapped = [this, piece, workPath, signature](){
            StrictLogManager::shared()->game_Peek_Answer("HundredPuzzle", workPath, signature, signature);
            
            piece->getParent()->reorderChild(piece, piece->getLocalZOrder());
            
//            piece->retain();
//            piece->removeFromParent();
//            _backNode->addChild(piece);
//            piece->release();
            
            
            int target = _pieces.size();
            for (auto p : _pieces) {
                if (p->isSnapped()) target--;
            }
            if (target==0) {
                solveEffect().play();
                if (_currentLevel==9) {
                    fillBoard();
                }
                CompletePopup::create()->show(1.5, [](){
                    CCAppController::sharedAppController()->handleGameComplete(1);
                });
            } else {
                snapEffect().play();
            }
        };
        
        
        _pieces.push_back(piece);
        
        
    };
    
    
    switch (_currentLevel) {
        case 1: {
            

            
            for (int i=0; i<8; i++) {
                auto ret = pickNumbers(6);
                if (ret.size()>0) createOnePiece(ret);
                
            }

            break;
        }
            
        case 2: {
         
            int trial = 0;
            while (_pieces.size()<8 && trial<100) {
                trial++;
                int w = random(2, 3);
                int h = (w==2) ? 3 : random(2, 3);
                
                auto ret = pickRect(w, h);
                if (ret.size()>0) createOnePiece(ret);
            }
            
            break;
        }
        case 3: {
            
            vector<int> ret;
            for (int i=0; i<10; i++) {
                ret.clear();
                for (int j=0; j<10; j++) {
                    ret.push_back(i*10+j);
                }
                createOnePiece(ret);
            }
            
            
            break;
        }
        case 4: {
            
            vector<int> ret;
            for (int i=0; i<10; i++) {
                ret.clear();
                for (int j=0; j<10; j++) {
                    ret.push_back(i+10*j);
                }
                createOnePiece(ret);
            }
            
            
            break;
        }
        case 5: {
            vector<int> cand, ret;
            for (int i=0; i<100; i+=5) {
                cand.push_back(i);
            }
            
            random_shuffle(cand.begin(), cand.end());
            
            for (int i=0; i<17; i++) {
                ret.clear();
                int s = cand.at(i);
                for (int j=0; j<5; j++) {
                    ret.push_back(s+j);
                }
                createOnePiece(ret);
            }
            
            
            break;
        }
        case 6: {
            for (int i=0; i<20; i++) {
                vector<int> ret = {5*i+4};
                createOnePiece(ret);
            }
            
            
            break;
        }
        case 7: {
            int trial = 0;
            while (_pieces.size()<8 && trial<100) {
                trial++;
                auto ret = pickCross();
                if (ret.size()>0) createOnePiece(ret);
            }
            
            break;
        }
        case 8: {
            vector<int> cand, ret;
            for (int i=0; i<10; i+=2) {
                for (int j=0; j<10; j+=2) {
                    cand.push_back(i*10+j);
                }
            }
            
            random_shuffle(cand.begin(), cand.end());
            
            for (int i=0; i<17; i++) {
                ret.clear();
                int s = cand.at(i);
                
                ret.push_back(s);
                ret.push_back(s+1);
                ret.push_back(s+10);
                ret.push_back(s+11);
            
                createOnePiece(ret);
            }
            
            
            break;
        }
        case 9: {
            
            
            
            for (int i=0; i<8; i++) {
                auto ret = pickNumbers(6);
                if (ret.size()>0) createOnePiece(ret);
                
            }
            
            break;
        }
        case 10: {
            for (int i=1; i<100; i+=2) {
                vector<int> ret = {i};
                createOnePiece(ret);
            }
            
            
            break;
        }
        case 11: {
            for (int i=0; i<100; i+=2) {
                vector<int> ret = {i};
                createOnePiece(ret);
            }
            
            
            break;
        }
        case 12: {
            
            int trial = 0;
            while (_pieces.size()<45 && trial<1000) {
                trial++;
                int w = random(1, 2);
                int h = (w==2) ? 1 : 2;
                
                auto ret = pickRect(w, h);
                if (ret.size()>0) createOnePiece(ret);
            }
            
            break;
        }
        case 13: {
            for (int i=0; i<100; i++) {
                vector<int> ret = {i};
                createOnePiece(ret);
            }
            
            
            break;
        }
        default:
            break;
            
    }
    
    
    float pieceAreaX = _gameBoard->getPositionX()*2 + _gameBoard->getContentSize().width;
    
    for (auto p : _pieces) {
        auto pSize = p->getContentSize();
        p->setPosition(Vec2(random(pieceAreaX, gameSize.width-pSize.width-unitSize.width), random(0.f, gameSize.height-pSize.height)));

        setTargetForPiece(p);
        
        p->setColor(_colors.at(_currentColorIndex++));
        if (_currentColorIndex>=_colors.size()) {
         
            random_shuffle(_colors.begin(), _colors.end());
            _currentColorIndex = 0;
        }
        
        _gameNode->addChild(p);
    }

    
}

void HundredPuzzleScene::setTargetForPiece(HundredPiece *piece)
{
    piece->_targetPos = Vec2(_gameBoard->getPositionX() + boardMargin.x + unitSize.width*(piece->_originNum%10),
                         _gameBoard->getPositionY() + boardMargin.y + unitSize.height*(9-piece->_originNum/10));
}



vector<int> HundredPuzzleScene::pickNumbers(int num)
{
    vector<int> ret, border;
    
    int seed;
    
    do {
        seed = random(0, 99);
    } while (_picked[seed]);
    
    bool borderPicked[100], numberPicked[100];
    
    for (int i=0; i<100; i++) {
        borderPicked[i] = false;
        numberPicked[i] = false;
    }
    
    numberPicked[seed] = true;
    ret.push_back(seed);
    
    
    auto checkBorder = [this, &numberPicked, &borderPicked, &border](int now, int dx, int dy) {
        int x = now % 10;
        int y = now / 10;
        int nx = x+dx;
        int ny = y+dy;
        if (nx<0 || nx>9 || ny<0 || ny>9) return;
        int nn = ny*10+nx;
        
        if (numberPicked[nn]) return;
        if (borderPicked[nn]) return;
        if (_picked[nn]) return;
        
        borderPicked[nn] = true;
        border.push_back(nn);
        return;
    };
    
    do {
        border.clear();
        
        for (auto p : ret) {
            checkBorder(p, 1, 0);
            checkBorder(p, -1, 0);
            checkBorder(p, 0, 1);
            checkBorder(p, 0, -1);
        }
        
        if (border.size()==0) return ret;
        
        int delta = random(1, MIN((int)border.size(), num-(int)ret.size()));
        
        random_shuffle(border.begin(), border.end());
        for (int i=0; i<delta; i++) {
            int n = border[i];
            numberPicked[n] = true;
            ret.push_back(n);
        }
        
        for (int i=0; i<100; i++) borderPicked[i] = false;
        
    } while (ret.size()<num);
    
    
    return ret;
    
    
    
    
    
    
}


vector<int> HundredPuzzleScene::pickRect(int w, int h)
{
    
    vector<int> ret;
    
    
    
    int x, y, p;
    
    do {
        x = random(0, 10-w);
        y = random(0, 10-h);
        p = x+y*10;
    } while (_picked[p]);
    
    
    for (int i=x; i<x+w; i++) {
        for (int j=y; j<y+h; j++) {
            p = i+j*10;
            if (p>=100) {
                return vector<int>();
                
            }
            if (_picked[p]) return vector<int>();
            ret.push_back(p);
        }
    }
    

    return ret;
    
    
}

vector<int> HundredPuzzleScene::pickCross()
{
    
    vector<int> ret;
    
    
    
    int x, y, p;
    
    do {
        x = random(1, 8);
        y = random(1, 8);
        p = x+y*10;
    } while (_picked[p]);
    
    vector<Point> diffs = { Point(0, 0), Point(-1, 0), Point(1, 0), Point(0, -1), Point(0, 1) };
    
    for (auto d : diffs) {
        p = (x+d.x)+(y+d.y)*10;
        
        if (p>=100 || _picked[p]) return vector<int>();
        ret.push_back(p);
    }
    
    
    return ret;
    
    
}

void HundredPuzzleScene::fillBoard()
{
    for (int i=0; i<100; i++) {
        if (!_picked[i]) {
            vector<int> ret = {i};
            auto piece = HundredPiece::create();
            piece->setPieces(ret);
            piece->setColor(Color3B::BLACK);
            setTargetForPiece(piece);
            piece->setSnapped(true);
            
            _gameNode->addChild(piece);
        }
    }
}

////////////////////

bool HundredPiece::init()
{

    if (!Node::init()) return false;

    onTouched = nullptr;
    onSnapped = nullptr;
    
    _snapped = false;
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        if (_snapped) return false;
        
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        auto box = this->getBoundingBox();
        
        if (this->getBoundingBox().containsPoint(pos)) {


            pickEffect().play();
            
            this->setPicked(true);
            
            this->getParent()->reorderChild(this, this->getLocalZOrder());
//            this->retain();
//            this->removeFromParent();
//            P->addChild(this);
//            this->release();
            
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
        
        if (_targetPos.distance(getPosition())<snapRadiusOnMoved) {

            setPicked(false);

            setSnapped(true);
            

        }
    };
    
    listener->onTouchEnded = [this](Touch* T, Event* E) {
        if (_snapped) return;
        setPicked(false);
        if (onTouched) { onTouched(); }

        auto tp = _targetPos;
        auto cp = getPosition();
        
        if (_targetPos.distance(getPosition())<snapRadiusOnEnded) {
            setSnapped(true);

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
    
    
    _shadow = Node::create();
    addChild(_shadow);
    
    _body = Node::create();
    addChild(_body);
    

    
    _colorMask = Node::create();
    addChild(_colorMask);
    
    _face = Node::create();
    addChild(_face);
    

    
    return true;
    

}

void HundredPiece::setPieces(vector<int> &numbers)
{
    int minY = 10;
    int minX = 10;
    int maxY = -1;
    int maxX = -1;
    

    for (auto n : numbers) {
        int x = n%10;
        int y = n/10;
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }
    
    auto cSize = Size(unitSize.width*(maxX-minX+1), unitSize.height*(maxY-minY+1));
    setContentSize(cSize);
    
    
    
    
    _originNum = maxY*10 + minX;
    
//    auto draw = DrawNode::create();
//    addChild(draw);
//    
//    draw->drawRect(Vec2::ZERO, getContentSize(), Color4F::RED);
    
    auto addSprite = [&](Vec2 p, Node *node, string file) {
        auto sprite = Sprite::create(file);
        sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        sprite->setPosition(p+Vec2(0, -20));
        node->addChild(sprite);
        
        sprite->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    };
    
    for (auto n : numbers) {
        int x = n%10;
        int y = n/10;
        Vec2 p0 = Vec2((x-minX)*unitSize.width, (maxY-y)*unitSize.height);
        
        //draw->drawSolidRect(p0, p0+unitSize, Color4F::GRAY);
       
        addSprite(p0, _colorMask, "HundredPuzzle/hundredpuzzle_block_middle_1_surface.png");
        addSprite(p0, _face, "HundredPuzzle/hundredpuzzle_block_middle_2_texture.png");
        addSprite(p0, _body, "HundredPuzzle/hundredpuzzle_block_middle_3_depth.png");
        addSprite(p0, _shadow, "HundredPuzzle/hundredpuzzle_block_middle_4_shadow.png");
        
        
        
        auto l = TodoUtil::createLabel(TodoUtil::itos(n+1), 75, Size::ZERO, defaultFont, Color4B(240, 240, 213, 255));
        l->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        l->setPosition(p0+unitSize/2);
        _face->addChild(l);
        
        
    }
    
//    auto l = TodoUtil::createLabel(TodoUtil::itos(_originNum+1), 50, Size::ZERO, defaultFont, Color4B::GREEN);
//    l->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    l->setPosition(unitSize/2);
//    addChild(l);
    

    
    
}



void HundredPiece::setPicked(bool picked)
{
    Size size = getContentSize();
    
    if (picked) {
        _shadow->setPosition(Vec2(8, -8));
        _colorMask->setPosition(Vec2(0, 8));
        _face->setPosition(Vec2(0, 8));
        _body->setPosition(Vec2(0, 8));

    } else {
        _shadow->setPosition(Vec2::ZERO);
        _colorMask->setPosition(Vec2::ZERO);
        _face->setPosition(Vec2::ZERO);
        _body->setPosition(Vec2::ZERO);
    }
}

void HundredPiece::setColor(cocos2d::Color3B color)
{
    if (color==Color3B::BLACK) {
        _colorMask->setVisible(false);
    } else {
        for (auto c : _colorMask->getChildren()) {
            c->setColor(color);
        }
    }
}

void HundredPiece::setSnapped(bool snapped)
{
    _snapped = snapped;
    
    if (snapped) {
        this->setPosition(_targetPos);
        _shadow->setVisible(false);
        _body->setVisible(false);
        
        if (onSnapped) {
            onSnapped();
        }
    } else {
        _shadow->setVisible(true);
        _body->setVisible(true);
    }
    
}
