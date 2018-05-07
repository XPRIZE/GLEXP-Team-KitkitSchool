//
//  CoinTab.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 9/18/17.
//
//

#include "CoinTab.hpp"
#include "Managers/UserManager.hpp"
#include "Utils/TodoUtil.h"
#include "Common/Basic/SoundEffect.h"

int CoinTab::_numCoin;

bool CoinTab::init()
{
    
    if (!Node::init()) return false;

    const auto frameRect = Rect(0, 0, 248, 248);
    
    for (int i=1; i<=6; i++) {
        auto filename = "Common/Controls/Coin/coin_frame_" + TodoUtil::itos(i)+".png";
        auto f = SpriteFrame::create(filename, frameRect);
        _frames.pushBack(f);
    }

    
    
    auto bg = Sprite::create("Common/Controls/daily_coinstatus_bg.png");
    auto size = bg->getContentSize();
    
    this->setContentSize(size);
    bg->setPosition(size/2);
    this->addChild(bg);
    
    auto coin = Sprite::create("Common/Controls/Coin/daily_coinstatus_coin.png");
    coin->setPosition(Vec2(0, size.height/2));
    this->addChild(coin);
    
    _coinLabel = nullptr;
    updateCoinLabel();
    
    this->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    
    return true;
 
}

void CoinTab::onEnter()
{
    Node::onEnter();
    setCoinLabel(_numCoin);
}

void CoinTab::updateCoinLabel()
{

    setCoinLabel(UserManager::getInstance()->getStars());
    
    
}

void CoinTab::setCoinLabel(int num)
{
    _numCoin = num;
    
    if (_coinLabel) {
        _coinLabel->removeFromParent();
    }
    auto size = this->getContentSize();
    
    _coinLabel = TodoUtil::createLabel(TodoUtil::itos(num), 70, Size::ZERO, "fonts/TodoMainCurly.ttf", Color4B::WHITE);
    _coinLabel->setPosition(size/2);
    this->addChild(_coinLabel);
    
    

}


void CoinTab::addCoin(int numCoin, cocos2d::Vec2 fromPosWorld, bool modeGameSelect)
{
    

    if (numCoin<=0) return;
    
    int ratio = 1;
    if (numCoin > 99) {
        numCoin = numCoin/10;
        ratio = 10;
    }
    float inoutTime = MIN(2.5 / numCoin, 0.5);
    float delayTime = MIN(1.0 / numCoin, 0.2);
    
    SoundEffect::startInEffect().preload();
    SoundEffect::starOutEffect().play();

    
    if (modeGameSelect) {
        auto parent = this->getParent();
        float step = parent->getContentSize().width / (numCoin+1);

        for (int i=0; i<numCoin; i++) {
            Node *coin = Node::create();
            coin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            parent->addChild(coin);
            coin->setPosition(Vec2(step*(i+1), 150));
            
            Sprite *coinSprite = Sprite::createWithSpriteFrame(_frames.front());
            auto coinSize = coinSprite->getContentSize();
            coin->setContentSize(coinSize);
            coinSprite->setPosition(coinSize/2);
            coin->addChild(coinSprite);
            
            auto animation = Animation::createWithSpriteFrames(_frames, 1.0/24.0, 1);
            auto animate = Animate::create(animation);
            
            coinSprite->runAction(RepeatForever::create(animate));

            auto toPos = Vec2(this->getPositionX()-this->getContentSize().width, this->getPositionY());
            auto inSpawn = Spawn::create(EaseOut::create(MoveTo::create(inoutTime, toPos), 2.0),
                                         EaseOut::create(ScaleTo::create(inoutTime, 0.0), 2.0), nullptr);
            coin->runAction(Sequence::create(DelayTime::create(delayTime*i + 0.1),
                                             DelayTime::create(1.2),
                                             inSpawn,
                                             CallFunc::create([coin, ratio, this](){
                SoundEffect::startInEffect().play();
                this->addCoinLabel(1*ratio);
                coin->removeFromParent();
            }),
                                             nullptr));

        }
        
    } else {
        auto fromPosLocal = this->convertToNodeSpace(fromPosWorld);
        
        auto size = this->getContentSize();
        auto winSize = Director::getInstance()->getWinSize();
        
        auto toPos = Vec2(0, size.height/2);
        
        auto width = MIN(300 * (numCoin-1), 2100);
        auto stepX = (numCoin==1) ? 0 : width / (numCoin-1);
        auto startX = winSize.width/2 - width/2;
        
        for (int i=0; i<numCoin; i++) {
            
            Node *coin = Node::create();
            coin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            this->addChild(coin);
            
            Sprite *coinSprite = Sprite::createWithSpriteFrame(_frames.front());
            auto coinSize = coinSprite->getContentSize();
            coin->setContentSize(coinSize);
            coinSprite->setPosition(coinSize/2);
            coin->addChild(coinSprite);
            
            auto animation = Animation::createWithSpriteFrames(_frames, 1.0/24.0, 1);
            auto animate = Animate::create(animation);
            
            coinSprite->runAction(RepeatForever::create(animate));
            
            
            auto outPos = this->convertToNodeSpace( Vec2(startX+i*stepX, fromPosWorld.y+400) );
            auto outTime = inoutTime;
            auto outSpawn = Spawn::create(EaseIn::create(MoveTo::create(outTime, outPos), 2.0),
                                          EaseIn::create(ScaleTo::create(outTime, 1.0), 2.0), nullptr);
            auto inTime = inoutTime;
            auto inSpawn = Spawn::create(EaseOut::create(MoveTo::create(inTime, toPos), 2.0),
                                         EaseOut::create(ScaleTo::create(inTime, 0.0), 2.0), nullptr);
            
            coin->setPosition(fromPosLocal);
            coin->setScale(0.01);
            coin->runAction(Sequence::create(DelayTime::create(delayTime*i + 0.1),
                                             outSpawn,
                                             DelayTime::create(1.2),
                                             inSpawn,
                                             CallFunc::create([coin, ratio, this](){
                SoundEffect::startInEffect().play();
                this->addCoinLabel(1*ratio);
                coin->removeFromParent();
            }),
                                             nullptr));
            
        }
    }
    
}
