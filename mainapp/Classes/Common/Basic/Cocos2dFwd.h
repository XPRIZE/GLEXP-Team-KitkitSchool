//
//  Cocos2dFwd.h on Apr 20, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

// NB(xenosoz, 2016): Forward declarations for cocos2d

#ifndef TODOSCHOOL_BASIC_COCOS2DFWD_H
#define TODOSCHOOL_BASIC_COCOS2DFWD_H

namespace cocos2d {
    class Size;
    class Vec2;
    class Vec3;
    class Vec4;
    class Rect;
    class Sprite;
    class Action;
    class Touch;
    class Event;
    class Scene;
    class Node;
    class Label;
    class LayerColor;
    class Color3B;
    class Color4B;

    namespace ui {
        class Button;
    } // namespace ui

    namespace experimental {
        namespace ui {
            class VideoPlayer;
        } // end namespace ui
    } // end namespace experimental
} // end namespace cocos2d

#endif // !defined(TODOSCHOOL_BASIC_COCOS2DFWD_H)
