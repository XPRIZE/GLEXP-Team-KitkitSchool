//
//  ImageObject.hpp
//  enumaXprize
//
//  Created by timewalker on 06/12/2016.
//
//

#ifndef ImageObject_hpp
#define ImageObject_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "MatchingScene.hpp"

USING_NS_CC;

namespace ComprehensionTest
{
namespace Matching
{

class ImageObject : public Node
{
public:
    bool init() override;
    ImageObject();
    CREATE_FUNC(ImageObject);
    
    std::string id;
    ObjectLocation location;
    bool isClosed = false;
    
    void setImage(std::string imagePath);
    Vec2 getDotPosition();
    void drawDot(DotDirection direction);
    bool isTouchedIn(Vec2 touchPoint);
    
private:
    DotDirection _dotDirection;
    Sprite* _image;
    Sprite* _backgroundImage;
    DrawNode* _dot;
};

}  // namespace Matching
}  // namespace ComprehensionTest

#endif /* ImageObject_hpp */
