//
//  StringUtils.cpp on May 25, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "JsonUtils.h"
#include <cocos/cocos2d.h>
#include <3rdParty/json/json.h>

using namespace cocos2d;
using namespace std;

namespace todoschool {

bool jsonFromFile(const string& Filename, Json::Value& RetValue) {
    RetValue = Json::Value();

    // XXX: What if getStringFromFile fails?
    string JsonStr = FileUtils::getInstance()->getStringFromFile(Filename);

    Json::Reader JsonReader;
    if (!JsonReader.parse(JsonStr, RetValue)) {
        CCLOGERROR("JSON reader parse error: %s", Filename.c_str());
        return false;
    }

    return true;
}

}  // namespace todoschool
