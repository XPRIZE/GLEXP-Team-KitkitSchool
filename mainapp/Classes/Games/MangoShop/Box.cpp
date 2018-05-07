//
//  Box.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 3/7/18.
//

#include "Box.hpp"
#include "Managers/GameSoundManager.h"

namespace MangoShop
{
    const string kBoxImageLandscape = "MangoShop/mango-shop_image_box_horizontal.png";
    const string kBoxImagePortrait = "MangoShop/mango-shop_image_box_vertical.png";

    const string kCoverImageLandscape = "MangoShop/mango-shop_image_box_horizontal-cover.png";
    const string kCoverImagePortrait = "MangoShop/mango-shop_image_box_vertical-cover.png";
    
    const string kStickerImageLandscape = "MangoShop/mango-shop_image_box_horizontal-cover-number.png";
    const string kStickerImagePortrait = "MangoShop/mango-shop_image_box_vertical-cover-number.png";
    
    const string kMinusCompleteBoxImageLandscape = "MangoShop/mango-shop_image_box_horizontal-for10guide.png";
    const string kMinusCompleteBoxImagePortrait = "MangoShop/mango-shop_image_box_vertical-for10guide.png";
    
    const string kCloseButtonImage = "MangoShop/mango-shop_image_box_xbutton.png";
    
    Box::Box():
    _cover(nullptr),
    _isShowCover(false),
    _closeButton(nullptr),
    isSlot(false),
    isSlotIsFull(false),
    isActive(true),
    isBoxMovable(false),
    isOnTray(false),
    onTouchBegan(nullptr),
    onTouchMoved(nullptr),
    onTouchEnded(nullptr),
    onCloseButtonTouchBegan(nullptr),
    onCloseButtonTouchEnded(nullptr),
    onStartParticle(nullptr),
    onStopParticle(nullptr)
    {
        
    }
    
    bool Box::init()
    {
        if (!Node::init()) { return false; }
        
        bindingCloseButtonEventListener();
        bindingBoxEventListener();
        return true;
    }
    
    void Box::bindingBoxEventListener()
    {
        auto *listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this](Touch* T, Event* E) {
            if (isShowCover() == false)
                return false;
            
            auto P = getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (this->getBoundingBox().containsPoint(pos)) {
                if (isActive == false)
                    return false;
                
                if (isSlot == true)
                    return false;
                
                if(onTouchBegan) onTouchBegan();
                return true;
            }
            return false;
        };
        
        listener->onTouchMoved = [this](Touch* T, Event* E) {
            if (isBoxMovable == false)
                return;
            
            if (isShowCover() == false)
                return;
            
            auto P = getParent();
            auto pl = P->convertToNodeSpace(T->getPreviousLocation());
            auto cl = P->convertToNodeSpace(T->getLocation());
            auto delta = cl-pl;
            this->setPosition(this->getPosition()+delta);
            if(onTouchMoved) onTouchMoved();
        };
        
        listener->onTouchEnded = [this](Touch* T, Event* E) {
            if(onTouchEnded) onTouchEnded();
        };
        
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
    void Box::bindingCloseButtonEventListener()
    {
        auto *listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this](Touch* T, Event* E) {
            if (_closeButton == nullptr)
                return false;
            if (_closeButton->isVisible() == false)
                return false;
            
            // 닫기 버튼이 있었을 경우의 코드
//            auto P = _closeButton->getParent();
//            auto pos = P->convertToNodeSpace(T->getLocation());
//            if (_closeButton->getBoundingBox().containsPoint(pos)) {
//                if (isActive == false)
//                    return false;
//
//                if (onCloseButtonTouchBegan) onCloseButtonTouchBegan();
//                return true;
//            }
            
            // 닫기 버튼을 임시로 지워서 아래 코드로 대체
            auto P = getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (this->getBoundingBox().containsPoint(pos)) {
                if (isActive == false)
                    return false;
                
                if (onCloseButtonTouchBegan) onCloseButtonTouchBegan();
                return true;
            }
            
            return false;
        };
        
        listener->onTouchEnded = [this](Touch* T, Event* E) {
            if (onCloseButtonTouchEnded) onCloseButtonTouchEnded();
        };
        
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
    void Box::initMinusLayoutWithOrientation(bool isLandscape, bool isCompleteBox)
    {
        _isLandscape = isLandscape;
        if (isCompleteBox == true)
        {
            _box = Sprite::create(_isLandscape ? kMinusCompleteBoxImageLandscape : kMinusCompleteBoxImagePortrait);
        }
        else
        {
            _box = Sprite::create(_isLandscape ? kBoxImageLandscape : kBoxImagePortrait);
            //isTransparency(true);
        }
        this->setContentSize(_box->getContentSize());
        _box->setPosition(this->getContentSize() / 2);
        this->addChild(_box);
    }
    
    void Box::initWithOrientation(bool isLandscape)
    {
        _isLandscape = isLandscape;
        
        _box = Sprite::create(_isLandscape ? kBoxImageLandscape : kBoxImagePortrait);
        this->setContentSize(_box->getContentSize());
        _box->setPosition(this->getContentSize() / 2);
        this->addChild(_box);
        
        _cover = Sprite::create(_isLandscape ? kCoverImageLandscape : kCoverImagePortrait);
        _cover->setPosition(this->getContentSize() / 2);
        this->addChild(_cover);
        
        _sticker = Sprite::create(_isLandscape ? kStickerImageLandscape : kStickerImagePortrait);
        _sticker->setPosition(_cover->getContentSize() / 2);
        _sticker->setVisible(false);
        _cover->addChild(_sticker);
        
        _closeButton = Sprite::create(kCloseButtonImage);
        _closeButton->setPosition(120.f, this->getContentSize().height - 120.f);
        _closeButton->setVisible(false);
        _closeButton->setOpacity(0.f); //TODO: 닫기 버튼을 임시로 가려야 해서 추가한 코드
        this->addChild(_closeButton);
        
        showCover(false, false);
    }
    
    void Box::isTransparency(bool isEnable)
    {
        if (isEnable)
        {
            _box->setOpacity(255 * 0.4f);
        }
        else
        {
            _box->setOpacity(255 * 1.f);
        }
    }
    
    vector<MangoSlot*> Box::getSlots()
    {
        return _slots;
    }
    
    void Box::startParticle()
    {
        if (onStartParticle != nullptr)
        {
            onStartParticle(this->convertToWorldSpace(this->getContentSize() / 2));
        }
    }
    
    void Box::stopParticle()
    {
        if (onStopParticle != nullptr)
        {
            onStopParticle();
        }
    }
    
    bool Box::isShowCover()
    {
        return _isShowCover;
    }
    
    void Box::showCover(bool isShow, bool animate = false)
    {
        if (_cover == nullptr)
            return;
        
        _isShowCover = isShow;
        
        if (isShow)
        {
            if (animate == true)
            {
                isActive = false;
                _closeButton->setVisible(true);
                _cover->setVisible(true);
                _sticker->setScale(1.8f);
                
                _sticker->runAction(Sequence::create(DelayTime::create(0.41f), CallFunc::create([this](){
                    startParticle();
                    MangoShop::Mango::_isOtherMovingCard = false;
                } ), nullptr));
                
                _sticker->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create([this](){
                    _sticker->setVisible(true);
                    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/Mango_Close.m4a");
                }), EaseBackOut::create(ScaleTo::create(0.15f, 1.f)), CallFunc::create([this](){
                    isActive = true;
                    stopParticle();
                } ), nullptr));
            }
            else
            {
                _closeButton->setVisible(true);
                _cover->setVisible(true);
                _sticker->setVisible(true);
                //GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/Mango_Close.m4a");
            }
        }
        else
        {
            _closeButton->setVisible(false);
            _cover->setVisible(false);
            _sticker->setVisible(false);
            
            if (animate)
            {
                startParticle();
                //GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/Mango_Open.m4a");
                GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/Mango_Close.m4a");
            }
        }
    }
    
    void Box::initMangoSlots(int slotCount, bool visible)
    {
        _slots.clear();
        
        for (int i = 0; i < slotCount; i++)
        {
            auto ms = MangoSlot::create();
            ms->parentBox = this;
            ms->initWithOrientation(_isLandscape);
            float w = ms->getContentSize().width;
            float h = ms->getContentSize().height;
            ms->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            
            float x, y;
            if (_isLandscape)
            {
                x = 18.f + (w / 2) + ((i % 5) * (w - 3.f));
                y = this->getContentSize().height - 29.f - (h / 2) - ((i / 5) * h);
            }
            else
            {
                x = 19.f + (w / 2) + ((i % 2) * w);
                y = this->getContentSize().height - 21.f - (h / 2) - ((i / 2) * (h - 3.f));
            }
            
            ms->setPosition(x, y);
            ms->setVisible(visible);
            this->addChild(ms);
            _slots.push_back(ms);
        }
    }
    
    bool Box::isFull()
    {
        for (const auto &s : _slots)
        {
            if (s->targetMango == nullptr)
                return false;
        }
        return true;
    }
    
    bool Box::isEmpty()
    {
        for (const auto &s : _slots)
        {
            if (s->targetMango != nullptr)
                return false;
        }
        return true;
    }
    
    void Box::animateCloseButton(bool isClick)
    {
        if (isClick)
        {
            _closeButton->runAction(ScaleTo::create(0.1f, 1.3f));
        }
        else
        {
            _closeButton->runAction(ScaleTo::create(0.1f, 1.f));
        }
    }
    
} // end namespace
