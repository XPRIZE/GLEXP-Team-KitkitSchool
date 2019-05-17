//
//  MultiplicationBoardScene.cpp
//
//  Created by harunom on 7/21/18.
//
//


#include "MultiplicationBoardScene.hpp"
#include "Common/Basic/SoundEffect.h"
#include "Common/Sounds/CommonSound.hpp"

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include "ui/CocosGUI.h"

#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"

#include "Utils/TodoUtil.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"


#include "CCAppController.hpp"

using namespace cocos2d::ui;
using namespace std;



namespace MultiplicationBoardSceneSpace
{
    const char* defaultFont = "fonts/mukta-bold.ttf";
    const char* andikaFont = "fonts/mukta-bold.ttf";
    
    const string resourcePath = "MultiplicationBoard/";
    
    const Size gameSize = Size(2560, 1800);
    
    const Color3B numberColor = Color3B(255, 216, 0);
    const Color3B symbolColor = Color3B(0, 181, 8);
    
    const CommonSound CommonSoundPlayer;
    
    SoundEffect pickEffect() { return SoundEffect("Common/Sounds/Effect/picklamp.m4a"); }
    SoundEffect snapEffect() { return SoundEffect("Common/Sounds/Effect/putdownamp.m4a"); }
    SoundEffect solveEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Counting_Win.m4a"); }
    SoundEffect onlampEffect1() { return SoundEffect("Common/Sounds/Effect/turnonlamp_1.m4a"); }
    SoundEffect onlampEffect2() { return SoundEffect("Common/Sounds/Effect/turnonlamp_2.m4a"); }
    SoundEffect onlampEffect3() { return SoundEffect("Common/Sounds/Effect/turnonlamp_3.m4a"); }
    SoundEffect onlampEffect4() { return SoundEffect("Common/Sounds/Effect/turnonlamp_4.m4a"); }
    SoundEffect onlampEffect5() { return SoundEffect("Common/Sounds/Effect/turnonlamp_5.m4a"); }
}

using namespace MultiplicationBoardSceneSpace;


Scene* MultiplicationBoardScene::createScene(string levelID)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MultiplicationBoardScene::create();
    layer->setLevel(TodoUtil::stoi(levelID));
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool MultiplicationBoardScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _isSkip = false;
    
    pickEffect().preload();
    snapEffect().preload();
    solveEffect().preload();
    onlampEffect1().preload();
    onlampEffect2().preload();
    onlampEffect3().preload();
    onlampEffect4().preload();
    onlampEffect5().preload();
    
    auto winSize = getContentSize();
    auto gameScale = winSize.height / gameSize.height;
    
    auto bg = Sprite::create(resourcePath+"bg.png");
    auto bgSize = bg->getContentSize();
    float bgScale = MAX(winSize.width/bgSize.width, winSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(winSize/2);
    addChild(bg);

    
    _gameNode = Node::create();
    _gameNode->setContentSize(gameSize);
    _gameNode->setScale(gameScale);
    _gameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameNode->setPosition(winSize.width / 2, winSize.height / 2);
    addChild(_gameNode);

    //기준 해상도 가로너비보다 단말기 가로너비가 작으면 축소시키자
    if (winSize.width < gameSize.width)
    {
        _gameNode->setScale(winSize.width / _gameNode->getContentSize().width);
    }
    

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
            if (e == Widget::TouchEventType::ENDED)
            {
                if (_isSkip == false)
                {
                    for (auto piece : _multiplePieces)
                    {
                        if (piece->_snapped == false)
                        {
                            _isSkip = true;
                            piece->snapTarget();
                        }
                    }
                }
            }
        });
        
    }

    loadDurationsheet();
    
    return true;
}

void MultiplicationBoardScene::onEnter()
{
    Layer::onEnter();
}

void MultiplicationBoardScene::onEnterTransitionDidFinish()
{

    onStart();

}

void MultiplicationBoardScene::onStart()
{
    loadData(_currentLevel, &_currentSheetNo);
    _currentBoardIndex = 0;
    
    _progressBar->setMax(_boards.size());
    
    showBoard(_currentBoardIndex);
}


void MultiplicationBoardScene::showBoard(int index)
{
    _progressBar->setCurrent(index+1, false);
    createBoard(index);
    
}

void MultiplicationBoardScene::createBoard(int index)
{
    _gameNode->removeAllChildren();
    
    _multiplePieces.clear();
    _guides.clear();
    _numSnappedPieces = 0;
    
    //공용파티클
    createShiningParticle();
    
    //수식 노드 추가
    _expressionNode = Node::create();
    _gameNode->addChild(_expressionNode);
    
    //데이터 읽어오기
    auto info = _boards.at(index);
    _multiplicand = info.multiplicand;
    _multiplier = info.multiplier;
    _product = info.product;
    
    //게임 보드 셋팅
    auto gameBoard = Sprite::create(resourcePath + "window.png");
    Size boardSize = gameBoard->getContentSize();
    auto boardPos = Vec2(0, _gameNode->getContentSize().height - 166);
    
    gameBoard->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    gameBoard->setPosition(boardPos);
    _gameNode->addChild(gameBoard);
    
    //별
    auto star = Sprite::create(resourcePath + "star.png");
    star->setPosition(boardPos.x + 1254, boardPos.y - 804);
    _gameNode->addChild(star, 10000);
    
    star = Sprite::createWithTexture(star->getTexture());
    star->setScale(0.25, 0.25);
    star->setPosition(boardPos.x + 1474, boardPos.y - 1032);
    _gameNode->addChild(star, 10000);
    
    star = Sprite::createWithTexture(star->getTexture());
    star->setScale(0.25, 0.25);
    star->setPosition(boardPos.x + 452, boardPos.y - 614);
    _gameNode->addChild(star, 10000);

    star = Sprite::createWithTexture(star->getTexture());
    star->setScale(0.5, 0.5);
    star->setOpacity(255 * 0.42);
    star->setPosition(boardPos.x + 1540, boardPos.y - 288);
    _gameNode->addChild(star, 10000);

    star = Sprite::createWithTexture(star->getTexture());
    star->setScale(0.5, 0.5);
    star->setOpacity(255 * 0.34);
    star->setPosition(boardPos.x + 836, boardPos.y - 438);
    _gameNode->addChild(star, 10000);

    star = Sprite::createWithTexture(star->getTexture());
    star->setScale(0.5, 0.5);
    star->setOpacity(255 * 0.49);
    star->setPosition(boardPos.x + 566, boardPos.y - 1100);
    _gameNode->addChild(star, 10000);

    star = Sprite::createWithTexture(star->getTexture());
    star->setScale(0.5, 0.5);
    star->setPosition(boardPos.x + 972, boardPos.y - 488);
    _gameNode->addChild(star, 10000);

    
    //보드 그림자
    auto boardLight = Sprite::create(resourcePath + "bg_light.png");

    //기준 해상도 가로너비보다 단말기 가로너비가 작으면 게임노드를 축소시켰다. 그림자는 그만큼 확대해야함.
    if (getContentSize().width < gameSize.width)
    {
        boardLight->setScale(_gameNode->getContentSize().width / getContentSize().width);
        boardLight->setContentSize(Size(getContentSize().width, boardLight->getContentSize().height));
        boardLight->setPosition(_gameNode->getContentSize().width/2, (getContentSize().height * (_gameNode->getContentSize().width / getContentSize().width)) - boardLight->getContentSize().height/2);
    }
    else
    {
        boardLight->setContentSize(Size(getContentSize().width, boardLight->getContentSize().height));
        boardLight->setPosition(_gameNode->getContentSize().width/2, getContentSize().height - boardLight->getContentSize().height/2);
    }
    
    _gameNode->addChild(boardLight, 1000);

    //줄
    auto boardString = Sprite::create(resourcePath + "string.png");
    boardString->setPosition(Vec2(boardSize.width/2, boardSize.height - 186));
    
    gameBoard->addChild(boardString);
    
    //보드 왼쪽/상단 숫자
    Vec2 multiplicandNumberPos = Vec2(84, boardPos.y - 358 - 46 + 166 + 10);
    Vec2 multiplierNumberPos = Vec2(294 - (154/2), boardPos.y - 20);
    
    for (int i = 0; i < 9; i++)
    {
        //왼쪽 숫자
        auto boardNumber = Label::createWithCharMap(resourcePath + "number.png", 144, 126, '1');
        Size numberSize = boardNumber->getContentSize();
        
        boardNumber->setString(TodoUtil::itos(i+1));
        boardNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        boardNumber->setPosition(Vec2(multiplicandNumberPos.x, multiplicandNumberPos.y - (i * 122)));
        boardNumber->setName("boardMultiplicand"+TodoUtil::itos(i+1));
        boardNumber->setVisible(false);
        
        //현재 스테이지에 해당하는것만 보여주자
        if (_multiplicand == i+1)
            boardNumber->setVisible(true);
        
        _gameNode->addChild(boardNumber, 1100);

        //상단 숫자
        //상단 백숫자
        auto boardNumberBack = Label::createWithTTF(TodoUtil::itos(i+1), defaultFont, 117);
        boardNumberBack->setColor(Color3B(38, 33, 33));
        boardNumberBack->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        boardNumberBack->setPosition(Vec2(multiplierNumberPos.x + (i * 154) + 40, multiplierNumberPos.y - 10));
        boardNumberBack->setVisible(true);
        
        //현재 스테이지에 해당하지 않는것만 보여주자
        if (_multiplier == i+1)
            boardNumberBack->setVisible(false);
        
        _gameNode->addChild(boardNumberBack, 1100);
        
        //상단 전면숫자
        boardNumber = Label::createWithCharMap(resourcePath + "number.png", 144, 126, '1');
        numberSize = boardNumber->getContentSize();
        
        boardNumber->setString(TodoUtil::itos(i+1));
        boardNumber->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        boardNumber->setPosition(Vec2(multiplierNumberPos.x + i * 154, multiplierNumberPos.y));
        boardNumber->setName("boardMultiplier"+TodoUtil::itos(i+1));
        boardNumber->setVisible(false);
        
        //현재 스테이지에 해당하는것만 보여주자
        if (_multiplier == i+1)
            boardNumber->setVisible(true);
        
        _gameNode->addChild(boardNumber, 1100);
    }
    
    //보드 왼쪽상단 X표시
    auto boardX = Sprite::create(resourcePath + "x.png");
    boardX->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    boardX->setPosition(Vec2(106, boardPos.y - 20));
    boardX->setName("boardX");
    
    _gameNode->addChild(boardX, 1100);

    
    //전구 가이드 셋팅
    Node *bulbGuide;
    for (int i = 0; i < _multiplier; i++)
    {
        Sprite *bulb_guide = Sprite::create(resourcePath + "bulb_guide.png");
        
        bulbGuide = Node::create();
        Size guideSize = Size(bulb_guide->getContentSize().width, (bulb_guide->getContentSize().height + 26) * _multiplicand);
        bulbGuide->setContentSize(guideSize);
        
        for (int j = 0; j < _multiplicand; j++)
        {
            bulb_guide = Sprite::createWithTexture(bulb_guide->getTexture());
            bulb_guide->setOpacity(255 * 0.4);
            bulb_guide->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            bulb_guide->setPosition(Vec2(guideSize.width/2, guideSize.height - (j * (bulb_guide->getContentSize().height + 26))));
            
            bulbGuide->addChild(bulb_guide);
        }
        
        bulbGuide->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        bulbGuide->setPosition(Vec2(294 + (i * 154), _gameNode->getContentSize().height - 358 - 46));
        bulbGuide->setVisible(false);
        
        if (i == 0)
            bulbGuide->setVisible(true);
        
        //게임노드에 추가
        _gameNode->addChild(bulbGuide);
        _guides.push_back(bulbGuide);
    }

    //피스 셋팅
    auto winSize = getContentSize();

    //피스 생성
    for (int i = 0; i < _multiplier; i++)
    {
        auto piece = MultiplicationBoardPiece::create();
        piece->setTexture(_multiplicand, i+1);

        //초기 위치
        auto pSize = piece->getContentSize();
        int pPosX = 0;
        int pPosY = 0;
        
        int loopCount = 0;
        
        while (true)
        {
            pPosX = random(1700 + (int)(pSize.width/2), (int)gameSize.width - 156 - (int)(pSize.width/2));
            pPosY = random(boardPos.y - 100 - (int)(pSize.height/2), boardPos.y - 100 - 1400 + (int)(pSize.height/2));
            
            piece->_originPos = Point(pPosX, pPosY);
            piece->setPosition(piece->_originPos);

            //겹치는지 체크
            bool isIntersect = false;
            for (auto p : _multiplePieces)
            {
                //5개부터는 가로는 3/4 세로는 1/3 겹칠 수 있도록 하자
                int divideAreaWidth = (float)(_multiplier >= 5 ? 4 : 1);
                int divideAreaHeight = (float)(_multiplier >= 5 ? 1.5 : 1);
                Rect a = Rect(p->getPosition().x , p->getPosition().y, p->getContentSize().width/divideAreaWidth, p->getContentSize().height/divideAreaHeight);
                Rect b = Rect(piece->getPosition().x, piece->getPosition().y, piece->getContentSize().width/divideAreaWidth,  piece->getContentSize().height/divideAreaHeight);
                
                if (a.intersectsRect(b))
                {
                    isIntersect = true;
                    break;
                }
            }
            
            if (isIntersect == false)
                break;
            
            //루프가 50번 넘으면 빠져나가자
            if (loopCount++ > 50)
                break;
        }
        
        //루프가 50번 넘으면 모두 초기화하고 피스 새로 생성
        if (loopCount > 50)
        {
            i = -1;
            _multiplePieces.clear();
            continue;
        }
        
        piece->_touchEnabled = true;
        
        piece->onPicked = [this, piece]() {
            
            //가이드 100%로 보이기
            for (auto guide : _guides)
            {
                for (auto guideBulb : guide->getChildren())
                {
                    guideBulb->setOpacity(255);
                }
            }
            
            //중간 수식 만들기
            if (getCurGuide()->isVisible())
            {
                makeExpression(true, false);
            }
            else
            {
                //가이드가 안보이는 상태면 연출 진행중이므로 연출 종료후 하도록 하자
                this->runAction(Sequence::create(
                                    DelayTime::create(getDuration("d_"+ (TodoUtil::itos(_multiplicand*_numSnappedPieces)) + ".m4a")),
                                    CallFunc::create([this, piece](){
                                        //이때까지 픽상태라면...
                                        if (piece->_picked)
                                            makeExpression(true, false);
                                    }),
                                    nullptr));
            }
            
            //게임 로그 남기기
            string workPath = [this] {
                stringstream SS;
                SS << "/" << "MultiplicationBoard";
                SS << "/" << "level-" << _currentLevel << "-" << _currentSheetNo;
                SS << "/" << "work-" << _currentBoardIndex;
                return SS.str();
            }();
            
            StrictLogManager::shared()->game_Peek_Answer("MultiplicationBoard", workPath, "", "");

        };
        
        piece->onSnapped = [this, piece]() {
            snapEffect().play();
            
            //가이드 40%로 보이기
            for (auto guide : _guides)
            {
                for (auto guideBulb : guide->getChildren())
                {
                    guideBulb->setOpacity(255 * 0.4);
                }
            }

            //현재 가이드 사라지자
            _guides.at(_numSnappedPieces)->setVisible(false);
            
            _numSnappedPieces++;
            
            //스킵 기능
            if (UserManager::getInstance()->isDebugMode())
            {
                if (_isSkip)
                {
                    if (_numSnappedPieces >= _multiplePieces.size())
                    {
                        onBoardComplete();
                        _isSkip = false;
                    }
                    else
                    {
                        //가이드 표시
                        if (_numSnappedPieces < _multiplier)
                            _guides.at(_numSnappedPieces)->setVisible(true);
                        
                        //다음 가이드를 피스 타겟 노드로 설정
                        for (auto p : _multiplePieces)
                            p->_targetNode = _guides.at(_numSnappedPieces);
                        
                        //그리고 바로 다음 피스 스냅 시켜버리자
                        for (auto piece : _multiplePieces)
                        {
                            if (piece->_snapped == false)
                            {
                                piece->snapTarget();
                            }
                        }
                    }
                    return;
                }
            }
            
            //전구 및 수식 연출
            auto seq = Sequence::create(DelayTime::create(0.2),
                CallFunc::create([this, piece](){
                    //전구 켜지는 연출
                    int aniCount = 0;
                    vector<Sprite*> onLights;
                    
                    while (true)
                    {
                        int rand = random(0, _multiplicand-1);
                        
                        Sprite *light = (Sprite*)piece->_bulb_light->getChildByName("bulbLight"+TodoUtil::itos(rand));
                        Sprite *bulb = (Sprite*)piece->_bulb->getChildByName("bulb"+TodoUtil::itos(rand));
                        
                        if (find(onLights.begin(), onLights.end(), light) != onLights.end())
                            continue;
                        
                        auto seqLight = Sequence::create(DelayTime::create(0.1f * aniCount),
                                                         CallFunc::create([this, light, bulb](){ light->setVisible(true); bulb->setVisible(false); playOnLampSound(); }),
                                                         DelayTime::create(0.1f * (aniCount + 1)),
                                                         CallFunc::create([this, light, bulb](){ light->setVisible(false); bulb->setVisible(true); }),
                                                         nullptr);
                        
                        light->runAction(seqLight);
                        onLights.push_back(light);
                        
                        aniCount++;
                        
                        if (aniCount >= _multiplicand)
                            break;
                        
                    }
                }),
                DelayTime::create((0.1f * _multiplicand) + 0.1f),
                CallFunc::create([this, piece](){
                    //전구 끄기
                    piece->setLightOff();
                
                    //마지막 숫자 표시
                    for (auto p : _multiplePieces)
                    {
                        Sprite *back = (Sprite*)p->_bulb->getChildByName("bulbNumberBack");
                        back->setVisible(false);
                        
                        Label *num = (Label*)p->_bulb->getChildByName("bulbNumber");
                        num->setVisible(false);
                        num->setString(TodoUtil::itos(_multiplicand*_numSnappedPieces));
                        
                        if (piece == p)
                        {
                            back->setVisible(true);
                            num->setVisible(true);
                        }
                    }

                    //숫자 사운드 플레이                
                    CommonSoundPlayer.cardinalNumber(_multiplicand*_numSnappedPieces).play();
                
                    //결과 수식 보여주기
                    makeExpression(false, true);
                }),
                DelayTime::create(getDuration("d_"+ (TodoUtil::itos(_multiplicand*_numSnappedPieces)) + ".m4a")),
                CallFunc::create([this, piece](){
                    //완료되었다면...
                    if (_numSnappedPieces >= _multiplePieces.size())
                    {
                        auto seqClear = Sequence::create(
                            DelayTime::create(1.0f),
                            CallFunc::create([this, piece](){
                                //완성된 수식 만들기
                                makeCompleteExpression();

                                auto seqExpress = Sequence::create(
                                            //곱해지는 수
                                           CallFunc::create([this](){
                                                auto multiplicand = _gameNode->getChildByName("boardMultiplicand"+TodoUtil::itos(_multiplicand));
                                                auto expMultiplicand = _expressionNode->getChildByName("expMultiplicand");
                                                auto movePos = Vec2(_expressionNode->getPositionX() - (_expressionNode->getContentSize().width/2) + expMultiplicand->getPositionX(), _expressionNode->getPositionY());
                                    
                                                multiplicand->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                                multiplicand->setPosition(Vec2(multiplicand->getPositionX(), multiplicand->getPositionY() - (multiplicand->getContentSize().height/2)));
                                                multiplicand->runAction(Sequence::create(
                                                                            ScaleTo::create(0.2, expMultiplicand->getContentSize().height/multiplicand->getContentSize().height),
                                                                            Spawn::create(EaseOut::create(MoveTo::create(0.3, movePos), 0.3), EaseOut::create(FadeOut::create(0.3), 0.3), nullptr),
                                                                            nullptr));
                                            }),
                                           DelayTime::create(0.5),
                                           CallFunc::create([this](){
                                                auto expMultiplicand = _expressionNode->getChildByName("expMultiplicand");
                                                expMultiplicand->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                                expMultiplicand->setVisible(true);
                                                expMultiplicand->runAction(Sequence::create(ScaleTo::create(0.2, 1.5), ScaleTo::create(0.2, 1), nullptr));
                                                CommonSoundPlayer.cardinalNumber(_multiplicand).play();
                                            }),
                                           DelayTime::create(getDuration("d_" + TodoUtil::itos(_multiplicand) + ".m4a") * 0.5),

                                            //곱하기 기호
                                           CallFunc::create([this](){
                                                auto x = _gameNode->getChildByName("boardX");
                                                auto expX = _expressionNode->getChildByName("expX");
                                                auto movePos = Vec2(_expressionNode->getPositionX() - (_expressionNode->getContentSize().width/2) + expX->getPositionX(), _expressionNode->getPositionY());
            
                                                x->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                                x->setPosition(Vec2(x->getPositionX() + (x->getContentSize().width/2), x->getPositionY() - (x->getContentSize().height/2)));
                                                x->runAction(Sequence::create(
                                                                  ScaleTo::create(0.2, expX->getContentSize().height/x->getContentSize().height),
                                                                  Spawn::create(EaseOut::create(MoveTo::create(0.3, movePos), 0.3), EaseOut::create(FadeOut::create(0.3), 0.3), nullptr),
                                                                  nullptr));
                                            }),
                                           DelayTime::create(0.5),
                                           CallFunc::create([this](){
                                                auto expX = _expressionNode->getChildByName("expX");
                                                expX->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                                expX->setVisible(true);
                                                expX->runAction(Sequence::create(ScaleTo::create(0.2, 1.5), ScaleTo::create(0.2, 1), nullptr));
                                                CommonSoundPlayer.times().play();
                                            }),
                                           DelayTime::create(getDuration("times.m4a") * 0.5),

                                           //곱하는 수
                                           CallFunc::create([this](){
                                                auto multiplier = _gameNode->getChildByName("boardMultiplier"+TodoUtil::itos(_multiplier));
                                                auto expMultiplier = _expressionNode->getChildByName("expMultiplier");
                                                auto movePos = Vec2(_expressionNode->getPositionX() - (_expressionNode->getContentSize().width/2) + expMultiplier->getPositionX(), _expressionNode->getPositionY());
            
                                                multiplier->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                                multiplier->setPosition(Vec2(multiplier->getPositionX() + (multiplier->getContentSize().width/2), multiplier->getPositionY() - (multiplier->getContentSize().height/2)));
                                                multiplier->runAction(Sequence::create(
                                                                           ScaleTo::create(0.2, expMultiplier->getContentSize().height/multiplier->getContentSize().height),
                                                                           Spawn::create(EaseOut::create(MoveTo::create(0.3, movePos), 0.3), EaseOut::create(FadeOut::create(0.3), 0.3), nullptr),
                                                                           nullptr));
                                            }),
                                           DelayTime::create(0.5),
                                           CallFunc::create([this](){
                                                auto expMultiplier = _expressionNode->getChildByName("expMultiplier");
                                                expMultiplier->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                                expMultiplier->setVisible(true);
                                                expMultiplier->runAction(Sequence::create(ScaleTo::create(0.2, 1.5), ScaleTo::create(0.2, 1), nullptr));
                                                CommonSoundPlayer.cardinalNumber(_multiplier).play();
                                            }),
                                           DelayTime::create(getDuration("d_" + TodoUtil::itos(_multiplier) + ".m4a") * 0.5),

                                            //이퀄 기호
                                            CallFunc::create([this](){
                                                auto expEqual = _expressionNode->getChildByName("expEqual");
                                                expEqual->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                                expEqual->setVisible(true);
                                                expEqual->runAction(Sequence::create(ScaleTo::create(0.2, 1.5), ScaleTo::create(0.2, 1), nullptr));
                                                CommonSoundPlayer.equals().play();
                                            }),
                                            DelayTime::create(getDuration("equals.m4a") * 0.5),

                                           //결과 수
                                           CallFunc::create([this, piece](){
                                                auto result = Label::createWithTTF(TodoUtil::itos(_multiplicand * _multiplier), defaultFont, 80);
                                                result->setColor(numberColor);
                                                result->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                                result->setPosition(Vec2(piece->getPosition().x + 20, piece->getPosition().y - (piece->getContentSize().height/2) + 60));
                                                _gameNode->addChild(result);
                                    
                                                auto expResult = _expressionNode->getChildByName("expResult");
                                                auto movePos = Vec2(_expressionNode->getPositionX() - (_expressionNode->getContentSize().width/2) + expResult->getPositionX(), _expressionNode->getPositionY());
                                    
                                                piece->_bulb->getChildByName("bulbNumber")->setVisible(false);
                                    
                                                result->runAction(Sequence::create(
                                                                       ScaleTo::create(0.2, expResult->getContentSize().height/result->getContentSize().height),
                                                                       Spawn::create(EaseOut::create(MoveTo::create(0.3, movePos), 0.3), EaseOut::create(FadeOut::create(0.3), 0.3), nullptr),
                                                                       nullptr));
                                            }),
                                            DelayTime::create(0.5),
                                            CallFunc::create([this](){
                                                auto expResult = _expressionNode->getChildByName("expResult");
                                                expResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                                expResult->setVisible(true);
                                                expResult->runAction(Sequence::create(ScaleTo::create(0.2, 1.5), ScaleTo::create(0.2, 1), nullptr));
                                                CommonSoundPlayer.cardinalNumber(_multiplicand * _multiplier).play();
                                            }),
                                            DelayTime::create(getDuration("d_" + TodoUtil::itos(_multiplicand * _multiplier) + ".m4a") * 0.5),
                                                                   nullptr);
                            
                                this->runAction(seqExpress);
                            }),
                            DelayTime::create(getCompleteExpressionAniTime()),
                            CallFunc::create([this, piece](){
                            
                                //성공 사운드 플레이
                                solveEffect().play();

                                //파티클 연출 시작
                                startParticle();
                            
                                //전체 연출 시작이므로 마지막 숫자백그라운드 이미지 감추자
                                Sprite *back = (Sprite*)piece->_bulb->getChildByName("bulbNumberBack");
                                back->setVisible(false);
                            
                                //전구 켜지는 연출
                                int aniCount = 0;
                            
                                while (true)
                                {
                                    int rand = random(0, _multiplicand*_numSnappedPieces-1);
                                    
                                    int pindex = rand / _multiplicand;
                                    int bindex = rand % _multiplicand;
                                    
                                    auto p = _multiplePieces.at(pindex);
                                    
                                    Sprite *light = (Sprite*)p->_bulb_light->getChildByName("bulbLight"+TodoUtil::itos(bindex));
                                    Sprite *bulb = (Sprite*)p->_bulb->getChildByName("bulb"+TodoUtil::itos(bindex));
                                    
                                    auto seqLight = Sequence::create(DelayTime::create(0.1f * aniCount),
                                                                     CallFunc::create([this, light, bulb](){ light->setVisible(true); bulb->setVisible(false); playOnLampSound(); }),
                                                                     DelayTime::create(0.1f * (aniCount + 1)),
                                                                     CallFunc::create([this, light, bulb](){ light->setVisible(false); bulb->setVisible(true); }),
                                                                     nullptr);
                                    
                                    light->runAction(seqLight);
                                    
                                    aniCount++;
                                    
                                    if (aniCount >= (_multiplicand*_numSnappedPieces * 5))
                                        break;
                                    
                                }
                            
                                //기존 수식 숨기기
                                for (auto exp : _expressionNode->getChildren())
                                {
                                    exp->runAction(FadeOut::create(0.5f));
                                }
                            }),
                            DelayTime::create(0.5f),
                            CallFunc::create([this, piece](){
                            
                                //최종 수식 까만색으로 띄우자
                                auto lastExpression = Label::createWithTTF(TodoUtil::itos(_multiplicand) + " x " + TodoUtil::itos(_numSnappedPieces) + " = " + TodoUtil::itos(_multiplicand * _numSnappedPieces), andikaFont, 120);
                                lastExpression->setName("lastExpression");
                                lastExpression->setColor(Color3B(0, 0, 0));
                                lastExpression->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                lastExpression->setPosition(Vec2(_gameNode->getContentSize().width/2, 140));
                            
                                _gameNode->addChild(lastExpression, 5500);
                            
                                lastExpression->setOpacity(0);
                                lastExpression->runAction(FadeIn::create(0.5f));
                            
                            }),
                            DelayTime::create(1.5f),
                            CallFunc::create([this](){
                                //파티클 스탑
                                stopParticle();
                            
                                //검정수식도 점점 사라지자
                                auto lastExpression = _gameNode->getChildByName("lastExpression");
                                lastExpression->runAction(FadeOut::create(0.5f));
                            
                            }),
                            CallFunc::create([this](){ onBoardComplete(); }),
                            nullptr);
                        this->runAction(seqClear);
                    }
                    else
                    {
                        //가이드 표시
                        if (_numSnappedPieces < _multiplier)
                            _guides.at(_numSnappedPieces)->setVisible(true);
                        
                        //원래 수식 보여주기
                        makeExpression(false, false);
                        
                        //다음 가이드를 피스 타겟 노드로 설정
                        for (auto p : _multiplePieces)
                            p->_targetNode = _guides.at(_numSnappedPieces);
                        
                    }

                }),
                nullptr);
            this->runAction(seq);
        };

        piece->onLaid = [this, piece] {
            
            snapEffect().play();
            
            //가이드 40%로 보이기
            for (auto guide : _guides)
            {
                for (auto guideBulb : guide->getChildren())
                {
                    guideBulb->setOpacity(255 * 0.4);
                }
            }

            //수식 되돌리기
            makeExpression(false, false);
            
        };

        //첫 가이드를 피스 타겟 노드로 설정
        piece->_targetNode = _guides.at(0);
        
        _multiplePieces.push_back(piece);
    }
    
    //피스를 모두 게임노드에 추가
    for (auto piece : _multiplePieces)
    {
        _gameNode->addChild(piece);
    }
}

void MultiplicationBoardScene::onBoardComplete()
{
        auto e = _boards[_currentBoardIndex];
    
        _progressBar->setCurrent(_currentBoardIndex+1, true);
        _currentBoardIndex++;
    
        auto winSize = getContentSize();
    
        if (_currentBoardIndex>=_boards.size())
        {
            auto CP = CompletePopup::create();
            CP->show(1.5f, [this] {
                CCAppController::sharedAppController()->handleGameComplete(1);
            });
        }
        else
        {
            auto seq = Sequence::create(DelayTime::create(1.0),
                                        CallFunc::create([this](){ showBoard(_currentBoardIndex); }),
                                        nullptr);
            this->runAction(seq);
        }
}

void MultiplicationBoardScene::makeExpression(bool isPick, bool isSnapped)
{
    _expressionNode->removeAllChildren();

    auto expNumber = Label::createWithTTF(TodoUtil::itos(_multiplicand), andikaFont, 160);
    auto resultNumber = Label::createWithTTF(TodoUtil::itos(_multiplicand * _multiplier), andikaFont, 160);
    auto plusSymbol = Label::createWithTTF("+", andikaFont, 160);
    auto equalSymbol = Label::createWithTTF("=", andikaFont, 160);
    
    int count = _numSnappedPieces + (isPick ? 1 : 0);
    
    //전체 수식 길이 계산
    int expWidth = (expNumber->getContentSize().width * _multiplier); //숫자
    expWidth += (plusSymbol->getContentSize().width * _multiplier-1);//플러스 기호
    expWidth += (resultNumber->getContentSize().width);//결과 숫자
    expWidth += (equalSymbol->getContentSize().width);//이퀄 기호
    
    //곱하는수가 9까지 가면 자리가 부족하므로 gap이 0이고 9보다 작을때마다 10픽셀씩 간격을 넓혀주자
    int expGap = ((9 - _multiplier) * 10);
    expWidth += (_multiplier + (_multiplier + 1)) * expGap;
    
    int expX = 0;
    int expY = expNumber->getHeight() / 2;
    
    //수식 노드 크기 설정
    _expressionNode->setContentSize(Size(expWidth, 1));
    
    for (int i = 0; i < count; i++)
    {
        if (i > 0)
        {
            auto expSymbol = Label::createWithTTF("+", andikaFont, 160);
            expSymbol->setColor(symbolColor);
            expSymbol->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            expSymbol->setPosition(Vec2(expX, expY));
            
            expX += expSymbol->getContentSize().width + expGap;
            
            //픽한것은 40%로 보여주자
            if (isPick && i == count-1)
                expSymbol->setOpacity(255 * 0.4);
            else
                expSymbol->setOpacity(255);
            
            _expressionNode->addChild(expSymbol);
        }
        
        auto expNumber = Label::createWithTTF(TodoUtil::itos(_multiplicand), andikaFont, 160);
        expNumber->setColor(numberColor);
        expNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        expNumber->setPosition(Vec2(expX, expY));
        
        expX += expNumber->getContentSize().width + expGap;

        //픽한것은 40%로 보여주자
        if (isPick && i == count-1)
            expNumber->setOpacity(255 * 0.4);
        else
            expNumber->setOpacity(255);

        _expressionNode->addChild(expNumber);
    }
    
    if (isSnapped && _numSnappedPieces > 1)
    {
        //= 이퀄
        auto expSymbol = Label::createWithTTF("=", andikaFont, 160);
        expSymbol->setName("");
        expSymbol->setColor(symbolColor);
        expSymbol->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        expSymbol->setPosition(Vec2(expX, expY));
        
        expX += expSymbol->getContentSize().width + expGap;
        
        _expressionNode->addChild(expSymbol);
        
        //결과수
        auto expNumber = Label::createWithTTF(TodoUtil::itos(_multiplicand * _numSnappedPieces), andikaFont, 160);
        expNumber->setColor(numberColor);
        expNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        expNumber->setPosition(Vec2(expX, expY));
        
        expX += expNumber->getContentSize().width + expGap;
        
        _expressionNode->addChild(expNumber);
    }
    
    _expressionNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _expressionNode->setPosition(Vec2(_gameNode->getContentSize().width/2, 140));
}

float MultiplicationBoardScene::getCompleteExpressionAniTime()
{
    return 0.5 + 0.5 + 0.5 + 0.5 + ((getDuration("d_"+ TodoUtil::itos(_multiplicand) + ".m4a") + getDuration("times.m4a") + getDuration("d_"+ TodoUtil::itos(_multiplier) + ".m4a") + getDuration("equals.m4a") + getDuration("d_"+ TodoUtil::itos(_multiplicand * _multiplier) + ".m4a")) * 0.5);
}

void MultiplicationBoardScene::makeCompleteExpression()
{
    _expressionNode->removeAllChildren();
    
    //사용될 라벨 생성
    auto multiplicandNumber = Label::createWithTTF(TodoUtil::itos(_multiplicand), andikaFont, 160);
    auto xSymbol = Label::createWithTTF("X", andikaFont, 160);
    auto multiplierNumber = Label::createWithTTF(TodoUtil::itos(_multiplier), andikaFont, 160);
    auto equalSymbol = Label::createWithTTF("=", andikaFont, 160);
    auto resultNumber = Label::createWithTTF(TodoUtil::itos(_multiplicand * _multiplier), andikaFont, 160);
    
    //전체 수식 길이 계산
    int expWidth = (multiplicandNumber->getContentSize().width + xSymbol->getContentSize().width + multiplierNumber->getContentSize().width +
    equalSymbol->getContentSize().width + resultNumber->getContentSize().width) * 2;
    
    int expX = 0;
    int expY = resultNumber->getHeight() / 2;

    //수식 노드 크기 설정
    _expressionNode->setContentSize(Size(expWidth, 1));
    
    //곱해지는수
    multiplicandNumber->setName("expMultiplicand");
    multiplicandNumber->setVisible(false);
    multiplicandNumber->setColor(numberColor);
    multiplicandNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    multiplicandNumber->setPosition(Vec2(expX, expY));
    
    expX += (multiplicandNumber->getContentSize().width * 2);
    
    _expressionNode->addChild(multiplicandNumber);

    //곱하기표시
    xSymbol->setName("expX");
    xSymbol->setVisible(false);
    xSymbol->setColor(symbolColor);
    xSymbol->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    xSymbol->setPosition(Vec2(expX, expY));
    
    expX += (xSymbol->getContentSize().width * 2);
    
    _expressionNode->addChild(xSymbol);
    
    //곱하는수
    multiplierNumber->setName("expMultiplier");
    multiplierNumber->setVisible(false);
    multiplierNumber->setColor(numberColor);
    multiplierNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    multiplierNumber->setPosition(Vec2(expX, expY));
    
    expX += (multiplierNumber->getContentSize().width * 2);
    
    _expressionNode->addChild(multiplierNumber);
    
    //= 이퀄
    equalSymbol->setName("expEqual");
    equalSymbol->setVisible(false);
    equalSymbol->setColor(symbolColor);
    equalSymbol->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    equalSymbol->setPosition(Vec2(expX, expY));
    
    expX += (equalSymbol->getContentSize().width * 2);
    
    _expressionNode->addChild(equalSymbol);

    //결과수
    resultNumber->setName("expResult");
    resultNumber->setVisible(false);
    resultNumber->setColor(numberColor);
    resultNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    resultNumber->setPosition(Vec2(expX, expY));
    
    expX += (resultNumber->getContentSize().width * 2);
    
    _expressionNode->addChild(resultNumber);
    
    _expressionNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _expressionNode->setPosition(Vec2(_gameNode->getContentSize().width/2, 140));
}

Node *MultiplicationBoardScene::getCurGuide()
{
    return _guides.at(_numSnappedPieces);
}

void MultiplicationBoardScene::setLevel(int level)
{
    _currentLevel = level;
}

void MultiplicationBoardScene::loadData(int level, int *workSheetID)
{
    string P = "Games/" + resourcePath + "multiplicationboard_level.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);

    auto data = TodoUtil::readTSV(S);
   
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();

    string LanguageTag;

    int maxWorkSheet = 1;

    //최대 worksheet 값 찾기
    for (int i = 0; i < data.size(); i++)
    {
        //찾으려는 레벨과 다르면 다음으로 통과
        if (TodoUtil::stoi(data[i][1]) != level)
            continue;

        int workSheet = TodoUtil::stoi(data[i][3]);

        if (workSheet > maxWorkSheet)
            maxWorkSheet = workSheet;
    }

    //worksheet 고르기
    *workSheetID = random(1, maxWorkSheet);
    
    _boards.clear();
    
    MultiplicationBoardLevelStruct e;

    int rowIndex = 0;
    while (rowIndex < data.size())
    {
        auto row = data[rowIndex++];
        LanguageTag = row[0];
        e.level = TodoUtil::stoi(row[1]);
        e.stage = TodoUtil::stoi(row[2]);
        e.worksheet = TodoUtil::stoi(row[3]);
        e.multiplicand = TodoUtil::stoi(row[4]);
        e.multiplier = TodoUtil::stoi(row[5]);
        e.product = TodoUtil::stoi(row[6]);
        
        if (e.level == level)
        {
            if (UserManager::getInstance()->isWorksheetTestMode() == false)
            {
                if (e.worksheet == *workSheetID)
                {
                    _boards.push_back(e);
                }
            }
            else
            {
                _boards.push_back(e);
            }
        }
    }
}

void MultiplicationBoardScene::loadDurationsheet() {

    if (_duration.size() != 0) return;
    std::string rawString = cocos2d::FileUtils::getInstance()->getStringFromFile("NumberVoice/Durations.tsv");
    auto data = TodoUtil::readTSV(rawString);

    for (auto row : data) {
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row.size()==1) continue;
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;
        _duration[row[0]] = rowDuration;
        
//        CCLOG("%s, %f", row[0].c_str(), rowDuration);
    }
}

float MultiplicationBoardScene::getDuration(string name) {

    if (_duration.count(name))
        return _duration[name];

    return 2.5;
}

void MultiplicationBoardScene::playOnLampSound()
{
    int index = random(1, 5);
    
    switch (index) {
        case 1:
            onlampEffect1().play();
            break;
        case 2:
            onlampEffect2().play();
            break;
        case 3:
            onlampEffect3().play();
            break;
        case 4:
            onlampEffect4().play();
            break;
        case 5:
            onlampEffect5().play();
            break;
    }
}

void MultiplicationBoardScene::createShiningParticle()
{
    _shiningParticleNode = Node::create();
    
    ParticleSystemQuad* particleEffect = nullptr;
    
    auto createParticleEffect = [&](string name, string plist) {
        particleEffect = ParticleSystemQuad::create(plist);
        particleEffect->setName(name);
        particleEffect->setTotalParticles(120);
        particleEffect->setPosVar(Vec2(300, 100));
        particleEffect->setScale(1.8f);
        particleEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        particleEffect->setPosition(Vec2::ZERO);
        particleEffect->stopSystem();
        _shiningParticleNode->addChild(particleEffect);
    };
    
    createParticleEffect("particle1", "Common/Effects/Particle/shining_particle_blur.plist");
    createParticleEffect("particle2", "Common/Effects/Particle/shining_particle_circle.plist");
    createParticleEffect("particle3", "Common/Effects/Particle/shining_particle_star.plist");
    
    _shiningParticleNode->setPosition(this->getContentSize()/2);
    _gameNode->addChild(_shiningParticleNode, 5000);
}

void MultiplicationBoardScene::startParticle()
{
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle1")->resetSystem();
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle2")->resetSystem();
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle3")->resetSystem();
    
    _shiningParticleNode->setPosition(_expressionNode->getPosition());
}

void MultiplicationBoardScene::stopParticle()
{
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle1")->stopSystem();
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle2")->stopSystem();
    _shiningParticleNode->getChildByName<ParticleSystemQuad*>("particle3")->stopSystem();
}

////////////////////



bool MultiplicationBoardPiece::init()
{
    if (!Node::init()) return false;

    _snapped = false;
    _picked = false;
    _touchEnabled = false;
    
    onPicked = nullptr;
    onSnapped = nullptr;
    onLaid = nullptr;
    
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
            
            this->getParent()->reorderChild(this, this->getLocalZOrder());
            
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
        
        if (getBoundingBox().getMinX() + 30 < 0 || getBoundingBox().getMaxX() - 30 > P->getBoundingBox().getMaxX() ||
            getBoundingBox().getMinY() + 30 < 0 || getBoundingBox().getMaxY() - 30 > P->getBoundingBox().getMaxY())
        {
            this->setPosition(this->getPosition()-delta);
        }
        
        auto tp = _targetPos;
        auto cp = getPosition();

        //현재 가이드가 켜졌을때만 snap
        MultiplicationBoardScene* scene = (MultiplicationBoardScene*)P->getParent();
        
        if (_targetNode->getBoundingBox().containsPoint(getPosition()) && scene->getCurGuide()->isVisible())
        {
            this->snapTarget();
        }
    };
    
    listener->onTouchEnded = [this](Touch* T, Event* E) {
        if (_snapped) return;
        setPicked(false);
        
        auto P = getParent();
        MultiplicationBoardScene* scene = (MultiplicationBoardScene*)P->getParent();
        
        //스냅 영역에 들어오고 현재 가이드가 켜졌을때만 snap
        if (_targetNode->getBoundingBox().containsPoint(getPosition()) && scene->getCurGuide()->isVisible())
        {
            this->snapTarget();
        }
        else
        {
            this->layTarget();
        }
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}


void MultiplicationBoardPiece::setTexture(int multiplicand, int multiplier)
{
    string bulbName[] = {"bulb_g", "bulb_r", "bulb_y"};
    Size bulb_size;
    int height;
    
    _bulb = Node::create();
    _bulb_light = Node::create();
    _shadow = Node::create();
    
    for (int i = 0; i < multiplicand; i++)
    {
        Sprite *bulb = Sprite::create(resourcePath + bulbName[i % (multiplicand % 2 == 0 ? 2 : 3)] + ".png");
        Sprite *bulb_light = Sprite::create(resourcePath + bulbName[i % (multiplicand % 2 == 0 ? 2 : 3)] + "_light.png");
        Sprite *shadow = Sprite::create(resourcePath + "bulb_shadow.png");
        
        bulb_size = bulb->getContentSize();
        
        bulb->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        bulb_light->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        shadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        
        bulb->setPosition(Point(0, 0 - i * (bulb->getContentSize().height - 33)));
        bulb_light->setPosition(Point(0, 0 - i * (bulb->getContentSize().height - 33)));
        shadow->setPosition(Point(0, 0 - i * (bulb->getContentSize().height - 33)));
        
        bulb->setName("bulb"+TodoUtil::itos(i));
        bulb_light->setName("bulbLight"+TodoUtil::itos(i));
        bulb_light->setVisible(false);
        
        _bulb->addChild(bulb);
        _bulb_light->addChild(bulb_light);
        _shadow->addChild(shadow);
        
        if (i == multiplicand-1)
        {
            auto numberBack = Sprite::create(resourcePath + "number_shadow.png");
            numberBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            numberBack->setScale(1.5, 1.5);
            numberBack->setPosition(Vec2(bulb->getPosition().x, bulb->getPosition().y - bulb->getContentSize().height/2 + 2));
            
            numberBack->setName("bulbNumberBack");
            numberBack->setVisible(false);
            
            auto addBack = Sprite::createWithTexture(numberBack->getTexture());
            addBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            addBack->setPosition(Vec2(numberBack->getContentSize().width/2, numberBack->getContentSize().height/2));
            numberBack->addChild(addBack);
            
            _bulb->addChild(numberBack);

            auto number = Label::createWithTTF(TodoUtil::itos(multiplicand * multiplier), defaultFont, 80);
            number->setColor(numberColor);
            number->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            number->setPosition(Vec2(bulb->getPosition().x + 4, bulb->getPosition().y - bulb->getContentSize().height/2 - 4));
            
            number->setName("bulbNumber");
            number->setVisible(false);
            
            _bulb->addChild(number);
            
            height = i * (bulb->getContentSize().height - 33) + bulb->getContentSize().height;
        }
    }
    
    //묶음들 piece에 추가
    _bulb->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _bulb_light->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _shadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    
    _bulb->setPosition(bulb_size.width/2, height);
    _bulb_light->setPosition(bulb_size.width/2, height);
    _shadow->setPosition(bulb_size.width/2, height);
    
    //훅
    auto hook = Sprite::create(resourcePath + "hook.png");
    hook->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    hook->setPosition(Vec2(0, 0));
    
    _bulb->addChild(hook, 100);

    //그림자, 빛, 전구순으로 그려야 함
    addChild(_shadow);
    addChild(_bulb_light);
    addChild(_bulb);
    
    
    //piece 사이즈 설정
    setContentSize( Size(bulb_size.width, height) );
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
}

void MultiplicationBoardPiece::setLightOn()
{
    auto P = getParent();
    MultiplicationBoardScene* scene = (MultiplicationBoardScene*)P->getParent();
    
    for (int i = 0; i < _bulb_light->getChildrenCount(); i++)
    {
        Sprite *light = (Sprite*)_bulb_light->getChildByName("bulbLight"+TodoUtil::itos(i));
        
        if (light != nullptr)
            light->setVisible(true);
    }
    
    for (int i = 0; i < _bulb->getChildrenCount(); i++)
    {
        Sprite *bulb = (Sprite*)_bulb->getChildByName("bulb"+TodoUtil::itos(i));
        
        if (bulb != nullptr)
            bulb->setVisible(false);
    }
    
    scene->playOnLampSound();
}

void MultiplicationBoardPiece::setLightOff()
{
    for (int i = 0; i < _bulb_light->getChildrenCount(); i++)
    {
        Sprite *light = (Sprite*)_bulb_light->getChildByName("bulbLight"+TodoUtil::itos(i));
        
        if (light != nullptr)
            light->setVisible(false);
    }
    
    for (int i = 0; i < _bulb->getChildrenCount(); i++)
    {
        Sprite *bulb = (Sprite*)_bulb->getChildByName("bulb"+TodoUtil::itos(i));
        
        if (bulb != nullptr)
            bulb->setVisible(true);
    }
}

void MultiplicationBoardPiece::setPicked(bool picked)
{
    Size size = getContentSize();
    
    _picked = picked;
    
    if (picked)
    {
        _bulb->setPosition(size.width/2, size.height+5);
        _bulb_light->setPosition(size.width/2, size.height+5);
        _shadow->setPosition(size.width/2+5, size.height-5);
        
        if (onPicked) {
            onPicked();
        }
    }
    else
    {
        _bulb->setPosition(size.width/2, size.height);
        _bulb_light->setPosition(size.width/2, size.height);
        _shadow->setPosition(size.width/2, size.height);
    }
}

void MultiplicationBoardPiece::snapTarget()
{
    Size size = getContentSize();
    Point targetPos = _targetNode->getPosition();
    
    this->setPosition(Vec2(targetPos.x, targetPos.y - (size.height / 2) + 33));
    setPicked(false);
    _shadow->setVisible(false);
    _snapped = true;
    _touchEnabled = false;
    
    if (onSnapped) {
        onSnapped();
    }

}

void MultiplicationBoardPiece::layTarget()
{
    //원래 자리로 갖다놓자
    runAction(EaseOut::create(MoveTo::create(0.12, _originPos), 2.0));
    
    if (onLaid)
    {
        onLaid();
    }
}
