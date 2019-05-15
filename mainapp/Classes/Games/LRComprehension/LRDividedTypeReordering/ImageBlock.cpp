//
//  ImageBlock.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 20/12/2016.
//
//

#include "ImageBlock.hpp"
#include "Common/Controls/DottedRect.hpp"

namespace
{
    Size kImageSize  = Size(460.f, 330.f);
}

BEGIN_NS_LR_DIVIDED_TYPE_REORDERING;

ImageBlock::ImageBlock():
_bSlot(false)
{ }

Size ImageBlock::wordBlockSize = Size(479, 345); // Size(532, 383);

bool ImageBlock::init()
{
    if (!Node::init())
        return false;
    
    setContentSize(wordBlockSize);
    _snapped = false;
    _pair = nullptr;
    
    return true;
}

void ImageBlock::initAsSlot(std::string imageFile)
{
    CCLOG("initAsSlot");
    _bSlot = true;
    _solutionImageFile = imageFile;
    
    auto face = Node::create();
    face->setContentSize(wordBlockSize);
    face->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _face = face;

    addChild(_face);
    
    _face->addChild(DottedRect::create(wordBlockSize));
}

void ImageBlock::initAsBlock(std::string imageFile)
{
    _bSlot = false;
    _imageFile = imageFile;
    
//    _faceOriginPos = Point(getContentSize().width / 2, getContentSize().height - 2);
    _faceOriginPos = Vec2::ZERO;
    
    _shadow = Sprite::create("lrcomprehension/common/comprehensive_shadow.png");
    _shadow->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    addChild(_shadow);
    
    _face = Sprite::create("lrcomprehension/common/comprehensive_picture.png");
    _face->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    addChild(_face);

    auto image = Sprite::create("lrcomprehension/images/" + imageFile);
    if (image == nullptr) { NativeAlert::show("Image does not exist.", imageFile, "OK"); return; }
    
    image->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    float scaleFactor = MIN(kImageSize.width / image->getContentSize().width, kImageSize.height / image->getContentSize().height);
    image->setScale(scaleFactor);
    image->setPosition(_face->getContentSize().width / 2, _face->getContentSize().height);
    _face->addChild(image);
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        // Touch의 Location은 world좌표계이고, getBoundingBox는 부모 좌표계이기 때문에 둘을 맞춰줄 필요가 있습니다.
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        if (this->getBoundingBox().containsPoint(pos))
        {    
            this->setPicked(true);
            
            // 원위치로 되돌릴 일이 있을 때를 위하여 원래 위치를 저장해둡니다.
            this->_originPos = getPosition();
            
            // 선택된 객체를 위로 올려줍니다.
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
        // getDelta()로는 world좌표계상에서 touch의 차이를 받아오게 되기 때문에, 부모좌표계상에서의 차이를 알기 위해
        // previousLocation과 Location을 받아서 각각 변환을 해준 다음에 차이를 구합니다.
        auto P = getParent();
        auto pl = P->convertToNodeSpace(T->getPreviousLocation());
        auto cl = P->convertToNodeSpace(T->getLocation());
        auto delta = cl-pl;
        this->setPosition(this->getPosition()+delta);
    };
    
    listener->onTouchEnded = [this](Touch* T, Event* E) {
        // 놓았을 때 빈칸에 들어갔는지 확인하기 위한 callback입니다.
        setPicked(false);
        
        if (onCheckTarget)
        {
            onCheckTarget();
        }
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void ImageBlock::setPicked(bool picked)
{
    if (picked)
    {
        // snap된 놈을 들었을 때 다시 그림자가 나타나게 합니다.
        _shadow->setVisible(true);
        _face->setPosition(Point(0, 5.f));
        _shadow->setPosition(5.f, -5.f);
    }
    else
    {
        // snap된 놈을 놓았을 떄 그림자를 다시 숨깁니다.
        if (_snapped) _shadow->setVisible(false);
        _face->setPosition(_faceOriginPos);
        _shadow->setPosition(Vec2::ZERO);
    }
}

void ImageBlock::setSnapped(bool snapped)
{
    _snapped = snapped;
    _shadow->setVisible(!snapped);
}

void ImageBlock::returnToOrigin()
{
    setPosition(_originPos);
}

std::string ImageBlock::getValue()
{
    if (_bSlot) return "";
    return _imageFile;
}

std::string ImageBlock::getSolutionValue()
{
    if (!_bSlot) return "";
    return _solutionImageFile;
}

END_NS_LR_DIVIDED_TYPE_REORDERING;
