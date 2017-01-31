//
//  TextObject.hpp
//  enumaXprize
//
//  Created by timewalker on 06/12/2016.
//
//

#ifndef TextObject_hpp
#define TextObject_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "MatchingScene.hpp"

USING_NS_CC;

namespace ComprehensionTest
{
namespace Matching
{

class TextObject : public Node
{
public:
    bool init() override;
    TextObject();
    CREATE_FUNC(TextObject);

    std::string id;
    ObjectLocation location;
    bool isClosed = false;
    
    Vec2 getDotPosition();
    void setText(std::string text);
    void drawDot(DotDirection direction);
    bool isTouchedIn(Vec2 touchPoint);
    
private:
    DotDirection _dotDirection;
    Label* _textLabel;
    DrawNode* _dot;
};

}  // namespace Matching
}  // namespace ComprehensionTest

#endif /* TextObject_hpp */
