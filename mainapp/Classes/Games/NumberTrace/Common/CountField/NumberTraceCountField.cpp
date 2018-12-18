//
//  CountField.cpp on Jun 21, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "NumberTraceCountField.h"
#include "Utils/CountFieldDepot.h"
#include "Common/Basic/DeviceSpec.h"
#include "Common/Basic/ScopeGuard.h"
#include "Common/Repr/AllRepr.h"
#include <cocos/cocos2d.h>

using namespace cocos2d;
using namespace std;

namespace todoschool {
namespace countfield {

bool CountField::init() {
    if (!Node::init()) { return false; }
    
    clear();
    return true;
}

void CountField::clear() {
    ShouldUpdateByValueChange = false;
    
    clearInternals();
    updateCountObjects();
    
    ShouldUpdateByValueChange = true;  // XXX: Maybe we need RAII guard for this.
}

void CountField::clearInternals() {
    CountFieldDepot Depot;
    Size WindowSize = Depot.windowSize();
    Size GameSize = Depot.gameSize();
    
    float ButtonHeightInGame = 169.f * GameSize.height / WindowSize.height;  // XXX
    Size VS = DeviceSpec::Google_Pixel_C().FullScreenResolution;
    
    setContentSize(Size(VS.width/2.f, VS.height - ButtonHeightInGame));
    AssetCountInRestPoint = 0;
    AssetCountInEndPoint = 0;
    
    using namespace std::placeholders;
    Enabled.OnValueUpdate = bind(&CountField::handleEnabledValueUpdated, this, _1);
    Enabled.update(false);
    AssetCount.OnValueUpdate = bind(&CountField::handleAssetCountValueUpdated, this, _1);
    AssetCount.update(0);
}

void CountField::updateCountObjects() {
    CountFieldDepot Depot;

    float W = getContentSize().width;
    float H = getContentSize().height;

    for (auto& CountObject : CountObjects)
        CountObject->removeFromParent();
    CountObjects.clear();
    
    if (AssetCount() <= 0) { return; }
    
    float BeginRotation = M_PI;
    float RestRotation = M_PI/3.f*2.f;
    float EndRotation = M_PI;
    float RotationNoise = MATH_DEG_TO_RAD(35.f);
    
    Poses BeginPoses;
    Poses RestPoses;
    Poses EndPoses;
    
    float Scale = 1.f;
    int TrialCount = 0;
    while (BeginPoses.empty() || RestPoses.empty() || EndPoses.empty()) {
        Scale = 1.f / (int(TrialCount / 3.f) + 1.f);
        Size AssetSize = Depot.sizeForCountObjectWithType(TheAssetType) * Scale;
        
        BeginPoses = Depot.makePosesWithMultipleTrial(
                                    AssetCount(), AssetSize,
                                    Point(W, -H), getContentSize(),
                                    BeginRotation - RotationNoise,
                                    BeginRotation + RotationNoise);
        RestPoses = Depot.makePosesWithMultipleTrial(
                                    AssetCount(), AssetSize,
                                    Point::ZERO, getContentSize(),
                                    RestRotation - RotationNoise,
                                    RestRotation + RotationNoise);
        EndPoses = Depot.makePosesWithMultipleTrial(
                                    AssetCount(), AssetSize,
                                    Point(-W, H), getContentSize(),
                                    EndRotation - RotationNoise,
                                    EndRotation + RotationNoise);
        TrialCount += 1;
    }

    for (size_t I = 0, E = AssetCount(); I < E; ++I) {
        CountObject* It = Depot.createCountObjectWithType(TheAssetType);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(getContentSize() / 2.f);
        It->Scale.update(Scale);
        
        const float MoveInDuration = 2.f + random(-4, 0) * 0.25f;
        const float FadeInDuration = .5f;
        
        It->jumpTo(BeginPoses[I]);
        It->moveIn(RestPoses[I], MoveInDuration, FadeInDuration);
        It->Enabled.update(Enabled());

        Pose EndPose = EndPoses[I];
        
        using std::placeholders::_1;
        It->OnTouchDidBegin = [this, It](CountObject*) {
            updateEnabledValueForAllCountObjects(false);
        };
        It->OnTouchDidEnd =
        It->OnTouchDidCancel = [this, It](CountObject*) {
            updateEnabledValueForAllCountObjects(Enabled());
        };

        It->OnTouchUpInside = [this, It, EndPose](CountObject*) {
            // NB(xenosoz, 2016): Guide the bug to the end position.
            if (It->Moving()) {
                return;
            }

            const float MoveOutDuration = 2.f;
            const float FadeOutDuration = .5f;

            It->OnMovingDidEnd = bind(&CountField::handleCountObjectReachedEndPoint, this, _1);
            It->moveOut(EndPose, MoveOutDuration, FadeOutDuration);

            if (OnGoodAssetClicked)
                OnGoodAssetClicked(this);

            // XXX: Changing labmda variable inside itself is not a good idea.
            It->OnTouchUpInside = [this](CountObject*){
                if (OnBadAssetClicked)
                    OnBadAssetClicked(this);
            };
        };
        It->OnMovingDidEnd = bind(&CountField::handleCountObjectReachedRestPoint, this, _1);


        addChild(It);
        CountObjects.pushBack(It);
    };
}
    
void CountField::updateEnabledValueForAllCountObjects(bool Value) {
    for (auto& CountObject : CountObjects) {
        CountObject->Enabled.update(Value);
    }
}

void CountField::handleEnabledValueUpdated(bool& Value) {
    updateEnabledValueForAllCountObjects(Value);
}

void CountField::handleAssetCountValueUpdated(size_t &) {
    if (!ShouldUpdateByValueChange) { return; }

    updateCountObjects();
}

void CountField::handleCountObjectReachedRestPoint(CountObject*) {
    updateEnabledValueForAllCountObjects(Enabled());

    AssetCountInRestPoint += 1;
    
    auto Guard = ScopeGuard([this]{ retain(); },
                            [this]{ release(); });
    
    if (OnCountWorkDidBecomeReady && AssetCountInRestPoint == AssetCount())
        OnCountWorkDidBecomeReady(this);
}

void CountField::handleCountObjectReachedEndPoint(CountObject*) {
    updateEnabledValueForAllCountObjects(Enabled());

    AssetCountInEndPoint += 1;

    auto Guard = ScopeGuard([this]{ retain(); },
                            [this]{ release(); });

    if (OnCountWorkDidEnd && AssetCountInEndPoint == AssetCount())
        OnCountWorkDidEnd(this);
}

}  // namespace countfield
}  // namespace todoschool
