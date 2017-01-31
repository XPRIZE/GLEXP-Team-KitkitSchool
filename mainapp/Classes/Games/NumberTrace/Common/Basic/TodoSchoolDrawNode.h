//
//  TodoSchoolDrawNode.h on Jul 3, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef TODOSCHOOL_BASIC_TODOSCHOOLDRAWNODE_H
#define TODOSCHOOL_BASIC_TODOSCHOOLDRAWNODE_H

#include <cocos/cocos2d.h>

namespace todoschool {

class TodoSchoolDrawNode: public cocos2d::DrawNode {
    typedef cocos2d::DrawNode Super;

public:
    CREATE_FUNC(TodoSchoolDrawNode);

    // NB(xenosoz, 2016): Support for multiple line segments.
    void drawSegments(const cocos2d::Vec2 *verts, int count,
                      float thickness, const cocos2d::Color4B& color);
};
    
}  // namespace todoschool

#endif  // !defined(TODOSCHOOL_BASIC_TODOSCHOOLDRAWNODE_H)
