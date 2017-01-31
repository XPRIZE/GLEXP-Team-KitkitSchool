//
//  TraceKnotPoint.cpp on Apr 22, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//
//  Based on the great work from: todomathandroid_kh (Locomotive Labs)
//

#include "TraceKnotPoint.h"


BEGIN_NS_TRACEFIELD

TraceKnotPoint::TraceKnotPoint(float X_, float Y_, TraceKnotPointType Type_)
    : X(X_), Y(Y_), Type(Type_) {}

TraceKnotPoint::TraceKnotPoint(Json::Value& P)
{
    X = P["x"].asFloat();
    Y = P["y"].asFloat();

    string TypeString = P["type"].asString();
    Type = TraceKnotPoint::createTypeWithString(TypeString);
}

TraceKnotPoint::TraceKnotPointType TraceKnotPoint::createTypeWithString(const string& TypeString)
{
    if (TypeString == "Point")
        return TraceKnotPointType::Point;

    if (TypeString == "Separator")
        return TraceKnotPointType::Separator;

    // NB(xenosoz, 2016): We didn't implement LetterStop and WordStop yet in 2016.

    //if (TypeString == "LetterStop")
    //    return TraceKnotPointType::LetterStop;

    //if (TypeString == "WordStop")
    //    return TraceKnotPointType::WordStop;

    throw std::domain_error("Not Implemented: TraceKnotPoint::Type: " + TypeString);
}

float TraceKnotPoint::x() const {
    return X;
}

float TraceKnotPoint::y() const {
    return Y;
}

cocos2d::Point TraceKnotPoint::point() const {
    return cocos2d::Point(x(), y());
}

TraceKnotPoint::TraceKnotPointType TraceKnotPoint::type() const {
    return Type;
}

END_NS_TRACEFIELD
