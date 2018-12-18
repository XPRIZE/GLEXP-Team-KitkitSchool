//
//  Piece.cpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 07/02/2018.
//

#include "ThirtyPuzzlePiece.hpp"

#include "Managers/GameSoundManager.h"

BEGIN_NS_THIRTYPUZZLE;

const string defaultFont = "fonts/TodoSchoolV2.ttf";

bool Piece::init() {
    if (!Node::init()) return false;
    _snapped = false;
    onSnapEnded = nullptr;
    onTouchEnded = nullptr;
    return true;
}

int Piece::_maxZIndex = 1000;

void Piece::setAnswerPos(Vec2 answerPos) {
    _answerPos = answerPos;
}

void Piece::setPicked(bool picked) {
    Size size = getContentSize();
    if (picked) {
        _label->setPosition(size.width/2, size.height/2+5);
        _top->setPosition(size.width/2, size.height/2+5);
        _middle->setPosition(size.width/2, size.height/2+5);
        _shadow->setPosition(size.width/2, size.height/2-5);

        _shadow->setVisible(true);
        _middle->setVisible(true);

    } else {
        _label->setPosition(size/2);
        _top->setPosition(size/2);
        _middle->setPosition(size/2);
        _shadow->setPosition(size/2);
    }
}

void Piece::reorderRelease() {
    /*
    _label->setLocalZOrder(_number*100+4);
    _top->setLocalZOrder(_number*100+3);
    _middle->setLocalZOrder(_number*100+2);
    _shadow->setLocalZOrder(_number*100+1);
     */
    this->setLocalZOrder(_number);
}

void Piece::checkInLastLine() {
    if (_inLastLine) {
        _shadow->setVisible(false);
        _middle->setVisible(false);
    }
}

void Piece::setAttribute(int number, float fontSize, bool fixed, bool inLastLine, int pieceColor) {
    CCLOG("pieceColor:%d", pieceColor);

    _touchEnabled = true;
    _number = number;
    _inLastLine = inLastLine;
    
    _shadow = Sprite::create("ThirtyPuzzle/boards_button_shadow.png");
    _shadow->setPosition(_shadow->getContentSize()/2);
    addChild(_shadow);
    
    _middle = Sprite::create("ThirtyPuzzle/boards_button_middle.png");
    _middle->setPosition(_middle->getContentSize()/2);
    addChild(_middle);
    
    if (fixed) {
        _top = Sprite::create("ThirtyPuzzle/boards_button_top_00.png");
        _snapped = true;
    } else {
        char filename[256];
        sprintf(filename, "ThirtyPuzzle/boards_button_top_%02d.png", pieceColor);
        _top = Sprite::create(filename);
    }
    _top->setPosition(_top->getContentSize()/2);
    addChild(_top);
    
    string numberString = TodoUtil::itos(number);
    _label = TodoUtil::createLabel(numberString, fontSize, Size::ZERO, defaultFont, fixed ? Color4B(247, 212, 141, 191) : Color4B(237, 237, 211, 255), TextHAlignment::CENTER);
    _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _label->setPosition(_top->getPosition());
    _top->addChild(_label);
    reorderRelease();
    
    Size size = _top->getContentSize();
    setContentSize(size);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    if (fixed) checkInLastLine();
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        
        if (_snapped) return false;
        
        if (this->getBoundingBox().containsPoint(pos)) {
            this->setLocalZOrder(_maxZIndex++);
            //this->getParent()->reorderChild(this, this->getLocalZOrder());
            this->setPicked(true);
            GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_Wood_SlideOut.m4a");
            
            // if (onTouchBegan) onTouchBegan();
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
    };

    listener->onTouchEnded = [this](Touch* T, Event* E) {
        if (_snapped) return;
        setPicked(false);
        
        //CCLOG("answerX:%f, answerY:%f", _answerPos.x, _answerPos.y);
        //CCLOG("positionX:%f, positionY:%f", getPositionX(), getPositionY());

        if (_answerPos.distance(getPosition())<120.f) {
            _snapped = true;
            if (onSnapEnded) onSnapEnded();

            this->runAction(Sequence::create(
                MoveTo::create(0.1, Vec2(_answerPos.x, _answerPos.y+20)),
                DelayTime::create(0.1),
                CallFunc::create([this](){
                    this->reorderRelease();
                    this->checkInLastLine();
                    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/SFX_Wood_Correct.m4a");
                }),
                MoveTo::create(0.1, _answerPos),
                nullptr
            ));
        } else {
            if (this->getPositionX() < 120) this->runAction(EaseIn::create(MoveBy::create(0.3, Vec2(120, 0)), 3.0));
            if (this->getPositionX() > this->getParent()->getContentSize().width-120) this->runAction(EaseIn::create(MoveBy::create(0.3, Vec2(-120, 0)), 3.0));
            if (this->getPositionY() < 120) this->runAction(EaseIn::create(MoveBy::create(0.3, Vec2(0, 120)), 3.0));
            if (this->getPositionY() > this->getParent()->getContentSize().height-300) this->runAction(EaseIn::create(MoveBy::create(0.3, Vec2(0, -300)), 3.0));
        }

        if (onTouchEnded) onTouchEnded();
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    //auto rectNode = DrawNode::create(); rectNode->drawRect(Vec2(this->getBoundingBox().getMinX(), this->getBoundingBox().getMinY()), Vec2(this->getBoundingBox().getMaxX(), this->getBoundingBox().getMaxY()), Color4F::GREEN); addChild(rectNode);

}

END_NS_THIRTYPUZZLE;

