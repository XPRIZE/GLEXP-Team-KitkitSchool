//
//  FishTankScene.cpp
//  todomath
//
//  Created by Jesun Kim on 2/15/16.
//
//

#include "FishTankScene.h"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"
#include "Utils/TodoUtil.h"
#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Controls/ProgressIndicator.hpp"

#include "CCAppController.hpp"

#define WTE_DEBUG 1

std::string strFishFileNames[5] = {
    "FishTank/fishgame_fish_1.png",
    "FishTank/fishgame_fish_2.png",
    "FishTank/fishgame_fish_3.png",
    "FishTank/fishgame_fish_4.png",
    "FishTank/fishgame_fish_5.png"
};

FishTankScene::FishTankScene(){
    m_currentLevel = 0;
    m_currentStage = 0;
    m_problemCount = 0;
    m_readCount = 0;
    
    m_isBlockFishMove = false;
    
    m_selectedFish = NULL;
    m_selectedCard = NULL;
}

FishTankScene::~FishTankScene()
{
    m_vecFishObject.clear();
    m_vecTankObject.clear();
    m_vecCardObject.clear();
    m_vecSlotObject.clear();
    m_vecTotalProblems.clear();
    m_vecNowProblem.clear();
    m_vecWrong.clear();
}

bool FishTankScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
//    m_WinSize = Size(2048, 1440);
    
    
    auto dirSize = Director::getInstance()->getWinSize();
    
    double scale = dirSize.height / 1440;
    m_WinSize = Size(dirSize.width/scale, 1440);
    
    this->setScale(scale);
    this->setAnchorPoint(Vec2::ZERO);
    
//    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    this->setPosition(dirSize/2);
    
    
    
    this->setContentSize(m_WinSize);
    
    initUI();
    
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, m_WinSize.height-25));
    backButton->setScale(1.0/scale);
    addChild(backButton);
    
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setScale(1.0/scale);
    _progressBar->setPosition(Vec2(m_WinSize.width/2, m_WinSize.height - _progressBar->getContentSize().height));
    addChild(_progressBar);
    
    
    scheduleUpdate();
    
    return true;
}

void FishTankScene::onEnter()
{
    Layer::onEnter();
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(FishTankScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(FishTankScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(FishTankScene::onTouchEnded, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void FishTankScene::onExit()
{
    
    Layer::onExit();
    
    GameSoundManager::getInstance()->unloadAllEffect();
    
    this->removeAllChildren();
    this->unscheduleAllCallbacks();
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}


void FishTankScene::initUI()
{
    m_mainNode = Node::create();
    m_mainNode->setPosition(Vec2::ZERO);

    this->addChild(m_mainNode);
    
    auto winSize = getContentSize();
    
    Sprite *bg = Sprite::create("FishTank/fishgame_background_wall.png");
    auto bgSize = bg->getContentSize();
    float bgScale = MAX(winSize.width/bgSize.width, winSize.height/bgSize.height);
    bg->setScale(bgScale);
    bg->setPosition(winSize/2);
    m_mainNode->addChild(bg);
    
    Sprite *bgBottom = Sprite::create("FishTank/fishgame_background_floor.png");
    auto bgBottomSize = bgBottom->getContentSize();
    float bgBottomScale = MAX(winSize.width/bgBottomSize.width, (148+550)/bgBottomSize.height);
    bgBottom->setScale(bgBottomScale);
    bgBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    bgBottom->setPosition(Vec2(winSize.width/2, 148));
    m_mainNode->addChild(bgBottom);
    
    
//    // 상단 노란 배경 그리기
//    LayerColor *bgColorTop = LayerColor::create(Color4B(255, 205, 31, 255),
//                                                m_WinSize.width, 1292);
//    bgColorTop->setPositionY(148);
//    m_mainNode->addChild(bgColorTop);
//    
//    // 하단 초록 배경 그리기
//    LayerColor *bgColorBottom = LayerColor::create(Color4B(35, 150, 55, 255),
//                                                   m_WinSize.width, 148 + 550);
//    bgColorBottom->setPositionY(-550);
//    m_mainNode->addChild(bgColorBottom);
}

void FishTankScene::drawTank()
{
    if (m_isMinus) {
        // 상단 수조 그리기
        {
            FishTankTankObject *tankObject = new FishTankTankObject(m_strTopTankName);
            m_vecTankObject.pushBack(tankObject);
            CC_SAFE_RELEASE(tankObject);
            
            Sprite *sprite = static_cast<Sprite *>(tankObject->m_node->getChildByName("sprite"));
            
            tankObject->m_node->setPositionX(m_WinSize.width/2);
            tankObject->m_node->setPositionY(744);
            m_mainNode->addChild(tankObject->m_node);
            
            // shelf
            m_spriteShelf = Sprite::create("FishTank/fishgame_shelf.png");
            m_spriteShelf->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            m_spriteShelf->setPosition(Vec2(m_WinSize.width/2, tankObject->m_node->getPositionY() - sprite->getContentSize().height/2));
            m_mainNode->addChild(m_spriteShelf);
            
            // 상하좌우 경계 설정
            tankObject->setBoundary();
            
            tankObject->m_isSource = true;
            tankObject->m_spriteCircle->setVisible(false);
            tankObject->m_text->setOpacity(0);
        }
        
        // 하단 수조 그리기
        {
            FishTankTankObject *tankObject = new FishTankTankObject(m_strBottomTankName);
            m_vecTankObject.pushBack(tankObject);
            CC_SAFE_RELEASE(tankObject);
            
            tankObject->m_node->setPositionX(m_WinSize.width/2 + 193);
            tankObject->m_node->setPositionY(m_vecTankObject.at(0)->m_node->getPositionY() + 47);
            m_mainNode->addChild(tankObject->m_node);
            
            // 상하좌우 경계 설정
            tankObject->setBoundary();
            
            tankObject->m_isSource = false;
            tankObject->m_spriteCircle->setVisible(false);
            tankObject->m_text->setOpacity(0);
        }
    }else{
        // 하단 수조 그리기
        for (int i=0; i<m_bottomTankCount; i++) {
            FishTankTankObject *tankObject = new FishTankTankObject(m_strBottomTankName);
            m_vecTankObject.pushBack(tankObject);
            CC_SAFE_RELEASE(tankObject);
            
            Sprite *sprite = static_cast<Sprite *>(tankObject->m_node->getChildByName("sprite"));
            
            // 수조 좌우에 위치 시키기
            if (m_bottomTankCount == 1) {
                tankObject->m_node->setPositionX(m_WinSize.width/2);
            }else{
                tankObject->m_node->setPositionX(m_WinSize.width/2 - 103 - sprite->getContentSize().width/2 + i*206 + i*sprite->getContentSize().width);
            }
            //tankObject->m_node->setPositionY(148 + sprite->getContentSize().height/2);
            tankObject->m_node->setPositionY(118 + sprite->getContentSize().height/2);
            
            // 상하좌우 경계 설정
            tankObject->setBoundary();
            
            // 타겟 수조로 설정
            tankObject->m_isSource = false;
            tankObject->m_text->setVisible(true);
            tankObject->m_text->setOpacity(0);
            tankObject->m_textPhase2->setVisible(false);
            
            m_mainNode->addChild(tankObject->m_node);
            
//            auto dn = DrawNode::create();
//            dn->drawRect(Vec2(tankObject->m_leftBoundary, tankObject->m_bottomBoundary), Vec2(tankObject->m_rightBoundary, tankObject->m_topBoundary), Color4F::RED);
//            
//            m_mainNode->addChild(dn);
            
            //    m_node->addChild(dn);
            
        }
        
        // 상단 수조 그리기
        for (int i=0; i<m_topTankCount; i++) {
            // 상단 수조 그리기
            FishTankTankObject *tankObject = new FishTankTankObject(m_strTopTankName);
            m_vecTankObject.pushBack(tankObject);
            CC_SAFE_RELEASE(tankObject);
            Sprite *sprite = static_cast<Sprite *>(tankObject->m_node->getChildByName("sprite"));
            
            // 수조 위치
            if (m_topTankCount == 1) {
                tankObject->m_node->setPositionX(m_WinSize.width/2);
            }else{
                tankObject->m_node->setPositionX(m_WinSize.width/2 - 103 - sprite->getContentSize().width/2 + i*206 + i*sprite->getContentSize().width);
            }
            tankObject->m_node->setPositionY(m_WinSize.height-132-sprite->getContentSize().height/2);
            
            m_mainNode->addChild(tankObject->m_node);
            
            tankObject->m_isSource = true;
            tankObject->m_text->setVisible(false);
            tankObject->m_textPhase2->setOpacity(0);
            
            // 상하좌우 경계 설정
            tankObject->setBoundary();
            
            if (i == 0) {
                // shelf
                Sprite *spriteShelf = Sprite::create("FishTank/fishgame_shelf.png");
                spriteShelf->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                spriteShelf->setPosition(Vec2(m_WinSize.width/2, tankObject->m_node->getPositionY() - sprite->getContentSize().height/2));
                m_mainNode->addChild(spriteShelf);
            }
//            
//            auto dn = DrawNode::create();
//            dn->drawRect(Vec2(tankObject->m_leftBoundary, tankObject->m_bottomBoundary), Vec2(tankObject->m_rightBoundary, tankObject->m_topBoundary), Color4F::RED);
//            
//            m_mainNode->addChild(dn);
//            
        }
    }
}

void FishTankScene::initFishes(FishTankTankObject *tankObject, int fishCount)
{
    int fishNumber;
    do {
        fishNumber = RandomHelper::random_int(0, (int)(sizeof(strFishFileNames)/sizeof(strFishFileNames[0]))-1);
    } while (m_isAlreadyFish[fishNumber]);
    m_isAlreadyFish[fishNumber] = true;
    
    for (int i=0; i<fishCount; i++) {
        FishTankFishObject *fishObject = new FishTankFishObject();
        m_vecFishObject.pushBack(fishObject);
        CC_SAFE_RELEASE(fishObject);
        
        //fishObject->m_node = CSLoader::createNode(strFishFileNames[fishNumber]);
        fishObject->m_node = Sprite::create(strFishFileNames[fishNumber]);
        fishObject->m_node->setOpacity(0);
        fishObject->m_node->setPositionX(RandomHelper::random_int(tankObject->m_leftBoundary, tankObject->m_rightBoundary));
        fishObject->m_node->setPositionY(RandomHelper::random_int(tankObject->m_bottomBoundary, tankObject->m_topBoundary));
        m_mainNode->addChild(fishObject->m_node);
        
        auto bubbleNode = CSLoader::createNode("FishTank/nodeBubble.csb");
        bubbleNode->setPosition(Vec2(0, fishObject->m_node->getContentSize().height/2));
        bubbleNode->setName("nodeBubble");
        fishObject->m_node->addChild(bubbleNode);
        
        
        // 방향 설정
        fishObject->m_isLeft = RandomHelper::random_int(0, 1);
        if (!fishObject->m_isLeft) {
            fishObject->setDirection();
        }
        
        // 물고기 애니메이션
//        ActionTimeline *action = CSLoader::createTimeline(strFishFileNames[fishNumber]);
//        fishObject->m_node->runAction(action);
//        action->gotoFrameAndPlay(0, true);
        
        // 현재 이 물고기가 속한 tank 설정
        fishObject->m_myTank = tankObject;
        
        // 물고기 fadeIn
        fishObject->m_node->runAction(FadeIn::create(0.3f));
    }
}

void FishTankScene::drawPhase1Problem()
{
    m_isPhase1 = true;
    
    // 물고기 사용여부 초기화
    ssize_t totalFishCount = (int)(sizeof(m_isAlreadyFish)/sizeof(m_isAlreadyFish[0]));
    for (ssize_t i=0; i<totalFishCount; i++) {
        m_isAlreadyFish[i] = false;
    }
    
    // 상단 수조에 물고기 넣기
    int fishCount = 0;
    int findIndex = 0;
    for (int i=0; i<m_vecTankObject.size(); i++) {
        FishTankTankObject *tankObject = m_vecTankObject.at(i);
        if (tankObject->m_isSource) {
            if (m_topTankCount == 1) {
                if (m_isMinus) {
                    auto it = std::find(m_vecNowProblem.begin(), m_vecNowProblem.end(), "-");
                    auto position = std::distance(m_vecNowProblem.begin(), it);
                    fishCount = TodoUtil::stoi(m_vecNowProblem.at(position-1));
                }else{
                    auto it = std::find(m_vecNowProblem.begin(), m_vecNowProblem.end(), "=");
                    auto position = std::distance(m_vecNowProblem.begin(), it);
                    fishCount = TodoUtil::stoi(m_vecNowProblem.at(position+1));
                }
                
            }else{
                auto it = std::find(m_vecNowProblem.begin(), m_vecNowProblem.end(), "+");
                auto position = std::distance(m_vecNowProblem.begin(), it);
                if (findIndex == 0) {
                    findIndex++;
                    fishCount = TodoUtil::stoi(m_vecNowProblem.at(position-1));
                }else{
                    fishCount = TodoUtil::stoi(m_vecNowProblem.at(position+1));
                }
            }
            this->initFishes(tankObject, fishCount);
        }
    }
    
    if (m_isMinus) {
        FishTankTankObject *tankObject = m_vecTankObject.at(0);
        tankObject->m_fishFirstCount = fishCount;
        tankObject->m_fishCount = fishCount;
        tankObject->m_textPhase2->setString(TodoUtil::itos(fishCount));
    }
}

void FishTankScene::drawPhase2Problem()
{
    m_vecCardObject.clear();
    m_vecSlotObject.clear();
        
    // slot 그리기
    for (int i=0; i<5; i++) {
        FishTankSlotObject *slotObject = new FishTankSlotObject();
        m_vecSlotObject.pushBack(slotObject);
        CC_SAFE_RELEASE(slotObject);
        
        slotObject->m_spriteSlot = Sprite::create("FishTank/fishgame_greenboard_slot.png");
        slotObject->m_spriteSlot->setPositionX(m_WinSize.width/2 - 82*2 - slotObject->m_spriteSlot->getContentSize().width*2 + i*(slotObject->m_spriteSlot->getContentSize().width + 82));
        slotObject->m_spriteSlot->setPositionY(m_WinSize.height - 958 - slotObject->m_spriteSlot->getContentSize().height/2);
        slotObject->m_spriteSlot->setOpacity(0);
        slotObject->m_spriteSlot->runAction(Sequence::createWithTwoActions(DelayTime::create(i*0.1f),
                                                                           FadeIn::create(0.1f)));
        this->addChild(slotObject->m_spriteSlot);
    }
    
    // 카드 그리기
    int maxCard = 6;
    if (m_isReversePhase2) {
        maxCard = 7;
    }
    for (int i=0; i<maxCard ; i++) {
        FishTankCardObject *cardObject = new FishTankCardObject();
        m_vecCardObject.pushBack(cardObject);
        CC_SAFE_RELEASE(cardObject);
        
        // 작은 카드 그림자
        cardObject->m_spriteCardSmallShadow = Sprite::create("FishTank/fishgame_card_small_with_shadow.png");
        cardObject->m_spriteCardSmallShadow->setOpacity(0);
        this->addChild(cardObject->m_spriteCardSmallShadow);
        
        // 작은 카드
        cardObject->m_spriteCardSmall = Sprite::create("FishTank/fishgame_card_small.png");
        cardObject->m_spriteCardSmall->setOpacity(0);
        this->addChild(cardObject->m_spriteCardSmall);
        
        // 작은 카드 문자
        std::string str;
        if (i >= m_vecNowProblem.size()) {
            str = this->findWrongString();
        }else{
            str = m_vecNowProblem.at(i);
            // 슬롯과 카드의 char도 설정
            FishTankSlotObject *slotObject = m_vecSlotObject.at(i);
            cardObject->m_Answer = str;
            slotObject->m_arrAnswer[0] = str;
            slotObject->m_arrAnswer[1] = str;
        }
        if (str.compare("-") == 0) {
            // 마이너스는 이미지로 처리
            cardObject->m_spriteMinusSmall = Sprite::create("FishTank/fishgame_minus.png");
            cardObject->m_spriteMinusSmall->setPosition(cardObject->m_spriteCardSmall->getContentSize()/2);
            cardObject->m_spriteMinusSmall->setOpacity(0);
            cardObject->m_spriteCardSmall->addChild(cardObject->m_spriteMinusSmall);
        }else{
            bool isNumber = true;
            if (str.compare("+") == 0 ||
                str.compare("=") == 0) {
                isNumber = false;
            }
            cardObject->m_labelSmall = TodoUtil::createLabel(str,
                                                             116.0f,
                                                             Size::ZERO,
                                                             defaultFont,
                                                             Color4B(69, 69, 69, 255));
            cardObject->m_labelSmall->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            Point p = cardObject->m_spriteCardSmall->getContentSize()/2;
            if (isNumber) p.y-=10;
            cardObject->m_labelSmall->setPosition(p);
            cardObject->m_labelSmall->setOpacity(0);
            cardObject->m_spriteCardSmall->addChild(cardObject->m_labelSmall);
        }
        
        // 큰 카드
        cardObject->m_spriteCardLarge = Sprite::create("FishTank/fishgame_card_large.png");
        cardObject->m_spriteCardLarge->setScale(cardObject->m_spriteCardSmallShadow->getContentSize().width / cardObject->m_spriteCardLarge->getContentSize().width);
        cardObject->m_spriteCardLarge->setVisible(false);
        this->addChild(cardObject->m_spriteCardLarge);
        
        // 큰 카드 depth
        cardObject->m_spriteCardLargeDepth = Sprite::create("FishTank/fishgame_card_large_with_depth.png");
        cardObject->m_spriteCardLargeDepth->setPosition(cardObject->m_spriteCardLarge->getContentSize()/2);
        cardObject->m_spriteCardLarge->addChild(cardObject->m_spriteCardLargeDepth);
        
        // 큰 카드 문자
        if (str.compare("-") == 0) {
            // 마이너스는 이미지로 처리
            cardObject->m_spriteMinusLarge = Sprite::create("FishTank/fishgame_minus.png");
            cardObject->m_spriteMinusLarge->setScale(1.4);
            cardObject->m_spriteMinusLarge->setPosition(cardObject->m_spriteCardLarge->getContentSize()/2);
            cardObject->m_spriteCardLarge->addChild(cardObject->m_spriteMinusLarge);
        }else{
            bool isNumber = true;
            if (str.compare("+") == 0 ||
                str.compare("=") == 0) {
                isNumber = false;
            }
            cardObject->m_labelLarge = TodoUtil::createLabel(str,
                                                             116.0f,
                                                             Size::ZERO,
                                                             defaultFont,
                                                             Color4B(69, 69, 69, 255));
            cardObject->m_labelLarge->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            Point p = cardObject->m_spriteCardLarge->getContentSize()/2;
            if (isNumber) p.y-=10;
            cardObject->m_labelLarge->setPosition(p);
            cardObject->m_spriteCardLarge->addChild(cardObject->m_labelLarge);
        }
    }
    
    // slot에 들어갈 수 있는 정답 설정
    if (!m_isMinus) {
        // 마이너스는 교환법칙이 안되므로.....
        m_vecSlotObject.at(0)->m_arrAnswer[0] = m_vecSlotObject.at(2)->m_arrAnswer[0];
        m_vecSlotObject.at(2)->m_arrAnswer[1] = m_vecSlotObject.at(0)->m_arrAnswer[1];
    }
    
    
    // 카드 섞기
    for (int i=0; i<m_vecCardObject.size(); i++) {
        std::random_shuffle(m_vecCardObject.begin(), m_vecCardObject.end());
    }
    
    // 카드 좌표 설정
    FishTankCardObject *cardObject = m_vecCardObject.at(0);
    int totalWidth = cardObject->m_spriteCardSmallShadow->getContentSize().width * m_vecCardObject.size() + (38*(m_vecCardObject.size()-1));
    int startX = (m_WinSize.width/2) - (totalWidth/2) + (cardObject->m_spriteCardLargeDepth->getContentSize().width/2);
    for (int i=0; i<m_vecCardObject.size(); i++) {
        Sequence *action = Sequence::createWithTwoActions(DelayTime::create(i*0.1f), FadeIn::create(0.1f));
        
        FishTankCardObject *cardObject = m_vecCardObject.at(i);

        // 작은 카드 그림자
        cardObject->m_spriteCardSmallShadow->setPositionX(startX + i*(cardObject->m_spriteCardSmallShadow->getContentSize().width + 38));
        cardObject->m_spriteCardSmallShadow->setPositionY(46 + cardObject->m_spriteCardSmallShadow->getContentSize().height/2);
        cardObject->m_spriteCardSmallShadow->runAction(action->clone());

        
        // 작은 카드
        cardObject->m_spriteCardSmall->setPosition(cardObject->m_spriteCardSmallShadow->getPosition());
        cardObject->m_spriteCardSmall->runAction(action->clone());

        // 작은 카드 문자
        if (cardObject->m_labelSmall) {
            cardObject->m_labelSmall->runAction(action->clone());
        }else{
            cardObject->m_spriteMinusSmall->runAction(action->clone());
        }
        

        
        // 큰 카드
        cardObject->m_spriteCardLarge->setPosition(cardObject->m_spriteCardSmallShadow->getPosition());
        cardObject->m_spriteCardLarge->runAction(action->clone());
    }
    
    m_clearCount = (int)m_vecSlotObject.size();
    
    if (m_isReversePhase2) {
        // 정답 벡터의 순서를 바꾼다.(2+3=5 -> 5=2+3)
        std::rotate(m_vecNowProblem.begin(), m_vecNowProblem.end()-1, m_vecNowProblem.end());
        std::rotate(m_vecNowProblem.begin()+1, m_vecNowProblem.end()-1, m_vecNowProblem.end());
        
        // 슬롯의 순서도 바꾼다.
        std::rotate(m_vecSlotObject.begin(), m_vecSlotObject.end()-1, m_vecSlotObject.end());
        std::rotate(m_vecSlotObject.begin()+1, m_vecSlotObject.end()-1, m_vecSlotObject.end());
        
        // 슬롯 위치 재설정
        int index = 0;
        for (int i=0; i<m_vecSlotObject.size(); i++) {
            FishTankSlotObject *slotObject = m_vecSlotObject.at(i);
            slotObject->m_spriteSlot->setPositionX(m_WinSize.width/2 - 82*2 - slotObject->m_spriteSlot->getContentSize().width*2 + index*(slotObject->m_spriteSlot->getContentSize().width + 82));
            index++;
            slotObject->m_spriteSlot->stopAllActions();
            slotObject->m_spriteSlot->runAction(Sequence::createWithTwoActions(DelayTime::create(i*0.1f),
                                                                               FadeIn::create(0.1f)));
        }
    }
    
    // format 0_1, 0_2, 1_1는 미리 답이 나와있게 설정
    if (m_isAlreadyPhase2) {
        for (int i=0; i<m_vecNowProblem.size(); i++) {
            FishTankSlotObject *slotObject = m_vecSlotObject.at(i);
            std::string strAnswer = m_vecNowProblem.at(i);
            for (int j=0; j<m_vecCardObject.size(); j++) {
                FishTankCardObject *cardObject = m_vecCardObject.at(j);
                cardObject->m_spriteCardSmallShadow->setVisible(false);
                if (!cardObject->m_isAnswered && strAnswer.compare(cardObject->m_labelLarge->getString()) == 0) {
                    slotObject->m_isAnswered = true;
                    cardObject->setCardTouched();
                    cardObject->m_isAnswered = true;
                    cardObject->m_mySlot = slotObject;
                    cardObject->m_spriteCardLargeDepth->setVisible(false);
                    cardObject->m_spriteCardLarge->setPosition(slotObject->m_spriteSlot->getPosition());
                    break;
                }
            }
        }
        
        // 남은 카드도 숨기기
        for (int i=0; i<m_vecCardObject.size(); i++) {
            FishTankCardObject *cardObject = m_vecCardObject.at(i);
            if (!cardObject->m_isAnswered) {
                cardObject->m_spriteCardSmall->setVisible(false);
                cardObject->m_spriteCardSmallShadow->setVisible(false);
            }
        }
        
        m_isBlockTouch = true;
        this->schedule(CC_SCHEDULE_SELECTOR(FishTankScene::readPhase2), 1.0f, (int)m_vecSlotObject.size(), 0.5f);
    }else{
        if (m_isReversePhase2) {
            if (m_isShowAnswer) {
                // x=y+z일때 x미리 보이기
                FishTankSlotObject *slotObject = m_vecSlotObject.at(0);
                std::string strAnswer = m_vecNowProblem.at(0);
                for (int i=0; i<m_vecCardObject.size(); i++) {
                    FishTankCardObject *cardObject = m_vecCardObject.at(i);
                    if (!cardObject->m_isAnswered && strAnswer.compare(cardObject->m_Answer) == 0) {
                        cardObject->m_spriteCardSmallShadow->setVisible(false);
                        m_succCount++;
                        slotObject->m_isAnswered = true;
                        cardObject->setCardTouched();
                        cardObject->m_isAnswered = true;
                        cardObject->m_mySlot = slotObject;
                        cardObject->m_spriteCardLargeDepth->setVisible(false);
                        cardObject->m_spriteCardLarge->setPosition(slotObject->m_spriteSlot->getPosition());
                        break;
                    }
                }
            }
            
            if (m_isHideTankText) {
                // "?"가 있을때는 "?" 부분만 빼고 다 보이기
                int questionTankIndex = -1;
//                int findCount = -1;
                for (int i=0; i<m_vecTankObject.size(); i++) {
                    FishTankTankObject *tankObject = m_vecTankObject.at(i);
                    if (tankObject->m_text->getString().compare("?") == 0) {
                        questionTankIndex = i;
                        break;
                    }
                }
                int problemIndex = 2;
                if (questionTankIndex == 1) {
                    problemIndex = 4;
                }
                for (int i=0; i<m_vecNowProblem.size(); i++) {
                    // x=y+z일때 y만 빼고 다 보이기
                    if (i == problemIndex) {
                        continue;
                    }
                    FishTankSlotObject *slotObject = m_vecSlotObject.at(i);
                    std::string strAnswer = m_vecNowProblem.at(i);
                    for (int j=0; j<m_vecCardObject.size(); j++) {
                        FishTankCardObject *cardObject = m_vecCardObject.at(j);
                        if (!cardObject->m_isAnswered && strAnswer.compare(cardObject->m_Answer) == 0) {
                            cardObject->m_spriteCardSmallShadow->setVisible(false);
                            m_succCount++;
                            slotObject->m_isAnswered = true;
                            cardObject->setCardTouched();
                            cardObject->m_isAnswered = true;
                            cardObject->m_mySlot = slotObject;
                            cardObject->m_spriteCardLargeDepth->setVisible(false);
                            cardObject->m_spriteCardLarge->setPosition(slotObject->m_spriteSlot->getPosition());
                            break;
                        }
                    }
                }
            }
            
            // 답이 미리 나오지 않지만 역순이면 "=", "+", "-"는 미리 나오게...
            for (int i=0; i<m_vecNowProblem.size(); i++) {
                FishTankSlotObject *slotObject = m_vecSlotObject.at(i);
                std::string strAnswer = m_vecNowProblem.at(i);
                if (strAnswer.compare("=") != 0 && strAnswer.compare("+") != 0 && strAnswer.compare("-") != 0) {
                    continue;
                }
                for (int j=0; j<m_vecCardObject.size(); j++) {
                    FishTankCardObject *cardObject = m_vecCardObject.at(j);
                    if (!cardObject->m_isAnswered && strAnswer.compare(cardObject->m_Answer) == 0) {
                        cardObject->m_spriteCardSmallShadow->setVisible(false);
                        m_succCount++;
                        slotObject->m_isAnswered = true;
                        cardObject->setCardTouched();
                        cardObject->m_isAnswered = true;
                        cardObject->m_mySlot = slotObject;
                        cardObject->m_spriteCardLargeDepth->setVisible(false);
                        cardObject->m_spriteCardLarge->setPosition(slotObject->m_spriteSlot->getPosition());
                        break;
                    }
                }
            }
        }
        
        if (m_isOnlyShowSubtrahend) {
            // x-y=z에서 y는 보이기
            FishTankSlotObject *slotObject = m_vecSlotObject.at(2);
            std::string strAnswer = m_vecNowProblem.at(2);
            for (int i=0; i<m_vecCardObject.size(); i++) {
                FishTankCardObject *cardObject = m_vecCardObject.at(i);
                if (!cardObject->m_isAnswered && strAnswer.compare(cardObject->m_Answer) == 0) {
                    cardObject->m_spriteCardSmallShadow->setVisible(false);
                    m_succCount++;
                    slotObject->m_isAnswered = true;
                    cardObject->setCardTouched();
                    cardObject->m_isAnswered = true;
                    cardObject->m_mySlot = slotObject;
                    cardObject->m_spriteCardLargeDepth->setVisible(false);
                    cardObject->m_spriteCardLarge->setPosition(slotObject->m_spriteSlot->getPosition());
                    break;
                }
            }
            
            // "=", "+", "-"는 미리 나오게...
            for (int i=0; i<m_vecNowProblem.size(); i++) {
                FishTankSlotObject *slotObject = m_vecSlotObject.at(i);
                std::string strAnswer = m_vecNowProblem.at(i);
                if (strAnswer.compare("=") != 0 && strAnswer.compare("+") != 0 && strAnswer.compare("-") != 0) {
                    continue;
                }
                for (int j=0; j<m_vecCardObject.size(); j++) {
                    FishTankCardObject *cardObject = m_vecCardObject.at(j);
                    if (!cardObject->m_isAnswered && strAnswer.compare(cardObject->m_Answer) == 0) {
                        cardObject->m_spriteCardSmallShadow->setVisible(false);
                        m_succCount++;
                        slotObject->m_isAnswered = true;
                        cardObject->setCardTouched();
                        cardObject->m_isAnswered = true;
                        cardObject->m_mySlot = slotObject;
                        cardObject->m_spriteCardLargeDepth->setVisible(false);
                        cardObject->m_spriteCardLarge->setPosition(slotObject->m_spriteSlot->getPosition());
                        break;
                    }
                }
            }
        }
        
        // 남은 카드 위치 재정렬
        int cardCount = 0;
        for (int i=0; i<m_vecCardObject.size(); i++) {
            FishTankCardObject *cardObject = m_vecCardObject.at(i);
            if (!cardObject->m_isAnswered) {
                cardCount++;
            }
        }
        int index = 0;
        FishTankCardObject *cardObject = m_vecCardObject.at(0);
        int totalWidth = (cardObject->m_spriteCardSmallShadow->getContentSize().width * cardCount) + (38*(cardCount-1));
        int startX = (m_WinSize.width/2) - (totalWidth/2) + (cardObject->m_spriteCardSmallShadow->getContentSize().width/2);
        
        for (int i=0; i<m_vecCardObject.size(); i++) {
            FishTankCardObject *cardObject = m_vecCardObject.at(i);
            if (!cardObject->m_isAnswered) {
                Sequence *action = Sequence::createWithTwoActions(DelayTime::create(index*0.1f), FadeIn::create(0.1f));
                
                // 작은 카드 그림자
                cardObject->m_spriteCardSmallShadow->stopAllActions();
                cardObject->m_spriteCardSmallShadow->setPositionX(startX + (index*(cardObject->m_spriteCardSmallShadow->getContentSize().width + 38)));
                cardObject->m_spriteCardSmallShadow->runAction(action->clone());
                
                // 작은 카드
                cardObject->m_spriteCardSmall->stopAllActions();
                cardObject->m_spriteCardSmall->setPosition(cardObject->m_spriteCardSmallShadow->getPosition());
                cardObject->m_spriteCardSmall->runAction(action->clone());
                
                // 작은 카드 문자
                if (cardObject->m_labelSmall) {
                    cardObject->m_labelSmall->stopAllActions();
                    cardObject->m_labelSmall->runAction(action->clone());
                }else{
                    cardObject->m_spriteMinusSmall->stopAllActions();
                    cardObject->m_spriteMinusSmall->runAction(action->clone());
                }
                
                
                // 큰 카드
                cardObject->m_spriteCardLarge->stopAllActions();
                cardObject->m_spriteCardLarge->setPosition(cardObject->m_spriteCardSmallShadow->getPosition());
                cardObject->m_spriteCardLarge->runAction(action->clone());
                
                index++;
            }
        }
    }
}

std::string FishTankScene::findWrongString()
{
    while (true) {
        int index = RandomHelper::random_int(0, (int)(m_vecWrong.size()-1));
        std::string str = m_vecWrong.at(index);
        auto it = std::find(m_vecNowProblem.begin(), m_vecNowProblem.end(), str);
        if (it == m_vecNowProblem.end()) {
            return str;
        }
        CCLOG("오답 다시 찾기");
    }
}

void FishTankScene::prepareNewGameWithLevel(Json::Value problemParameter)
{
    
    answers = problemParameter["answers"];
    Json::Value wrongData = problemParameter["wrong"];
    m_strLevelFormat = problemParameter["format"].asString();
    m_currentLevel = problemParameter["level"].asInt();
    m_topTankCount = problemParameter["TopTankCount"].asInt();
    m_strTopTankName = problemParameter["TopTankName"].asString();
    m_bottomTankCount = problemParameter["BottomTankCount"].asInt();
    m_strBottomTankName = problemParameter["BottomTankName"].asString();
    m_isAlreadyPhase2 = problemParameter["IsAlreadyPhase2"].asBool();
    m_isHideTankText = problemParameter["IsHideTankText"].asBool();
    m_isReversePhase2 = problemParameter["IsReversePhase2"].asBool();
    m_isMinus = problemParameter["IsMinus"].asBool();
    m_isShowAnswer = problemParameter["ShowAnswer"].asBool();
    m_isOnlyShowSubtrahend = problemParameter["OnlyShowSubtrahend"].asBool();
    
    m_currentStage = 0;
    m_problemCount = answers.size();
    
    for (int i=0; i<answers.size(); i++) {
        std::vector<std::string> strVec;
        for (int j=0; j<answers[i].size(); j++) {
            strVec.push_back(answers[i][j].asString());
        }
        m_vecTotalProblems.push_back(strVec);
    }
    
    for (int i=0; i<wrongData.size(); i++) {
        m_vecWrong.push_back(wrongData[i].asString());
    }
    
    this->drawTank();
    
    this->scheduleOnce(schedule_selector(FishTankScene::setProblemInit), 1.f);
}

void FishTankScene::setProblemInit(float)
{
    // 사운드 preload
    for(int i = K_SOUND_CARD_TOUCH ; i < K_SOUND_MAX; ++i){
        std::string name = getSoundName(i);
        GameSoundManager::getInstance()->preloadEffect(name);
    }

    
    this->scheduleOnce(schedule_selector(FishTankScene::setProblem), 0.3f);
}

void FishTankScene::setProblem(float)
{
    m_readCount = 0;
    m_selectedCard = NULL;
    m_selectedFish = NULL;
    
    m_vecNowProblem = m_vecTotalProblems[m_currentStage];

    // phase2 문자
    auto it = std::find(m_vecNowProblem.begin(), m_vecNowProblem.end(), "=");
    auto position = std::distance(m_vecNowProblem.begin(), it);
   
    // 수조 숫자 설정
    int findCount = 0;
    int hideTankPostion = 0;
    if (m_isHideTankText) {
        hideTankPostion = RandomHelper::random_int(0, 1);
    }
    for (int i=0; i<m_vecTankObject.size(); i++) {
        FishTankTankObject *tankObject = m_vecTankObject.at(i);
        if (tankObject->m_isSource) {
            // 상단에 수조가 2개면 phase2때 보여줄 숫자 생성
            if (m_topTankCount == 2) {
                auto it = std::find(m_vecNowProblem.begin(), m_vecNowProblem.end(), "+");
                auto position = std::distance(m_vecNowProblem.begin(), it);
                std::string str;
                if (findCount == 0) {
                    findCount++;
                    str = m_vecNowProblem.at(position-1);
                }else{
                    str = m_vecNowProblem.at(position+1);
                }
                tankObject->m_textPhase2->setString(str);
                tankObject->m_textPhase2->setOpacity(0);
            }else{
                if (m_isMinus) {
                    tankObject->m_text->setString(m_vecNowProblem.at(0));
                    tankObject->m_text->runAction(FadeIn::create(0.3f));
                }
            }
        }else{
            // 하단에 수조가 하나면 하단의 숫자는 맨 마지막 숫자
            if (m_bottomTankCount == 1) {
                if (m_isMinus) {
                    // 마이너스 문제일 경우 빼는 수를 표시하기
                    auto it = std::find(m_vecNowProblem.begin(), m_vecNowProblem.end(), "-");
                    auto position = std::distance(m_vecNowProblem.begin(), it);
                    tankObject->m_needCount = TodoUtil::stoi(m_vecNowProblem.at(position+1));
                    tankObject->m_text->setString(m_vecNowProblem.at(position+1));
                    tankObject->m_text->runAction(FadeIn::create(0.3f));
                }else{
                    tankObject->m_needCount = TodoUtil::stoi(m_vecNowProblem.at(position+1));
                    tankObject->m_text->setString(m_vecNowProblem.at(position+1));
                    tankObject->m_text->runAction(FadeIn::create(0.3f));
                }
            }else{
                auto it = std::find(m_vecNowProblem.begin(), m_vecNowProblem.end(), "+");
                auto position = std::distance(m_vecNowProblem.begin(), it);
                std::string str;
                if (findCount == 0) {
                    str = m_vecNowProblem.at(position-1);
                }else{
                    str = m_vecNowProblem.at(position+1);
                }
                tankObject->m_text->setString(str);
                tankObject->m_needCount = TodoUtil::stoi(str);
                if (str.compare("0") == 0) {
//                    tankObject->setFishCount(0);
                    tankObject->m_isClear = true;
                    tankObject->drawZeroAnimation();
                }else{
                    tankObject->m_text->setVisible(true);
                    tankObject->m_text->runAction(FadeIn::create(0.3f));
                }
                // 수조 숫자 ?로 바꾸기
                if (m_isHideTankText) {
                    if (findCount == hideTankPostion) {
                        tankObject->m_text->setString("?");
                        tankObject->stopZeroAnimation();
                    }
                }
                
                findCount++;
            }
        }
    }
    
    // phase1 그리기
    this->drawPhase1Problem();
    
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.3f),
                                                   CallFunc::create(CC_CALLBACK_0(FishTankScene::setGameStart, this))));
}

void FishTankScene::drawFishBubble(float)
{
    int count = (int)m_vecFishObject.size()/2;
    if(count == 0){
        count = 1;
    }
//    int count = RandomHelper::random_int(1, 2);
//    if (count > m_vecFishObject.size()) {
//        count = (int)m_vecFishObject.size();
//    }
    
    Vector<FishTankFishObject *> vector;
    for (int i=0; i<count; i++) {
        FishTankFishObject *fishObject = m_vecFishObject.at(RandomHelper::random_int(0, (int)(m_vecFishObject.size()-1)));
        auto it = std::find(vector.begin(), vector.end(), fishObject);
        if (it != vector.end()) {
            // 이미 있으면 다시
            i--;
        }else{
            vector.pushBack(fishObject);
        }
    }
    
    for (int i=0; i<vector.size(); i++) {
        FishTankFishObject *fishObject = vector.at(i);
        Node *nodeBubble = fishObject->m_node->getChildByName("nodeBubble");
        //Node *nodeBubble = fishObject->m_node;
        ActionTimeline *action = CSLoader::createTimeline("FishTank/nodeBubble.csb");
        nodeBubble->runAction(action);
//        auto timelines = action->getTimelines();
//        for (auto timeline : timelines) {
//            timeline->setNode(nodeBubble);
//        }
        action->gotoFrameAndPlay(0, false);
    }
    
    vector.clear();
    
    GameSoundManager::getInstance()->playEffectSound(this->getSoundName(K_SOUND_FISH_BUBBLE));
}

void FishTankScene::setGameStart()
{
    GameSoundManager::getInstance()->playEffectSound(this->getSoundName(K_SOUND_FISH_START));
    
    m_succCount = 0;

    m_isBlockTouch = false;
    
    //CCAppController::sharedAppController()->onReady();
    _progressBar->setCurrent(m_currentStage+1);
    
    this->schedule(CC_SCHEDULE_SELECTOR(FishTankScene::drawFishBubble), 3.0f);

}

void FishTankScene::handleStart()
{
    _progressBar->setMax(answers.size());
    //CCAppController::sharedAppController()->onBeginWithTotalProblems(answers.size());
}

void FishTankScene::handlePause()
{
    
}

void FishTankScene::handleQuit()
{
    this->stopAllActions();
    this->unscheduleAllCallbacks();
    
}

void FishTankScene::handleResume()
{
    
}

void FishTankScene::showHint()
{
    FishTankSlotObject *slotObject;
    
    // 채워지지 않은 첫번재 슬롯 찾기
    for (int i=0; i<m_vecSlotObject.size(); i++) {
        slotObject = m_vecSlotObject.at(i);
        if (!slotObject->m_isAnswered) {
            break;
        }
    }
    
    // 슬롯에 맞는 카드 찾기
    m_hintCard = NULL;
    int findIndex = 0;
    bool isFind = false;
    while (true) {
        for (int i=0; i<m_vecCardObject.size(); i++) {
            m_hintCard = m_vecCardObject.at(i);
            if (!m_hintCard->m_isAnswered) {
                if (slotObject->m_arrAnswer[findIndex] == m_hintCard->m_Answer) {
                    isFind = true;
                    break;
                }
            }
        }
        if (isFind) {
            break;
        }else{
            findIndex++;
        }
    }
    
    m_hintCard->m_spriteCardLarge->setScale(1.0f);
    m_hintCard->m_spriteCardLarge->setVisible(true);
    m_hintCard->m_spriteCardLarge->setPosition(slotObject->m_spriteSlot->getPosition());
    m_hintCard->m_spriteCardLargeDepth->setVisible(false);
}

void FishTankScene::hideHint()
{
    m_hintCard->m_spriteCardLarge->setScale(m_hintCard->m_spriteCardSmallShadow->getContentSize().width / m_hintCard->m_spriteCardLarge->getContentSize().width);
    m_hintCard->m_spriteCardLarge->setVisible(false);
    m_hintCard->m_spriteCardLargeDepth->setVisible(true);
    m_hintCard = NULL;
}

void FishTankScene::handleAnswerEntered(std::string &answer)
{
    //CCAppController::sharedAppController()->onGameComplete();

    
}

bool FishTankScene::checkIsFishInTank(FishTankTankObject *tankObject)
{
    if (m_selectedFish->m_node->getPositionX() > tankObject->m_leftEnter &&
        m_selectedFish->m_node->getPositionX() < tankObject->m_rightEnter &&
        m_selectedFish->m_node->getPositionY() > tankObject->m_bottomEnter &&
        m_selectedFish->m_node->getPositionY() < tankObject->m_topEnter) {
        this->fixFishPosition(tankObject);
        return true;
    }
    return false;
}

void FishTankScene::fixFishPosition(FishTankTankObject *tankObject)
{
    int moveX = m_selectedFish->m_node->getPositionX();
    int moveY = m_selectedFish->m_node->getPositionY();
    if (m_selectedFish->m_node->getPositionX() < tankObject->m_leftBoundary) {
        // 왼쪽 boundary밖인지...
        moveX = tankObject->m_leftBoundary+10;
    }else if (m_selectedFish->m_node->getPositionX() > tankObject->m_rightBoundary) {
        // 오른쪽 boundary 밖인지...
        moveX = tankObject->m_rightBoundary-10;
    }
    
    if (m_selectedFish->m_node->getPositionY() > tankObject->m_topBoundary) {
        // 위쪽 boundary 밖인지...
        moveY = tankObject->m_topBoundary-10;
    }else if (m_selectedFish->m_node->getPositionY() < tankObject->m_bottomBoundary) {
        // 아래쪽 boundary 밖인지...
        moveY = tankObject->m_bottomBoundary+10;
    }
    m_selectedFish->m_node->runAction(MoveTo::create(0.1f, Vec2(moveX, moveY)));
}

bool FishTankScene::checkClearPhase1()
{
    for (int i=0; i<m_vecTankObject.size(); i++) {
        FishTankTankObject *tankObject = m_vecTankObject.at(i);
        if (!tankObject->m_isSource && !tankObject->m_isClear) {
            return false;
        }
    }
    return true;
}

void FishTankScene::drawPhase2Move()
{
    m_isPhase1 = false;
    float moveTime = 0.3f;
    
    // m_mainNode 전체를 위로 이동
    m_mainNode->runAction(Sequence::createWithTwoActions(MoveBy::create(moveTime, Vec2(0, 550-148)),
                                                       CallFunc::create(CC_CALLBACK_0(FishTankScene::drawPhase2Problem, this))));
    
    // 뺄셈 문제의 경우 수조를 밑으로 내린다.
    if (m_isMinus) {
        m_isBlockFishMove = true;
        MoveBy *move = MoveBy::create(moveTime, Vec2(0, -402 + 180));
        CallFunc *call = CallFunc::create([&](){
            m_isBlockFishMove = false;
        });
        for (int i=0; i<m_vecTankObject.size(); i++) {
            FishTankTankObject *tankObject = m_vecTankObject.at(i);
            tankObject->m_node->runAction(move->clone());
        }
        for (int i=0; i<m_vecFishObject.size(); i++) {
            FishTankFishObject *fishObject = m_vecFishObject.at(i);
            fishObject->m_node->runAction(move->clone());
            
        }
        m_spriteShelf->runAction(Sequence::createWithTwoActions(move->clone(), call));
    }
    
    for (int i=0; i<m_vecTankObject.size(); i++) {
        FishTankTankObject *tankObject = m_vecTankObject.at(i);
        if (tankObject->m_isSource) {
            if (m_topTankCount == 2) {
                tankObject->m_textPhase2->runAction(FadeIn::create(moveTime));
            }else{
                if (m_isMinus) {
                    // 수조에 남아 있는 물고기 아래로 보내기
//                    for (int j=0; j<m_vecFishObject.size(); j++) {
//                        FishTankFishObject *fishObject = m_vecFishObject.at(j);
//                        if (fishObject->m_myTank == tankObject) {
//                            fishObject->m_node->setPositionY(tankObject->m_bottomBoundary + 30);
//                        }
//                    }
                }
            }
        }else{
            // ? == 0 일때는 ?가 보이는 상태이므로 사라지게 한다
            if (tankObject->m_needCount == 0 && tankObject->m_text->getString().compare("?") == 0) {
                tankObject->m_text->runAction(FadeOut::create(0.3f));
            }
            // target tank에 있는 text는 fadeOut
//            tankObject->m_text->runAction(FadeTo::create(moveTime, 0));
        }
    }
}

void FishTankScene::checkSlotChar(std::string str)
{
    // str은 정답에서 빠진 문자이므로 slot에서도 뺀다
    int index = 0;
    if (m_isReversePhase2) {
        index = 1;
    }
    for (int i=0; i<2; i++) {
        FishTankSlotObject *slotObject = m_vecSlotObject.at((i+index)*2);
        for (int j=0; j<2; j++) {
            if (slotObject->m_arrAnswer[j].compare(str) == 0) {
                slotObject->m_arrAnswer[j] = "!";
                break;
            }
        }
    }
}

void FishTankScene::checkLevelClearDelay(float){
    _progressBar->setCurrent(m_currentStage+1, true);
    m_currentStage++;
    //CCAppController::sharedAppController()->successStageAnimation(m_currentStage);

    
    this->scheduleOnce(schedule_selector(FishTankScene::checkLevelClear), 1.3f);
}

void FishTankScene::checkLevelClear(float){
    if(m_currentStage >= m_problemCount){
//        m_GameState = K_TAG_GAME_END;
        m_isBlockTouch = false;
       // CCAppController::sharedAppController()->onGameComplete();
        CompletePopup::create()->show(1.0, [](){
            CCAppController::sharedAppController()->handleGameComplete(1);
            //TodoSchoolBackButton::popGameScene();
        });
    }
    else{
        removeFishes();
        removeCards();
        removeTexts();
        
        m_mainNode->runAction(MoveBy::create(0.3f, Vec2(0, -402)));
        // 뺄셈의 경우 어항 위치 재조정
        if (m_isMinus) {
            MoveBy *move = MoveBy::create(0.3f, Vec2(0, 402 - 180));
            CallFunc *call = CallFunc::create([&](){
                m_isBlockFishMove = false;
            });
            for (int i=0; i<m_vecTankObject.size(); i++) {
                FishTankTankObject *tankObject = m_vecTankObject.at(i);
                tankObject->m_node->runAction(move->clone());
            }
            for (int i=0; i<m_vecFishObject.size(); i++) {
                FishTankFishObject *fishObject = m_vecFishObject.at(i);
                fishObject->m_node->runAction(move->clone());
                
            }
            m_spriteShelf->runAction(Sequence::createWithTwoActions(move->clone(), call));
        }
        
        this->scheduleOnce(schedule_selector(FishTankScene::setProblem), 0.5f);
    }
}

void FishTankScene::removeFishes()
{
    for (int i=0; i<m_vecFishObject.size(); i++) {
        FishTankFishObject *fishObject = m_vecFishObject.at(i);
        fishObject->m_node->removeFromParentAndCleanup(true);
    }
    m_vecFishObject.clear();
}

void FishTankScene::removeCards()
{
    for (int i=0; i<m_vecCardObject.size(); i++) {
        FishTankCardObject *cardObject = m_vecCardObject.at(i);
        cardObject->m_spriteCardLargeDepth->removeFromParentAndCleanup(true);
        cardObject->m_spriteCardLarge->removeFromParentAndCleanup(true);
        cardObject->m_spriteCardSmall->removeFromParentAndCleanup(true);
        cardObject->m_spriteCardSmallShadow->removeFromParentAndCleanup(true);
    }
    m_vecCardObject.clear();
    
    for (int i=0; i<m_vecSlotObject.size(); i++) {
        FishTankSlotObject *slotObject = m_vecSlotObject.at(i);
        slotObject->m_spriteSlot->removeFromParentAndCleanup(true);
    }
    m_vecSlotObject.clear();
}

void FishTankScene::removeTexts()
{
    for (int i=0; i<m_vecTankObject.size(); i++) {
        FishTankTankObject *tankObject = m_vecTankObject.at(i);
        tankObject->initData();
        tankObject->m_text->setOpacity(0);
        tankObject->m_textPhase2->setOpacity(0);
        if (m_isMinus) {
            tankObject->m_spriteCircle->setVisible(false);
            tankObject->m_textPhase2->setOpacity(255);
        }
    }
}

std::string FishTankScene::getSoundName(int type){
    std::string ret = "";
    
    if(type == K_SOUND_CARD_SLOT_IN) ret = "FishTank/Sounds/Matrix_RightAnswer.m4a";
    else if(type == K_SOUND_CARD_TOUCH) ret = "FishTank/Sounds/Matrix_ClickBlock.m4a";
    else if(type == K_SOUND_CARD_MISS) ret = "FishTank/Sounds/Matrix_WrongMove.m4a";
    else if(type == K_SOUND_FISH_TOUCH) ret = "FishTank/Sounds/HB_turn on.m4a";
    else if(type == K_SOUND_FISH_RELEASE) ret = "FishTank/Sounds/HB_turn off.m4a";
    else if(type == K_SOUND_FISH_RELEASE_1) ret = "FishTank/Sounds/HB_turn off_1.m4a";
    else if(type == K_SOUND_FISH_BUBBLE) ret = "FishTank/Sounds/Bubble_up_2.m4a";
    else if(type == K_SOUND_FISH_PLACED) ret = "FishTank/Sounds/Fish_placed_1.m4a";
    else if(type == K_SOUND_FISH_START) ret = "FishTank/Sounds/Start_1.m4a";
    
    return ret;
}

void FishTankScene::readPhase2(float dt)
{
    FishTankSlotObject *slotObject = m_vecSlotObject.at(m_readCount);
    for (int i=0; i<m_vecCardObject.size(); i++) {
        FishTankCardObject *cardObject = m_vecCardObject.at(i);
        if (cardObject->m_mySlot == slotObject) {
            if (cardObject->m_labelSmall) {
                // 마이너스가 아닌지...
                if (cardObject->m_labelSmall->getString().compare("+") == 0) {
                    GameSoundManager::getInstance()->playAdultVoice(0, GameSoundManager::K_ADULT_VOICE_PLUS);
                }else if (cardObject->m_labelSmall->getString().compare("=") == 0) {
                    GameSoundManager::getInstance()->playAdultVoice(0, GameSoundManager::K_ADULT_VOICE_EQ);
                }else{
                    GameSoundManager::getInstance()->playAdultVoice(TodoUtil::stoi(cardObject->m_labelSmall->getString()));
                }
            }else{
                // 마이너스...
                GameSoundManager::getInstance()->playAdultVoice(0, GameSoundManager::K_ADULT_VOICE_MINUS);
            }
            break;
        }
    }
    
    m_readCount++;
    if (m_readCount == m_vecSlotObject.size()) {
        this->unschedule(CC_SCHEDULE_SELECTOR(FishTankScene::readPhase2));
        this->scheduleOnce(schedule_selector(FishTankScene::checkLevelClearDelay), 1.0f);
    }
}

bool FishTankScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (m_isBlockTouch) {
        return false;
    }
    if (m_isPhase1 && m_selectedFish) {
        return false;
    }else if(!m_isPhase1 && m_selectedCard){
        return false;
    }
    
    Vec2 touchPoint = touch->getLocation();
    CCLOG("touch id = %d", touch->getID());
    
    if (m_isPhase1) {
        // 물고기 옮기기
        for (ssize_t i=m_vecFishObject.size()-1; i>=0; i--) {
            FishTankFishObject *fishObject = m_vecFishObject.at(i);
            if (fishObject->getFishBoundingBox().containsPoint(touchPoint) &&
                !fishObject->m_node->getActionByTag(333)) {
                this->unschedule(CC_SCHEDULE_SELECTOR(FishTankScene::drawFishBubble));
                m_selectedFish = fishObject;
                if (m_selectedFish->m_isLeft) {
                    m_selectedFish->m_node->setScale(1.5f, 1.5f);
                }else{
                    m_selectedFish->m_node->setScale(-1.5f, 1.5f);
                }
                m_selectedFish->m_prevPosition = m_selectedFish->m_node->getPosition();
                m_selectedFish->m_isMoving = false;
                m_selectedFish->m_node->getParent()->reorderChild(m_selectedFish->m_node, 1);
                GameSoundManager::getInstance()->playEffectSound(this->getSoundName(K_SOUND_FISH_TOUCH));
                return true;
            }
        }
    }else{
        // 카드 이동시키기
        
        if (m_isAlreadyPhase2) {
            return false;
        }
        for (int i=0; i<m_vecCardObject.size(); i++) {
            FishTankCardObject *cardObject =  m_vecCardObject.at(i);
            auto localPoint = cardObject->m_spriteCardSmall->getParent()->convertToNodeSpace(touchPoint);
            if (!cardObject->m_isAnswered &&
                cardObject->m_spriteCardSmall->getBoundingBox().containsPoint(localPoint) &&
                !cardObject->m_spriteCardLarge->getActionByTag(333)) {
                m_selectedCard = cardObject;
                m_selectedCard->setCardTouched();
                GameSoundManager::getInstance()->playEffectSound(this->getSoundName(K_SOUND_CARD_TOUCH));
                return true;
            }
        }
    }
    return false;
}

void FishTankScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (m_selectedFish) {
        m_selectedFish->m_node->setPosition(m_selectedFish->m_node->getPosition() + touch->getDelta() / 2.5f);
    }else if(m_selectedCard){
        m_selectedCard->m_spriteCardLarge->setPosition(m_selectedCard->m_spriteCardLarge->getPosition() + touch->getDelta() / 2.5f);
        // 현재 선택된 카드가 들어갈 슬롯
        for (int i=0; i<m_vecSlotObject.size(); i++) {
            FishTankSlotObject *slotObject = m_vecSlotObject.at(i);
            if (!slotObject->m_isAnswered &&
                slotObject->m_spriteSlot->getBoundingBox().containsPoint(m_selectedCard->m_spriteCardLarge->getPosition())) {
                if ((slotObject->m_arrAnswer[0].compare(m_selectedCard->m_Answer) == 0) ||
                    (slotObject->m_arrAnswer[1].compare(m_selectedCard->m_Answer) == 0)) {
                    if (!m_isReversePhase2 && i != m_vecSlotObject.size() -1) {
                        this->checkSlotChar(m_selectedCard->m_Answer);
                    }else if(m_isReversePhase2 && i != 0){
                        this->checkSlotChar(m_selectedCard->m_Answer);
                    }
                    
                    // NB(xenosoz, 2018): Log for future analysis (#1/2)
                    auto workPath = [this] {
                        stringstream ss;
                        ss << "/" << "FishTank";
                        ss << "/" << "level-" << m_currentLevel;
                        ss << "/" << "work-" << m_currentStage;
                        return ss.str();
                    }();
                    
                    auto wrapStr = [](const string& s) {
                        stringstream ss;
                        ss << "'" << s << "'";
                        return ss.str();
                    };
                    
                    StrictLogManager::shared()->game_Peek_Answer("FishTank", workPath,
                                                                 wrapStr(m_selectedCard->m_Answer),
                                                                 wrapStr(m_selectedCard->m_Answer));

                    //
                    GameSoundManager::getInstance()->playEffectSound(this->getSoundName(K_SOUND_CARD_SLOT_IN));
                    slotObject->m_isAnswered = true;
                    m_selectedCard->m_mySlot = slotObject;
                    m_selectedCard->m_isAnswered = true;
                    m_selectedCard->m_spriteCardLargeDepth->setVisible(false);
                    m_selectedCard->m_spriteCardLarge->runAction(MoveTo::create(0.1f, slotObject->m_spriteSlot->getPosition()));
                    m_selectedCard = NULL;
                    
                    m_succCount++;
                    if (m_succCount >= m_clearCount) {
                        m_isBlockTouch = true;
                        this->schedule(CC_SCHEDULE_SELECTOR(FishTankScene::readPhase2), 1.0f, (int)m_vecSlotObject.size(), 0.5f);
                    }
                    break;
                }
            }
        }
    }
}

void FishTankScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (m_isPhase1) {
        this->schedule(CC_SCHEDULE_SELECTOR(FishTankScene::drawFishBubble), 3.0f);
        
        // 해당 물고기가 어떤 수조에 들어갔는지 체크
        bool isFind = false;
        for (int i=0; i<m_vecTankObject.size(); i++) {
            FishTankTankObject *tankObject = m_vecTankObject.at(i);
            if (this->checkIsFishInTank(tankObject)) {
                GameSoundManager::getInstance()->playEffectSound(this->getSoundName(K_SOUND_FISH_PLACED));
                m_selectedFish->m_myTank->setFishCount(-1);
                m_selectedFish->m_myTank = tankObject;
                m_selectedFish->m_myTank->setFishCount(1);
                isFind = true;
                
                break;
            }
        }
        if (!isFind) {
            // 원래 위치로 돌아가기
            GameSoundManager::getInstance()->playEffectSound(this->getSoundName(K_SOUND_FISH_RELEASE + RandomHelper::random_int(0, 1)));
            MoveTo *move = MoveTo::create(0.1f, m_selectedFish->m_prevPosition);
            move->setTag(333);
            m_selectedFish->m_node->runAction(move);
        }else{
            // phase1 클리어 했는지 체크
            if (this->checkClearPhase1()) {
                this->unschedule(CC_SCHEDULE_SELECTOR(FishTankScene::drawFishBubble));
                this->drawPhase2Move();
            }
        }
        m_selectedFish->m_node->getParent()->reorderChild(m_selectedFish->m_node, 0);
        m_selectedFish->m_isMoving = true;
        if (m_selectedFish->m_isLeft) {
            m_selectedFish->m_node->setScale(1.0f, 1.0f);
        }else{
            m_selectedFish->m_node->setScale(-1.0f, 1.0f);
        }
        m_selectedFish = NULL;
    }else{
        if (m_selectedCard) {
            // NB(xenosoz, 2018): Log for future analysis (#2/2)
            auto workPath = [this] {
                stringstream ss;
                ss << "/" << "FishTank";
                ss << "/" << "level-" << m_currentLevel;
                ss << "/" << "work-" << m_currentStage;
                return ss.str();
            }();
            
            auto wrapStr = [](const string& s) {
                stringstream ss;
                ss << "'" << s << "'";
                return ss.str();
            };
            
            StrictLogManager::shared()->game_Peek_Answer("FishTank", workPath,
                                                         wrapStr(m_selectedCard->m_Answer),
                                                         "None");

            GameSoundManager::getInstance()->playEffectSound(this->getSoundName(K_SOUND_CARD_MISS));
            // 잘못된 slot이면 오류 처리
            for (int i=0; i<m_vecSlotObject.size(); i++) {
                FishTankSlotObject *slotObject = m_vecSlotObject.at(i);
                if (slotObject->m_spriteSlot->getBoundingBox().containsPoint(m_selectedCard->m_spriteCardLarge->getPosition())) {
                    //CCAppController::sharedAppController()->onMiss();
                    break;
                }
            }
            
            // 원래 위치로 돌아가기
            m_selectedCard->setCardTouchCancel();
            m_selectedCard = NULL;
            
        }
    }
}

void FishTankScene::update(float fDelta)
{
    if (m_isBlockFishMove) {
        return;
    }
    // 물고기의 움직임
    for (int i=0; i<m_vecFishObject.size(); i++) {
        FishTankFishObject *fishObject = m_vecFishObject.at(i);
        if (fishObject->m_isMoving) {
            fishObject->m_changeMovingSpeedCount--;
            fishObject->m_forceDirectionCount--;
            // 좌/우 이동
            if (fishObject->m_isLeft) {
                fishObject->m_node->setPositionX(fishObject->m_node->getPositionX() - fishObject->m_movingSpeed);
                // 왼쪽 끝에 다다르면 오른쪽으로 방향 전환
                if (fishObject->m_node->getPositionX() < fishObject->m_myTank->m_leftBoundary) {
                    fishObject->m_node->setPositionX(fishObject->m_node->getPositionX() + fishObject->m_movingSpeed);
                    fishObject->m_isLeft = false;
                    fishObject->setDirection();
                    fishObject->setForceDirectionCount();
                }
            }else{
                fishObject->m_node->setPositionX(fishObject->m_node->getPositionX() + fishObject->m_movingSpeed);
                // 오른쪽 끝에 다다르면 왼쪽으로 방향 전환
                if (fishObject->m_node->getPositionX() > fishObject->m_myTank->m_rightBoundary) {
                    fishObject->m_node->setPositionX(fishObject->m_node->getPositionX() - fishObject->m_movingSpeed);
                    fishObject->m_isLeft = true;
                    fishObject->setDirection();
                    fishObject->setForceDirectionCount();
                }
            }
            // 벽에 닿지 않아도 강제로 방향 회전
            if (fishObject->m_forceDirectionCount == 0) {
                fishObject->setForceDirectionCount();
                int forceDirection = RandomHelper::random_int(0, 10);
                if (forceDirection == 0) {
                    fishObject->m_isLeft = !fishObject->m_isLeft;
                    fishObject->setDirection();
                }
            }
            
            // 상/하 이동
//            int moveY = RandomHelper::random_int(-1, 1);
//            fishObject->m_node->setPositionY(fishObject->m_node->getPositionY() + moveY);
//            if (fishObject->m_node->getPositionY() > fishObject->m_topBoundary) {
//                fishObject->m_node->setPositionY(fishObject->m_node->getPositionY() - 1);
//            }else if(fishObject->m_node->getPositionY() < fishObject->m_bottomBoundary){
//                fishObject->m_node->setPositionY(fishObject->m_node->getPositionY() + 1);
//            }
            
            // 이동속도 변경
            if (fishObject->m_changeMovingSpeedCount == 0) {
                fishObject->setChangeMovingSpeedCount();
                fishObject->setMovingSpeed();
            }
        }
    }
}
