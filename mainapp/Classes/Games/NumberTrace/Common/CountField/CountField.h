//
//  CountField.h on Jun 21, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef TODOSCHOOL_COUNTFIELD_COUNTFIELD_H
#define TODOSCHOOL_COUNTFIELD_COUNTFIELD_H

#include "ChildNodes/CountObject.h"
#include <Common/ADT/Chain.h>
#include <cocos/cocos2d.h>
#include <functional>


namespace todoschool {
namespace countfield {

class CountField: public cocos2d::Node {

    bool ShouldUpdateByValueChange;
    
public:
    CREATE_FUNC(CountField);
    
    cocos2d::Vector<CountObject*> CountObjects;
    std::string TheAssetType;
    Chain<size_t> AssetCount;
    int AssetCountInRestPoint;
    int AssetCountInEndPoint;

    // NB(xenosoz, 2016): The control responds to user event only when it's enabled.
    Chain<bool> Enabled;

    std::function<void(CountField*)> OnGoodAssetClicked;
    std::function<void(CountField*)> OnBadAssetClicked;
    std::function<void(CountField*)> OnCountWorkDidBecomeReady;
    std::function<void(CountField*)> OnCountWorkDidEnd;

public:
    bool init() override;
    void clear();

private:
    void clearInternals();
    void updateCountObjects();
    void updateEnabledValueForAllCountObjects(bool Value);

    void handleEnabledValueUpdated(bool& Value);
    void handleAssetCountValueUpdated(size_t&);

    void handleCountObjectReachedRestPoint(CountObject*);
    void handleCountObjectReachedEndPoint(CountObject*);
};

}  // namespace countfield
}  // namespace todoschool

#endif  // !defined(TODOSCHOOL_COUNTFIELD_COUNTFIELD_H)
