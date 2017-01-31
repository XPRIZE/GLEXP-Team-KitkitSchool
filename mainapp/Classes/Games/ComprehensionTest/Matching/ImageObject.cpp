//
//  ImageObject.cpp
//  enumaXprize
//
//  Created by timewalker on 06/12/2016.
//
//

#include "ImageObject.hpp"

#include "3rdParty/CCNativeAlert.h"

namespace ComprehensionTest
{
    namespace Matching
    {
        const Size imageFrameSize = Size(510, 374);
        const Size imageSize = Size(510, 364);
        
        ImageObject::ImageObject()
        :_dotDirection(DotDirection::Right)
        {
            
        }

        bool ImageObject::init()
        {
            if (!Node::init()) { return false; }
            
            setContentSize(imageFrameSize);
            
            _backgroundImage = Sprite::create("ComprehensionTest/Matching/illustration_small.png");
            _backgroundImage->setPosition(getContentSize() / 2);
            addChild(_backgroundImage);
            
            return true;
        }

        void ImageObject::setImage(std::string imagePath)
        {
            
            CCLOG("%s", imagePath.c_str());
            _image = Sprite::create("ComprehensionTest/Image/"+imagePath);
            
            if (_image == nullptr) { NativeAlert::show("Image does not exist.", "ComprehensionTest/Image/" + imagePath, "OK"); return; }
            
            float scaleFactor = MIN(imageSize.width / _image->getContentSize().width, imageSize.height / _image->getContentSize().height);
            _image->setScale(scaleFactor);
            
            if (!_image) {
                NativeAlert::show("Image not found in Comp-Matching", imagePath + "does not exist", "OK");
                return;
            }
         
            _image->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            _image->setPosition(getContentSize().width / 2, getContentSize().height);
            addChild(_image);
        }
    
        Vec2 ImageObject::getDotPosition()
        {
            auto pos = convertToWorldSpace(_dot->getPosition());
            return pos;
        }

        void ImageObject::drawDot(DotDirection direction)
        {
            _dot = DrawNode::create();
            _dot->setContentSize(Size(150, 150));
            if (direction == DotDirection::Left)
            {
                _dot->setPosition(-70.f, getContentSize().height / 2);
                _dot->drawSolidCircle(Vec2::ZERO, 15, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F(Color4B(77, 77, 77, 255)));
            }
            else
            {
                _dot->setPosition(getContentSize().width + 70.f, getContentSize().height / 2);
                _dot->drawSolidCircle(Vec2::ZERO, 15, CC_DEGREES_TO_RADIANS(90), 50, 1.0f, 1.0f, Color4F(Color4B(77, 77, 77, 255)));
            }
            addChild(_dot);
        }

        bool ImageObject::isTouchedIn(Vec2 touchPoint)
        {
            auto P = getParent();
            
            auto box1 = getBoundingBox();
            auto box2 = _dot->getBoundingBox();
//            box2.size = box2.size * 3;
            box2.origin = box1.origin + _dot->getPosition() - box2.size / 2;
            
            auto pos = P->convertToNodeSpace(touchPoint);
            return box1.containsPoint(pos) || box2.containsPoint(pos);
        }
    
    }  // namespace Matching
    
}  // namespace ComprehensionTest

