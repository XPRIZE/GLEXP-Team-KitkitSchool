//
//  TextObject.cpp
//  KitkitSchool
//
//  Created by timewalker on 06/12/2016.
//
//

#include "TextObject.hpp"
#include "Utils/TodoUtil.h"

namespace ComprehensionTest
{
    namespace Matching
    {
        const Size txtSize = Size(600, 300);
//        const Size txtSize = Size(510, 374);
        const string fontName = FONT_MUKTA_BOLD;
        
        TextObject::TextObject():
        _dotDirection(DotDirection::Right)
        {
            
        }

        bool TextObject::init()
        {
            if (!Node::init()) { return false; }
            
            return true;
        }

        void TextObject::setText(std::string text)
        {
            auto halign = (location == ObjectLocation::LeftSide ? TextHAlignment::RIGHT : TextHAlignment::LEFT);
            _textLabel = TodoUtil::createLabelMultilineToFit(text, 80, txtSize, fontName, Color4B(77, 77, 77, 255), halign, TextVAlignment::CENTER);
            addChild(_textLabel);
            
            setContentSize(_textLabel->getContentSize());
            
//            LayerColor* tempLayer = LayerColor::create(Color4B(100, 100, 100, 50));
//            tempLayer->setContentSize(getContentSize());
//            tempLayer->ignoreAnchorPointForPosition(false);
//            tempLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//            tempLayer->setPosition(getContentSize() / 2);
//            addChild(tempLayer);
            
            _textLabel->setAnchorPoint(location == ObjectLocation::LeftSide ? Vec2::ANCHOR_MIDDLE_RIGHT : Vec2::ANCHOR_MIDDLE_LEFT);
            _textLabel->setPosition(location == ObjectLocation::LeftSide ? getContentSize().width : 0.f, getContentSize().height / 2);
        }

        Vec2 TextObject::getDotPosition()
        {
            auto pos = convertToWorldSpace(_dot->getPosition());
            return pos;
        }

        void TextObject::drawDot(DotDirection direction)
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
            
//            LayerColor* lc1 = LayerColor::create(Color4B(100, 100, 100, 50));
//            lc1->setContentSize(_dot->getContentSize() * 3);
//            lc1->ignoreAnchorPointForPosition(false);
//            lc1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//            lc1->setPosition(_dot->getPosition());
//            addChild(lc1);
        }

        bool TextObject::isTouchedIn(Vec2 touchPoint)
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
