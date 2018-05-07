//
//  CocosRepr.cpp on Apr 21, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "CocosRepr.h"
#include <cocos/cocos2d.h>
#include <sstream>

using namespace cocos2d;
using namespace std;


namespace todoschool {

template<>
string repr<Size>(const Size &S) {
    stringstream SS;
    SS << "cocos2d::Size(";
    SS << "width: " << S.width << ", ";
    SS << "height: " << S.height << ")";
    return SS.str();
}

template<>
string repr<Vec2>(const Vec2 &V) {
    stringstream SS;
    SS << "cocos2d::Vec2(";
    SS << "x: " << V.x << ", ";
    SS << "y: " << V.y << ")";
    return SS.str();
}

template<>
string repr<Vec3>(const Vec3 &V) {
    stringstream SS;
    SS << "cocos2d::Vec2(";
    SS << "x: " << V.x << ", ";
    SS << "y: " << V.y << ", ";
    SS << "z: " << V.z << ")";
    return SS.str();
}

template<>
string repr<Vec4>(const Vec4 &V) {
    stringstream SS;
    SS << "cocos2d::Vec2(";
    SS << "x: " << V.x << ", ";
    SS << "y: " << V.y << ", ";
    SS << "z: " << V.z << ", ";
    SS << "w: " << V.w << ")";
    return SS.str();
}

template<>
std::string repr<cocos2d::Rect>(const cocos2d::Rect& R) {
    stringstream SS;
    SS << "cocos2d::Rect(";
    SS << "origin: " << repr(R.origin) << ", ";
    SS << "size: " << repr(R.size) << ")";
    return SS.str();
}

template<>
string repr<cocos2d::Touch>(const cocos2d::Touch &T) {
    stringstream SS;
    SS << "cocos2d::Touch(";
    SS << "id: " << T.getID() << ", ";
    SS << "startPoint: " << repr(T.getStartLocationInView()) << ", ";
    SS << "point: " << repr(T.getLocationInView()) << ", ";
    SS << "prevPoint: " << repr(T.getPreviousLocationInView()) << ", ";
    SS << "curForce: " << T.getCurrentForce() << ", ";
    SS << "maxForce: " << T.getMaxForce() << ")";
    return SS.str();
}

template<>
string repr<cocos2d::Color3B>(const cocos2d::Color3B &C) {
    stringstream SS;
    SS << "cocos2d::Color3B(";
    SS << "r: " << (int)C.r << ", ";
    SS << "g: " << (int)C.g << ", ";
    SS << "b: " << (int)C.b << ")";
    return SS.str();
}

template<>
string repr<cocos2d::Color4B>(const cocos2d::Color4B &C) {
    stringstream SS;
    SS << "cocos2d::Color4B(";
    SS << "r: " << (int)C.r << ", ";
    SS << "g: " << (int)C.g << ", ";
    SS << "b: " << (int)C.b << ", ";
    SS << "a: " << (int)C.a << ")";
    return SS.str();
}

}  // namespace todoschool
