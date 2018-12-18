//
//  JsonUtils.h on Jun 17, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef TODOSCHOOL_UTILS_JSONUTILS_H
#define TODOSCHOOL_UTILS_JSONUTILS_H


#include <string>

namespace Json {
    class Value;
}

namespace todoschool {

bool jsonFromFile(const std::string& Filename, Json::Value& RetValue);

}  // namespace todoschool

#endif // !defined(TODOSCHOOL_UTILS_JSONUTILS_H)
