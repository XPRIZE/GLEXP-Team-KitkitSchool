//
//  WoodPiece.cpp on Aug 11, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WoodPiece.h"
#include "Common/Components/TargetTouchBody.h"
#include "Common/Components/TargetDragBody.h"
#include "../Utils/WoodenPuzzleDepot.h"
#include "Common/Basic/ScopeGuard.h"


BEGIN_NS_WOODENPUZZLES;


namespace {
    bool showDebugLog() { return true; }
}  // unnamed namespace


WoodPiece::WoodPiece()
    : Super()
    , ShadowImage(nullptr)
{
}

bool WoodPiece::init() {
    if (!Super::init()) { return false; }
    
    return true;
}

void WoodPiece::setPosition(const Point& P) {
    Super::setPosition(P);
    
    auto Parent = getParent();
    if (Parent)
        PositionInWorldSpace.update(Parent->convertToWorldSpace(P));
}

void WoodPiece::setPosition3D(const Vec3& P) {
    // NB(xenosoz, 2016): Truncate P.z
    setPosition(Point(P.x, P.y));
}

void WoodPiece::clearInternals() {
    Super::clearInternals();
}

void WoodPiece::refreshComponents() {
    Super::refreshComponents();

    auto DragBody = TargetDragBody::create();
    DragBody->OnDragDidBegin.emplace_back([this](Touch* T, Event* E) {
        bool Hit = hitTestForShadowImage(T, E);
        if (!Hit) { return false; }
        
        auto Guard = NodeScopeGuard(this);
        if (OnTouch)
            OnTouch(T, E);
        
        return true;
    });
    
    DragBody->OnDrag.emplace_back([this](Touch* T, Event* E) {
        if (!OnDrag) { return; }

        auto Guard = NodeScopeGuard(this);
        OnDrag(T, E);
    });

    DragBody->OnDragDidEnd.emplace_back([this](Touch* T, Event* E) {
        if (!OnRelease) { return; }

        auto Guard = NodeScopeGuard(this);
        OnRelease(T, E);
    });

    addComponent(DragBody);
}

void WoodPiece::refreshChildNodes() {
    Super::refreshChildNodes();
    
    ShadowImage = ([&] {
        RefPtr<Image> It;
        It.weakAssign(new(std::nothrow) Image());

        if (!It || !It->initWithImageFile(ShadowSkin())) {
            It.reset();
            return It;
        }
        
        return It;
    }());
}

bool WoodPiece::hitTestForShadowImage(Touch* T, Event* E) {
    // NB(xenosoz, 2016): We choose shadow image for picking since it is more generous than face image.
    auto HitTestImage = ShadowImage;
    
    if (!FaceNode || !HitTestImage) { return false; }
    
    if (HitTestImage->getBitPerPixel() != 32) {
        // NB(xenosoz, 2016): We're not aware of this image type. Just pass it.
        return true;
    }
    
    // NB(xenosoz, 2016): Quick and dirty detection of alpha channel.
    auto Data = HitTestImage->getData();
    auto BytesPerPixel = 4;
    
    Point LocalP = FaceNode->convertToNodeSpace(T->getLocation());
    GLint X = LocalP.x * CC_CONTENT_SCALE_FACTOR();
    GLint Y = LocalP.y * CC_CONTENT_SCALE_FACTOR();
    ssize_t OffsetIndex = (X + (HitTestImage->getHeight() - 1 - Y) * HitTestImage->getWidth()) * BytesPerPixel;
    
    if (showDebugLog()) {
        REPR(LocalP);
        CCLOG("X: %d, Y: %d", X, Y);
        CCLOG("BufferIndex: %zu/%d", OffsetIndex, (int)HitTestImage->getDataLen());
    }
    
    if (OffsetIndex + 3 >= HitTestImage->getDataLen()) { return false; }
    
    Color4B PixelColor(Data[OffsetIndex + 0],
                       Data[OffsetIndex + 1],
                       Data[OffsetIndex + 2],
                       Data[OffsetIndex + 3]);
    
    if (showDebugLog()) {
        CCLOG("r:%d g:%d b:%d a:%d", PixelColor.r, PixelColor.g, PixelColor.b, PixelColor.a);
    }
    
    return PixelColor.a != 0;
}

END_NS_WOODENPUZZLES;
